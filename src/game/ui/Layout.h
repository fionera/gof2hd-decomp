#ifndef GOF2_LAYOUT_H
#define GOF2_LAYOUT_H
// Galaxy on Fire 2 — Layout: the shared menu/window chrome (header bar, footer,
// background fill, fade overlay, scroll bar, mission-reward popup and the three
// navigation buttons) used by every full-screen game screen.

#include <cstdint>
#include "common.h"   // String, Array<T>

class TouchButton;
class ChoiceWindow;
class PaintCanvas;

class Layout {
public:
    uint8_t choiceWindowOpen;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    int footerTextInset;                // footer text bottom inset
    int headerTitleY;                   // header title baseline Y
    int field_0x1c;
    int field_0x20;
    int field_0x24;
    int buttonInsetX;                   // back/secondary button left inset
    int field_0x2c;
    int field_0x30;
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
    int field_0x70;
    int field_0x84;
    int field_0x98;
    int field_0x9c;
    int field_0xa0;
    int field_0xa4;
    int field_0xa8;
    int field_0xac;
    int field_0xcc;
    int field_0x114;
    int field_0x1f0;
    int field_0x1f4;
    int field_0x1f8;
    int field_0x200;
    int field_0x238;
    int field_0x264;
    int field_0x278;
    int field_0x27c;
    int field_0x280;
    uint8_t field_0x284;
    uint8_t field_0x285;
    uint8_t field_0x286;
    int field_0x288;
    int field_0x28c;
    int field_0x290;
    int field_0x2bc;
    int field_0x2cc;
    int field_0x2d4;
    int field_0x2d8;
    int windowX;
    int windowY;
    int windowWidth;
    int windowHeight;
    uint8_t rewardMessageActive;
    uint8_t rewardMessageFlag;
    int backButtonWidth;
    int field_0x2f4;
    int field_0x2f8;
    int field_0x2fc;
    int field_0x300;
    int field_0x304;
    int field_0x308;
    int field_0x30c;
    int field_0x310;
    int field_0x314;
    int field_0x318;
    int field_0x31c;
    int field_0x320;
    unsigned bgPatternImage;
    unsigned headerPatternImage;        // header bar fill pattern
    unsigned headerIconImage;           // header/title icon image
    unsigned headerCapImage;            // header end-cap image
    unsigned footerImageLeft;
    unsigned field_0x338;
    int footerImageRight;
    unsigned field_0x340;               // footer image; loaded in reload
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
    unsigned field_0x378;
    unsigned field_0x37c;               // image; loaded in reload
    unsigned field_0x380;
    int field_0x384;
    unsigned field_0x388;
    unsigned field_0x38c;
    unsigned field_0x390;
    int field_0x394;
    int tipBoxImage;
    unsigned field_0x39c;
    int field_0x3a0;
    int field_0x3a4;
    int field_0x3a8;
    int textBaselineAdjust;             // text vertical centring adjust = th/2-1
    unsigned drawColor;
    TouchButton* backButton;
    TouchButton* secondaryButton;
    TouchButton* helpButton;
    uint8_t helpPressedFlag;
    ChoiceWindow* choiceWindow;
    Array<String*>* tipLines;           // tip line-array; size() == line count
    uint8_t helpButtonEnabled;
    int rewardMessageTimer;
    int rewardCredits;
    int field_0x3d8;
    uint8_t field_0x3dc;
    int scrollBarInset;                 // scrollbar track inset
    int scrollBarHandle;                // scrollbar thumb handle offset
    int rewardBoxWidth;                 // mission-reward box width
    int rewardBoxHeight;                // mission-reward box height
    int rewardBoxX;                     // mission-reward box X
    int rewardBoxY;                     // mission-reward box Y
    int rewardBoxY2;                    // mission-reward second line Y
    int footerButtonOffset;             // footer back-button bottom offset
    uint8_t fading;
    uint8_t fadeOut;
    uint32_t fadeColor;
    int fadeProgress;
    int field_0x409;
    int fadeDuration;
    int field_0x40d;
    uint8_t fillScreen;

    Layout();
    ~Layout();

    // ---- methods ----
    int OnTouchBegin(int x, int y);
    int OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void drawBG();
    void drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad);
    void drawBGPattern(unsigned img, int x, int y, int w, int h);
    void drawBox(int style, int x, int y, int w, int h, String *text, unsigned flags);
    void drawBox6(int p2, int p3, int p4, int p5, int p6, const String *str);
    void drawEmptyFooter(int showBack);
    uint8_t drawFade();
    void drawFooter();
    void drawFooterImpl(int stationMode, int showBack);
    void drawFooterNoBackButton();
    void drawFooterStation();
    void drawHeader0();
    void drawHeader1(const String *param);
    void drawHeader7(String *title, int transition);
    void drawHelpWindow();
    int drawMask4(int p1, int p2, int p3, int p4);
    void drawMissionRewardMessage(int transition);
    void drawScrollBar(int x, int y, int trackH, int pos, int range);
    void drawTip();
    void drawWindow1(const String *param);
    void drawWindow2(const String *param, int flag);
    void drawWindow5(const String *param, int p3, int p4, int p5, int p6);
    void drawWindow7(String *title, int x, int y, int w, int h, int drawBG);
    void enableFillScreen(bool v);
    int getFooterTransitionWidth();
    int getHelpButtonOffset();
    float getPulseValue(float speed);
    uint8_t helpPressed();
    void initHelpWindow(String *text);
    void initTip();
    uint8_t isFading();
    void reload();
    void resetWindowDimensions();
    void setDrawColor(int color);
    void setWindowDimensions(int p1, int p2, int p3, int p4);
    void showMissionRewardMessage(int show, bool flag);
    void startFade(uint8_t fadeOut, int color, int duration);
    void update(int dt);

    // ---- internal helpers ----
    void drawWindowImpl(const String *str, int flag);
    void drawWindowImpl5(String *str, int x, int y, int w, int h, int flag);
    void drawHeaderImpl(String *title, int transition);
    void drawBoxImpl(int style, int x, int y, int w, int h, String *text, int flags);
    void drawBoxStr(int style, int x, int y, int w, int h, String *text);
    void drawBox8(int kind, int x, int y, int w, int color, String *text, int z);
    void drawMaskImpl(int x, int y, int w, int h);
    void drawMaskTail(PaintCanvas *pc, int savedColor);
    void drawHelpWindowImpl();
    void drawBGBorder6(unsigned corner, unsigned edge, int x, int y, int w);
    void drawBGBorder8(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad);
    void setBtnRect(TouchButton *btn, int x, int y, int anchor);
    void loadImage(PaintCanvas *canvas, int imageId, void *field);
    void headerAnim(TouchButton *btn);
    void initConstBlock(int hd, int wide, int scale, int res);
    int  dispatchTouchBegin(TouchButton *btn, int x, int y);
    int  dispatchTouchMove(TouchButton *btn, int x, int y);
    int  dispatchTouchEnd(TouchButton *btn, int x, int y);
    int  touch_end(int x, int y);
    void drawHeader_call(String *title);

    // ---- canonical-named public draw entries ----
    void drawWindow(String *title, int x, int y, int w, int h);
    void drawMask();
    void drawBGBorderImpl(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad);
};
#endif
