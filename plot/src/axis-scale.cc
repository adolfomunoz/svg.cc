#include "../axis-scale.h"
#include <cmath>
#include <sstream>

namespace svg {
namespace plot {

//Adjust a floating point number so it only has two meaningful digits
float adjust(float x) noexcept {
    float div_by = std::pow(int(std::log10(std::abs(x))),10.0f)/100.0f;
    return int(x/div_by)*div_by;
}

std::vector<float> ScaleBase::ticks(int target_ticks, float xmin, float xmax) const noexcept {
    float tick_ref = 0;
    if ((xmax*xmin) > 0.0f) tick_ref = adjust(0.5*(xmax+xmin));
    float tick_step = adjust((xmax-xmin)/float(target_ticks-1));

    std::vector<float> sol;
    for (int i = (target_ticks/2); i>0; --i) sol.push_back(tick_ref - tick_step*i);
    sol.push_back(tick_ref);
    for (int i = 1; i<=(target_ticks/2); ++i) sol.push_back(tick_ref + tick_step*i);
    return sol;
}
    
std::string ScaleBase::ticklabel(float value) const noexcept {
    std::stringstream s; 
	s<<((value==0)?0:value);
	return s.str(); 
}

Transform Scale::transform(float xmin, float xmax, float canvas_min, float canvas_max) const noexcept {
    return Transform(*this, xmin, xmax, canvas_min, canvas_max);
}

Transform::Transform(const Scale& scale, float xmin, float xmax, float canvas_min, float canvas_max) noexcept :
    scale(scale), xmin(xmin), xmax(xmax), canvas_min(canvas_min), canvas_max(canvas_max) {}

float Transform::operator()(float x) const noexcept {
    return scale.transform(x,xmin,xmax,canvas_min,canvas_max);
}

float scale::linear::transform(float x, float xmin, float xmax, float canvas_min, float canvas_max) const noexcept {
    float t = (x - xmin)/(xmax - xmin);
    return t*(canvas_max - canvas_min) + canvas_min;
}

}
plot::scale::linear linear;
}