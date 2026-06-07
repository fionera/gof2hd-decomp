#include "class.h"

struct MiningGame {
    bool isInCurrentLayer();
    int update(int delta);
};

__attribute__((visibility("hidden"))) extern void **g_MiningGame_random;
__attribute__((visibility("hidden"))) extern int (*g_MiningGame_randomNext)(void *random, int limit);
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layoutUpdate;
__attribute__((visibility("hidden"))) extern float *g_MiningGame_layerSpeedUpdate;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_sound;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusUpdate;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_achievements;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusMedal;

extern "C" void MiningGame_MarqueeImage_update(void *self, int delta);
extern "C" void MiningGame_MarqueeImage_setSpeed_update(void *self, float speed);
extern "C" void MiningGame_FModSound_setParamValue(void *sound, int index, int param, float value);
extern "C" void MiningGame_FModSound_play(void *sound, int id, void *a, void *b, void *c);
extern "C" void MiningGame_FModSound_stop(void *sound, int id);
extern "C" void MiningGame_Sprite_nextFrame(void *sprite);
extern "C" int MiningGame_Achievements_hasMedal(void *achievements, int medal, int value);
extern "C" int MiningGame_Achievements_getValue(void *achievements, int medal, int value);
extern "C" int MiningGame_idivmod(int value, int divisor);
extern "C" void MiningGame_Hud_hudEventMedal(void *hud, int medal, int value);

int MiningGame::update(int delta)
{
    int pulse = I(this, 0xcc) + delta;
    if (pulse >= 2000) {
        pulse = 0;
    }
    I(this, 0xcc) = pulse;

    int wasInLayer = isInCurrentLayer();
    int driftTimer = I(this, 0x6c) + delta;
    I(this, 0x6c) = driftTimer;
    if (driftTimer >= 2501) {
        void **randomHolder = g_MiningGame_random;
        int (*next)(void *, int) = g_MiningGame_randomNext;
        I(this, 0x6c) = next(*randomHolder, 2000) + 500;

        int value = next(*randomHolder, 7);
        int sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        F(this, 0x08) = ((float)((value + 5) * sign) / 10.0f) / F(this, 0x2c);

        value = next(*randomHolder, 7);
        sign = -1;
        if (next(*randomHolder, 2) == 0) {
            sign = 1;
        }
        F(this, 0x0c) = ((float)(sign * (value + 5)) / 10.0f) / F(this, 0x2c);

        if (UC(this, 0x80) != 0 && I(this, 0x78) == I(this, 0x7c) - 1) {
            F(this, 0x08) *= 0.3f;
            F(this, 0x0c) *= 0.3f;
        }
    }

    if (UC(this, 0x84) == 0 && !isInCurrentLayer()) {
        F(this, 0x08) = ((float)I(this, 0x58) - F(this, 0x10)) * 0.03f;
        F(this, 0x0c) = ((float)I(this, 0x5c) - F(this, 0x14)) * 0.03f;
    }

    void **layoutHolder = g_MiningGame_layoutUpdate;
    void *layout = *layoutHolder;
    float frameScale = (float)delta;
    F(this, 0x10) += ((F(this, 0x00) + F(this, 0x08)) / F(layout, 0xe4)) * frameScale;
    F(this, 0x14) += ((F(this, 0x04) + F(this, 0x0c)) / F(layout, 0xe4)) * frameScale;

    MiningGame_MarqueeImage_update(P(this, 0x8c), delta);
    MiningGame_MarqueeImage_update(P(this, 0x90), delta);

    float *layerSpeed = g_MiningGame_layerSpeedUpdate;
    void **soundHolder = g_MiningGame_sound;
    MiningGame_FModSound_setParamValue(*soundHolder, 0, 1,
                                       ((layerSpeed[I(this, 0x78)] - 5.0f) / 33.0f) * 3.0f);

    if (isInCurrentLayer()) {
        if (wasInLayer == 0) {
            MiningGame_FModSound_play(*soundHolder, 1, 0, 0, 0);
            MiningGame_FModSound_stop(*soundHolder, 3);
        }

        float anim = F(this, 0x68) +
                     (frameScale / 1000.0f) * F(layout, 0xe0) * layerSpeed[I(this, 0x78)] * 3.0f;
        F(this, 0x68) = anim;
        if (!(anim < 1.0f)) {
            MiningGame_Sprite_nextFrame(P(this, 0x94));
            F(this, 0x68) = 0.0f;
        }

        MiningGame_MarqueeImage_update(P(this, 0x88), delta);
        int nextLayer = I(this, 0x78) + 1;
        float oldOre = F(this, 0x24);
        float layerFactor = 0.15f + ((float)nextLayer / 7.0f) * 2.35f;
        float newOre = oldOre + ((F(this, 0x28) * layerFactor) / 1000.0f) * frameScale;
        F(this, 0x24) = newOre;
        float alpha = oldOre < newOre ? 0.0f : F(this, 0x64);
        int layerTimer = I(this, 0x74) + delta;
        I(this, 0x74) = layerTimer;
        alpha += frameScale / 500.0f;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        F(this, 0x64) = alpha;

        if (layerTimer > 6000) {
            I(this, 0x74) = 0;
            I(this, 0x78) = nextLayer;
            if (nextLayer >= I(this, 0x7c)) {
                UC(this, 0x81) = 1;
                UC(this, 0x83) = I(this, 0x7c) == 7;
                void **achHolder = g_MiningGame_achievements;
                if (MiningGame_Achievements_hasMedal(*achHolder, 0x26, 1) == 0) {
                    void **statusHolder = g_MiningGame_statusMedal;
                    int count = I(*statusHolder, 0x124) + 1;
                    I(*statusHolder, 0x124) = count;
                    int goal = MiningGame_Achievements_getValue(*achHolder, 0x26, 1);
                    int percent = (int)(((float)count / (float)goal) * 100.0f);
                    MiningGame_idivmod(percent, 10);
                    if (percent % 10 == 0) {
                        int shown = (int)(((float)I(*statusHolder, 0x124) /
                                           (float)MiningGame_Achievements_getValue(*achHolder, 0x26, 1)) *
                                          100.0f);
                        if (shown > 29) {
                            MiningGame_Hud_hudEventMedal(P(this, 0xd0), 0x26, shown);
                        }
                    }
                    if (MiningGame_Achievements_getValue(*achHolder, 0x26, 1) <= I(*statusHolder, 0x124)) {
                        UC(*statusHolder, 0x128) = 1;
                    }
                }
                return 0;
            }
            MiningGame_MarqueeImage_setSpeed_update(P(this, 0x88), F(layout, 0xe0) * layerSpeed[nextLayer]);
        }
    } else {
        if (wasInLayer != 0) {
            MiningGame_FModSound_stop(*soundHolder, 1);
            MiningGame_FModSound_play(*soundHolder, 3, 0, 0, 0);
        }
        int lossTimer = I(this, 0x20) + delta;
        I(this, 0x20) = lossTimer;
        if (lossTimer >= 2501) {
            I(this, 0x20) = 0x9c4;
            I(this, 0x24) = 0;
            UC(this, 0x82) = 1;
            I(*g_MiningGame_statusUpdate, 0x124) = 0;
            return 0;
        }
        F(this, 0x64) = 1.0f;
    }

    return 1;
}
