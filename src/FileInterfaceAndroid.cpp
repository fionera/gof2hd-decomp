#include "gof2/FileInterfaceAndroid.h"
#include "gof2/String.h"
#include <cstdio>
#include <cstdlib>

struct zip_file;
using AbyssEngine::String12;

extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);
extern "C" unsigned int zip_fread(void *zf, void *buf, unsigned int n);
extern "C" unsigned int JNI_CallIntMethod(void *env, void *m, void *arg0, void *arg1);
extern "C" void *FileInterfaceAndroid_completeDtor(FileInterfaceAndroid *self);
extern "C" void JNI_CallVoidMethod(void *env, void *m, void *arg, ...);
extern "C" void String_append(void *self, const void *rhs);
extern "C" void String_concat(void *out, const void *a, const void *b);
extern "C" zip_file *zip_fopen(void *za, const char *name, int flags);
extern "C" int zip_fclose(void *zf);
extern "C" const unsigned short *String_GetAEWChar(const void *s);
extern "C" void String_ctor_wide(void *out, const unsigned short *w, bool);

// ---- GetDirPreFix_6e80c.cpp ----
// AbyssEngine::String::String(String* out, const char* cstr, bool) -> void (0x6ee18)

// FileInterfaceAndroid::GetDirPreFix() -> String built from a literal prefix (sret in r0).
// The copy ctor returns void, so the compiler keeps a frame and restores the sret pointer.
extern const char kDirPreFix[] __attribute__((visibility("hidden")));


RetStr FileInterfaceAndroid_GetDirPreFix()
{
    RetStr r;
    String_ctor_cstr(&r, kDirPreFix, false);
    return r;
}

// ---- GetFileSize_6e5e0.cpp ----
// FileInterfaceAndroid::GetFileSize() — seek end, tell, seek start; FILE* at +0x8.
int FileInterfaceAndroid::GetFileSize() {
    FileInterfaceAndroid *self = this;
    fseek((FILE *)self->file, 0, 2);
    int size = ftell((FILE *)self->file);
    fseek((FILE *)self->file, 0, 0);
    return size;
}

// ---- SetZipDirectory_6e7fa.cpp ----
// FileInterfaceAndroid::SetZipDirectory(void*) — cbz r1; str r1,[r0,#0x34]; bx lr
void FileInterfaceAndroid::SetZipDirectory(void *p) {
    FileInterfaceAndroid *self = this;
    if (p != 0)
        self->zipDirectory = p;
}

// ---- _FileInterfaceAndroid_6df88.cpp ----
// Complete-object destructor (D2). Re-establishes the vptr, closes any open handle, then either
// decrements the live-instance count or, when it is already zero, clears the alive byte at +0x4.
// Returns `this` (ARM EABI D1/D2 convention -> mov r0,r4; pop).
extern void *gFIAVtable_dtor __attribute__((visibility("hidden")));
extern int *gFIAInstCount_dtor __attribute__((visibility("hidden")));

FileInterfaceAndroid::~FileInterfaceAndroid()
{
    this->field_0x0 = (char *)gFIAVtable_dtor + 8;
    ((FileInterfaceAndroid *)(this))->Close();
    int *cnt = gFIAInstCount_dtor;
    if (*cnt != 0)
        *cnt = *cnt - 1;
    else
        this->alive = 0;
}

// ---- SetAppRootDir_6e7f4.cpp ----
// FileInterfaceAndroid::SetAppRootDir(void*) — cbz r1; str r1,[r0,#0x30]; bx lr
void FileInterfaceAndroid::SetAppRootDir(void *p) {
    FileInterfaceAndroid *self = this;
    if (p != 0)
        self->appRootDir = (const char *)p;
}

// ---- FileInterfaceAndroid_6de18.cpp ----
struct zip_file;
extern void *gFIAVtable_zip __attribute__((visibility("hidden")));
extern int *gFIAInstCount_zip __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid(zip_file*, bool, int, int, int) — wraps a libzip
// handle, bumps the instance count, then Seeks to the start offset. Returns `this`.
// NOTE: near-match (29/29 instr multiset identical; 2 instrs one scheduling slot off). clang
// -Oz orders the early `strd` and the `add.w r1,r2,#8` vptr finalize slightly differently from
// the target. Scheduling-only.
FileInterfaceAndroid * FileInterfaceAndroid::ctor_zip(zip_file *zf, bool append, int start, int p4, int p5) {
    FileInterfaceAndroid *self = this;
    int *cnt = gFIAInstCount_zip;
    char *vtbase = (char *)gFIAVtable_zip;   // *gVtable loaded; +8 applied at the store
    self->file = 0;
    self->zipFile = zf;
    self->jniStream = 0;
    self->modeFlag = 0;
    self->field_0x0 = vtbase + 8;
    *cnt = *cnt + 1;
    self->field_0x1c = 0;
    self->field_0x28 = 0;
    ((FileInterfaceAndroid *)(self))->Seek(start);
    self->field_0x24 = append;
    return self;
}

