#ifndef WORK_CLASSES_AEPAKFILE_SRC_CLASS_H
#define WORK_CLASSES_AEPAKFILE_SRC_CLASS_H

// Galaxy on Fire 2 -- AEPakFile (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "AEPakFile::..." (NOT in a namespace).
// AEPakFile wraps a FileInterface* (held at +0x4) plus an offset/size window
// (+0x8 size limit, +0xc base offset, +0x10 current position). Fields are accessed
// by byte offset taken from each work-item's target disasm.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;
void *operator new[](uint32_t size);
void operator delete[](void *ptr) noexcept;

struct FileInterface;

struct AEPakFile {
    AEPakFile(FileInterface *file, int param_2, int param_3);
    ~AEPakFile();

    uint32_t Read(uint32_t bytes, void *buffer);
    uint32_t Skip(uint32_t bytes);
    uint32_t Release();
};

// ---- held FileInterface object ------------------------------------------------
struct FI;

struct FIVTable {
    void (*slot_00)(FI *);                                 // 0x00
    void (*Free)(FI *);                                    // 0x04
    void (*slot_08)(FI *);                                 // 0x08
    void (*slot_0c)(FI *);                                 // 0x0c
    void (*slot_10)(FI *);                                 // 0x10
    uint32_t (*Read)(FI *, uint32_t, void *);              // 0x14
    void (*slot_18)(FI *);                                 // 0x18
    void (*slot_1c)(FI *);                                 // 0x1c
    void (*slot_20)(FI *);                                 // 0x20
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

// ---- AEPakFile's own vtable (for the self-call in Skip at slot 0xc) -----------
struct PakVTable {
    void (*slot_00)(AEPakFile *);                          // 0x00
    void (*slot_04)(AEPakFile *);                          // 0x04
    void (*slot_08)(AEPakFile *);                          // 0x08
    uint32_t (*ReadInto)(AEPakFile *, uint32_t, void *);   // 0x0c
};

struct PakObj {
    PakVTable *vtable;
};

static inline FI *&pak_fi(void *self)      { return *(FI **)((char *)self + 0x4); }
static inline int &pak_size(void *self)    { return *(int *)((char *)self + 0x8); }
static inline int &pak_pos(void *self)     { return *(int *)((char *)self + 0x10); }

// ---- tiny offset-cast helpers -------------------------------------------------
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
static inline int &I(void *p, int off)   { return *(int *)((char *)p + off); }

// AEPakFile's own vtable, referenced PC-relative (hidden visibility), offset by +8
// past the RTTI header when installed.
__attribute__((visibility("hidden"))) extern void *g_AEPakFile_vtable;

#endif
