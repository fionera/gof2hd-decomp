#include "class.h"

// String temp lifecycle helpers (compiler emits the canary via -fstack-protector).
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void String_assign(void *dst, void *src);   // operator=(String*, String*)
extern "C" void String_dtor(void *s);

// Agent::setMissionString(String) — this in r0, source String* in r1.
extern "C" void Agent_setMissionString(Agent *self, void *src)
{
    char tmp[12];
    String_copy_ctor(tmp, src, false);
    String_assign((char *)self + 0x6c, tmp);
    String_dtor(tmp);
}
