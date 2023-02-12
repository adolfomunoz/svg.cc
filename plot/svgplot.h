#pragma once
#include "axis-scale.h"
#include "plottable.h"
#include "plot.h"
#include "../src/svg.h"

#include <list>
#include <array>
#include <optional>
#include <filesystem>

namespace svg {
namespace plot {

class SVGPlot {
    std::list<Plottable> plottables;
    /*****************************
     * AXIS SCALES
     *****************************/
private:
    Scale xscale_ = scale::linear();
    Scale yscale_ = scale::linear();

public:
    Scale& set_xscale(const std::string& s);
    Scale& set_yscale(const std::string& s);
    Scale& set_xscale(const Scale& s);
    Scale& set_yscale(const Scale& s);
    SVGPlot& xscale(const std::string& s);
    SVGPlot& yscale(const std::string& s);
    SVGPlot& xscale(const Scale& s);
    SVGPlot& yscale(const Scale& s);
    const Scale& xscale() const noexcept;
    const Scale& yscale() const noexcept;

    /***************
     * PLOT variants
     ****************/
    Plot& plot(std::list<float>&& x, std::list<float>&& y) noexcept;

    /***************
     * GRAPH setup
     ****************/
private:
    std::optional<std::array<float,2>> figsize_;
    std::optional<std::array<float,4>> axis_;
    float linewidth_ = 1;
    std::vector<svg::Color> color_cycle{
        svg::ColorHex("#1f77b4"),svg::ColorHex("#ff7f0e"),svg::ColorHex("#2ca02c"),
        svg::ColorHex("#d62728"),svg::ColorHex("#9467bd"),svg::ColorHex("#8c564b"),
        svg::ColorHex("#e377c2"),svg::ColorHex("#7f7f7f"),svg::ColorHex("#bcbd22"),
        svg::ColorHex("#17becf")}; 
    std::size_t color_cycle_pos{0};
    svg::Color next_color() noexcept;

    std::array<float,2> figsize() const noexcept;
    std::array<float,4> axis() const noexcept;
    float linewidth() const noexcept;

    std::array<float,4> margin() const noexcept;
public:
    SVGPlot& figsize(const std::array<float,2>& fs) noexcept;
    SVGPlot& axis(const std::array<float,4>& a) noexcept;
    SVGPlot& linewidth(float lw) noexcept;
    /****************
     * OUTPUT
     ****************/
private:
    svg::SVG svg() const noexcept;
public:
    void savefig(const std::filesystem::path& name) const;
};

}
}