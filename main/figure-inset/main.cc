#include <svg.cc.h>
#include <iostream>
#include <list>

struct Config : public pattern::Reflectable<Config> {
    std::string input = "", output = "";
    int separation = 10;
    auto reflect() { return std::tie(input,output,separation);}
    auto reflect_names() const { return std::tuple("input","output","separation"); }
};

int main(int argc, char** argv) {
    Config config;
    pattern::load_commandline(config,argc,argv);
    svg::SVG file;
    if (config.input.empty()) file.load(std::cin);
    else file.load(config.input);

    std::list<svg::Rect> insets = file.find_all<svg::Rect>();
    svg::Image image = file.find_all<svg::Image>().front();

    svg::SVG out;
    out.add(image);
    for (const auto& inset : insets) out.add(inset);
    out.viewBox(svg::Box(image.x(),image.y(),image.width(),image.height()));

    if (config.output.empty()) { file.save(std::cout); std::cout<<std::endl; } 
    else  { file.save(config.output); }
}

