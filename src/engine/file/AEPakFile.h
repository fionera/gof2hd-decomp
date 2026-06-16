#ifndef GOF2_AEPAKFILE_H
#define GOF2_AEPAKFILE_H
#include "common.h"

// Galaxy on Fire 2 -- AEPakFile.
// A virtual file reader that exposes a windowed view (base offset + size limit) over
// a held FileInterface. The window tracks the current read position; Read clamps to
// the remaining bytes and forwards to the underlying interface, Skip drains bytes by
// reading them into a throwaway buffer.

// The held file interface. It has no dedicated header of its own, so the small vtable
// it is accessed through is described here. Only the Read/Free/Discard slots are used.
struct FileInterface;

struct FileInterfaceVTable {
    void     (*slot_00)(FileInterface *);
    void     (*Free)(FileInterface *);
    void     (*slot_08)(FileInterface *);
    void     (*slot_0c)(FileInterface *);
    void     (*slot_10)(FileInterface *);
    uint32_t (*Read)(FileInterface *, uint32_t, void *);
    void     (*slot_18)(FileInterface *);
    void     (*slot_1c)(FileInterface *);
    void     (*slot_20)(FileInterface *);
    void     (*pad_24)(FileInterface *);
    void     (*pad_28)(FileInterface *);
    void     (*pad_2c)(FileInterface *);
    void     (*pad_30)(FileInterface *);
    void     (*pad_34)(FileInterface *);
    void     (*pad_38)(FileInterface *);
    void     (*pad_3c)(FileInterface *);
    void     (*pad_40)(FileInterface *);
    void     (*Discard)(FileInterface *);
};

struct FileInterface {
    FileInterfaceVTable *vtable;
};

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
