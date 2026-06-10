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
};

#endif
