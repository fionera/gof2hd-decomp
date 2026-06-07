#include "class.h"

extern "C" void *ShaderBaseStruct_dtor(ShaderBaseStruct *self);
extern "C" void operator_delete(void *self);

namespace AbyssEngine {

BlurShader::~BlurShader()
{
    operator_delete(ShaderBaseStruct_dtor((ShaderBaseStruct *)this));
}

} // namespace AbyssEngine
