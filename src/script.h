#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include "element.h"




namespace svg {

enum class State : char { SlashOC, StarIC, SingleLineComment, MultiLineComment, NotAComment };

template <typename InputIt, typename OutputIt>
constexpr OutputIt copy_without_comments(InputIt first, InputIt last, OutputIt out) {
    State state = State::NotAComment;
    while (first != last) {
        switch (state) {
            case State::SlashOC:
                if (*first == '/') state = State::SingleLineComment;
                else if (*first == '*') state = State::MultiLineComment;
                else {
                    state = State::NotAComment;
                    *out++ = '/';
                    *out++ = *first;
                }
                break;
            case State::StarIC:
                if (*first == '/') state = State::NotAComment;
                else state = State::MultiLineComment;
                break;
            case State::NotAComment:
                if (*first == '#') state = State::SingleLineComment;
                else if (*first == '/') state = State::SlashOC;
                else *out++ = *first;
                break;
            case State::SingleLineComment:
                if (*first == '\n') {
                    state = State::NotAComment;
                    *out++ = '\n';
                }
                break;
            case State::MultiLineComment:
                if (*first == '*') state = State::StarIC;
        }
        ++first;
    }
    return out;
}

class Code {
    std::string _code;
public:
    Code(const std::string& code) : _code(code) {} 
    const std::string& code() const { return _code; }  
};

std::ostream& operator<<(std::ostream& os, const Code& c) {
//    os<<"// <![CDATA["<<std::endl;
    os<<c.code()<<std::endl;
//    os<<"// ]]>"<<std::endl;
    return os;
} 

std::istream& operator>>(std::istream& is, Code& c) {
    std::stringstream code;
    std::copy(std::istream_iterator<char>(is),std::istream_iterator<char>(),std::ostream_iterator<char>(code));
    c = Code(code.str());
    return is;
} 

class Script : public pattern::Reflectable<Script,ElementBase> {
    std::string _type;
    Code _code;
public:
    Script(const Code& code) : _type("text/javascript"),
        _code(code) { }
    Script(const std::string& code = std::string()) :
        _code(code) { }
    static const char* type_name() { return "script"; } 
    auto reflect() { return std::tie(_type,_code); }
    auto reflect_names() const { return std::tuple("type"); }
};



} 