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
// Holds the GL program handle, the dirty flag, the shader name, the attribute/uniform locations
// and the scrolling-animation accumulator. Layout matches the sibling AbyssEngine shaders.
class TextureConference : public ShaderBaseStruct  {
public:
    static int ShaderIndex;

    int       programHandle;          // +0x04  GL program handle (ES2LoadProgram result)
    uint8_t   offsetDirty;            // +0x09  upload-u_offset flag
    String    shaderName;             // +0x0c  shader name ("TextureConference")
    int       attribPosition;         // +0x20  attrib a_position
    int       attribTexCoord;         // +0x24  attrib a_texCoord
    int       colorLoc;               // +0x28  uniform u_color
    int       mvpMatrixLoc;           // +0x2c  uniform u_mvp
    int       offsetLoc;              // +0x30  uniform u_offset
    int       textureLoc;             // +0x34  uniform u_texture (sampler)
    long long animTime;               // +0x38  scrolling-animation accumulator

    void SetInActive();
    void UpdateMeshData(Mesh *mesh, Engine *engine);
    void Init(Engine *engine);
};

} // namespace AbyssEngine

extern "C" char _ZTVN11AbyssEngine17TextureConferenceE[];

#endif
