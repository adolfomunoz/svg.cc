#include "layout.h"
#include "layout-single-image-bottom-insets.h"


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

