#ifndef GOF2_NEWSTICKER_H
#define GOF2_NEWSTICKER_H
#include "common.h"

// Galaxy on Fire 2 -- the scrolling station news banner. Builds a single concatenated
// String of localized ticker lines and animates it horizontally across the HUD.
class NewsTicker {
public:
    float scrollOffset;     // current horizontal scroll position
    int x;                  // banner origin x
    int y;                  // banner origin y
    int width;              // banner width
    int textWidth;          // rendered width of the full ticker string
    String tickerText;      // concatenated, localized ticker text
    uint8_t touched;        // set while the banner is being manually dragged
    int lastTouchX;         // x of the previous touch sample (for drag delta)

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
