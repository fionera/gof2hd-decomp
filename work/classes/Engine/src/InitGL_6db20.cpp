#include "class.h"

extern "C" void FileInterfaceAndroid_ctor(void *self);
extern "C" void AEFile_SetInterface(void *fileInterface);
extern "C" void Engine_ResetLightParam(Engine *self);
extern "C" void Engine_ShaderInit(Engine *self);
extern "C" void Engine_AfterGLInit(Engine *self);
extern "C" void Engine_GlEnable(Engine *self, unsigned int cap, bool enable);
extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_dtor(String *self);
extern "C" void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_Create(FBOContainer *self, int width, int height, bool depth, bool color);
extern "C" void PaintCanvas_Initialize(void *canvas, bool value);
extern "C" void glViewport(int x, int y, int width, int height);
extern "C" void glEnable(unsigned int cap);
extern "C" void glDisable(unsigned int cap);
extern "C" void glLineWidth(float width);
extern "C" void glCullFace(unsigned int mode);
extern "C" void glGetIntegerv(unsigned int name, void *out);
extern "C" uint8_t g_Engine_useShaders;
extern "C" uint8_t g_Engine_supportsFBO;

extern "C" int Engine_InitGL(Engine *self, bool shaders, int width, int height)
{
    void * volatile cookie = __stack_chk_guard;
    *(uint32_t *)((char *)self + 0x418) = 0;
    *(int *)((char *)self + 0x368) = width;
    *(int *)((char *)self + 0x36c) = height;
    *(int *)((char *)self + 0x370) = width;
    *(int *)((char *)self + 0x374) = height;

    void *fileInterface = operator new(0x38);
    FileInterfaceAndroid_ctor(fileInterface);
    *(void **)((char *)self + 0x24) = fileInterface;
    AEFile_SetInterface(fileInterface);

    *(uint32_t *)((char *)self + 0x10) = 0;
    *(uint8_t *)((char *)self + 0x2c) = 0;
    *(uint8_t *)((char *)self + 0x480) = 0;
    g_Engine_useShaders = shaders;
    *(uint32_t *)((char *)self + 0x40c) = 0;

    Engine_ResetLightParam(self);
    glViewport(0, 0, *(int *)((char *)self + 0x374), *(int *)((char *)self + 0x370));
    if (g_Engine_useShaders != 0) {
        Engine_ShaderInit(self);
    } else {
        glEnable(0x803a);
        glDisable(0xb50);
        glLineWidth(1.0f);
    }

    Vector value;
    value.x = 0.0f;
    value.y = 1.0f;
    value.z = 0.0f;
    *(Vector *)((char *)self + 0x468) = value;
    *(uint32_t *)((char *)self + 0x378) = 0;
    *(Vector *)((char *)self + 0x474) = value;
    *(uint32_t *)((char *)self + 0x37c) = 0;

    glEnable(0xb71);
    Engine_GlEnable(self, 0xde1, true);
    glDisable(0xbe2);
    glCullFace(0x405);
    glEnable(0xb44);
    Engine_AfterGLInit(self);
    PaintCanvas_Initialize(**(void ***)((char *)self + 0x30), false);
    *(uint32_t *)((char *)self + 0x0c) = 0;
    glGetIntegerv(0xd33, (char *)self + 0x0c);

    if (g_Engine_useShaders != 0 && g_Engine_supportsFBO != 0) {
        FBOContainer *fbo = (FBOContainer *)operator new(0x38);
        char nameStorage[sizeof(String)];
        String *name = (String *)nameStorage;
        String_ctor_char(name, "refract", false);
        FBOContainer_ctor(fbo, self, name);
        *(FBOContainer **)((char *)self + 0x418) = fbo;
        String_dtor(name);
        FBOContainer_Create(fbo, *(int *)((char *)self + 0x368),
                            *(int *)((char *)self + 0x36c), false, true);
    }

    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return 1;
    }
    __stack_chk_fail();
}
