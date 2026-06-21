#include "engine/file/FileInterfaceAndroid.h"
#include "game/core/String.h"
#include "platform/libc.h"
#include <cstdio>
#include <new>

struct zip_file;
using AbyssEngine::String;

extern "C" unsigned int zip_fread(void *zf, void *buf, unsigned int n);
extern "C" unsigned int JNI_CallIntMethod(void *env, void *m, void *arg0, void *arg1);
extern "C" void JNI_CallVoidMethod(void *env, void *m, void *arg, ...);
extern "C" zip_file *zip_fopen(void *za, const char *name, int flags);
extern "C" int zip_fclose(void *zf);

// The UTF-16 backing buffer of an AbyssEngine::String.
static inline const unsigned short *GetAEWChar(const String &s)
{
    return reinterpret_cast<const unsigned short *>(s.text());
}

// Live-instance count cell, maintained on every constructed instance. (The vptr is now managed
// by the compiler; the former gFIAVtable plants are gone.)
extern int *gFIAInstCount __attribute__((visibility("hidden")));

// GetDirPreFix() returns a String built from a fixed directory prefix.
extern const char kDirPreFix[] __attribute__((visibility("hidden")));

String FileInterfaceAndroid::GetDirPreFix()
{
    return String(kDirPreFix);
}

FileInterfaceAndroid::FileInterfaceAndroid()
{
    this->enabled = 1;
    this->appRootDir = 0;
    this->zipDirectory = 0;
    this->zipAppend = 0;
    this->zipReadLen = 0;
}

FileInterfaceAndroid::FileInterfaceAndroid(FILE *f, bool append)
{
    this->file = f;
    this->zipFile = 0;
    this->jniStream = 0;
    this->modeFlag = append;
    ++*gFIAInstCount;
}

// JNI env: *env is a function table; we call slot 0x7c (GetObjectClass) and 0x84 (GetMethodID).
typedef void *(*jni_getclass)(void *env);
typedef void *(*jni_getmethod)(void *env, void *cls, const char *name, const char *sig);

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

// Wraps a JNI stream; lazily resolves and caches the JNI method IDs it will use.
FileInterfaceAndroid::FileInterfaceAndroid(jobject stream, bool reading)
{
    void *env = *gJniEnvObj;
    this->file = 0;
    this->zipFile = 0;
    this->jniStream = stream;
    this->modeFlag = reading;
    ++*gFIAInstCount;

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
}

// Wraps a libzip handle, then seeks to the start offset.
FileInterfaceAndroid::FileInterfaceAndroid(zip_file *zf, bool append, int start, int p4, int p5)
{
    this->file = 0;
    this->zipFile = zf;
    this->jniStream = 0;
    this->modeFlag = 0;
    ++*gFIAInstCount;
    this->zipReadPos = 0;
    this->zipReadLen = 0;
    this->Seek(start);
    this->zipAppend = append;
}

// Re-establishes the vptr, closes any open handle, then either decrements the live-instance
// count or, when it is already zero, clears the alive flag.
FileInterfaceAndroid::~FileInterfaceAndroid()
{
    this->Close();
    if (*gFIAInstCount != 0)
        --*gFIAInstCount;
    else
        this->enabled = 0;
}

// Close whichever handle is open; for a JNI stream, notify Java of the close.
extern void *gJniEnv __attribute__((visibility("hidden")));
extern void *gModeWrite __attribute__((visibility("hidden")));
extern void *gModeAppend __attribute__((visibility("hidden")));

void FileInterfaceAndroid::Close()
{
    if (this->file != 0) {
        fclose((FILE *)this->file);
        this->file = 0;
    }
    if (this->zipFile != 0) {
        zip_fclose(this->zipFile);
        this->zipFile = 0;
    }
    void *m = this->jniStream;
    if (m != 0) {
        void *env = *(void **)gJniEnv;
        void *modePtr = (this->modeFlag == 0) ? gModeWrite : gModeAppend;
        JNI_CallVoidMethod(env, m, *(void **)modePtr);
        this->jniStream = 0;
    }
}

