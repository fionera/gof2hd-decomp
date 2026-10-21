#ifndef GOF2_LAYOUT_H
#define GOF2_LAYOUT_H

#include <cstdint>

#include "ChoiceWindow.h"
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "TouchButton.h"
#include "engine/render/PaintCanvas.h"
#include "Blk16.h"

class ChoiceWindow;
class TouchButton;

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

class Layout {
public:
    uint8_t choiceWindowOpen;

    uint8_t _pad_0x1[3];
    int field_0x4;

    int windowTopInset;

    int field_0xc_leftMargin;

    int field_0x10_rightMargin;

    int footerTextInset;

    int headerTitleY;

    int field_0x1c;

    int field_0x20_top;

    int field_0x24;

    int buttonInsetX;

    int field_0x2c_rowHeight;

    int field_0x30_rowHeight;

    int field_0x34;
    int field_0x38;
    int field_0x3c;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    int field_0x4c;
    int field_0x50;
    int field_0x54;
    int field_0x58;
    int field_0x5c;
    int field_0x60;
    int field_0x64;
    int field_0x68;   // lounge panel width
    int field_0x6c;   // lounge panel height

    int field_0x70_rowHeight;

    int field_0x74;
    int field_0x78;
    int field_0x7c;
    int field_0x80_touchMargin;
    int field_0x84;
    int field_0x88;   // StarMap ctor: hitRadius = layout->field_0x88
    int field_0x8c;
    float field_0x90;   // StarMap drag scale (copied raw into StarMap::dragScale, a float)
    int field_0x94;
    int field_0x98;
    int field_0x9c;
    int field_0xa0;
    int field_0xa4;
    int field_0xa8;
    int field_0xac;
    uint8_t _pad_0xb0[28];
    int field_0xcc;
    int centerYBase;
    int marqueeWidth;
    int progressBarYBase;
    int field_0xdc;
    float animSpeedScale;
    float driftSpeedDivisor;
    float ringRadiusScale;
    int ringNearMidThreshold;
    uint8_t _pad_0xf0[4];
    int ringMidFarThreshold;
    uint8_t _pad_0xf8[4];
    int hudInset;
    int oreLabelYOffset;
    int oreTextYOffset;
    int field_0x108;
    int field_0x10c_rowBaseY;
    int field_0x110_strip58;

    int field_0x114_strip5c;

    int field_0x118;
    int field_0x11c;
    int field_0x120;
    int field_0x124;
    int field_0x128;
    int field_0x12c;
    int field_0x130;
    int field_0x134;
    int field_0x138;
    int field_0x13c;
    int field_0x140;
    int field_0x144;
    int field_0x148;
    int field_0x14c;
    int field_0x150;
    int field_0x154;
    int field_0x158;
    int field_0x15c;
    int field_0x160;
    int field_0x164;
    int field_0x168;
    int field_0x16c;
    int field_0x170;
    int field_0x174;
    int field_0x178;
    int field_0x17c;
    int field_0x180;
    int field_0x184;
    int field_0x188;
    int field_0x18c;
    int field_0x190;
    int field_0x194;
    int field_0x198;
    int field_0x19c;
    int field_0x1a0;
    int field_0x1a4;
    int field_0x1a8;
    int field_0x1ac;
    int field_0x1b0;
    int field_0x1b4;
    int field_0x1b8;
    int field_0x1bc;
    int field_0x1c0;
    int field_0x1c4;
    int field_0x1c8;
    int field_0x1cc;
    int field_0x1d0;
    int field_0x1d4;
    int field_0x1d8;
    int field_0x1dc;
    float eventBannerScale; // 0x1e0, read as float by Hud::drawEventQueue
    int field_0x1e4;
    int field_0x1e8;
    int field_0x1ec;
    int field_0x1f0;
    int field_0x1f4;
    int field_0x1f8;
    int field_0x1fc;
    int field_0x200;
    int field_0x204;
    int field_0x208;
    int field_0x20c;
    int field_0x210;
    int field_0x214;
    int field_0x218;
    int orbitInfoTextX;     // 0x21c: HUD orbit-information text x, right of the faction logo
    int orbitInfoStationY;  // 0x220
    int orbitInfoSystemY;   // 0x224
    int orbitInfoSecurityY; // 0x228
    int quickMenuHeaderLift; // 0x22c: quick-menu header icon y lift
    int quickMenuIconGap;    // 0x230: gauge icon / label x spacing
    int field_0x234;

    Blk16 field_0x238_blk16;

    int field_0x248;
    int field_0x24c;
    int field_0x250;
    int field_0x254;
    int field_0x258;
    int field_0x25c;
    int field_0x260;
    int field_0x264;
    int field_0x268;
    int field_0x26c;
    int field_0x270;
    int field_0x274;
    int field_0x278;
    int field_0x27c;
    int field_0x280;

    uint8_t field_0x284_sliderSlot5Enabled;

