#include "../barh.h"
#include "../../src/group.h"




namespace svg {
namespace plot {

BarH::BarH(const std::vector<float>& x, const std::vector<float>& h) : bar(x,h) {}
BarH::BarH(std::vector<float>&& x, const std::vector<float>& h) : bar(std::move(x),h) {}
BarH::BarH(const std::vector<float>& x, std::vector<float>&& h) : bar(x,std::move(h)) {}
BarH::BarH(std::vector<float>&& x, std::vector<float>&& h) : bar(std::move(x),std::move(h)) {}

svg::Element BarH::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Group output;
    output.add(bar.plot(yscale,xscale));
//    output.add_transform(svg::Scale(1,-1));
    output.add_transform(svg::Translate(0,-xscale.canvas_min()));
    output.add_transform(svg::Rotate(-90));
    output.add_transform(svg::Translate(xscale.canvas_min(),0.5f*xscale.canvas_min()+yscale.canvas_min()));
    return output;
}    

std::array<float,4> BarH::axis() const noexcept {
    std::array<float,4> av = bar.axis();
    return std::array<float,4>{av[2],av[3],av[0],av[1]}; 
}

BarH& BarH::color(const std::initializer_list<const char*>& t) noexcept {
    bar.color(t); return (*this);
}


}
}
