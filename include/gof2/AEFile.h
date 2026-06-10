#ifndef GOF2_AEFILE_H
#define GOF2_AEFILE_H
#include "gof2/common.h"
#include "gof2/String.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {



String operator+(const String &left, const String &right);

} // namespace AbyssEngine

using uint = uint32_t;
using uchar = uint8_t;
using ushort = uint16_t;
using longlong = int64_t;
using String = AbyssEngine::String;



struct AEPakFileEntry {
    uint32_t crc;
    String name;
    uint32_t offset;
    uint32_t packedSize;
    uint32_t size;
};

struct AELowLevelFile;

struct AELowLevelFileVTable {
    void (*field_00)(AELowLevelFile *);
    void (*Close)(AELowLevelFile *);
    uint32_t (*Write)(AELowLevelFile *, uint32_t, const void *);
    uint32_t (*Read)(AELowLevelFile *, uint32_t, void *);
    uint32_t (*Skip)(AELowLevelFile *, uint32_t);
    uint32_t (*field_14)(AELowLevelFile *);
    uint32_t (*GetFileSize)(AELowLevelFile *);
};

struct AELowLevelFile {
    AELowLevelFileVTable *vtable;
};

struct AELowLevelNativeFile : AELowLevelFile {
    void *handle;
};

struct AELowLevelPakFile : AELowLevelFile {
    void *handle;
    uint32_t packedSize;
    uint32_t size;
    uint32_t position;
};

struct FileInterface;

struct FileInterfaceVTable {
    void (*field_00)(FileInterface *);
    void (*field_04)(FileInterface *);
    void *(*OpenRead)(FileInterface *, const String &, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
    void *(*OpenWrite)(FileInterface *, const String &, uint32_t, uint32_t);
    void *(*OpenAppend)(FileInterface *, const String &, uint32_t, uint32_t, uint32_t);
    void (*field_14)(FileInterface *);
    void (*field_18)(FileInterface *);
    void (*field_1c)(FileInterface *);
    void (*field_20)(FileInterface *);
    uint32_t (*FileExist)(FileInterface *, const String &);
    uint32_t (*FileDelete)(FileInterface *, const String &);
    uint32_t (*GetDeviceFreeSpace)(FileInterface *);
    const char *(*GetAppRootDir)(FileInterface *);
    void (*ResetSaveDirectory)(FileInterface *);
    void (*field_38)(FileInterface *);
    uint32_t (*OpenDirectory)(FileInterface *, void *, uint32_t);
    uint32_t (*ReadDirectory)(FileInterface *, String &);
    void (*field_44)(FileInterface *);
    void (*SetAppRootDir)(FileInterface *, const char *);
    void (*SetZipDirectory)(FileInterface *, const char *);
    void (*SetSaveDirectory)(FileInterface *, const String &);
};

struct FileInterface {
    FileInterfaceVTable *vtable;
    uint8_t enabled;
};



// NOTE: AEPakFileEntry::name holds the std::u16string-backed String (common.h), so the original
// 0x1c byte layout no longer holds. The low-level file structs derive from AELowLevelFile (a
// vtable-pointer base), which is the natural native layout: vtable at 0x0, then members.

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;

template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayReleaseClasses(Array<T> &array);

enum FileOpenType : uint32_t {
    OPEN_READ   = 0,
    OPEN_WRITE  = 1,
    OPEN_APPEND = 2,
};

// AEFile is a fully static utility class (a namespace of file I/O routines over the active
// FileInterface plus the registered .pak archives). No instance state.
struct AEFile {
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
    static uint32_t    ReadSwitched(String &value, uint32_t handle, bool wide);

    static uint32_t    Write(uint32_t bytes, const void *buffer, uint32_t handle);
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

    static void        RegisterPakFile(String &path);
    static void        collectPakFiles(const String &path);
    static void        collectFilesInPakFiles(String &path);
    static void        sortPakFileEntryList();
    static AELowLevelFile *findPakFile(const String &path);
    static uint32_t    crc32_ccitt(const String &text);

    static uint32_t    FileExist(const String &path);
    static uint32_t    FileDelete(const String &path);
    static uint32_t    GetDeviceFreeSpace();
    static const char *GetAppRootDir();
    static void        SetAppRootDir(const char *path);
    static void        SetZipDirectory(const char *path);
    static void        SetSaveDirectory(String path);
    static void        ResetSaveDirectory();
};
#endif
