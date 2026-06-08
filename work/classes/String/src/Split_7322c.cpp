#include "class.h"

extern "C" {
void *operator_new(unsigned int n);
void operator_delete(void *p);
void String_ArrayString_ctor(void *arr);                       // Array<String*>::Array()
unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_ArrayString_add(void *item, void *arr);            // ArrayAdd<String*>
void String_ArrayString_removeAll(void *arr);                  // ArrayRemoveAll<String*>
void *String_ArrayString_dtor(void *arr);                      // Array<String*>::~Array()
}

// AbyssEngine::String::Split(AbyssEngine::String sep) -> Array<String*>* (null if no splits).
extern "C" void *String_Split(String *self, String *sep)
{
    if (F<int>(self, 0x4) != 0 && F<int>(self, 0x8) != 0 && F<int>(sep, 0x8) != 0) {
        void *arr = operator_new(0xc);
        String_ArrayString_ctor(arr);

        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = String_IndexOf_from(self, pos, sep)) != 0xffffffff) {
            if (pos < idx) {
                void *piece = operator_new(0xc);
                String_SubString((String *)piece, self, pos, idx);
                String_ArrayString_add(piece, arr);
            }
            pos = F<int>(sep, 0x8) + idx;
        }
        if (pos != 0 && pos < F<uint32_t>(self, 0x8)) {
            void *piece = operator_new(0xc);
            String_SubString((String *)piece, self, pos, F<uint32_t>(self, 0x8));
            String_ArrayString_add(piece, arr);
        }

        if (*(int *)arr != 0)
            return arr;

        // No elements collected: tear the array down and report failure.
        String_ArrayString_removeAll(arr);
        operator_delete(String_ArrayString_dtor(arr));
    }
    return 0;
}
