#include <svg.cc.h>
#include <iostream>
#include <list>

int main(int argc, char** argv) {
    svg::SVG file;
    if (argc > 1) file.load_from_file(argv[1]);
    else          file.load_from_stream(std::cin);
    svg::for_all<svg::Image>(file,
        [] (svg::Image& image) {
            if ((image.href().length()>=4) && (image.href()[image.href().length()-4]=='.'))
                image.embed(image.href());
        });
    if (argc > 2) file.save_to_file(argv[2]);
    else          { file.save_to_stream(std::cout); std::cout<<std::endl; }
}

