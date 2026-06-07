#include "class.h"

extern "C" int g_Engine_defaultShader;
extern "C" int g_Engine_altShader;
extern "C" uint8_t g_Engine_shaderDirty;
extern "C" int g_Engine_currentShader;

typedef void ShaderEnable(ShaderBaseStruct *, bool);
typedef void ShaderApply(ShaderBaseStruct *, Mesh *, Engine *);

extern "C" void Engine_ShaderSetActive(Engine *self, int shaderIndex, Mesh *mesh)
{
    while (shaderIndex == -1) {
        shaderIndex = g_Engine_defaultShader;
        if ((((uint32_t)*(uint8_t *)mesh) << 30) >= 0) {
            shaderIndex = g_Engine_altShader;
        }
    }

    bool hasExtra = mesh != 0 && *(uint8_t *)((char *)mesh + 0x85) != 0;
    ShaderBaseStruct *shader =
        *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + shaderIndex * 4);
    if (shader == 0) {
        return;
    }
    g_Engine_shaderDirty = 1;

    void **vtable = *(void ***)shader;
    if (*(int *)((char *)shader + 4) != *(int *)((char *)self + 0x3e4)) {
        ((ShaderEnable *)vtable[0x28 / 4])(shader, hasExtra);
        shader = *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + shaderIndex * 4);
        *(int *)((char *)self + 0x3e4) = *(int *)((char *)shader + 4);
        g_Engine_currentShader = shaderIndex;
        vtable = *(void ***)shader;
    }
    ((ShaderEnable *)vtable[0x28 / 4])(shader, hasExtra);
    shader = *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + shaderIndex * 4);
    vtable = *(void ***)shader;
    ((ShaderApply *)vtable[0x0c / 4])(shader, mesh, self);
    if (mesh != 0) {
        uint32_t triangles = __aeabi_uidiv(*(uint16_t *)((char *)mesh + 0x28), 3);
        uint32_t *slot = (uint32_t *)(*(char **)((char *)self + 0x3dc) + shaderIndex * 4);
        *slot += triangles;
    }
}
