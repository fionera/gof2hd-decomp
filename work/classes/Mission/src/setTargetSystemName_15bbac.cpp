#include "class.h"

// AbyssEngine::String::operator=(String* this, const String& rhs) -> 0x1ac548
extern "C" void *String_assign_ref(void *self, const String12 &rhs);

extern "C" void *Mission_setTargetSystemName(Mission *self, const String12 &rhs) {
    return String_assign_ref((char *)self + 0x4c, rhs);
}
