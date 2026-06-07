#include "class.h"

void PlayerJumpgate::addJumpAnimationHandle(uint32_t handle)
{
    F<uint32_t>(this, 0x144) = handle;
}