// ---- Read_6e2e4.cpp ----
// FileInterfaceAndroid::Read(unsigned int, void*) — reads from the zip handle, the stdio handle,
// or the JNI input stream (in which case the bytes are copied out of a Java byte[] region).
//
// NOTE: near-match only. The zip/stdio fast paths are straightforward, but the JNI path builds an
// AbyssEngine::String per byte under a stack-protector canary; that loop, the String ABI, and the
// -Oz frame layout do not reproduce byte-exact. Faithful semantic model.

extern void **gEnvR __attribute__((visibility("hidden")));
extern void *gReadMidArg __attribute__((visibility("hidden")));

typedef void *(*fn_i)(void *env, unsigned int n);
typedef void (*fn_getregion)(void *env, void *arr, int start, unsigned int len, void *buf);
typedef int (*fn_check)(void *env);
typedef void (*fn_void)(void *env);
typedef void (*fn_del)(void *env, void *arr);

bool FileInterfaceAndroid::Read(unsigned int n, void *buf) {
    FileInterfaceAndroid *self = this;
    if (self->zipFile != 0)
        return zip_fread(self->zipFile, buf, n) == n;
    if (self->file != 0)
        return fread(buf, 1, n, (FILE *)self->file) == n;
    if (self->jniStream == 0)
        return false;

    void *r9 = *gEnvR;
    void *env = *(void **)r9;
    void *table = *(void **)env;
    void *arr = (*(fn_i *)((char *)table + 0x2c0))(env, n);
    unsigned int got = JNI_CallIntMethod(*(void **)r9, self->jniStream, *(void **)gReadMidArg, arr);

    bool ok;
    table = *(void **)(*(void **)r9);
    if ((*(fn_check *)((char *)table + 0x3c))(*(void **)r9) == 0 && got == n) {
        env = *(void **)r9;
        table = *(void **)env;
        (*(fn_getregion *)((char *)table + 0x320))(env, arr, 0, n, buf);
        ok = true;
    } else {
        table = *(void **)(*(void **)r9);
        (*(fn_void *)((char *)table + 0x40))(*(void **)r9);
        (*(fn_void *)((char *)(*(void **)(*(void **)r9)) + 0x44))(*(void **)r9);
        ok = false;
    }
    table = *(void **)(*(void **)r9);
    (*(fn_del *)((char *)table + 0x5c))(*(void **)r9, arr);
    return ok;
}

// ---- Seek_6e590.cpp ----
// FileInterfaceAndroid::Seek(unsigned int) — skip `n` bytes forward. For a zip handle this means
// reading (and discarding) into a scratch buffer; for a stdio handle a relative fseek.
// NOTE: near-match (33/33 instruction multiset identical; ~2 instrs differ in scheduling slot —
// where `n` and the malloc result are spilled to callee-saved regs relative to their cbz checks).
bool FileInterfaceAndroid::Seek(unsigned int n) {
    FileInterfaceAndroid *self = this;
    if (n == 0)
        return true;
    void *zf = self->zipFile;
    int delta;
    if (zf != 0) {
        void *tmp = malloc(n);
        if (tmp == 0)
            return false;
        unsigned int got = zip_fread(zf, tmp, n);
        free(tmp);
        delta = got - n;
    } else {
        void *f = self->file;
        if (f == 0)
            return false;
        delta = fseek((FILE *)f, n, 1);
    }
    return delta == 0;
}

// ---- _FileInterfaceAndroid_6dfce.cpp ----
// FileInterfaceAndroid::~FileInterfaceAndroid() [deleting destructor] — runs the complete-object
// dtor (external), then tail-calls operator delete:
//   blx <complete dtor> ; pop {r7,lr} ; b.w <operator delete>

