#include "engine/file/FileInterfaceAndroid.h"
#include "game/core/String.h"

#include <jni.h>

#include <cstdio>
#include <new>

unsigned int JNI_CallIntMethod(void *env, void *m, void *arg0, void *arg1); // lint: void_ptr imported Pv-mangled JNI shim

// lint: void_ptr imported Pv-mangled JNI shim

void JNI_CallVoidMethod(void *env, void *m, void *arg, ...); // lint: void_ptr imported Pv-mangled JNI shim

static inline const JNINativeInterface *JniTable(JNIEnv *env) {
    return *reinterpret_cast<const JNINativeInterface *const *>(env);
}

#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(JNINativeInterface, GetObjectClass) == 0x7c, "JNI layout");
static_assert(offsetof(JNINativeInterface, GetMethodID) == 0x84, "JNI layout");
static_assert(offsetof(JNINativeInterface, NewByteArray) == 0x2c0, "JNI layout");
static_assert(offsetof(JNINativeInterface, GetByteArrayRegion) == 0x320, "JNI layout");
static_assert(offsetof(JNINativeInterface, SetByteArrayRegion) == 0x340, "JNI layout");
static_assert(offsetof(JNINativeInterface, ExceptionOccurred) == 0x3c, "JNI layout");
static_assert(offsetof(JNINativeInterface, ExceptionDescribe) == 0x40, "JNI layout");
static_assert(offsetof(JNINativeInterface, ExceptionClear) == 0x44, "JNI layout");
static_assert(offsetof(JNINativeInterface, DeleteLocalRef) == 0x5c, "JNI layout");
#endif

static inline const unsigned short *GetAEWChar(const String &s) {
    return reinterpret_cast<const unsigned short *>(s.text());
}


static const char kDirPreFix[] = "";

String FileInterfaceAndroid::GetDirPreFix() {
    return String(kDirPreFix);
}

FileInterfaceAndroid::FileInterfaceAndroid() {
    this->enabled = 1;
    this->appRootDir = 0;
    this->zipDirectory = 0;
    this->zipAppend = 0;
    this->zipReadLen = 0;
}

FileInterfaceAndroid::FileInterfaceAndroid(FILE *f, bool append) {
    this->file = f;
    this->zipFile = 0;
    this->jniStream = 0;
    this->modeFlag = append;
    ++*&FileInterfaceAndroid::fileCounter;
}




FileInterfaceAndroid::FileInterfaceAndroid(jobject stream, bool reading) {
    JNIEnv *env = *&FileInterfaceAndroid::env;
    this->file = 0;
    this->zipFile = 0;
    this->jniStream = stream;
    this->modeFlag = reading;
    ++*&FileInterfaceAndroid::fileCounter;

    JNIEnv *jenv = reinterpret_cast<JNIEnv *>(env);
    const JNINativeInterface *jni = JniTable(env);
    jobject cls = jni->GetObjectClass(jenv, stream);

    jmethodID *selB;
    const char *nmB;
    const char *sgB;
    if (reading) {
        if (*&FileInterfaceAndroid::methodCloseRead == 0)
            *&FileInterfaceAndroid::methodCloseRead = jni->GetMethodID(jenv, reinterpret_cast<jclass>(cls), "close", "()V");
        selB = &FileInterfaceAndroid::methodRead;
        nmB = "read";
        sgB = "([B)I";
    } else {
        if (*&FileInterfaceAndroid::methodCloseWrite == 0)
            *&FileInterfaceAndroid::methodCloseWrite = jni->GetMethodID(jenv, reinterpret_cast<jclass>(cls), "close", "()V");
        selB = &FileInterfaceAndroid::methodWrite;
        nmB = "write";
        sgB = "([B)V";
    }
    if (*selB == 0)
        *selB = jni->GetMethodID(jenv, reinterpret_cast<jclass>(cls), nmB, sgB);
}

FileInterfaceAndroid::FileInterfaceAndroid(zip_file *zf, bool append, int start, int p4, int p5) {
    this->file = 0;
    this->zipFile = zf;
    this->jniStream = 0;
    this->modeFlag = 0;
    ++*&FileInterfaceAndroid::fileCounter;
    this->zipReadPos = 0;
    this->zipReadLen = 0;
    this->Seek(start);
    this->zipAppend = append;
}

FileInterfaceAndroid::~FileInterfaceAndroid() {
    this->Close();
    if (*&FileInterfaceAndroid::fileCounter != 0)
        --*&FileInterfaceAndroid::fileCounter;
    else
        this->enabled = 0;
}

static jobject *gModeWrite = nullptr;
static jobject *gModeAppend = nullptr;

