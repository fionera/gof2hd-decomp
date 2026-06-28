#ifndef GOF2_AELOWLEVELFILE_H
#define GOF2_AELOWLEVELFILE_H
#include <cstdint>

#include "game/core/String.h"

#include "engine/core/AEString.h"

#include "engine/file/AELowLevelHeldFile.h"
using String = AbyssEngine::String;

class AELowLevelFile {
public:
    virtual ~AELowLevelFile() {
    }

    virtual uint32_t Write(uint32_t bytes, void *buffer) { return 0; }
    // lint: void_ptr virtual buffer param; Pv baked into mangled name
    virtual uint32_t Read(uint32_t bytes, void *buffer) { return 0; }
    // lint: void_ptr virtual buffer param; Pv baked into mangled name
    virtual uint32_t Skip(uint32_t bytes) { return 0; }
    virtual uint32_t Release() { return 1; }
    virtual uint32_t GetFileSize() { return 0; }
};

#endif
