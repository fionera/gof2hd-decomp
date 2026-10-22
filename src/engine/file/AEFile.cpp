#include "engine/file/AEFile.h"
#include "game/core/String.h"
#include <new>
#include "engine/file/AEPakFile.h"
#include "engine/file/AENormalFile.h"

namespace {
    inline void AEStr_set(String &self, const char *s) { self.Set_char(s); }
    inline void AEStr_set(String &self, const uint16_t *s) { self.Set_wchar(s); }

    inline uint16_t *AEStr_wchar(const String &self) {
        return reinterpret_cast<uint16_t *>(const_cast<char16_t *>(self.text()));
    }

    inline char *AEStr_char(const String &self) { return const_cast<String &>(self).GetAEChar(); }

    inline uint16_t *AEStr_index(const String &self, int i) { return const_cast<String &>(self).index(i); }

    inline uint32_t AEStr_indexOf(const String &self, const String &needle) {
        return const_cast<String &>(self).IndexOf(needle);
    }
}

AEPakFile::AEPakFile(FileInterface *fileInterface, int sizeLimit, int baseOffset)
    : fileInterface(fileInterface),
      sizeLimit(static_cast<uint32_t>(sizeLimit)),
      baseOffset(static_cast<uint32_t>(baseOffset)),
      position(0) {
}

AEPakFile::~AEPakFile() {
    Release();
}

bool AEPakFile::Read(uint32_t bytes, void *buffer) { // lint: void_ptr virtual buffer param; Pv baked into mangled name
    // lint: void_ptr virtual buffer param; Pv baked into mangled name
    FileInterface *file;
    if (bytes != 0 && (file = fileInterface) != nullptr) {
        if (static_cast<int>(position + bytes) > static_cast<int>(sizeLimit)) {
            bytes = static_cast<uint32_t>(static_cast<int>(sizeLimit) - static_cast<int>(position));
        }
        if (bytes != 0) {
            position += bytes;
            return file->Read(bytes, buffer);
        }
    }
    return 0;
}

uint32_t AEPakFile::Skip(uint32_t bytes) {
    char *buffer = new char[bytes];
    Read(bytes, buffer);
    delete[] buffer;
    return 1;
}

uint32_t AEPakFile::GetFileSize() {
    return sizeLimit;
}

uint32_t AEPakFile::Release() {
    if (fileInterface != nullptr) {
        fileInterface->Close();
        if (fileInterface != nullptr) {
            delete fileInterface;
        }
        fileInterface = nullptr;
    }
    return 1;
}

AENormalFile::AENormalFile(FileInterface *file)
    : file(file) {
}

AENormalFile::~AENormalFile() {
    Release();
}

uint32_t AENormalFile::Release() {
    if (file != nullptr) {
        file->Close();
        if (file != nullptr) {
            delete file;
        }
    }
    file = nullptr;
    return 1;
}

bool AENormalFile::Read(uint32_t bytes, void *buffer) { // lint: void_ptr virtual buffer param; Pv baked into mangled name
    // lint: void_ptr virtual buffer param; Pv baked into mangled name
    if (file != nullptr) {
        return file->Read(bytes, buffer);
    }
    return 0;
}

uint32_t AENormalFile::Write(uint32_t bytes, void *buffer) { // lint: void_ptr virtual buffer param; Pv baked into mangled name
    // lint: void_ptr virtual buffer param; Pv baked into mangled name
    if (file != nullptr) {
        return file->Write(bytes, buffer);
    }
    return 0;
}

uint32_t AENormalFile::Skip(uint32_t bytes) {
    if (file != nullptr) {
        return file->Seek(bytes);
    }
    return 0;
}

uint32_t AENormalFile::GetFileSize() {
    if (file != nullptr) {
        return file->GetFileSize();
    }
    return 0;
}

FileInterface *OpenAppend(unsigned short * /*name*/, int /*size*/, bool /*append*/, unsigned int /*mode*/) {
    return nullptr;
}

void AEFile::SetInterface(FileInterface *fileInterface) {
    if (fileInterface == nullptr || fileInterface->enabled == 0) {
        return;
    }

    if (AEFile::appRoot != nullptr) {
        fileInterface->SetAppRootDir(AEFile::appRoot);
    }

    if (AEFile::pakFileEntryList == nullptr) {
        AEFile::pakFileEntryList = new Array<AEPakFileEntry *>();
    }
    if (AEFile::file == nullptr) {
        AEFile::file = new Array<AELowLevelFile *>();
    }
    AEFile::fileInterface = fileInterface;
}

