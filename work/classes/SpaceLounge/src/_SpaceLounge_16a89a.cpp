#include "class.h"

extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void *CutScene_dtor(void *p);
extern "C" void *Array_StringPtr_dtor(void *p);
extern "C" void *Array_TouchButtonPtr_dtor(void *p);
extern "C" void *Array_ImagePartPtr_dtor(void *p);
extern "C" void *Array_ArrayImagePartPtr_dtor(void *p);
extern "C" void *Array_VectorPtr_dtor(void *p);
extern "C" void ArrayRelease_StringPtr(void *p);
extern "C" void ArrayRelease_TouchButtonPtr(void *p);
extern "C" void ArrayRelease_ImagePartPtr(void *p);
extern "C" void ArrayRelease_ArrayImagePartPtr(void *p);
extern "C" void ArrayRelease_VectorPtr(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void String_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void operator_delete_arr(void *p);

extern "C" void *_ZN11SpaceLoungeD2Ev(SpaceLounge *self)
{
    void *p = self->f_8;
    if (p != 0) {
        operator_delete(ChoiceWindow_dtor(p));
    }
    self->f_8 = 0;

    p = self->f_44;
    if (p != 0) {
        operator_delete(CutScene_dtor(p));
    }
    self->f_44 = 0;

    operator_delete_arr(self->f_58);
    self->f_58 = 0;

    p = self->f_28;
    if (p != 0) {
        ArrayRelease_StringPtr(p);
        p = self->f_28;
        if (p != 0) {
            operator_delete(Array_StringPtr_dtor(p));
        }
    }
    self->f_28 = 0;

    p = self->f_5c;
    if (p != 0) {
        ArrayRelease_TouchButtonPtr(p);
        p = self->f_5c;
        if (p != 0) {
            operator_delete(Array_TouchButtonPtr_dtor(p));
        }
    }
    self->f_5c = 0;

    if (I(self, 0x3c) != 0) {
        return self;
    }
    I(self, 0x3c) = 0;

    p = self->f_38;
    if (p != 0) {
        unsigned i = 0;
        for (; i < U(p, 0x0); ++i) {
            void **data = (void **)p->f_4;
            void *inner = data[i];
            ArrayRelease_ImagePartPtr(inner);
            inner = ((void **)P(self->f_38, 0x4))[i];
            if (inner != 0) {
                operator_delete(Array_ImagePartPtr_dtor(inner));
                ((void **)P(self->f_38, 0x4))[i] = 0;
            } else {
                data[i] = 0;
            }
            p = self->f_38;
        }
        ArrayRelease_ArrayImagePartPtr(p);
        p = self->f_38;
        if (p != 0) {
            operator_delete(Array_ArrayImagePartPtr_dtor(p));
        }
    }
    self->f_38 = 0;

    p = self->f_40;
    if (p != 0) {
        ArrayRelease_VectorPtr(p);
        p = self->f_40;
        if (p != 0) {
            operator_delete(Array_VectorPtr_dtor(p));
        }
    }
    self->f_40 = 0;

    p = self->f_48;
    if (p != 0) {
        operator_delete(EaseInOutMatrix_dtor(p));
    }
    self->f_48 = 0;

    p = self->f_c0;
    if (p != 0) {
        operator_delete(p);
    }
    self->f_c0 = 0;
    String_dtor(B(self, 0xa4));
    return self;
}
