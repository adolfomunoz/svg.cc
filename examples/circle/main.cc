#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    {
        svg::Circle circle(1,1,1);
        std::cout<<pattern::xml(circle,pattern::xml_reflect_attributes_from_stream)<<std::endl;
    }
}

