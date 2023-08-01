#pragma once
#include "plottable.h"
#include <optional>
#include "src/color.h"

namespace svg {
namespace plot {

class OrientationBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual bool is_horizontal() const noexcept = 0;
    bool is_vertical() const noexcept { return !is_horizontal(); }
};
class Orientation : public pattern::Pimpl<OrientationBase> {
public:
    using pattern::Pimpl<OrientationBase>::Pimpl;
    using pattern::Pimpl<OrientationBase>::operator=;
    bool is_horizontal() const noexcept override { return impl()->is_horizontal(); }
};
class OrientationHorizontal : public pattern::Reflectable<OrientationHorizontal, OrientationBase> {
public:
    OrientationHorizontal(){} 
    static const char* type_name() { return "horizontal"; } 
    bool is_horizontal() const noexcept override { return true; }
};
extern OrientationHorizontal horizontal;
class OrientationVertical : public pattern::Reflectable<OrientationVertical, OrientationBase> {
public:
    OrientationVertical(){} 
    static const char* type_name() { return "vertical"; } 
    bool is_horizontal() const noexcept override { return false; }
};
extern OrientationVertical vertical;

class Hist;
class HistTypeBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual Plottable representation(const Hist& hist) const noexcept = 0;
};
class HistType : public pattern::Pimpl<HistTypeBase> {
public:
    using pattern::Pimpl<HistTypeBase>::Pimpl;
    using pattern::Pimpl<HistTypeBase>::operator=;
    Plottable representation(const Hist& hist) const noexcept override {
        return impl()->representation(hist);
    }
};
class HistTypeBar : public pattern::Reflectable<HistTypeBar, HistTypeBase> {
public:
    HistTypeBar(){} 
    static const char* type_name() { return "bar"; } 
    Plottable representation(const Hist& hist) const noexcept override;
};
extern HistTypeBar bar;
class HistTypeStep : public pattern::Reflectable<HistTypeStep, HistTypeBase> {
public:
    HistTypeStep(){} 
    static const char* type_name() { return "step"; } 
    Plottable representation(const Hist& hist) const noexcept override;
};
extern HistTypeStep step;


class Hist : public PlottableBase {
    std::vector<float> x_; //Todo: enable multiple data (std::vector<std::vector<float>>)
    std::size_t bins_value = 10; 
    std::optional<std::tuple<float,float>> range_;
    std::vector<float> bins_ = std::vector<float>(0); 
    std::vector<float> weights_ = std::vector<float>(1,1.0f);
    bool density_ = false, cumulative_ = false;
    svg::Color color_ = svg::black;
    float alpha_ = 1.0f;

    Orientation orientation_ = horizontal;
    HistType histtype_ = bar;
public:
	Hist(const std::vector<float>& x);
	Hist(std::vector<float>&& x);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;

    std::tuple<float,float> range() const noexcept;  
    Hist& range(const std::tuple<float,float> r) noexcept;  
    Hist& weights(const std::vector<float>& w) noexcept;
    
private:       
    std::size_t bins_size() const noexcept;
    float bin(std::size_t i) const noexcept;
    
public:
    Hist& orientation(const Orientation& o) noexcept;
    const Orientation& orientation() const noexcept;
    Hist& histtype(const HistType& h) noexcept;
    const HistType& histtype() const noexcept;
    
private:
    float weight_(int i) const noexcept; 
public:
    Hist& density(bool b = true) noexcept;
    Hist& cumulative(bool c = true) noexcept;
    
    Hist& bins(std::size_t b) noexcept;
    Hist& bins(int b) noexcept;
    Hist& bins(const std::vector<float>& b) noexcept;
    template<typename Collection>
    Hist& bins(Collection&& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->bins(std::vector<float>(vf.begin(),vf.end()));    
    }    
    
    std::vector<float> hist_values() const noexcept;
    std::vector<float> hist_positions() const noexcept;
    std::vector<float> hist_widths() const noexcept;

    Hist& alpha(float f) noexcept;
    float alpha() const noexcept;


	Hist& color(const svg::Color& c) noexcept;
	Hist& color(const std::string& c) noexcept;
	Hist& color(const char* c) noexcept;
	const svg::Color& color() const;
    
private:
/*    std::unique_ptr<Plottable> representation() const {
        if (histtype_==HistType::bar) {
            if (orientation_==Orientation::vertical) {
                auto sol = std::make_unique<Bar>(hist_positions(),hist_values());
                sol->width(hist_widths()).color(color_).alpha(alpha());
                return sol;
            } else {
                auto sol = std::make_unique<BarH>(hist_positions(),hist_values());
                sol->height(hist_widths()).color(color_).alpha(alpha());
                return sol;
            }
        } else { // histtype_==HistType::step
            if (orientation_==Orientation::vertical) {
                auto sol = std::make_unique<Plot>(hist_positions(),hist_values());
                sol->color(color_).alpha(alpha());
                return sol;
            } else { 
                auto sol = std::make_unique<Plot>(hist_values(),hist_positions());
                sol->color(color_).alpha(alpha());
                return sol;
            }
        }  
    }*/
};

}
}
