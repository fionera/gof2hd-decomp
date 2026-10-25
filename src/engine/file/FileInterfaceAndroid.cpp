#include "engine/file/FileInterfaceAndroid.h"
#include "game/core/String.h"

#include <jni.h>

#include <cstdio>
#include <new>

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



char *logi(char *message) {
    return message;
}

char *loge(char *message) {
    return message;
}

FileInterfaceAndroid::FileInterfaceAndroid() {
    this->enabled = 1;
    this->appRootDir = 0;
    this->zipDirectory = 0;
    this->zipAppend = 0;
    this->zipReadLen = 0;
}




void *FileInterfaceAndroid::OpenAppend(String, int, bool, unsigned int) { // lint: void_ptr virtual override return type baked into vtable/symbol
    // lint: void_ptr virtual override return type baked into vtable/symbol
    return 0;
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

FileInterfaceAndroid::FileInterfaceAndroid(FILE *f, bool append) {
    this->file = f;
    this->zipFile = 0;
    this->jniStream = 0;
    this->modeFlag = append;
    ++*&FileInterfaceAndroid::fileCounter;
}

FileInterfaceAndroid::FileInterfaceAndroid(jobject stream, bool writing) {
    this->file = 0;
    this->zipFile = 0;
    this->jniStream = stream;
    this->modeFlag = writing;
    ++fileCounter;

    jclass cls = env->GetObjectClass(stream);
    if (writing) {
        if (methodWrite == 0)
            methodWrite = env->GetMethodID(cls, "write", "([B)V");
        if (methodCloseWrite == 0)
            methodCloseWrite = env->GetMethodID(cls, "close", "()V");
    } else {
        if (methodRead == 0)
            methodRead = env->GetMethodID(cls, "read", "([B)I");
        if (methodCloseRead == 0)
            methodCloseRead = env->GetMethodID(cls, "close", "()V");
    }
}

FileInterfaceAndroid::~FileInterfaceAndroid() {
    this->Close();
    if (*&FileInterfaceAndroid::fileCounter != 0)
        --*&FileInterfaceAndroid::fileCounter;
    else
        this->enabled = 0;
}

void *FileInterfaceAndroid::OpenRead(String name, int p2, bool p3, int p4, int p5, unsigned int p6) { // lint: void_ptr virtual override return type baked into vtable/symbol
    const unsigned short *wname = name.GetAEWChar();
    if (this->enabled == 0)
        return 0;

    const unsigned short *relative = (*wname == '/') ? wname + 1 : wname;
    String apkPath("assets/");
    apkPath += String(relative, false);
    String zipPath("47947_GOF2CONTENT_ETC/assets/");
    zipPath += String(relative, false);

    fprintf(stderr, "OpenRead('%s', %d, %d, %d, %d, %d)\n", apkPath.GetAEChar(), p2, p3, p4, p5, p6);

    zip_file *apkFile = zip_fopen(APKArchive, apkPath.GetAEChar(), 0);
    zip_file *zipFile = 0;
    if (ZIPArchive != 0)
        zipFile = zip_fopen(ZIPArchive, zipPath.GetAEChar(), 0);

    FileInterfaceAndroid *opened = 0;
    if (apkFile != 0) {
        FileInterfaceAndroid *p = new FileInterfaceAndroid(apkFile, p3, p6, p4, p5);
        opened = p;
    } else if (zipFile != 0) {
        FileInterfaceAndroid *q = new FileInterfaceAndroid(zipFile, p3, p6, p4, p5);
        opened = q;
    } else if (this->appRootDir != 0) {
        // Not packaged: fall back to the unpacked data directory.
        String dir(this->appRootDir);
        String full = dir + String(relative, false);
        FILE *f = fopen(full.GetAEChar(), "rb");
        if (f != 0) {
            FileInterfaceAndroid *p = new FileInterfaceAndroid(f, false);
            opened = p;
        }
    }
    return opened;
}

void *FileInterfaceAndroid::OpenWrite(String name, int, bool, unsigned int) { // lint: void_ptr virtual override return type baked into vtable/symbol
    const unsigned short *wname = name.GetAEWChar();
    const unsigned short *end = wname;
    while (*end++ != 0) {
    }

    String dir(this->appRootDir);
    String full = dir + String(wname, false);
    FILE *f = fopen(full.GetAEChar(), "wb");
    if (f == 0)
        return 0;
    return new FileInterfaceAndroid(f, true);
}

bool FileInterfaceAndroid::Read(uint32_t n, void *buf) { // lint: void_ptr virtual override param (FileInterface byte buffer)
    if (this->zipFile != 0)
        return zip_fread(this->zipFile, buf, n) == n;

    uint32_t ok = 0;
    if (this->file != 0) {
        ok = fread(buf, 1, n, this->file) == n;
    } else if (this->jniStream != 0) {
        jbyteArray array = env->NewByteArray(n);
        unsigned int got = env->CallIntMethod(this->jniStream, methodRead, array);
        if (env->ExceptionOccurred() != 0) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        } else if (got == n) {
            env->GetByteArrayRegion(array, 0, n, static_cast<jbyte *>(buf));
            // Leftover hex-dump builder from a debug log; the text is never used.
            String dump;
            for (uint32_t i = 0; i < n; ++i) {
                dump += String(" ");
                dump += String(static_cast<char>(static_cast<unsigned char *>(buf)[i]));
            }
            ok = 1;
        }
        env->DeleteLocalRef(array);
    }
    if (ok == 1)
        return true;
    return false;
}