void _ZN20FileInterfaceAndroidD0Ev(FileInterfaceAndroid *self)
{
    ::operator delete(FileInterfaceAndroid_completeDtor(self));
}

// ---- Write_6e4ac.cpp ----
// JNI: env holder global. r9 = *gEnvW; the jobject is *r9; its function table is *(jobject).
// Slots used: 0x2c0 NewByteArray, 0x340 SetByteArrayRegion, 0x3c ExceptionCheck,
// 0x44 ExceptionClear, 0x5c DeleteLocalRef.
extern void **gEnvW __attribute__((visibility("hidden")));
extern void *gWriteMidArg __attribute__((visibility("hidden")));

typedef void *(*fn_i)(void *env, unsigned int n);
typedef void (*fn_setregion)(void *env, void *arr, int start, unsigned int len, const void *buf);
typedef int (*fn_check)(void *env);
typedef void (*fn_void)(void *env);
typedef void (*fn_del)(void *env, void *arr);

// FileInterfaceAndroid::Write(unsigned int, void const*) — stdio fast path, else JNI byte stream.
// NOTE: near-match. The diff harness only compares up to the first `pop {..pc}` (the stdio
// fast-path return). That fast path is correct but its callee-saved register coloring (r11 vs
// r10) and result register (r0 vs r5) are forced by the whole-function allocation that also
// serves the JNI slow path; clang -Oz colors them differently from the target. Resistant.
bool FileInterfaceAndroid::Write(unsigned int n, const void *buf) {
    FileInterfaceAndroid *self = this;
    if (self->file != 0)
        return fwrite(buf, 1, n, (FILE *)self->file) == n;
    if (self->jniStream == 0)
        return true;

    void *r9 = *gEnvW;
    void *envObj = *(void **)r9;
    void *table = *(void **)envObj;
    void *arr = (*(fn_i *)((char *)table + 0x2c0))(envObj, n);
    envObj = *(void **)r9;
    table = *(void **)envObj;
    (*(fn_setregion *)((char *)table + 0x340))(envObj, arr, 0, n, buf);
    JNI_CallVoidMethod(envObj, self->jniStream, *(void **)gWriteMidArg, arr);
    table = *(void **)(*(void **)r9);
    bool ok = (*(fn_check *)((char *)table + 0x3c))(*(void **)r9) == 0;
    if (!ok)
        (*(fn_void *)((char *)(*(void **)(*(void **)r9)) + 0x44))(*(void **)r9);
    table = *(void **)(*(void **)r9);
    (*(fn_del *)((char *)table + 0x5c))(*(void **)r9, arr);
    return ok;
}

// ---- FileDelete_6e764.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) (0x6f028) and ~String (0x6ee30).

// FileInterfaceAndroid::FileDelete(AbyssEngine::String) — static. The String arrives by value
// (homed to the stack); a local copy is built and immediately destroyed. The on-stack String
// buffer trips the stack-protector canary.
// NOTE: near-match. The copy/dtor sequence and canary are correct, but the target homes the
// by-value String through the prologue push {r0-r3} and overlaps the temp at sp+0, giving a
// 16-byte frame; clang -Oz lays the temp out at a higher slot with a 32-byte frame. The
// by-value-aggregate homing/frame layout is not reachable from source form.
void FileInterfaceAndroid_FileDelete(String12 s)
{
    char tmp[12];
    ((String *)(tmp))->ctor_copy((String *)(&s), false);
    ((String *)(tmp))->dtor();
}

// ---- FileExist_6e630.cpp ----
// FileInterfaceAndroid::FileExist(AbyssEngine::String) — probes both zip directories and, failing
// that, the filesystem (fopen) under AppRootDir. Returns whether the file exists.
//
// NOTE: near-match only. The String temporaries, the canary frame, and the AbyssEngine::String
// by-value ABI homing do not reproduce the target's exact -Oz frame layout. Faithful model.

extern void **gZipMain __attribute__((visibility("hidden")));
extern void **gZipPatch __attribute__((visibility("hidden")));
extern const char *gZipPrefixA __attribute__((visibility("hidden")));
extern const char *gZipPrefixB __attribute__((visibility("hidden")));
extern const char *gModeRb __attribute__((visibility("hidden")));

