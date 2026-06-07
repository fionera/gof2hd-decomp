#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" int g_Engine_cloakShader;

typedef void ShaderDrawCloak(ShaderBaseStruct *);

extern "C" void Engine_DrawCloakFBO(Engine *self)
{
    if (g_Engine_useShaders != 0) {
        ShaderBaseStruct *shader =
            *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + g_Engine_cloakShader * 4);
        void **vtable = *(void ***)shader;
        return ((ShaderDrawCloak *)vtable[0x14 / 4])(shader);
    }
}
