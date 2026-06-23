#include "engine/render/ResourceMaterial.h"
#include "engine/render/ResourceTexture.h"
#include "engine/render/Engine.h"
#include "engine/core/ApplicationManager.h"

#ifndef GOF2_ENUM_BlendMode
#define GOF2_ENUM_BlendMode
#endif
#include "engine/render/PaintCanvas.h"
#include "game/core/String.h"

namespace AbyssEngine {
    ResourceMaterial::ResourceMaterial(uint16_t texId, uint16_t texId2, BlendMode blend) {
        this->blendMode = static_cast<int>(blend);
        this->field_14 = 0;
        this->field_18 = 0;
        this->field_1c = 0x3f800000;
        this->field_20 = 0;
        this->field_24 = 0;
        for (int i = 0; i != 8; ++i)
            this->texIndices[i] = 0xffff;
        this->texIndices[1] = texId2;
        this->texIndices[0] = texId;
    }

    ResourceMaterial::ResourceMaterial(uint16_t texId, BlendMode blend) {
        this->blendMode = static_cast<int>(blend);
        this->field_14 = 0;
        this->field_18 = 0;
        this->field_1c = 0x3f800000;
        this->field_20 = 0;
        this->field_24 = 0;
        for (int i = 0; i != 8; ++i)
            this->texIndices[i] = 0xffff;
        this->texIndices[0] = texId;
    }

    struct Resource {
        unsigned short id;
        int kind;
        int unused;
        void *payload;
    };
}

static char g_Portraits_zeroFlag = 0;
static char *g_Portraits_ipadLargeFlag = &g_Portraits_zeroFlag;
static char *g_Portraits_ipad1440Flag = &g_Portraits_zeroFlag;
static char *g_Portraits_ipadFlagA = &g_Portraits_zeroFlag;
static char *g_Portraits_ipadFlagB = &g_Portraits_zeroFlag;
static char *g_Portraits_ipadFlagC = &g_Portraits_zeroFlag;

__attribute__ ((always_inline))

static inline void
addPortrait(AbyssEngine::PaintCanvas *canvas,
            const AbyssEngine::String &suffix,
            unsigned short id, const char *base) {
    AbyssEngine::String fullName = AbyssEngine::String(base, false) + suffix +
                                   AbyssEngine::String(".aei", false);
    AbyssEngine::Resource *res = new AbyssEngine::Resource;
    res->id = id;
    res->kind = 2;
    res->unused = -1;
    res->payload = new AbyssEngine::ResourceTexture(fullName, 0.0f);
    canvas->AddResource(res);
}