void AEFile::Release() {
    if (AEFile::file != nullptr) {
        ArrayReleaseClasses(*AEFile::file);
        delete AEFile::file;
        AEFile::file = nullptr;
    }

    if (AEFile::pakFileEntryList != nullptr) {
        for (uint32_t i = 0; i < AEFile::pakFileEntryList->size(); i++) {
            AEPakFileEntry *entry = AEFile::pakFileEntryList->data()[i];
            if (entry != nullptr) {
                delete entry;
            }
            AEFile::pakFileEntryList->data()[i] = nullptr;
        }
        delete AEFile::pakFileEntryList;
        AEFile::pakFileEntryList = nullptr;
    }
}

uint32_t AEFile::Open(String &path, FileOpenType openType, uint32_t *handle) {
    if (AEFile::fileInterface == nullptr)
        return 0;

    unsigned short *text = (unsigned short *) path;
    AELowLevelFile *file = nullptr;
    FileInterface *nativeHandle;
    switch (openType) {
        case OPEN_READ: {
            nativeHandle = reinterpret_cast<FileInterface *>(
                    AEFile::fileInterface->OpenRead(String(text, false), path.size(), 0, 0, 0, 0));
            if (nativeHandle != nullptr) {
                file = new AENormalFile(nativeHandle);
            } else {
                // Not on disk: look it up in the registered pak files (paths there are rooted).
                if (*path[0] != '/')
                    path = String("/") + String(path);
                file = findPakFile(path);
                if (file == nullptr)
                    return 0;
            }
            break;
        }
        case OPEN_WRITE: {
            nativeHandle = reinterpret_cast<FileInterface *>(
                    AEFile::fileInterface->OpenWrite(String(text, false), path.size(), false, 0));
            if (nativeHandle == nullptr)
                return 0;
            file = new AENormalFile(nativeHandle);
            break;
        }
        case OPEN_APPEND: {
            nativeHandle = reinterpret_cast<FileInterface *>(
                    AEFile::fileInterface->OpenAppend(String(text, false), path.size(), false, 0));
            if (nativeHandle != nullptr)
                file = new AENormalFile(nativeHandle);
            else
                return 0;
            break;
        }
        default:
            break;
    }

    if (handle != nullptr) {
        uint32_t count = AEFile::file->size();
        for (uint32_t i = 1; i < count; i++) {
            if (AEFile::file->data()[i] == nullptr) {
                AEFile::file->data()[i] = file;
                *handle = i;
                return 1;
            }
        }
        if (count == 0)
            ArrayAdd(static_cast<AELowLevelFile *>(nullptr), *AEFile::file);
        ArrayAdd(file, *AEFile::file);
        *handle = AEFile::file->size() - 1;
    } else if (AEFile::file->size() == 0) {
        ArrayAdd(file, *AEFile::file);
    } else {
        if (AEFile::file->data()[0] != nullptr)
            delete AEFile::file->data()[0];
        AEFile::file->data()[0] = nullptr;
        AEFile::file->data()[0] = file;
    }
    return 1;
}

uint32_t AEFile::OpenRead(String &path, uint32_t *handle) {
    return Open(path, OPEN_READ, handle);
}

uint32_t AEFile::OpenRead(const char *path, uint32_t *handle) {
    String string(path);
    return OpenRead(string, handle);
}

uint32_t AEFile::OpenWrite(String &path, uint32_t *handle) {
    return Open(path, OPEN_WRITE, handle);
}

uint32_t AEFile::OpenWrite(const char *path, uint32_t *handle) {
    String string(path);
    return OpenWrite(string, handle);
}

uint32_t AEFile::OpenAppend(String &path, uint32_t *handle) {
    return Open(path, OPEN_APPEND, handle);
}

uint32_t AEFile::OpenAppend(const char *path, uint32_t *handle) {
    String string(path);
    return OpenAppend(string, handle);
}

void AEFile::Close(uint32_t handle) {
    if (AEFile::fileInterface == nullptr) {
        return;
    }
    if (handle >= AEFile::file->size()) {
        return;
    }
    AELowLevelFile *slot = AEFile::file->data()[handle];
    if (slot != nullptr) {
        delete slot;
    }
    AEFile::file->data()[handle] = nullptr;
}

bool AEFile::Read(uint32_t bytes, void *buffer, uint32_t handle) { // lint: void_ptr ABI signature (Pv mangled)
    // lint: void_ptr ABI signature (Pv mangled)
    if (AEFile::fileInterface != nullptr && handle < AEFile::file->size()) {
        AELowLevelFile *file = AEFile::file->data()[handle];
        if (file != nullptr) {
            return file->Read(bytes, buffer);
        }
    }
    return 0;
}

