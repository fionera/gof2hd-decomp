#include "class.h"

namespace AbyssEngine {

void TextureLightShader::SetInActive()
{
    glDisableVertexAttribArray(this->f_20);
    glDisableVertexAttribArray(this->f_24);
    return glDisableVertexAttribArray(this->f_28);
}

} // namespace AbyssEngine
