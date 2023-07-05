#pragma once
#include "axis-scale.h"
#include "plottable.h"
#include "plot.h"
#include "scatter.h"
#include "imshow.h"
#include "bar.h"
#include "linspace.h"
#include "../src/svg.h"

#include <list>
#include <vector>
#include <array>
#include <optional>
#include <filesystem>
#include <algorithm>

namespace svg {
namespace plot {

class SVGPlot {
    /**********************************
     * For subplots
     **********************************/
private:
    std::vector<std::unique_ptr<SVGPlot>> subplots_;
    int nrows = 0, ncols = 0;

    class SubplotsAdjust {
        float left_, right_, bottom_, top_, wspace_, hspace_;
    public:
        SubplotsAdjust();
        SubplotsAdjust& left(float v);
        SubplotsAdjust& right(float v);
        SubplotsAdjust& top(float v);
        SubplotsAdjust& bottom(float v);
        SubplotsAdjust& wspace(float v);
        SubplotsAdjust& hspace(float v);
        float left() const;
        float right() const;   
        float top() const;   
        float bottom() const;     
        float wspace() const; 
        float hspace() const;       
    } subplots_adjust_;
    SVGPlot* parent = nullptr;

private:
    float subplots_xmax(int c) const;
    float subplots_ymax(int r) const;
    std::array<float,2> subplot_size() const;
    bool has_subplots() const;
public:
    SubplotsAdjust& subplots_adjust();  
    SVGPlot& subplot(int nrows_, int ncols_, std::size_t index);
    SVGPlot& subplot(int pos);
    /**********************************
     * For plotting here
     **********************************/ 
private:
     
    std::list<Plottable> plottables;
    /*****************************
     * AXIS SCALES
     *****************************/
    Scale xscale_ = scale::linear();
    Scale yscale_ = scale::linear();

public:
    Scale& set_xscale(const std::string& s);
    Scale& set_yscale(const std::string& s);
    Scale& set_xscale(const Scale& s);
    Scale& set_yscale(const Scale& s);
    SVGPlot& xscale(const std::string& s);
    SVGPlot& yscale(const std::string& s);
    SVGPlot& xscale(const Scale& s);
    SVGPlot& yscale(const Scale& s);
    const Scale& xscale() const noexcept;
    const Scale& yscale() const noexcept;

    /***************
     * PLOT variants
     ****************/
    Plot& plot(std::list<float>&& x, std::list<float>&& y) noexcept;
    Plot& plot(const std::list<float>& x, std::list<float>&& y) noexcept;
    Plot& plot(std::list<float>&& x, const std::list<float>& y) noexcept;
    Plot& plot(const std::list<float>& x, const std::list<float>& y) noexcept;

    Plot& plot(std::list<float>&& y) noexcept;
    
    template<typename CollectionX, typename CollectionY>
    Plot& plot(CollectionX&& x, CollectionY&& y, const std::string& fmt = "", std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<CollectionX>::value_type> && std::is_arithmetic_v<typename std::decay_t<CollectionY>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->plot(std::list<float>(x.begin(),x.end()),std::list<float>(y.begin(),y.end())).fmt(fmt);    
    }

    template<typename Collection>
    Plot& plot(Collection&& y, const std::string& fmt = "", std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->plot(arange(y.size()),std::forward<Collection>(y)).fmt(fmt);    
    }

    template<typename Collection, typename Function>
    Plot& plot(Collection&& x, const Function& f, const std::string& fmt = "", std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type> && std::is_arithmetic_v<decltype(std::declval<Function>()(0.0f))>,void*> sfinae = nullptr) noexcept {
        std::list<float> y;
        for (auto ex : x) y.push_back(f(ex));
        return this->plot(std::forward<Collection>(x),std::move(y)).fmt(fmt);    
    }


    /***************
     * SCATTER variants
     ****************/
    Scatter& scatter(std::vector<float>&& x, std::vector<float>&& y) noexcept;
    Scatter& scatter(const std::vector<float>& x, std::vector<float>&& y) noexcept;
    Scatter& scatter(std::vector<float>&& x, const std::vector<float>& y) noexcept;
    Scatter& scatter(const std::vector<float>& x, const std::vector<float>& y) noexcept;

    template<typename CollectionX, typename CollectionY>
    Scatter& scatter(CollectionX&& x, CollectionY&& y, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<CollectionX>::value_type> && std::is_arithmetic_v<typename std::decay_t<CollectionY>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->scatter(std::vector<float>(x.begin(),x.end()),std::vector<float>(y.begin(),y.end()));    
    }

