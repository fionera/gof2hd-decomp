#ifndef GOF2_AEFILE_H
#define GOF2_AEFILE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

using String = AbyssEngine::String;

struct AEPakFileEntry {
    uint32_t crc;
    String name;
    uint32_t offset;
    uint32_t packedSize;
    uint32_t size;
};

class AELowLevelFile {
public:
    virtual ~AELowLevelFile() {
    }

    virtual uint32_t Write(uint32_t bytes, void *buffer) { return 0; }
    virtual uint32_t Read(uint32_t bytes, void *buffer) { return 0; }
    virtual uint32_t Skip(uint32_t bytes) { return 0; }
    virtual uint32_t Release() { return 1; }
    virtual uint32_t GetFileSize() { return 0; }
};

class AELowLevelHeldFile {
public:
    virtual ~AELowLevelHeldFile() {
    } // +0x04 Free
    virtual void *OpenRead(const String &, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) { return nullptr; }
    virtual void *OpenWrite(const String &, uint32_t, uint32_t) { return nullptr; }
    virtual void *OpenAppend(const String &, uint32_t, uint32_t) { return nullptr; }
    virtual uint32_t Read(uint32_t bytes, void *buffer) { return 0; } // +0x14
    virtual uint32_t Write(uint32_t bytes, const void *buffer) { return 0; } // +0x18
    virtual uint32_t Skip(uint32_t bytes) { return 0; } // +0x1c
    virtual uint32_t GetFileSize() { return 0; } // +0x20
};

class FileInterface;

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

class AEPakFile : public AELowLevelFile {
public:
    FileInterface *fileInterface;
    uint32_t sizeLimit;
    uint32_t baseOffset;
    uint32_t position;

    AEPakFile(FileInterface *fileInterface, int sizeLimit, int baseOffset);

    ~AEPakFile() override { Release(); }

    uint32_t Write(uint32_t bytes, void *buffer) override;

    uint32_t Read(uint32_t bytes, void *buffer) override;

    uint32_t Skip(uint32_t bytes) override;

    uint32_t Release() override;

    uint32_t GetFileSize() override;
};

class FileInterface {
public:
    uint8_t enabled;

    virtual ~FileInterface() {
    }

    virtual void *OpenRead(String name, int size, bool windowed, int packedSize, int rawSize, unsigned int offset) = 0;

    virtual void *OpenWrite(String name, int size, bool append, unsigned int mode) = 0;

    virtual void *OpenAppend(String name, int size, bool append, unsigned int mode) = 0;

    virtual uint32_t Read(uint32_t bytes, void *buffer) = 0;

    virtual uint32_t Write(uint32_t bytes, const void *buffer) = 0;

    virtual uint32_t Seek(uint32_t bytes) = 0;

    virtual uint32_t GetFileSize() = 0;

    virtual uint32_t FileExist(String name) = 0;

    virtual uint32_t FileDelete(String name) = 0;

    virtual uint32_t GetDeviceFreeSpace() = 0;

    virtual const char *GetAppRootDir() = 0;

    virtual void SetAppRootDir(void *path) = 0;

    virtual void SetZipDirectory(void *path) = 0;

    virtual uint32_t FileEnumInit(char *pattern, bool recurse) = 0;

    virtual uint32_t FileGetNextEnum(String &out) = 0;

    virtual void Close() = 0;

    virtual String GetDirPreFix() = 0;

    virtual char *Output(char *line) = 0;

    virtual void SetSaveDirectory(String dir) = 0;

    virtual void ResetSaveDirectory() = 0;
};

void *OpenAppend(unsigned short *name, int size, bool append, unsigned int mode);

class AEFile {
public:
    enum FileOpenType : uint32_t {
        OPEN_READ = 0,
        OPEN_WRITE = 1,
        OPEN_APPEND = 2,
    };

    static void SetInterface(FileInterface *fileInterface);

    static void Release();

    static uint32_t Open(String &path, FileOpenType openType, uint32_t *handle);

    static uint32_t OpenRead(String &path, uint32_t *handle);

    static uint32_t OpenRead(const char *path, uint32_t *handle);

    static uint32_t OpenWrite(String &path, uint32_t *handle);

    static uint32_t OpenWrite(const char *path, uint32_t *handle);

    static uint32_t OpenAppend(String &path, uint32_t *handle);

    static uint32_t OpenAppend(const char *path, uint32_t *handle);

    static void Close(uint32_t handle);

    static uint32_t Read(uint32_t bytes, void *buffer, uint32_t handle);

    static uint32_t Read(bool &value, uint32_t handle);

    static uint32_t Read(char &value, uint32_t handle);

    static uint32_t Read(int8_t &value, uint32_t handle);

    static uint32_t Read(uint8_t &value, uint32_t handle);

    static uint32_t Read(int16_t &value, uint32_t handle);

    static uint32_t Read(uint16_t &value, uint32_t handle);

    static uint32_t Read(int32_t &value, uint32_t handle);

    static uint32_t Read(uint32_t &value, uint32_t handle);

    static uint32_t Read(int64_t &value, uint32_t handle);

    static uint32_t Read(float &value, uint32_t handle);

    static uint32_t Read(String &value, uint32_t handle, bool wide);

    static uint32_t ReadSwitched(int16_t &value, uint32_t handle);

    static uint32_t ReadSwitched(uint16_t &value, uint32_t handle);

    static uint32_t ReadSwitched(int32_t &value, uint32_t handle);

    static void ReadSwitched(String &value, uint32_t handle);

    static uint32_t Write(uint32_t bytes, void *buffer, uint32_t handle);

    static void Write(bool value, uint32_t handle);

    static void Write(char value, uint32_t handle);

    static void Write(int8_t value, uint32_t handle);

    static void Write(uint8_t value, uint32_t handle);

    static void Write(int16_t value, uint32_t handle);

    static void Write(uint16_t value, uint32_t handle);

    static void Write(int32_t value, uint32_t handle);

    static void Write(uint32_t value, uint32_t handle);

    static void Write(int64_t value, uint32_t handle);

    static void Write(float value, uint32_t handle);

    static uint32_t Write(const String &value, uint32_t handle, bool wide);

    static uint32_t Skip(uint32_t bytes, uint32_t handle);

    static uint32_t GetFileSize(uint32_t handle);

    static void RegisterPakFile(String path);

    static void collectPakFiles(const String &path);

    static void collectFilesInPakFiles(String &path);

    static void sortPakFileEntryList();

    static AELowLevelFile *findPakFile(const String &path);

    static uint32_t crc32_ccitt(const String &text);

    static uint32_t FileExist(const String &path);

    static uint32_t FileDelete(const String &path);

    static uint32_t GetDeviceFreeSpace();

    static const char *GetAppRootDir();

    static void SetAppRootDir(void *path);

    static void SetZipDirectory(void *path);

    static void SetSaveDirectory(String path);

    static void ResetSaveDirectory();
};
#endif
