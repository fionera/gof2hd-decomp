#ifndef GOF2_GLOBALS_H
#define GOF2_GLOBALS_H
#include "gof2/common.h"

// Galaxy on Fire 2 - top-level Globals class (no AbyssEngine namespace).
// Owns the shared game singletons and a handful of stateful helpers; most members
// are static-style helper routines driven by the global game state.

class Globals {
public:
    Array<int>* soundResources;         // active sound-resource id list
    int field_0x34;
    void* field_0x3c;
    void* field_0x40;
    void* field_0x48;
    unsigned int* field_0x54;
    int field_0xac;
    unsigned short field_0x110;
    int field_0x114;
    int field_0x14c;

    Globals();
    ~Globals();

    unsigned getRandomEnemyFighter(int kind);
    // Resolve the ship-group id for a given ship type/race (kind==0xf builds capital-ship LOD geometry).
    int getShipGroup(int type, int race, int flag);
    int init(void* app);
    float sqrt(float x);

    // Plain runtime sqrtf wrapper (the instance is unused).
    static float sqrt_impl(float x);
    // 64-bit signed divide returning the quotient; the remainder is written through *rem.
    static long long lts_divmod(long long num, int den, int* rem);
    // Append a sound-resource id to the given list.
    static void startNewSoundResourceList_tail(int val, Array<int>* list);
    static void addSoundResource_tail(int val, Array<int>* list);
    // Add one sound resource to the active list (skipping it if already present).
    void addSoundResource_oi(int val);
    // Same, addressed by sound id (the assignGuns weapon-sound registration path).
    void addSoundResourceToList(int snd);
    // Record the drawn system index for the chosen slot.
    static void getRandomSystemForDrinks_tail(int systemSlot, int picked);
    // Set the spacing of the freshly created extra font to 0.
    static void loadFont_tail(void* canvas, void* font, int spacing);
    // Release the secondary canvas' resources too.
    static void releaseResources_tail(void* secondaryCanvas);
    // Map a resolved race/gender bucket + dialogue code to a sound id (-1 when unmapped).
    int dialogueDispatch(int category, int code);
    // Offer/event briefing-text assembly used by getAgentMissionText().
    static void buildAgentMissionText(String* out, void* agent, int offer);
    // The kind==0xf (capital-ship) branch of getShipGroup(): build the articulated LOD geometry.
    void buildShipGroup0f(int variant, void* canvas);
};

extern Globals* gGlobals;          // canonical Globals singleton (binary .bss 0x2281d0)

#endif // GOF2_GLOBALS_H
