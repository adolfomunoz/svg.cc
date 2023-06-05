#include "../svgplot.h"
#include "../../src/group.h"
#include "../../src/rect.h"
#include "../../src/clipPath.h"
#include "../../src/line.h"
#include "../../src/text.h"
#include "../arange.h"
#include "color.h"
#include <codecvt>
#include <fstream>

namespace svg {
namespace plot {

    /****************************
     * SUBPLOTS
     ****************************/

SVGPlot::SubplotsAdjust::SubplotsAdjust() : left_(0.01), right_(0.99), bottom_(0.01), top_(0.99), wspace_(0.01), hspace_(0.01) {}
SVGPlot::SubplotsAdjust& SVGPlot::SubplotsAdjust::left(float v) { left_=v; return *this; }
SVGPlot::SubplotsAdjust& SVGPlot::SubplotsAdjust::right(float v) { right_=v; return *this; }
SVGPlot::SubplotsAdjust& SVGPlot::SubplotsAdjust::top(float v) { top_=v; return *this; }
SVGPlot::SubplotsAdjust& SVGPlot::SubplotsAdjust::bottom(float v) { bottom_=v; return *this; }
SVGPlot::SubplotsAdjust& SVGPlot::SubplotsAdjust::wspace(float v) { wspace_=v; return *this; }
SVGPlot::SubplotsAdjust& SVGPlot::SubplotsAdjust::hspace(float v) { wspace_=v; return *this; }
float SVGPlot::SubplotsAdjust::left() const { return left_; }       
float SVGPlot::SubplotsAdjust::right() const { return right_; }       
float SVGPlot::SubplotsAdjust::top() const { return top_; }       
float SVGPlot::SubplotsAdjust::bottom() const { return bottom_; }       
float SVGPlot::SubplotsAdjust::wspace() const { return wspace_; }       
float SVGPlot::SubplotsAdjust::hspace() const { return hspace_; } 

float SVGPlot::subplots_xmax(int c) const {
    float x = 0;
    for (int r = 0; r<nrows; ++r)
        if (const auto& sp = subplots_[r*ncols+c])
            x = std::max(sp->figsize()[0],x);
    return x;
}

float SVGPlot::subplots_ymax(int r) const {
    float y = 0;
    for (int c = 0; c<ncols; ++c)
        if (const auto& sp = subplots_[r*ncols+c])
            y = std::max(sp->figsize()[1],y);
    return y;
}

std::array<float,2> SVGPlot::subplot_size() const {
    float width = 0, height = 0;
    for (int r = 0; r<nrows; ++r) height += subplots_ymax(r);
    for (int c = 0; c<ncols; ++c) width += subplots_xmax(c);
    return std::array<float,2>{width*(1.0f+(ncols-1)*subplots_adjust_.wspace()/float(ncols)),height*(1.0f+(nrows-1)*subplots_adjust_.hspace()/float(nrows))};
}

bool SVGPlot::has_subplots() const {
    return (ncols>0)&&(nrows>0);

}

SVGPlot::SubplotsAdjust& SVGPlot::subplots_adjust() { return subplots_adjust_; }  

SVGPlot& SVGPlot::subplot(int nrows_, int ncols_, std::size_t index) {
    if ((nrows_!=nrows) && (ncols_!=ncols)) {
        nrows=nrows_; 
        ncols=ncols_;
        subplots_.resize(nrows*ncols);
    }
    if (!subplots_[index]) {
        subplots_[index] = std::make_unique<SVGPlot>();
        subplots_[index]->parent = this;
    }
    return *subplots_[index];
}

SVGPlot& SVGPlot::subplot(int pos) {
    return subplot(pos / 100, (pos % 100)/10, pos%10);
}

    /*****************************
     * AXIS SCALES
     *****************************/

Scale& SVGPlot::set_xscale(const Scale& s) {
    xscale_ = s; return xscale_;
}
Scale& SVGPlot::set_yscale(const Scale& s) {
    yscale_ = s; return yscale_;
}
Scale& SVGPlot::set_xscale(const std::string& s) {
    xscale_.set_type(s); return xscale_;
}
Scale& SVGPlot::set_yscale(const std::string& s) {
    yscale_.set_type(s); return yscale_;
}
const Scale& SVGPlot::xscale() const noexcept { return xscale_; }
const Scale& SVGPlot::yscale() const noexcept { return yscale_; }

SVGPlot& SVGPlot::xscale(const Scale& s) {
    set_xscale(s); return (*this);
}
SVGPlot& SVGPlot::yscale(const Scale& s) {
    set_yscale(s); return (*this);
}
SVGPlot& SVGPlot::xscale(const std::string& s) {
    set_xscale(s); return (*this);
}
SVGPlot& SVGPlot::yscale(const std::string& s) {
    set_yscale(s); return (*this);
}

