#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodChildTransform(unsigned int)
struct AEGeometry { void setLodChildTransform(uint32_t param); };
void AEGeometry::setLodChildTransform(uint32_t param)
{
    int count = this->f_50;
    if (count > 0) {
        uint32_t *arr = new uint32_t[count];
        this->f_58 = arr;
        for (int i = 0; i < count; i++) {
            PaintCanvas::TransformAddChild((PaintCanvas *)this->f_2c,
                                           ((uint32_t *)this->f_54)[i], param);
            count = this->f_50;
        }
    }
}
