#include "class.h"

namespace AbyssEngine {

void BumpShader::SetInActive()
{
    if (this->f_20 >= 0)
        glDisableVertexAttribArray(this->f_20);
    if (this->f_24 >= 0)
        glDisableVertexAttribArray(this->f_24);
    if (this->f_28 >= 0)
        glDisableVertexAttribArray(this->f_28);
}

} // namespace AbyssEngine
