#ifndef GOF2_LISTITEMWINDOW_H
#define GOF2_LISTITEMWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

class AEGeometry;
class ListItem;
class ScrollTouchWindow;

class ListItemWindow {
public:
    Array<AbyssEngine::String *> *labels; // label-row strings (owns elements)
    Array<AbyssEngine::String *> *values; // value-row strings (owns elements)
    Array<int> *statsCur; // current ship-stat values
    Array<int> *statsPrev; // previous ship-stat values

    AEGeometry *previewGeometry; // rotating 3D ship preview
    ListItem *item; // the ListItem being displayed
    ScrollTouchWindow *scrollWindow; // inner scroll window hosting the rows
    int textHalfHeight; // half the canvas text height (row centring)
    int previewHeight; // height of the preview/progress region

    int scrollBarX; // +0x24 scrollbar thumb x
    int scrollBarY; // +0x28 scrollbar thumb baseline y
    int scrollBarOffsetX; // +0x2c scrollbar thumb x offset
    int scrollBarTrackLength; // +0x30 scrollbar track length

    uint32_t param2; // mode/colour parameter (from set())
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;

    int scrollThumbImage; // scrollbar thumb image id
    int arrowUpImage; // "trend up" arrow image id
    int arrowDownImage; // "trend down" arrow image id
    int arrowEqualImage; // "trend equal" arrow image id
    uint8_t shows3DShipFlag; // true => render the rotating preview

    int arrowSeparator; // trend-arrow separator width
    int x; // window x
    int y; // window y
    int width; // window width
    int height; // window height

    AbyssEngine::String str74; // window-scoped scratch string
    AbyssEngine::String str80; // window-scoped scratch string

    int previewSentinel; // -1 => no secondary transform pass
    AbyssEngine::AEMath::Matrix previewTransform; // preview world transform
    AbyssEngine::AEMath::Matrix previewTransform2; // preview secondary transform
    float baseAngle; // per-ship base preview angle
    float previewAngle; // current preview rotation angle (radians)
    int dragAccum; // accumulated drag angle (integer steps)
    int dragLastX; // last touch x captured for drag delta
    int dragSettled; // settled drag angle
    int dragDelta; // current drag delta
    float spinDamping; // spin-damping factor
    float spinVelocity; // spin velocity
    int dragStartX; // drag start x
    uint8_t dragging; // true while the preview is being spun

    ListItemWindow();

    ~ListItemWindow();

    void OnTouchBegin(int x, int y);

    void OnTouchMove(int x, int y);

    void OnTouchEnd(int x, int y);

    uint8_t shows3DShip();

    void render();

    void set(ListItem *item, unsigned p2, unsigned p3, unsigned p4, unsigned p5, bool p6);

    void draw();

    void update(int frameTime);
};

#endif
