#include "../scatter.h"
#include "../../src/poly.h"

namespace svg {
namespace plot {

Scatter::Scatter(const std::vector<float>& x, const std::vector<float>& y) : x(x),y(y) {}
Scatter::Scatter(std::vector<float>&& x, const std::vector<float>& y) : x(std::move(x)), y(y) {}
Scatter::Scatter(const std::vector<float>& x, std::vector<float>&& y) : x(x), y(std::move(y)) {}
Scatter::Scatter(std::vector<float>&& x, std::vector<float>&& y) : x(std::move(x)), y(std::move(y)) {}

svg::Element Scatter::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Group output;
    for (std::size_t i = 0; (i<x.size()) && (i<y.size()); ++i) {

    }
    std::list<float>::const_iterator ix, iy;
    for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy)
        output.add_point(xscale(*ix),yscale(*iy));
    output.fill(none).stroke_width(linewidth()).stroke(color()).opacity(alpha());
    return output;
}
std::array<float,4> Scatter::axis() const noexcept {
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

    //Arbitrary expansion, maybe not a good idea but it is worse to account for markersize which is in a different space
    float dx = std::abs(ax[1]-ax[0])/32.0f; //+ max_size/std::abs(ax[1]-ax[0]);
    float dy = std::abs(ax[3]-ax[2])/32.0f; //+ max_size/std::abs(ax[3]-ax[2]);
    ax[0]-=dx; ax[1]+=dx; ax[2]-=dy; ax[3]+=dy;
}

Plot& Plot::linewidth(float f) noexcept { linewidth_=f; return *this; }
float Plot::linewidth() const noexcept { return linewidth_; }
Plot& Plot::alpha(float f) noexcept { alpha_=f; return *this; }
float Plot::alpha() const noexcept { return alpha_; }
Plot& Plot::color(const svg::Color& c) { color_=c; return *this; }
const svg::Color& Plot::color() const { return color_; }
} 
}