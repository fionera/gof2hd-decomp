#include "class.h"

extern "C" void ShaderBaseStruct_ctor(AbyssEngine::ShaderBaseStruct *self);

extern "C" char g_VertexColorShader_vtable[];
extern "C" __attribute__((visibility("hidden"))) int *g_VertexColorShader_typeinfo_src;
extern "C" __attribute__((visibility("hidden"))) int *g_VertexColorShader_typeinfo_dst;

namespace AbyssEngine {

VertexColorShader::VertexColorShader()
{
    void *stack[4];
    stack[3] = __stack_chk_guard;

    ShaderBaseStruct_ctor((ShaderBaseStruct *)this);
    *(void * volatile *)this = g_VertexColorShader_vtable + 8;
    *g_VertexColorShader_typeinfo_dst = *g_VertexColorShader_typeinfo_src;
    new (stack) String("VertexColorShader", false);
    *(String *)((char *)this + 0x0c) = *(String *)stack;
    ((String *)stack)->~String();

    uint32_t guardDelta = (uint32_t)stack[3];
    guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - guardDelta;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
