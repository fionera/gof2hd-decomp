#ifndef GOF2_AELOWLEVELHELDFILE_H
#define GOF2_AELOWLEVELHELDFILE_H
#include <cstdint>
#include "game/core/String.h"
#include "engine/core/AEString.h"

class AELowLevelHeldFile {
public:
    virtual ~AELowLevelHeldFile() {
    }

    virtual void *OpenRead(const String &, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) { return nullptr; } // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    virtual void *OpenWrite(const String &, uint32_t, uint32_t) { return nullptr; } // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    virtual void *OpenAppend(const String &, uint32_t, uint32_t) { return nullptr; } // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    virtual uint32_t Read(uint32_t bytes, void *buffer) { return 0; } // lint: void_ptr virtual file-handle return; type baked into vtable/override covariance
    // lint: void_ptr virtual buffer param; Pv baked into mangled name
    virtual uint32_t Write(uint32_t bytes, const void *buffer) { return 0; } // lint: void_ptr virtual buffer param; Pv baked into mangled name
    // lint: void_ptr virtual buffer param; PKv baked into mangled name
    virtual uint32_t Skip(uint32_t bytes) { return 0; }
    virtual uint32_t GetFileSize() { return 0; }
};
#endif
