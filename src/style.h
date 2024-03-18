#pragma once
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"
#include <iostream>
#include <sstream>
#include <list>

namespace svg {

class StyleAttr;
std::istream& operator>>(std::istream& is, StyleAttr& style);

class StyleAttr : public pattern::Reflectable<StyleAttr,PresentationAttributes<StyleAttr>,TextPresentationAttributes<StyleAttr>> {
public:
    StyleAttr() {} 
    StyleAttr(const std::string& str) {
        std::stringstream sstr(str);
        sstr>>(*this);
    }  
    virtual ~StyleAttr() {}
    static const char* type_name() { return "style"; } 
//    auto reflect() {}
//    auto reflect_names() const { }
};

namespace detail {
    template<typename T, typename Enable = void>
    struct CSS {
        static void load(const std::string& value, T& t) {
            std::stringstream s(value);
            s>>t;
        }
        static bool has_value(const T& t) {
            return true;
        }
        static std::string to_string(const T& t) {
            std::stringstream s;
            s<<t;
            return s.str();
        }
    };
    template<typename T>
    struct CSS<std::list<T>> {
        static void load(const std::string& value, std::list<T>& t) {
            t.clear();
            const auto end = value.end(); auto to = value.begin(); decltype(to) from;
            while((from = std::find_if(to, end, [](char c){ return !std::isspace(c); })) != end) {
                to = std::find_if(from, end, [](char c){ return std::isspace(c); });
                T item;
                CSS<T>::load(std::string(from,to),item);
                t.push_back(item);
            }
        }
        static bool has_value(const std::list<T>& t) {
            return !t.empty();
        }
        static std::string to_string(const std::list<T>& t) {
            std::stringstream sstr;
            for (const T& e : t) sstr<<CSS<T>::to_string(e)<<" ";
            return sstr.str();
        }
    };
    
    template<typename T>
    struct CSS<std::optional<T>> {
        static void load(const std::string& value, std::optional<T>& t) {
            T v;
            CSS<T>::load(value,v);
            t = v;
        }
        static bool has_value(const std::optional<T>& t) {
            return bool(t);
        }
        static std::string to_string(const std::optional<T>& t) {
            if (t) return CSS<T>::to_string(*t);
            else return "";
        }
    };

    template<typename T>
    void load(const std::string& value, T& t) {
        CSS<std::decay_t<T>>::load(value,t);
    }
    template<typename T>
    bool has_value(const T& t) {
        return CSS<std::decay_t<T>>::has_value(t);
    }    
    template<typename T>
    std::string to_string(const T& t) {
        return CSS<std::decay_t<T>>::to_string(t);
    }    

}

inline std::istream& operator>>(std::istream& is, StyleAttr& style) {
    std::string name; std::string value;
    while (!is.eof()) {
        std::getline(is,name,':'); //Remove trailing and ending spaces
        name.erase(0,name.find_first_not_of(" \t\n\r\f\v"));
        name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);
        std::getline(is,value,';'); //Remove trailing and ending spaces
        value.erase(0,value.find_first_not_of(" \t\n\r\f\v"));
        value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
        style.for_each_attribute([&name,&value] (const std::string& parname, auto& parvalue) {
            if (name == parname) {
                detail::load(value,parvalue);
            }
        });
    }
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const StyleAttr& style) {
    style.for_each_attribute([&os] (const std::string& name, const auto& value) {
        if (detail::has_value(value)) {
            os<<name<<": "<<detail::to_string(value)<<"; ";
        }
    });
    return os;
}

class StyleRule {
    std::string selector_;
    StyleAttr declaration_;
public:
    StyleRule(const std::string& selector = "", const StyleAttr& declaration = StyleAttr()) :
        selector_(selector), declaration_(declaration) {}
    StyleRule(const std::string& selector, const std::string& declaration) :
        selector_(selector), declaration_(declaration) {}

    const std::string& selector() const { return selector_; }
    const StyleAttr& declaration() const { return declaration_; }
};

inline std::istream& operator>>(std::istream& is, StyleRule& style) {
    std::string selector; std::string declaration;
    std::getline(is,selector,'{');
    selector.erase(0,selector.find_first_not_of(" \t\n\r\f\v"));
    selector.erase(selector.find_last_not_of(" \t\n\r\f\v") + 1);
    std::getline(is,declaration,'}');
    declaration.erase(0,declaration.find_first_not_of(" \t\n\r\f\v"));
    declaration.erase(declaration.find_last_not_of(" \t\n\r\f\v") + 1);
    style = StyleRule(selector,declaration);
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const StyleRule& style) {
    os<<style.selector()<<" {\n   "<<style.declaration()<<"\n}\n";
    return os;
}

class Style : public pattern::Reflectable<Style,ElementBase> {
    std::list<StyleRule> _rules;
public:
    Style(const std::list<StyleRule>& rules = std::list<StyleRule>()) :
        _rules(rules) { }
    static const char* type_name() { return "style"; } 
    auto reflect() { return std::tie(_rules); }
    auto reflect_names() const { return std::tuple(); }
    StyleRule& add(const StyleRule& sr) {
        _rules.push_back(sr); return _rules.back();
    }
    StyleRule& add(const std::string& selector, const std::string& description) {
        return add(StyleRule(selector,description));
    }
    
};

}