bool AEFile::Read(bool &value, uint32_t handle) { return Read(1, &value, handle); }
bool AEFile::Read(char &value, uint32_t handle) { return Read(1, &value, handle); }
bool AEFile::Read(int8_t &value, uint32_t handle) { return Read(1, &value, handle); }
bool AEFile::Read(uint8_t &value, uint32_t handle) { return Read(1, &value, handle); }
bool AEFile::Read(int16_t &value, uint32_t handle) { return Read(2, &value, handle); }
bool AEFile::Read(uint16_t &value, uint32_t handle) { return Read(2, &value, handle); }
bool AEFile::Read(int32_t &value, uint32_t handle) { return Read(4, &value, handle); }
bool AEFile::Read(uint32_t &value, uint32_t handle) { return Read(4, &value, handle); }
bool AEFile::Read(int64_t &value, uint32_t handle) { return Read(8, &value, handle); }
bool AEFile::Read(float &value, uint32_t handle) { return Read(4, &value, handle); }

uint32_t AEFile::Read(String &value, uint32_t handle, bool wide) {
    uint32_t length;
    uint32_t result = Read(4, &length, handle);
    if (wide) {
        if (result == 0) {
            return 0;
        }
        uint16_t *buffer = new uint16_t[length + 1];
        uint32_t ok = 0;
        if (Read(length << 1, buffer, handle) != 0) {
            buffer[length] = 0;
            value.Set(buffer);
            ok = 1;
        }
        delete[] buffer;
        return ok;
    }
    if (result == 0) {
        return 0;
    }
    char *buffer = new char[length + 1];
    uint32_t ok = 0;
    if (Read(length, buffer, handle) != 0) {
        buffer[length] = 0;
        value.Set(buffer);
        ok = 1;
    }
    delete[] buffer;
    return ok;
}

uint32_t AEFile::ReadSwitched(int16_t &value, uint32_t handle) {
    uint32_t result = Read(2, &value, handle);
    if (result != 0) {
        uint32_t v = (uint16_t) value;
        value = (int16_t)((uint16_t)((v << 0x18) >> 0x10) | (v >> 8));
    }
    return result;
}

uint32_t AEFile::ReadSwitched(uint16_t &value, uint32_t handle) {
    uint32_t result = Read(2, &value, handle);
    if (result != 0) {
        uint32_t v = value;
        value = (uint16_t)((v << 0x18) >> 0x10) | (v >> 8);
    }
    return result;
}

uint32_t AEFile::ReadSwitched(int32_t &value, uint32_t handle) {
    uint32_t result = Read(4, &value, handle);
    if (result != 0) {
        value = __builtin_bswap32(value);
    }
    return result;
}

uint32_t AEFile::ReadSwitched(String &value, uint32_t handle) {
    uint16_t length;

    if (ReadSwitched(length, handle) == 0) {
        return 0;
    }
    uint32_t bytes = length;
    char *buffer = new char[bytes + 1];
    uint32_t ok = 0;
    if (Read(bytes, buffer, handle) != 0) {
        buffer[length] = '\0';
        AEStr_set(value, buffer);
        ok = 1;
    }
    delete[] buffer;
    return ok;
}

uint32_t AEFile::Write(uint32_t bytes, void *buffer, uint32_t handle) { // lint: void_ptr ABI signature (Pv mangled)
    // lint: void_ptr ABI signature (Pv mangled)
    if (AEFile::fileInterface != nullptr && handle < AEFile::file->size()) {
        AELowLevelFile *file = AEFile::file->data()[handle];
        if (file != nullptr) {
            return file->Write(bytes, buffer);
        }
    }
    return 0;
}

uint32_t AEFile::Write(bool value, uint32_t handle) { return Write(1, &value, handle); }
uint32_t AEFile::Write(char value, uint32_t handle) { return Write(1, &value, handle); }
uint32_t AEFile::Write(int8_t value, uint32_t handle) { return Write(1, &value, handle); }
uint32_t AEFile::Write(uint8_t value, uint32_t handle) { return Write(1, &value, handle); }
uint32_t AEFile::Write(int16_t value, uint32_t handle) { return Write(2, &value, handle); }
uint32_t AEFile::Write(uint16_t value, uint32_t handle) { return Write(2, &value, handle); }
uint32_t AEFile::Write(int32_t value, uint32_t handle) { return Write(4, &value, handle); }
uint32_t AEFile::Write(uint32_t value, uint32_t handle) { return Write(4, &value, handle); }
uint32_t AEFile::Write(int64_t value, uint32_t handle) { return Write(8, &value, handle); }
uint32_t AEFile::Write(float value, uint32_t handle) { return Write(4, &value, handle); }

