#include "class.h"

namespace AbyssEngine {

TextureAlphaTestShader::~TextureAlphaTestShader()
{
    ((ShaderBaseStruct *)this)->~ShaderBaseStruct();
    ::operator delete(this);
}

} // namespace AbyssEngine
