#ifndef WORK_CLASSES_AENORMALFILE_SRC_CLASS_H
#define WORK_CLASSES_AENORMALFILE_SRC_CLASS_H

// Galaxy on Fire 2 -- AENormalFile (Android libgof2hdaa.so).
// PORTABLE form: real struct with named fields (no hardcoded byte offsets), so the same source
// compiles correctly for 32-bit (armv7, matching) AND 64-bit (native macOS) -- pointer fields are
// real pointers, so the layout adjusts automatically. AENormalFile is a thin AELowLevelFile-style
// wrapper that forwards Read/Write/Skip/GetFileSize/Release through a held FileInterface* vtable.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(__SIZE_TYPE__ size);   // size_t: 4 bytes on armv7, 8 on arm64 (portable)
void operator delete(void *ptr) noexcept;

struct FileInterface;
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
    void (*pad_24)(FI *), (*pad_28)(FI *), (*pad_2c)(FI *), (*pad_30)(FI *);
    void (*pad_34)(FI *), (*pad_38)(FI *), (*pad_3c)(FI *), (*pad_40)(FI *);
    void (*Discard)(FI *);                                 // 0x44
};
struct FI { FIVTable *vtable; };

// Real layout (was: vptr@0x0, held FileInterface*@0x4 via fi_held()).
struct AENormalFile {
    void *vptr;        // Itanium vptr (manually installed: &vtable + 8)
    FI   *held;        // the wrapped FileInterface

    AENormalFile(FileInterface *file);
    ~AENormalFile();
    uint32_t Read(uint32_t bytes, void *buffer);
    uint32_t Write(uint32_t bytes, const void *buffer);
    uint32_t Skip(uint32_t bytes);
    uint32_t GetFileSize();
    uint32_t Release();
};

__attribute__((visibility("hidden"))) extern void *g_AENormalFile_vtable;

#endif
