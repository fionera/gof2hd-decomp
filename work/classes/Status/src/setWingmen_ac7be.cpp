#include "class.h"

extern "C" void ArrayReleaseClasses_String(Array<String *> *a);
extern "C" void Array_String_ctor(Array<String *> *a);
extern "C" void ArraySetLength_String(uint32_t n, Array<String *> *a);
extern "C" void String_copyctor(String *dst, String *src, bool b);

// Replaces the wingmen roster with deep copies of the strings in `list` (or clears it).
void Status::setWingmen(Array<String *> *list) {
    Array<String *> *cur = (Array<String *> *)(void *)wingmen;
    if (cur != 0) {
        ArrayReleaseClasses_String(cur);
    }
    if (list == 0) {
        wingmen = 0;
        *(int32_t *)((char *)this + 0x28) = 0;
        *(int32_t *)((char *)this + 0x30) = 0;
    } else {
        Array<String *> *na = (Array<String *> *)operator new(0xc);
        Array_String_ctor(na);
        wingmen = (int32_t)(uint32_t)na;
        ArraySetLength_String(list->size, na);
        for (unsigned i = 0; i < list->size; i = i + 1) {
            String *s = (String *)operator new(0xc);
            String_copyctor(s, ((String **)list->data)[i], 0);
            ((String **)((Array<String *> *)(void *)wingmen)->data)[i] = s;
        }
    }
}
