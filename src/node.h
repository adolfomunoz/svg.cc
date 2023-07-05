#pragma once
#include "element.h"

namespace svg {

class Node : public pattern::Reflectable<Node,ElementBase> {
    std::list<Element> children_;
public:
    auto reflect() { return std::tie(children_); }
    const std::list<Element>& children() const { return children_; }
    std::list<Element>& children() { return children_; } 

    template<typename T>
    T& add(const T& t) { 
        children_.push_back(t); 
        return children_.back().cast_static<T>(); 
    }
    template<typename T>
    T& add(T&& t) { 
        children_.push_back(std::forward<T>(t)); 
        return children_.back().cast_static<T>(); 
    }

};

}
