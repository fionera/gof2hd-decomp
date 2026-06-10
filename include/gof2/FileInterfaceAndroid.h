#ifndef GOF2_FILEINTERFACEANDROID_H
#define GOF2_FILEINTERFACEANDROID_H
#include "gof2/common.h"
// Galaxy on Fire 2 - FileInterfaceAndroid class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class), with file I/O over stdio FILE*,
// libzip zip_file*, and JNI streams. Real named struct; field_0xNN member names are
// kept so cross-class references resolve. No byte-offset access.
//
// Layout (from the ctors / Close / Read / Write):
//   0x00 vtable
//   0x04 alive/refcount byte (set 1 in default ctor)
//   0x08 FILE*           (stdio handle)
//   0x0c zip_file*       (libzip handle)
//   0x10 jmethodID* / jobject stream handle
//   0x14 byte            (append/write-mode flag)
//   0x1c int
//   0x24 byte
//   0x28 int
//   0x30 void* AppRootDir
//   0x34 void* ZipDirectory

// AbyssEngine::String passed/returned by value: 12-byte trivially-copied aggregate.
// String12 lives in namespace AbyssEngine (see common.h) and is not pulled in by a
// global using-declaration there, so bring it into scope for this header's signatures.
using AbyssEngine::String12;

// JNI handle: only ever used through a pointer here, so a forward declaration is enough.
struct _jobject;
typedef struct _jobject *jobject;

struct zip_file;

struct FileInterfaceAndroid {
    char        *field_0x0;             // +0x0  vtable
    uint8_t      field_0x4;             // +0x4  alive byte
    char         pad_5[3];
    void        *field_0x8;             // +0x8  stdio FILE*
    void        *field_0xc;             // +0xc  zip_file*
    void        *field_0x10;            // +0x10 jobject / jmethodID stream handle
    uint8_t      field_0x14;            // +0x14 append/write-mode flag
    char         pad_15[7];
    int32_t      field_0x1c;            // +0x1c
    char         pad_20[4];
    uint8_t      field_0x24;            // +0x24
    char         pad_25[3];
    int32_t      field_0x28;            // +0x28
    char         pad_2c[4];
    const char  *field_0x30;            // +0x30 AppRootDir
    void        *field_0x34;            // +0x34 ZipDirectory

    FileInterfaceAndroid();
    ~FileInterfaceAndroid();

    // ---- methods (converted from free functions) ----
    void Close();
    bool FileExist(String12 name);
    int GetFileSize();
    FileInterfaceAndroid * OpenRead(String12 name, int p2, bool p3, int p4, int p5, unsigned int p6);
    FileInterfaceAndroid * OpenWrite(String12 name, int, bool, unsigned int);
    bool Read(unsigned int n, void *buf);
    bool Seek(unsigned int n);
    void SetAppRootDir(void *p);
    void SetZipDirectory(void *p);
    bool Write(unsigned int n, const void *buf);
    void ctor_default();
    void ctor_file(FILE *f, bool append);
    FileInterfaceAndroid * ctor_obj(jobject *stream, bool reading);
    FileInterfaceAndroid * ctor_zip(zip_file *zf, bool append, int start, int p4, int p5);
};

#endif
