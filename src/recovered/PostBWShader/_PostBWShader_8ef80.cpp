#define POSTBW_NONVIRTUAL_DTOR
#include "class.h"

extern "C" void *PostBWShader_vtable[];

namespace AbyssEngine {

PostBWShader::~PostBWShader()
{
    field_ptr(this, 0x0) = (char *)PostBWShader_vtable + 8;
}

} // namespace AbyssEngine
