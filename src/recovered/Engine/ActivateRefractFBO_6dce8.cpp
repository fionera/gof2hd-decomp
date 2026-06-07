#include "class.h"

extern "C" void FBOContainer_ActivateTexture(FBOContainer *self);

extern "C" void Engine_ActivateRefractFBO(Engine *self)
{
    FBOContainer *fbo = *(FBOContainer **)((char *)self + 0x418);
    if (fbo != 0) {
        return FBOContainer_ActivateTexture(fbo);
    }
}
