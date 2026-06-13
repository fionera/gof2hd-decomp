#include "gof2/AENormalFile.h"


AENormalFile *_ZN12AENormalFileD2Ev(AENormalFile *self);
extern "C" uint32_t _ZN12AENormalFile7ReleaseEv(AENormalFile *self);

// ---- Read_70162.cpp ----
uint32_t AENormalFile::Read(uint32_t bytes, void *buffer)
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->Read(h, bytes, buffer);
    }
    return 0;
}

// ---- _AENormalFile_70130.cpp ----
void _ZN12AENormalFileD0Ev(AENormalFile *self)
{
    ((AENormalFile *)(_ZN12AENormalFileD2Ev(self)))->deleteTail();
}

// Deleting-destructor tail: the D2 body has already run, so just free the object.
void AENormalFile::deleteTail()
{
    ::operator delete(this);
}

// ---- AENormalFile_700f8.cpp ----
AENormalFile::AENormalFile(FileInterface *file)
{
    void *vtable = g_AENormalFile_vtable;
    P(this, 0) = (char *)vtable + 8;
    fi_held(this) = (FI *)file;
}

// ---- Release_70140.cpp ----
uint32_t AENormalFile::Release()
{
    FI *h = fi_held(this);
    if (h != 0) {
        h->vtable->Discard(h);
        FI *h2 = fi_held(this);
        if (h2 != 0) {
            h2->vtable->Free(h2);
        }
    }
    fi_held(this) = 0;
    return 1;
}

// ---- Skip_7017e.cpp ----
uint32_t AENormalFile::Skip(uint32_t bytes)
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->Skip(h, bytes);
    }
    return 0;
}

// ---- GetFileSize_7018c.cpp ----
uint32_t AENormalFile::GetFileSize()
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->GetFileSize(h);
    }
    return 0;
}

// ---- Write_70170.cpp ----
uint32_t AENormalFile::Write(uint32_t bytes, const void *buffer)
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->Write(h, bytes, buffer);
    }
    return 0;
}

// ---- _AENormalFile_7010c.cpp ----
__attribute__((visibility("hidden"))) extern void *g_AENormalFile_vtable;


AENormalFile *_ZN12AENormalFileD2Ev(AENormalFile *self)
{
    P(self, 0) = (char *)g_AENormalFile_vtable + 8;
    _ZN12AENormalFile7ReleaseEv(self);
    return self;
}

// ---- ~AENormalFile (base destructor) -----------------------------------------
// Re-installs the class vtable then discards/frees the held FileInterface via
// Release(). Matches the recovered base-destructor body at 0x8010c.
AENormalFile::~AENormalFile()
{
    P(this, 0) = (char *)g_AENormalFile_vtable + 8;
    Release();
}
