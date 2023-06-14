#include "../imshow.h"
#include "../../src/group.h"
#include "../../src/rect.h"
#include <limits>

namespace svg {
namespace plot {

ImShow::ImShow(std::vector<std::vector<float>>&& v) noexcept : values_(std::move(v)) {}
ImShow::ImShow(const std::vector<std::vector<float>>& v) noexcept : values_(v) {}
ImShow::ImShow(std::vector<std::vector<svg::Color>>&& v) noexcept : colors_(std::move(v)) {}
ImShow::ImShow(const std::vector<std::vector<svg::Color>>& v) noexcept : colors_(v) {} 

ImShow& ImShow::opacity(std::vector<std::vector<float>>&& v) noexcept {
    opacities_ = std::move(v); return (*this);
}
ImShow& ImShow::opacity(const std::vector<std::vector<float>>& v) noexcept {
    opacities_ = v; return (*this);
}

ImShow& ImShow::vmin(float v) noexcept {
    vmin_=v; return (*this);
}
ImShow& ImShow::vmax(float v) noexcept {
    vmax_=v; return (*this);
}
ImShow& ImShow::cmap(const ColorMap& c) noexcept {
    cmap_=c; return (*this);
}
ImShow& ImShow::cmap(const std::string& c) noexcept {
    return cmap(ColorMap::from_name(c));
}
ImShow& ImShow::cmap(const char* c) noexcept {
    return cmap(std::string(c));
}

float ImShow::vmin() const noexcept {
    if (vmin_) return *vmin_;
    else {
        float r = std::numeric_limits<float>::max();
        for (auto row : values_) for (float v : row) if (v<r) r=v;
        return r;
    }
}
float ImShow::vmax() const noexcept {
    if (vmax_) return *vmax_;
    else {
        float r = std::numeric_limits<float>::lowest();
        for (auto row : values_) for (float v : row) if (v>r) r=v;
        return r;
    }
}

std::array<float,4> ImShow::axis() const noexcept {
    if (extent_) return *extent_;
    else { 
        auto sz = size();
        return std::array<float,4>{
            -0.5f,float(sz[0])-0.5f,
            -0.5f,float(sz[1])-0.5f};
    } 
}

std::array<float,4> ImShow::extent() const {
    return axis();
}

ImShow& ImShow::extent(const std::array<float,4> & e) noexcept {
    extent_ = e; return (*this);
}

std::array<std::size_t,2> ImShow::size() const noexcept {
    std::array<std::size_t,2> s{0,0}; 
    if (!values_.empty()) {
        s[1] = values_.size();
        for (auto r : values_) if (float(r.size())>s[0]) s[0]=r.size(); 
    } else if (!colors_.empty()) {
        s[1] = colors_.size();
        for (auto r : colors_) if (float(r.size())>s[0]) s[0]=r.size(); 
    } 
    return s;
}  

svg::Color ImShow::color(std::size_t i, std::size_t j) const noexcept {
    if (!values_.empty()) {
        if (j<values_.size()) if (i<values_[j].size()) return cmap_(values_[j][i],vmin(),vmax());
    } else if (!colors_.empty()) {
        if (j<colors_.size()) if (i<colors_[j].size()) return colors_[j][i];
    } 
    return svg::black;  
} 

float ImShow::opacity(std::size_t i, std::size_t j) const noexcept {
    if (!values_.empty()) {
        if (j<values_.size()) if (i<values_[j].size()) {
            if (j<opacities_.size()) if (i<opacities_[j].size()) return opacities_[j][i];  
            return 1.0f;
        } 
    } else if (!colors_.empty()) {
        if (j<colors_.size()) if (i<colors_[j].size()) {
            if (j<opacities_.size()) if (i<opacities_[j].size()) return opacities_[j][i];  
            return 1.0f;
        } 
    }
    return 0.0f; 
} 

ImShow& ImShow::interpolation(const Interpolation& i) noexcept {
    interpolation_ = i; return (*this);
} 
ImShow& ImShow::interpolation(const std::string& i) noexcept {
    interpolation_.set_type(i); return (*this);
} 
ImShow& ImShow::interpolation(const char* i) noexcept {
    return interpolation(std::string(i));
} 
svg::Element ImShow::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    return interpolation_.plot(*this,xscale,yscale);
} 


InterpolationNearest nearest;
svg::Element InterpolationNearest::plot(const ImShow& imshow, const Transform& xscale, const Transform& yscale) const noexcept {
    auto size = imshow.size();
    auto extent = imshow.extent();
    float dx = (extent[1]-extent[0])/float(size[0]);
    float dy = (extent[3]-extent[2])/float(size[1]);
    svg::Group group;
    for (std::size_t i = 0; i<size[0]; ++i) {
        float x = extent[0]+float(i)*dx; 
        for (std::size_t j = 0; j<size[1]; ++j) {
            float y = extent[2]+float(j)*dy;
            float op = imshow.opacity(i,j);
            if (op>0.0f) {
                group.add(svg::Rect(xscale(x),yscale(y),xscale(x+dx),yscale(y+dy)))
                    .stroke_width(0)
                    .fill(imshow.color(i,j))
                    .fill_opacity(op);
            } 
        } 
    } 
    return group;     
} 



}
}