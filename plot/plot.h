#include "axis-scale.h"
#include "plottable.h"

namespace svg {
namespace plot {

class SVGPlot {
    std::list<Plottable> plottables;
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
};

}
}