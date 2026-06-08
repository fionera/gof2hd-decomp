#include "class.h"

// MenuTouchWindow::~MenuTouchWindow(). Tears down all owned arrays, buttons, choice window,
// scroll windows and two heap buffers, then returns this (base dtor is not chained here).
extern "C" void operator_delete(void *p);
extern "C" void operator_delete_arr(void *p);                 // operator delete[]
extern "C" void _mtw_ArrayReleaseClasses_TB(void *arr);       // Array<TouchButton*> release
extern "C" void *_mtw_Array_TB_dtor(void *arr);
extern "C" void _mtw_ArrayReleaseClasses_TS(void *arr);       // Array<TouchSlider*> release
extern "C" void *_mtw_Array_TS_dtor(void *arr);
extern "C" void _mtw_ArrayReleaseClasses_Str(void *arr);      // Array<String*> release
extern "C" void *_mtw_Array_Str_dtor(void *arr);
extern "C" void *_mtw_TouchButton_dtor(void *p);
extern "C" void *_mtw_ChoiceWindow_dtor(void *p);
extern "C" void *_mtw_ScrollTouchWindow_dtor(void *p);

static inline void freeArrayTB(void *self, int off) {
    void *a = pp(self, off);
    if (a != 0) {
        _mtw_ArrayReleaseClasses_TB(a);
        void *b = pp(self, off);
        if (b != 0) operator_delete(_mtw_Array_TB_dtor(b));
    }
    pp(self, off) = 0;
}
static inline void freeObj(void *self, int off, void *(*dtor)(void *)) {
    void *o = pp(self, off);
    if (o != 0) operator_delete(dtor(o));
    pp(self, off) = 0;
}

struct MenuTouchWindow { MenuTouchWindow *dtor(); };
MenuTouchWindow *MenuTouchWindow::dtor()
{
    void *self = this;

    freeArrayTB(self, 0x4);
    freeArrayTB(self, 0xac);
    freeArrayTB(self, 0xc0);
    freeArrayTB(self, 0xb4);
    freeArrayTB(self, 0xb8);
    freeArrayTB(self, 0xb0);

    // Array<TouchSlider*> at 0xec
    {
        void *a = pp(self, 0xec);
        if (a != 0) {
            _mtw_ArrayReleaseClasses_TS(a);
            void *b = pp(self, 0xec);
            if (b != 0) operator_delete(_mtw_Array_TS_dtor(b));
        }
        pp(self, 0xec) = 0;
    }

    freeArrayTB(self, 0xf8);

    // Array<String*> at 0x9c and 0xa0
    {
        int offs[2] = {0x9c, 0xa0};
        for (int k = 0; k < 2; k++) {
            int off = offs[k];
            void *a = pp(self, off);
            if (a != 0) {
                _mtw_ArrayReleaseClasses_Str(a);
                void *b = pp(self, off);
                if (b != 0) operator_delete(_mtw_Array_Str_dtor(b));
            }
            pp(self, off) = 0;
        }
    }

    // single TouchButton members
    freeObj(self, 0xcc, _mtw_TouchButton_dtor);
    freeObj(self, 0xc4, _mtw_TouchButton_dtor);
    freeObj(self, 0xd0, _mtw_TouchButton_dtor);
    freeObj(self, 0xd4, _mtw_TouchButton_dtor);
    freeObj(self, 0xd8, _mtw_TouchButton_dtor);
    freeObj(self, 0xdc, _mtw_TouchButton_dtor);

    freeObj(self, 0x104, _mtw_ChoiceWindow_dtor);
    freeObj(self, 0xc8, _mtw_TouchButton_dtor);
    freeObj(self, 0xe8, _mtw_TouchButton_dtor);

    freeObj(self, 0xf0, _mtw_ScrollTouchWindow_dtor);
    freeObj(self, 0xf4, _mtw_ScrollTouchWindow_dtor);

    // two heap byte-buffers freed with operator delete[]
    if (pp(self, 0x134) != 0) operator_delete_arr(pp(self, 0x134));
    pp(self, 0x134) = 0;
    if (pp(self, 0x138) != 0) operator_delete_arr(pp(self, 0x138));
    pp(self, 0x138) = 0;

    return this;
}
