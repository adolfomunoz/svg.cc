#pragma once

#include "layout.h"

class LayoutSingleImageBottomInsets : public pattern::Reflectable<LayoutSingleImageBottomInsets,LayoutBase> {
    float separation = 0.02f;
public:
    LayoutSingleImageBottomInsets() {} //Needed for self registration
    auto reflect() { return std::tie(separation);}
    auto reflect_names() const { return std::tuple("separation"); }

    static const char* type_name() { return "layout-single-image-bottom-insets"; } \
    void draw_insets(svg::SVG& out, 
        const std::list<svg::Image>& images, 
        const std::list<svg::Rect>& insets,
        const std::list<std::string>& labels) const override {
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
                        .add_transform(svg::Translate(-inset.x(),-inset.y()))
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
