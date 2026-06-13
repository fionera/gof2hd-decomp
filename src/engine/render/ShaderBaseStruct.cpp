#include "gof2/engine/render/ShaderBaseStruct.h"
#include "gof2/game/core/String.h"

namespace AbyssEngine {

// Engine-wide counter of constructed shaders; each shader snapshots it as its index.
int ShaderBaseStruct::shaderIndexIntern;

} // namespace AbyssEngine

namespace AbyssEngine {

String ShaderBaseStruct::GetShaderName()
{
    String copy;
    copy.s = shader_name(this)->s;
    return copy;
}

} // namespace AbyssEngine

namespace AbyssEngine {

ShaderBaseStruct::~ShaderBaseStruct()
{
    shader_vtable(this) = (char *)_ZTVN11AbyssEngine16ShaderBaseStructE + 8;
    shader_name(this)->~String();
}

} // namespace AbyssEngine

namespace AbyssEngine {

void ShaderBaseStruct::UseShader()
{
    glUseProgram(shader_program(this));
}

} // namespace AbyssEngine

namespace AbyssEngine {

void ShaderBaseStruct::DeleteShader()
{
    glDeleteProgram(shader_program(this));
}

} // namespace AbyssEngine

namespace AbyssEngine {

void ShaderBaseStruct::Update()
{
    shader_dirty(this) = 1;
}

} // namespace AbyssEngine

namespace AbyssEngine {

ShaderBaseStruct::ShaderBaseStruct()
{
    shader_vtable(this) = (char *)_ZTVN11AbyssEngine16ShaderBaseStructE + 8;
    new (shader_name(this)) String();
    program = -1;
    flags = 0x100;
    ++shaderIndexIntern;
    shader_vertex_path(this) = 0;
    shader_fragment_path(this) = 0;

    // base name initialized to the empty string (concrete shaders overwrite it)
    name = "";
}

} // namespace AbyssEngine

namespace AbyssEngine {

uint32_t ShaderBaseStruct::ES2LoadProgram(const char *vertexSource, const char *fragmentSource)
{
    uint32_t program = 0;

    uint32_t vertexShader = this->ES2LoadShader(0x8b31, vertexSource);
    if (vertexShader != 0) {
        uint32_t fragmentShader = ((ShaderBaseStruct *)(vertexShader))->ES2LoadShader(0x8b30, fragmentSource);
        if (fragmentShader == 0) {
            glDeleteShader(vertexShader);
        } else {
            program = glCreateProgram();
            if (program != 0) {
                glAttachShader(program, vertexShader);
                glAttachShader(program, fragmentShader);
                glLinkProgram(program);

                int status;
                glGetProgramiv(program, 0x8b82, &status);
                if (status != 0) {
                    glDeleteShader(vertexShader);
                    glDeleteShader(fragmentShader);

                    char *name = (char *)((String *)(shader_name(this)))->GetAEChar();
                    AELabelObject(0x8b40, program, name);
                    operator delete[](name);
                } else {
                    int logLength = 0;
                    glGetProgramiv(program, 0x8b84, &logLength);
                    if (logLength >= 2) {
                        void *log = malloc(logLength);
                        glGetProgramInfoLog(program, logLength, 0, log);
                        free(log);
                    }
                    glDeleteProgram(program);
                    program = 0;
                }
            }
        }
    }

    return program;
}

} // namespace AbyssEngine

namespace AbyssEngine {

uint32_t ShaderBaseStruct::ES2LoadShader(uint32_t type, const char *source)
{
    const char *localSource;
    localSource = source;
    uint32_t shader = glCreateShader(type);
    uint32_t shaderId = shader;

    if (shaderId != 0) {
        int status;

        glShaderSource(shaderId, 1, &localSource, 0);
        glCompileShader(shaderId);
        glGetShaderiv(shaderId, 0x8b81, &status);
        if (status != 0) {
            goto done;
        }

        int logLength = 0;
        glGetShaderiv(shaderId, 0x8b84, &logLength);
        if (logLength >= 2) {
            void *log = malloc(logLength);
            glGetShaderInfoLog(shaderId, logLength, 0, log);
            free(log);
        }
        glDeleteShader(shaderId);
        shader = 0;
    }

done:
    return shader;
}

} // namespace AbyssEngine

namespace AbyssEngine {

uint32_t ShaderBaseStruct::LoadBindShader(const char *vertexPath, const char *fragmentPath)
{
    register const char *fragment = fragmentPath;
    uint32_t result = 0;

    if (vertexPath != 0 && fragment != 0) {
        shader_vertex_path(this) = vertexPath;
        shader_fragment_path(this) = fragment;

        uint32_t handle;
        if (AEFile::OpenRead(vertexPath, &handle) != 0) {
            uint32_t vertexSize = AEFile::GetFileSize(handle);
            uint32_t allocSize = vertexSize + 1;
            if ((int)vertexSize < -1) {
                allocSize = 0xffffffff;
            }
            char *vertexSource = (char *)operator new[](allocSize);
            AEFile::Read(vertexSize, vertexSource, handle);
            AEFile::Close(handle);
            vertexSource[vertexSize] = 0;

            if (AEFile::OpenRead(fragment, &handle) != 0) {
                uint32_t fragmentSize = AEFile::GetFileSize(handle);
                allocSize = fragmentSize + 1;
                if ((int)fragmentSize < -1) {
                    allocSize = 0xffffffff;
                }
                char *fragmentSource = (char *)operator new[](allocSize);
                AEFile::Read(fragmentSize, fragmentSource, handle);
                AEFile::Close(handle);
                fragmentSource[fragmentSize] = 0;

                result = ES2LoadProgram(vertexSource, fragmentSource);
                operator delete[](fragmentSource);
                operator delete[](vertexSource);
            }
        }
    }

    return result;
}

} // namespace AbyssEngine
