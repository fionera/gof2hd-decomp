#include "class.h"

namespace AbyssEngine {

void BumpShaderV2::SetInActive()
{
    if (this->f_20 >= 0)
        glDisableVertexAttribArray(this->f_20);
    if (this->f_24 >= 0)
        glDisableVertexAttribArray(this->f_24);
    if (this->f_28 >= 0)
        glDisableVertexAttribArray(this->f_28);
    if (this->f_2c >= 0)
        glDisableVertexAttribArray(this->f_2c);
    if (this->f_30 >= 0)
        glDisableVertexAttribArray(this->f_30);
}

} // namespace AbyssEngine
