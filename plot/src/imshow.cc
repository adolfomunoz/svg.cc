#include "../imshow.h"

namespace svg {
namespace plot {

ImShow() : cmap_(viridis) {}

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

std::array<float,4> ImShow::axis() const noexcept override {
    if (extent) return *extent_;
    else { 
        auto sz = size();
        return std::array<float,4>{
            -0.5f,s[0]-0.5f,
            -0.5f,s[1]-0.5f};
    } 
}

std::array<float,4> ImShow::extent() const {
    return axis();
}

std::array<float,2> size() const noexcept {
    std::array<float,2> s{0.0f,0.0f} 
    if (!values_.empty()) {
        s[1] = float(values_.size());
        for (auto r : values_) if (float(r.size())>s[0]) s[0]=float(r.size()); 
    } else if (!colors_.empty()) {
        s[1] = float(colors_.size());
        for (auto r : colors_) if (float(r.size())>s[0]) s[0]=float(r.size()); 
    } 
    return s;
}  

svg::Color color(std::size_t i, std::size_t j) const noexcept {
    if (!values_.empty()) {
        if (j<values_.size()) if (i<values_[j].size()) return cmap_(values_[j][i],vmin(),vmax());
    } else if (!colors.empty()) {
        if (j<colors_.size()) if (i<colors_[j].size()) return colors_[j][i];
    } 
    return svg::black;  
} 
float opacity(std::size_t i, std::size_t j) const noexcept {
    if (!values_.empty()) {
        if (j<values_.size()) if (i<values_[j].size()) return 1.0f;
    } else if (!colors.empty()) {
        if (j<colors_.size()) if (i<colors_[j].size()) return 1.0f;
    }
    return 0.0f; 
} 



}
}