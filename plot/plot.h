#pragma once
#include "plottable.h"
#include "../src/color.h"
#include <list>
#include "scatter.h"

namespace svg {
namespace plot {

class Plot : public PlottableBase {
    std::list<float> x, y;
    svg::Color color_ = svg::black;
    float linewidth_ = 1.0f, alpha_ = 1.0f;
    Scatter markers;
    std::list<float> linestyle_;
public:
    Plot(const std::list<float>& x, const std::list<float>& y);
    Plot(std::list<float>&& x, const std::list<float>& y);
    Plot(const std::list<float>& x, std::list<float>&& y);
    Plot(std::list<float>&& x, std::list<float>&& y);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;

    /*NAMED ATTRIBUTES*/
    Plot& linewidth(float f) noexcept;
	float linewidth() const noexcept; 
    Plot& alpha(float f) noexcept;
    float alpha() const noexcept;
	Plot& color(const svg::Color& c) noexcept;
	Plot& color(const std::string& c) noexcept;
	Plot& color(const char* c) noexcept;
	const svg::Color& color() const;
    Plot& fmt(const std::string& f) noexcept;
    Plot& linestyle(const std::list<float>& l) noexcept;
    Plot& linestyle(const std::string& ls) noexcept;
    const std::list<float>& linestyle() const noexcept;
};
}
}