    /***************
     * PLOT variants
     ****************/

Plot& SVGPlot::plot(std::list<float>&& x, std::list<float>&& y) noexcept {
    plottables.push_back(Plot(std::move(x),std::move(y)).color(next_color()));
    return plottables.back().cast_static<Plot>();
}

Plot& SVGPlot::plot(const std::list<float>& x, std::list<float>&& y) noexcept {
    plottables.push_back(Plot(x,std::move(y)).color(next_color()));
    return plottables.back().cast_static<Plot>();
}

Plot& SVGPlot::plot(std::list<float>&& x, const std::list<float>& y) noexcept {
    plottables.push_back(Plot(std::move(x),y).color(next_color()));
    return plottables.back().cast_static<Plot>();
}

Plot& SVGPlot::plot(const std::list<float>& x, const std::list<float>& y) noexcept {
    plottables.push_back(Plot(x,y).color(next_color()));
    return plottables.back().cast_static<Plot>();
}

Plot& SVGPlot::plot(std::list<float>&& y) noexcept {
    return plot(arange(y.size()),std::move(y));
}

    /***************
     * SCATTER variants
     ****************/
Scatter& SVGPlot::scatter(std::vector<float>&& x, std::vector<float>&& y) noexcept {
    plottables.push_back(Scatter(std::move(x),std::move(y)).c(next_color()));
    return plottables.back().cast_static<Scatter>();
}

Scatter& SVGPlot::scatter(const std::vector<float>& x, std::vector<float>&& y) noexcept {
    plottables.push_back(Scatter(x,std::move(y)).c(next_color()));
    return plottables.back().cast_static<Scatter>();
}

Scatter& SVGPlot::scatter(std::vector<float>&& x, const std::vector<float>& y) noexcept {
    plottables.push_back(Scatter(std::move(x),y).c(next_color()));
    return plottables.back().cast_static<Scatter>();
}

Scatter& SVGPlot::scatter(const std::vector<float>& x, const std::vector<float>& y) noexcept {
    plottables.push_back(Scatter(x,y).c(next_color()));
    return plottables.back().cast_static<Scatter>();
}

    /***************
     * IMSHOW variants
     ****************/
ImShow& SVGPlot::imshow(std::vector<std::vector<float>>&& x) noexcept {
    plottables.push_back(ImShow(x));
    return plottables.back().cast_static<ImShow>();
}
ImShow& SVGPlot::imshow(const std::vector<std::vector<float>>& x) noexcept {
    plottables.push_back(ImShow(x));
    return plottables.back().cast_static<ImShow>();
}
ImShow& SVGPlot::imshow(std::vector<std::vector<svg::Color>>&& x) noexcept {
    plottables.push_back(ImShow(x));
    return plottables.back().cast_static<ImShow>();
}
ImShow& SVGPlot::imshow(const std::vector<std::vector<svg::Color>>& x) noexcept {
    plottables.push_back(ImShow(x));
    return plottables.back().cast_static<ImShow>();
}


ImShow& SVGPlot::imshow(const std::vector<std::vector<std::array<float,1>>>& x) noexcept {
    std::vector<std::vector<float>> colors;
    for (const auto& r : x) {
        std::vector<float> vc(r.size());
        std::transform(r.begin(),r.end(),vc.begin(),[] (const std::array<float,1>& cc) { return cc[0]; });
        colors.push_back(std::move(vc));
    } 
    return imshow(std::move(colors));
}
ImShow& SVGPlot::imshow(const std::vector<std::vector<std::array<float,2>>>& x) noexcept {
    std::vector<std::vector<float>> colors;
    std::vector<std::vector<float>> opacities;
    for (const auto& r : x) {
        std::vector<float> vc(r.size());
        std::vector<float> vo(r.size());
        std::transform(r.begin(),r.end(),vc.begin(),[] (const std::array<float,2>& cc) { return cc[0]; });
        std::transform(r.begin(),r.end(),vo.begin(),[] (const std::array<float,2>& cc) { return cc[1]; });
        colors.push_back(std::move(vc));
        opacities.push_back(std::move(vo));
    } 
    return imshow(std::move(colors)).opacity(std::move(opacities));
} 
ImShow& SVGPlot::imshow(const std::vector<std::vector<std::array<float,3>>>& x) noexcept {
    std::vector<std::vector<svg::Color>> colors;
    for (const auto& r : x) {
        std::vector<svg::Color> vc(r.size());
        std::transform(r.begin(),r.end(),vc.begin(),[] (const std::array<float,3>& cc) { 
            return svg::rgb(cc[0],cc[1], cc[2]);  });
        colors.push_back(std::move(vc));
    } 
    return imshow(std::move(colors));
}
ImShow& SVGPlot::imshow(const std::vector<std::vector<std::array<float,4>>>& x) noexcept {
    std::vector<std::vector<svg::Color>> colors;
    std::vector<std::vector<float>> opacities;
    for (const auto& r : x) {
        std::vector<svg::Color> vc(r.size());
        std::vector<float> vo(r.size());
        std::transform(r.begin(),r.end(),vc.begin(),[] (const std::array<float,4>& cc) { 
            return svg::rgb(cc[0],cc[1], cc[2]);  });
        std::transform(r.begin(),r.end(),vo.begin(),[] (const std::array<float,4>& cc) { return cc[3]; });
        colors.push_back(std::move(vc));
        opacities.push_back(std::move(vo));
    } 
    return imshow(std::move(colors)).opacity(std::move(opacities));
} 

