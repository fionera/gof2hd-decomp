#include "gof2/AEFile.h"
#include "gof2/String.h"
#include <new>   // placement new used by the in-place String construction below

// ---------------------------------------------------------------------------------------------
// String adapter helpers.
// The recovered AEFile code used the engine String's member API (ctor(const char*,bool),
// GetAEChar(), GetAEWChar(), Set(), IndexOf(), operator[]). The canonical String (common.h) is
// backed by std::u16string and exposes that API as the C entry points declared in String.h.
// These thin inline helpers bridge the two so the bodies read naturally and route through the
// real, ABI-stable String implementation in src/String.cpp.
namespace {

// Construct a String in place from a NUL-terminated 8-bit string.
inline void AEStr_init(String *self, const char *s)            { ((String *)(self))->ctor_char(s, false); }
// Construct a String in place from a NUL-terminated UTF-16 buffer.
inline void AEStr_init(String *self, const uint16_t *s)        { ((String *)(self))->ctor_wchar(s, false); }
// Construct a String in place as a copy of another String.
inline void AEStr_init(String *self, const String &other)      { ((String *)(self))->ctor_copy(const_cast<String *>(&other), false); }

// Reset a String's contents from a NUL-terminated 8-bit / UTF-16 buffer.
inline void AEStr_set(String &self, const char *s)             { ((String *)(&self))->Set_char(s); }
inline void AEStr_set(String &self, const uint16_t *s)         { ((String *)(&self))->Set_wchar(s); }

// Raw UTF-16 backing buffer (engine GetAEWChar / implicit uint16_t* conversion).
inline uint16_t *AEStr_wchar(const String &self)               { return reinterpret_cast<uint16_t *>(const_cast<char16_t *>(self.text())); }
// Newly allocated 8-bit (low-byte) copy (engine GetAEChar; caller frees with operator delete).
inline char     *AEStr_char(const String &self)                { return ((String *)(const_cast<String *>(&self)))->GetAEChar(); }
// UTF-16 code unit at index i.
inline uint16_t *AEStr_index(const String &self, int i)        { return ((String *)(const_cast<String *>(&self)))->index(i); }
// First index of needle, or 0xffffffff if absent.
inline uint32_t  AEStr_indexOf(const String &self, const String &needle) { return ((String *)(const_cast<String *>(&self)))->IndexOf(const_cast<String *>(&needle)); }

} // namespace



// ---- OpenRead_70530.cpp ----
uint32_t AEFile::OpenRead(String &path, uint32_t *handle) {
    return Open(path, OPEN_READ, handle);
}

// ---- OpenAppend_70b8c.cpp ----
uint32_t AEFile::OpenAppend(String &path, uint32_t *handle) {
    return Open(path, OPEN_APPEND, handle);
}

// ---- Close_70610.cpp ----
extern FileInterface *g_fileInterface;
extern Array<AELowLevelFile *> *g_openFiles;

__attribute__((minsize)) void AEFile::Close(uint32_t handle) {
    if (g_fileInterface != 0) {
        Array<AELowLevelFile *> *files = g_openFiles;
        if (files->size() <= handle) {
            goto done;
        }
        AELowLevelFile **slot = &files->data()[handle];
        AELowLevelFile *file = *slot;
        if (file != 0) {
            file->vtable->Close(file);
            slot = &g_openFiles->data()[handle];
        }
        *slot = 0;
    }
done:
    return;
}

// ---- Read_70c30.cpp ----
uint32_t AEFile::Read(bool &value, uint32_t handle)
{
    return Read(1, &value, handle);
}

// ---- OpenWrite_70b38.cpp ----
uint32_t AEFile::OpenWrite(String &path, uint32_t *handle) {
    return Open(path, OPEN_WRITE, handle);
}

// ---- Read_70c26.cpp ----
uint32_t AEFile::Read(int64_t &value, uint32_t handle)
{
    return AEFile::Read(8, &value, handle);
}

// ---- RegisterPakFile_70658.cpp ----
void AEFile::RegisterPakFile(String &path)
{
    collectFilesInPakFiles(path);
    return sortPakFileEntryList();
}

// ---- Read_70c3a.cpp ----
uint32_t AEFile::Read(float &value, uint32_t handle)
{
    return Read(4, &value, handle);
}

// ---- Read_70c08.cpp ----
uint32_t AEFile::Read(int16_t &value, uint32_t handle)
{
    return Read(2, &value, handle);
}

