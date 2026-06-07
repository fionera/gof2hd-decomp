#include "class.h"

extern "C" void String_ctor(String *self);
extern "C" void Array_int_ctor(void *array);
extern "C" void Array_ShaderBaseStruct_ptr_ctor(void *array);
extern "C" void ApplicationManager_ctor(ApplicationManager *self, Engine *engine);
extern "C" void Engine_initFileInterface(Engine *self);

extern "C" void Engine_Engine(Engine *self)
{
    void * volatile cookie = __stack_chk_guard;
    String_ctor((String *)self);
    String_ctor((String *)((char *)self + 0x14));
    String_ctor((String *)((char *)self + 0x3c));
    String_ctor((String *)((char *)self + 0x4c));
    *(uint64_t *)((char *)self + 0x340) = 0;
    *(uint64_t *)((char *)self + 0x34c) = 0;
    *(uint32_t *)((char *)self + 0x354) = 0;
    *(uint64_t *)((char *)self + 0x3cc) = 0;
    *(uint32_t *)((char *)self + 0x3d4) = 0;
    Vector up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    *(Vector *)((char *)self + 0x330) = up;
    Array_int_ctor((char *)self + 0x3d8);
    *(uint64_t *)((char *)self + 0x478) = 0;
    *(uint64_t *)((char *)self + 0x400) = 0;
    *(Vector *)((char *)self + 0x468) = up;
    *(Vector *)((char *)self + 0x3f0) = up;
    Array_ShaderBaseStruct_ptr_ctor((char *)self + 0x510);
    *(uint32_t *)((char *)self + 0x380) = 0;
    *(uint32_t *)((char *)self + 0x40c) = 0;
    *(uint32_t *)((char *)self + 0x410) = 0;
    *(uint8_t *)((char *)self + 0x3c4) = 0;
    *(uint32_t *)((char *)self + 0x3c8) = 0;
    up.x = 0.5f;
    up.y = 0.0f;
    up.z = 0.0f;
    *(Vector *)((char *)self + 0x3cc) = up;
    *(uint32_t *)((char *)self + 0x358) = 0;
    *(uint32_t *)((char *)self + 0x414) = 0;
    *(uint32_t *)((char *)self + 0x418) = 0;
    *(uint8_t *)((char *)self + 0x41c) = 0;
    *(uint32_t *)((char *)self + 0x360) = 0;
    *(uint32_t *)((char *)self + 0x4a8) = 0;
    *(uint32_t *)((char *)self + 0x70) = 0;
    *(uint32_t *)((char *)self + 0x100) = 0;
    *(int *)((char *)self + 0x3e4) = -1;
    for (int i = 0x1f; i != 0x33; i += 1) {
        *(int *)((char *)self + i * 4) = -1;
    }
    *(int *)((char *)self + 0x48c) = -1;
    *(int *)((char *)self + 0x490) = -1;
    *(uint32_t *)((char *)self + 0x4a4) = 0;
    *(uint16_t *)((char *)self + 0xfd) = 0x100;
    *(int *)((char *)self + 0x78) = -1;
    *(uint32_t *)((char *)self + 0x420) = 0;
    *(uint8_t *)((char *)self + 0x424) = 0;
    *(uint32_t *)((char *)self + 0x484) = 0;
    *(uint64_t *)((char *)self + 0x320) = 0;
    *(uint32_t *)((char *)self + 0x328) = 0;
    *(uint32_t *)((char *)self + 0x34) = 0;
    *(uint64_t *)((char *)self + 0x4c0) = 0;
    *(uint64_t *)((char *)self + 0x368) = 0;
    *(uint64_t *)((char *)self + 0x370) = 0;
    *(uint64_t *)((char *)self + 0x4b0) = 0;
    *(uint64_t *)((char *)self + 0x4b8) = 0;
    *(uint64_t *)((char *)self + 0x4e0) = 0;
    *(uint64_t *)((char *)self + 0x4e8) = 0;
    *(uint64_t *)((char *)self + 0x4f0) = 0;
    *(uint32_t *)((char *)self + 0x28) = 0x14;
    *(uint8_t *)((char *)self + 0x20) = 1;
    void *manager = operator new(0xc0);
    ApplicationManager_ctor((ApplicationManager *)manager, self);
    *(void **)((char *)self + 0x30) = manager;
    *(uint32_t *)((char *)self + 0x3e8) = 0;
    *(uint32_t *)((char *)self + 0x3ec) = 0;
    *(float *)((char *)self + 0xd0) = -1.0f;
    *(float *)((char *)self + 0xd4) = -1.0f;
    *(float *)((char *)self + 0xd8) = -1.0f;
    *(float *)((char *)self + 0xdc) = -1.0f;
    up.x = 1.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    *(Vector *)((char *)self + 0x3f0) = up;
    Engine_initFileInterface(self);
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
