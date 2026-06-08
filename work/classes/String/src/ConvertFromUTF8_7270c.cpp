#include "class.h"

extern "C" {
char *String_GetAEChar(String *self);                       // 0x6ef68
uint16_t *String_getWCharFromUtf8(char *s, int len);        // 0x6f598
void String_Set_wchar(String *self, const uint16_t *s);
void operator_delete__(void *p);
void String_freeWide(void *p);                              // 0x1ab108 (tail-called delete[])
}

// AbyssEngine::String::ConvertFromUTF8() - reinterpret the stored bytes as UTF-8 and re-store.
extern "C" void String_ConvertFromUTF8(String *self)
{
    if (F<void *>(self, 0x4) == 0)
        return;

    char *bytes = String_GetAEChar(self);
    char *bytes2 = String_GetAEChar(self);
    uint16_t *wide = String_getWCharFromUtf8(bytes2, F<int>(self, 0x8));
    String_Set_wchar(self, wide);
    operator_delete__(bytes);
    String_freeWide(wide);
}
