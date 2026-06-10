#include "gof2/AEPakFile.h"


extern "C" uint32_t _ZN9AEPakFile7ReleaseEv(AEPakFile *self);
extern "C" AEPakFile *_ZN9AEPakFileD2Ev(AEPakFile *self);
extern "C" void AEPakFile_deleteTail(AEPakFile *self);

// ---- _AEPakFile_70034.cpp ----
__attribute__((visibility("hidden"))) extern void *g_AEPakFile_vtable;


extern "C" AEPakFile *_ZN9AEPakFileD2Ev(AEPakFile *self)
{
    P(self, 0) = (char *)g_AEPakFile_vtable + 8;
    _ZN9AEPakFile7ReleaseEv(self);
    return self;
}

// ---- Release_700d4.cpp ----
uint32_t AEPakFile::Release()
{
    FI *h = pak_fi(this);
    if (h != 0) {
        h->vtable->Discard(h);
        FI *h2 = pak_fi(this);
        if (h2 != 0) {
            h2->vtable->Free(h2);
        }
        pak_fi(this) = 0;
    }
    return 1;
}

// ---- _AEPakFile_70058.cpp ----
extern "C" void _ZN9AEPakFileD0Ev(AEPakFile *self)
{
    AEPakFile_deleteTail(_ZN9AEPakFileD2Ev(self));
}

// ---- Read_70068.cpp ----
uint32_t AEPakFile::Read(uint32_t bytes, void *buffer)
{
    FI *h;
    if (bytes != 0 && (h = pak_fi(this)) != 0) {
        int pos = pak_pos(this);
        int size = pak_size(this);
        if ((int)(pos + bytes) > size) {
            bytes = (uint32_t)(size - pos);
        }
        if (bytes != 0) {
            pak_pos(this) = pos + bytes;
            return h->vtable->Read(h, bytes, buffer);
        }
    }
    return 0;
}

// ---- AEPakFile_70010.cpp ----
AEPakFile::AEPakFile(FileInterface *file, int param_2, int param_3)
{
    void *vtable = g_AEPakFile_vtable;
    P(this, 0x0) = (char *)vtable + 8;
    P(this, 0x4) = file;
    I(this, 0x8) = param_2;
    I(this, 0xc) = param_3;
    I(this, 0x10) = 0;
}

// ---- Skip_700a2.cpp ----
uint32_t AEPakFile::Skip(uint32_t bytes)
{
    void *buffer = operator new[](bytes);
    PakObj *self = (PakObj *)this;
    self->vtable->ReadInto(this, bytes, buffer);
    operator delete[](buffer);
    return 1;
}