   /***************
     * GRAPH setup
     ****************/
std::array<float,2> SVGPlot::figsize() const noexcept {
    if (figsize_) return *figsize_;
    else if (has_subplots()) {
        auto marg = margin();
        auto [width,height] = subplot_size();
        return std::array<float,2>{marg[0]+marg[1]+width,
                                    marg[2]+marg[3]+height};
    } 
    else if (parent && parent->figsize_) {
        return std::array<float,2>{(*(parent->figsize_))[0]/float(parent->ncols),(*(parent->figsize_))[1]/float(parent->nrows)};
    }
    else return std::array<float,2>{640.0f,480.0f}; // Default size in matplotlib, although instead of inches we have dots and assume 100 dpi
}

std::array<float,4> axis_join(const std::array<float,4>& a, const std::array<float,4>& b) noexcept {
    return std::array<float,4>{
        std::min(a[0],b[0]),
        std::max(a[1],b[1]),
        std::min(a[2],b[2]),
        std::max(a[3],b[3])
    };
}

std::array<float,4> SVGPlot::axis() const noexcept {
    if (axis_) return *axis_;
    else if (plottables.empty()) return std::array<float,4>{0.0f,0.0f,0.0f,0.0f};
    else {
        std::array<float,4> a = plottables.front().axis();
        auto it = plottables.begin(); ++it;
        for (;it!=plottables.end();++it) a = axis_join(a,(*it).axis());
        return a;
	}
}

float SVGPlot::linewidth() const noexcept {
    return linewidth_;
}

std::vector<float> SVGPlot::xticks() const noexcept {
    if (xticks_) return *xticks_;
    else if (xticklabels_) { //If there are labels we distribute the ticks uniformly
        auto [xmin,xmax,d1,d2] = axis();
        std::vector<float> xt((*xticklabels_).size());
        float dx = (xmax-xmin)/xt.size();
        for (std::size_t i = 0;i<xt.size();++i) xt[i] = xmin + (0.5f + float(i))*dx;
        return xt;
    } else { //If we don't have any information we rely on the scales
        auto [xmin,xmax,d1,d2] = axis();
        return xscale().ticks(5,xmin,xmax);
    }
}

std::vector<float> SVGPlot::yticks() const noexcept {
   if (yticks_) return *yticks_;
    else if (yticklabels_) { //If there are labels we distribute the ticks uniformly
        auto [d1,d2,ymin,ymax] = axis();
        std::vector<float> yt((*yticklabels_).size());
        float dy = (ymax-ymin)/yt.size();
        for (std::size_t i = 0;i<yt.size();++i) yt[i] = ymin + (0.5f + float(i))*dy;
        return yt;
    } else { //If we don't have any information we rely on the scales
        auto [d1,d2,ymin,ymax] = axis();
        return yscale().ticks(5,ymin,ymax);
    }
}

std::vector<std::string> SVGPlot::xticklabels() const noexcept {
    if (xticklabels_) return *xticklabels_;
    else {
        auto xt = xticks();
        std::vector<std::string> xtl(xt.size());
        for (std::size_t i = 0; i<xtl.size(); ++i) xtl[i] = xscale().ticklabel(xt[i]);
        return xtl;
    }
}

std::vector<std::string> SVGPlot::yticklabels() const noexcept {
    if (yticklabels_) return *yticklabels_;
    else {
        auto yt = yticks();
        std::vector<std::string> ytl(yt.size());
        for (std::size_t i = 0; i<ytl.size(); ++i) ytl[i] = yscale().ticklabel(yt[i]);
        return ytl;
    }
}

svg::Color SVGPlot::next_color() noexcept {
    auto c = color_cycle[color_cycle_pos];
    color_cycle_pos = (color_cycle_pos + 1)%color_cycle.size();
    return c;
}

float SVGPlot::yticklabels_size() const noexcept {
    auto ylabs = yticklabels();
    float yticksspace = 0.0f;
    for (auto y : ylabs) {
        float yspace = float(y.size())*fontsize()*3.0f/4.0f;
        if (yspace > yticksspace) yticksspace = yspace;
    }
    return yticksspace;
}

std::array<float,4> SVGPlot::margin() const noexcept {
    if (has_subplots()) {
        std::array<float,2> s;
        if (figsize_) s = *figsize_;
        else s = subplot_size();
        float w=s[0]; float h=s[1];
        return std::array<float,4>({
            w*subplots_adjust_.left(),
            w*(1.0f-subplots_adjust_.right()),
            h*(1.0f-subplots_adjust_.top()) + (title().empty()?0.0f:title_fontsize()*1.5f+2.0f),
            h*subplots_adjust_.bottom()
        });
    } else {
        bool xt = !xticks().empty(); bool xtl = !xticklabels().empty();
        bool yt = !yticks().empty(); bool ytl = !yticklabels().empty();
        //Assuming font size of 10 but if we add fonts this needs to change

        //Right and top are affected by xticklabels and yticklabels because the text might get out of there
        return std::array<float,4>{
            0.5f*linewidth() + (xt?3.0f:0.0f) + yticklabels_size() + (ylabel().empty()?0.0f:(2.0f*label_fontsize()+1.0f)), //Left
            0.5f*linewidth() + (ytl?10.0f:0.0f), //Right
            0.5f*linewidth() + (title().empty()?0.0f:title_fontsize()*1.5f+2.0f) + (xtl?7.5f:0.0f), //Top
            0.5f*linewidth() + (yt?3.0f:0.0f) + (ytl?(fontsize()+2.0f):0.0f) + (xlabel().empty()?0.0f:(label_fontsize()+1.0f)) //Bottom
        };
    }
}

SVGPlot& SVGPlot::figsize(const std::array<float,2>& fs) noexcept {
    figsize_ = fs; return (*this);
}

SVGPlot& SVGPlot::axis(const std::array<float,4>& a) noexcept {
    axis_ = a; return (*this);
}

SVGPlot& SVGPlot::linewidth(float lw) noexcept {
    linewidth_ = lw; return (*this);
}

SVGPlot& SVGPlot::xticks(const std::vector<float>& xt) noexcept {
    xticks_ = xt; return (*this);
}

SVGPlot& SVGPlot::yticks(const std::vector<float>& yt) noexcept {
    yticks_ = yt; return (*this);
}

SVGPlot& SVGPlot::xticklabels(const std::vector<std::string>& xtl) noexcept {
    xticklabels_ = xtl; return (*this);
}

SVGPlot& SVGPlot::yticklabels(const std::vector<std::string>& ytl) noexcept {
    yticklabels_ = ytl; return (*this);
}

SVGPlot& SVGPlot::xticks(const std::vector<float>& xt, const std::vector<std::string>& xtl) noexcept {
    return xticks(xt).xticklabels(xtl);
}

SVGPlot& SVGPlot::yticks(const std::vector<float>& yt, const std::vector<std::string>& ytl) noexcept {
    return yticks(yt).yticklabels(ytl);
}

std::string_view SVGPlot::title() const noexcept { return title_; }
SVGPlot& SVGPlot::title(std::string_view l) noexcept { title_=l; return (*this); }
std::string_view SVGPlot::ylabel() const noexcept { return ylabel_; }
SVGPlot& SVGPlot::ylabel(std::string_view l) noexcept { ylabel_=l; return (*this); }
std::string_view SVGPlot::xlabel() const noexcept { return xlabel_; }
SVGPlot& SVGPlot::xlabel(std::string_view l) noexcept { xlabel_=l; return (*this); }


