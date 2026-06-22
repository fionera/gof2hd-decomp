#ifndef GOF2_AELOWLEVELFILE_H
#define GOF2_AELOWLEVELFILE_H
#include <cstdint>

#include "game/core/String.h"

using String = AbyssEngine::String;

// Abstract base for an open file handle. The two concrete implementations are
// AENormalFile (a plain file served by the platform FileInterface) and AEPakFile
// (a slice of a .pak archive). AEFile owns these polymorphically.
class AELowLevelFile {
public:
    virtual ~AELowLevelFile() {
    }

    virtual uint32_t Write(uint32_t bytes, void *buffer) { return 0; }
    virtual uint32_t Read(uint32_t bytes, void *buffer) { return 0; }
    virtual uint32_t Skip(uint32_t bytes) { return 0; }
    virtual uint32_t Release() { return 1; }
    virtual uint32_t GetFileSize() { return 0; }
};

class AELowLevelHeldFile {
public:
    virtual ~AELowLevelHeldFile() {
    }

    virtual void *OpenRead(const String &, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) { return nullptr; }
    virtual void *OpenWrite(const String &, uint32_t, uint32_t) { return nullptr; }
    virtual void *OpenAppend(const String &, uint32_t, uint32_t) { return nullptr; }
    virtual uint32_t Read(uint32_t bytes, void *buffer) { return 0; }
    virtual uint32_t Write(uint32_t bytes, const void *buffer) { return 0; }
    virtual uint32_t Skip(uint32_t bytes) { return 0; }
    virtual uint32_t GetFileSize() { return 0; }
};
#endif
