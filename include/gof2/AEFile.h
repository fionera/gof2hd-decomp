#ifndef GOF2_AEFILE_H
#define GOF2_AEFILE_H
#include "gof2/common.h"
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
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;

template <class T>
void ArrayAdd(T item, Array<T> &array);

template <class T>
void ArrayReleaseClasses(Array<T> &array);

struct AEFile { void* _opaque; };  // no offset accesses observed
#endif
