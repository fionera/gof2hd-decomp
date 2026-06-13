#ifndef GOF2_TEXTURECONFERENCE_H
#define GOF2_TEXTURECONFERENCE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>

extern "C" void glUseProgram(uint32_t program);
extern "C" int glGetUniformLocation(uint32_t program, const char *name);
extern "C" int glGetAttribLocation(uint32_t program, const char *name);
extern "C" void glUniform1i(int location, int value);
extern "C" void glUniform1f(int location, float value);
extern "C" void glUniform4fv(int location, int count, const float *value);
extern "C" void glUniformMatrix4fv(int location, int count, uint8_t transpose, const void *value);
extern "C" void glEnableVertexAttribArray(uint32_t index);
extern "C" void glDisableVertexAttribArray(uint32_t index);
extern "C" void glBindBuffer(uint32_t target, uint32_t buffer);
extern "C" void glActiveTexture(uint32_t texture);
extern "C" void glBindTexture(uint32_t target, uint32_t texture);
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

// AbyssEngine::TextureConference — GLES2 scrolling-texture shader (derives from ShaderBaseStruct).
// Most members are accessed through the field_i32/field_u8/field_ptr helpers above; only the
// byte-match-recovered animation accumulator at +0x38 keeps a named field.
class TextureConference : public ShaderBaseStruct  {
public:
    static int ShaderIndex;

    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void Init(Engine *engine);

    // raw field storage up to the named animation accumulator at +0x38.
    char field_storage[0x38];
    long long animTime;               // +0x38
    String shaderName;                // shader name ("TextureConference")
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine17TextureConferenceE[];

#endif
