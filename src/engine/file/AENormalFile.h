#ifndef GOF2_AENORMALFILE_H
#define GOF2_AENORMALFILE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- AENormalFile.
//
// A thin wrapper around a held low-level file object. Read/Write/Skip/GetFileSize
// forward through the held object's own vtable; Release discards and frees it.
//
// The held object is reached only through a fixed set of vtable slots, so it is
// modelled here as a minimal call-through interface (HeldFile) rather than the full
// FileInterface type.

// The held low-level file object, reached only through a fixed set of virtual slots. Modelled as
// a minimal call-through interface; the virtual destructor stands in for the engine's Free slot
// (+0x04) and Discard is the engine's close slot (+0x44). At runtime the held object is a
// FileInterfaceAndroid, defined in its own translation unit.
class HeldFile {
public:
    virtual ~HeldFile() {}                                     // +0x04 Free
    virtual uint32_t Read(uint32_t bytes, void *buffer)        { return 0; }   // +0x14
    virtual uint32_t Write(uint32_t bytes, const void *buffer) { return 0; }   // +0x18
    virtual uint32_t Skip(uint32_t bytes)                      { return 0; }   // +0x1c
    virtual uint32_t GetFileSize()                             { return 0; }   // +0x20
    virtual void     Discard()                                 {}              // +0x44
};

class AENormalFile {
public:
    explicit AENormalFile(HeldFile *file);
    virtual ~AENormalFile();

    virtual uint32_t Read(uint32_t bytes, void *buffer);
    virtual uint32_t Write(uint32_t bytes, const void *buffer);
    virtual uint32_t Skip(uint32_t bytes);
    virtual uint32_t GetFileSize();
    virtual uint32_t Release();

private:
    HeldFile *held;
};

#endif
