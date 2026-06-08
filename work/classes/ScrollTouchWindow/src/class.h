// ScrollTouchWindow is a top-level class. Keep the object opaque and access
// fields only through offsets from the work-items.
#ifndef WORK_CLASSES_SCROLLTOUCHWINDOW_SRC_CLASS_H
#define WORK_CLASSES_SCROLLTOUCHWINDOW_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct String {
    char bytes[12];

    String();
    String(const String &other, bool copy);
    String(const char *text, bool copy);
    ~String();
    String &operator=(const String &other);
};

}

struct ScrollTouchWindow {
    // @portable-fields
    void* f_0; // 0x0

    ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame);
    ScrollTouchWindow(int x, int y, int w, int h);
    ~ScrollTouchWindow();

    void setText(AbyssEngine::String title, AbyssEngine::String text);
    void setText(AbyssEngine::String title, AbyssEngine::String text, int color);
    void setYPosition(int y);
    void draw();
    void setTextCentered(bool centered);
    void drawTextBG();
    void update(int dt);
    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
};

static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline unsigned char &UC(void *p, int off) { return *(unsigned char *)((char *)p + off); }
static inline unsigned short &US(void *p, int off) { return *(unsigned short *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

#endif
