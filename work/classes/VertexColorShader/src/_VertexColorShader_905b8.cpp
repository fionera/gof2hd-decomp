#include "class.h"

extern "C" void ShaderBaseStruct_dtor(AbyssEngine::ShaderBaseStruct *self);

namespace AbyssEngine {

VertexColorShader::~VertexColorShader()
{
    ShaderBaseStruct_dtor((ShaderBaseStruct *)this);
}

} // namespace AbyssEngine
