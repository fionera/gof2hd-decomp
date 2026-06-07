#include "class.h"

// ~TouchButton (0x74d7c) and ~ChoiceWindow (0x74d88) return the object pointer;
// operator delete (0x6eb48) frees it. Each owned field is deleted then nulled.
extern "C" void *TouchButton_dtor(void *p);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void operator_delete_li(void *p);

Layout::~Layout() {
    void *p = F<void *>(this, 0x3b4);
    if (p) operator_delete_li(TouchButton_dtor(p));
    F<void *>(this, 0x3b4) = 0;
    p = F<void *>(this, 0x3b8);
    if (p) operator_delete_li(TouchButton_dtor(p));
    F<void *>(this, 0x3b8) = 0;
    p = F<void *>(this, 0x3bc);
    if (p) operator_delete_li(TouchButton_dtor(p));
    F<void *>(this, 0x3bc) = 0;
    p = F<void *>(this, 0x3c4);
    if (p) operator_delete_li(ChoiceWindow_dtor(p));
    F<void *>(this, 0x3c4) = 0;
}
