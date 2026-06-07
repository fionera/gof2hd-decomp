#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" bool Engine_IsPostEffectActivated(Engine *self);
extern "C" void Engine_DeactivateRender2TextureFBO(Engine *self);
extern "C" void Engine_DrawCloakFBO(Engine *self, FBOContainer *fbo);
extern "C" void Engine_ActivateRender2TextureFBO(Engine *self);
extern "C" void Engine_DeactivateRender2FracFBO(Engine *self);
extern "C" void Engine_ActivateViewBuffer(Engine *self);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDepthMask(unsigned int flag);
extern "C" void glDisable(unsigned int cap);
extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);

extern "C" void Engine_CopyFBO(Engine *self)
{
    if (g_Engine_useShaders == 0) {
        return;
    }

    if (Engine_IsPostEffectActivated(self)) {
        Engine_DeactivateRender2TextureFBO(self);
        Engine_DrawCloakFBO(self, *(FBOContainer **)((char *)self + 0x414));
        Engine_ActivateRender2TextureFBO(self);
    } else {
        Engine_DeactivateRender2FracFBO(self);
        Engine_DrawCloakFBO(self, *(FBOContainer **)((char *)self + 0x418));
        Engine_ActivateViewBuffer(self);
    }

    glEnable(0xb71);
    glDepthMask(1);
    glDisable(0xbe2);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return glClear(0x100);
}
