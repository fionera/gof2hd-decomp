#include "class.h"

extern "C" {
String *String_dtor(String *self);   // 0x6f580 - the non-deallocating destructor
void operator_delete(void *p);        // 0x1ab098 - deallocating operator delete
}

// AbyssEngine::String::~String() - deleting destructor: destroy then free the object.
extern "C" void String_dtor_del(String *self)
{
    String_dtor(self);
    operator_delete(self);
}
