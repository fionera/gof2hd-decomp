#include "engine/file/AEPakFile.h"

AEPakFile::AEPakFile(PakHeldFile *file, int sizeLimit, int baseOffset)
    : fileInterface(file)
    , sizeLimit(sizeLimit)
    , baseOffset(baseOffset)
    , position(0)
{
}

AEPakFile::~AEPakFile()
{
    Release();
}

uint32_t AEPakFile::Release()
{
    PakHeldFile *h = fileInterface;
    if (h != nullptr) {
        h->Discard();
        PakHeldFile *h2 = fileInterface;
        if (h2 != nullptr) {
            delete h2;
        }
        fileInterface = nullptr;
    }
    return 1;
}

uint32_t AEPakFile::Read(uint32_t bytes, void *buffer)
{
    PakHeldFile *h;
    if (bytes != 0 && (h = fileInterface) != nullptr) {
        if ((int)(position + bytes) > sizeLimit) {
            bytes = (uint32_t)(sizeLimit - position);
        }
        if (bytes != 0) {
            position += bytes;
            return h->Read(bytes, buffer);
        }
    }
    return 0;
}

uint32_t AEPakFile::Skip(uint32_t bytes)
{
    char *buffer = new char[bytes];
    Read(bytes, buffer);
    delete[] buffer;
    return 1;
}
