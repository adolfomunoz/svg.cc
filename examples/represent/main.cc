#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    svg::SVG file;
    if (argc > 1) file.load(argv[1]);
    else          file.load(std::cin);
    if (argc > 2) file.save(argv[2]);
    else          file.save(std::cout);
}

