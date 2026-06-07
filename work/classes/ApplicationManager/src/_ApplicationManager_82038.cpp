#include "class.h"

typedef void ModuleCallback(void *);

extern "C" void ArrayRelease_modules(void *array);
extern "C" void ArrayRelease_uint(void *array);
extern "C" void ArrayDtor_long_long(void *array);
extern "C" void ArrayDtor_uint(void *array);
extern "C" void ArrayDtor_modules(void *array);
extern "C" void PaintCanvas_dtor(void *canvas);
extern "C" void AESoundRessource_dtor(void *sound);
extern "C" void CheatHandler_dtor(void *cheats);
extern "C" void ConfigReader_dtor(void *reader);
extern "C" void operator_delete(void *ptr);
extern "C" void operator_delete_array(void *ptr);

namespace AbyssEngine {
struct ApplicationManager {
    ~ApplicationManager();
};
}

__attribute__((minsize)) AbyssEngine::ApplicationManager::~ApplicationManager()
{
    ApplicationManager *self = this;
    void *module = *(void **)((char *)self + 0x18);
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x0c / 4](module);
    }

    void *modules = (char *)self + 0x44;
    unsigned int offset = 0;
    for (unsigned int i = 0; i < *(unsigned int *)modules; ++i) {
        void **slot = (void **)(*(char **)((char *)self + 0x48) + offset);
        void *entry = *slot;
        if (entry != 0) {
            ModuleCallback **vtable = *(ModuleCallback ***)entry;
            vtable[1](entry);
            slot = (void **)(*(char **)((char *)self + 0x48) + offset);
        }
        *slot = 0;
        offset += 4;
    }
    ArrayRelease_modules(modules);
    ArrayRelease_uint((char *)self + 0x50);

    void *canvas = *(void **)self;
    if (canvas != 0) {
        PaintCanvas_dtor(canvas);
        operator_delete(canvas);
    }
    *(void **)self = 0;

    void *sound = *(void **)((char *)self + 0xac);
    if (sound != 0) {
        AESoundRessource_dtor(sound);
        operator_delete(sound);
    }
    *(void **)((char *)self + 0xac) = 0;

    void *cheats = *(void **)((char *)self + 0x30);
    if (cheats != 0) {
        CheatHandler_dtor(cheats);
        operator_delete(cheats);
    }
    *(void **)((char *)self + 0x30) = 0;

    void *reader = *(void **)((char *)self + 0x38);
    if (reader != 0) {
        ConfigReader_dtor(reader);
        operator_delete(reader);
    }
    *(void **)((char *)self + 0x38) = 0;

    char *keys = *(char **)((char *)self + 0x10);
    if (keys != 0) {
        unsigned int count = *(unsigned int *)(keys - 4);
        for (unsigned int i = count; i != 0; --i) {
            ((String *)(keys - 12 + i * 0x10))->~String();
        }
        operator_delete_array(keys - 8);
    }
    *(void **)((char *)self + 0x10) = 0;

    ArrayDtor_long_long((char *)self + 0x88);
    ArrayDtor_uint((char *)self + 0x50);
    ArrayDtor_modules(modules);
}
