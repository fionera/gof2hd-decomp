#include "class.h"

extern "C" void ArrayCtor_modules(void *array);
extern "C" void ArrayCtor_uint(void *array);
extern "C" void ArrayCtor_long_long(void *array);
extern "C" void *operator_new(unsigned int size);
extern "C" void PaintCanvas_ctor(void *canvas, void *engine);
extern "C" void AESoundRessource_ctor(void *sound);
extern "C" void ConfigReader_ctor(void *reader, void *engine);
extern "C" void CheatHandler_ctor(void *cheats, void *keys);

extern "C" ApplicationManager *ApplicationManager_ctor(ApplicationManager *self, void *engine)
{
    void * volatile cookie = __stack_chk_guard;

    ArrayCtor_modules((char *)self + 0x44);
    ArrayCtor_uint((char *)self + 0x50);
    ArrayCtor_long_long((char *)self + 0x88);

    *(uint64_t *)((char *)self + 0x70) = 0;
    *(uint64_t *)((char *)self + 0x78) = 0;
    *(void **)((char *)self + 0x60) = 0;
    *(uint64_t *)((char *)self + 0x68) = 0;
    *(uint64_t *)((char *)self + 0x08) = 0;
    *(int *)((char *)self + 0x3c) = 5;
    *(uint64_t *)((char *)self + 0x18) = 0;
    *(uint64_t *)((char *)self + 0x80) = 0;
    *(void **)((char *)self + 0xa8) = engine;

    void *canvas = operator_new(0x20c);
    PaintCanvas_ctor(canvas, engine);
    *(void **)self = canvas;

    void *sound = operator_new(0x14);
    AESoundRessource_ctor(sound);
    *(bool *)((char *)self + 0x34) = false;
    *(void **)((char *)self + 0xac) = sound;

    void *reader = operator_new(0x14);
    ConfigReader_ctor(reader, engine);
    *(void **)((char *)self + 0x20) = 0;
    *(void **)((char *)self + 0x38) = reader;
    *(void **)((char *)self + 0x28) = 0;
    *(uint16_t *)((char *)self + 0xb0) = 0x101;
    *(bool *)((char *)self + 0xb2) = true;
    *(bool *)((char *)self + 0x14) = false;

    char *storage = (char *)operator_new(0x408);
    *(uint32_t *)storage = 0x10;
    *(uint32_t *)(storage + 4) = 0x40;
    char *keys = storage + 8;
    for (int offset = 0; offset != 0x400; offset += 0x10) {
        new ((String *)(keys + offset + 4)) String();
        *(uint32_t *)(keys + offset) = 0;
    }
    *(char **)((char *)self + 0x10) = keys;

    void *cheats = operator_new(0x10);
    CheatHandler_ctor(cheats, keys);
    *(void **)((char *)self + 0x30) = cheats;
    *(int *)((char *)self + 0xb4) = -1;
    *(int *)((char *)self + 0xb8) = -1;

    if (cookie == __stack_chk_guard) {
        return self;
    }
    __stack_chk_fail();
}
