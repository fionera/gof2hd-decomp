#ifndef GOF2_AEFILE_H
#define GOF2_AEFILE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

using String = AbyssEngine::String;

// A single entry in a registered .pak archive: the CRC32 of its name (the lookup key), the
// original name, and the location/size of its data within the archive.
struct AEPakFileEntry {
    uint32_t crc;
    String   name;
    uint32_t offset;
    uint32_t packedSize;
    uint32_t size;
};

// Low-level file backend: an opened file handle. The base is abstract; concrete backends
// (native FILE* / windowed pak entry) override the I/O slots. The vtable layout recovered from
// the binary is: [field_00 = complete dtor][Close = deleting dtor][Write][Read][Skip][Release =
// field_14][GetFileSize]; the virtual destructor occupies the first two slots, so the engine's
// "Close" slot is just `delete this` and is expressed at call sites as `delete file`.
class AELowLevelFile {
public:
    virtual ~AELowLevelFile() {}
    virtual uint32_t Write(uint32_t bytes, void *buffer)       { return 0; }
    virtual uint32_t Read(uint32_t bytes, void *buffer)        { return 0; }
    virtual uint32_t Skip(uint32_t bytes)                      { return 0; }
    virtual uint32_t Release()                                 { return 1; }
    virtual uint32_t GetFileSize()                             { return 0; }
};

// A minimal held low-level file object, reached only through this fixed set of slots. AEFile's
// native / pak backends forward their I/O to one of these. (At runtime the held object is a
// FileInterfaceAndroid, defined in its own translation unit.)
class AELowLevelHeldFile {
public:
    virtual ~AELowLevelHeldFile() {}                                      // +0x04 Free
    virtual void     *OpenRead(const String &, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)  { return nullptr; }
    virtual void     *OpenWrite(const String &, uint32_t, uint32_t)       { return nullptr; }
    virtual void     *OpenAppend(const String &, uint32_t, uint32_t)      { return nullptr; }
    virtual uint32_t  Read(uint32_t bytes, void *buffer)                  { return 0; }     // +0x14
    virtual uint32_t  Write(uint32_t bytes, const void *buffer)           { return 0; }     // +0x18
    virtual uint32_t  Skip(uint32_t bytes)                                { return 0; }     // +0x1c
    virtual uint32_t  GetFileSize()                                       { return 0; }     // +0x20
};

// Forward declaration: the held file handle returned by FileInterface::OpenRead/OpenWrite is itself
// a FileInterface (the platform FileInterfaceAndroid). The concrete class is defined below.
class FileInterface;

// Native file backend: forwards every operation to the platform file handle it holds.
class AENormalFile : public AELowLevelFile {
public:
    FileInterface *file;

    AENormalFile(FileInterface *file);
    ~AENormalFile() override { Release(); }
    uint32_t Write(uint32_t bytes, void *buffer) override;
    uint32_t Read(uint32_t bytes, void *buffer) override;
    uint32_t Skip(uint32_t bytes) override;
    uint32_t Release() override;
    uint32_t GetFileSize() override;
};

// Pak file backend: a windowed view (base offset + size limit) over the held platform file.
// It forwards reads to the held FileInterface while clamping them to the [baseOffset, sizeLimit)
// window carved out for this archived entry.
class AEPakFile : public AELowLevelFile {
public:
    FileInterface *fileInterface;
    uint32_t       sizeLimit;
    uint32_t       baseOffset;
    uint32_t       position;

    AEPakFile(FileInterface *fileInterface, int sizeLimit, int baseOffset);
    ~AEPakFile() override { Release(); }
    uint32_t Write(uint32_t bytes, void *buffer) override;
    uint32_t Read(uint32_t bytes, void *buffer) override;
    uint32_t Skip(uint32_t bytes) override;
    uint32_t Release() override;
    uint32_t GetFileSize() override;
};

// Platform file-system backend. Pure-abstract base for the concrete platform implementation
// (FileInterfaceAndroid), which overrides every slot. The base declares only the interface, so
// the original emits typeinfo for it but no vtable and no out-of-line method bodies; the virtual
// destructor occupies the first two vtable slots, then the I/O slots follow in the order below
// (recovered from the FileInterfaceAndroid vtable). `enabled` is the only instance state.
//
// The slot signatures match the derived overrides exactly: the file name is passed by value as an
// AbyssEngine::String, and the open calls carry the extra mode/window parameters the platform
// backend needs.
class FileInterface {
public:
    uint8_t enabled;

