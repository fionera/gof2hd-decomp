#include "class.h"

using namespace AbyssEngine;

// AEGeometry::addChild(unsigned int child)
struct AEGeometry { void addChild(uint32_t child); };
void AEGeometry::addChild(uint32_t child)
{
    PaintCanvas::TransformAddChild((PaintCanvas *)this->f_2c, this->f_c, child);
    uint32_t old = this->f_14;
    if (old != 0xffffffffu)
        this->f_10 = old;
    this->f_14 = child;
}