uint32_t FileInterfaceAndroid::Write(uint32_t n, const void *buf) { // lint: void_ptr virtual override param (FileInterface byte buffer)
    if (this->file != 0)
        return fwrite(buf, 1, n, this->file) == n;
    if (this->jniStream == 0)
        return true;

    jbyteArray array = env->NewByteArray(n);
    env->SetByteArrayRegion(array, 0, n, static_cast<const jbyte *>(buf));
    env->CallVoidMethod(this->jniStream, methodWrite, array);
    bool ok;
    if (env->ExceptionOccurred() != 0) {
        env->ExceptionClear();
        ok = false;
    } else {
        ok = true;
    }
    env->DeleteLocalRef(array);
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

uint32_t FileInterfaceAndroid::GetFileSize() {
    fseek(this->file, 0, SEEK_END);
    int size = ftell(this->file);
    fseek(this->file, 0, SEEK_SET);
    return (uint32_t) size;
}


const char *FileInterfaceAndroid::GetAppRootDir() {
    return this->appRootDir;
}

uint32_t FileInterfaceAndroid::FileExist(String name) {
    String apkPath("assets/");
    apkPath += name;
    String zipPath("47947_GOF2CONTENT_ETC/assets/");
    zipPath += name;

    zip_file *apkFile = zip_fopen(APKArchive, apkPath.GetAEChar(), 0);
    zip_file *zipFile = zip_fopen(ZIPArchive, zipPath.GetAEChar(), 0);

    if (apkFile != 0) {
        zip_fclose(apkFile);
        return true;
    }
    if (zipFile != 0) {
        zip_fclose(zipFile);
        return true;
    }
    String dir(this->appRootDir);
    String full = dir + name;
    FILE *f = fopen(full.GetAEChar(), "rb");
    if (f != 0) {
        fclose(f);
        return true;
    }
    return false;
}

uint32_t FileInterfaceAndroid::FileDelete(String name) {
    String discard(name, false);
    return 0;
}

uint32_t FileInterfaceAndroid::GetDeviceFreeSpace() {
    return 0;
}

uint32_t FileInterfaceAndroid::FileEnumInit(char *, bool) {
    return 0;
}

uint32_t FileInterfaceAndroid::FileGetNextEnum(String &) {
    return 0;
}

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
        if (this->modeFlag != 0)
            env->CallVoidMethod(m, FileInterfaceAndroid::methodCloseWrite);
        else
            env->CallVoidMethod(m, FileInterfaceAndroid::methodCloseRead);
        this->jniStream = 0;
    }
}

void FileInterfaceAndroid::SetAppRootDir(void *p) { // lint: void_ptr virtual override param (FileInterface)
    // lint: void_ptr virtual override param (FileInterface)
    if (p != 0)
        this->appRootDir = (const char *) p;
}

struct zip *APKArchive = nullptr;
struct zip *ZIPArchive = nullptr;
void FileInterfaceAndroid::SetZipDirectory(void *p) { // lint: void_ptr virtual override param (FileInterface)
    // lint: void_ptr virtual override param (FileInterface)
    if (p != 0)
        this->zipDirectory = (const char *) p;
}

char *FileInterfaceAndroid::Output(char *) {
    return reinterpret_cast<char *>(this);
}

void FileInterfaceAndroid::SetSaveDirectory(String) {
}

void FileInterfaceAndroid::ResetSaveDirectory() {
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
