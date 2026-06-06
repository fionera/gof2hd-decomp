#ifndef GOF2_DECOMP_AEFILE_CLASS_H
#define GOF2_DECOMP_AEFILE_CLASS_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

namespace AbyssEngine {

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;

    String();
    String(const char *text, bool copy);
    String(const uint16_t *text, bool copy);
    String(const String &other, bool copy);
    ~String();

    String &operator=(const String &other);
    uint16_t *operator[](uint32_t index);
    operator uint16_t *();

    char *GetAEChar() const;
    uint16_t *GetAEWChar() const;
    int32_t IndexOf(const String &needle) const;
    void Set(const char *text);
    void Set(const uint16_t *text);
};

String operator+(const String &left, const String &right);

} // namespace AbyssEngine

using uint = uint32_t;
using uchar = uint8_t;
using ushort = uint16_t;
using longlong = int64_t;
using String = AbyssEngine::String;

template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t size2;

    Array();
    ~Array();
};

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

struct AEFile {
    enum FileOpenType {
        OPEN_WRITE = 0,
        OPEN_READ = 1,
        OPEN_APPEND = 2,
    };

    static void SetInterface(FileInterface *fileInterface);
    static void Release();
    static void sortPakFileEntryList();
    static void collectFilesInPakFiles(String &path);

    static uint32_t OpenRead(String &path, uint32_t *handle);
    static uint32_t Read(uint32_t bytes, void *buffer, uint32_t handle);
    static uint32_t crc32_ccitt(const String &text);
    static uint32_t Skip(uint32_t bytes, uint32_t handle);
    static void Close(uint32_t handle);
    static void RegisterPakFile(String &path);
    static void collectPakFiles(const String &path);
    static AELowLevelFile *findPakFile(const String &path);
    static uint32_t Open(String &path, FileOpenType openType, uint32_t *handle);

    static uint32_t OpenRead(const char *path, uint32_t *handle);
    static uint32_t OpenWrite(String &path, uint32_t *handle);
    static uint32_t OpenWrite(const char *path, uint32_t *handle);
    static uint32_t OpenAppend(String &path, uint32_t *handle);
    static uint32_t OpenAppend(const char *path, uint32_t *handle);

    static uint32_t Read(char &value, uint32_t handle);
    static uint32_t Read(uint32_t &value, uint32_t handle);
    static uint32_t Read(int32_t &value, uint32_t handle);
    static uint32_t Read(uint16_t &value, uint32_t handle);
    static uint32_t Read(int16_t &value, uint32_t handle);
    static uint32_t Read(uint8_t &value, uint32_t handle);
    static uint32_t Read(int8_t &value, uint32_t handle);
    static uint32_t Read(int64_t &value, uint32_t handle);
    static uint32_t Read(bool &value, uint32_t handle);
    static uint32_t Read(float &value, uint32_t handle);
    static uint32_t Read(String &value, uint32_t handle, bool wide);

    static uint32_t ReadSwitched(int32_t &value, uint32_t handle);
    static uint32_t ReadSwitched(uint16_t &value, uint32_t handle);
    static uint32_t ReadSwitched(int16_t &value, uint32_t handle);
    static uint32_t ReadSwitched(String &value, uint32_t handle, bool wide);

    static uint32_t Write(uint32_t bytes, const void *buffer, uint32_t handle);
    static void Write(char value, uint32_t handle);
    static void Write(uint32_t value, uint32_t handle);
    static void Write(int32_t value, uint32_t handle);
    static void Write(uint16_t value, uint32_t handle);
    static void Write(int16_t value, uint32_t handle);
    static void Write(uint8_t value, uint32_t handle);
    static void Write(int8_t value, uint32_t handle);
    static void Write(bool value, uint32_t handle);
    static void Write(float value, uint32_t handle);
    static void Write(int64_t value, uint32_t handle);
    static uint32_t Write(const String &value, uint32_t handle, bool wide);

    static uint32_t GetFileSize(uint32_t handle);
    static uint32_t FileExist(const String &path);
    static uint32_t FileDelete(const String &path);
    static uint32_t GetDeviceFreeSpace();
    static const char *GetAppRootDir();
    static void SetAppRootDir(const char *path);
    static void SetZipDirectory(const char *path);
    static void SetSaveDirectory(String path);
    static void ResetSaveDirectory();
};

static_assert(sizeof(String) == 0xc, "String layout");
static_assert(__builtin_offsetof(String, size) == 0x8, "String::size");
static_assert(sizeof(Array<void *>) == 0xc, "Array layout");
static_assert(sizeof(AEPakFileEntry) == 0x1c, "AEPakFileEntry layout");
static_assert(__builtin_offsetof(AEPakFileEntry, crc) == 0x0, "AEPakFileEntry::crc");
static_assert(__builtin_offsetof(AEPakFileEntry, name) == 0x4, "AEPakFileEntry::name");
static_assert(__builtin_offsetof(AEPakFileEntry, offset) == 0x10, "AEPakFileEntry::offset");
static_assert(__builtin_offsetof(AEPakFileEntry, packedSize) == 0x14, "AEPakFileEntry::packedSize");
static_assert(__builtin_offsetof(AEPakFileEntry, size) == 0x18, "AEPakFileEntry::size");
static_assert(sizeof(AELowLevelNativeFile) == 0x8, "AELowLevelNativeFile layout");
static_assert(sizeof(AELowLevelPakFile) == 0x14, "AELowLevelPakFile layout");
static_assert(__builtin_offsetof(AELowLevelPakFile, handle) == 0x4, "AELowLevelPakFile::handle");
static_assert(__builtin_offsetof(AELowLevelPakFile, packedSize) == 0x8, "AELowLevelPakFile::packedSize");
static_assert(__builtin_offsetof(AELowLevelPakFile, size) == 0xc, "AELowLevelPakFile::size");
static_assert(__builtin_offsetof(AELowLevelPakFile, position) == 0x10, "AELowLevelPakFile::position");
static_assert(__builtin_offsetof(FileInterface, enabled) == 0x4, "FileInterface::enabled");

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(...);
void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
void *operator new[](uint32_t size);
void operator delete[](void *ptr) noexcept;

template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayReleaseClasses(Array<T> &array);

#endif
