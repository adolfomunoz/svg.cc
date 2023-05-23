#include "../scatter.h"
//We have to include everything, otherwise the svg in marker might not load propperly
#include "../../svg.cc.h"
#include "color.h"
#include <cmath>
#include <limits>

namespace svg {
namespace plot {

Scatter::Scatter(const std::vector<float>& x, const std::vector<float>& y) : x(x),y(y) {}
Scatter::Scatter(std::vector<float>&& x, const std::vector<float>& y) : x(std::move(x)), y(y) {}
Scatter::Scatter(const std::vector<float>& x, std::vector<float>&& y) : x(x), y(std::move(y)) {}
Scatter::Scatter(std::vector<float>&& x, std::vector<float>&& y) : x(std::move(x)), y(std::move(y)) {}

svg::Element Scatter::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Group output;
    auto& defs = output.add(svg::Defs());
    defs.add(marker_);
    auto& points = output.add(svg::Group());
    for (std::size_t i = 0; (i<x.size()) && (i<y.size()); ++i) {
        points.add(svg::Use(marker_)).
            add_transform(svg::Scale(markersize(i))).add_transform(svg::Translate(xscale(x[i]),yscale(y[i]))).
            fill(color(i)).stroke_width(linewidth(i)/markersize(i)).stroke(edgecolor(i)).  //We divide stroke_width by markersize in order to undo the effect of scaling
            opacity(alpha(i)).fill_opacity(alpha(i));
    }
    return output;
}
std::array<float,4> Scatter::axis() const noexcept {
    if (x.empty() || y.empty()) return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
    std::array<float,4> a{x.front(),x.front(),y.front(),y.front()};
    std::vector<float>::const_iterator ix, iy;
    for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy) {
        if (*ix < a[0]) a[0] = *ix;
        if (*ix > a[1]) a[1] = *ix;
        if (*iy < a[2]) a[2] = *iy;
        if (*iy > a[3]) a[3] = *iy;
    }

    //Arbitrary expansion, maybe not a good idea but it is worse to account for markersize which is in a different space
    float dx = std::abs(a[1]-a[0])/32.0f; //+ max_size/std::abs(ax[1]-ax[0]);
    float dy = std::abs(a[3]-a[2])/32.0f; //+ max_size/std::abs(ax[3]-ax[2]);
    a[0]-=dx; a[1]+=dx; a[2]-=dy; a[3]+=dy;
    return a;
}

Scatter& Scatter::linewidths(std::vector<float>&& vf) noexcept {
    this->linewidth_ = std::move(vf);
    return (*this);
}
Scatter& Scatter::linewidths(const std::vector<float>& vf) noexcept {
    this->linewidth_ = vf;
    return (*this);
}
Scatter& Scatter::linewidths(float f) noexcept {
    return linewidths(std::vector<float>(1,f));
}
Scatter& Scatter::alpha(std::vector<float>&& vf) noexcept {
    this->alpha_ = std::move(vf);
    return (*this);
}
Scatter& Scatter::alpha(const std::vector<float>& vf) noexcept {
    this->alpha_ = vf;
    return (*this);
}
Scatter& Scatter::alpha(float f) noexcept {
    return alpha(std::vector<float>(1,f));
}
Scatter& Scatter::c(std::vector<svg::Color>&& sc) noexcept {
    value_.clear();
    this->color_ = std::move(sc);
    return (*this);
}
Scatter& Scatter::c(const std::vector<svg::Color>& sc) noexcept {
    value_.clear();
    this->color_ = sc; return (*this);
}
Scatter& Scatter::c(const std::vector<std::string>& sc) noexcept {
    std::vector<svg::Color> cs(sc.size(),svg::black);
    std::transform(sc.begin(),sc.end(),cs.begin(),color_from_string);
    return c(std::move(cs));
}
Scatter& Scatter::c(const std::vector<const char*>& sc) noexcept {
    std::vector<svg::Color> cs(sc.size(),svg::black);
    std::transform(sc.begin(),sc.end(),cs.begin(),[] (const char* cc) { return color_from_string(std::string(cc)); });
    return c(std::move(cs));
}
Scatter& Scatter::c(const svg::Color& sc) noexcept {
    return c(std::vector<svg::Color>(1,sc));
}
Scatter& Scatter::c(const std::string& sc) noexcept {
    return c(color_from_string(sc));
}
Scatter& Scatter::c(const char* sc) noexcept {
    return c(std::string(sc));
}
Scatter& Scatter::c(std::vector<float>&& v) noexcept {
    this->value_=std::move(v); return (*this);
}
Scatter& Scatter::c(const std::vector<float>& v) noexcept {
    this->value_=v; return (*this);
}

