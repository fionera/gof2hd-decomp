#include "class.h"

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
