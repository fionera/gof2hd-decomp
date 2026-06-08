#include "class.h"

extern "C" void PlayerStatic_render_geometry(void *geometry);

void PlayerStatic::render()
{
    return PlayerStatic_render_geometry(pp(this, 0x8));
}
