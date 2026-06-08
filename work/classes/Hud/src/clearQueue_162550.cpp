#include "class.h"

extern "C" void *ListItem_dtor(void *p);   // ListItem::~ListItem
extern "C" void operator_delete(void *p);

extern "C" void Hud_clearQueue(Hud *self)
{
    unsigned int i = 1;
    int off = 4;
    unsigned int *arr;
    while (arr = (unsigned int *)self->f_264, i < arr[0]) {
        unsigned int data = arr[1];
        void *item = *(void **)(data + off);
        int *dst;
        if (item == 0) {
            dst = (int *)(data + i * 4);
        } else {
            void *p = ListItem_dtor(item);
            operator_delete(p);
            dst = (int *)(I(self->f_264, 4) + off);
        }
        *dst = 0;
        off = off + 4;
        i = i + 1;
    }
    I(self, 0x270) = 0;
}
