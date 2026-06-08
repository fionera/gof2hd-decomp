#include "class.h"

extern "C" {
void *operator_new(unsigned int n);
void operator_delete(void *p);
String *String_ctor_char(String *self, const char *s, bool rev);
void String_concat(String *out, String *a, String *b);
void String_assign_op(String *self, String *src);
void String_dtor(String *self);
void String_ArrayString_ctor(void *arr);
unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_ArrayString_add(void *item, void *arr);
void String_ArrayString_removeAll(void *arr);
void *String_ArrayString_dtor(void *arr);
}

struct SBuf { uint32_t a, b, c; };

// Tag delimiter fragments (PC-relative literals in the target).
static const char kOpen[]  = "<";
static const char kClose[] = ">";
static const char kSlash[] = "</";

// AbyssEngine::String::SplitTags(AbyssEngine::String tag)
// Wrap `tag` as "<tag>", split this string on it, ending each run at the matching "</".
extern "C" void String_SplitTags(String *self, String *tag)
{
    if (F<int>(self, 0x4) == 0 || F<int>(self, 0x8) == 0 || F<int>(tag, 0x8) == 0)
        return;

    // tag := "<" + tag + ">"
    {
        SBuf open, lhs, close, full;
        String_ctor_char((String *)&open, kOpen, false);
        String_concat((String *)&lhs, (String *)&open, tag);
        String_ctor_char((String *)&close, kClose, false);
        String_concat((String *)&full, (String *)&lhs, (String *)&close);
        String_assign_op(tag, (String *)&full);
        String_dtor((String *)&full);
        String_dtor((String *)&close);
        String_dtor((String *)&lhs);
        String_dtor((String *)&open);
    }

    void *arr = operator_new(0xc);
    String_ArrayString_ctor(arr);

    int endPos = 0;
    unsigned int pos = 0;
    unsigned int idx;
    while ((idx = String_IndexOf_from(self, pos, tag)) != 0xffffffff) {
        if (pos <= idx) {
            void *piece = operator_new(0xc);
            String_SubString((String *)piece, self, pos, idx);
            String_ArrayString_add(piece, arr);

            int afterTag = F<int>(tag, 0x8) + idx;
            SBuf closer;
            String_ctor_char((String *)&closer, kSlash, false);
            endPos = (int)String_IndexOf_from(self, afterTag, (String *)&closer);
            String_dtor((String *)&closer);
            if (endPos == -1)
                goto done;

            void *piece2 = operator_new(0xc);
            String_SubString((String *)piece2, self, afterTag, (unsigned int)endPos);
            String_ArrayString_add(piece2, arr);
        }
        pos = endPos + 1;
    }

    if (pos != 0 && pos < F<uint32_t>(self, 0x8)) {
        void *piece = operator_new(0xc);
        String_SubString((String *)piece, self, pos, F<uint32_t>(self, 0x8));
        String_ArrayString_add(piece, arr);
    }

    if (*(int *)arr == 0) {
        String_ArrayString_removeAll(arr);
        operator_delete(String_ArrayString_dtor(arr));
    }
done:
    ;
}
