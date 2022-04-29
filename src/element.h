#pragma once
#include <patterns/patterns.h>

namespace svg {

class ElementBase : public pattern::SelfRegisteringReflectableBase, public pattern::VisitableBase {
public:
    static const char* type_name() { return "element"; } 
};

class Element : public pattern::Pimpl<ElementBase> {
public:
    using pattern::Pimpl<ElementBase>::Pimpl;
    using pattern::Pimpl<ElementBase>::operator=;
};
    
}
