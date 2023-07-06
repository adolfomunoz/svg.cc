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
    for (std::size_t i = 0; i<nbars();++i)
        output.add(svg::Rect(x(i)-0.5f*width(i),bottom(i),x(i)+0.5f*width(i),bottom(i)+height(i)))
            .opacity(alpha(i))
            .fill(color(i))
            .stroke_width(0);
   
    return output;
}    
std::size_t Bar::nbars() const noexcept {
    return std::min(x_.size(),height_.size());
}

std::array<float,4> Bar::axis() const noexcept {
    std::array<float,4> a{x(0)-0.5f,x(0)+0.5f,bottom(0),height(0)};
    for (std::size_t i = 1; i<nbars();++i) {
        a[0] = std::min(a[0],x(i)-0.5f);
        a[1] = std::max(a[1],x(i)+0.5f);
        a[2] = std::min(a[2],bottom(i));
        a[3] = std::max(a[3],bottom(i)+height(i));
    }
    return a;
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
