#pragma once
#include "plottable.h"
#include "../src/color.h"
#include "../src/circle.h"
#include <vector>

namespace svg {
namespace plot {

class Scatter : public PlottableBase {
    std::vector<float> x, y;
    std::vector<float> markersize_ = std::vector<float>(1,1.0f);
    std::vector<float> linewidth_ = std::vector<float>(1,0.0f);
    std::vector<float> alpha_ = std::vector<float>(1,1.0f);
    svg::Color color_ = svg::black;
    svg::Color edgecolor_ = svg::black;
    svg::Element marker_ = svg::Circle(0,0,1);

public:
    Scatter(const std::vector<float>& x, const std::vector<float>& y);
    Scatter(std::vector<float>&& x, const std::vector<float>& y);
    Scatter(const std::vector<float>& x, std::vector<float>&& y);
    Scatter(std::vector<float>&& x, std::vector<float>&& y);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;

    /*NAMED ATTRIBUTES*/
    Scatter& linewidth(float f) noexcept;
    Scatter& alpha(float f) noexcept;
	Scatter& c(const svg::Color& c) noexcept;
    Scatter& s(float f) noexcept;
    Scatter& edgecolors(const svg::Color& c) noexcept;
private:
	svg::Color color(std::size_t i = 0) const noexcept;
	svg::Color edgecolor(std::size_t i = 0) const noexcept;
    float alpha(std::size_t i = 0) const noexcept;
	float linewidth(std::size_t i = 0) const noexcept; 
    float markersize(std::size_t i = 0) const noexcept;
};
}
}