    virtual ~FileInterface() {}
    virtual void       *OpenRead(String name, int size, bool windowed, int packedSize, int rawSize, unsigned int offset) = 0;
    virtual void       *OpenWrite(String name, int size, bool append, unsigned int mode) = 0;
    virtual void       *OpenAppend(String name, int size, bool append, unsigned int mode) = 0;
    virtual uint32_t    Read(uint32_t bytes, void *buffer)            = 0;
    virtual uint32_t    Write(uint32_t bytes, const void *buffer)     = 0;
    virtual uint32_t    Seek(uint32_t bytes)                          = 0;
    virtual uint32_t    GetFileSize()                                 = 0;
    virtual uint32_t    FileExist(String name)                        = 0;
    virtual uint32_t    FileDelete(String name)                       = 0;
    virtual uint32_t    GetDeviceFreeSpace()                          = 0;
    virtual const char *GetAppRootDir()                               = 0;
    virtual void        SetAppRootDir(void *path)                     = 0;
    virtual void        SetZipDirectory(void *path)                   = 0;
    virtual uint32_t    FileEnumInit(char *pattern, bool recurse)     = 0;
    virtual uint32_t    FileGetNextEnum(String &out)                  = 0;
    virtual void        Close()                                       = 0;
    virtual String      GetDirPreFix()                                = 0;
    virtual char       *Output(char *line)                            = 0;
    virtual void        SetSaveDirectory(String dir)                  = 0;
    virtual void        ResetSaveDirectory()                          = 0;
};

// Free-function append-open entry point exported by the engine. On this build it is a stub: the
// append-mode file path was never wired up, so it ignores its arguments and reports failure (a null
// handle). The wide name buffer / size / append flag / mode parameters mirror the real OpenWrite
// slot signature.
void *OpenAppend(unsigned short *name, int size, bool append, unsigned int mode);

// AEFile is a fully static utility class: a collection of file I/O routines over the active
// FileInterface plus the registered .pak archives. It has no instance state.
class AEFile {
public:
    enum FileOpenType : uint32_t {
        OPEN_READ   = 0,
        OPEN_WRITE  = 1,
        OPEN_APPEND = 2,
    };

    static void        SetInterface(FileInterface *fileInterface);
    static void        Release();

    static uint32_t    Open(String &path, FileOpenType openType, uint32_t *handle);
    static uint32_t    OpenRead(String &path, uint32_t *handle);
    static uint32_t    OpenRead(const char *path, uint32_t *handle);
    static uint32_t    OpenWrite(String &path, uint32_t *handle);
    static uint32_t    OpenWrite(const char *path, uint32_t *handle);
    static uint32_t    OpenAppend(String &path, uint32_t *handle);
    static uint32_t    OpenAppend(const char *path, uint32_t *handle);
    static void        Close(uint32_t handle);

    static uint32_t    Read(uint32_t bytes, void *buffer, uint32_t handle);
    static uint32_t    Read(bool &value, uint32_t handle);
    static uint32_t    Read(char &value, uint32_t handle);
    static uint32_t    Read(int8_t &value, uint32_t handle);
    static uint32_t    Read(uint8_t &value, uint32_t handle);
    static uint32_t    Read(int16_t &value, uint32_t handle);
    static uint32_t    Read(uint16_t &value, uint32_t handle);
    static uint32_t    Read(int32_t &value, uint32_t handle);
    static uint32_t    Read(uint32_t &value, uint32_t handle);
    static uint32_t    Read(int64_t &value, uint32_t handle);
    static uint32_t    Read(float &value, uint32_t handle);
    static uint32_t    Read(String &value, uint32_t handle, bool wide);

    static uint32_t    ReadSwitched(int16_t &value, uint32_t handle);
    static uint32_t    ReadSwitched(uint16_t &value, uint32_t handle);
    static uint32_t    ReadSwitched(int32_t &value, uint32_t handle);
    static void        ReadSwitched(String &value, uint32_t handle);
    static uint32_t    ReadSwitched(String &value, uint32_t handle, bool wide);

    static uint32_t    Write(uint32_t bytes, void *buffer, uint32_t handle);
    static void        Write(bool value, uint32_t handle);
    static void        Write(char value, uint32_t handle);
    static void        Write(int8_t value, uint32_t handle);
    static void        Write(uint8_t value, uint32_t handle);
    static void        Write(int16_t value, uint32_t handle);
    static void        Write(uint16_t value, uint32_t handle);
    static void        Write(int32_t value, uint32_t handle);
    static void        Write(uint32_t value, uint32_t handle);
    static void        Write(int64_t value, uint32_t handle);
    static void        Write(float value, uint32_t handle);
    static uint32_t    Write(const String &value, uint32_t handle, bool wide);

    static uint32_t    Skip(uint32_t bytes, uint32_t handle);
    static uint32_t    GetFileSize(uint32_t handle);

    static void        RegisterPakFile(String path);
    static void        collectPakFiles(const String &path);
    static void        collectFilesInPakFiles(String &path);
    static void        sortPakFileEntryList();
    static AELowLevelFile *findPakFile(const String &path);
    static uint32_t    crc32_ccitt(const String &text);

    static uint32_t    FileExist(const String &path);
    static uint32_t    FileDelete(const String &path);
    static uint32_t    GetDeviceFreeSpace();
    static const char *GetAppRootDir();
    static void        SetAppRootDir(void *path);
    static void        SetZipDirectory(void *path);
    static void        SetSaveDirectory(String path);
    static void        ResetSaveDirectory();
};
#endif
