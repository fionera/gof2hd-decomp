#include "class.h"

extern "C" void FBOContainer_ActivateRender2Texture(FBOContainer *self);

extern "C" void Engine_ActivateRender2TextureFBO(Engine *self)
{
    FBOContainer *fbo = *(FBOContainer **)((char *)self + 0x414);
    if (fbo != 0) {
        return FBOContainer_ActivateRender2Texture(fbo);
    }
}
