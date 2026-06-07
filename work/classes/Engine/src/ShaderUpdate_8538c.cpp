#include "class.h"

extern "C" void ShaderBaseStruct_Update(ShaderBaseStruct *self);

extern "C" void Engine_ShaderUpdate(Engine *self)
{
    uint32_t index = 0;
    while (index < *(uint32_t *)((char *)self + 0x510)) {
        ShaderBaseStruct_Update(
            *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + index * 4));
        index += 1;
    }
}
