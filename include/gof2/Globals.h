#ifndef GOF2_GLOBALS_H
#define GOF2_GLOBALS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - Globals class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace). Most members are static-style helpers.
// Field offsets recovered per-method from target disasm; access via byte-offset casts.


struct Globals;

// AbyssEngine::String - 12-byte value type (text*, size, ...). Pass-by-value uses the
// >8-byte struct ABI (sret / on stack), matching the target.
typedef AbyssEngine::String String;



// Field accessor via byte offset (used by the few stateful methods).

class Globals {
public:
    Array<int>* field_0x4;              // +0x4  (sound-resource id list)
    int field_0x34;                     // +0x34
    void* field_0x3c;                   // +0x3c
    void* field_0x40;                   // +0x40
    void* field_0x48;                   // +0x48
    unsigned int* field_0x54;           // +0x54
    int field_0xac;                     // +0xac
    unsigned short field_0x110;         // +0x110
    int field_0x114;                    // +0x114
    int field_0x14c;                    // +0x14c

    // ---- methods (converted from free functions) ----
    Globals * ctor();
    void * dtor();
    unsigned getRandomEnemyFighter(int kind);
    int init(void *app);
    float sqrt(float x);

    // ---- recovered leaf/dispatch fragments ----
    // Plain runtime sqrtf (the float arrives in r1 on the target; the instance is unused).
    static float sqrt_impl(float x);
    // 64-bit signed divide returning the quotient; the remainder is written through *rem.
    static long long lts_divmod(long long num, int den, int *rem);
    // Tail of startNewSoundResourceList()/addSoundResourceToList(): append a sound-resource
    // id to the list. Both routed through the same Array<int>::add veneer in the target.
    static void startNewSoundResourceList_tail(int val, Array<int> *list);
    static void addSoundResource_tail(int val, Array<int> *list);
    // Public "add one sound resource to the active list" entry (the (list,id) overload form).
    void addSoundResource_oi(int val);
    // Tail of getRandomSystemForDrinks(): record the drawn system index for the chosen slot.
    static void getRandomSystemForDrinks_tail(int systemSlot, int picked);
    // Tail of loadFont(): set the spacing of the freshly created extra font to 0.
    static void loadFont_tail(void *canvas, void *font, int spacing);
    // Tail of releaseResources(): release the secondary canvas' resources too.
    static void releaseResources_tail(void *secondaryCanvas);
    // Second stage of getDialogueSoundId(): map a resolved race/gender bucket + dialogue
    // code to a sound id (returns -1 when unmapped).
    int dialogueDispatch(int category, int code);
    // Offer/event briefing-text assembly used by getAgentMissionText().
    static void buildAgentMissionText(String *out, void *agent, int offer);
    // The kind==0xf (capital-ship) branch of getShipGroup(): build the articulated LOD geometry.
    void buildShipGroup0f(int variant, void *canvas);
};
#endif
