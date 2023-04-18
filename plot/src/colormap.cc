#include "../colormap.h"
#include <cmath>

namespace svg {
namespace plot {

ColorMap::ColorMap(const std::vector<svg::Color>& s, const std::string& name) : steps(s) {
	if (steps.size()<1) steps.push_back(svg::rgb(0,0,0));
	if (steps.size()<2) steps.push_back(svg::rgb(255,255,255));
    if (!name.empty()) registered[name]=this;
}

ColorMap::ColorMap(const std::vector<std::string>& s, const std::string& name) {
    for (const auto& name : s) steps.push_back(color_from_string(name));
	if (steps.size()<1) steps.push_back(svg::rgb(0,0,0));
	if (steps.size()<2) steps.push_back(svg::rgb(255,255,255));
    if (!name.empty()) registered[name]=this;
}

svg::ColorRGB ColorMap::operator()(float t, float tmin, float tmax) const noexcept {
    float pos = (steps.size()-1)*((t-tmin)/(tmax-tmin));
    int s = std::floor(pos);
    if (s < 0) return steps.front();
    else if (std::size_t(s) >= (steps.size()-1)) return steps.back();
    else {
        float t = pos - s; 
        return steps[s]*(1.0f-t) + steps[s+1]*t;
    }	
}

const ColorMap& ColorMap::from_name(const std::string& name) noexcept {
    if (auto it = registered.find(name); it != registered.end()) {
                return *(it->second);
            } else return viridis;
}

}
}