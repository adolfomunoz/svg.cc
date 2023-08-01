#include "../hist.h"
#include "../bar.h"
#include "../barh.h"
#include "../plot.h"
#include <algorithm>

namespace svg {
namespace plot {

Hist::Hist(const std::vector<float>& x) : x_(x) {}
Hist::Hist(std::vector<float>&& x) : x_(std::move(x)) {}

svg::Element Hist::plot(const Transform& xscale, const Transform& yscale) const noexcept {
    return histtype().representation(*this).plot(xscale,yscale);
}

std::array<float,4> Hist::axis() const noexcept {
    return histtype().representation(*this).axis();
}

std::tuple<float,float> Hist::range() const noexcept {
    if (range_) return *range_;
    else if (!bins_.empty()) return std::tuple<float,float>(bins_.front(),bins_.back());
    else return std::tuple<float,float>(*std::min_element(x_.begin(),x_.end()),*std::max_element(x_.begin(),x_.end()));
}
    
Hist& Hist::range(const std::tuple<float,float> r) noexcept {
    range_=r; return *this;
}
    
Hist& Hist::weights(const std::vector<float>& w) noexcept { 
    weights_=w; return *this; 
}
           
std::size_t Hist::bins_size() const noexcept {
    if (!bins_.empty()) return bins_.size()-1;
    else return bins_value;
}

float Hist::bin(std::size_t i) const noexcept {
    if (!bins_.empty()) return bins_[i];
    else {
        auto [xmin,xmax] = range();
        return xmin+float(i)*(xmax-xmin)/float(bins_size());
    }
}     
    
float Hist::weight_(int i) const noexcept {
    return ((i<0) || (i>=int(weights_.size())))?1.0f:weights_[i];
}
    
Hist& Hist::density(bool b) noexcept { density_ = b; return *this;}
Hist& Hist::cumulative(bool c) noexcept { cumulative_=c; return *this; }
    
Hist& Hist::bins(std::size_t b) noexcept { bins_value=b; return *this;}
Hist& Hist::bins(int b) noexcept { return bins(std::size_t(b)); }
Hist& Hist::bins(const std::vector<float>& b) noexcept { bins_=b; return *this; }
    
std::vector<float> Hist::hist_values() const noexcept {
    std::vector<float> hist(bins_size(),0.0f);
    float w = 0.0f;
    for (std::size_t ix=0; ix<x_.size(); ++ix) {
        w += weight_(ix);
        for (std::size_t ib = 0; ib<(bins_size()-1); ++ib) {
            if ((x_[ix] >= bin(ib)) && (x_[ix] < bin(ib+1)))
                hist[ib]+=weight_(ix);
        }
        if ((x_[ix] >= bin(bins_size()-1)) && (x_[ix] <= bin(bins_size())))
                hist[bins_size()-1]+=weight_(ix);
    }
    if (density_) { //We need to account for the size of the bin as well
        for (std::size_t ib = 0; ib<bins_size(); ++ib) 
            hist[ib] /= w*(bin(ib+1)-bin(ib));
    }
    if (cumulative_) {
        float c = 0.0f;
        for (auto& h : hist) { c += h; h = c;  }
        if (density_) for (auto& h : hist) h /= hist[bins_size()-1]; 
    }
    return hist;
}
    
std::vector<float> Hist::hist_positions() const noexcept {
    std::vector<float> hist(bins_size(),0.0f);
    for (std::size_t ib = 0; ib<bins_size(); ++ib)
        hist[ib]=0.5*(bin(ib)+bin(ib+1));
    return hist;
}

std::vector<float> Hist::hist_widths() const noexcept {
    std::vector<float> hist(bins_size(),0.0f);
    for (std::size_t ib = 0; ib<bins_size(); ++ib)
        hist[ib]=bin(ib+1)-bin(ib);
    return hist;
}  

Hist& Hist::alpha(float f) noexcept { alpha_=f; return *this; }
float Hist::alpha() const noexcept { return alpha_; }

Hist& Hist::color(const svg::Color& c) noexcept { 
    color_=c; return *this; 
}
Hist& Hist::color(const std::string& sc) noexcept {
    return color(color_from_string(sc));
}
Hist& Hist::color(const char* sc) noexcept {
    return color(std::string(sc));
}
const svg::Color& Hist::color() const { return color_; }
Hist& Hist::orientation(const Orientation& o) noexcept {
    orientation_=o; return (*this);
}
const Orientation& Hist::orientation() const noexcept { return orientation_; }
Hist& Hist::histtype(const HistType& h) noexcept {
    histtype_=h; return (*this);
}
const HistType& Hist::histtype() const noexcept { return histtype_; }

Plottable HistTypeBar::representation(const Hist& hist) const noexcept {
    if (hist.orientation().is_horizontal()) {
        return Bar(hist.hist_positions(),hist.hist_values())
            .width(hist.hist_widths()).color(hist.color()).alpha(hist.alpha());
    } else {
        return BarH(hist.hist_positions(),hist.hist_values())
            .height(hist.hist_widths()).color(hist.color()).alpha(hist.alpha());
    }
}

Plottable HistTypeStep::representation(const Hist& hist) const noexcept {
    auto posv = hist.hist_positions();
    auto valv = hist.hist_values();
    std::list<float> pos(posv.begin(),posv.end());
    std::list<float> val(valv.begin(),valv.end());
    if (hist.orientation().is_horizontal()) {
        return Plot(pos,val).color(hist.color()).alpha(hist.alpha());
    } else {
        return Plot(val,pos).color(hist.color()).alpha(hist.alpha());
    }
}

OrientationHorizontal horizontal;
OrientationVertical vertical;
HistTypeBar bar;
HistTypeStep step;

}
}