#ifndef GOF2_STATUSWINDOW_H
#define GOF2_STATUSWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class TouchButton;
class ImagePart;

typedef AbyssEngine::String String;

// StatusWindow — the player status / achievements screen. Hosts a two-tab bar
// (stats / medals), a scrollable medal grid and a per-medal detail panel.
class StatusWindow {
public:
    int medalCount;                          // number of medal buttons
    Array<TouchButton*>* tabButtons;
    Array<TouchButton*>* medalButtons;
    Array<ImagePart*>* imageParts;           // glyph image tiles for drawChar
    Array<String*>* detailLines;
    unsigned rankImage0;                     // rank/medal tile image handles
    unsigned rankImage1;
    unsigned rankImage2;
    unsigned rankImage3;
    unsigned standingEmblemImage;
    unsigned standingBarImage;
    unsigned standingFrameImage;
    unsigned activeTab;
    int selectedMedal;                       // selected medal index (-1 == none)
    int scrollOffset;
    int lastTouchY;
    int scrollTarget;
    int scrollVelocity;
    float scrollDamping;                     // inertia damping factor
    float scrollVelocityF;                   // float scroll velocity
    int touchStartY;
    unsigned char isDragging;
    int contentHeight;                       // current-tab content height
    int viewportHeight;                      // scrollable viewport height
    int charImageWidth;                      // credit-glyph image width
    int charImageHeight;                     // credit-glyph image height
    int* tabHeights;                         // per-tab scroll content heights
    int boxWidth;                            // content column / box width
    int standingBarWidth;                    // standing bar half-width
    int standingBarHeight;
    int medalRowHeight;                      // medal grid row height

    StatusWindow();
    ~StatusWindow();

    int OnTouchBegin(int x, int y);
    void OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void draw();
    float getRelativeScrollHeight();
    float getRelativeScrollStartPos();
    void reInit();
    void update();
    void update(int unused);
};
#endif
