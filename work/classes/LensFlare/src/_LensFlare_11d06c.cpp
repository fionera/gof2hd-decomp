#include "class.h"

// LensFlare::~LensFlare()
//   if (this->images) operator delete(this->images); this->images = 0;
struct LensFlare { ~LensFlare(); };
LensFlare::~LensFlare()
{
    void *imgs = pp(this, 0x10);
    if (imgs != 0) {
        operator delete(imgs);
    }
    pp(this, 0x10) = 0;
}