uint32_t FileInterfaceAndroid::GetFileSize()
{
    fseek((FILE *)this->file, 0, SEEK_END);
    int size = ftell((FILE *)this->file);
    fseek((FILE *)this->file, 0, SEEK_SET);
    return (uint32_t)size;
}

const char *FileInterfaceAndroid::GetAppRootDir()
{
    // appRootDir now holds a void* (set by SetAppRootDir(void*)); GetAppRootDir must hand back
    // a const char*. This single internal cast mirrors the binary's own behavior.
    return (const char *)this->appRootDir;
}

uint32_t FileInterfaceAndroid::GetDeviceFreeSpace()
{
    return 0;
}

void FileInterfaceAndroid::SetZipDirectory(void *p)
{
    if (p != 0)
        this->zipDirectory = p;
}

void FileInterfaceAndroid::SetAppRootDir(void *p)
{
    if (p != 0)
        this->appRootDir = p;
}

// JNIEnv function-table slot indices used by the byte-stream Read/Write paths.
typedef void *(*fn_i)(void *env, unsigned int n);
typedef void (*fn_getregion)(void *env, void *arr, int start, unsigned int len, void *buf);
typedef void (*fn_setregion)(void *env, void *arr, int start, unsigned int len, const void *buf);
typedef int (*fn_check)(void *env);
typedef void (*fn_void)(void *env);
typedef void (*fn_del)(void *env, void *arr);

extern void **gEnvR __attribute__((visibility("hidden")));
extern void *gReadMidArg __attribute__((visibility("hidden")));

// Reads from the zip handle, the stdio handle, or the JNI input stream (in which case the
// bytes are copied out of a Java byte[] region).
uint32_t FileInterfaceAndroid::Read(uint32_t n, void *buf)
{
    if (this->zipFile != 0)
        return zip_fread(this->zipFile, buf, n) == n;
    if (this->file != 0)
        return fread(buf, 1, n, (FILE *)this->file) == n;
    if (this->jniStream == 0)
        return false;

    void *r9 = *gEnvR;
    void *env = *(void **)r9;
    void *table = *(void **)env;
    void *arr = (*(fn_i *)((char *)table + 0x2c0))(env, n);              // NewByteArray
    unsigned int got = JNI_CallIntMethod(*(void **)r9, this->jniStream, *(void **)gReadMidArg, arr);

    bool ok;
    table = *(void **)(*(void **)r9);
    if ((*(fn_check *)((char *)table + 0x3c))(*(void **)r9) == 0 && got == n) {   // ExceptionCheck
        env = *(void **)r9;
        table = *(void **)env;
        (*(fn_getregion *)((char *)table + 0x320))(env, arr, 0, n, buf);  // GetByteArrayRegion
        ok = true;
    } else {
        table = *(void **)(*(void **)r9);
        (*(fn_void *)((char *)table + 0x40))(*(void **)r9);               // ExceptionDescribe
        (*(fn_void *)((char *)(*(void **)(*(void **)r9)) + 0x44))(*(void **)r9); // ExceptionClear
        ok = false;
    }
    table = *(void **)(*(void **)r9);
    (*(fn_del *)((char *)table + 0x5c))(*(void **)r9, arr);               // DeleteLocalRef
    return ok;
}

// Skip `n` bytes forward. For a zip handle this reads and discards into a scratch buffer;
// for a stdio handle it is a relative fseek.
uint32_t FileInterfaceAndroid::Seek(uint32_t n)
{
    if (n == 0)
        return true;
    void *zf = this->zipFile;
    int delta;
    if (zf != 0) {
        void *tmp = malloc(n);
        if (tmp == 0)
            return false;
        unsigned int got = zip_fread(zf, tmp, n);
        free(tmp);
        delta = got - n;
    } else {
        void *f = this->file;
        if (f == 0)
            return false;
        delta = fseek((FILE *)f, n, SEEK_CUR);
    }
    return delta == 0;
}

extern void **gEnvW __attribute__((visibility("hidden")));
extern void *gWriteMidArg __attribute__((visibility("hidden")));

