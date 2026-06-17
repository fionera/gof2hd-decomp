#include "engine/file/AENormalFile.h"

AENormalFile::AENormalFile(HeldFile *file)
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
        return held->Skip(bytes);
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
        held->Discard();
        if (held != nullptr) {
            delete held;
        }
    }
    held = nullptr;
    return 1;
}
