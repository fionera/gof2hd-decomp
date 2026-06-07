#include "class.h"

extern "C" void FBOContainer_ActivateRender2Texture(FBOContainer *self);

extern "C" void Engine_ActivateRender2FracFBO(Engine *self)
{
    FBOContainer *fbo = *(FBOContainer **)((char *)self + 0x418);
    if (fbo != 0) {
        return FBOContainer_ActivateRender2Texture(fbo);
    }
}
