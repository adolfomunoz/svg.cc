#pragma once
#include <vector>
#include <string>
#include <patterns/patterns.h>

namespace svg {
namespace plot {
  
class ScaleBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual float transform(float x, float xmin, float xmax, float canvas_min, float canvas_max) const noexcept = 0;
    virtual std::vector<float> ticks(int target_ticks, float xmin, float xmax) const noexcept; 
    virtual std::string ticklabel(float value) const noexcept;
    virtual ~ScaleBase() {}
};

class Transform;

class Scale : public pattern::Pimpl<ScaleBase> {
public:
    using pattern::Pimpl<ScaleBase>::Pimpl;
    using pattern::Pimpl<ScaleBase>::operator=;

    float transform(float x, float xmin, float xmax, float canvas_min, float canvas_max) const noexcept {
        return impl()->transform(x,xmin,xmax,canvas_min,canvas_max);
    }
    virtual std::vector<float> ticks(int target_ticks, float xmin, float xmax) const noexcept {
        return impl()->ticks(target_ticks,xmin,xmax);
    }
    virtual std::string ticklabel(float value) const noexcept {
        return impl()->ticklabel(value);
    }

    Transform transform(float xmin, float xmax, float canvas_min, float canvas_max) const noexcept;
};

class Transform {
    Scale scale; float xmin, xmax, canvas_min, canvas_max;
public:
    Transform(const Scale& scale, float xmin, float xmax, float canvas_min, float canvas_max) noexcept;
    float operator()(float x) const noexcept;
};

namespace scale {

class linear : public pattern::Reflectable<linear,ScaleBase> {
public:
    static const char* type_name() { return "linear"; }
    float transform(float x, float xmin, float xmax, float canvas_min, float canvas_max)     const noexcept override;
};

/*
class log : public ScaleBase {
    float base_;
public:
    log(float base = 10.0f) : base_(base) {}
    log& base(float b) { base_ = b; return (*this); }
    float base() const { return base_; }

    bool is_valid(float x)       const noexcept override { return x>0; }
    float transform(float x)     const noexcept override { return std::log((x>0)?x:1.e-3f)/std::log(base()); }
    float antitransform(float y) const noexcept override { return std::pow(base(),y); }
    std::vector<float> ticks(int target_ticks, float xmin, float xmax) const noexcept override {
        int low = std::ceil(transform(xmin));
        int top = std::floor(transform(xmax));
        int de = std::max(1,(top - low)/(target_ticks-1));
		std::vector<float> sol;
        for (int e = low; e<=top; e+=de) sol.push_back(antitransform(e));
		return sol;
    }
    virtual std::tuple<float,float> axis_adjust(float xmin, float xmax) const noexcept {
        return std::tuple<float,float>(antitransform(std::floor(transform(xmin))),antitransform(std::ceil(transform(xmax))));
    }
    
    std::string ticklabel(float value) const noexcept override {
        std::stringstream s; 
        float l = transform(value);
        if (std::abs(l - int(l)) < 1.e-3) s<<"<tspan dy=\"2\" font-size=\".9em\">"<<base()<<"</tspan><tspan dy=\"-6\" font-size=\".7em\">"<<int(l)<<"</tspan>";
        else s<<((value==0)?0:value);
		return s.str(); 
    }
};

class symlog : public Base {
    float linthresh_;
    float linscale_;     
    log l;
public:
    symlog(float lt = 0.01f, float ls = 1.0f, float b = 10.0f) : linthresh_(lt), linscale_(ls), l(b) {}
    symlog& base(float b) { l.base(b); return (*this); }
    float base() const { return l.base(); }
    float linthresh() const { return linthresh_; }
    symlog& linthresh(float l) { linthresh_ = l; return (*this); }
    float linscale() const { return linscale_; }
    symlog& linscale(float l) { linscale_ = l; return (*this); }
   
    float transform(float x)     const noexcept override {        
        float linscale_adj = linscale() / (1.0f - 1.0f/l.base());
        if (std::abs(x)<linthresh()) return x*linscale_adj;
        else {
            float sign = (x>=0)?1.0f:-1.0f;
            return sign * linthresh()* (linscale_adj + l.transform(std::abs(x) / linthresh()));
        }
    }
    
    float antitransform(float y)     const noexcept override {
        float linscale_adj = linscale() / (1.0f - 1.0f/l.base());
        if (std::abs(y) < transform(linthresh())) return y/linscale_adj;
        else {
            float sign = (y>=0)?1.0f:-1.0f;
            return sign * linthresh()* l.antitransform(std::abs(y)/linthresh() - linscale_adj);
        }
    } 

    std::vector<float> ticks(int target_ticks, float xmin, float xmax) const noexcept override {
        std::vector<float> sol;
        //Negative area
        if (xmin < -linthresh()) {
            std::vector<float> negative_sol = l.ticks(target_ticks/2,linthresh(),-xmin);
            std::reverse(negative_sol.begin(),negative_sol.end());
            std::transform(negative_sol.begin(),negative_sol.end(),negative_sol.begin(),std::negate<float>());
            sol.insert(sol.end(),negative_sol.begin(),negative_sol.end());
        }
        if ((xmin < 0) && (xmax > 0)) sol.push_back(0);
        if (xmax > linthresh()) {
            std::vector<float> positive_sol = l.ticks(target_ticks/2,linthresh(),xmax);
            sol.insert(sol.end(),positive_sol.begin(),positive_sol.end());
        }
		return sol;
    }

    virtual std::tuple<float,float> axis_adjust(float xmin, float xmax) const noexcept {
        if (xmin > linthresh()) { return l.axis_adjust(xmin,xmax); }
        else if (xmax < -linthresh()) { 
            auto [a,b] = l.axis_adjust(-xmax,-xmin);
            return std::tuple<float,float>(-b,-a);
        } else {
            auto [pa,pb] = l.axis_adjust(linthresh(),xmax);
            auto [nb,na] = l.axis_adjust(linthresh(),-xmin); na*=-1; nb*=-1;
            return std::tuple<float,float>(std::min(0.0f,std::min(pa,na)),std::max(0.0f,std::max(pb,nb)));
        }
    }
    
    std::string ticklabel(float value) const noexcept override {
        if (value == 0) return "0";
        else if (value > 0) return l.ticklabel(value);
        else { // value < 0
            return std::string("-")+l.ticklabel(-value);
        }
    }
    
};
} 
*/ 
}

   
scale::linear linear;
/*
scale::symlog symlog;
scale::log log;
*/

}  
}
