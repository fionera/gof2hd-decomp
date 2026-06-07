#include "class.h"

extern "C" Objective *_ZN9ObjectiveD2Ev(Objective *self)
{
    Array<Objective *> *children = F<Array<Objective *> *>(self, 0x10);
    if (children != 0) {
        ArrayReleaseClasses_Objective(children);
        children = F<Array<Objective *> *>(self, 0x10);
        if (children != 0)
            operator_delete(Array_ObjectivePtr_dtor(children));
    }
    F<void *>(self, 0x10) = 0;

    void *text = F<void *>(self, 0x14);
    if (text != 0) {
        typedef void (*ReleaseFn)(void *);
        ReleaseFn release = *(ReleaseFn *)((char *)*(void **)text + 4);
        release(text);
    }
    F<void *>(self, 0x14) = 0;
    return self;
}
