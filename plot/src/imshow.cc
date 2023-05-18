#include "../imshow.h"

namespace svg {
namespace plot {

ImShow() : cmap_(viridis) {}

ImShow& ImShow::vmin(float f) noexcept { vmin_=f; return (*this);}
ImShow& ImShow::vmax(float f) noexcept { vmax_=f; return (*this);}
ImShow& ImShow::extent(const std::array<float,4> & e) noexcept { extent_=e; return (*this); }
ImShow& ImShow::cmap(const ColorMap& c) noexcept { cmap_=c; return (*this); }
ImShow& ImShow::cmap(const std::string& c) noexcept { return cmap(ColorMap::from_name(c)); }
ImShow& ImShow::cmap(const char* c) noexcept { return cmap(std::string(c)); }      
}
}