void FileInterfaceAndroid::Close() {
    if (this->file != 0) {
        fclose(this->file);
        this->file = 0;
    }
    if (this->zipFile != 0) {
        zip_fclose(this->zipFile);
        this->zipFile = 0;
    }
    jobject m = this->jniStream;
    if (m != 0) {
        JNIEnv *env = *&FileInterfaceAndroid::env;
        jobject *modePtr = (this->modeFlag == 0) ? gModeWrite : gModeAppend;
        JNI_CallVoidMethod(env, m, *modePtr);
        this->jniStream = 0;
    }
}

uint32_t FileInterfaceAndroid::GetFileSize() {
    fseek(this->file, 0, SEEK_END);
    int size = ftell(this->file);
    fseek(this->file, 0, SEEK_SET);
    return (uint32_t) size;
}

const char *FileInterfaceAndroid::GetAppRootDir() {
    return this->appRootDir;
}

uint32_t FileInterfaceAndroid::GetDeviceFreeSpace() {
    return 0;
}

void FileInterfaceAndroid::SetZipDirectory(void *p) { // lint: void_ptr virtual override param (FileInterface)
    // lint: void_ptr virtual override param (FileInterface)
    if (p != 0)
        this->zipDirectory = (const char *) p;
}

void FileInterfaceAndroid::SetAppRootDir(void *p) { // lint: void_ptr virtual override param (FileInterface)
    // lint: void_ptr virtual override param (FileInterface)
    if (p != 0)
        this->appRootDir = (const char *) p;
}


uint32_t FileInterfaceAndroid::Read(uint32_t n, void *buf) { // lint: void_ptr virtual override param (FileInterface byte buffer)
    // lint: void_ptr virtual override param (FileInterface byte buffer)
    if (this->zipFile != 0)
        return zip_fread(this->zipFile, buf, n) == n;
    if (this->file != 0)
        return fread(buf, 1, n, this->file) == n;
    if (this->jniStream == 0)
        return false;

    JNIEnv **r9 = &FileInterfaceAndroid::env;
    JNIEnv *env = *r9;
    JNIEnv *jenv = env;
    jbyteArray arr = JniTable(env)->NewByteArray(jenv, (jsize) n);
    unsigned int got = JNI_CallIntMethod(env, this->jniStream, FileInterfaceAndroid::methodRead, arr);

    bool ok;
    if (JniTable(env)->ExceptionOccurred(jenv) == 0 && got == n) {
        JniTable(env)->GetByteArrayRegion(jenv, arr, 0, (jsize) n, (jbyte *) buf);
        ok = true;
    } else {
        JniTable(env)->ExceptionDescribe(jenv);
        JniTable(env)->ExceptionClear(jenv);
        ok = false;
    }
    JniTable(env)->DeleteLocalRef(jenv, arr);
    return ok;
}

uint32_t FileInterfaceAndroid::Seek(uint32_t n) {
    if (n == 0)
        return true;
    zip_file *zf = this->zipFile;
    int delta;
    if (zf != 0) {
        unsigned char *tmp = (unsigned char *) malloc(n);
        if (tmp == 0)
            return false;
        unsigned int got = zip_fread(zf, tmp, n);
        free(tmp);
        delta = got - n;
    } else {
        FILE *f = this->file;
        if (f == 0)
            return false;
        delta = fseek(f, n, SEEK_CUR);
    }
    return delta == 0;
}


uint32_t FileInterfaceAndroid::Write(uint32_t n, const void *buf) { // lint: void_ptr virtual override param (FileInterface byte buffer)
    // lint: void_ptr virtual override param (FileInterface byte buffer)
    if (this->file != 0)
        return fwrite(buf, 1, n, this->file) == n;
    if (this->jniStream == 0)
        return true;

    JNIEnv **r9 = &FileInterfaceAndroid::env;
    JNIEnv *envObj = *r9;
    JNIEnv *jenv = envObj;
    jbyteArray arr = JniTable(envObj)->NewByteArray(jenv, (jsize) n);
    JniTable(envObj)->SetByteArrayRegion(jenv, arr, 0, (jsize) n, (const jbyte *) buf);
    JNI_CallVoidMethod(envObj, this->jniStream, FileInterfaceAndroid::methodWrite, arr);
    bool ok = JniTable(envObj)->ExceptionOccurred(jenv) == 0;
    if (!ok)
        JniTable(envObj)->ExceptionClear(jenv);
    JniTable(envObj)->DeleteLocalRef(jenv, arr);
    return ok;
}

void *FileInterfaceAndroid::OpenAppend(String, int, bool, unsigned int) { // lint: void_ptr virtual override return type baked into vtable/symbol
    // lint: void_ptr virtual override return type baked into vtable/symbol
    return 0;
}

char *FileInterfaceAndroid::Output(char *) {
    return reinterpret_cast<char *>(this);
}

