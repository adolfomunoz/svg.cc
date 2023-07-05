#pragma once
#include "plottable.h"
#include "../src/color.h"
#include "../src/circle.h"
#include "colormap.h"
#include <vector>

namespace svg {
namespace plot {

class Bar : public PlottableBase {
    std::vector<float> x_, height_;
    std::vector<float> width_ = std::vector<float>(1,1.0f);
    std::vector<float> bottom_ = std::vector<float>(0,1.0f);
    std::vector<float> alpha_ = std::vector<float>(1,1.0f);
    std::vector<svg::Color> color_ = std::vector<svg::Color>(1,svg::black);
public:
    Bar(const std::vector<float>& x, const std::vector<float>& h);
    Bar(std::vector<float>&& x, const std::vector<float>& h);
    Bar(const std::vector<float>& x, std::vector<float>&& h);
    Bar(std::vector<float>&& x, std::vector<float>&& h);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;

    /*NAMED ATTRIBUTES*/
    Bar& alpha(float f) noexcept;
    Bar& alpha(const std::vector<float>& vf) noexcept;
    Bar& alpha(std::vector<float>&& vf) noexcept;
    template<typename Collection>
    Bar& alpha(const Collection& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->alpha(std::vector<float>(vf.begin(),vf.end()));    
    }
    Bar& bottom(float f) noexcept;
    Bar& bottom(const std::vector<float>& vf) noexcept;
    Bar& bottom(std::vector<float>&& vf) noexcept;
    template<typename Collection>
    Bar& bottom(const Collection& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->bottom(std::vector<float>(vf.begin(),vf.end()));    
    }
    Bar& color(const svg::Color& sc) noexcept;
    Bar& color(const std::string& sc) noexcept;
    Bar& color(const char* sc) noexcept;
    Bar& color(std::vector<svg::Color>&& c) noexcept;
    Bar& color(const std::vector<svg::Color>& c) noexcept;
    Bar& color(const std::vector<std::string>& c) noexcept;
    Bar& color(const std::vector<const char*>& c) noexcept;
    template<typename Collection>
    Bar& color(const Collection& vc, std::enable_if_t<std::is_base_of_v<svg::ColorBase,typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->color(std::vector<svg::Color>(vc.begin(),vc.end()));    
    }
    Bar& width(float f) noexcept;
    Bar& width(const std::vector<float>& vf) noexcept;
    Bar& width(std::vector<float>&& vf) noexcept;
    template<typename Collection>
    Bar& width(Collection&& vf, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->width(std::vector<float>(vf.begin(),vf.end()));    
    }    
private:
	svg::Color color(std::size_t i = 0) const noexcept;
    float alpha(std::size_t i = 0) const noexcept;
	float width(std::size_t i = 0) const noexcept; 
    float x(std::size_t i = 0) const noexcept;
    float height(std::size_t i = 0) const noexcept;
    float bottom(std::size_t i = 0) const noexcept;
};

}
}