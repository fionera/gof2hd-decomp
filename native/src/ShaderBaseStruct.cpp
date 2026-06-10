#include "ShaderBaseStruct.h"




// ---- GetShaderName_839d0.cpp ----
namespace AbyssEngine {

String ShaderBaseStruct::GetShaderName()
{
    return String(*shader_name(this), false);
}

} // namespace AbyssEngine

// ---- _ShaderBaseStruct_8e4c4.cpp ----
namespace AbyssEngine {

ShaderBaseStruct::~ShaderBaseStruct()
{
    shader_vtable(this) = (uint32_t)ShaderBaseStruct_vtable + 8;
    shader_name(this)->~String();
}

} // namespace AbyssEngine

// ---- UseShader_8e744.cpp ----
namespace AbyssEngine {

void ShaderBaseStruct::UseShader()
{
    glUseProgram(shader_program(this));
}

} // namespace AbyssEngine

// ---- DeleteShader_8e4ea.cpp ----
namespace AbyssEngine {

void ShaderBaseStruct::DeleteShader()
{
    glDeleteProgram(shader_program(this));
}

} // namespace AbyssEngine

// ---- Update_8e74a.cpp ----
namespace AbyssEngine {

void ShaderBaseStruct::Update()
{
    shader_dirty(this) = 1;
}

} // namespace AbyssEngine

// ---- ShaderBaseStruct_8e424.cpp ----
using AbyssEngine::ShaderBaseStruct;
using AbyssEngine::String;
using AbyssEngine::shader_name;
using AbyssEngine::shader_paths;

extern "C" ShaderBaseStruct *ShaderBaseStruct_8e424(ShaderBaseStruct *self)
{
    register uint32_t *stackGuardPtr = &__stack_chk_guard;
    volatile uint32_t stackGuard = *stackGuardPtr;

    shader_vtable(self) = (uint32_t)ShaderBaseStruct_vtable + 8;
    new (shader_name(self)) String();
    *(volatile uint32_t *)((char *)self + 0x4) = 0xffffffff;
    *(volatile uint16_t *)((char *)self + 0x8) = 0x100;
    ++ShaderBaseStruct_count;
    shader_paths(self) = 0;

    char nameStorage[sizeof(String)];
    new ((String *)nameStorage) String("", false);
    *shader_name(self) = *(String *)nameStorage;
    ((String *)nameStorage)->~String();

    uint32_t stackDifference = *stackGuardPtr - stackGuard;
    if (stackDifference != 0) {
        __stack_chk_fail(stackDifference);
    }
    return self;
}

// ---- ES2LoadProgram_8e584.cpp ----
namespace AbyssEngine {

uint32_t ShaderBaseStruct::ES2LoadProgram(const char *vertexSource, const char *fragmentSource)
{
    uint32_t guard = __stack_chk_guard;
    volatile uint32_t stackGuard = guard;
    uint32_t program = 0;

    uint32_t vertexShader = ShaderBaseStruct_ES2LoadShader(guard, 0x8b31, vertexSource);
    if (vertexShader != 0) {
        uint32_t fragmentShader = ShaderBaseStruct_ES2LoadShader(vertexShader, 0x8b30, fragmentSource);
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

                    char *name = shader_name(this)->GetAEChar();
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

    uint32_t stackDifference = __stack_chk_guard - stackGuard;
    if (stackDifference == 0) {
        return program;
    }
    __stack_chk_fail(stackDifference);
}

} // namespace AbyssEngine

// ---- ES2LoadShader_8e4f0.cpp ----
namespace AbyssEngine {

uint32_t ShaderBaseStruct::ES2LoadShader(uint32_t type, const char *source)
{
    volatile uint32_t stackGuard;
    const char *localSource;
    uint32_t guard = __stack_chk_guard;
    localSource = source;
    stackGuard = guard;
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
    uint32_t stackDifference = __stack_chk_guard - stackGuard;
    if (stackDifference == 0) {
        return shader;
    }
    __stack_chk_fail(stackDifference);
}

} // namespace AbyssEngine

// ---- LoadBindShader_8e674.cpp ----
namespace AbyssEngine {

uint32_t ShaderBaseStruct::LoadBindShader(const char *vertexPath, const char *fragmentPath)
{
    volatile uint32_t stackGuard = __stack_chk_guard;
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

    uint32_t savedGuard = stackGuard;
    uint32_t currentGuard = *(volatile uint32_t *)&__stack_chk_guard;
    uint32_t stackDifference = currentGuard - savedGuard;
    if (stackDifference == 0) {
        return result;
    }
    __stack_chk_fail(stackDifference);
}

} // namespace AbyssEngine
