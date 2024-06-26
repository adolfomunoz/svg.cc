#pragma once
#include <svg.cc.h>
#include <iostream>
#include <list>

class LayoutBase : public pattern::SelfRegisteringReflectableBase {
public:
    static const char* type_name() { return "layout"; }
    virtual void draw_insets(svg::SVG& out, 
            const std::list<svg::Image>& images, 
            const std::list<svg::Rect>& insets,
            const std::list<std::string>& labels = {}) const = 0;
    virtual ~LayoutBase() {}
};

class Layout : public pattern::Pimpl<LayoutBase> {
public:
    using pattern::Pimpl<LayoutBase>::Pimpl;
    using pattern::Pimpl<LayoutBase>::operator=;

    void draw_insets(svg::SVG& out, 
            const std::list<svg::Image>& images, 
            const std::list<svg::Rect>& insets,
            const std::list<std::string>& lables = {}) const override {
                this->impl()->draw_insets(out,images,insets);
            }
}; 

inline float inset_stroke_width(const svg::Rect& inset) {
    float stroke_width = inset.stroke_width();
    if (inset.style().has_stroke_width()) stroke_width = inset.style().stroke_width();
    return stroke_width;
}