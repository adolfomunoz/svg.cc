#pragma once
#include <vector>
#include "src/color.h"
#include <string>
#include <map>


namespace svg {
namespace plot {

class ColorMap {
	std::vector<svg::Color> steps;
    static std::map<std::string,ColorMap*> registered;
public:
	ColorMap(const std::vector<svg::Color>& s, const std::string& name = "");
    ColorMap(const std::vector<std::string>& s, const std::string& name = "");	
	svg::ColorRGB operator()(float t, float tmin = 0.0f, float tmax = 1.0f) const noexcept;
    static const ColorMap& from_name(const std::string& name) noexcept;
};

extern ColorMap viridis;
extern ColorMap magma;
extern ColorMap inferno;
extern ColorMap plasma;

extern ColorMap bwr;
extern ColorMap coolwarm;
extern ColorMap seismic;
extern ColorMap spectral;
extern ColorMap piyg;
extern ColorMap grayscale;

}
}
