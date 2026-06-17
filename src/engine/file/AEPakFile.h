#ifndef GOF2_AEPAKFILE_H
#define GOF2_AEPAKFILE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/file/AEFile.h"

// Galaxy on Fire 2 -- AEPakFile.
// A virtual file reader that exposes a windowed view (base offset + size limit) over
// a held FileInterface. The window tracks the current read position; Read clamps to
// the remaining bytes and forwards to the underlying interface, Skip drains bytes by
// reading them into a throwaway buffer. At runtime the held FileInterface is a
// FileInterfaceAndroid, defined in its own translation unit.

class AEPakFile {
public:
    FileInterface *fileInterface;
    int            sizeLimit;
    int            baseOffset;
    int            position;

    AEPakFile(FileInterface *file, int sizeLimit, int baseOffset);
    virtual ~AEPakFile();

    virtual uint32_t Release();
    // Read up to `bytes`, clamped to the remaining window, into `buffer`.
    virtual uint32_t Read(uint32_t bytes, void *buffer);
    // Drain `bytes` by reading them into a scratch buffer.
    uint32_t Skip(uint32_t bytes);
};

#endif
