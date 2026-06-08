#include "class.h"

namespace AbyssEngine {

// AbyssEngine::SandboxShader::SandboxShader()
SandboxShader::SandboxShader()
{
    // Base ShaderBaseStruct ctor.
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install the SandboxShader vtable (object pointer points 8 bytes into the
    // vtable, past the RTTI / offset-to-top slots).
    *(void **)this = (void *)(_ZTVN11AbyssEngine13SandboxShaderE + 8);

    // Register this shader into the global registry list.
    *SandboxShader_registerDst = SandboxShader_registerSrc;

    // Initialise the String member at +0xc from a literal.
    String tmp("SandboxShader", false);
    *(String *)((char *)this + 0xc) = tmp;
    tmp.~String();
}

} // namespace AbyssEngine