    /***************
     * IMSHOW variants
     ****************/
    ImShow& imshow(std::vector<std::vector<float>>&& x) noexcept;
    ImShow& imshow(const std::vector<std::vector<float>>& x) noexcept;
    ImShow& imshow(std::vector<std::vector<svg::Color>>&& x) noexcept;
    ImShow& imshow(const std::vector<std::vector<svg::Color>>& x) noexcept;

    ImShow& imshow(const std::vector<std::vector<std::array<float,1>>>& x) noexcept;
    ImShow& imshow(const std::vector<std::vector<std::array<float,2>>>& x) noexcept;
    ImShow& imshow(const std::vector<std::vector<std::array<float,3>>>& x) noexcept;
    ImShow& imshow(const std::vector<std::vector<std::array<float,4>>>& x) noexcept;
        
    template<typename Collection>
    ImShow& imshow(const Collection& x, 
        std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type::value_type> ,void*> sfinae = nullptr) noexcept {
        std::vector<std::vector<float>> vx;
        for (const auto& r : x) vx.push_back(std::vector<float>(r.begin(),r.end()));
        return this->imshow(std::move(vx));    
    }
    template<typename Collection>
    ImShow& imshow(const Collection& x, 
        std::enable_if_t<std::is_base_of_v<ColorBase,typename std::decay_t<Collection>::value_type::value_type> ,void*> sfinae = nullptr) noexcept {
        std::vector<std::vector<svg::Color>> vx;
        for (const auto& r : x) vx.push_back(std::vector<svg::Color>(r.begin(),r.end()));
        return this->imshow(std::move(vx));    
    }
    template<typename Collection>
    ImShow& imshow(const Collection& x, 
        std::enable_if_t<std::is_same_v<std::string,typename std::decay_t<Collection>::value_type::value_type> ,void*> sfinae = nullptr) noexcept {
    
        std::vector<std::vector<svg::Color>> colors;
        for (const auto& r : x) {
            std::vector<svg::Color> vc(r.size());
            std::transform(r.begin(),r.end(),vc.begin(),[] (const std::string& cc) { return color_from_string(cc); });
            colors.push_back(std::move(vc));
        } 
        return imshow(std::move(colors));    
    }
    template<typename Collection>
    ImShow& imshow(const Collection& x, 
        std::enable_if_t<std::is_same_v<const char*,typename std::decay_t<Collection>::value_type::value_type> ,void*> sfinae = nullptr) noexcept {
    
        std::vector<std::vector<svg::Color>> colors;
        for (const auto& r : x) {
            std::vector<svg::Color> vc(r.size());
            std::transform(r.begin(),r.end(),vc.begin(),[] (const char* cc) { return color_from_string(std::string(cc)); });
            colors.push_back(std::move(vc));
        } 
        return imshow(std::move(colors));    
    }
    template<typename Collection>
    ImShow& imshow(const Collection& x, 
        std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<Collection>::value_type::value_type::value_type> ,void*> sfinae = nullptr) noexcept {
        std::vector<std::vector<Color>> colors;
        std::vector<std::vector<float>> opacities;
        for (const auto& r : x) { 
            std::vector<Color> vc;
            std::vector<float> vo;
            for (const auto& v : r) {
                if (v.size()==1) {
                    vc.push_back(svg::rgb(255.0f*v.front(),255.0f*v.front(),255.0f*v.front()));
                    vo.push_back(1.0f);
                } else if (v.size()==2) {
                    vc.push_back(svg::rgb(255.0f*v.front(),255.0f*v.front(),255.0f*v.front()));
                    vo.push_back(v.back());
                } else if (v.size()==3) {
                    auto i = v.begin();
                    vc.push_back(svg::rgb(255.0f*(*(i++)),255.0f*(*(i++)),255.0f*(*(i++))));
                    vo.push_back(1.0f);
                } else if (v.size()>=4) {
                    auto i = v.begin();
                    vc.push_back(svg::rgb(255.0f*(*(i++)),255.0f*(*(i++)),255.0f*(*(i++))));
                    vo.push_back(*(i++));
                } else {
                    vc.push_back(svg::black);
                    vo.push_back(0.0f);
                }  
            }
            colors.push_back(vc);
            opacities.push_back(vo);
        } 
        return this->imshow(std::move(colors)).opacity(std::move(opacities));    
    }
    template<typename CollectionX, typename CollectionY, typename Function>
    ImShow& imshow(const CollectionX& xs,const CollectionY& ys, const Function& f,
        std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<CollectionX>::value_type> &&
                         std::is_arithmetic_v<typename std::decay_t<CollectionX>::value_type>,void*> sfinae = nullptr) noexcept {
        
        using T = std::decay_t<decltype(f(std::declval<typename CollectionX::value_type>(),std::declval<typename CollectionY::value_type>()))>;
        std::vector<std::vector<T>> data;
        for (auto y : ys) {
            data.push_back(std::vector<T>());
            for (auto x : xs) data.back().push_back(f(x,y));
        }
        float dx = (xs.back() - xs.front())/float(xs.size());
        float dy = (ys.back() - ys.front())/float(ys.size());
        return imshow(std::move(data)).extent({xs.front()-0.5f*dx, xs.back()+0.5f*dx, ys.front()-0.5f*dy, ys.back() + 0.5f*dy});   
    }

