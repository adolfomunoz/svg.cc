#pragma once
#include <patterns/patterns.h>
#include "transform.h"
#include "clip.h"

namespace svg {

template<typename T>
class GraphicalAttributes : public pattern::Reflectable<GraphicalAttributes<T>> {
	constexpr T& t() noexcept { return *static_cast<T*>(this); } 
	constexpr const T& t() const noexcept { return *static_cast<const T*>(this); } 
    
    std::list<Transform> _transform;
    std::optional<Clip> _clip_path;
public:
    GraphicalAttributes() {}
    
    T& transform(const std::list<Transform>& l) noexcept {
        _transform=l; return t();
    }
    T& transform(std::list<Transform>&& l) noexcept {
        _transform=std::forward<std::list<Transform>>(l); return t();
    }
    const std::list<Transform>& transform() const noexcept { return _transform; }  
	T& add_transform(const Transform& tr) {
        (_transform).push_front(tr);
        return t();
	}
    
    T& clip_path(const Clip& c) noexcept {
        _clip_path=c; return t();
    }
    T& clip_path(Clip&& c) noexcept {
        _clip_path=std::forward<Clip>(c); return t();
    }
    Clip clip_path() const noexcept { return _clip_path.value_or(Clip()); }

    auto reflect() { return std::tie(_transform,_clip_path); }
    auto reflect_names() const { return std::tuple("transform","clip-path"); }
};

}
