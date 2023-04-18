#pragma once
#include <vector>
#include "src/color.h"
#include <string>
#include <map>


namespace svg {
namespace plot {

class ColorMap {
	std::vector<svg::Color> steps;
    static std::map<std::string,ColorMap*> registered;
public:
	ColorMap(const std::vector<svg::Color>& s, const std::string& name = "");
    ColorMap(const std::vector<std::string>& s, const std::string& name = "");	
	svg::ColorRGB operator()(float t, float tmin = 0.0f, float tmax = 1.0f) const noexcept;
    static const ColorMap& from_name(const std::string& name) noexcept;
};

inline ColorMap viridis({
    svg::rgb(68.0f,1.0f,84.0f),
	svg::rgb(65.0f,68.0f,135.0f),
    svg::rgb(42.0f,120.0f,142.0f),
	svg::rgb(34.0f,168.0f,132.0f),
	svg::rgb(122.0f,209.0f,81.0f),
	svg::rgb(253.0f,231.0f,37.0f)
},"viridis");

inline ColorMap magma({
    255.0f*svg::rgb(0.000f,0.000f,0.016f),
	255.0f*svg::rgb(0.173f,0.067f,0.373f),
	255.0f*svg::rgb(0.447f,0.122f,0.506f),
	255.0f*svg::rgb(0.718f,0.216f,0.475f),
	255.0f*svg::rgb(0.945f,0.376f,0.365f),
	255.0f*svg::rgb(0.996f,0.682f,0.467f),
	255.0f*svg::rgb(0.988f,0.992f,0.749f)
},"magma");

inline ColorMap inferno({
    255.0f*svg::rgb(0.003921568627451,0,0.015686274509804),
    255.0f*svg::rgb(0.188235294117647,0.027450980392157,0.329411764705882),
    255.0f*svg::rgb(0.411764705882353,0.058823529411765,0.435294117647059),
    255.0f*svg::rgb(0.619607843137255,0.156862745098039,0.392156862745098),
    255.0f*svg::rgb(0.815686274509804,0.282352941176471,0.262745098039216),
    255.0f*svg::rgb(0.937254901960784,0.490196078431373,0.082352941176471),
    255.0f*svg::rgb(0.949019607843137,0.76078431372549,0.137254901960784),
    255.0f*svg::rgb(0.96078431372549,1,0.63921568627451)
},"inferno");

inline ColorMap plasma({
    255.0f*svg::rgb(0.184313725490196,0,0.529411764705882),
    255.0f*svg::rgb(0.384313725490196,0,0.643137254901961),
    255.0f*svg::rgb(0.572549019607843,0,0.650980392156863),
    255.0f*svg::rgb(0.729411764705882,0.184313725490196,0.541176470588235),
    255.0f*svg::rgb(0.847058823529412,0.356862745098039,0.411764705882353),
    255.0f*svg::rgb(0.933333333333333,0.537254901960784,0.286274509803922),
    255.0f*svg::rgb(0.964705882352941,0.741176470588235,0.152941176470588),
    255.0f*svg::rgb(0.894117647058824,0.980392156862745,0.082352941176471)
},"plasma");

inline ColorMap bwr({svg::blue,svg::white,svg::red},"bwr");
inline ColorMap coolwarm({
    255.0f*svg::rgb(0.230f, 0.299f, 0.754f),
    255.0f*svg::rgb(0.865f, 0.865f, 0.865f),
    255.0f*svg::rgb(0.706f, 0.016f, 0.150f) },"coolwarm");
inline ColorMap seismic({
    255.0f*svg::rgb(0.0f,0.0f,0.1f),
    255.0f*svg::rgb(0.0f,0.0f,1.0f),
    255.0f*svg::rgb(1.0f,1.0f,1.0f),
    255.0f*svg::rgb(1.0f,0.0f,0.0f),
    255.0f*svg::rgb(0.1f,0.0f,0.0f)},"seismic");
inline ColorMap spectral({
    255.0f*svg::rgb(1.0f,0.25f,0.25f),
    255.0f*svg::rgb(1.0f,0.75f,0.25f),
    255.0f*svg::rgb(1.0f,1.0f,0.75f),
    255.0f*svg::rgb(0.25f,0.75f,0.5f),
    255.0f*svg::rgb(0.5f,0.25f,0.75f)},"spectral");
inline ColorMap piyg({
    255.0f*svg::rgb(0.8f,0.3f,0.6f),
    255.0f*svg::rgb(0.9f,0.5f,0.9f),
    255.0f*svg::rgb(1.0f,1.0f,1.0f),
    255.0f*svg::rgb(0.5f,0.75f,0.0f),
    255.0f*svg::rgb(0.0f,0.5f,0.0f)
},"piyg");
inline ColorMap grayscale({svg::black,svg::white},"grayscale");

}
}
