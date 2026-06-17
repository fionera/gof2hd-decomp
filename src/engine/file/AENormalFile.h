#ifndef GOF2_AENORMALFILE_H
#define GOF2_AENORMALFILE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/file/AEFile.h"

// Galaxy on Fire 2 -- AENormalFile.
//
// A thin wrapper around a held platform FileInterface. Read/Write/GetFileSize forward
// through the held object's own vtable; Skip reads-and-discards into a throwaway buffer;
// Release frees the held object. At runtime the held object is a FileInterfaceAndroid,
// defined in its own translation unit.

class AENormalFile {
public:
    explicit AENormalFile(FileInterface *file);
    virtual ~AENormalFile();

    virtual uint32_t Read(uint32_t bytes, void *buffer);
    virtual uint32_t Write(uint32_t bytes, void *buffer);
    virtual uint32_t Skip(uint32_t bytes);
    virtual uint32_t GetFileSize();
    virtual uint32_t Release();

private:
    FileInterface *held;
};

#endif
