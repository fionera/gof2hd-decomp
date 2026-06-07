#include "class.h"

extern "C" void String_assign(void *dst, String12 src);

extern "C" void Agent_setSystemName(Agent *self, String12 src)
{
    String_assign((char *)self + 0x18, src);
}
