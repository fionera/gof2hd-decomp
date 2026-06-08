#include "class.h"

namespace AbyssEngine {

// AbyssEngine::ResourceTexture::~ResourceTexture()
ResourceTexture::~ResourceTexture()
{
    operator_delete__(*(void **)this);
    *(void **)this = 0;
}

} // namespace AbyssEngine