    /***************
     * BAR variants
     ****************/
    Bar& bar(std::vector<float>&& x, std::vector<float>&& y) noexcept;
    Bar& bar(const std::vector<float>& x, std::vector<float>&& y) noexcept;
    Bar& bar(std::vector<float>&& x, const std::vector<float>& y) noexcept;
    Bar& bar(const std::vector<float>& x, const std::vector<float>& y) noexcept;

    template<typename CollectionX, typename CollectionY>
    Bar& bar(CollectionX&& x, CollectionY&& y, std::enable_if_t<std::is_arithmetic_v<typename std::decay_t<CollectionX>::value_type> && std::is_arithmetic_v<typename std::decay_t<CollectionY>::value_type>,void*> sfinae = nullptr) noexcept {
        return this->bar(std::vector<float>(x.begin(),x.end()),std::vector<float>(y.begin(),y.end()));    
    }

    /***************
     * GRAPH setup
     ****************/
private:
    std::optional<std::array<float,2>> figsize_;
    std::optional<std::array<float,4>> axis_;
    std::optional<std::vector<float>> xticks_, yticks_;
    std::optional<std::vector<std::string>> xticklabels_, yticklabels_;
    std::string ylabel_, xlabel_, title_;

    float linewidth_ = 1;
    std::vector<svg::Color> color_cycle{
        svg::ColorHex("#1f77b4"),svg::ColorHex("#ff7f0e"),svg::ColorHex("#2ca02c"),
        svg::ColorHex("#d62728"),svg::ColorHex("#9467bd"),svg::ColorHex("#8c564b"),
        svg::ColorHex("#e377c2"),svg::ColorHex("#7f7f7f"),svg::ColorHex("#bcbd22"),
        svg::ColorHex("#17becf")}; 
    std::size_t color_cycle_pos{0};
    svg::Color next_color() noexcept;

    std::array<float,2> figsize() const noexcept;
    std::array<float,4> axis() const noexcept;
    std::vector<float> xticks() const noexcept;
    std::vector<float> yticks() const noexcept;
    std::vector<std::string> xticklabels() const noexcept;
    std::vector<std::string> yticklabels() const noexcept;
    float linewidth() const noexcept;

    constexpr float fontsize() const noexcept { return 10.0; }
    constexpr float title_fontsize() const noexcept { return 2.4*fontsize(); }
    constexpr float label_fontsize() const noexcept { return 1.6*fontsize(); }
    float yticklabels_size() const noexcept;
    std::array<float,4> margin() const noexcept;
public:
    SVGPlot& figsize(const std::array<float,2>& fs) noexcept;
    SVGPlot& axis(const std::array<float,4>& a) noexcept;
    SVGPlot& linewidth(float lw) noexcept;
    SVGPlot& xticks(const std::vector<float>& xt) noexcept;
    SVGPlot& yticks(const std::vector<float>& yt) noexcept;
    SVGPlot& xticks(const std::vector<float>& xt, const std::vector<std::string>& xtl) noexcept;
    SVGPlot& yticks(const std::vector<float>& yt, const std::vector<std::string>& ytl) noexcept;
    SVGPlot& xticklabels(const std::vector<std::string>& xtl) noexcept;
    SVGPlot& yticklabels(const std::vector<std::string>& ytl) noexcept;
    std::string_view title() const noexcept;
	SVGPlot& title(std::string_view l) noexcept;
	std::string_view ylabel() const noexcept;
	SVGPlot& ylabel(std::string_view l) noexcept;
	std::string_view xlabel() const noexcept;
	SVGPlot& xlabel(std::string_view l) noexcept;

    /****************
     * OUTPUT
     ****************/
private:
    svg::SVG svg() const noexcept;
public:
    void savefig(const std::filesystem::path& name) const;
};

}
}
