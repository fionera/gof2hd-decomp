#include "class.h"

extern FileInterface *g_fileInterface;
extern Array<AEPakFileEntry *> *g_pakFileEntries;

__attribute__((minsize)) void AEFile::collectFilesInPakFiles(String &path)
{
    FileInterface *fileInterface = g_fileInterface;
    volatile uint32_t cookie = reinterpret_cast<uint32_t>(__stack_chk_guard);

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
            String nameString(name, false);
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

    if (cookie == reinterpret_cast<uint32_t>(__stack_chk_guard)) {
        return;
    }
    __stack_chk_fail();
}
