#ifndef WORK_CLASSES_CHOICEWINDOW_SRC_CLASS_H
#define WORK_CLASSES_CHOICEWINDOW_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct String;
struct TouchButton;
struct ScrollTouchWindow;

struct ChoiceWindow {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    void* f_10; // 0x10
    void* f_14; // 0x14
    void* f_18; // 0x18
    void* f_1c; // 0x1c
    unsigned char _pad_20[12];
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    int f_38; // 0x38
    unsigned char _pad_3c[4];
    int f_40; // 0x40
    unsigned char _pad_44[4];
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    unsigned char _pad_5c[520];
    int f_264; // 0x264
    int f_268; // 0x268
    unsigned char _pad_26c[12];
    int f_278; // 0x278
    unsigned char _pad_27c[64];
    int f_2bc; // 0x2bc

    ChoiceWindow();
    ~ChoiceWindow();

    void set(String const &text);
    void set(String const &text, bool flag);
    void set(String const &a, String const &b);
    void set(String const &a, String const &b, String const &c);
    void set(String const &title, String const &message, bool flag);
    void set(String const &title, String const &message, bool flag,
             String const &left, String const &center, String const &right,
             int leftValue, int rightValue);
    void setWidth(int width);
    void setHeight(int height);
    void setMedal(int medal, int count);
    void setMiscButton(String const &text);
    void setMiscButton(bool enabled, String const &text, int value);
    void removeButtons();
    uint8_t hasChoice();
    void draw();
    void update(int dt);
    int OnTouchBegin(int x, int y);
    int OnTouchMove(int x, int y);
    int OnTouchEnd(int x, int y);
};

template <class T>
static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