bool FileInterfaceAndroid::FileExist(String12 name) {
    FileInterfaceAndroid *self = this;
    char a[12], b[12];
    String_ctor_cstr(a, gZipPrefixA, false);
    String_append(a, &name);
    String_ctor_cstr(b, gZipPrefixB, false);
    String_append(b, &name);

    void *z1 = zip_fopen(*gZipMain, ((String *)(a))->GetAEChar(), 0);
    void *z2 = zip_fopen(*gZipPatch, ((String *)(b))->GetAEChar(), 0);

    bool exists;
    if (z1 != 0) {
        zip_fclose(z1);
        exists = true;
    } else if (z2 != 0) {
        zip_fclose(z2);
        exists = true;
    } else {
        char dir[12], full[12];
        String_ctor_cstr(dir, self->appRootDir, false);
        String_concat(full, dir, &name);
        FILE *f = fopen(((String *)(full))->GetAEChar(), gModeRb);
        if (f != 0) {
            fclose(f);
            exists = true;
        } else {
            exists = false;
        }
        ((String *)(full))->dtor();
        ((String *)(dir))->dtor();
    }
    ((String *)(b))->dtor();
    ((String *)(a))->dtor();
    return exists;
}

// ---- FileInterfaceAndroid_6de68.cpp ----
// Vtable group base + an instance-count cell, both PC-relative globals.
extern void *gFIAVtable_file __attribute__((visibility("hidden")));
extern int *gFIAInstCount __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid(__sFILE*, bool) — wraps a stdio FILE*.
// NOTE: near-match. Identical instruction multiset; clang -Oz schedules the
// instance-count pointer deref and colors its temp register (r0 vs r1) differently
// than the target. Scheduling/coloring-only difference.
void FileInterfaceAndroid::ctor_file(FILE *f, bool append) {
    FileInterfaceAndroid *self = this;
    int *cnt = gFIAInstCount;
    self->file = f;
    self->zipFile = 0;
    self->jniStream = 0;
    self->field_0x0 = (char *)gFIAVtable_file + 8;
    self->modeFlag = append;
    *cnt = *cnt + 1;
}

// ---- FileInterfaceAndroid_6de9c.cpp ----
// JNI env: *env is a function table; we call slot 0x7c (GetObjectClass) and 0x84 (GetMethodID).
typedef void *(*jni_getclass)(void *env);
typedef void *(*jni_getmethod)(void *env, void *cls, const char *name, const char *sig);

extern void *gFIAVtable_obj __attribute__((visibility("hidden")));
extern int *gFIAInstCount_obj __attribute__((visibility("hidden")));
extern void **gJniEnvObj __attribute__((visibility("hidden")));

// First lazily-cached method-id cell per mode (reading vs writing) + (name,sig) literals.
extern void **gMidA_read __attribute__((visibility("hidden")));
extern const char *gNmA_read __attribute__((visibility("hidden")));
extern const char *gSgA_read __attribute__((visibility("hidden")));
extern void **gMidA_write __attribute__((visibility("hidden")));
extern const char *gNmA_write __attribute__((visibility("hidden")));
extern const char *gSgA_write __attribute__((visibility("hidden")));
// Second cell, selected by mode, plus its literals.
extern void **gMidB_read __attribute__((visibility("hidden")));
extern void **gMidB_write __attribute__((visibility("hidden")));
extern const char *gNmB __attribute__((visibility("hidden")));
extern const char *gSgB __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid(_jobject*, bool) — JNI stream; lazily resolves and
// caches the method IDs it will use. Returns `this`.
// NOTE: near-match only. The field init + instance-count bump match, but the JNI env / function-
// table indirection levels and the three lazily-cached method-id cells do not reproduce the
// target's exact global-deref chains and -Oz scheduling. Left as a faithful semantic model.
FileInterfaceAndroid * FileInterfaceAndroid::ctor_obj(jobject *stream, bool reading) {
    FileInterfaceAndroid *self = this;
    void *env = *gJniEnvObj;
    int *cnt = gFIAInstCount_obj;
    self->file = 0;
    self->zipFile = 0;
    self->jniStream = stream;
    self->modeFlag = reading;
    self->field_0x0 = (char *)gFIAVtable_obj + 8;
    *cnt = *cnt + 1;
    void *cls = (*(jni_getclass *)((char *)*(void **)env + 0x7c))(env);

    void **selB;
    if (reading) {
        if (*gMidA_read == 0)
            *gMidA_read = (*(jni_getmethod *)((char *)*(void **)env + 0x84))(env, cls, gNmA_read, gSgA_read);
        selB = gMidB_read;
    } else {
        if (*gMidA_write == 0)
            *gMidA_write = (*(jni_getmethod *)((char *)*(void **)env + 0x84))(env, cls, gNmA_write, gSgA_write);
        selB = gMidB_write;
    }
    if (*selB == 0)
        *selB = (*(jni_getmethod *)((char *)*(void **)env + 0x84))(env, cls, gNmB, gSgB);
    return self;
}

