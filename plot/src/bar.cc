#include "../bar.h"
//We have to include everything, otherwise the svg in marker might not load propperly
#include "../../src/rect.h"
#include "../../src/group.h"
#include "color.h"
#include <cmath>
#include <limits>

namespace svg {
namespace plot {

Bar::Bar(const std::vector<float>& x, const std::vector<float>& h) : x_(x),height_(h) {}
Bar::Bar(std::vector<float>&& x, const std::vector<float>& h) : x_(std::move(x)), height_(h) {}
Bar::Bar(const std::vector<float>& x, std::vector<float>&& h) : x_(x), height_(std::move(h)) {}
Bar::Bar(std::vector<float>&& x, std::vector<float>&& h) : x_(std::move(x)), height_(std::move(h)) {}

svg::Element Bar::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Group output;
    return output;
}
std::array<float,4> Bar::axis() const noexcept {
//    if (x.empty() || y.empty()) 
    return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
/*    std::array<float,4> a{x.front(),x.front(),y.front(),y.front()};
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
    return a;*/
}

Bar& Bar::width(std::vector<float>&& vf) noexcept {
    this->width_ = std::move(vf);
    return (*this);
}
Bar& Bar::width(const std::vector<float>& vf) noexcept {
    this->width_ = vf;
    return (*this);
}
Bar& Bar::width(float f) noexcept {
    return width(std::vector<float>(1,f));
}
Bar& Bar::alpha(std::vector<float>&& vf) noexcept {
    this->alpha_ = std::move(vf);
    return (*this);
}
Bar& Bar::alpha(const std::vector<float>& vf) noexcept {
    this->alpha_ = vf;
    return (*this);
}
Bar& Bar::alpha(float f) noexcept {
    return alpha(std::vector<float>(1,f));
}
Bar& Bar::color(std::vector<svg::Color>&& sc) noexcept {
    this->color_ = std::move(sc);
    return (*this);
}
Bar& Bar::color(const std::vector<svg::Color>& sc) noexcept {
    this->color_ = sc; return (*this);
}
Bar& Bar::color(const std::vector<std::string>& sc) noexcept {
    std::vector<svg::Color> cs(sc.size(),svg::black);
    std::transform(sc.begin(),sc.end(),cs.begin(),color_from_string);
    return color(std::move(cs));
}
Bar& Bar::color(const std::vector<const char*>& sc) noexcept {
    std::vector<svg::Color> cs(sc.size(),svg::black);
    std::transform(sc.begin(),sc.end(),cs.begin(),[] (const char* cc) { return color_from_string(std::string(cc)); });
    return color(std::move(cs));
}
Bar& Bar::color(const svg::Color& sc) noexcept {
    return color(std::vector<svg::Color>(1,sc));
}
Bar& Bar::color(const std::string& sc) noexcept {
    return color(color_from_string(sc));
}
Bar& Bar::color(const char* sc) noexcept {
    return color(std::string(sc));
}

Bar& Bar::bottom(std::vector<float>&& vf) noexcept {
    this->bottom_ = std::move(vf);
    return (*this);
}
Bar& Bar::bottom(const std::vector<float>& vf) noexcept {
    this->bottom_ = vf;
    return (*this);
}
Bar& Bar::bottom(float f) noexcept {
    return bottom(std::vector<float>(1,f));
}

svg::Color Bar::color(std::size_t i) const noexcept {
    return color_[i%color_.size()];
}

float Bar::alpha(std::size_t i) const noexcept {
    return alpha_[i%alpha_.size()];
}
float Bar::width(std::size_t i) const noexcept {
    return width_[i%width_.size()];
}
float Bar::bottom(std::size_t i) const noexcept {
    return bottom_[i%bottom_.size()];
}
float Bar::x(std::size_t i) const noexcept {
    return x_[i%x_.size()];
}
float Bar::height(std::size_t i) const noexcept {
    return height_[i%height_.size()];
}


}
}
