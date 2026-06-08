#include "class.h"

// Hud::init() — one-time HUD setup. Loads the ~70 HUD image atlases into the per-image fields,
// loads the current system's faction badge (unless in an alien orbit), allocates the key-state
// tracking arrays (+0x28c keys / +0x290 element bits, 0x19 entries each), seeds the message
// string, and resets the menu/key state. Returns 0.
//
// The flat list of Image2DCreate(canvas, atlasId, &field) calls is delegated to an extern that
// performs the genuine loads; everything with observable side effects on `this` is inline.
extern "C" void Hud_loadImages(Hud *self);   // the 70+ Image2DCreate calls
extern "C" void Image2DCreate(void *canvas, unsigned short id, void *outField);
extern "C" void *operator_new(uint32_t);
extern "C" void *operator_new_arr(uint32_t);
extern "C" void Array_void_ctor(void *arr);
extern "C" void ArraySetLength_void(int n, void *arr);
extern "C" int  Status_inAlienOrbit();
extern "C" void *Status_getSystem();
extern "C" int  SolarSystem_getRace(void *sys);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_op_assign(void *dst, void *src);
extern "C" void String_dtor(void *s);
extern "C" void Hud_closeHudMenu(Hud *self);
extern "C" void Hud_checkIfQuickMenuIsEmpty(Hud *self);
extern "C" void Hud_releaseAllKeys(Hud *self);
extern "C" int  PaintCanvas_GetImage2DWidth(void *canvas);

__attribute__((visibility("hidden"))) extern void **g_Hud_initCanvas;  // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_initLayout;  // *holder -> layout (+0x194,+0x198)
__attribute__((visibility("hidden"))) extern void **g_Hud_initBound;   // *holder -> [0] x bound
__attribute__((visibility("hidden"))) extern void **g_Hud_initOutX;    // *holder -> int cell (x out)
__attribute__((visibility("hidden"))) extern void **g_Hud_initOutY;    // *holder -> int cell (y out)
__attribute__((visibility("hidden"))) extern const unsigned short g_Hud_raceBadge[]; // race->atlas id
extern const char g_Hud_initMsg[] __attribute__((visibility("hidden")));

extern "C" int Hud_init(Hud *self)
{
    Hud_loadImages(self);

    UC(self, 0x4c8) = 0;
    UC(self, 0x528) = 0;

    // key-state arrays: 0x19 slots each
    void *keys = operator_new(0xc);
    Array_void_ctor(keys);
    P(self, 0x28c) = keys;
    ArraySetLength_void(0x19, keys);
    void *bits = operator_new_arr(100);
    P(self, 0x290) = bits;
    for (int i = 0; i != 0x19; i++) {
        *(int *)(I(P(self, 0x28c), 4) + i * 4) = 0;
        *(int *)(I(self, 0x290) + i * 4) = 0;
    }
    I(self, 0x284) = 0;

    // current system faction badge
    if (Status_inAlienOrbit() == 0) {
        void *canvas = *g_Hud_initCanvas;
        Status_getSystem();
        int race = SolarSystem_getRace(Status_getSystem());
        Image2DCreate(canvas, g_Hud_raceBadge[race], B(self, 0x1c4));
    }

    // seed the (empty) HUD message
    I(self, 0x514) = -1;
    I(self, 0x518) = 0;
    {
        char tmp[12];
        String_ctor_cstr(tmp, g_Hud_initMsg, false);
        String_op_assign(B(self, 0x51c), tmp);
        String_dtor(tmp);
    }

    Hud_closeHudMenu(self);
    Hud_checkIfQuickMenuIsEmpty(self);
    Hud_releaseAllKeys(self);
    P(self, 0x530) = 0;

    int *layout = (int *)*g_Hud_initLayout;
    int w = PaintCanvas_GetImage2DWidth(*g_Hud_initCanvas);
    int bound = *(int *)*g_Hud_initBound;
    *(int *)*g_Hud_initOutX = (bound - w) - layout[0x65]; // +0x194
    *(int *)*g_Hud_initOutY = layout[0x66];               // +0x198
    return 0;
}
