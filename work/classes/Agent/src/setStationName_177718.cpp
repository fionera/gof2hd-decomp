#include "class.h"

// AbyssEngine::String::operator=(String* dst, String src-by-value)
extern "C" void String_assign(void *dst, String12 src);

// Agent::setStationName(String) — this in r0, String by value in r1..r3.
// Tail-calls operator= on the field at +0x78.
extern "C" void Agent_setStationName(Agent *self, String12 src)
{
    String_assign((char *)self + 0x78, src);
}
