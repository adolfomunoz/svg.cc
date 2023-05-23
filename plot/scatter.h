#pragma once
#include "plottable.h"
#include "../src/color.h"
#include "../src/circle.h"
#include "colormap.h"
#include <vector>

namespace svg {
namespace plot {

class Scatter : public PlottableBase {
    std::vector<float> x, y;
    std::vector<float> markersize_ = std::vector<float>(1,1.0f);
    std::vector<float> linewidth_ = std::vector<float>(1,0.0f);
    std::vector<float> alpha_ = std::vector<float>(1,1.0f);
    std::vector<svg::Color> color_ = std::vector<svg::Color>(1,svg::black);
    std::vector<svg::Color> edgecolor_ = std::vector<svg::Color>(1,svg::black);
    ColorMap cmap_ = viridis;
    std::vector<float> value_ = std::vector<float>(0);
    std::optional<float> vmin_, vmax_;

    svg::Element marker_ = svg::Circle(0,0,1);

public:
    Scatter(const std::vector<float>& x, const std::vector<float>& y);
    Scatter(std::vector<float>&& x, const std::vector<float>& y);
    Scatter(const std::vector<float>& x, std::vector<float>&& y);
    Scatter(std::vector<float>&& x, std::vector<float>&& y);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;

    /*NAMED ATTRIBUTES*/
    Scatter& linewidths(float f) noexcept;
    Scatter& linewidths(const std::vector<float>& vf) noexcept;
    Scatter& linewidths(std::vector<float>&& vf) noexcept;
    template<typename Collection>
    Scatter& linewidths(const Collection& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->linewidths(std::vector<float>(vf.begin(),vf.end()));    
    }
    Scatter& alpha(float f) noexcept;
    Scatter& alpha(const std::vector<float>& vf) noexcept;
    Scatter& alpha(std::vector<float>&& vf) noexcept;
    template<typename Collection>
    Scatter& alpha(const Collection& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->alpha(std::vector<float>(vf.begin(),vf.end()));    
    }
    Scatter& c(const svg::Color& sc) noexcept;
	Scatter& c(const std::string& sc) noexcept;
	Scatter& c(const char* sc) noexcept;
    Scatter& c(std::vector<svg::Color>&& c) noexcept;
    Scatter& c(const std::vector<svg::Color>& c) noexcept;
    Scatter& c(const std::vector<std::string>& c) noexcept;
    Scatter& c(const std::vector<const char*>& c) noexcept;
    Scatter& c(std::vector<float>&& v) noexcept;
    Scatter& c(const std::vector<float>& v) noexcept;
    template<typename Collection>
    Scatter& c(const Collection& vc, std::enable_if_t<std::is_base_of_v<svg::ColorBase,typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->c(std::vector<svg::Color>(vc.begin(),vc.end()));    
    }
    template<typename Collection>
    Scatter& c(const Collection& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->c(std::vector<float>(vf.begin(),vf.end()));    
    }    
    template<typename Collection>
    Scatter& c(const Collection& vc, std::enable_if_t<std::is_same_v<char,typename std::decay_t<decltype(vc[0])>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->c(std::vector<std::string>(vc.begin(),vc.end()));    
    }
    Scatter& s(float f) noexcept;
    Scatter& s(const std::vector<float>& vf) noexcept;
    Scatter& s(std::vector<float>&& vf) noexcept;
    template<typename Collection>
    Scatter& s(Collection&& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->s(std::vector<float>(vf.begin(),vf.end()));    
    }    
    Scatter& edgecolors(const svg::Color& c) noexcept;
    Scatter& edgecolors(const std::string& c) noexcept;
	Scatter& edgecolors(const char* c) noexcept;
    Scatter& edgecolors(std::vector<svg::Color>&& c) noexcept;
    Scatter& edgecolors(const std::vector<svg::Color>& c) noexcept;
    Scatter& edgecolors(const std::vector<std::string>& c) noexcept;
    Scatter& edgecolors(const std::vector<const char*>& c) noexcept;
    template<typename Collection>
    Scatter& edgecolors(const Collection& vc, std::enable_if_t<std::is_base_of_v<svg::ColorBase,typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->edgecolors(std::vector<svg::Color>(vc.begin(),vc.end()));    
    }
    template<typename Collection>
    Scatter& edgecolors(const Collection& vc, std::enable_if_t<std::is_same_v<char,typename std::decay_t<decltype(vc[0])>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->edgecolors(std::vector<std::string>(vc.begin(),vc.end()));    
    }
    Scatter& marker(const svg::Element& m) noexcept;
    Scatter& marker(const std::string& f) noexcept;
    Scatter& marker(const char* f) noexcept;

    Scatter& vmin(float v) noexcept;
    Scatter& vmax(float v) noexcept;
    Scatter& cmap(const ColorMap& c) noexcept;
    Scatter& cmap(const std::string& c) noexcept;
    Scatter& cmap(const char* c) noexcept;
private:
    float vmin() const noexcept;
    float vmax() const noexcept;
	svg::Color color(std::size_t i = 0) const noexcept;
	svg::Color edgecolor(std::size_t i = 0) const noexcept;
    float alpha(std::size_t i = 0) const noexcept;
	float linewidth(std::size_t i = 0) const noexcept; 
    float markersize(std::size_t i = 0) const noexcept;
};

}
}