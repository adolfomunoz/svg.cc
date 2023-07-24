#include "../axis-scale.h"
#include <cmath>
#include <sstream>

namespace svg {
namespace plot {

//Adjust a floating point number so it only has "digits" meaningful digits
float adjust(float x, float digits = 2) noexcept {
    float div_by = std::pow(10.0f,int(std::log10(std::abs(x))))/std::pow(10.0f,digits-1);
    return int(x/div_by)*div_by;
}

//Adjust a floating point number so it is rounded to the corresponding floating point number
float round_to(float x, float value = 1) noexcept {
    return int(x/value)*value;
}


std::vector<float> ScaleBase::ticks(int target_ticks, float xmin, float xmax) const noexcept {
    float tick_ref = 0;
    if ((xmax*xmin) > 0.0f) tick_ref = adjust(0.5*(xmax+xmin),1);
    int digits=1;
    float to_adjust = (xmax-xmin)/float(target_ticks-1);
    float tick_step = adjust(to_adjust,digits);
    while (std::abs(tick_step)<1.e-10) {
        ++digits;
        tick_step = adjust(to_adjust,digits);
    }

    std::vector<float> sol;
    for (int i = (target_ticks+1); i>0; --i) {
        float tick = tick_ref - tick_step*i;
        if (tick >= xmin) sol.push_back(tick);
    }
    sol.push_back(tick_ref); 

    for (int i = 1; i<=(target_ticks+1); ++i) {
        float tick = tick_ref + tick_step*i;
        if (tick <= xmax) sol.push_back(tick);
    }
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
    scale_(scale), xmin_(xmin), xmax_(xmax), canvas_min_(canvas_min), canvas_max_(canvas_max) {}

float Transform::operator()(float x) const noexcept {
    return scale().transform(x,xmin(),xmax(),canvas_min(),canvas_max());
}

Transform Transform::with_new_canvas(const Transform& that) const noexcept {
    return Transform(this->scale(),this->xmin(),this->xmax(),that.canvas_min(),that.canvas_max());
}

float Transform::canvas_min() const noexcept { return canvas_min_; }
float Transform::canvas_max() const noexcept { return canvas_max_; }
float Transform::xmin() const noexcept { return xmin_; }
float Transform::xmax() const noexcept { return xmax_; }

const Scale& Transform::scale() const noexcept { return scale_; }

float scale::linear::transform(float x, float xmin, float xmax, float canvas_min, float canvas_max) const noexcept {
    float t = (x - xmin)/(xmax - xmin);
    return t*(canvas_max - canvas_min) + canvas_min;
}

}
plot::scale::linear linear;
}
