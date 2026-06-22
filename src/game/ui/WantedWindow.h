#ifndef GOF2_WANTEDWINDOW_H
#define GOF2_WANTEDWINDOW_H

#include <cstdint>
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class StarMap;
class ScrollTouchWindow;
class TouchButton;
class ImagePart;
class Wanted;
class Mission;

class WantedWindow {
public:
    uint32_t lastButtonHit;
    StarMap *starMap;
    Array<ImagePart *> *imageParts;
    Array<TouchButton *> *buttons;
    int halfTextHeight;
    uint8_t showingMap;
    uint8_t hangarUpdate;
    TouchButton *detailButton;
    int windowX;
    int windowY;
    int windowWidth;
    int windowHeight;
    ScrollTouchWindow *scrollWindow;
    uint32_t selectedWanted;
    uint32_t highlightedWanted;
    Array<Wanted *> *wantedList;
    String fromText; // detail: "from" line (origin station + system)
    String toText; // detail: "to" line (destination station + system)
    String nameText; // selected wanted's name
    String detailText; // detail line (assigned getText(0xc9d))
    String atText; // detail: "at" line (current station + system)
    String rewardText; // detail: reward line
    int scrollOffset;
    int lastDragY;
    int scrollOffsetSnapshot;
    int dragDelta;
    float scrollDamping;
    float scrollVelocity;
    int touchStartY;
    uint8_t dragging;
    int contentHeight;
    int visibleHeight;
    int bgImage;
    Mission *mission;

    WantedWindow();

    ~WantedWindow();

    void setHangarUpdate(bool needsUpdate);

    bool hangarNeedsUpdate();

    int OnTouchBegin(int x, int y);

    void OnTouchEnd(int x, int y);

    int OnTouchMove(int x, int y);

    void draw();

    float getRelativeScrollHeight();

    float getRelativeScrollStartPos();

    uint32_t getWantedAtPosition(int x, int y);

    int init();

    void render3D();

    void selectWanted(int idx);

    void update(int dt);
};
#endif