    /****************
     * OUTPUT
     ****************/

svg::SVG SVGPlot::svg() const noexcept {
    auto fs = figsize();
    auto mg = margin();
    svg::SVG output;
    output.viewBox(svg::Box(0.0f,0.0f,fs[0],fs[1]));
    if (has_subplots()) {
        float ypos = 0.0f; 
        for (int r = 0; r<nrows; ++r) {
            float xpos = 0.0f; 
            float height = subplots_ymax(r);
            for (int c = 0; c<ncols; ++c) 
                if (subplots_[r*ncols+c]) {
                    float width = subplots_xmax(c);
                    output.add(subplots_[r*ncols+c]->svg()).x(xpos).y(ypos).height(height).width(width);
                    xpos += width+(fs[0]-mg[0]-mg[1])*subplots_adjust_.wspace()/float(ncols);
                }
            ypos += height+(fs[1]-mg[2]-mg[3])*subplots_adjust_.hspace()/float(nrows);
        }
    } else {
        auto ax = axis();

        svg::Rect border(mg[0],mg[2],fs[0]-mg[1],fs[1]-mg[3]);
        svg::ClipPath& clip = output.add(ClipPath());
        clip.add(border);
        svg::Group& plots = output.add(svg::Group());
        plots.clip_path(svg::Url(clip.id()));
        for (const Plottable& plottable : plottables)
            plots.add(plottable.plot(xscale().transform(ax[0],ax[1],border.x(),border.x()+border.width()),
                                    yscale().transform(ax[2],ax[3],border.y()+border.height(),border.y())));
        output.add(border).fill(svg::none).stroke(svg::black).stroke_width(linewidth());

        auto xt = xticks();
        auto xtl = xticklabels();
        //We don't give the option of configuring tick appearance yet. Width = linewidth and length = 3
        const float ticklength = 3.0f;
        for (std::size_t i = 0; i<xt.size(); ++i) {
            float x = xscale().transform(xt[i],ax[0],ax[1],border.x(),border.x()+border.width());
            output.add(svg::Line(x,border.y()+border.height(),x,border.y()+border.height()+ticklength)).stroke_width(linewidth()).stroke(svg::black);
            if (i<xtl.size())
                output.add(svg::Text(xtl[i],x,border.y()+border.height()+ticklength+3.0f))
                    .font_size(fontsize()).font_family("sans-serif").dominant_baseline(svg::hanging).text_anchor(svg::middle);
        }

        auto yt = yticks();
        auto ytl = yticklabels();
        for (std::size_t i = 0; i<yt.size(); ++i) {
            float y = yscale().transform(yt[i],ax[2],ax[3],border.y()+border.height(),border.y());
            output.add(svg::Line(border.x(),y,border.x()-ticklength,y)).stroke_width(linewidth()).stroke(svg::black);
            if (i<ytl.size())
                output.add(svg::Text(ytl[i],border.x()-ticklength-1.0f,y))
                    .font_size(fontsize()).font_family("sans-serif").dominant_baseline(svg::middle).text_anchor(svg::end);
        }

        if (!title().empty()) {
            output.add(svg::Text(title(),0.5f*figsize()[0],border.y()-0.5f*title_fontsize()))
                .font_size(title_fontsize()).font_family("sans-serif").text_anchor(svg::middle);
        }

        if (!xlabel().empty()) {
            output.add(svg::Text(xlabel(),border.x()+0.5f*border.width(),border.y()+border.height()+ticklength+fontsize()+5.0f))
                .font_size(label_fontsize()).font_family("sans-serif").dominant_baseline(svg::hanging).text_anchor(svg::middle);
        }

        if (!ylabel().empty()) {
            float x = border.x() - ticklength - yticklabels_size() - 2.0f;
            float y = border.y() + 0.5f*border.height();
            output.add(svg::Text(ylabel(),x,y))
                .font_size(label_fontsize()).font_family("sans-serif").text_anchor(svg::middle)
                .add_transform(svg::Rotate(-90,x,y));
        }
    }

    return output;
}

const std::string& convert(const std::string& s) { return s; }
template<typename T>
static std::string convert(const std::basic_string<T>& s) {
    std::wstring_convert<std::codecvt_utf8<T>, T> converter;
    return converter.to_bytes(s);
}

void SVGPlot::savefig(const std::filesystem::path& name) const {
    std::filesystem::path svg_name = name;
    svg_name.replace_extension("svg");
    std::ofstream f(svg_name);
    svg().save_to_stream(f);
    f.close();
    int status = -1;
    if (name.extension() == ".png") {
        status = std::system((std::string("inkscape --export-type=\"png\" ")+convert(svg_name.native())).c_str());
    } else if (name.extension() == ".pdf") {
        status = std::system((std::string("inkscape --export-type=\"pdf\" ")+convert(svg_name.native())).c_str());
    } else if (name.extension() == ".eps") {
        status = std::system((std::string("inkscape --export-type=\"eps\" ")+convert(svg_name.native())).c_str());
    } else if (name.extension() == ".pdf_tex") {
        status = std::system((std::string("inkscape --export-type=\"pdf\" --export-latex ")+convert(svg_name.native())).c_str());
    }

    if (status == 0) std::filesystem::remove(svg_name);
}	

}
}
