#include "engine/file/AENormalFile.h"

AENormalFile::AENormalFile(FileInterface *file)
    : held(file)
{
}

AENormalFile::~AENormalFile()
{
    Release();
}

uint32_t AENormalFile::Read(uint32_t bytes, void *buffer)
{
    if (held != nullptr) {
        return held->Read(bytes, buffer);
    }
    return 0;
}

uint32_t AENormalFile::Write(uint32_t bytes, void *buffer)
{
    if (held != nullptr) {
        return held->Write(bytes, buffer);
    }
    return 0;
}

uint32_t AENormalFile::Skip(uint32_t bytes)
{
    if (held != nullptr) {
        char *buffer = new char[bytes];
        uint32_t read = held->Read(bytes, buffer);
        delete[] buffer;
        return read;
    }
    return 0;
}

uint32_t AENormalFile::GetFileSize()
{
    if (held != nullptr) {
        return held->GetFileSize();
    }
    return 0;
}

uint32_t AENormalFile::Release()
{
    if (held != nullptr) {
        delete held;
    }
    held = nullptr;
    return 1;
}
