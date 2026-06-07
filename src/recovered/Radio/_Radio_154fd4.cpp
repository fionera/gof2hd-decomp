#include "class.h"

Radio::~Radio()
{
    void *array = F<void *>(this, 0x0c);
    if (array != 0) {
        ArrayReleaseClasses_ImagePart(array);
        array = F<void *>(this, 0x0c);
        if (array != 0) {
            operator_delete(Array_ImagePart_dtor(array));
        }
    }
    F<void *>(this, 0x0c) = 0;

    void *parts = F<void *>(this, 0x10);
    if (parts != 0) {
        operator_delete__(parts);
    }
    F<void *>(this, 0x10) = 0;

    array = F<void *>(this, 0x08);
    if (array != 0) {
        ArrayReleaseClasses_String(array);
        array = F<void *>(this, 0x08);
        if (array != 0) {
            operator_delete(Array_String_dtor(array));
        }
    }
    F<void *>(this, 0x08) = 0;
}
