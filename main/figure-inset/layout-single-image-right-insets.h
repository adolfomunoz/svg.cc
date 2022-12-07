#pragma once

#include "layout.h"

class LayoutSingleImageRightInsets : public pattern::Reflectable<LayoutSingleImageRightInsets,LayoutBase> {
    float separation = 0.02f;
    float label_relative_size = 0.95f;
public:
    LayoutSingleImageRightInsets() {} //Needed for self registration
    auto reflect() { return std::tie(separation,label_relative_size);}
    auto reflect_names() const { return std::tuple("separation","label-relative-size"); }

    static const char* type_name() { return "layout-single-image-right-insets"; }

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
            for (const svg::Rect& inset : insets) ar += inset.height()/inset.width();
            float inset_size = (image.height()-separation*float(insets.size()-1)-inset_stroke_width(insets.front()));
            float final_width = inset_size/ar;
            float inset_x = image.x() + image.width() + separation;
            float label_font_size = label_relative_size*final_width*4.0f/3.0f;
            auto label_it = labels.begin();
            for (const svg::Image& image : images) {
                float inset_y = image.y();
                int inset_id = 0;
                if (label_it != labels.end()) {
                    svg::Text label(*label_it);
                    label
                        .x(inset_x + 0.5f*inset_size)
                        .y(inset_y - separation)
                        .font_size(label_font_size)
                        .font_family("sans-serif")
                        .text_anchor(svg::middle);
                    out.add(label);
                }
                for (const svg::Rect& inset : insets) {
                    svg::Rect outside_inset = inset;
                    float size_factor = final_width/inset.width();
                    float stroke_width = inset_stroke_width(inset);

                    outside_inset
                        .x(inset_x - 0.5f*stroke_width)
                        .y(inset_y + 0.5f*stroke_width)
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
                    inset_y+=outside_inset.height()+separation;
                }
                inset_x+= (final_width + separation);
            }

            out.viewBox(svg::Box(image.x(),image.y()-(labels.empty()?0.0f:(separation + label_font_size)),image.width()+float(images.size())*(final_width+separation),image.height()));
                
    }
};
