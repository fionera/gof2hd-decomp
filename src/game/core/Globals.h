#ifndef GOF2_GLOBALS_H
#define GOF2_GLOBALS_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 - top-level Globals class (no AbyssEngine namespace).
// Owns the shared game singletons and a handful of stateful helpers; most members
// are static-style helper routines driven by the global game state.

// Touched by pointer only in the two-argument init() entry point.
namespace AbyssEngine { class ApplicationManager; class Engine; }
class AEGeometry;
class Agent;
class Station;
class BoundingVolume;

class Globals {
public:
    Array<int>* soundResources;         // active sound-resource id list
    int field_0x34;                     // passenger/cargo count (HangarWindow)
    void* field_0x3c;                   // sell-price table handle ([count, int-array-addr])
    void* field_0x40;                   // buy-price table handle ([count, int-array-addr])
    void* field_0x48;                   // system-price table handle ([count, int-array-addr])
    unsigned int* field_0x54;           // availability descriptor ([count, byte-array-addr])
    int field_0xac;                     // item table handle (+0x4 is its int-array base)
    unsigned short field_0x110;
    int field_0x114;                    // hangar/upgrade mode selector (compared == 3)
    int field_0x14c;                    // selected slot index (HangarWindow)

    Globals();
    ~Globals();

    unsigned getRandomEnemyFighter(int kind);
    // Zero the opaque per-station hint-state blob (clears all hint flags).
    void resetHints();
    // Look up a localized item name by item index.
    String getItemName(int item);
    // Build the LOD geometry for the given ship type/variant (kind==0xf builds the
    // articulated capital-ship geometry). `wireframe` skips material creation.
    AEGeometry* getShipGroup(int type, int variant, bool wireframe);
    int init(void* app);
    // Engine bring-up entry point; the renderer root is supplied for completeness but the
    // body only needs the application manager (forwarded to init(void*)).
    int init(AbyssEngine::ApplicationManager* app, AbyssEngine::Engine* engine);
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
    // (Re)create the bitmap fonts for the given language/script kind and apply per-kind glyph spacing.
    void loadFont(int kind);
    // Set the spacing of the freshly created extra font to 0.
    static void loadFont_tail(void* canvas, void* font, int spacing);
    // Release the secondary canvas' resources too.
    static void releaseResources_tail(void* secondaryCanvas);
    // Map a resolved race bucket (0..8) + gender + dialogue code to a sound id (-1 when unmapped).
    // Only bucket 0/5 consults isMale; all other buckets ignore it.
    int dialogueDispatch(int category, int code, int isMale);
    // Offer/event briefing-text assembly used by getAgentMissionText().
    static void buildAgentMissionText(String* out, void* agent, int offer);
    // The kind==0xf (capital-ship) branch of getShipGroup(): build and return the articulated LOD geometry.
    AEGeometry* buildShipGroup0f(int variant, void* canvas);
    // Word-wrap `text` to `maxWidth` (in the given font), producing one trimmed String per line
    // into `out` (which is resized to the line count and filled with freshly allocated Strings).
    void getLineArray(unsigned int font, const String& text, int maxWidth, Array<String*>* out);
    // Extract the next wrapped line of `text` that fits `maxWidth` in `font`, written into `*out`.
    void getLine(unsigned int font, String text, int maxWidth, String* out);

    // Release all canvas/render resources (both primary and secondary canvas).
    void releaseResources();
    // Report the current kill count into the platform leaderboard slot.
    void reportLeaderboards();
    // Localized key-action label (currently returns an empty String).
    String getKeyActionName(int action);
    // Pick a random system index suitable for the bar-drink minigame.
    void getRandomSystemForDrinks();
    // Substitute the configured key-binding tokens inside `src`, returning the result.
    String replaceKeyBindingTokens(const String& src);
    // Load and return a randomly chosen station.
    Station* getRandomStation();
    // Format `ms` (milliseconds) as "h:mm:ss" into `out`.
    void longToTimeString(long long ms, String& out);
    // Format `ms` (milliseconds) as "h:mm" (no seconds) into `out`.
    void longToTimeStringNoSeconds(long long ms, String& out);
    // Truncate `text` to fit `width` (in pixels), appending an ellipsis when clipped.
    String getBoundedString(const String& text, int width);
    // Assemble the briefing/offer text shown for `agent`.
    String getAgentMissionText(Agent* agent);
    // Build the screen-space coordinates of the steering HUD overlay.
    void setCoordsSteer(int p1, int p2, int p3, int p4,
                        unsigned short& o5, unsigned short& o6, unsigned short& o7,
                        unsigned short& o8, unsigned short& o9, unsigned short& o10,
                        unsigned short& o11, unsigned short& o12, unsigned short& o13,
                        unsigned short& o14);
    // Build the screen-space coordinates of the weapon-fire HUD overlay.
    void setCoordsFire(int p1, int p2, unsigned int p3, unsigned int p4,
                       unsigned int& o5, unsigned short& o6, unsigned short& o7,
                       unsigned short& o8, unsigned short& o9, unsigned short& o10,
                       unsigned short& o11, unsigned short& o12, unsigned short& o13,
                       unsigned short& o14, unsigned short& o15, unsigned short& o16,
                       unsigned short& o17);
    // Build a textured billboard quad mesh in the shared canvas; returns the new mesh id.
    unsigned int createBillBoard(int p1, int height, float u0, float v0, float u1, float v1, int width);
    // Build the wreck collision volumes for ship `kind` (optionally fitted to `geom`).
    Array<BoundingVolume*>* getWreckCollision(int kind, AEGeometry* geom);
    // Map a dialogue text id (and optionally `agent`'s race/gender) to a voice-over sound id.
    int getDialogueSoundId(int code, Agent* agent);
    // Load and return a random planet name.
    String getRandomPlanetName();
    // Compose a random "first last" agent name for race `kind` (`both` selects the secondary table).
    String getRandomName(int kind, bool both);
    // The active sound-resource id list.
    Array<int>* getSoundResourceList();
};

extern Globals* gGlobals;          // canonical Globals singleton (binary .bss 0x2281d0)

#endif // GOF2_GLOBALS_H