Scatter& Scatter::s(std::vector<float>&& vf) noexcept {
    this->markersize_ = std::move(vf);
    return (*this);
}
Scatter& Scatter::s(const std::vector<float>& vf) noexcept {
    this->markersize_ = vf;
    return (*this);
}
Scatter& Scatter::s(float f) noexcept {
    return s(std::vector<float>(1,f));
}
Scatter& Scatter::edgecolors(std::vector<svg::Color>&& c) noexcept {
    this->edgecolor_ = std::move(c);
    return (*this);
}
Scatter& Scatter::edgecolors(const std::vector<svg::Color>& c) noexcept {
    this->edgecolor_ = c; return (*this);
}
Scatter& Scatter::edgecolors(const std::vector<std::string>& sc) noexcept {
    std::vector<svg::Color> cs(sc.size(),svg::black);
    std::transform(sc.begin(),sc.end(),cs.begin(),color_from_string);
    return edgecolors(std::move(cs));
}
Scatter& Scatter::edgecolors(const std::vector<const char*>& sc) noexcept {
    std::vector<svg::Color> cs(sc.size(),svg::black);
    std::transform(sc.begin(),sc.end(),cs.begin(),[] (const char* cc) { return color_from_string(std::string(cc)); });
    return edgecolors(std::move(cs));
}
Scatter& Scatter::edgecolors(const svg::Color& c) noexcept {
    return edgecolors(std::vector<svg::Color>(1,c));
    return (*this);
}
Scatter& Scatter::edgecolors(const std::string& c) noexcept {
    return edgecolors(color_from_string(c));
}
Scatter& Scatter::edgecolors(const char* c) noexcept {
    return edgecolors(std::string(c));
}
Scatter& Scatter::marker(const svg::Element& m) noexcept {
    this->marker_ = m;
    return (*this);
}

namespace {
inline svg::Polygon plus(float s, float w) {
    const float hs = 0.5f*s;
    const float hw = 0.5f*w;
    return svg::Polygon({{hw,hw},{hs,hw},{hs,-hw},{hw,-hw},{hw,-hs},{-hw,-hs},{-hw,-hw},{-hs,-hw},{-hs,hw},{-hw,hw},{-hw,hs},{hw,hs}});
}
inline svg::Polygon times(float s, float w) {
    return plus(s*std::sqrt(2.0f),w).add_transform(svg::Rotate(45));
}
}

Scatter& Scatter::marker(const std::string& f) noexcept {
		if (f.size()>4) {
            if (f.substr(f.size()-4)==".svg") {
                svg::SVG marker_from_file; marker_from_file.load_from_file(f);
                if (!marker_from_file.children().empty()){
                    marker_from_file.x(-1).y(-1).width(2).height(2).preserveAspectRatio(svg::par::xMidYMid,svg::par::meet);
                    return marker(marker_from_file);
                } else return marker(svg::Circle(0,0,1)); //By default, a circle
            } else if ((f.substr(f.size()-4)==".jpg") || (f.substr(f.size()-4)==".png")) {
                svg::Image image;
                image.embed(f);
                image.x(-1).y(-1).width(2).height(2).preserveAspectRatio(svg::par::xMidYMid,svg::par::meet);
                return marker(image);
            } else return marker(svg::Circle(0,0,1)); //By default, a circle
        }
        else if (f == "o") return marker(svg::Circle(0,0,1));
        else if (f == ".") return marker(svg::Circle(0,0,0.5));
		else if (f == ",") return marker(svg::Circle(0,0,0.23));
		else if (f == "v") return marker(svg::Polygon({{0,1},{1,-1},{-1,-1}}));
		else if (f == ">") return marker(svg::Polygon({{1,0},{-1,1},{-1,-1}}));
		else if (f == "^") return marker(svg::Polygon({{0,-1},{1,1},{-1,1}}));
		else if (f == "<") return marker(svg::Polygon({{-1,0},{1,1},{1,-1}}));
		else if (f == "s") return marker(svg::Rect(-1,-1,1,1));
        else if (f == "+") return marker(plus(2,0.3));
 		else if (f == "P") return marker(plus(2,0.7)); 
		else if (f == "x") return marker(times(2,0.3));
		else if (f == "X") return marker(times(1.5,0.7));
		else return marker(svg::Circle(0,0,1)); //By default, a circle
}

Scatter& Scatter::marker(const char* f) noexcept {
    return marker(std::string(f));
}

svg::Color Scatter::color(std::size_t i) const noexcept {
    if (!value_.empty()) return cmap_(value_[i%value_.size()],vmin(),vmax());
    else return color_[i%color_.size()];
}
svg::Color Scatter::edgecolor(std::size_t i) const noexcept {
    return edgecolor_[i%color_.size()];
}

float Scatter::alpha(std::size_t i) const noexcept {
    return alpha_[i%alpha_.size()];
}
float Scatter::linewidth(std::size_t i) const noexcept {
    return linewidth_[i%linewidth_.size()];
}
float Scatter::markersize(std::size_t i) const noexcept {
    return markersize_[i%markersize_.size()];
}

Scatter& Scatter::vmin(float v) noexcept {
    vmin_=v; return (*this);
}
Scatter& Scatter::vmax(float v) noexcept {
    vmax_=v; return (*this);
}
Scatter& Scatter::cmap(const ColorMap& c) noexcept {
    cmap_=c; return (*this);
}
Scatter& Scatter::cmap(const std::string& c) noexcept {
    return cmap(ColorMap::from_name(c));
}
Scatter& Scatter::cmap(const char* c) noexcept {
    return cmap(std::string(c));
}

float Scatter::vmin() const noexcept {
    if (vmin_) return *vmin_;
    else {
        float r = std::numeric_limits<float>::max();
        for (float v : value_) if (v<r) r=v;
        return r;
    }
}
float Scatter::vmax() const noexcept {
    if (vmax_) return *vmax_;
    else {
        float r = std::numeric_limits<float>::lowest();
        for (float v : value_) if (v>r) r=v;
        return r;
    }
}

}
}
