#include "class.h"

extern "C" void  _mw_ArrayReleaseImagePart(void *a);   // 0x75ae4
extern "C" void *_mw_ImagePartArray_dtor(void *a);     // 0x75af0
extern "C" void  _mw_ArrayReleaseTouchButton(void *a); // 0x75328
extern "C" void *_mw_TouchButtonArray_dtor(void *a);   // 0x75334
extern "C" void *_mw_STW_dtor(void *w);                // 0x75afc
extern "C" void *_mw_ChoiceWindow_dtor(void *w);       // 0x74d88
extern "C" void *_mw_TouchButton_dtor(void *b);        // 0x74d7c
extern "C" void *_mw_WantedWindow_dtor(void *w);       // 0x772a8

// MissionsWindow::~MissionsWindow()
struct MissionsWindow { ~MissionsWindow(); };
MissionsWindow::~MissionsWindow()
{
    if (pp(this, 0x18)) {
        _mw_ArrayReleaseImagePart(pp(this, 0x18));
        if (pp(this, 0x18)) operator delete(_mw_ImagePartArray_dtor(pp(this, 0x18)));
    }
    pp(this, 0x18) = 0;
    if (pp(this, 0x14)) {
        _mw_ArrayReleaseTouchButton(pp(this, 0x14));
        if (pp(this, 0x14)) operator delete(_mw_TouchButtonArray_dtor(pp(this, 0x14)));
    }
    pp(this, 0x14) = 0;
    if (pp(this, 0x0)) operator delete(_mw_STW_dtor(pp(this, 0x0)));
    pp(this, 0x0) = 0;
    if (pp(this, 0x4)) operator delete(_mw_STW_dtor(pp(this, 0x4)));
    pp(this, 0x4) = 0;
    if (pp(this, 0xc)) operator delete(_mw_ChoiceWindow_dtor(pp(this, 0xc)));
    pp(this, 0xc) = 0;
    if (pp(this, 0x24)) operator delete(_mw_TouchButton_dtor(pp(this, 0x24)));
    pp(this, 0x24) = 0;
    if (pp(this, 0x28)) operator delete(_mw_TouchButton_dtor(pp(this, 0x28)));
    pp(this, 0x28) = 0;
    if (pp(this, 0x2c)) operator delete(_mw_TouchButton_dtor(pp(this, 0x2c)));
    pp(this, 0x2c) = 0;
    if (pp(this, 0x10)) operator delete(_mw_WantedWindow_dtor(pp(this, 0x10)));
    pp(this, 0x10) = 0;
}
