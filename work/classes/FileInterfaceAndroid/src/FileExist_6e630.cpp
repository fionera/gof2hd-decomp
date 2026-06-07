#include "class.h"

// FileInterfaceAndroid::FileExist(AbyssEngine::String) — probes both zip directories and, failing
// that, the filesystem (fopen) under AppRootDir. Returns whether the file exists.
//
// NOTE: near-match only. The String temporaries, the canary frame, and the AbyssEngine::String
// by-value ABI homing do not reproduce the target's exact -Oz frame layout. Faithful model.

struct sFILE;
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);
extern "C" void String_append(void *self, const void *rhs);
extern "C" void String_concat(void *out, const void *a, const void *b);
extern "C" const char *String_GetAEChar(const void *s);
extern "C" void String_dtor(void *s);
extern "C" void *zip_fopen(void *za, const char *name, int flags);
extern "C" void zip_fclose(void *zf);
extern "C" sFILE *fopen(const char *path, const char *mode);
extern "C" void fclose(sFILE *f);
extern void **gZipMain __attribute__((visibility("hidden")));
extern void **gZipPatch __attribute__((visibility("hidden")));
extern const char *gZipPrefixA __attribute__((visibility("hidden")));
extern const char *gZipPrefixB __attribute__((visibility("hidden")));
extern const char *gModeRb __attribute__((visibility("hidden")));

extern "C" bool FileInterfaceAndroid_FileExist(FileInterfaceAndroid *self, String12 name)
{
    char a[12], b[12];
    String_ctor_cstr(a, gZipPrefixA, false);
    String_append(a, &name);
    String_ctor_cstr(b, gZipPrefixB, false);
    String_append(b, &name);

    void *z1 = zip_fopen(*gZipMain, String_GetAEChar(a), 0);
    void *z2 = zip_fopen(*gZipPatch, String_GetAEChar(b), 0);

    bool exists;
    if (z1 != 0) {
        zip_fclose(z1);
        exists = true;
    } else if (z2 != 0) {
        zip_fclose(z2);
        exists = true;
    } else {
        char dir[12], full[12];
        String_ctor_cstr(dir, *(const char **)((char *)self + 0x30), false);
        String_concat(full, dir, &name);
        sFILE *f = fopen(String_GetAEChar(full), gModeRb);
        if (f != 0) {
            fclose(f);
            exists = true;
        } else {
            exists = false;
        }
        String_dtor(full);
        String_dtor(dir);
    }
    String_dtor(b);
    String_dtor(a);
    return exists;
}