// ---- Release_70254.cpp ----
extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern Array<AEPakFileEntry *> *g_AEFile_pakFiles;

__attribute__((minsize)) void AEFile::Release()
{
    if (g_AEFile_openFiles != 0) {
        ArrayReleaseClasses(*g_AEFile_openFiles);
        if (g_AEFile_openFiles != 0) {
            delete g_AEFile_openFiles;
        }
        g_AEFile_openFiles = 0;
    }

    Array<AEPakFileEntry *> *entries = g_AEFile_pakFiles;
    if (entries != 0) {
        uint32_t offset = 0;
        uint32_t index = 0;
        while (index < entries->size()) {
            AEPakFileEntry *entry = *(AEPakFileEntry **)((uint8_t *)entries->data() + offset);
            AEPakFileEntry **slot;
            if (entry == 0) {
                slot = &entries->data()[index];
            } else {
                entry->name.~String();
                operator delete(entry);
                slot = (AEPakFileEntry **)((uint8_t *)g_AEFile_pakFiles->data() + offset);
            }
            *slot = 0;
            index += 1;
            offset += 4;
            entries = g_AEFile_pakFiles;
        }
        if (entries != 0) {
            delete entries;
        }
        g_AEFile_pakFiles = 0;
    }
}

// ---- OpenAppend_70b94.cpp ----
__attribute__((minsize)) uint32_t AEFile::OpenAppend(const char *path, uint32_t *handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;
    {
        String string;
        AEStr_init(&string, path);
        result = OpenAppend(string, handle);
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

// ---- ReadSwitched_70d06.cpp ----
__attribute__((minsize)) uint32_t AEFile::ReadSwitched(uint16_t &value, uint32_t handle) {
    uint32_t result = Read(2, &value, handle);
    if (result != 0) {
        uint32_t v = value;
        value = (uint16_t)((v << 0x18) >> 0x10) | (v >> 8);
    }
    return result;
}

// ---- Read_70538.cpp ----
extern FileInterface *g_AEFile_fileInterface;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;

uint32_t AEFile::Read(uint32_t bytes, void *buffer, uint32_t handle)
{
    if (g_AEFile_fileInterface != 0 && handle < g_AEFile_openFiles->size()) {
        AELowLevelFile *file = g_AEFile_openFiles->data()[handle];
        if (file != 0) {
            return file->vtable->Read(file, bytes, buffer);
        }
    }
    return 0;
}

namespace {
__attribute__((used)) void ae_file_70538_trailer() {}
}

// ---- Read_70bea.cpp ----
uint32_t AEFile::Read(uint32_t &value, uint32_t handle)
{
    return Read(4, &value, handle);
}

// ---- Read_70c44.cpp ----
__attribute__((minsize)) uint32_t AEFile::Read(String &value, uint32_t handle, bool wide)
{
    uint32_t locals[2];
    uint32_t result = 0;

    locals[1] = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    result = Read(4, &locals[0], handle);

    if (wide) {
        if (result != 0) {
            uint32_t length = locals[0];
            uint16_t *buffer = new uint16_t[length + 1];
            result = Read(length << 1, buffer, handle);
            if (result != 0) {
                buffer[locals[0]] = 0;
                AEStr_set(value, buffer);
                result = 1;
            }
            delete[] buffer;
        } else {
            result = 0;
        }
    } else {
        if (result != 0) {
            uint32_t length = locals[0];
            char *buffer = new char[length + 1];
            result = Read(length, buffer, handle);
            if (result != 0) {
                buffer[locals[0]] = 0;
                AEStr_set(value, buffer);
                result = 1;
            }
            delete[] buffer;
        } else {
            result = 0;
        }
    }

    uint32_t stack_difference = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - locals[1];
    if (stack_difference == 0) {
        return result;
    }
    __stack_chk_fail(stack_difference);
}

// ---- GetDeviceFreeSpace_71228.cpp ----
extern FileInterface *fileInterface;

__attribute__((minsize))
uint32_t AEFile::GetDeviceFreeSpace() {
    FileInterface *interface = fileInterface;
    if (interface != 0) {
        return interface->vtable->GetDeviceFreeSpace(interface);
    }
    return 0;
}

// ---- Read_70be0.cpp ----
uint32_t AEFile::Read(char &value, uint32_t handle) {
    return Read(1, &value, handle);
}

// ---- SetAppRootDir_71258.cpp ----
extern FileInterface *g_AEFile_fileInterface;

__attribute__((minsize)) void AEFile::SetAppRootDir(const char *path) {
    FileInterface *fileInterface = g_AEFile_fileInterface;
    if (fileInterface != 0) {
        reinterpret_cast<void (*)(FileInterface *, const char *)>(
            fileInterface->vtable->ResetSaveDirectory)(fileInterface, path);
    }
}

// ---- OpenRead_70aec.cpp ----
__attribute__((minsize)) uint32_t AEFile::OpenRead(const char *path, uint32_t *handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;
    {
        String string;
        AEStr_init(&string, path);
        result = OpenRead(string, handle);
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Read_70bfe.cpp ----
uint32_t AEFile::Read(uint16_t &value, uint32_t handle)
{
    return Read(2, &value, handle);
}

// ---- Write_70fe4.cpp ----
uint32_t AEFile::Write(const String &value, uint32_t handle, bool wide)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;

    if (wide) {
        const uint16_t *text = AEStr_wchar(value);
        uint32_t size = value.size();

        result = Write(4, &size, handle);
        if (result != 0) {
            result = Write(value.size() << 1, text, handle);
        }
    } else {
        char *text = AEStr_char(value);
        uint32_t size = value.size();

        result = Write(4, &size, handle);
        if (result != 0) {
            result = Write(value.size(), text, handle);
        }
        operator delete(text);
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

// ---- ResetSaveDirectory_712f0.cpp ----
extern FileInterface *gAEFileInterface;

__attribute__((minsize))
void AEFile::ResetSaveDirectory()
{
    FileInterface *fileInterface = gAEFileInterface;
    if (fileInterface != 0) {
        typedef void (*ResetSaveDirectoryFn)(FileInterface *);
        ResetSaveDirectoryFn resetSaveDirectory =
            *reinterpret_cast<ResetSaveDirectoryFn *>(
                reinterpret_cast<uint8_t *>(fileInterface->vtable) + 0x54);
        resetSaveDirectory(fileInterface);
    }
}

// ---- OpenWrite_70b40.cpp ----
__attribute__((minsize)) uint32_t AEFile::OpenWrite(const char *path, uint32_t *handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t result;
    {
        String string;
        AEStr_init(&string, path);
        result = OpenWrite(string, handle);
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

// ---- ReadSwitched_70d22.cpp ----
__attribute__((minsize)) uint32_t AEFile::ReadSwitched(int16_t &value, uint32_t handle)
{
    uint32_t result = Read(2, &value, handle);
    if (result != 0) {
        uint32_t v = (uint16_t)value;
        value = (int16_t)((uint16_t)((v << 0x18) >> 0x10) | (v >> 8));
    }
    return result;
}

// ---- Read_70bf4.cpp ----
uint32_t AEFile::Read(int32_t &value, uint32_t handle) {
    return Read(4, &value, handle);
}

// ---- GetAppRootDir_71240.cpp ----
extern FileInterface *fileInterface;

__attribute__((minsize))
const char *AEFile::GetAppRootDir()
{
    FileInterface *interface = fileInterface;
    if (interface != 0) {
        return interface->vtable->GetAppRootDir(interface);
    }
    return 0;
}

// ---- Read_70c12.cpp ----
uint32_t AEFile::Read(uint8_t &value, uint32_t handle)
{
    return Read(1, &value, handle);
}

// ---- GetFileSize_71074.cpp ----
extern FileInterface *g_fileInterface;
extern Array<AELowLevelFile *> *g_openFiles;

uint32_t AEFile::GetFileSize(uint32_t handle) {
    if (g_fileInterface != 0) {
        Array<AELowLevelFile *> *files = g_openFiles;
        if (handle < files->size()) {
            AELowLevelFile *file = files->data()[handle];
            if (file != 0) {
                return file->vtable->GetFileSize(file);
            }
        }
    }
    return 0;
}

// ---- Skip_705d8.cpp ----
extern FileInterface *g_fileInterface;
extern Array<AELowLevelFile *> *g_openFiles;

uint32_t AEFile::Skip(uint32_t bytes, uint32_t handle)
{
    if (g_fileInterface != 0) {
        Array<AELowLevelFile *> *files = g_openFiles;
        if (handle < files->size()) {
            AELowLevelFile *file = files->data()[handle];
            if (file != 0) {
                return file->vtable->Skip(file, bytes);
            }
        }
    }
    return 0;
}

// ---- SetZipDirectory_71270.cpp ----
extern FileInterface *g_AEFile_fileInterface;

__attribute__((minsize))
void AEFile::SetZipDirectory(const char *path)
{
    FileInterface *fileInterface = g_AEFile_fileInterface;
    if (fileInterface != 0) {
        reinterpret_cast<void (*)(FileInterface *, const char *)>(fileInterface->vtable->field_38)(
            fileInterface, path);
    }
}

// ---- Write_70db4.cpp ----
extern FileInterface *gAEFileInterface;
extern Array<AELowLevelFile *> *gAEFileOpenFiles;

uint32_t AEFile::Write(uint32_t bytes, const void *buffer, uint32_t handle) {
    if (gAEFileInterface != 0 && handle < gAEFileOpenFiles->size()) {
        AELowLevelFile *file = gAEFileOpenFiles->data()[handle];
        if (file != 0) {
            return file->vtable->Write(file, bytes, buffer);
        }
    }

    return 0;
}

// ---- Read_70c1c.cpp ----
uint32_t AEFile::Read(int8_t &value, uint32_t handle)
{
    return Read(1, &value, handle);
}

// ---- ReadSwitched_70d40.cpp ----
__attribute__((minsize)) uint32_t AEFile::ReadSwitched(String &value, uint32_t handle, bool)
{
    void * volatile cookie = __stack_chk_guard;
    uint16_t length;
    uint32_t result = 0;

    if (ReadSwitched(length, handle) != 0) {
        uint32_t bytes = length;
        char *buffer = static_cast<char *>(operator new(bytes + 1));
        if (Read(bytes, buffer, handle) != 0) {
            buffer[length] = '\0';
            AEStr_set(value, buffer);
            result = 1;
        }
        operator delete(buffer);
    }

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

// ---- ReadSwitched_70cec.cpp ----
uint32_t __attribute__((cold)) AEFile::ReadSwitched(int32_t &value, uint32_t handle)
{
    uint32_t result = Read(4, &value, handle);
    if (__builtin_unpredictable(result != 0)) {
        value = __builtin_bswap32(value);
    }
    return result;
}

// ---- Open_708c4.cpp ----
extern FileInterface *g_AEFile_fileInterface;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern char g_AELowLevelNativeFile_vtable[];

typedef void (*StringDestructor)(String *);
extern StringDestructor g_AEString_destructor;

__attribute__((minsize)) uint32_t AEFile::Open(String &path, FileOpenType openType, uint32_t *handle)
{
    FileInterface *fileInterface = g_AEFile_fileInterface;
    uint32_t result;

    if (fileInterface == 0) {
        result = 0;
    } else {
        uint16_t *text = AEStr_wchar(path);
        AELowLevelFile *file = 0;
        void *nativeHandle = 0;

        if (openType == OPEN_WRITE) {
            String localPath; AEStr_init(&localPath, text);
            nativeHandle = fileInterface->vtable->OpenWrite(fileInterface, localPath, path.size(), 0);
        } else if (openType == OPEN_APPEND) {
            String localPath; AEStr_init(&localPath, text);
            nativeHandle = fileInterface->vtable->OpenAppend(fileInterface, localPath, path.size(), 0, 0);
        } else if (openType == OPEN_READ) {
            String localPath; AEStr_init(&localPath, text);
            nativeHandle = fileInterface->vtable->OpenRead(fileInterface, localPath, path.size(), 0, 0, 0, 0);
            if (nativeHandle == 0) {
                if (*AEStr_index(path, 0) != '/') {
                    String prefix;  AEStr_init(&prefix, "/");
                    String oldPath; AEStr_init(&oldPath, path);
                    String newPath = prefix + oldPath;
                    path = newPath;
                    StringDestructor destruct = g_AEString_destructor;
                    destruct(&newPath);
                    destruct(&oldPath);
                    destruct(&prefix);
                }
                file = findPakFile(path);
                if (file == 0) {
                    result = 0;
                    goto done;
                }
            }
        }

        if (file == 0) {
            if (nativeHandle == 0) {
                result = 0;
                goto done;
            }
            AELowLevelNativeFile *nativeFile = new AELowLevelNativeFile;
            nativeFile->vtable = reinterpret_cast<AELowLevelFileVTable *>(g_AELowLevelNativeFile_vtable + 8);
            nativeFile->handle = nativeHandle;
            file = nativeFile;
        }

        Array<AELowLevelFile *> *files = g_AEFile_openFiles;
        uint32_t count = files->size();
        if (handle == 0) {
            if (count == 0) {
                ArrayAdd(file, *files);
            } else {
                AELowLevelFile **slot = files->data();
                if (*slot != 0) {
                    (*slot)->vtable->Close(*slot);
                    slot = files->data();
                }
                *slot = 0;
                files->data()[0] = file;
            }
        } else {
            for (uint32_t i = 1; i < count; i++) {
                if (files->data()[i] == 0) {
                    files->data()[i] = file;
                    *handle = i;
                    result = 1;
                    goto done;
                }
            }
            if (count == 0) {
                ArrayAdd(static_cast<AELowLevelFile *>(0), *files);
                files = g_AEFile_openFiles;
            }
            ArrayAdd(file, *files);
            *handle = g_AEFile_openFiles->size() - 1;
        }
        result = 1;
    }

done:
    return result;
}

// ---- Write_70ee8.cpp ----
__attribute__((minsize)) void AEFile::Write(uint8_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint8_t local = value;
    Write(1, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- FileDelete_711b8.cpp ----
extern FileInterface *gAEFileFileInterface;

__attribute__((minsize)) uint32_t AEFile::FileDelete(const String &path)
{
    void * volatile cookie = __stack_chk_guard;

    FileInterface *fileInterface = gAEFileFileInterface;
    uint32_t result = 0;
    if (fileInterface != 0) {
        String localPath; AEStr_init(&localPath, path);
        result = fileInterface->vtable->FileDelete(fileInterface, localPath);
    }

    uint32_t saved = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t current = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
    uint32_t guardDelta = current - saved;
    if (guardDelta == 0) {
        return result;
    }
    __stack_chk_fail(guardDelta);
}

// ---- collectPakFiles_70668.cpp ----
extern FileInterface *g_fileInterface;

__attribute__((minsize)) void AEFile::collectPakFiles(const String &path)
{
    FileInterface *fileInterface = g_fileInterface;
    if (fileInterface == 0 || fileInterface->enabled == 0) {
        return;
    }

    char *pathChars = AEStr_char(path);
    if (fileInterface->vtable->OpenDirectory(fileInterface, pathChars, 0) != 0) {
        String suffix; AEStr_init(&suffix, ".pak");
        String entry;
        while (fileInterface->vtable->ReadDirectory(fileInterface, entry) != 0) {
            String entryCopy; AEStr_init(&entryCopy, entry);
            if (AEStr_indexOf(entry, suffix) != 0xffffffffu) {
                collectFilesInPakFiles(entryCopy);
            }
        }
    }
    operator delete(pathChars);
    sortPakFileEntryList();
}

// ---- FileExist_710a8.cpp ----
extern FileInterface *gAEFileFileInterface;

__attribute__((minsize)) uint32_t AEFile::FileExist(const String &path)
{
    FileInterface *fileInterface = gAEFileFileInterface;
    if (fileInterface == 0) {
        return 0;
    }

    String nativePath; AEStr_init(&nativePath, path);
    uint32_t result = fileInterface->vtable->FileExist(fileInterface, nativePath);
    if (result != 0) {
        return 1;
    }

    String pakPath; AEStr_init(&pakPath, path);
    if (*AEStr_index(pakPath, 0) != '/') {
        String prefix;  AEStr_init(&prefix, "/");
        String pathCopy; AEStr_init(&pathCopy, path);
        pakPath = prefix + pathCopy;
    }

    AELowLevelFile *pakFile = findPakFile(pakPath);
    return pakFile != 0;
}

// ---- SetSaveDirectory_71288.cpp ----
extern FileInterface *gAEFileInterface;

__attribute__((minsize)) void AEFile::SetSaveDirectory(String path) {
    void * volatile cookie = __stack_chk_guard;

    FileInterface *fileInterface = gAEFileInterface;
    if (fileInterface != 0) {
        String savePath; AEStr_init(&savePath, path);
        fileInterface->vtable->SetSaveDirectory(fileInterface, savePath);
    }

    uint32_t saved = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t current = (uint32_t)(__UINTPTR_TYPE__)*(void *volatile *)&__stack_chk_guard;
    uint32_t guardDelta = current - saved;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- findPakFile_7075c.cpp ----
extern Array<AEPakFileEntry *> *g_pakFileEntries;
extern FileInterface *g_fileInterface;
extern char g_AELowLevelPakFile_vtable[];

__attribute__((stack_protect))
__attribute__((minsize)) AELowLevelFile *AEFile::findPakFile(const String &path)
{
    AELowLevelFile *result;
    Array<AEPakFileEntry *> **entriesGlobal = &g_pakFileEntries;

    if ((*entriesGlobal)->size() != 0) {
        uint32_t wantedCrc = crc32_ccitt(path);
        Array<AEPakFileEntry *> *entries = *entriesGlobal;
        int32_t low = 0;
        int32_t high = entries->size();

search:
        do {
            int32_t distance = high - low;
            int32_t half = distance >> 1;
            int32_t index = low + half;
            uint32_t entryCrc;

            if (high <= low) {
                goto not_found;
            }

            AEPakFileEntry *entry = entries->data()[index];
            entryCrc = entry->crc;

            if (entryCrc == wantedCrc) {
                uint16_t *name = AEStr_wchar(entry->name);
                AEPakFileEntry *foundEntry = (*entriesGlobal)->data()[index];
                FileInterface *fileInterface = g_fileInterface;
                void *handle;

                if (foundEntry->size == 0xffffffff) {
                    String entryName; AEStr_init(&entryName, name);
                    AEPakFileEntry *openEntry = (*entriesGlobal)->data()[index];
                    handle = fileInterface->vtable->OpenRead(fileInterface, entryName, openEntry->name.size(), 0, 0, 0, openEntry->offset);
                } else {
                    String entryName; AEStr_init(&entryName, name);
                    AEPakFileEntry *openEntry = (*entriesGlobal)->data()[index];
                    handle = fileInterface->vtable->OpenRead(fileInterface, entryName, openEntry->name.size(), 1, openEntry->packedSize, openEntry->size, openEntry->offset);
                }

                AELowLevelPakFile *pakFile = new AELowLevelPakFile;
                AEPakFileEntry *newEntry = (*entriesGlobal)->data()[index];
                pakFile->vtable = (AELowLevelFileVTable *)(g_AELowLevelPakFile_vtable + 8);
                pakFile->handle = handle;
                pakFile->packedSize = newEntry->packedSize;
                pakFile->size = newEntry->size;
                pakFile->position = 0;
                result = pakFile;
                goto done;
            }

            if (wantedCrc <= entryCrc) {
                bool done = high == index;
                high = index;
                if (done) {
                    goto done_zero;
                }
                goto search;
            }

            low = index;
            if (half == 0) {
                goto not_found;
            }
        } while (true);
    }

not_found:
    result = 0;
    return result;

done_zero:
    result = 0;
done:
    return result;
}

// ---- Write_70f1c.cpp ----
__attribute__((minsize)) void AEFile::Write(int8_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    int8_t local = value;
    Write(1, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- sortPakFileEntryList_7035c.cpp ----
extern Array<AEPakFileEntry *> *g_AEFile_pakFiles;

__attribute__((minsize)) void AEFile::sortPakFileEntryList()
{
    Array<AEPakFileEntry *> **slot = &g_AEFile_pakFiles;
    int32_t count = (int32_t)(*slot)->size();
    if (count == 0) {
        return;
    }

    while (--count, count >= 0) {
        for (int32_t index = 0; count != index; index++) {
            AEPakFileEntry **entries = (*slot)->data();
            AEPakFileEntry *left = entries[index];
            AEPakFileEntry *right = entries[index + 1];
            if (right->crc < left->crc) {
                entries[index] = right;
                (*slot)->data()[index + 1] = left;
            }
        }
    }
}

// ---- Write_70dec.cpp ----
__attribute__((minsize)) void AEFile::Write(char value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    char local = value;
    Write(1, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Write_70e50.cpp ----
__attribute__((minsize)) void AEFile::Write(int32_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    int32_t local = value;
    Write(4, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- SetInterface_7019c.cpp ----
extern Array<AEPakFileEntry *> *g_AEFile_pakFiles;
extern Array<AELowLevelFile *> *g_AEFile_openFiles;
extern FileInterface *g_AEFile_fileInterface;
extern uint32_t g_AEFile_initialized;

__attribute__((minsize)) void AEFile::SetInterface(FileInterface *fileInterface)
{
    if (fileInterface == 0) {
        return;
    }
    if (fileInterface->enabled == 0) {
        return;
    }

    if (g_AEFile_initialized != 0) {
        fileInterface->vtable->ResetSaveDirectory(fileInterface);
    }

    if (g_AEFile_pakFiles == 0) {
        g_AEFile_pakFiles = new Array<AEPakFileEntry *>();
    }
    if (g_AEFile_openFiles == 0) {
        g_AEFile_openFiles = new Array<AELowLevelFile *>();
    }
    g_AEFile_fileInterface = fileInterface;
}

// ---- Write_70eb4.cpp ----
__attribute__((minsize)) void AEFile::Write(int16_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    int16_t local = value;
    Write(2, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Write_70e80.cpp ----
__attribute__((minsize)) void AEFile::Write(uint16_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint16_t local = value;
    Write(2, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- collectFilesInPakFiles_703bc.cpp ----
extern FileInterface *g_fileInterface;
extern Array<AEPakFileEntry *> *g_pakFileEntries;

__attribute__((minsize)) void AEFile::collectFilesInPakFiles(String &path)
{
    FileInterface *fileInterface = g_fileInterface;
    volatile __UINTPTR_TYPE__ cookie = reinterpret_cast<__UINTPTR_TYPE__>(__stack_chk_guard);

    if (fileInterface != 0 && fileInterface->enabled != 0) {
        AEFile::OpenRead(path, 0);

        int8_t nameLength;
        uint32_t readResult = AEFile::Read(1, &nameLength, 0);
        uint32_t (*readFunc)(uint32_t, void *, uint32_t) = AEFile::Read;
        Array<AEPakFileEntry *> **entriesGlobal = &g_pakFileEntries;
        uint32_t offset = 1;

        while ((readResult & 1) != 0) {
            int32_t length = nameLength;
            char *name = new char[length + 1];
            readFunc(length, name, 0);
            name[nameLength] = 0;

            uint32_t packedSize;
            uint32_t size;
            readFunc(4, &packedSize, 0);
            readFunc(4, &size, 0);

            AEPakFileEntry *entry = new AEPakFileEntry();
            String nameString; AEStr_init(&nameString, name);
            entry->crc = AEFile::crc32_ccitt(nameString);
            nameString.~String();

            entry->name = path;
            offset = offset + nameLength + 8;
            entry->offset = offset;
            entry->packedSize = packedSize;
            entry->size = size;
            ArrayAdd(entry, **entriesGlobal);
            delete[] name;

            uint32_t *skipped;
            if (size == 0xffffffff) {
                AEFile::Skip(packedSize, 0);
                skipped = &packedSize;
            } else {
                AEFile::Skip(size, 0);
                skipped = &size;
            }

            uint32_t skippedBytes = *skipped;
            readResult = readFunc(1, &nameLength, 0);
            offset = offset + skippedBytes + 1;
        }

        AEFile::Close(0);
    }

    if (cookie == reinterpret_cast<__UINTPTR_TYPE__>(__stack_chk_guard)) {
        return;
    }
    __stack_chk_fail();
}

// ---- crc32_ccitt_70570.cpp ----
namespace {

static const uint32_t crc32Table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

} // namespace

__attribute__((minsize)) uint32_t AEFile::crc32_ccitt(const String &text)
{
    const String &s = text;
    uint32_t crc = 0;
    const uint32_t *table = crc32Table;
    for (int index = 0; index < (int32_t)s.size(); index++) {
        uint16_t value = *AEStr_index(s, index);
        crc = table[(value ^ crc) & 0xff] ^ (crc >> 8);
    }
    return crc;
}

// ---- Write_70f50.cpp ----
__attribute__((minsize)) void AEFile::Write(int64_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    int64_t local = value;
    Write(8, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Write_70f80.cpp ----
__attribute__((minsize)) void AEFile::Write(bool value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    bool local = value;
    Write(1, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Write_70fb4.cpp ----
__attribute__((minsize)) void AEFile::Write(float value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    float local = value;
    Write(4, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

// ---- Write_70e20.cpp ----
__attribute__((minsize)) void AEFile::Write(uint32_t value, uint32_t handle)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t local = value;
    Write(4, &local, handle);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)cookie - (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}
