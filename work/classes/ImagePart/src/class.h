#ifndef WORK_CLASSES_IMAGEPART_SRC_CLASS_H
#define WORK_CLASSES_IMAGEPART_SRC_CLASS_H

// ImagePart — top-level class (only arg types are namespaced).
// Layout from ctor/draw decompiles:
//   +0x00  uint   id            (image2d handle)
//   +0x04  int    field_04      (flags; draw ORs with 1)
//   +0x08  int    pos_y
//   +0x0c  int    scale_x       (image width)
//   +0x10  int    scale_y       (image height)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);

struct ImagePart {
    // @portable-fields
    int id; // 0x0
    int f_4; // 0x4
    int pos_y; // 0x8
    int scale_x; // 0xc
    int scale_y; // 0x10
};

template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }

#endif
