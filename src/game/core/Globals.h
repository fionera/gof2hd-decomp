#ifndef GOF2_GLOBALS_H
#define GOF2_GLOBALS_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "fieldaccess.h"
#include "engine/core/ApplicationManager.h"
#include "engine/math/BoundingVolume.h"
#include "engine/render/AEGeometry.h"
#include "game/ship/Agent.h"
#include "game/ui/Layout.h"
#include "game/world/Station.h"


class Globals {
public:
    Array<int> *soundResources;
    int field_0x34;
    void *field_0x3c;
    void *field_0x40;
    void *field_0x48;
    unsigned int *field_0x54;
    int field_0xac;
    unsigned short field_0x110;
    int field_0x114;
    int field_0x14c;

    Globals();

    ~Globals();

    unsigned getRandomEnemyFighter(int kind);

    void resetHints();

    String getItemName(int item);

    AEGeometry *getShipGroup(int type, int variant, bool wireframe);

    int init(AbyssEngine::ApplicationManager *app, AbyssEngine::Engine *engine);

    float sqrt(float x);

    void startNewSoundResourceList();

    void playMusicAndFadeOutCurrent(int mode);

    void addSoundResourceToList(int snd);

    void loadFont(int kind);

    void getLineArray(unsigned int font, const String &text, int maxWidth, Array<String *> *out);

    void getLine(unsigned int font, String text, int maxWidth, String *out);

    void releaseResources();

    void reportLeaderboards();

    String getKeyActionName(int action);

    void getRandomSystemForDrinks();

    String replaceKeyBindingTokens(const String &src);

    Station *getRandomStation();

    void longToTimeString(long long ms, String &out);

    void longToTimeStringNoSeconds(long long ms, String &out);

    String getBoundedString(const String &text, int width);

    String getAgentMissionText(Agent *agent);

    void setCoordsSteer(int p1, int p2, int p3, int p4,
                        unsigned short &o5, unsigned short &o6, unsigned short &o7,
                        unsigned short &o8, unsigned short &o9, unsigned short &o10,
                        unsigned short &o11, unsigned short &o12, unsigned short &o13,
                        unsigned short &o14);

    void setCoordsFire(int p1, int p2, unsigned int p3, unsigned int p4,
                       unsigned int &o5, unsigned short &o6, unsigned short &o7,
                       unsigned short &o8, unsigned short &o9, unsigned short &o10,
                       unsigned short &o11, unsigned short &o12, unsigned short &o13,
                       unsigned short &o14, unsigned short &o15, unsigned short &o16,
                       unsigned short &o17);

    unsigned int createBillBoard(int p1, int height, float u0, float v0, float u1, float v1, int width);

    Array<BoundingVolume *> *getWreckCollision(int kind, AEGeometry *geom);

    int getDialogueSoundId(int code, Agent *agent);

    String getRandomPlanetName();

    String getRandomName(int kind, bool both);

    Array<int> *getSoundResourceList();

    int getInAppPurchaseArrayIndex(int productCode, Array<String *> *list);

    String getKeyBindingReplaceString(int key);

    void reportSupernovaChallengeScore();

    void drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY);

    void drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY, bool centered);

    void drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY,
                   unsigned int rightX, bool centered);

    static int is_dialogue_window_visible;
    static int is_choice_window_visible;
    static int is_menu_visible;
    static int is_hacking_visible;
    static unsigned char isStarMapVisible;
    static unsigned char isCinematicModeActive;
    static int mouseCursorActivated;
    static unsigned char showMouseDuringGameOver;
    static unsigned char keyBindings[8];

    static int left_edge;
    static int right_edge;
    static int top_edge;
    static int bottom_edge;
    static int resetKeyboard;
    static int rotateShipInStation;
    static int translateStarMapInXDirection;
    static int translateStarMapInYDirection;
    static int smallButton_dim;
    static int touch_stick_x;
    static int touch_stick_y;

    static unsigned char iPad;
    static unsigned char iPadHD;
    static unsigned char retinaDisplay;
    static unsigned char n9;
    static unsigned char iPadLarge;
    static unsigned char iPadLargePossible;
    static unsigned char iPadAssetsWithLowerRes;
    static unsigned char enterSpaceLounge;
    static int switch_to_target_setting;
};

extern Globals *gGlobals;
extern Layout *gLayout;
extern void *gFont;
extern int gScreenWidth;
extern int gScreenHeight;
extern AbyssEngine::ApplicationManager *gAppManager;

#endif
