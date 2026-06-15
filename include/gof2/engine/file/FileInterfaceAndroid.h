#ifndef GOF2_FILEINTERFACEANDROID_H
#define GOF2_FILEINTERFACEANDROID_H
#include "gof2/common.h"

// File I/O abstraction for the Android build: reads and writes go through a stdio FILE*,
// a libzip zip_file* (for assets packed in the APK), or a JNI input/output stream.

// AbyssEngine::String is used by value in this header's signatures; bring it into scope.
using AbyssEngine::String;

// JNI handle: only ever used through a pointer here, so a forward declaration is enough.
struct _jobject;
typedef struct _jobject *jobject;

struct zip_file;

class FileInterfaceAndroid {
public:
    char        *vtable;
    uint8_t      alive;
    char         pad_5[3];
    void        *file;          // stdio FILE*
    void        *zipFile;       // zip_file*
    void        *jniStream;     // jobject / jmethodID stream handle
    uint8_t      modeFlag;      // append/write-mode flag
    char         pad_15[7];
    int32_t      field_0x1c;
    char         pad_20[4];
    uint8_t      field_0x24;
    char         pad_25[3];
    int32_t      field_0x28;
    char         pad_2c[4];
    const char  *appRootDir;
    void        *zipDirectory;

    FileInterfaceAndroid();
    FileInterfaceAndroid(FILE *f, bool append);
    FileInterfaceAndroid(jobject *stream, bool reading);
    FileInterfaceAndroid(zip_file *zf, bool append, int start, int p4, int p5);
    ~FileInterfaceAndroid();

    void Close();
    bool FileExist(String name);
    int GetFileSize();
    FileInterfaceAndroid *OpenRead(String name, int p2, bool p3, int p4, int p5, unsigned int p6);
    FileInterfaceAndroid *OpenWrite(String name, int, bool, unsigned int);
    bool Read(unsigned int n, void *buf);
    bool Seek(unsigned int n);
    void SetAppRootDir(void *p);
    void SetZipDirectory(void *p);
    bool Write(unsigned int n, const void *buf);
};

#endif
