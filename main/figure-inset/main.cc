#include <svg.cc.h>
#include <iostream>
#include <list>

struct Config : public pattern::Reflectable<Config> {
    std::string input = "", output = "";
    float separation = 0.02f;
    auto reflect() { return std::tie(input,output,separation);}
    auto reflect_names() const { return std::tuple("input","output","separation"); }
};

float inset_stroke_width(const svg::Rect& inset) {
    float stroke_width = inset.stroke_width();
    if (inset.style().has_stroke_width()) stroke_width = inset.style().stroke_width();
    return stroke_width;
}

int main(int argc, char** argv) {
    Config config;
    pattern::load_commandline(config,argc,argv);
    svg::SVG file;
    if (config.input.empty()) file.load(std::cin);
    else file.load(config.input);

    std::list<svg::Rect> insets = svg::find_all<svg::Rect>(file);
    std::list<svg::Image> images = svg::find_all<svg::Image>(file);

    if (images.empty()) {
        std::cerr<<"Input svg has no image."<<std::endl;
        return 1;
    }
    svg::Image image = images.front();
    if (insets.empty()) {
        std::cerr<<"No rectangles that define the insets."<<std::endl;
        return 1;
    }

    float separation = config.separation*image.height();


    svg::SVG out;
    svg::Defs& defs = out.add(svg::Defs());
    out.add(image);
    for (const auto& inset : insets) out.add(inset);

    float ar = 0.0f;
    for (const svg::Rect& inset : insets) ar += inset.width()/inset.height();
    float inset_width = (image.width()-separation*float(insets.size()-1)-inset_stroke_width(insets.front()));
    float final_height = inset_width/ar;
    float x = image.x();
    for (const svg::Rect& inset : insets) {
        svg::Rect outside_inset = inset;
        float size_factor = final_height/inset.height();
        float stroke_width = inset_stroke_width(inset);
        outside_inset
            .x(x+0.5f*stroke_width)
            .y(image.y()-0.5f*stroke_width+image.height()+separation)
            .width(inset.width()*size_factor)
            .height(inset.height()*size_factor);
        //Not very clear on the 0.5f*stroke_width on width and height but it seems to be
        // aligning everything propperly
        out.add(outside_inset);
        x+=outside_inset.width()+separation;
    }

    out.viewBox(svg::Box(image.x(),image.y(),image.width(),image.height()+final_height+separation));

    if (config.output.empty()) { out.save(std::cout); std::cout<<std::endl; } 
    else  { out.save(config.output); }
    return 0;
}

