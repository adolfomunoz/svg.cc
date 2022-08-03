#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    std::cout<<"Registered SVG elements = "<<svg::Element::registered()<<std::endl;
    std::cout<<"Registered length units = "<<svg::LengthUnit::registered()<<std::endl;
    std::cout<<"Registered colors       = "<<svg::Color::registered()<<std::endl;
    std::cout<<"Registered path commands= "<<svg::Command::registered()<<std::endl;
    std::cout<<"Registered transforms   = "<<svg::Transform::registered()<<std::endl;
    std::cout<<"Registered clips        = "<<svg::Clip::registered()<<std::endl;
}

