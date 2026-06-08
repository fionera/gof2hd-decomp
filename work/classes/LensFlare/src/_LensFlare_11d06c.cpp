#include "class.h"

// LensFlare::~LensFlare()
//   if (this->images) operator delete(this->images); this->images = 0;
struct LensFlare {
    // @portable-fields
    int f_0; // 0x0
    void* f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    void* f_10; // 0x10
 ~LensFlare(); };
LensFlare::~LensFlare()
{
    void *imgs = this->f_10;
    if (imgs != 0) {
        operator delete(imgs);
    }
    this->f_10 = 0;
}
