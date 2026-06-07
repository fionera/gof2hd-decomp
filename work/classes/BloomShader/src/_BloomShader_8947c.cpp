#include "class.h"

extern "C" void *_ZN11AbyssEngine11BloomShaderD1Ev(BloomShader *self);
extern "C" void operator_delete(void *self);

extern "C" void _ZN11AbyssEngine11BloomShaderD0Ev(BloomShader *self)
{
    return operator_delete(_ZN11AbyssEngine11BloomShaderD1Ev(self));
}
