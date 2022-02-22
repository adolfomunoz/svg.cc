#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    {
        svg::SVG test;
        test.add(svg::Circle(300,300,100));
        std::cout<<test.code();
    }
    
    {
        svg::SVG test;
        test.load_from_code("<svg>\
                                <circle cx=\"1\"/>\
                             </svg>");
        std::cout<<test.code()<<std::endl;
    }

}

