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

// The object passed to the constructor; only ever reached through HeldFile's slots.
struct FileInterface;

// Vtable of the held low-level file object. Only the slots AENormalFile actually
// forwards to are named; the rest are padding to preserve slot offsets.
struct HeldFile;

struct HeldFileVTable {
    void     (*slot_00)(HeldFile*);                            // 0x00
    void     (*Free)(HeldFile*);                               // 0x04
    void     (*slot_08)(HeldFile*);                            // 0x08
    void     (*slot_0c)(HeldFile*);                            // 0x0c
    void     (*slot_10)(HeldFile*);                            // 0x10
    uint32_t (*Read)(HeldFile*, uint32_t, void*);              // 0x14
    uint32_t (*Write)(HeldFile*, uint32_t, const void*);       // 0x18
    uint32_t (*Skip)(HeldFile*, uint32_t);                     // 0x1c
    uint32_t (*GetFileSize)(HeldFile*);                        // 0x20
    void     (*pad_24)(HeldFile*);                             // 0x24
    void     (*pad_28)(HeldFile*);                             // 0x28
    void     (*pad_2c)(HeldFile*);                             // 0x2c
    void     (*pad_30)(HeldFile*);                             // 0x30
    void     (*pad_34)(HeldFile*);                             // 0x34
    void     (*pad_38)(HeldFile*);                             // 0x38
    void     (*pad_3c)(HeldFile*);                             // 0x3c
    void     (*pad_40)(HeldFile*);                             // 0x40
    void     (*Discard)(HeldFile*);                            // 0x44
};

struct HeldFile {
    HeldFileVTable *vtable;
};

class AENormalFile {
public:
    explicit AENormalFile(FileInterface *file);
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
