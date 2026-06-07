#include "class.h"

extern "C" void FBOContainer_DeactivateRender2Texture(FBOContainer *self);

extern "C" void Engine_DeactivateRender2FracFBO(Engine *self)
{
    FBOContainer *fbo = *(FBOContainer **)((char *)self + 0x418);
    if (fbo != 0) {
        return FBOContainer_DeactivateRender2Texture(fbo);
    }
}
