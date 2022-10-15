#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    svg::SVG file;
    if (argc > 1) file.load_from_file(argv[1]);
    else          file.load_from_stream(std::cin);
    if (argc > 2) file.save_to_file(argv[2]);
    else          { file.save_to_stream(std::cout); std::cout<<std::endl; }
}

