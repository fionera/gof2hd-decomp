#include "class.h"

extern "C" void FBOContainer_DeactivateRender2Texture(FBOContainer *self);

extern "C" void Engine_DeactivateRender2TextureFBO(Engine *self)
{
    FBOContainer *fbo = *(FBOContainer **)((char *)self + 0x414);
    if (fbo != 0) {
        return FBOContainer_DeactivateRender2Texture(fbo);
    }
}
