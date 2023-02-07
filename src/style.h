#pragma once
#include "presentation-attributes.h"
#include "text-presentation-attributes.h"

namespace svg {

class Style : public pattern::Reflectable<Style,PresentationAttributes<Style>,TextPresentationAttributes<Style>> {
public:
    Style() {}   
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

inline std::istream& operator>>(std::istream& is, Style& style) {
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

inline std::ostream& operator<<(std::ostream& os, const Style& style) {
    style.for_each_attribute([&os] (const std::string& name, const auto& value) {
        if (detail::has_value(value)) {
            os<<name<<": "<<detail::to_string(value)<<"; ";
        }
    });
    return os;
}


    
}