uint32_t AEFile::Write(const String &value, uint32_t handle, bool wide) {
    if (wide) {
        const unsigned short *text = value.GetAEWChar();
        uint32_t size = value.size();
        if (Write(4, &size, handle) == 0) {
            return 0;
        }
        return Write(value.size() << 1, const_cast<unsigned short *>(text), handle);
    }
    char *text = value.GetAEChar();
    uint32_t size = value.size();
    uint32_t result = 0;
    if (Write(4, &size, handle) != 0) {
        result = Write(value.size(), text, handle);
    }
    delete[] text;
    return result;
}

uint32_t AEFile::Skip(uint32_t bytes, uint32_t handle) {
    if (AEFile::fileInterface != nullptr) {
        Array<AELowLevelFile *> *files = AEFile::file;
        if (handle < files->size()) {
            AELowLevelFile *file = files->data()[handle];
            if (file != nullptr) {
                return file->Skip(bytes);
            }
        }
    }
    return 0;
}

uint32_t AEFile::GetFileSize(uint32_t handle) {
    if (AEFile::fileInterface != nullptr) {
        Array<AELowLevelFile *> *files = AEFile::file;
        if (handle < files->size()) {
            AELowLevelFile *file = files->data()[handle];
            if (file != nullptr) {
                return file->GetFileSize();
            }
        }
    }
    return 0;
}

void AEFile::RegisterPakFile(String path) {
    collectFilesInPakFiles(path);
    sortPakFileEntryList();
}

void AEFile::collectPakFiles(const String &path) {
    if (AEFile::fileInterface == nullptr || AEFile::fileInterface->enabled == 0) {
        return;
    }

    char *pathChars = path.GetAEChar();
    // FileEnumInit is declared uint32_t here, but the original returns bool;
    // call it through a bool-returning member pointer so the branch keeps the
    // original's 50/50 probability (and hence register allocation).
    typedef bool (FileInterface::*FileEnumInitFn)(char *, bool);
    FileEnumInitFn fileEnumInit = reinterpret_cast<FileEnumInitFn>(&FileInterface::FileEnumInit);
    if ((AEFile::fileInterface->*fileEnumInit)(pathChars, false)) {
        String entry;
        while (AEFile::fileInterface->FileGetNextEnum(entry) != 0) {
            String entryCopy(entry);
            if (entry.IndexOf(String(".aez")) != 0xffffffffu) {
                collectFilesInPakFiles(entryCopy);
            }
        }
    }
    delete[] pathChars;
    sortPakFileEntryList();
}

void AEFile::collectFilesInPakFiles(String &path) {
    if (AEFile::fileInterface == nullptr || AEFile::fileInterface->enabled == 0) {
        return;
    }

    OpenRead(path, nullptr);

    int8_t nameLength;
    uint32_t packedSize;
    uint32_t size;
    uint32_t offset = 1;
    uint32_t readResult = Read(1, &nameLength, 0);
    while ((readResult & 1) != 0) {
        char *name = new char[nameLength + 1];
        Read(nameLength, name, 0);
        name[nameLength] = 0;

        Read(4, &packedSize, 0);
        Read(4, &size, 0);

        AEPakFileEntry *entry = new AEPakFileEntry();
        entry->crc = crc32_ccitt(String(name));
        entry->name = path;
        offset = offset + nameLength + 8;
        entry->offset = offset;
        entry->size = size;
        entry->packedSize = packedSize;
        ArrayAdd(entry, *AEFile::pakFileEntryList);
        delete[] name;

        uint32_t skippedBytes;
        if (size == 0xffffffff) {
            Skip(packedSize, 0);
            skippedBytes = packedSize;
        } else {
            Skip(size, 0);
            skippedBytes = size;
        }

        readResult = Read(1, &nameLength, 0);
        offset = offset + skippedBytes + 1;
    }

    Close(0);
}

