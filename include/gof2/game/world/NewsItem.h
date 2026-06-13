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
class NewsItem {
public:
    int   id;          // +0x00  ticker entry id (param_1)
    byte  flag;        // +0x04  bool flag (param_2)
    void *data;        // +0x08  byte buffer (new[]/delete[], param_3)
    int   length;      // +0x0c  buffer length in bytes (param_4)
    int   field_0x10;  // +0x10  ticker file int (param_5; semantics not in binary)
    int   field_0x14;  // +0x14  ticker file int (param_6; semantics not in binary)
    byte  field_0x18;  // +0x18  init-only flag (= 0; semantics not in binary)

    NewsItem(int p1, bool p2, void *p3, int p4, int p5, int p6);
    ~NewsItem();
    void *clone();
};
#endif
