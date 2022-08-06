#pragma once
#include "element.h"

namespace svg {

class Node : public pattern::Reflectable<Node,ElementBase> {
    std::list<Element> children;
public:
    auto reflect() { return std::tie(children); }

    template<typename T>
    T& add(const T& t) { 
        children.push_back(t); 
        return children.back().cast_static<T>(); 
    }
    template<typename T>
    T& add(T&& t) { 
        children.push_back(std::forward<T>(t)); 
        return children.back().cast_static<T>(); 
    }

    template<typename T>
    std::list<T> find_all() const {
        std::list<T> all;
        for (const Element& e : children)
            e.apply([&all] (const T& t) { all.push_back(t); });
        return all;
    }
};

}
