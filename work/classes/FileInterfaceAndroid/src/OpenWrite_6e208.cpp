#include "class.h"

// FileInterfaceAndroid::OpenWrite(AbyssEngine::String, int, bool, unsigned int) — static.
// Builds "<dir><name>" from the AppRootDir prefix and the requested file name, fopen("..","wb"),
// and on success wraps the FILE* in a freshly-allocated FileInterfaceAndroid.
//
// NOTE: near-match only. The String concatenation temporaries, the stack-protector canary frame,
// and the placement-construction of the result object do not reproduce the target's exact
// AbyssEngine::String ABI homing and -Oz frame layout. Faithful semantic model.

struct sFILE;
extern "C" const unsigned short *String_GetAEWChar(const void *s);
extern "C" const char *String_GetAEChar(const void *s);
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);
extern "C" void String_ctor_wide(void *out, const unsigned short *w, bool);
extern "C" void String_concat(void *out, const void *a, const void *b);
extern "C" void String_dtor(void *s);
extern "C" sFILE *fopen(const char *path, const char *mode);
extern "C" void *operator_new(unsigned int n);
extern void *gFIAVtable_ow __attribute__((visibility("hidden")));
extern int *gFIAInstCount_ow __attribute__((visibility("hidden")));
extern const char *gModeWb __attribute__((visibility("hidden")));

extern "C" FileInterfaceAndroid *FileInterfaceAndroid_OpenWrite(FileInterfaceAndroid *self,
                                                                String12 name, int, bool, unsigned int)
{
    const unsigned short *w = String_GetAEWChar(&name);
    while (*w)
        ++w;

    char dir[12], wide[12], full[12];
    String_ctor_cstr(dir, *(const char **)((char *)self + 0x30), false);
    String_ctor_wide(wide, String_GetAEWChar(&name), false);
    String_concat(full, dir, wide);
    String_dtor(wide);

    sFILE *f = fopen(String_GetAEChar(full), gModeWb);
    FileInterfaceAndroid *result = 0;
    if (f != 0) {
        int *p = (int *)operator_new(0x38);
        p[2] = (int)f;
        p[3] = 0;
        p[4] = 0;
        *(unsigned char *)(p + 5) = 1;
        p[0] = (int)((char *)gFIAVtable_ow + 8);
        int *cnt = gFIAInstCount_ow;
        *cnt = *cnt + 1;
        result = (FileInterfaceAndroid *)p;
    }
    String_dtor(full);
    String_dtor(dir);
    return result;
}
