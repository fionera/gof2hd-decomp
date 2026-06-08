#include "class.h"

// Hidden PC-relative globals recovered from the constructor disasm:
//   g_ctorCounter : *(int**) at 0x9dae8 — read its [0] into local_14 for stack guard.
//   g_shaderSrc   : *(int**) at 0x9dafa — value copied into *g_shaderDst.
//   g_shaderDst   : *(int**) at 0x9db02 — destination of the copied source pointer.
//   g_vtableBase  : *(int*)  at 0x9dafe — vtable address; this->vtable = base + 8.
//   g_nameLiteral : char* at 0x9db12 — the "TextureLightShader" name string literal.
__attribute__((visibility("hidden"))) extern int *g_tls_ctorCounter;
__attribute__((visibility("hidden"))) extern int **g_tls_shaderSrc;
__attribute__((visibility("hidden"))) extern int **g_tls_shaderDst;
__attribute__((visibility("hidden"))) extern int g_tls_vtableBase;
__attribute__((visibility("hidden"))) extern const char g_tls_nameLiteral[];

namespace AbyssEngine {

// TextureLightShader::TextureLightShader()
TextureLightShader::TextureLightShader()
{
    int *counter = g_tls_ctorCounter;
    int guard = *counter;

    ShaderBaseStruct_ctor(this);

    // Install vtable (base + 8 to skip RTTI/offset slots).
    this->f_0 = g_tls_vtableBase + 8;

    // Copy the shader-source pointer into its destination slot.
    *g_tls_shaderDst = *g_tls_shaderSrc;

    // Initialize the shader-name String member at this+0xc.
    String tmp(g_tls_nameLiteral, false);
    String *dst = (String *)((char *)this + 0xc);
    *dst = tmp;

    if (*counter - guard != 0)
        __stack_chk_fail(*counter - guard);
}

} // namespace AbyssEngine