// ---- OpenRead_6dfe0.cpp ----
// FileInterfaceAndroid::OpenRead(AbyssEngine::String, int, bool, int, int, unsigned int) —
// resolves the requested file name against the two zip archives (or the filesystem under
// AppRootDir) and, when found, wraps the handle in a freshly-allocated FileInterfaceAndroid.
//
// NOTE: near-match only. The leading-'/' name normalization, the fprintf debug line, the String
// temporaries, the stack-protector canary and the placement-construction of the result object do
// not reproduce the target's exact AbyssEngine::String ABI and -Oz frame layout. Faithful model.

struct zip_file;

extern void **gZipMainR __attribute__((visibility("hidden")));
extern void **gZipPatchR __attribute__((visibility("hidden")));
extern const char *gPrefixSlash __attribute__((visibility("hidden")));
extern const char *gPrefixPlain __attribute__((visibility("hidden")));
extern const char *gOpenReadFmt __attribute__((visibility("hidden")));
extern char *gStderrBase __attribute__((visibility("hidden")));
extern const char *gModeRbR __attribute__((visibility("hidden")));
extern void *gFIAVtable_or __attribute__((visibility("hidden")));
extern int *gFIAInstCount_or __attribute__((visibility("hidden")));

FileInterfaceAndroid * FileInterfaceAndroid::OpenRead(String12 name, int p2, bool p3, int p4, int p5, unsigned int p6) {
    FileInterfaceAndroid *self = this;
    const unsigned short *w = String_GetAEWChar(&name);
    if (self->alive == 0)
        return 0;

    const unsigned short *body = (*w == '/') ? w + 1 : w;

    char a[12], wide[12], b[12], wide2[12];
    String_ctor_cstr(a, gPrefixSlash, false);
    String_ctor_wide(wide, body, false);
    String_append(a, wide);
    ((String *)(wide))->dtor();
    String_ctor_cstr(b, gPrefixPlain, false);
    String_ctor_wide(wide2, body, false);
    String_append(b, wide2);
    ((String *)(wide2))->dtor();

    fprintf((FILE *)((char *)*(void **)gStderrBase + 0xa8), gOpenReadFmt, ((String *)(b))->GetAEChar(), p3, p4, p5, p6, p2);

    zip_file *z1 = zip_fopen(*gZipMainR, ((String *)(a))->GetAEChar(), 0);
    zip_file *z2 = (*gZipPatchR != 0) ? zip_fopen(*gZipPatchR, ((String *)(b))->GetAEChar(), 0) : 0;

    FileInterfaceAndroid *result = 0;
    if (z1 != 0) {
        result = ((FileInterfaceAndroid *)((FileInterfaceAndroid *)::operator new(0x38)))->ctor_zip(z1, (bool)p4, p2, p5, p4);
    } else if (z2 != 0) {
        result = ((FileInterfaceAndroid *)((FileInterfaceAndroid *)::operator new(0x38)))->ctor_zip(z2, (bool)p4, p2, p5, p4);
    } else if (self->appRootDir != 0) {
        char dir[12], full[12];
        String_ctor_cstr(dir, self->appRootDir, false);
        String_concat(full, dir, body == w ? (void *)a : (void *)a);
        FILE *f = fopen(((String *)(full))->GetAEChar(), gModeRbR);
        if (f != 0) {
            FileInterfaceAndroid *p = (FileInterfaceAndroid *)::operator new(0x38);
            p->file = f;       // +0x8 stdio handle
            p->zipFile = 0;       // +0xc zip handle
            p->jniStream = 0;      // +0x10 jobject stream
            p->modeFlag = 0;      // +0x14 mode flag (read)
            p->field_0x0 = (char *)gFIAVtable_or + 8;
            int *cnt = gFIAInstCount_or;
            *cnt = *cnt + 1;
            result = p;
        }
        ((String *)(full))->dtor();
        ((String *)(dir))->dtor();
    }
    ((String *)(b))->dtor();
    ((String *)(a))->dtor();
    return result;
}