// stdio fast path, else JNI byte stream.
uint32_t FileInterfaceAndroid::Write(uint32_t n, const void *buf)
{
    if (this->file != 0)
        return fwrite(buf, 1, n, (FILE *)this->file) == n;
    if (this->jniStream == 0)
        return true;

    void *r9 = *gEnvW;
    void *envObj = *(void **)r9;
    void *table = *(void **)envObj;
    void *arr = (*(fn_i *)((char *)table + 0x2c0))(envObj, n);            // NewByteArray
    envObj = *(void **)r9;
    table = *(void **)envObj;
    (*(fn_setregion *)((char *)table + 0x340))(envObj, arr, 0, n, buf);   // SetByteArrayRegion
    JNI_CallVoidMethod(envObj, this->jniStream, *(void **)gWriteMidArg, arr);
    table = *(void **)(*(void **)r9);
    bool ok = (*(fn_check *)((char *)table + 0x3c))(*(void **)r9) == 0;   // ExceptionCheck
    if (!ok)
        (*(fn_void *)((char *)(*(void **)(*(void **)r9)) + 0x44))(*(void **)r9); // ExceptionClear
    table = *(void **)(*(void **)r9);
    (*(fn_del *)((char *)table + 0x5c))(*(void **)r9, arr);               // DeleteLocalRef
    return ok;
}

// The Android backend never implemented these slots; the original emitted them as standalone
// stubs that return a constant (and otherwise ignore their arguments).
void *FileInterfaceAndroid::OpenAppend(String, int, bool, unsigned int)
{
    return 0;
}

char *FileInterfaceAndroid::Output(char *line)
{
    return line;
}

// Copy-constructs a throwaway String from the by-value argument's second backing word
// reinterpreted as a String pointer (the original's degenerate FileDelete body); the copy is
// discarded immediately.
void FileInterfaceAndroid::FileDelete(String name)
{
    String *src = *reinterpret_cast<String **>(reinterpret_cast<char *>(&name) + 4);
    String discard(*src, false);
}

uint32_t FileInterfaceAndroid::FileEnumInit(char *, bool)
{
    return 0;
}

uint32_t FileInterfaceAndroid::FileGetNextEnum(String &)
{
    return 0;
}

void FileInterfaceAndroid::SetSaveDirectory(String)
{
}

void FileInterfaceAndroid::ResetSaveDirectory()
{
}

// Probes both zip directories and, failing that, the filesystem (fopen) under AppRootDir.
extern void **gZipMain __attribute__((visibility("hidden")));
extern void **gZipPatch __attribute__((visibility("hidden")));
extern const char *gZipPrefixA __attribute__((visibility("hidden")));
extern const char *gZipPrefixB __attribute__((visibility("hidden")));
extern const char *gModeRb __attribute__((visibility("hidden")));

uint32_t FileInterfaceAndroid::FileExist(String name)
{
    String a(gZipPrefixA);
    a.addAssign_str(&name);
    String b(gZipPrefixB);
    b.addAssign_str(&name);

    void *z1 = zip_fopen(*gZipMain, a.GetAEChar(), 0);
    void *z2 = zip_fopen(*gZipPatch, b.GetAEChar(), 0);

    bool exists;
    if (z1 != 0) {
        zip_fclose(z1);
        exists = true;
    } else if (z2 != 0) {
        zip_fclose(z2);
        exists = true;
    } else {
        String dir((const char *)this->appRootDir);
        String full = dir + name;
        FILE *f = fopen(full.GetAEChar(), gModeRb);
        if (f != 0) {
            fclose(f);
            exists = true;
        } else {
            exists = false;
        }
    }
    return exists;
}

// Resolves the requested file name against the two zip archives (or the filesystem under
// AppRootDir) and, when found, wraps the handle in a freshly-allocated FileInterfaceAndroid.
extern void **gZipMainR __attribute__((visibility("hidden")));
extern void **gZipPatchR __attribute__((visibility("hidden")));
extern const char *gPrefixSlash __attribute__((visibility("hidden")));
extern const char *gPrefixPlain __attribute__((visibility("hidden")));
extern const char *gOpenReadFmt __attribute__((visibility("hidden")));
extern char *gStderrBase __attribute__((visibility("hidden")));
extern const char *gModeRbR __attribute__((visibility("hidden")));

