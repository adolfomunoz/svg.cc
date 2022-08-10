#include <svg.cc.h>
#include <iostream>
#include <list>

float inset_stroke_width(const svg::Rect& inset) {
    float stroke_width = inset.stroke_width();
    if (inset.style().has_stroke_width()) stroke_width = inset.style().stroke_width();
    return stroke_width;
}

struct Config : public pattern::Reflectable<Config> {
    std::string input = "", output = "";
    float separation = 0.02f;
    auto reflect() { return std::tie(input,output,separation);}
    auto reflect_names() const { return std::tuple("input","output","separation"); }
};


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
    float pos = image.x();
    int inset_id = 0;
    for (const svg::Rect& inset : insets) {
        svg::Rect outside_inset = inset;
        float size_factor = final_height/inset.height();
        float stroke_width = inset_stroke_width(inset);
        float x = pos + 0.5f*stroke_width;
        float y = image.y()-0.5f*stroke_width+image.height()+separation;
        float width = inset.width()*size_factor;
        float height = inset.height()*size_factor;
        outside_inset.x(x).y(y).width(width).height(height);
        std::stringstream id; id<<"inset-"<<std::setw(5) << std::setfill('0')<<(++inset_id);
        defs.add(svg::ClipPath()).id(id.str()).add(inset);
        out.add(image)
            .add_transform(svg::Translate(-image.x()-inset.x(),-image.y()-inset.y()))
            .add_transform(svg::Scale(size_factor))
            .add_transform(svg::Translate(x,y))
            .clip_path(svg::Url(id.str()));
        out.add(outside_inset);
        pos+=outside_inset.width()+separation;
    }

    out.viewBox(svg::Box(image.x(),image.y(),image.width(),image.height()+final_height+separation));

    if (config.output.empty()) { out.save(std::cout); std::cout<<std::endl; } 
    else  { out.save(config.output); }
    return 0;
}