// ---- OpenWrite_6e208.cpp ----
// FileInterfaceAndroid::OpenWrite(AbyssEngine::String, int, bool, unsigned int) — static.
// Builds "<dir><name>" from the AppRootDir prefix and the requested file name, fopen("..","wb"),
// and on success wraps the FILE* in a freshly-allocated FileInterfaceAndroid.
//
// NOTE: near-match only. The String concatenation temporaries, the stack-protector canary frame,
// and the placement-construction of the result object do not reproduce the target's exact
// AbyssEngine::String ABI homing and -Oz frame layout. Faithful semantic model.

extern void *gFIAVtable_ow __attribute__((visibility("hidden")));
extern int *gFIAInstCount_ow __attribute__((visibility("hidden")));
extern const char *gModeWb __attribute__((visibility("hidden")));

FileInterfaceAndroid * FileInterfaceAndroid::OpenWrite(String12 name, int, bool, unsigned int) {
    FileInterfaceAndroid *self = this;
    const unsigned short *w = String_GetAEWChar(&name);
    while (*w)
        ++w;

    char dir[12], wide[12], full[12];
    String_ctor_cstr(dir, self->appRootDir, false);
    String_ctor_wide(wide, String_GetAEWChar(&name), false);
    String_concat(full, dir, wide);
    ((String *)(wide))->dtor();

    FILE *f = fopen(((String *)(full))->GetAEChar(), gModeWb);
    FileInterfaceAndroid *result = 0;
    if (f != 0) {
        FileInterfaceAndroid *p = (FileInterfaceAndroid *)::operator new(0x38);
        p->file = f;       // +0x8 stdio handle
        p->zipFile = 0;       // +0xc zip handle
        p->jniStream = 0;      // +0x10 jobject stream
        p->modeFlag = 1;      // +0x14 mode flag (write)
        p->field_0x0 = (char *)gFIAVtable_ow + 8;
        int *cnt = gFIAInstCount_ow;
        *cnt = *cnt + 1;
        result = p;
    }
    ((String *)(full))->dtor();
    ((String *)(dir))->dtor();
    return result;
}

// ---- FileInterfaceAndroid_6ddf4.cpp ----
// Vtable group base (PC-relative global); the installed vptr is *gVtable + 8.
extern void *gFIAVtable __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid() — default ctor.
// NOTE: near-match. Same instruction multiset; clang -Oz finalizes the vptr value
// (deref + `adds #8`) in a slightly different slot relative to the zero-stores than
// the target's interleaving. Scheduling-only difference.
void FileInterfaceAndroid::ctor_default() {
    FileInterfaceAndroid *self = this;
    self->alive = 1;
    char *vt = (char *)gFIAVtable + 8;
    self->appRootDir = 0;
    self->zipDirectory = 0;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->field_0x0 = vt;
}

// ---- Close_6e7a0.cpp ----
// _JNIEnv::CallVoidMethod(jobject env, jmethodID m, jobject arg) — variadic JNI stub.

// JNI env slot + the two cached jstring constants (append vs write mode), all PC-relative.
extern void *gJniEnv __attribute__((visibility("hidden")));
extern void *gModeWrite __attribute__((visibility("hidden")));
extern void *gModeAppend __attribute__((visibility("hidden")));

// FileInterfaceAndroid::Close() — close whichever handle is open; for a JNI stream, notify Java.
// NOTE: near-match. The fclose/zip_fclose/handle-clear logic is byte-identical; the only
// divergence is the mode-string select: the target branches to load one of two PC-relative
// jstring globals, while clang -Oz pre-loads both and uses a conditional move (it ne; movne),
// which also shifts a callee-saved register (r5 vs r6). A -Oz branch-vs-cmov choice.
void FileInterfaceAndroid::Close() {
    FileInterfaceAndroid *self = this;
    if (self->file != 0) {
        fclose((FILE *)self->file);
        self->file = 0;
    }
    if (self->zipFile != 0) {
        zip_fclose(self->zipFile);
        self->zipFile = 0;
    }
    void *m = self->jniStream;
    if (m != 0) {
        void *env = *(void **)gJniEnv;
        void *modePtr;
        if (self->modeFlag == 0)
            modePtr = gModeWrite;
        else
            modePtr = gModeAppend;
        JNI_CallVoidMethod(env, m, *(void **)modePtr);
        self->jniStream = 0;
    }
}