void loadPortraits(AbyssEngine::Engine *engine) {
    AbyssEngine::PaintCanvas *canvas = engine->appManager->paintCanvas;

    const char *infix;
    if (*g_Portraits_ipadLargeFlag != 0)
        infix = "_ipad_large";
    else if (*g_Portraits_ipad1440Flag != 0)
        infix = "_ipad_1440";
    else if ((*g_Portraits_ipadFlagA | *g_Portraits_ipadFlagB | *g_Portraits_ipadFlagC) != 0)
        infix = "_ipad";
    else
        infix = "";
    AbyssEngine::String suffix(infix, false);

    addPortrait(canvas, suffix, 0x27d8, "data/textures/0_0_0");
    addPortrait(canvas, suffix, 0x27d9, "data/textures/0_0_1");
    addPortrait(canvas, suffix, 0x27da, "data/textures/0_0_2");
    addPortrait(canvas, suffix, 0x27db, "data/textures/0_0_3");
    addPortrait(canvas, suffix, 0x27dc, "data/textures/0_0_4");
    addPortrait(canvas, suffix, 0x27dd, "data/textures/0_0_5");
    addPortrait(canvas, suffix, 0x27de, "data/textures/0_0_6");
    addPortrait(canvas, suffix, 0x27df, "data/textures/0_0_7");
    addPortrait(canvas, suffix, 0x27e0, "data/textures/0_0_8");
    addPortrait(canvas, suffix, 0x27e1, "data/textures/0_0_9");
    addPortrait(canvas, suffix, 0x27e2, "data/textures/0_0_10");
    addPortrait(canvas, suffix, 0x27e3, "data/textures/0_1_0");
    addPortrait(canvas, suffix, 0x27e4, "data/textures/0_1_1");
    addPortrait(canvas, suffix, 0x27e5, "data/textures/0_1_2");
    addPortrait(canvas, suffix, 0x27e6, "data/textures/0_1_3");
    addPortrait(canvas, suffix, 0x27e7, "data/textures/0_1_4");
    addPortrait(canvas, suffix, 0x27e8, "data/textures/0_1_5");
    addPortrait(canvas, suffix, 0x27e9, "data/textures/0_1_6");
    addPortrait(canvas, suffix, 0x27ea, "data/textures/0_1_7");
    addPortrait(canvas, suffix, 0x27eb, "data/textures/0_1_8");
    addPortrait(canvas, suffix, 0x27ec, "data/textures/0_1_9");
    addPortrait(canvas, suffix, 0x27ed, "data/textures/0_1_10");
    addPortrait(canvas, suffix, 0x27ee, "data/textures/0_2_0");
    addPortrait(canvas, suffix, 0x27ef, "data/textures/0_2_1");
    addPortrait(canvas, suffix, 0x27f0, "data/textures/0_2_2");
    addPortrait(canvas, suffix, 0x27f1, "data/textures/0_2_3");
    addPortrait(canvas, suffix, 0x27f2, "data/textures/0_2_4");
    addPortrait(canvas, suffix, 0x27f3, "data/textures/0_2_5");
    addPortrait(canvas, suffix, 0x27f4, "data/textures/0_2_6");
    addPortrait(canvas, suffix, 0x27f5, "data/textures/0_2_7");
    addPortrait(canvas, suffix, 0x27f6, "data/textures/0_2_8");
    addPortrait(canvas, suffix, 0x27f7, "data/textures/0_2_9");
    addPortrait(canvas, suffix, 0x27f8, "data/textures/0_2_10");
    addPortrait(canvas, suffix, 0x27f9, "data/textures/0_3_0");
    addPortrait(canvas, suffix, 0x27fa, "data/textures/0_3_1");
    addPortrait(canvas, suffix, 0x27fb, "data/textures/0_3_2");
    addPortrait(canvas, suffix, 0x27fc, "data/textures/0_3_3");
    addPortrait(canvas, suffix, 0x27fd, "data/textures/0_3_4");
    addPortrait(canvas, suffix, 0x27fe, "data/textures/0_3_5");
    addPortrait(canvas, suffix, 0x27ff, "data/textures/0_3_6");
    addPortrait(canvas, suffix, 0x2800, "data/textures/0_3_7");
    addPortrait(canvas, suffix, 0x2801, "data/textures/0_3_8");
    addPortrait(canvas, suffix, 0x2802, "data/textures/0_3_9");
    addPortrait(canvas, suffix, 0x2803, "data/textures/0_3_10");
    addPortrait(canvas, suffix, 0x2804, "data/textures/1_0_0");
    addPortrait(canvas, suffix, 0x2805, "data/textures/1_0_1");
    addPortrait(canvas, suffix, 0x2806, "data/textures/1_0_2");
    addPortrait(canvas, suffix, 0x2807, "data/textures/1_0_3");
    addPortrait(canvas, suffix, 0x2808, "data/textures/1_1_0");
    addPortrait(canvas, suffix, 0x2809, "data/textures/1_1_1");
    addPortrait(canvas, suffix, 0x280a, "data/textures/1_1_2");
    addPortrait(canvas, suffix, 0x280b, "data/textures/1_1_3");
    addPortrait(canvas, suffix, 0x280c, "data/textures/1_1_4");
    addPortrait(canvas, suffix, 0x280d, "data/textures/1_2_0");
    addPortrait(canvas, suffix, 0x280e, "data/textures/1_2_1");
    addPortrait(canvas, suffix, 0x280f, "data/textures/1_2_2");
    addPortrait(canvas, suffix, 0x2810, "data/textures/1_2_3");
    addPortrait(canvas, suffix, 0x2811, "data/textures/1_2_4");
    addPortrait(canvas, suffix, 0x2812, "data/textures/1_2_5");
    addPortrait(canvas, suffix, 0x2813, "data/textures/1_3_0");
    addPortrait(canvas, suffix, 0x2814, "data/textures/1_3_1");
    addPortrait(canvas, suffix, 0x2815, "data/textures/1_3_2");
    addPortrait(canvas, suffix, 0x2816, "data/textures/1_3_3");
    addPortrait(canvas, suffix, 0x2817, "data/textures/1_3_4");
    addPortrait(canvas, suffix, 0x2818, "data/textures/1_3_5");
    addPortrait(canvas, suffix, 0x2819, "data/textures/1_3_6");
    addPortrait(canvas, suffix, 0x281a, "data/textures/1_3_7");
    addPortrait(canvas, suffix, 0x281b, "data/textures/1_3_8");
    addPortrait(canvas, suffix, 0x281c, "data/textures/2_0_0");
    addPortrait(canvas, suffix, 0x281d, "data/textures/2_0_1");
    addPortrait(canvas, suffix, 0x281e, "data/textures/2_0_2");
    addPortrait(canvas, suffix, 0x281f, "data/textures/2_0_3");
    addPortrait(canvas, suffix, 0x2820, "data/textures/2_0_4");
    addPortrait(canvas, suffix, 0x2821, "data/textures/2_1_0");
    addPortrait(canvas, suffix, 0x2822, "data/textures/2_1_1");
    addPortrait(canvas, suffix, 0x2823, "data/textures/2_1_2");
    addPortrait(canvas, suffix, 0x2824, "data/textures/2_1_3");
    addPortrait(canvas, suffix, 0x2825, "data/textures/2_1_4");
    addPortrait(canvas, suffix, 0x2826, "data/textures/2_2_0");
    addPortrait(canvas, suffix, 0x2827, "data/textures/2_2_1");
    addPortrait(canvas, suffix, 0x2828, "data/textures/2_2_2");
    addPortrait(canvas, suffix, 0x2829, "data/textures/2_2_3");
    addPortrait(canvas, suffix, 0x282a, "data/textures/2_2_4");
    addPortrait(canvas, suffix, 0x282b, "data/textures/2_3_0");
    addPortrait(canvas, suffix, 0x282c, "data/textures/2_3_1");
    addPortrait(canvas, suffix, 0x282d, "data/textures/2_3_2");
    addPortrait(canvas, suffix, 0x282e, "data/textures/2_3_3");
    addPortrait(canvas, suffix, 0x282f, "data/textures/2_3_4");
    addPortrait(canvas, suffix, 0x2830, "data/textures/4_0_0");
    addPortrait(canvas, suffix, 0x2831, "data/textures/4_0_1");
    addPortrait(canvas, suffix, 0x2832, "data/textures/4_0_2");
    addPortrait(canvas, suffix, 0x2833, "data/textures/4_1_0");
    addPortrait(canvas, suffix, 0x2834, "data/textures/4_1_1");
    addPortrait(canvas, suffix, 0x2835, "data/textures/4_1_2");
    addPortrait(canvas, suffix, 0x2836, "data/textures/4_2_0");
    addPortrait(canvas, suffix, 0x2837, "data/textures/4_2_1");
    addPortrait(canvas, suffix, 0x2838, "data/textures/4_2_2");
    addPortrait(canvas, suffix, 0x2839, "data/textures/4_2_3");
    addPortrait(canvas, suffix, 0x283a, "data/textures/4_2_4");
    addPortrait(canvas, suffix, 0x283b, "data/textures/4_3_0");
    addPortrait(canvas, suffix, 0x283c, "data/textures/4_3_1");
    addPortrait(canvas, suffix, 0x283d, "data/textures/4_3_2");
    addPortrait(canvas, suffix, 0x283e, "data/textures/4_3_3");
    addPortrait(canvas, suffix, 0x283f, "data/textures/6_0_0");
    addPortrait(canvas, suffix, 0x2840, "data/textures/6_0_1");
    addPortrait(canvas, suffix, 0x2841, "data/textures/6_1_0");
    addPortrait(canvas, suffix, 0x2842, "data/textures/6_1_1");
    addPortrait(canvas, suffix, 0x2843, "data/textures/6_1_2");
    addPortrait(canvas, suffix, 0x2844, "data/textures/6_2_0");
    addPortrait(canvas, suffix, 0x2845, "data/textures/6_2_1");
    addPortrait(canvas, suffix, 0x2846, "data/textures/6_2_2");
    addPortrait(canvas, suffix, 0x2847, "data/textures/6_2_3");
    addPortrait(canvas, suffix, 0x2848, "data/textures/6_2_4");
    addPortrait(canvas, suffix, 0x2849, "data/textures/7_0_0");
    addPortrait(canvas, suffix, 0x284a, "data/textures/7_0_1");
    addPortrait(canvas, suffix, 0x284b, "data/textures/7_1_0");
    addPortrait(canvas, suffix, 0x284c, "data/textures/7_1_1");
    addPortrait(canvas, suffix, 0x284d, "data/textures/7_2_0");
    addPortrait(canvas, suffix, 0x284e, "data/textures/7_2_1");
    addPortrait(canvas, suffix, 0x284f, "data/textures/7_2_2");
    addPortrait(canvas, suffix, 0x2850, "data/textures/7_3_0");
    addPortrait(canvas, suffix, 0x2851, "data/textures/7_3_1");
    addPortrait(canvas, suffix, 0x2852, "data/textures/9_0_0");
    addPortrait(canvas, suffix, 0x2853, "data/textures/9_1_0");
    addPortrait(canvas, suffix, 0x2854, "data/textures/9_2_0");
    addPortrait(canvas, suffix, 0x2855, "data/textures/9_3_0");
    addPortrait(canvas, suffix, 0x2856, "data/textures/10_0_0");
    addPortrait(canvas, suffix, 0x2857, "data/textures/10_0_1");
    addPortrait(canvas, suffix, 0x2858, "data/textures/10_0_2");
    addPortrait(canvas, suffix, 0x2859, "data/textures/10_0_3");
    addPortrait(canvas, suffix, 0x285a, "data/textures/10_1_0");
    addPortrait(canvas, suffix, 0x285b, "data/textures/10_1_1");
    addPortrait(canvas, suffix, 0x285c, "data/textures/10_1_2");
    addPortrait(canvas, suffix, 0x285d, "data/textures/10_1_3");
    addPortrait(canvas, suffix, 0x285e, "data/textures/10_2_0");
    addPortrait(canvas, suffix, 0x285f, "data/textures/10_2_1");
    addPortrait(canvas, suffix, 0x2860, "data/textures/10_2_2");
    addPortrait(canvas, suffix, 0x2861, "data/textures/10_2_3");
    addPortrait(canvas, suffix, 0x2862, "data/textures/10_2_4");
    addPortrait(canvas, suffix, 0x2863, "data/textures/10_3_0");
    addPortrait(canvas, suffix, 0x2864, "data/textures/10_3_1");
    addPortrait(canvas, suffix, 0x2865, "data/textures/10_3_2");
    addPortrait(canvas, suffix, 0x2866, "data/textures/10_3_3");
    addPortrait(canvas, suffix, 0x2867, "data/textures/10_3_4");
    addPortrait(canvas, suffix, 0x2868, "data/textures/10_3_5");
    addPortrait(canvas, suffix, 0x2869, "data/textures/10_3_6");
    addPortrait(canvas, suffix, 0x286a, "data/textures/11_3_2");
    addPortrait(canvas, suffix, 0x286b, "data/textures/11_3_3");
    addPortrait(canvas, suffix, 0x286c, "data/textures/12_0_0");
    addPortrait(canvas, suffix, 0x286d, "data/textures/12_1_0");
    addPortrait(canvas, suffix, 0x286e, "data/textures/12_2_0");
    addPortrait(canvas, suffix, 0x286f, "data/textures/12_3_0");
}
