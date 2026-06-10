#ifndef GOF2_SPACELOUNGE_H
#define GOF2_SPACELOUNGE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct SpaceLounge;
struct Agent;

static inline char *B(void *p, int off) { return (char *)p + off; 
    // ---- methods (converted from free functions) ----
    void OnRender3D();
    void OnRenderBG();
    int OnTouchBegin(int x, int y);
    void OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void draw();
    void draw3DShip();
    void drawLounge();
    unsigned char hangarNeedsUpdate();
    int init();
    unsigned char introFinished();
    bool listMode();
    unsigned char mapMode();
    void onKeyPress(int key);
    void setHangarUpdate(bool value);
    void startChat();
    void update(int dt);
    void updateScreenPositions();
}
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline unsigned int &U(void *p, int off) { return *(unsigned int *)((char *)p + off); }
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }
static inline unsigned char &UC(void *p, int off) { return *(unsigned char *)((char *)p + off); }
static inline bool &BL(void *p, int off) { return *(bool *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
static inline void *PAt(void *p, int off) { return *(void **)((char *)p + off); }
static inline void *&PIndex(void *p, unsigned idx) { return *(void **)((char *)p + idx * 4); }

struct SpaceLounge { void* _opaque; };  // no offset accesses observed
#endif
