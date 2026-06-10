#ifndef WORK_CLASSES_NEWSITEM_SRC_CLASS_H
#define WORK_CLASSES_NEWSITEM_SRC_CLASS_H

// Galaxy on Fire 2 -- NewsItem class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. POD-ish ticker entry; 0x1c bytes. Field offsets recovered from the
// per-method target disassembly; accessed via byte-offset casts from `this`.
//
//   0x00  int    id          (param_1)
//   0x04  bool   flag        (param_2)
//   0x08  void*  data        (param_3 -- byte buffer, new[]/delete[])
//   0x0c  int    length      (param_4)
//   0x10  int    field_10    (param_5)
//   0x14  int    field_14    (param_6)
//   0x18  bool   field_18    (= 0)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// new(0x1c) scalar, and new[]/delete[] for the byte buffer.
void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