void *FileInterfaceAndroid::OpenRead(String name, int p2, bool p3, int p4, int p5, unsigned int p6)
{
    const unsigned short *w = GetAEWChar(name);
    if (this->enabled == 0)
        return 0;

    const unsigned short *body = (*w == '/') ? w + 1 : w;

    String a(gPrefixSlash);
    String wide;
    wide.ctor_wchar(body, false);
    a.addAssign_str(&wide);
    String b(gPrefixPlain);
    String wide2;
    wide2.ctor_wchar(body, false);
    b.addAssign_str(&wide2);

    fprintf((FILE *)((char *)*(void **)gStderrBase + 0xa8), gOpenReadFmt, b.GetAEChar(), p3, p4, p5, p6, p2);

    zip_file *z1 = zip_fopen(*gZipMainR, a.GetAEChar(), 0);
    zip_file *z2 = (*gZipPatchR != 0) ? zip_fopen(*gZipPatchR, b.GetAEChar(), 0) : 0;

    FileInterfaceAndroid *result = 0;
    if (z1 != 0) {
        result = new FileInterfaceAndroid(z1, (bool)p4, p2, p5, p4);
    } else if (z2 != 0) {
        result = new FileInterfaceAndroid(z2, (bool)p4, p2, p5, p4);
    } else if (this->appRootDir != 0) {
        String dir((const char *)this->appRootDir);
        String full = dir + a;
        FILE *f = fopen(full.GetAEChar(), gModeRbR);
        if (f != 0)
            result = new FileInterfaceAndroid(f, false);
    }
    return result;
}

// Builds "<dir><name>" from the AppRootDir prefix and the requested file name, fopen("..","wb"),
// and on success wraps the FILE* in a freshly-allocated FileInterfaceAndroid.
extern const char *gModeWb __attribute__((visibility("hidden")));

void *FileInterfaceAndroid::OpenWrite(const String &name, uint32_t, uint32_t)
{
    const unsigned short *w = GetAEWChar(name);
    while (*w)
        ++w;

    String dir((const char *)this->appRootDir);
    String wide;
    wide.ctor_wchar(GetAEWChar(name), false);
    String full = dir + wide;

    FILE *f = fopen(full.GetAEChar(), gModeWb);
    if (f == 0)
        return 0;
    return new FileInterfaceAndroid(f, true);
}

// Arity variant of OpenWrite taking the name by value (plus mode/flag args that do not affect
// the resolved path): builds "<dir><name>" from AppRootDir, fopen("..","wb"), and on success
// wraps the FILE* in a freshly-allocated FileInterfaceAndroid.
void *FileInterfaceAndroid::OpenWrite(String name, int, bool, unsigned int)
{
    const unsigned short *w = GetAEWChar(name);
    while (*w)
        ++w;

    String dir((const char *)this->appRootDir);
    String wide;
    wide.ctor_wchar(GetAEWChar(name), false);
    String full = dir + wide;

    FILE *f = fopen(full.GetAEChar(), gModeWb);
    if (f == 0)
        return 0;
    return new FileInterfaceAndroid(f, true);
}

// C-ABI default constructor (C1): construct in place.
extern "C" void FileInterfaceAndroid_ctor(void *self)
{
    new (self) FileInterfaceAndroid();
}

// C-ABI complete-object destructor (D1): run the destructor and hand the storage back.
extern "C" void *FileInterfaceAndroid_completeDtor(FileInterfaceAndroid *self)
{
    if (self)
        self->~FileInterfaceAndroid();
    return self;
}

// Deleting destructor (D0): destroy and release the storage.
extern "C" void FileInterfaceAndroid_deletingDtor(FileInterfaceAndroid *self)
{
    delete self;
}
