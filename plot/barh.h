#pragma once
#include "plottable.h"
#include "bar.h"





namespace svg {
namespace plot {

class BarH: public PlottableBase {
    Bar bar;
public:
    BarH(const std::vector<float>& x, const std::vector<float>& h);
    BarH(std::vector<float>&& x, const std::vector<float>& h);
    BarH(const std::vector<float>& x, std::vector<float>&& h);
    BarH(std::vector<float>&& x, std::vector<float>&& h);

    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
    std::array<float,4> axis() const noexcept override;

    /*NAMED ATTRIBUTES*/
    template<typename T>
    BarH& alpha(T&& t) noexcept {
        bar.alpha(std::forward<T>(t)); return (*this);
    } 
    template<typename T>
    BarH& left(T&& t) noexcept {
        bar.bottom(std::forward<T>(t)); return (*this);
    } 
    template<typename T>
    BarH& color(T&& t) noexcept {
        bar.color(std::forward<T>(t)); return (*this);
    } 
    BarH& color(const std::initializer_list<const char*>& t) noexcept;
    template<typename T>
    BarH& height(T&& t) noexcept {
        bar.width(std::forward<T>(t)); return (*this);
    } 
};

}
}