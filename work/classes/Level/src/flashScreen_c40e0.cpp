#include "class.h"

struct PlayerEgo {
    void hitCamera();
};

__attribute__((visibility("hidden"))) extern float *g_flash2_a;
__attribute__((visibility("hidden"))) extern float *g_flash2_b;
__attribute__((visibility("hidden"))) extern float *g_flash2_c;
__attribute__((visibility("hidden"))) extern float *g_flashCol_r;
__attribute__((visibility("hidden"))) extern float *g_flashCol_g;
__attribute__((visibility("hidden"))) extern float *g_flashCol_b;

static inline float clampChannel(float scaled) {
    int v = (int)scaled;
    if (v > 0xfe) {
        v = 0xff;
    }
    if (v < 0xb) {
        v = 10;
    }
    return (float)v;
}

void Level::flashScreen(int type) {
    flashType = type;
    flashActive = 1;
    int dur = 2000;
    if (type == 1) {
        dur = 7000;
    }
    if (type > 2) {
        dur = 10000;
    }
    flashDurationA = dur;
    flashDurationB = dur;
    if (type == 2) {
        flashColor.x = *g_flash2_a * 1.5f;
        flashColor.y = *g_flash2_b * 1.5f;
        flashColor.z = *g_flash2_c * 1.5f;
    } else if (type == 4) {
        flashColor.x = 0;
        flashColor.y = 0;
        flashColor.z = 255.0f;
    } else if (type == 3) {
        flashColor.x = 0;
        flashColor.y = 0;
        flashColor.z = 0;
        ((PlayerEgo *)player)->hitCamera();
    } else {
        float k = (type == 1) ? 8.0f : 5.0f;
        flashColor.x = clampChannel(k * *g_flashCol_r);
        flashColor.y = clampChannel(k * *g_flashCol_g);
        flashColor.z = clampChannel(k * *g_flashCol_b);
    }
    *(float *)&flashField = 255.0f;
}
