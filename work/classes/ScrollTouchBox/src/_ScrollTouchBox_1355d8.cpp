#include "class.h"

extern "C" void ArrayReleaseClasses_StringPtr(void *self);
extern "C" void *Array_StringPtr_dtor(void *self);
void operator delete(void *ptr) noexcept;

ScrollTouchBox::~ScrollTouchBox()
{
    void *lines = this->f_0;
    if (lines != 0) {
        ArrayReleaseClasses_StringPtr(lines);
        lines = this->f_0;
        if (lines != 0)
            operator delete(Array_StringPtr_dtor(lines));
        this->f_0 = 0;
    }
}
