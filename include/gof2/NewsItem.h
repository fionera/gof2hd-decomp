#ifndef GOF2_NEWSITEM_H
#define GOF2_NEWSITEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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


// POD-ish ticker entry; 0x1c bytes.
struct NewsItem {
    int   field_0x00;  // id (param_1)
    byte  field_0x04;  // flag (param_2)
    void *field_0x08;  // data (byte buffer, new[]/delete[])
    int   field_0x0c;  // length (param_4)
    int   field_0x10;  // (param_5)
    int   field_0x14;  // (param_6)
    byte  field_0x18;  // (= 0)

    NewsItem(int p1, bool p2, void *p3, int p4, int p5, int p6);
    ~NewsItem();
    void *clone();
};
#endif
