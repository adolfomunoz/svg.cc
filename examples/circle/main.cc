#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    {
        svg::SVG test;
        test.add(svg::Circle(300,300,100)).stroke_width(3);
        std::cout<<test.code();
    }
    
    {
        svg::SVG test;
        test.load_from_code("<svg>\
                                <circle cx=\"1\" stroke-width=\"2mm\"/>\
                             </svg>");
        std::cout<<test.code()<<std::endl;
        
    }
}