    uint8_t field_0x285;
    uint8_t field_0x286;
    uint8_t _pad_0x287[1];
    int field_0x288;
    int field_0x28c;
    int field_0x290;
    int field_0x294_buttonWidth;
    int field_0x298_buttonYBias;
    int field_0x29c_buttonRowGap;
    int field_0x2a0_listRowGap;
    int field_0x2a4_metricA;
    int field_0x2a8_metricB;
    int field_0x2ac_metricC;
    int field_0x2b0;
    int field_0x2b4;
    int field_0x2b8;
    int field_0x2bc;
    int field_0x2c0;
    int field_0x2c4;
    int field_0x2c8;
    int field_0x2cc;
    uint8_t _pad_0x2d0[4];
    int field_0x2d4;
    int field_0x2d8;
    int windowX;
    int windowY;
    int windowWidth;
    int windowHeight;
    uint8_t rewardMessageActive;
    uint8_t rewardMessageFlag;
    uint8_t _pad_0x2ee[2];
    int backButtonWidth;
    int field_0x2f4;
    int field_0x2f8;
    int field_0x2fc;
    int field_0x300;
    int field_0x304;
    int choiceWindowTitleHeight;

    int field_0x30c;
    int field_0x310;
    int field_0x314;
    int field_0x318;

    int helpWindowX;

    int helpWindowY;

    unsigned bgPatternImage;

    int headerPatternImage;

    unsigned headerIconImage;

    int headerCapImage;
    unsigned footerImageLeft;
    unsigned footerFillImage;
    int footerImageRight;
    unsigned field_0x340;
    unsigned footerPatternImage;
    unsigned field_0x348;
    unsigned field_0x34c;
    unsigned field_0x350;
    unsigned field_0x354;
    unsigned field_0x358;
    unsigned field_0x35c;
    unsigned field_0x360;
    unsigned field_0x364;
    unsigned field_0x368;
    unsigned field_0x36c;
    unsigned field_0x370;
    int scrollBarImage;

    unsigned scrollBarFillImage;
    unsigned field_0x37c;
    unsigned field_0x380;
    int field_0x384;
    unsigned field_0x388;
    unsigned field_0x38c;
    unsigned field_0x390;
    int field_0x394;
    int tipBoxImage;
    unsigned field_0x39c;
    int field_0x3a0;

    unsigned rewardIconImage;
    int field_0x3a8;

    int textBaselineAdjust;

    unsigned drawColor;
    TouchButton *backButton;
    TouchButton *secondaryButton;
    TouchButton *helpButton;
    uint8_t helpPressedFlag;
    uint8_t _pad_0x3c1[3];
    ChoiceWindow *choiceWindow;
    Array<String *> *tipLines;
    uint8_t helpButtonEnabled;
    uint8_t _pad_0x3cd[3];
    int rewardMessageTimer;
    int rewardCredits;
    int field_0x3d8;
    uint8_t field_0x3dc;
    uint8_t _pad_0x3dd[3];
    int scrollBarInset;

    int scrollBarHandle;

    int rewardBoxWidth;

    int rewardBoxHeight;

    int rewardBoxX;

    int rewardBoxY;

    int rewardBoxY2;

    int footerButtonOffset;
    uint8_t fading;
    uint8_t fadeOut;
    uint8_t _pad_0x402[2];
    unsigned fadeColor;

    // Fade-state region at 0x408. Canonical members are the aligned fade fields;
    // the misaligned offset-shifted views field_0x409 (int @0x409) and
    // field_0x40d (int @0x40d) are accessed via byte-offset reinterpret_cast over
    // fadeProgress.
    int fadeProgress;     // 0x408
    int fadeDuration;     // 0x40c
    uint8_t fillScreen;   // 0x410

    Layout();

    ~Layout();

    bool OnTouchBegin(int x, int y);

    int OnTouchEnd(int x, int y);

    int OnTouchMove(int x, int y);

    void drawBG();

    void drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad);

    void drawBGPattern(unsigned img, int x, int y, int w, int h);

    void drawBox(int style, int x, int y, int w, int h, String text, unsigned char flags);

    void drawEmptyFooter(bool showBack);

    uint8_t drawFade();

    void drawFooter();

    void drawFooterNoBackButton();

    void drawFooterStation();

    void drawHeader();

    void drawHeader(String title);

    void drawHeader(String title, bool transition);

    void drawHelpWindow();

    void drawMissionRewardMessage(bool transition);

    void drawScrollBar(int x, int y, int trackH, int pos, int range);

    void drawTip();

    void enableFillScreen(bool v);

    String formatNumber(int n);

    String tagString(String in);

    String formatCredits(int n);

    int getFooterTransitionWidth();

    int getHelpButtonOffset();

    float getPulseValue(float speed);

    uint8_t helpPressed();

    void initHelpWindow(String text);

    void initTip();

    uint8_t isFading();

    void reload();

    void resetWindowDimensions();

    void setDrawColor(int color);

    void setWindowDimensions(int p1, int p2, int p3, int p4);

    void showMissionRewardMessage(int show, bool flag);

    void startFade(bool fadeOut, int color, int duration);

    void update(int dt);

    void drawWindow(String title, int x, int y, int w, int h);

    void drawWindow(String title);

    void drawWindow(String title, bool drawBG);

    void drawWindow(String title, int x, int y, int w, int h, bool drawBG);

    void drawMask();

    void drawMask(int x, int y, int w, int h);

    void drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h);

    void drawBox(int style, int x, int y, int w, int h, String text);

    void drawFooter(bool stationMode, bool showBack);
};

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(__builtin_offsetof(Layout, fadeProgress) == 0x408, "fadeProgress");
static_assert(__builtin_offsetof(Layout, fadeDuration) == 0x40c, "fadeDuration");
static_assert(__builtin_offsetof(Layout, fillScreen) == 0x410, "fillScreen");
#endif
#endif
