#pragma once
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

inline float inset_stroke_width(const svg::Rect& inset) {
    float stroke_width = inset.stroke_width();
    if (inset.style().has_stroke_width()) stroke_width = inset.style().stroke_width();
    return stroke_width;
}