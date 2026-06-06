#include "class.h"

struct PaintCanvas {
    static void MeshCreate(PaintCanvas *self, int mesh, unsigned int *dst, bool flag);
    static void TextureCreate(PaintCanvas *self, int id, unsigned int *dst, bool flag);
};

struct KIPlayer {
    static void setDead(KIPlayer *self);
};

__attribute__((visibility("hidden"))) extern PaintCanvas **g_paintCanvas_intro;

void Level::switchSkyboxForIntro() {
    PaintCanvas **pc = g_paintCanvas_intro;
    PaintCanvas::MeshCreate(*pc, 0x4591, (unsigned int *)((char *)this + 4), false);
    PaintCanvas::TextureCreate(*pc, 0x275a, (unsigned int *)((char *)this + 0x198), false);
    Array *list = (Array *)asteroids;
    if (list != 0) {
        for (unsigned int i = 0; i < list->size; i = i + 1) {
            KIPlayer::setDead(((KIPlayer **)list->data)[i]);
            list = (Array *)asteroids;
        }
    }
}
