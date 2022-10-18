#include "element.h"
#include "presentation-attributes.h"
#include "graphical-attributes.h"
#include <list>
#include <cctype>

namespace svg {

class CommandBase : public pattern::SelfRegisteringReflectableBase {
protected:
    char letter() const { return this->object_type_name()[0]; }
public:
    virtual std::size_t size() const = 0;
    virtual float get(std::size_t i) const = 0;
    virtual void set(std::size_t i, float v) = 0;
};

class Command : public pattern::Pimpl<CommandBase> {
    bool is_absolute_ = true;

public:
    using pattern::Pimpl<CommandBase>::Pimpl;
    using pattern::Pimpl<CommandBase>::operator=;

    std::size_t size() const override {
        return this->impl()->size();
    }
    float get(std::size_t i) const override {
        return this->impl()->get(i);
    }
    void set(std::size_t i, float v) override { 
        this->impl()->set(i,v);
    }

    void set_absolute(bool v = true) { is_absolute_ = v; }
    void set_relative(bool v = true) { is_absolute_ = !v; }
    bool is_absolute() const { return is_absolute_; }
    bool is_relative() const { return !is_absolute_; }
    char token() const {
        if (is_absolute()) return std::toupper(letter());
        else return std::tolower(letter());
    }
};

inline std::ostream& operator<<(std::ostream& os, const Command& c) {
    os<<c.token();
    for (std::size_t i = 0; i<c.size(); ++i)
        os<<" "<<c.get(i);
    return os;
}

inline std::istream& operator>>(std::istream& is, Command& c) {
    is>>std::ws;
    char token;
    if (!std::isdigit(is.peek()) && (is.peek()!='-'))  {
        is>>token;
    } else {
        token = c.token();
        if (token=='M') token = 'L';
        if (token=='m') token = 'l';
    }
    std::cerr<<"TOKEN = "<<token<<std::endl;
    c.set_type(std::string(1,std::tolower(token)));
    c.set_absolute(std::isupper(token));
    for (std::size_t i = 0; i<c.size(); ++i) {
        std::cerr<<char(is.peek())<<std::endl;
        float v; is>>v;  c.set(i,v); 
        std::cerr<<v<<std::endl;
        if (is.peek() == ',') { char dummy; is>>dummy; }
    }
    std::cerr<<"At the end -> "<<char(is.peek())<<std::endl;
    return is;
}

//It needs empty constructor for propper registration
#define DECLARE_COMMAND(LETTER,N) \
    class LETTER : public pattern::Reflectable<LETTER, CommandBase> {\
            std::array<float,N> data;\
        public: \
            LETTER () { data.fill(0.0f); }\
            static const char* type_name() { return #LETTER; }\
            std::size_t size() const override { return N; }\
            float get(std::size_t i) const override { return data[i]; }\
            void set(std::size_t i, float v) override { data[i]=v; }\
    };

namespace path_command {
    DECLARE_COMMAND(m,2)
    DECLARE_COMMAND(l,2)
    DECLARE_COMMAND(h,1)
    DECLARE_COMMAND(v,1)
    DECLARE_COMMAND(z,0)
    DECLARE_COMMAND(c,6)
    DECLARE_COMMAND(s,4)
    DECLARE_COMMAND(q,4)
    DECLARE_COMMAND(t,2)
    DECLARE_COMMAND(a,7)
}

class Path : public pattern::Reflectable<Path,ElementBase,
        PresentationAttributes<Path>,CoreAttributes<Path>,GraphicalAttributes<Path>> {
    std::list<Command> _d;
public:
    Path(const std::list<Command>& d = std::list<Command>()) : _d(d) {}
    Path(std::list<Command>&& d) : _d(std::forward<std::list<Command>>(d)) {}
    
    auto reflect() { return std::tie(_d); }
    auto reflect_names() const { return std::tuple("d"); }
    
    Path& d(const std::list<Command>& l) noexcept {
        _d=l; return (*this);
    }
    Path& d(std::list<Command>&& l) noexcept {
        _d=std::forward<std::list<Command>>(l); return (*this);;
    }
    const std::list<Command>& d() const noexcept { return _d; }
   
	Path& add_command(const Command& c) {
        _d.push_back(c);
        return (*this);
	}

    static const char* type_name() { return "path"; }
};

    
}
