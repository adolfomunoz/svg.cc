#pragma once
#include "group.h"
#include "svg.h"
namespace svg {

template<typename T, typename U>
std::list<T> find_all(const U& u) {
    std::list<T> all;
    u.apply(
        [&all] (const T& t) { all.push_back(t);},
        [&all] (const Group& g) {
            for (const Element& e : g.children()) {
                all.splice(all.end(), find_all<T>(e));
            }
        },
        [&all] (const SVG& g) {
            for (const Element& e : g.children()) {
                all.splice(all.end(), find_all<T>(e));
            }
        }
    );
    return all;
}

template<typename T, typename U, typename F>
void for_all(U& u, const F& f) {
    u.apply(
        [&f] (T& t) { f(t); },
        [&f] (Group& g) {
            for (Element e : g.children()) {
                for_all<T>(e,f);
            }
        },
        [&f] (SVG& g) {
            for (Element e : g.children()) {
                for_all<T>(e,f);
            }
        }
    );
}

}