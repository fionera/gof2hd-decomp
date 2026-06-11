#ifndef GOF2_BUMPSHADERPARTICLE_H
#define GOF2_BUMPSHADERPARTICLE_H
#include "gof2/common.h"
#include <new>
// Galaxy on Fire 2 -- AbyssEngine::BumpShaderParticle (Android libgof2hdaa.so, armv7 Thumb).
// GLES2 bump-mapping shader for particle meshes. Derives from ShaderBaseStruct: program handle
// at 0x4, dirty flag byte at 0x9, name String at 0xc, attribute/uniform locations 0x20..0x68.
// Offset-addressed fields go through the field_* accessor helpers (shared shader-storage idiom).

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" char _ZTVN11AbyssEngine18BumpShaderParticleE[];

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform3f(int location, float x, float y, float z);
extern "C" void glUniform3fv(int location, int count, const float *value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix3fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glVertexAttribPointer(
    uint32_t index, int size, uint32_t type, uint8_t normalized, int stride, const void *pointer);

namespace AbyssEngine {

struct Engine;
struct Mesh;

struct ShaderBaseStruct {
    static int shaderIndexIntern;

    ShaderBaseStruct();
    ~ShaderBaseStruct();

    int ES2LoadProgram(const char *vertexShader, const char *fragmentShader);
};





// AbyssEngine::BumpShaderParticle — derives from ShaderBaseStruct (shared shader storage layout).
class BumpShaderParticle {
public:
    static int ShaderIndex;

    void Init(Engine *engine);
    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    BumpShaderParticle();

    // raw field storage (offsets referenced through the field_* helpers above)
    char field_storage[0x6c];
};

} // namespace AbyssEngine

#endif
