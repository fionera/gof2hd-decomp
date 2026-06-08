#ifndef WORK_CLASSES_AENORMALFILE_SRC_CLASS_H
#define WORK_CLASSES_AENORMALFILE_SRC_CLASS_H

// Galaxy on Fire 2 -- AENormalFile (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "AENormalFile::..." (NOT in a namespace).
// AENormalFile is a thin AELowLevelFile-style wrapper around a FileInterface* held
// at +0x4. Read/Write/Skip/GetFileSize forward through the held interface's vtable
// at fixed slot offsets (0x14/0x18/0x1c/0x20); Release uses 0x44 then 0x04.
// We do NOT model the full layout: fields are accessed by byte offset taken from
// each work-item's target disasm.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;

struct FileInterface;

struct AENormalFile {
    AENormalFile(FileInterface *file);
    ~AENormalFile();

    uint32_t Read(uint32_t bytes, void *buffer);
    uint32_t Write(uint32_t bytes, const void *buffer);
    uint32_t Skip(uint32_t bytes);
    uint32_t GetFileSize();
    uint32_t Release();
};

// ---- held FileInterface object ------------------------------------------------
// Forwarders tail-call its vtable slots passing the same args through.
struct FI;

struct FIVTable {
    void (*slot_00)(FI *);                                 // 0x00
    void (*Free)(FI *);                                    // 0x04
    void (*slot_08)(FI *);                                 // 0x08
    void (*slot_0c)(FI *);                                 // 0x0c
    void (*slot_10)(FI *);                                 // 0x10
    uint32_t (*Read)(FI *, uint32_t, void *);              // 0x14
    uint32_t (*Write)(FI *, uint32_t, const void *);       // 0x18
    uint32_t (*Skip)(FI *, uint32_t);                      // 0x1c
    uint32_t (*GetFileSize)(FI *);                         // 0x20
    void (*pad_24)(FI *);                                  // 0x24
    void (*pad_28)(FI *);                                  // 0x28
    void (*pad_2c)(FI *);                                  // 0x2c
    void (*pad_30)(FI *);                                  // 0x30
    void (*pad_34)(FI *);                                  // 0x34
    void (*pad_38)(FI *);                                  // 0x38
    void (*pad_3c)(FI *);                                  // 0x3c
    void (*pad_40)(FI *);                                  // 0x40
    void (*Discard)(FI *);                                 // 0x44
};

struct FI {
    FIVTable *vtable;
};

static inline FI *&fi_held(void *self) { return *(FI **)((char *)self + 0x4); }

// ---- tiny offset-cast helpers -------------------------------------------------
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

// AENormalFile's own vtable, referenced PC-relative (hidden visibility) and offset
// by +8 past the RTTI header when installed into the object.
__attribute__((visibility("hidden"))) extern void *g_AENormalFile_vtable;

#endif
