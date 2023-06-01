#pragma once

#include <optional>
#include "plottable.h"
#include "colormap.h"

namespace svg {
namespace plot {

class ImShow;
class InterpolationBase : public pattern::SelfRegisteringReflectableBase {
public:
    virtual svg::Element plot(const ImShow& imshow, const Transform& xscale, const Transform& yscale) const noexcept = 0;
};

class Interpolation : public pattern::Pimpl<InterpolationBase> {
public:
    using pattern::Pimpl<InterpolationBase>::Pimpl;
    using pattern::Pimpl<InterpolationBase>::operator=;
    
    svg::Element plot(const ImShow& imshow, const Transform& xscale, const Transform& yscale) const noexcept override {
        return impl()->plot(imshow,xscale,yscale);
    }
};

class InterpolationNearest : public pattern::Reflectable<InterpolationNearest, InterpolationBase> {
public:
    InterpolationNearest(){} 
    static const char* type_name() { return "nearest"; } 
    svg::Element plot(const ImShow& imshow, const Transform& xscale, const Transform& yscale) const noexcept override;
};
extern InterpolationNearest nearest;


class ImShow : public PlottableBase {
    std::vector<std::vector<svg::Color>> colors_;
    std::vector<std::vector<float>> values_;

	ColorMap cmap_ = viridis;
	std::optional<float> vmin_, vmax_; 
    std::optional<std::array<float,4>>  extent_;
    
    Interpolation interpolation_ = nearest;
public:
    ImShow(std::vector<std::vector<float>>&& v) noexcept;
    ImShow(const std::vector<std::vector<float>>& v) noexcept;
    ImShow(std::vector<std::vector<svg::Color>>&& v) noexcept;
    ImShow(const std::vector<std::vector<svg::Color>>& v) noexcept;
	ImShow& vmin(float f) noexcept; 
	ImShow& vmax(float f) noexcept; 
    ImShow& extent(const std::array<float,4> & e) noexcept; 
    ImShow& cmap(const ColorMap& c) noexcept; 
    ImShow& cmap(const std::string& c) noexcept;
    ImShow& cmap(const char* c) noexcept;  
    ImShow& interpolation(const Interpolation& i) noexcept;
    ImShow& interpolation(const std::string& i) noexcept;
    ImShow& interpolation(const char* i) noexcept;
    float vmin() const noexcept;
    float vmax() const noexcept;
    svg::Color color(std::size_t i, std::size_t j) const noexcept;
    float opacity(std::size_t i, std::size_t j) const noexcept;
    std::array<std::size_t,2> size() const noexcept;
	std::array<float,4> axis() const noexcept override;
    std::array<float,4> extent() const;
    svg::Element plot(const Transform& xscale, const Transform& yscale) const noexcept override;
};


/*
template<typename T>
class ImShowType : public ImShow {
    std::vector<std::vector<T>> data;

public:
    ImShowType(const std::vector<std::vector<T>>& data) : data(data) {}
    ImShowType(std::vector<std::vector<T>>&& data) : data(std::forward<std::vector<std::vector<T>>>(data)) {}
protected:
    float calculated_vmin() const override {
        float temp_vmin = detail::vmin_element(data[0][0]);
        for (const auto& row : data) for (const auto& value : row) {
            float v = detail::vmin_element(value);
            if (v < temp_vmin) temp_vmin = v;
        }
        return temp_vmin;       
    }
    
    float calculated_vmax() const override {
        float temp_vmax = detail::vmax_element(data[0][0]);
        for (const auto& row : data) for (const auto& value : row) {
            float v = detail::vmax_element(value);
            if (v > temp_vmax) temp_vmax = v;
        }
        return temp_vmax;         
    }
public:
    std::tuple<std::size_t,std::size_t> size() const override {
		std::size_t x(0);
		for (const auto& r : data) if (r.size()>x) x=r.size();
		return std::tuple<std::size_t,std::size_t>(x,data.size());        
    }
    
private:
    std::shared_ptr<_2d::Element> nearest(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept {
        auto ax = axis();
        auto cm = detail::colormap(cmap(),vmin(),vmax());
        auto image = std::make_shared<_2d::Group>();
        const float border_expansion=0;
		image->stroke_width(0);
        std::tuple<float,float> xmin{ax[0],ax[2]};
        std::tuple<float,float> xmax{ax[1],ax[3]};
        auto nsamples = size();
		float dx = (std::get<0>(xmax)-std::get<0>(xmin))/float(std::get<0>(nsamples));
		float dy = (std::get<1>(xmax)-std::get<1>(xmin))/float(std::get<1>(nsamples));
		float x, y;
		const float eps = border_expansion*dx;
        
        y = std::get<1>(xmin);
        for (const auto& row : data) {
            x = std::get<0>(xmin);
            for (const auto& v : row) {
                detail::set_color(image->add(_2d::rect({xscale.transform(x-eps),yscale.transform(y-eps)},
                    {xscale.transform(x+dx+eps),yscale.transform(y+dy+eps)})).stroke_width(0),cm,v);
                x+=dx;
            }
            y+=dy;
        }

        return image;
    }
    
    std::shared_ptr<_2d::Element> interpolated(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept {
        #ifndef USE_PNG
        return nearest(xscale,yscale);
        #else
        auto ax = axis();
        auto cm = detail::colormap(cmap(),vmin(),vmax());
        auto nsamples = size();
		PNG png(std::get<0>(nsamples),std::get<1>(nsamples));
		unsigned int i, j = 0;
        for (const auto& row : data) {
            i=0;
            for (const auto& v : row) {
                png.set_pixel(i,j,detail::color_of(v,cm)); ++i; 
            }
            ++j;
        }
		png.save("tmp.png");

		std::ifstream stream("tmp.png", std::ios::in | std::ios::binary);
		std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
		std::stringstream ss;
		ss << "data:image/png;base64,"<<base64_encode(contents.data(),contents.size());
        std::tuple<float,float> xmin{xscale.transform(ax[0]),yscale.transform(ax[2])};
        std::tuple<float,float> xmax{xscale.transform(ax[1]),yscale.transform(ax[3])};
		Image image(ss.str()); image.rect(xmin,xmax).preserveAspectRatio(preserve_aspect_ratio_none);
        return std::make_shared<_2d::primitive<Image>>(image);
        #endif
    }
    
    std::shared_ptr<_2d::Element> scaled(const axis_scale::Base& xscale, const axis_scale::Base& yscale) const noexcept override {
        if ((interpolation()=="") || (interpolation()=="nearest") ||(interpolation()=="none")) 
            return nearest(xscale,yscale);
        else
            return interpolated(xscale,yscale);
    }
 
};
*/


	
}
}