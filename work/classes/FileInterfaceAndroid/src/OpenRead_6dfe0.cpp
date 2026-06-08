#include "class.h"

// FileInterfaceAndroid::OpenRead(AbyssEngine::String, int, bool, int, int, unsigned int) —
// resolves the requested file name against the two zip archives (or the filesystem under
// AppRootDir) and, when found, wraps the handle in a freshly-allocated FileInterfaceAndroid.
//
// NOTE: near-match only. The leading-'/' name normalization, the fprintf debug line, the String
// temporaries, the stack-protector canary and the placement-construction of the result object do
// not reproduce the target's exact AbyssEngine::String ABI and -Oz frame layout. Faithful model.

struct sFILE;
struct zip_file;
extern "C" const unsigned short *String_GetAEWChar(const void *s);
extern "C" const char *String_GetAEChar(const void *s);
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);
extern "C" void String_ctor_wide(void *out, const unsigned short *w, bool);
extern "C" void String_append(void *self, const void *rhs);
extern "C" void String_concat(void *out, const void *a, const void *b);
extern "C" void String_dtor(void *s);
extern "C" zip_file *zip_fopen(void *za, const char *name, int flags);
extern "C" sFILE *fopen(const char *path, const char *mode);
extern "C" void *operator_new(unsigned int n);
extern "C" FileInterfaceAndroid *FileInterfaceAndroid_ctor_zip(FileInterfaceAndroid *self,
                                                               zip_file *zf, bool append,
                                                               int start, int p4, int p5);

extern void **gZipMainR __attribute__((visibility("hidden")));
extern void **gZipPatchR __attribute__((visibility("hidden")));
extern const char *gPrefixSlash __attribute__((visibility("hidden")));
extern const char *gPrefixPlain __attribute__((visibility("hidden")));
extern const char *gOpenReadFmt __attribute__((visibility("hidden")));
extern char *gStderrBase __attribute__((visibility("hidden")));
extern const char *gModeRbR __attribute__((visibility("hidden")));
extern void *gFIAVtable_or __attribute__((visibility("hidden")));
extern int *gFIAInstCount_or __attribute__((visibility("hidden")));
extern "C" int fprintf(void *stream, const char *fmt, ...);

extern "C" FileInterfaceAndroid *FileInterfaceAndroid_OpenRead(FileInterfaceAndroid *self,
                                                               String12 name, int p2, bool p3,
                                                               int p4, int p5, unsigned int p6)
{
    const unsigned short *w = String_GetAEWChar(&name);
    if (self->f_4 == 0)
        return 0;

    const unsigned short *body = (*w == '/') ? w + 1 : w;

    char a[12], wide[12], b[12], wide2[12];
    String_ctor_cstr(a, gPrefixSlash, false);
    String_ctor_wide(wide, body, false);
    String_append(a, wide);
    String_dtor(wide);
    String_ctor_cstr(b, gPrefixPlain, false);
    String_ctor_wide(wide2, body, false);
    String_append(b, wide2);
    String_dtor(wide2);

    fprintf((char *)*(void **)gStderrBase + 0xa8, gOpenReadFmt, String_GetAEChar(b), p3, p4, p5, p6, p2);

    zip_file *z1 = zip_fopen(*gZipMainR, String_GetAEChar(a), 0);
    zip_file *z2 = (*gZipPatchR != 0) ? zip_fopen(*gZipPatchR, String_GetAEChar(b), 0) : 0;

    FileInterfaceAndroid *result = 0;
    if (z1 != 0) {
        result = FileInterfaceAndroid_ctor_zip((FileInterfaceAndroid *)operator_new(0x38),
                                               z1, (bool)p4, p2, p5, p4);
    } else if (z2 != 0) {
        result = FileInterfaceAndroid_ctor_zip((FileInterfaceAndroid *)operator_new(0x38),
                                               z2, (bool)p4, p2, p5, p4);
    } else if (*(const char **)((char *)self + 0x30) != 0) {
        char dir[12], full[12];
        String_ctor_cstr(dir, *(const char **)((char *)self + 0x30), false);
        String_concat(full, dir, body == w ? (void *)a : (void *)a);
        sFILE *f = fopen(String_GetAEChar(full), gModeRbR);
        if (f != 0) {
            int *p = (int *)operator_new(0x38);
            p[2] = (int)f;
            p[3] = 0;
            p[4] = 0;
            *(unsigned char *)(p + 5) = 0;
            p[0] = (int)((char *)gFIAVtable_or + 8);
            int *cnt = gFIAInstCount_or;
            *cnt = *cnt + 1;
            result = (FileInterfaceAndroid *)p;
        }
        String_dtor(full);
        String_dtor(dir);
    }
    String_dtor(b);
    String_dtor(a);
    return result;
}
