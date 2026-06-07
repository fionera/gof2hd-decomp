#include "class.h"

extern "C" bool Engine_IsPostEffectActivated(Engine *self)
{
    return *(int *)((char *)self + 0x410) != 0;
}