void AEFile::sortPakFileEntryList() {
    uint32_t count = AEFile::pakFileEntryList->size();
    if (count == 0)
        return;

    // Bubble sort by CRC so findPakFile can binary-search.
    for (uint32_t pass = count - 1; (int32_t) pass >= 0; pass--) {
        for (uint32_t index = 0; index != pass; index++) {
            AEPakFileEntry **pair = &AEFile::pakFileEntryList->data()[index];
            AEPakFileEntry *left = pair[0];
            AEPakFileEntry *right = pair[1];
            if (left->crc > right->crc) {
                AEFile::pakFileEntryList->data()[index] = right;
                (&AEFile::pakFileEntryList->data()[index])[1] = left;
            }
        }
    }
}

namespace {
    // The original re-reads the entry through the global list at every use.
    inline AEPakFileEntry *pakEntry(int32_t index) { return AEFile::pakFileEntryList->data()[index]; }
}

AELowLevelFile *AEFile::findPakFile(const String &path) {
    if (AEFile::pakFileEntryList->size() == 0)
        return nullptr;

    uint32_t wantedCrc = crc32_ccitt(path);
    int32_t low = 0;
    int32_t high = (int32_t) AEFile::pakFileEntryList->size();
    while (low < high) {
        int32_t half = (high - low) >> 1;
        int32_t mid = low + half;
        uint32_t entryCrc = AEFile::pakFileEntryList->data()[mid]->crc;
        if (entryCrc == wantedCrc) {
            unsigned short *name = (unsigned short *) AEFile::pakFileEntryList->data()[mid]->name;
            FileInterface *fileInterface = AEFile::fileInterface;
            FileInterface *handle;
            if (pakEntry(mid)->size != 0xffffffff) {
                handle = reinterpret_cast<FileInterface *>(fileInterface->OpenRead(
                        String(name, false), pakEntry(mid)->name.size(), 1, pakEntry(mid)->packedSize, pakEntry(mid)->size,
                        pakEntry(mid)->offset));
            } else {
                handle = reinterpret_cast<FileInterface *>(fileInterface->OpenRead(
                        String(name, false), pakEntry(mid)->name.size(), 0, 0, 0, pakEntry(mid)->offset));
            }
            return new AEPakFile(handle, (int) pakEntry(mid)->packedSize, (int) pakEntry(mid)->size);
        }
        if (entryCrc > wantedCrc) {
            if (high == mid)
                return nullptr;
            high = mid;
        } else if (entryCrc < wantedCrc) {
            if (half == 0)
                return nullptr;
            low = mid;
        }
    }
    return nullptr;
}

uint32_t AEFile::FileExist(const String &path) {
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface == nullptr) {
        return 0;
    }
    if (fileInterface->FileExist(path) != 0) {
        return 1;
    }

    String pakPath(path);
    if (*pakPath[0] != '/') {
        pakPath = String("/") + String(path);
    }
    return findPakFile(pakPath) != nullptr;
}

uint32_t AEFile::FileDelete(const String &path) {
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface == nullptr) {
        return 0;
    }
    return fileInterface->FileDelete(path);
}

uint32_t AEFile::GetDeviceFreeSpace() {
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface != nullptr) {
        return fileInterface->GetDeviceFreeSpace();
    }
    return 0;
}

const char *AEFile::GetAppRootDir() {
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface != nullptr) {
        return fileInterface->GetAppRootDir();
    }
    return nullptr;
}

void AEFile::SetAppRootDir(void *path) { // lint: void_ptr ABI signature (Pv mangled)
    // lint: void_ptr ABI signature (Pv mangled)
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface != nullptr) {
        fileInterface->SetAppRootDir(path);
    }
}

void AEFile::SetZipDirectory(void *path) { // lint: void_ptr ABI signature (Pv mangled)
    // lint: void_ptr ABI signature (Pv mangled)
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface != nullptr) {
        fileInterface->SetZipDirectory(path);
    }
}

void AEFile::SetSaveDirectory(String path) {
    FileInterface *fileInterface = AEFile::fileInterface;
    if (fileInterface != nullptr) {
        fileInterface->SetSaveDirectory(path);
    }
}

void AEFile::ResetSaveDirectory() {
    if (AEFile::fileInterface != nullptr) {
        AEFile::fileInterface->ResetSaveDirectory();
    }
}

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
}

uint32_t AEFile::crc32_ccitt(const String &text) {
    uint32_t crc = 0;
    for (int index = 0; index < text.length; index++)
        crc = (crc >> 8) ^ crc32Table[(crc ^ *text[index]) & 0xff];
    return crc;
}

FileInterface *AEFile::fileInterface;
Array<AEPakFileEntry *> *AEFile::pakFileEntryList;
Array<AELowLevelFile *> *AEFile::file;
char *AEFile::appRoot;

