#include "class.h"

// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548
extern "C" void *String_assign_ref(void *self, const String12 &rhs);

// Mission::setTargetName(String by value): the String has a non-trivial copy
// ctor/dtor so it is passed by invisible reference (pointer in r1).
extern "C" void *Mission_setTargetName(Mission *self, const String12 &rhs) {
    return String_assign_ref((char *)self + 0x1c, rhs);
}
