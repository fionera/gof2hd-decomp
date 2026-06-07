#include "class.h"

extern "C" void ShaderBaseStruct_GetShaderName(String *result, ShaderBaseStruct *self);
extern "C" char *String_GetAEChar(String *self);
extern "C" void String_dtor(String *self);
extern "C" void operator_delete(void *ptr);
extern "C" void ArrayAdd_ShaderBaseStruct_ptr(ShaderBaseStruct *item, void *array);
extern "C" void ArrayAdd_int(int item, void *array);

typedef void ShaderInitFn(ShaderBaseStruct *, Engine *);

extern "C" void Engine_ShaderRegister(Engine *self, ShaderBaseStruct *shader)
{
    void * volatile cookie = __stack_chk_guard;
    if (shader != 0) {
        char nameStorage[sizeof(String)];
        String *name = (String *)nameStorage;
        ShaderBaseStruct_GetShaderName(name, shader);
        char *text = String_GetAEChar(name);
        String_dtor(name);

        void **vtable = *(void ***)shader;
        ((ShaderInitFn *)vtable[0x08 / 4])(shader, self);
        ArrayAdd_ShaderBaseStruct_ptr(shader, (char *)self + 0x510);
        ArrayAdd_int(0, (char *)self + 0x3d8);
        operator_delete(text);
    }
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
