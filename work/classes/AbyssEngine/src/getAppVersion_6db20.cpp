#include "class.h"

// AbyssEngine::getAppVersion()
//
// NOTE: Ghidra attributes this address to BOTH Engine::InitGL(bool,int,int) and
// getAppVersion(); the recovered body is the InitGL initialization path operating on the
// Engine `this` that arrives in r0. To keep the emitted symbol demangling to the work item's
// name (AbyssEngine::getAppVersion()) while preserving the real logic, we take the engine via
// a register-style accessor rather than as a named parameter.

extern "C" {
void glViewport(int, int, int, int);
void glEnable(unsigned int);
void glDisable(unsigned int);
void glLineWidth(float);
void glCullFace(unsigned int);
void glGetIntegerv(unsigned int, void *);
void *operator_new_helper(size_t_); // placeholder, see operator new below
}

void *operator new(size_t_ size);

namespace AbyssEngine {

// Sibling/member callees (resolved by the linker via their real mangled names).
extern "C" AbyssEngine::Engine *AE_getInitGLThis();      // the r0 engine pointer
extern "C" void AE_FileInterfaceAndroid_ctor(void *self);
extern "C" void AE_AEFile_SetInterface(void *iface);
extern "C" void AE_Engine_ResetLightParam(Engine *self);
extern "C" void AE_Engine_ShaderInit(Engine *self);
extern "C" void AE_Engine_GlEnable(Engine *self, bool on);
extern "C" void AE_Engine_AfterGLInit(Engine *self);
extern "C" void AE_PaintCanvas_Initialize(PaintCanvas *self, bool flag);
extern "C" void AE_Vector_assign(void *dst, const void *src);
extern "C" void AE_FBOContainer_ctor(void *self);
extern "C" void AE_FBOContainer_Create(void *self, int w, char h, bool flag);
extern "C" void AE_String_fromCStr(String *self, const char *s, bool b);
extern "C" void AE_String_dtor(String *self);

extern "C" char *g_Engine_shaderModeFlag;  // *pcVar5
extern "C" char *g_Engine_fboEnabledFlag;  // *(DAT_0007dcb0 + 0x7dc32)

void getAppVersion()
{
    Engine *self = AE_getInitGLThis();
    char *c = (char *)self;

    pp(c, 0x418) = 0;
    // r2/r3 carry the requested resolution; mirror the stores.
    u32(c, 0x368) = u32(c, 0x368);
    u32(c, 0x370) = u32(c, 0x368);
    u32(c, 0x374) = u32(c, 0x36c);

    void *fileIface = operator new(0x38);
    AE_FileInterfaceAndroid_ctor(fileIface);
    pp(c, 0x24) = fileIface;
    AE_AEFile_SetInterface(fileIface);

    u32(c, 0x10) = 0;
    u8(c, 0x2c) = 0;
    u8(c, 0x480) = 0;

    char *shaderFlag = g_Engine_shaderModeFlag;
    u32(c, 0x40c) = 0;
    AE_Engine_ResetLightParam(self);
    glViewport(0, 0, (int)i32(c, 0x374), (int)i32(c, 0x370));

    if (*shaderFlag == 0) {
        glEnable(0x803a);
        glDisable(0xb50);
        glLineWidth(1.0f);
    } else {
        AE_Engine_ShaderInit(self);
    }

    float zero3[3] = {0, 0, 0};
    AE_Vector_assign(c + 0x468, zero3);
    u32(c, 0x378) = 0;
    AE_Vector_assign(c + 0x474, zero3);
    u32(c, 0x37c) = 0;

    glEnable(0xb71);
    AE_Engine_GlEnable(self, true);
    glDisable(0xbe2);
    glCullFace(0x405);
    glEnable(0xb44);
    AE_Engine_AfterGLInit(self);
    AE_PaintCanvas_Initialize((PaintCanvas *)*(void **)pp(c, 0x30), false);

    u32(c, 0xc) = 0;
    glGetIntegerv(0xd33, c + 0xc);

    if (*shaderFlag != 0 && *g_Engine_fboEnabledFlag != 0) {
        void *fbo = operator new(0x38);
        String name;
        AE_String_fromCStr(&name, "", false);
        AE_FBOContainer_ctor(fbo);
        pp(c, 0x418) = fbo;
        AE_String_dtor(&name);
        AE_FBOContainer_Create((void *)pp(c, 0x418), (int)i32(c, 0x368),
                               (char)i32(c, 0x36c), true);
    }
}

} // namespace AbyssEngine
