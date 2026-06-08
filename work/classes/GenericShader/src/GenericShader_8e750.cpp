#include "class.h"

namespace AbyssEngine {

// AbyssEngine::GenericShader::GenericShader()
GenericShader::GenericShader()
{
    // Base ShaderBaseStruct ctor.
    ShaderBaseStruct_ctor(this);

    // Install the GenericShader vtable (object pointer points 8 bytes into the
    // vtable table, past the RTTI/offset-to-top slots).
    *(void **)this = (void *)(GenericShader_vtable + 8);

    // Register this shader into the global registry list.
    *GenericShader_registerDst = GenericShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp("GenericShader", false);
    *(String *)((char *)this + 0xc) = tmp;
    tmp.~String();
}

} // namespace AbyssEngine
