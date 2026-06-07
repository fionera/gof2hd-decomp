#include "class.h"

extern "C" int g_Engine_activeShader;

typedef void ShaderInactive(ShaderBaseStruct *);

extern "C" void Engine_ShaderSetInActive(Engine *self)
{
    ShaderBaseStruct *shader =
        *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + g_Engine_activeShader * 4);
    void **vtable = *(void ***)shader;
    return ((ShaderInactive *)vtable[0x10 / 4])(shader);
}
