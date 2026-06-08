#include "class.h"

extern "C" void Engine_ResetLightParam(Engine *self);
extern "C" void MeshCreate(Engine *self, int vertices, int faces, int flags, void *outMesh);
extern "C" const char *glGetString(unsigned int name);
extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_assign(String *self, const String *other);
extern "C" void String_dtor(String *self);
extern "C" String *g_Engine_vendorString;
extern "C" String *g_Engine_rendererString;

extern "C" void Engine_AfterGLInit(Engine *self)
{
    void * volatile cookie = __stack_chk_guard;
    Engine_ResetLightParam(self);
    MeshCreate(self, 4, 2, 0x13, (char *)self + 0x380);

    uint32_t *indices = *(uint32_t **)(*(char **)((char *)self + 0x380) + 0x2c);
    indices[0] = 0x20000;
    indices[1] = 1;
    indices[2] = 0x30002;

    char storage[sizeof(String)];
    String *text = (String *)storage;
    String_ctor_char(text, glGetString(0x1f00), false);
    String_assign(g_Engine_vendorString, text);
    String_dtor(text);
    String_ctor_char(text, glGetString(0x1f01), false);
    String_assign(g_Engine_rendererString, text);
    String_dtor(text);

    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
