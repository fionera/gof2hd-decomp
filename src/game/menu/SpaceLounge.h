#ifndef GOF2_SPACELOUNGE_H
#define GOF2_SPACELOUNGE_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/core/AbyssEngine.h"
#include "engine/math/EaseInOut.h"
#include "engine/math/EaseInOutMatrix.h"
#include "engine/math/Vector.h"
#include "engine/render/ImagePart.h"
#include "game/core/CutScene.h"
#include "game/ship/Agent.h"
#include "game/ui/ChoiceWindow.h"
#include "game/ui/ListItemWindow.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/ui/TouchButton.h"
#include "game/world/StarMap.h"

#include "engine/math/AEMath.h"
#include "engine/math/Matrix.h"

class Agent;
class ChoiceWindow;
class CutScene;
class ImagePart;
class ListItemWindow;
class ScrollTouchWindow;
class StarMap;
class TouchButton;

namespace AbyssEngine {
    class EaseInOut;
    class EaseInOutMatrix;
}

class SpaceLounge {
public:
    // ASM (OnRender3D, r4=this): starMap read @4 (field_0x0 precedes); field_0x18 is Ghidra-named @0x18 but
    // sat @0x10 -> a 2nd 4-byte field (field_0x14) precedes it. Matches the +4-then-+8 delta in OnRender3D.
    int field_0x0;
    StarMap *starMap;
    ChoiceWindow *choiceWindow;
    ListItemWindow *listWindow;
    int field_0x10;
    int mode;   // @0x14: every original reader (OnTouchBegin/OnTouchMove/draw/drawLounge/update) loads [this,#20]
    uint8_t field_0x18;
    uint8_t chatActive;
    uint8_t popupActive;
    uint8_t choiceVisible;
    uint8_t listVisible;
    int selectedAgent;
    Array<Agent *> *agents;
    Array<String *> *agentTexts;
    int chatScroll;
    int chatAnswer;
    uint8_t mapVisible;
    uint8_t hangarUpdate;
    uint8_t singleOffer;
    Array<Array<ImagePart *> *> *silhouetteGrid;
    Array<ImagePart *> *agentImageParts;
    Array<AbyssEngine::AEMath::Vector *> *agentRects;
    CutScene *cutScene;
    AbyssEngine::EaseInOutMatrix *cameraEase;
    AbyssEngine::Vector silhouettePos;
    char *agentVisited;
    Array<TouchButton *> *buttons;
    ScrollTouchWindow *scrollWindow;
    int buttonsHeight;
    int visibleButtonCount;
    int panelWidth;
    int panelX;
    int panelY;
    int buttonPanelY;
    int buttonY1;
    int buttonY0;
    int buttonX;
    int hoverAgent;
    AbyssEngine::Vector agentTopLeft;     // @0x8c: screen rect of the agent being talked to
    AbyssEngine::Vector agentBottomRight; // @0x98
    String title;
    uint8_t initialized;
    bool postEffectWasActive; // @0xb1
    uint8_t touchDown;
    int touchX;
    int touchY;
    uint8_t cameraAnimating;
    uint8_t introDone;
    AbyssEngine::EaseInOut *headEase;
    uint8_t headBobReverse;
    AbyssEngine::Matrix baseMatrix;
    float headBobPhase;
    int headBobSteps;

    SpaceLounge();

    ~SpaceLounge();

    void OnRender3D();

    void OnRenderBG();

    bool OnTouchBegin(int x, int y);

    bool OnTouchEnd(int x, int y);

    bool OnTouchMove(int x, int y);

    bool checkLocationMode();

    void draw();

    void draw3DShip();

    void drawLounge();

    int getSoundId(Agent *agent);

    int getSpecificSoundForRace(int soundId, int race, bool alternate);

    unsigned char hangarNeedsUpdate();

    int init();

    unsigned char introFinished();

    bool listMode();

    unsigned char mapMode();

    int onKeyPress(int key);

    void refresh();

    void setHangarUpdate(bool value);

    void startChat();

    void update(int dt);

    void updateScreenPositions();
};
#endif
