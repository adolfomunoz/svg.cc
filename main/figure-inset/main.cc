#include <svg.cc.h>
#include <iostream>
#include <list>

class LayoutBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual void draw_insets(svg::SVG& out, 
            const std::list<svg::Image>& images, 
            const std::list<svg::Rect>& insets) const = 0;
    virtual ~LayoutBase() {}
};

class Layout : public pattern::Pimpl<LayoutBase> {
public:
    using pattern::Pimpl<LayoutBase>::Pimpl;
    using pattern::Pimpl<LayoutBase>::operator=;

    void draw_insets(svg::SVG& out, 
            const std::list<svg::Image>& images, 
            const std::list<svg::Rect>& insets) const override {
                this->impl()->draw_insets(out,images,insets);
            }
}; 

float inset_stroke_width(const svg::Rect& inset) {
    float stroke_width = inset.stroke_width();
    if (inset.style().has_stroke_width()) stroke_width = inset.style().stroke_width();
    return stroke_width;
}

class LayoutSingleImageBottomInsets : public pattern::Reflectable<LayoutSingleImageBottomInsets,LayoutBase> {
    float separation = 0.02f;
public:
    auto reflect() { return std::tie(separation);}
    auto reflect_names() const { return std::tuple("separation"); }

    static const char* type_name() { return "layout-single-image-bottom-insets"; } \
    void draw_insets(svg::SVG& out, 
        const std::list<svg::Image>& images, 
        const std::list<svg::Rect>& insets) const override {
            svg::Defs& defs = out.add(svg::Defs());
            
            svg::Image image = images.front(); 
            float separation = this->separation*image.height();
            out.add(image);
            for (const auto& inset : insets) out.add(inset);

            float ar = 0.0f;
            for (const svg::Rect& inset : insets) ar += inset.width()/inset.height();
            float inset_size = (image.width()-separation*float(insets.size()-1)-inset_stroke_width(insets.front()));
            float final_height = inset_size/ar;
            float inset_y = image.y() + image.height() + separation;
            for (const svg::Image& image : images) {
                float inset_x = image.x();
                int inset_id = 0;
                for (const svg::Rect& inset : insets) {
                    svg::Rect outside_inset = inset;
                    float size_factor = final_height/inset.height();
                    float stroke_width = inset_stroke_width(inset);

                    outside_inset
                        .x(inset_x + 0.5f*stroke_width)
                        .y(inset_y - 0.5f*stroke_width)
                        .width(inset.width()*size_factor)
                        .height(inset.height()*size_factor);


                    std::stringstream id; id<<"inset-"<<std::setw(5) << std::setfill('0')<<(++inset_id);
                    defs.add(svg::ClipPath()).id(id.str()).add(inset);
                    out.add(image)
                        .add_transform(svg::Translate(-image.x()-inset.x(),-image.y()-inset.y()))
                        .add_transform(svg::Scale(size_factor))
                        .add_transform(svg::Translate(outside_inset.x(),outside_inset.y()))
                        .clip_path(svg::Url(id.str()));
                    out.add(outside_inset);
                    inset_x+=outside_inset.width()+separation;
                }
                inset_y+= (final_height + separation);
            }

            out.viewBox(svg::Box(image.x(),image.y(),image.width(),image.height()+float(images.size())*(final_height+separation)));
                
    }
};

struct Config : public pattern::Reflectable<Config> {
    std::string input = "", output = "";
    Layout layout = LayoutSingleImageBottomInsets();
    std::list<std::string> images;
    
    auto reflect() { return std::tie(input,output,layout,images);}
    auto reflect_names() const { return std::tuple("input","output","layout","images"); }
};


int main(int argc, char** argv) {
    for (int i = 1; i<argc;++i)
        if (std::string(argv[i])=="--help") {
            std::cout<<"figure-inset      - figure inset generator."<<std::endl;
            std::cout<<"   Parameters:"<<std::endl;
            std::cout<<"   --input=<input> - The input svg file with an image and several rectangles that mark the insets. If omitted, it is obtained from the standard input."<<std::endl;
            std::cout<<"   --output=<output> - The output svg file with the new insets. If omitted, the standard output."<<std::endl;
            std::cout<<"   --images <image1> <image2> <image3> - The filenames of the images for the insets, added to the image in the SVG file."<<std::endl;
            std::cout<<"   --layout-type=<layout> - The distribution of the insets, one of the following:"<<std::endl;
            std::cout<<"        "<<Layout::registered()<<std::endl;
            return 0;
        }

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
    if (insets.empty()) {
        std::cerr<<"No rectangles that define the insets."<<std::endl;
        return 1;
    }

    svg::SVG out;
    svg::Image image = images.front();
    for (auto& im : images)
        im.x(image.x()).y(image.y()).width(image.width()).height(image.height());
    for (auto filename : config.images)
        if ((filename != image.href()) && (filename != (std::string("./")+image.href())))
            images.push_back(image.href(filename));

    config.layout.draw_insets(out,images,insets);  

    if (config.output.empty()) { out.save(std::cout); std::cout<<std::endl; } 
    else  { out.save(config.output); }
    return 0;
}

