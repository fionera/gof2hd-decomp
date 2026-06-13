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

class NewsTicker {
public:
    float scrollOffset;                    // +0x0   scroll offset
    int x;                      // +0x4   x
    int y;                      // +0x8   y
    int width;                      // +0xc   width
    int textWidth;                     // +0x10  text width
    String tickerText;                  // +0x14  ticker text
    uint8_t touched;                 // +0x28  touched flag
    int lastTouchX;                     // +0x2c  last touch x

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
