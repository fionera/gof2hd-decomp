#include "class.h"

typedef void DestroyCallback(Engine *);
typedef void FileInterfaceRelease(void *);

extern "C" void ApplicationManager_dtor(ApplicationManager *self);
extern "C" void operator_delete(void *ptr);
extern "C" void AEFile_Release();
extern "C" void ArrayReleaseClasses_ShaderBaseStruct_ptr(void *array);
extern "C" void FBOContainer_dtor(FBOContainer *self);
extern "C" void MeshRelease(Engine *self, void *meshSlot);
extern "C" void Engine_ReleaseGL(Engine *self);
extern "C" void Array_ShaderBaseStruct_ptr_dtor(void *array);
extern "C" void Array_int_dtor(void *array);
extern "C" void String_dtor(String *self);

namespace AbyssEngine {

Engine::~Engine()
{
    DestroyCallback *destroy = *(DestroyCallback **)((char *)this + 0x484);
    if (destroy != 0) {
        destroy(this);
    }

    ApplicationManager *manager = *(ApplicationManager **)((char *)this + 0x30);
    if (manager != 0) {
        ApplicationManager_dtor(manager);
        operator_delete(manager);
    }
    *(uint32_t *)((char *)this + 0x30) = 0;

    void *fileInterface = *(void **)((char *)this + 0x24);
    if (fileInterface != 0) {
        void **vtable = *(void ***)fileInterface;
        ((FileInterfaceRelease *)vtable[1])(fileInterface);
    }
    *(uint32_t *)((char *)this + 0x24) = 0;

    AEFile_Release();
    void *shaders = (char *)this + 0x510;
    ArrayReleaseClasses_ShaderBaseStruct_ptr(shaders);

    FBOContainer *fbo = *(FBOContainer **)((char *)this + 0x414);
    if (fbo != 0) {
        FBOContainer_dtor(fbo);
        operator_delete(fbo);
    }
    *(uint32_t *)((char *)this + 0x414) = 0;

    fbo = *(FBOContainer **)((char *)this + 0x418);
    if (fbo != 0) {
        FBOContainer_dtor(fbo);
        operator_delete(fbo);
    }
    *(uint32_t *)((char *)this + 0x418) = 0;

    MeshRelease(this, (char *)this + 0x380);
    Engine_ReleaseGL(this);
    Array_ShaderBaseStruct_ptr_dtor(shaders);
    Array_int_dtor((char *)this + 0x3d8);
    String_dtor((String *)((char *)this + 0x4c));
    String_dtor((String *)((char *)this + 0x3c));
    String_dtor((String *)((char *)this + 0x14));
    String_dtor((String *)this);
}

} // namespace AbyssEngine
