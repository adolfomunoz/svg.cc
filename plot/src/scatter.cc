#include "../scatter.h"
#include "../../src/poly.h"
#include "../../src/use.h"
#include "../../src/group.h"
#include "../../src/defs.h"

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
            x(xscale(x[i])).y(yscale(y[i])).width(markersize(i)).height(markersize(i)).
            fill(color(i)).stroke_width(linewidth(i)).stroke(edgecolor(i)).
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

Scatter& Scatter::linewidth(float f) noexcept {
    this->linewidth_ = std::vector<float>(1,f);
    return (*this);
}
Scatter& Scatter::alpha(float f) noexcept {
    this->alpha_ = std::vector<float>(1,f);
    return (*this);
}
Scatter& Scatter::c(const svg::Color& c) noexcept {
    this->color_ = c;
    return (*this);
}
Scatter& Scatter::s(float f) noexcept {
    this->markersize_ = std::vector<float>(1,f);
    return (*this);
}
Scatter& Scatter::edgecolors(const svg::Color& c) noexcept {
    this->edgecolor_ = c;
    return (*this);
}

svg::Color Scatter::color(std::size_t i) const noexcept {
    return color_;
}
svg::Color Scatter::edgecolor(std::size_t) const noexcept {
    return edgecolor_;
}

float Scatter::alpha(std::size_t i) const noexcept {
    return alpha_[i%alpha_.size()];
}
float Scatter::linewidth(std::size_t i) const noexcept {
    return linewidth_[i%linewidth_.size()];
}
float Scatter::markersize(std::size_t i) const noexcept {
    return markersize_[i%linewidth_.size()];
}

}
}