#include "../svgplot.h"
#include "../../src/group.h"
#include "../../src/rect.h"
#include "../../src/clipPath.h"
#include <codecvt>
#include <fstream>

namespace svg {
namespace plot {

    /*****************************
     * AXIS SCALES
     *****************************/

Scale& SVGPlot::set_xscale(const Scale& s) {
    xscale_ = s; return xscale_;
}
Scale& SVGPlot::set_yscale(const Scale& s) {
    yscale_ = s; return yscale_;
}
Scale& SVGPlot::set_xscale(const std::string& s) {
    xscale_.set_type(s); return xscale_;
}
Scale& SVGPlot::set_yscale(const std::string& s) {
    yscale_.set_type(s); return yscale_;
}
const Scale& SVGPlot::xscale() const noexcept { return xscale_; }
const Scale& SVGPlot::yscale() const noexcept { return yscale_; }

SVGPlot& SVGPlot::xscale(const Scale& s) {
    set_xscale(s); return (*this);
}
SVGPlot& SVGPlot::yscale(const Scale& s) {
    set_yscale(s); return (*this);
}
SVGPlot& SVGPlot::xscale(const std::string& s) {
    set_xscale(s); return (*this);
}
SVGPlot& SVGPlot::yscale(const std::string& s) {
    set_yscale(s); return (*this);
}

    /***************
     * PLOT variants
     ****************/

Plot& SVGPlot::plot(std::list<float>&& x, std::list<float>&& y) noexcept {
    plottables.push_back(Plot(std::forward<std::list<float>>(x),std::forward<std::list<float>>(y)));
    return plottables.back().cast_static<Plot>();
}

   /***************
     * GRAPH setup
     ****************/
std::array<float,2> SVGPlot::figsize() const noexcept {
    if (figsize_) return *figsize_;
    else return std::array<float,2>{640.0f,480.0f}; // Default size in matplotlib, although instead of inches we have dots and assume 100 dpi
}

std::array<float,4> axis_join(const std::array<float,4>& a, const std::array<float,4>& b) noexcept {
    return std::array<float,4>{
        std::min(a[0],b[0]),
        std::max(a[1],b[1]),
        std::min(a[2],b[2]),
        std::max(a[3],b[3])
    };
}

std::array<float,4> SVGPlot::axis() const noexcept {
    if (axis_) return *axis_;
    else if (plottables.empty()) return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
    else {
        std::array<float,4> a = plottables.front().axis();
        auto it = plottables.begin(); ++it;
        for (;it!=plottables.end();++it) a = axis_join(a,(*it).axis());
        return a;
	}
}

SVGPlot& SVGPlot::figsize(const std::array<float,2>& fs) noexcept {
    figsize_ = fs; return (*this);
}

SVGPlot& SVGPlot::axis(const std::array<float,4>& a) noexcept {
    axis_ = a; return (*this);
}

    /****************
     * OUTPUT
     ****************/

svg::SVG SVGPlot::svg() const noexcept {
    auto fs = figsize();
    auto ax = axis();
    svg::SVG output;
    output.viewBox(svg::Box(0.0f,0.0f,fs[0],fs[1]));
    svg::ClipPath& clip = output.add(ClipPath());
    clip.add(svg::Rect(0.0f,0.0f,fs[0],fs[1]));
    svg::Group& plots = output.add(svg::Group());
    plots.clip_path(svg::Url(clip.id()));
    for (const Plottable& plottable : plottables)
        plots.add(plottable.plot(xscale().transform(ax[0],ax[1],0.0f,fs[0]),
                                 yscale().transform(ax[2],ax[3],0.0f,fs[1])));
    return output;
}

const std::string& convert(const std::string& s) { return s; }
template<typename T>
static std::string convert(const std::basic_string<T>& s) {
    std::wstring_convert<std::codecvt_utf8<T>, T> converter;
    return converter.to_bytes(s);
}

void SVGPlot::savefig(const std::filesystem::path& name) const {
    std::filesystem::path svg_name = name;
    svg_name.replace_extension("svg");
    std::ofstream f(svg_name);
    svg().save_to_stream(f);
    f.close();
    if (name.extension() == ".png") {
        std::system((std::string("inkscape --export-type=\"png\" ")+convert(svg_name.native())).c_str());
        std::filesystem::remove(svg_name);
    } else if (name.extension() == ".pdf") {
        std::system((std::string("inkscape --export-type=\"pdf\" ")+convert(svg_name.native())).c_str());
        std::filesystem::remove(svg_name);
    } else if (name.extension() == ".eps") {
        std::system((std::string("inkscape --export-type=\"eps\" ")+convert(svg_name.native())).c_str());
        std::filesystem::remove(svg_name);
    }
}	

}
}