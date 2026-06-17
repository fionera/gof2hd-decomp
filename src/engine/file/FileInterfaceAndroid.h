#ifndef GOF2_FILEINTERFACEANDROID_H
#define GOF2_FILEINTERFACEANDROID_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/file/AEFile.h"

// File I/O abstraction for the Android build: reads and writes go through a stdio FILE*,
// a libzip zip_file* (for assets packed in the APK), or a JNI input/output stream.

// AbyssEngine::String is used by value in this header's signatures; bring it into scope.
using AbyssEngine::String;

// JNI handle: only ever used through a pointer here, so a forward declaration is enough.
struct _jobject;
typedef struct _jobject *jobject;

struct zip_file;

// The platform backend for the Android build. `enabled` (the live flag, historically `alive`) is
// inherited from FileInterface and shares offset +4; the instance fields below start at +8.
class FileInterfaceAndroid : public FileInterface {
public:
    void        *file;          // +0x08 stdio FILE*
    void        *zipFile;       // +0x0c zip_file*
    void        *jniStream;     // +0x10 jobject / jmethodID stream handle
    uint8_t      modeFlag;      // +0x14 append/write-mode flag
    char         pad_15[7];
    int32_t      zipReadPos;    // +0x1c  zip read position tracker (zeroed on open)
    char         pad_20[4];
    uint8_t      zipAppend;     // +0x24  append flag for the zip path
    char         pad_25[3];
    int32_t      zipReadLen;    // +0x28  zip read length tracker (zeroed on open)
    char         pad_2c[4];
    const char  *appRootDir;    // +0x30
    void        *zipDirectory;  // +0x34

    FileInterfaceAndroid();
    FileInterfaceAndroid(FILE *f, bool append);
    FileInterfaceAndroid(jobject *stream, bool reading);
    FileInterfaceAndroid(zip_file *zf, bool append, int start, int p4, int p5);
    ~FileInterfaceAndroid() override;

    // FileInterface backend slots.
    void       *OpenRead(String name, int p2, bool p3, int p4, int p5, unsigned int p6);
    void       *OpenWrite(const String &name, uint32_t p2, uint32_t p3) override;
    uint32_t    Read(uint32_t n, void *buf) override;
    uint32_t    Write(uint32_t n, const void *buf) override;
    uint32_t    Seek(uint32_t n) override;
    uint32_t    GetFileSize() override;
    uint32_t    FileExist(String name);
    const char *GetAppRootDir() override;
    uint32_t    GetDeviceFreeSpace() override;
    void        SetAppRootDir(const char *p) override;
    void        SetZipDirectory(const char *p) override;

    // Not part of the FileInterface backend dispatch, but invoked directly.
    void Close();
};

#endif
