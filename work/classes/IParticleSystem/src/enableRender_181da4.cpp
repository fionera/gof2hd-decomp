#include "class.h"

typedef void (*RenderOffFn)(IParticleSystem *);

void IParticleSystem::enableRender(bool enabled)
{
    if (!enabled && U8(this, 0xd) != 0) {
        RenderOffFn fn = *(RenderOffFn *)((char *)this->f_0 + 8);
        fn(this);
    }
    U8(this, 0xd) = enabled;
}
