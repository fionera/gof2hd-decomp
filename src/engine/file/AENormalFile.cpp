#include "gof2/engine/file/AENormalFile.h"

AENormalFile::AENormalFile(FileInterface *file)
    : held(reinterpret_cast<HeldFile *>(file))
{
}

AENormalFile::~AENormalFile()
{
    Release();
}

uint32_t AENormalFile::Read(uint32_t bytes, void *buffer)
{
    if (held != nullptr) {
        return held->vtable->Read(held, bytes, buffer);
    }
    return 0;
}

uint32_t AENormalFile::Write(uint32_t bytes, const void *buffer)
{
    if (held != nullptr) {
        return held->vtable->Write(held, bytes, buffer);
    }
    return 0;
}

uint32_t AENormalFile::Skip(uint32_t bytes)
{
    if (held != nullptr) {
        return held->vtable->Skip(held, bytes);
    }
    return 0;
}

uint32_t AENormalFile::GetFileSize()
{
    if (held != nullptr) {
        return held->vtable->GetFileSize(held);
    }
    return 0;
}

uint32_t AENormalFile::Release()
{
    if (held != nullptr) {
        held->vtable->Discard(held);
        if (held != nullptr) {
            held->vtable->Free(held);
        }
    }
    held = nullptr;
    return 1;
}
