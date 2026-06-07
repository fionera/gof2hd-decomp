#include "class.h"

extern "C" void Engine_ShaderRegister(Engine *self, ShaderBaseStruct *shader);
extern "C" void glGetError();

typedef void ShaderCtor(void *);

extern "C" void ShaderCtor_0(void *);
extern "C" void ShaderCtor_1(void *);
extern "C" void ShaderCtor_2(void *);
extern "C" void ShaderCtor_3(void *);
extern "C" void ShaderCtor_4(void *);

extern "C" uint32_t Engine_ShaderInit(Engine *self)
{
    static const uint32_t sizes[] = {
        0x8c, 0x2c, 0x94, 0x2c, 0x30, 0x74, 0x5c, 0x68, 0x44, 0x50,
        0x64, 0x58, 0x58, 0x30, 0x60, 0x84, 0x5c, 0x5c, 0x5c, 0x84,
        0x5c, 0xa0, 0x64, 0x34, 0x70, 0x98, 0x98, 0x5c, 0x5c, 0x60,
        0x58, 0x34, 0xa8, 0x60, 0x40, 0x6c, 0x64, 0x4c, 0x98,
    };
    ShaderCtor *ctors[] = {ShaderCtor_0, ShaderCtor_1, ShaderCtor_2, ShaderCtor_3, ShaderCtor_4};
    for (uint32_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i += 1) {
        void *shader = operator new(sizes[i]);
        ctors[i % 5](shader);
        Engine_ShaderRegister(self, (ShaderBaseStruct *)shader);
    }
    glGetError();
    return 1;
}
