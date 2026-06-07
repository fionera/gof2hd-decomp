#ifndef WORK_CLASSES_CHOICEWINDOW_SRC_CLASS_H
#define WORK_CLASSES_CHOICEWINDOW_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct String;
struct TouchButton;
struct ScrollTouchWindow;

struct ChoiceWindow {
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
