#include "class.h"

namespace AbyssEngine {

// AbyssEngine::GenericShader1::GenericShader1()
// Constructs the ShaderBaseStruct base, installs the vtable, publishes the instance
// to a global pointer slot, and assigns the shader name string to field 0xc.
extern int *g_genericShader1GlobalSlot; // *(DAT_0009ec24 + 0x9ebc6)
extern int *g_genericShader1GlobalSrc;  // *(DAT_0009ec1c + 0x9ebbe)
extern "C" void _ZN11AbyssEngine16ShaderBaseStructC2Ev(ShaderBaseStruct *self);

GenericShader1::GenericShader1()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install vtable: *this = &vtable + 8 (skip the two RTTI/offset words).
    *(void **)this = (void *)(_ZTVN11AbyssEngine14GenericShader1E + 8);

    // Publish *src into the global pointer slot.
    *g_genericShader1GlobalSlot = *g_genericShader1GlobalSrc;

    String tmp("GenericShader1", false);
    *(String *)((char *)this + 0xc) = tmp;
    tmp.~String();
}

} // namespace AbyssEngine
