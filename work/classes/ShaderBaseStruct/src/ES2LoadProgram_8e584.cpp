#include "class.h"

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
