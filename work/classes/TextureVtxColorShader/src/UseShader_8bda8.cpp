#include "class.h"

namespace AbyssEngine {

__attribute__((minsize)) void TextureVtxColorShader::UseShader(bool)
{
    int program;
    if (Engine::fogEnabled && (program = field_i32(this, 0x20)) != 0) {
        return glUseProgram(program);
    }
    return glUseProgram(field_i32(this, 0x04));
}

} // namespace AbyssEngine
