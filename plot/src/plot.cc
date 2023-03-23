#include "../plot.h"
#include "../../src/poly.h"
#include "color.h"

namespace svg {
namespace plot {

std::vector<float> list_to_vector(const std::list<float>& l) {
    std::vector<float> v;
    for (float f : l) v.push_back(f);
    return v;
}

Plot::Plot(const std::list<float>& x, const std::list<float>& y) : x(x),y(y),markers(list_to_vector(x),list_to_vector(y)) {}
Plot::Plot(std::list<float>&& _x, const std::list<float>& y) : x(std::move(_x)), y(y),markers(list_to_vector(x),list_to_vector(y)) {}
Plot::Plot(const std::list<float>& x, std::list<float>&& _y) : x(x), y(std::move(_y)),markers(list_to_vector(x),list_to_vector(y)) {}
Plot::Plot(std::list<float>&& _x, std::list<float>&& _y) : x(std::move(_x)), y(std::move(_y)),markers(list_to_vector(x),list_to_vector(y)) {}
svg::Element Plot::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Polyline output;
    std::list<float>::const_iterator ix, iy;
    for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy)
        output.add_point(xscale(*ix),yscale(*iy));
    output.fill(none).stroke_width(linewidth()).stroke(color()).opacity(alpha());

    std::list<float> ls = linestyle();
    if (!ls.empty()) {
        std::transform(ls.begin(),ls.end(),ls.begin(),[lw = linewidth()] (float f) { return lw*f; });
        output.stroke_dasharray(ls);
    }

    return output;
}
std::array<float,4> Plot::axis() const noexcept {
    if (x.empty() || y.empty()) return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
    std::array<float,4> a{x.front(),x.front(),y.front(),y.front()};
    std::list<float>::const_iterator ix, iy;
    for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy) {
        if (*ix < a[0]) a[0] = *ix;
        if (*ix > a[1]) a[1] = *ix;
        if (*iy < a[2]) a[2] = *iy;
        if (*iy > a[3]) a[3] = *iy;
    }
    return a;
}

Plot& Plot::linewidth(float f) noexcept { linewidth_=f; return *this; }
float Plot::linewidth() const noexcept { return linewidth_; }
Plot& Plot::alpha(float f) noexcept { alpha_=f; return *this; }
float Plot::alpha() const noexcept { return alpha_; }
Plot& Plot::color(const svg::Color& c) noexcept { color_=c; return *this; }
Plot& Plot::color(const std::string& sc) noexcept {
    return color(color_from_string(sc));
}
Plot& Plot::color(const char* sc) noexcept {
    return color(std::string(sc));
}
const svg::Color& Plot::color() const { return color_; }
Plot& Plot::linestyle(const std::list<float>& f) noexcept { linestyle_=f; return (*this); }
Plot& Plot::linestyle(const std::string& ls) noexcept { 
    if ((ls=="solid") || (ls=="-")) return linestyle(std::list<float>());
    else if ((ls=="dotted") || (ls==":")) return linestyle(std::list{1.0f});
    else if ((ls=="dashed") || (ls=="--")) return linestyle(std::list{3.0f,1.0f});
    else if ((ls=="dashdot") || (ls=="-.")) return linestyle(std::list{5.0f,1.0f,1.0f,1.0f});
    else return (*this);
}


const std::list<float>& Plot::linestyle() const noexcept { return linestyle_; }

Plot& Plot::fmt(const std::string& f) noexcept {
    return (*this);
}


} 
}
