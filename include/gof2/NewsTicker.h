#ifndef GOF2_NEWSTICKER_H
#define GOF2_NEWSTICKER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
struct String;
}
typedef AbyssEngine::String String;



namespace AbyssEngine {


String operator+(const String &left, const String &right);
}

struct NewsTicker {
    float field_0x0;                    // +0x0   scroll offset
    int field_0x4;                      // +0x4   x
    int field_0x8;                      // +0x8   y
    int field_0xc;                      // +0xc   width
    int field_0x10;                     // +0x10  text width
    String field_0x14;                  // +0x14  ticker text
    uint8_t field_0x28;                 // +0x28  touched flag
    int field_0x2c;                     // +0x2c  last touch x

    NewsTicker(int x, int y, int width, int faction, int level);
    ~NewsTicker();
    void draw();
    void update(int dt);
    int getHeight();
    uint8_t OnTouchBegin(int x, int y);
    bool OnTouchMove(int x, int unused);
    uint32_t OnTouchEnd(int x, int unused);
    String replaceTokens(String text);
};
#endif
