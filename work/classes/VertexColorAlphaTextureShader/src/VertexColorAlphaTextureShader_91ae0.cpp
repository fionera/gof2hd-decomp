#include "class.h"

// AbyssEngine::VertexColorAlphaTextureShader::VertexColorAlphaTextureShader()
// Chains to the ShaderBaseStruct base ctor, installs this class's vtable, bumps the
// global shader-index counter, and stores the shader's resource name ("VCATShader").

namespace AbyssEngine {

// Base ctor (mangled): ShaderBaseStruct::ShaderBaseStruct()
extern "C" void *_ZN11AbyssEngine16ShaderBaseStructC2Ev(ShaderBaseStruct *self);

// Global shader-index counter copied into a destination slot during construction.
__attribute__((visibility("hidden"))) extern int g_shaderIndexSrc;
__attribute__((visibility("hidden"))) extern int g_shaderIndexDst;

VertexColorAlphaTextureShader::VertexColorAlphaTextureShader()
{
    _ZN11AbyssEngine16ShaderBaseStructC2Ev((ShaderBaseStruct *)this);

    // Install the derived vtable (base address + 8 -> first virtual slot).
    *(void **)this = (void *)(_ZTVN11AbyssEngine29VertexColorAlphaTextureShaderE + 8);

    // Publish the current shader index.
    g_shaderIndexDst = g_shaderIndexSrc;

    // Store the shader resource name in the embedded String at +0x0c.
    String name("VCATShader", false);
    *(String *)((char *)this + 0xc) = name;
}

} // namespace AbyssEngine
