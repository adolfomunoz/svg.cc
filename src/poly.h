#include "element.h"
#include "presentation-attributes.h"
#include <list>
#include <tuple>

namespace svg {

class PointList : public std::list<std::tuple<float,float>> {
public:
    using std::list<std::tuple<float,float>>::list;
    using std::list<std::tuple<float,float>>::operator=;
};
//using PointList = std::list<std::tuple<float,float>>;
//std::ostream& operator<<(std::ostream& os, const std::tuple<float,float>& t) {
//    os<<std::get<0>(t)<<","<<std::get<1>(t); 
//    return os;   
//}
std::ostream& operator<<(std::ostream& os, const PointList& pl) {
    for (auto [x,y] : pl) os<<x<<","<<y<<" "; 
    return os;   
}
//std::istream& operator>>(std::istream& is, std::tuple<float,float>& t) {
//    char dummy;
//    is>>std::get<0>(t)>>dummy>>std::get<1>(t); 
//    return is;   
//}
std::istream& operator>>(std::istream& is, PointList& pl) {
    pl.clear(); float x, y; char dummy;;
    while ( is>>x>>dummy>>y ) pl.push_back(std::tuple<float,float>(x,y));
    return is;
}

template<typename P>
class Poly : public pattern::Reflectable<Poly<P>,ElementBase> {
    PointList _points;
public:
    Poly(const PointList& pl = PointList()) : _points(pl) {}
    Poly(PointList&& pl) : _points(std::forward<PointList>(pl)) {}
    
    auto reflect() { return std::tie(_points); }
    auto reflect_names() const { return std::tuple("points"); }
    
    P& points(const PointList& pl) noexcept {
        _points = pl; return static_cast<P&>(*this);
    }
    P& points(PointList&& pl) noexcept {
        _points = std::forward<PointList>(pl); return static_cast<P&>(*this);
    }
    const PointList& points() const noexcept { return _points; }
   
	P& add_point(const std::tuple<float,float>& p) {
        _points.push_back(p);
		return static_cast<P&>(*this);
	}
    
	P& add_point(float x, float y) {
        return add_point(std::tuple<float,float>(x,y));
	}
};

class Polygon : public pattern::SelfRegisteringReflectable<Polygon,Poly<Polygon>,PresentationAttributes<Polygon>,CoreAttributes<Polygon>> {
public:
    //Needed for SelfRegisteringReflectable
    Polygon() {}
//    using Poly<Polygon>::Poly;
    static const char* type_name() { return "polygon"; } 
};

class Polyline : public pattern::SelfRegisteringReflectable<Polyline,Poly<Polyline>,PresentationAttributes<Polyline>,CoreAttributes<Polyline>> {
public:
    //Needed for SelfRegisteringReflectable
    Polyline() {}//    using Poly<Polyline>::Poly;
    static const char* type_name() { return "polyline"; } 
};

    
}