uint32_t FileInterfaceAndroid::FileDelete(String name) {
    String discard(name, false);
    return 0;
}

uint32_t FileInterfaceAndroid::FileEnumInit(char *, bool) {
    return 0;
}

uint32_t FileInterfaceAndroid::FileGetNextEnum(String &) {
    return 0;
}

void FileInterfaceAndroid::SetSaveDirectory(String) {
}

void FileInterfaceAndroid::ResetSaveDirectory() {
}

struct zip *APKArchive = nullptr;
struct zip *ZIPArchive = nullptr;
static const char *gZipPrefixA = nullptr;
static const char *gZipPrefixB = nullptr;

uint32_t FileInterfaceAndroid::FileExist(String name) {
    String a(gZipPrefixA);
    a += name;
    String b(gZipPrefixB);
    b += name;

    zip_file *z1 = zip_fopen((struct zip *) APKArchive, a.GetAEChar(), 0);
    zip_file *z2 = zip_fopen((struct zip *) ZIPArchive, b.GetAEChar(), 0);

    bool exists;
    if (z1 != 0) {
        zip_fclose(z1);
        exists = true;
    } else if (z2 != 0) {
        zip_fclose(z2);
        exists = true;
    } else {
        String dir(this->appRootDir);
        String full = dir + name;
        FILE *f = fopen(full.GetAEChar(), "rb");
        if (f != 0) {
            fclose(f);
            exists = true;
        } else {
            exists = false;
        }
    }
    return exists;
}

static const char *gPrefixSlash = nullptr;
static const char *gPrefixPlain = nullptr;
static const char *gOpenReadFmt = nullptr;
static char *gStderrBase = nullptr;

struct AndroidIoState {
    char pad_00[0xa8];
    FILE *stderrFile;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(AndroidIoState, stderrFile) == 0xa8, "AndroidIoState layout");
#endif

void *FileInterfaceAndroid::OpenRead(String name, int p2, bool p3, int p4, int p5, unsigned int p6) { // lint: void_ptr virtual override return type baked into vtable/symbol
    // lint: void_ptr virtual override return type baked into vtable/symbol
    const unsigned short *w = GetAEWChar(name);
    if (this->enabled == 0)
        return 0;

    const unsigned short *body = (*w == '/') ? w + 1 : w;

    String a(gPrefixSlash);
    String wide;
    wide.Set((const unsigned short *) (body));
    a += wide;
    String b(gPrefixPlain);
    String wide2;
    wide2.Set((const unsigned short *) (body));
    b += wide2;

    AndroidIoState *ioState = *reinterpret_cast<AndroidIoState **>(gStderrBase);
    fprintf(ioState->stderrFile, gOpenReadFmt, b.GetAEChar(), p3, p4, p5, p6, p2);

    zip_file *z1 = zip_fopen((struct zip *) APKArchive, a.GetAEChar(), 0);
    zip_file *z2 = (ZIPArchive != 0) ? zip_fopen((struct zip *) ZIPArchive, b.GetAEChar(), 0) : 0;

    FileInterfaceAndroid * result = 0;
    if (z1 != 0) {
        result = new FileInterfaceAndroid(z1, (bool) p4, p2, p5, p4);
    } else if (z2 != 0) {
        result = new FileInterfaceAndroid(z2, (bool) p4, p2, p5, p4);
    } else if (this->appRootDir != 0) {
        String dir(this->appRootDir);
        String full = dir + a;
        FILE *f = fopen(full.GetAEChar(), "rb");
        if (f != 0)
            result = new FileInterfaceAndroid(f, false);
    }
    return result;
}


void *FileInterfaceAndroid::OpenWrite(String name, int, bool, unsigned int) { // lint: void_ptr virtual override return type baked into vtable/symbol
    // lint: void_ptr virtual override return type baked into vtable/symbol
    const unsigned short *w = GetAEWChar(name);
    while (*w)
        ++w;

    String dir(this->appRootDir);
    String wide;
    wide.Set((const unsigned short *) (GetAEWChar(name)));
    String full = dir + wide;

    FILE *f = fopen(full.GetAEChar(), "wb");
    if (f == 0)
        return 0;
    return new FileInterfaceAndroid(f, true);
}

char *logi(char *message) {
    return message;
}

char *loge(char *message) {
    return message;
}

jmethodID FileInterfaceAndroid::methodRead;
int FileInterfaceAndroid::fileCounter;
jmethodID FileInterfaceAndroid::methodWrite;
jmethodID FileInterfaceAndroid::methodCloseRead;
jmethodID FileInterfaceAndroid::methodFileExist;
jmethodID FileInterfaceAndroid::methodCloseWrite;
JNIEnv *FileInterfaceAndroid::env;
jclass FileInterfaceAndroid::clazz;
jobject FileInterfaceAndroid::context;
