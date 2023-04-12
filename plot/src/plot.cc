#include "../plot.h"
#include "../../src/group.h"
#include "../../src/poly.h"
#include "color.h"
#include <cctype>

namespace svg {
namespace plot {

std::vector<float> list_to_vector(const std::list<float>& l) {
    std::vector<float> v;
    for (float f : l) v.push_back(f);
    return v;
}

Plot::Plot(const std::list<float>& x, const std::list<float>& y) : x(x),y(y),markers(list_to_vector(x),list_to_vector(y)) {}
Plot::Plot(std::list<float>&& _x, const std::list<float>& y) : x(std::move(_x)), y(y),markers(list_to_vector(x),list_to_vector(y)) {}
Plot::Plot(const std::list<float>& x, std::list<float>&& _y) : x(x), y(std::move(_y)),markers(list_to_vector(x),list_to_vector(y)) {}
Plot::Plot(std::list<float>&& _x, std::list<float>&& _y) : x(std::move(_x)), y(std::move(_y)),markers(list_to_vector(x),list_to_vector(y)) {}
svg::Element Plot::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    svg::Group g;
    std::list<float> ls = linestyle();
    if ((ls.empty()) || (ls.front()!=0)) { //ls.front = 0 marks a null linestyle
        svg::Polyline output;
        std::list<float>::const_iterator ix, iy;
        for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy)
            output.add_point(xscale(*ix),yscale(*iy));
        output.fill(none).stroke_width(linewidth()).stroke(color()).opacity(alpha());
        if (!ls.empty()) {
            std::transform(ls.begin(),ls.end(),ls.begin(),[lw = linewidth()] (float f) { return lw*f; });
            output.stroke_dasharray(ls);
        }    
        g.add(output);
    }

    if (markers_set) g.add(markers.plot(xscale,yscale));
    return g;
}
std::array<float,4> Plot::axis() const noexcept {
    if (markers_set) return markers.axis();
    else {
        if (x.empty() || y.empty()) return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
        std::array<float,4> a{x.front(),x.front(),y.front(),y.front()};
        std::list<float>::const_iterator ix, iy;
        for (ix = x.begin(), iy = y.begin(); (ix != x.end()) && (iy != y.end()); ++ix, ++iy) {
            if (*ix < a[0]) a[0] = *ix;
            if (*ix > a[1]) a[1] = *ix;
            if (*iy < a[2]) a[2] = *iy;
            if (*iy > a[3]) a[3] = *iy;
        }
        return a;
    }
}

Plot& Plot::linewidth(float f) noexcept { linewidth_=f; return *this; }
float Plot::linewidth() const noexcept { return linewidth_; }
Plot& Plot::alpha(float f) noexcept { alpha_=f; return *this; }
float Plot::alpha() const noexcept { return alpha_; }
Plot& Plot::color(const svg::Color& c) noexcept { 
    color_=c; markers.c(c); return *this; 
}
Plot& Plot::color(const std::string& sc) noexcept {
    return color(color_from_string(sc));
}
Plot& Plot::color(const char* sc) noexcept {
    return color(std::string(sc));
}
const svg::Color& Plot::color() const { return color_; }
Plot& Plot::linestyle(const std::list<float>& f) noexcept { linestyle_=f; return (*this); }
Plot& Plot::linestyle(const std::string& ls) noexcept { 
    if ((ls=="solid") || (ls=="-")) return linestyle(std::list<float>());
    else if ((ls=="dotted") || (ls==":")) return linestyle(std::list{1.0f});
    else if ((ls=="dashed") || (ls=="--")) return linestyle(std::list{3.0f,1.0f});
    else if ((ls=="dashdot") || (ls=="-.")) return linestyle(std::list{5.0f,1.0f,1.0f,1.0f});
    else if (ls=="none") return linestyle(std::list{0.0f});
    else return (*this);
}

Plot& Plot::marker(const svg::Element& m) noexcept {
    markers.marker(m); markers_set=true; return (*this);
}
Plot& Plot::marker(const std::string& f) noexcept {
    markers.marker(f); markers_set=true; return (*this);
}
Plot& Plot::marker(const char* f) noexcept {
    return marker(std::string(f));
}
Plot& Plot::markersize(float s) noexcept {
    markers.s(s); markers_set=true; return (*this);
}
Plot& Plot::markeredgecolor(const svg::Color& c) noexcept { 
    markers.edgecolors(c); markers_set=true; return *this; 
}
Plot& Plot::markeredgecolor(const std::string& sc) noexcept {
    return markeredgecolor(color_from_string(sc));
}
Plot& Plot::markeredgecolor(const char* sc) noexcept {
    return markeredgecolor(std::string(sc));
}
Plot& Plot::markeredgewidth(float s) noexcept {
    markers.linewidths(s); markers_set=true; return (*this);
}
const std::list<float>& Plot::linestyle() const noexcept { return linestyle_; }


Plot& Plot::markerfacecolor(const svg::Color& c) noexcept { 
    markers.c(c); markers_set=true; return *this; 
}
Plot& Plot::markerfacecolor(const std::string& sc) noexcept {
    return markerfacecolor(color_from_string(sc));
}
Plot& Plot::markerfacecolor(const char* sc) noexcept {
    return markerfacecolor(std::string(sc));
}

Plot& Plot::fmt(const std::string& f) noexcept {
    std::string col, mar, ls;
    for (std::size_t i = 0; i<f.size();++i) {
        if ((f[i]=='o') || (f[i]=='x')) mar = f.substr(i,i+1); //This is a marker, not a color
        else if (std::islower(f[i])) col = f.substr(i,i+1);
        else if (f[i]=='-') {
            if ( ((i+1) < f.size()) && ((f[i+1]=='.') || (f[i+1]=='-')) ) { //Two characters linestyle
                ++i;
                ls = f.substr(i-1,i+1);
            } else ls = f.substr(i,i+1);
        } 
        else if (f[i]==':') ls = f.substr(i,i+1); 
        else  mar = f.substr(i-i+1); //The only thing left is marker

    }
    if (!col.empty()) color(col);
    if (!ls.empty()) linestyle(ls);
    if (!mar.empty()) {
        marker(mar).markersize(2);
        if (!col.empty()) markerfacecolor(col);
        if (ls.empty()) linestyle("none");
    }
    return (*this);
}

} 
}
