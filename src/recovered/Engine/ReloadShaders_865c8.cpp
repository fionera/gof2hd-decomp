#include "class.h"

typedef void ShaderUnload(ShaderBaseStruct *);
typedef void ShaderInit(ShaderBaseStruct *, Engine *);

extern "C" void Engine_ReloadShaders(Engine *self)
{
    uint32_t index = 0;
    while (index < *(uint32_t *)((char *)self + 0x510)) {
        ShaderBaseStruct *shader =
            *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + index * 4);
        void **vtable = *(void ***)shader;
        ((ShaderUnload *)vtable[0x24 / 4])(shader);

        shader = *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + index * 4);
        vtable = *(void ***)shader;
        ((ShaderInit *)vtable[0x08 / 4])(shader, self);
        index += 1;
    }
}
