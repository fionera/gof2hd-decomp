#ifndef GOF2_STARMAP_H
#define GOF2_STARMAP_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// StarMap -- top-level galaxy/system map screen (~0x1e8 bytes). The object holds many
// scalar/pointer fields and several engine sub-objects (cameras, AEGeometry trees, Arrays).
// The full layout is large and not individually modelled here; fields are accessed by
// byte offset via the field<T>()/ptr_field() helpers below (the documented convention for
// this class). The one embedded math object we name is the scratch Vector at +0x78.

struct Mission;
struct SystemPathFinder;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;

struct AEGeometry;
struct ChoiceWindow;
struct TouchButton;
struct Station;
struct SolarSystem;
namespace AbyssEngine { class EaseInOut; }

// =====================================================================================
// StarMap — top-level galaxy/system map screen (~0x1e8 bytes). Field offsets recovered
// from the target binary (android_2.0.16_libgof2hdaa.so). Members are declared in
// original-offset order; each carries a `// +0xNN` comment giving that recovered 32-bit
// offset. The native build uses natural 64-bit layout, so the byte-exact spacing no longer
// holds — only the order and the names are load-bearing. Fields whose meaning is not
// recoverable from the binary keep deterministic field_0xNN names.
// =====================================================================================
class StarMap {
public:
    uint8_t  exitRequested;          // +0x000  set to 1 to leave the screen (restore prev camera)
    uint8_t  field_0x01;             // +0x001  init-only flag
    uint8_t  pad_0x02[2];            // +0x002
    int32_t  mode;                   // +0x004  0 = galaxy map, 3 = star-system/planet view
    float    cameraBaseX;            // +0x008  camera pan base X (screen/20)
    float    cameraBaseZ;            // +0x00c  camera pan base Z (screen/20)
    int32_t  field_0x10;             // +0x010  (= 500)
    int32_t  field_0x14;             // +0x014  (= 500)
    int32_t  lastDt;                 // +0x018  last update() delta-time
    uint8_t  pad_0x1c[0x20 - 0x1c];  // +0x01c
    uint32_t keyImageRetreat;        // +0x020  legend image: jump (0x452)
    uint32_t keyImageWanted;         // +0x024  legend image (0x454)
    uint32_t keyImageMission;        // +0x028  legend image (0x455)
    uint32_t keyImageCurrent;        // +0x02c  legend image (0x453)
    uint32_t keyImageDiscovered;     // +0x030  legend image (0x4a2)
    uint32_t systemNameImage;        // +0x034  selected system name image
    uint8_t  pad_0x38[0x40 - 0x38];  // +0x038
    uint32_t selectIcon;             // +0x040  selection marker image (0x48c)
    uint32_t cursorIcon;             // +0x044  cursor marker image (0x48a)
    uint32_t currentMarkerIcon;      // +0x048  current-system marker (0x4fd)
    uint8_t  pad_0x4c_lo[0x4c - 0x4c];
    TouchButton *backButton;         // +0x04c
    SystemPathFinder *pathFinder;    // +0x050
    Array<SolarSystem *> *systems;   // +0x054  (Galaxy::getSystems())
    Array<Station *> *stations;      // +0x058  current-system stations
    ChoiceWindow *choiceWindow;      // +0x05c
    int32_t  selectedSystem;         // +0x060
    int32_t  selectedStation;        // +0x064
    Array<AEGeometry *> *systemGeoms;// +0x068  per-system 3D icons
    AEGeometry *systemRoot;          // +0x06c  galaxy-view root geometry
    uint32_t camera;                 // +0x070  star-map camera handle
    uint32_t prevCamera;             // +0x074  camera to restore on exit
    Vector   scratchVector;          // +0x078  scratch screen-space Vector (0x78..0x84)
    Vector   scratchVector2;         // +0x084  second scratch Vector (0x84..0x90)
    Array<AEGeometry *> *stationGeoms;// +0x090  per-station 3D icons (system view)
    Array<AEGeometry *> *ringGeoms;  // +0x094  orbit-ring geometries
    int     *stationAngles;          // +0x098  per-station orbit angle [int]
    int     *stationDistances;       // +0x09c  per-station orbit radius [int]
    Array<int> *systemPath;          // +0x0a0  jump-route system index path
    AEGeometry *starSystemRoot;      // +0x0a4  system-view root geometry
    uint8_t  pad_0xa8_a;             // +0x0a8  jump-map mode flag
    uint8_t  choiceVisible;          // +0x0a9  choice window shown
    uint8_t  jumpMapModeA;           // +0x0aa  (set by setJumpMapMode `enabled`)
    uint8_t  jumpMapModeB;           // +0x0ab  (set by setJumpMapMode `value`)
    Vector   field_0xac;             // +0x0ac  (vecCtor'd)
    Vector   field_0xbc;             // +0x0bc  (vecCtor'd)
    Vector   field_0xcc;             // +0x0cc  (vecCtor'd)
    uint8_t  pad_0xdc_lo[0xdc - 0xdc];
    uint8_t  missionChangedFlag;     // +0x0dc
    uint8_t  pad_0xdd[0xe8 - 0xdd];  // +0x0dd
    int32_t  field_0xe8;             // +0x0e8  (= 0)
    int32_t  field_0xec;             // +0x0ec  (= 0)
    uint8_t  pad_0xf0[0xf4 - 0xf0];  // +0x0f0
    uint8_t  isGalaxyMode;           // +0x0f4  campaign > 0xf
    uint8_t  pad_0xf5[0xf8 - 0xf5];  // +0x0f5
    AEGeometry *markerGeom;          // +0x0f8  optional jump-route overlay mesh
    int     *iconBuffer;             // +0x0fc  scratch int[5] info-icon image ids
    Array<uint8_t> *usedFlags;       // +0x100  per-station "used" flags
    int32_t  targetSystem;           // +0x104  jump-map target system index
    uint8_t  showKey;                // +0x108  legend visible
    uint8_t  pad_0x109[0x10c - 0x109];// +0x109
    int32_t  keyBoxWidth;            // +0x10c
    int32_t  keyBoxHeight;           // +0x110
    int32_t  pulseSystem;            // +0x114  param4 auto-select system index
    uint8_t  autoMode;               // +0x118  param3 (tutorial/auto)
    uint8_t  pad_0x119[0x11c - 0x119];// +0x119
    int32_t  autoTimer;              // +0x11c  auto-select countdown (ms)
    uint8_t  alienJumpPending;       // +0x120
    uint8_t  pad_0x121[0x124 - 0x121];// +0x121
    uint32_t raceImageNeutral;       // +0x124  (0x4a1)
    uint32_t raceImageA;             // +0x128  (0x49c)
    uint32_t raceImageB;             // +0x12c  (0x49f)
    uint32_t raceImageDefault;       // +0x130  (0x49e)
    uint32_t image_0x134;            // +0x134  (0x545)
    uint8_t  transitionIn;           // +0x138  zoom-into-system transition active
    uint8_t  transitionOut;          // +0x139  zoom-out-to-galaxy transition active
    uint8_t  pathAnim;               // +0x13a  jump-route auto-pan active
    uint8_t  stationCenterAnim;      // +0x13b  station-centering animation active
    int32_t  panX;                   // +0x13c  galaxy pan offset X
    int32_t  panY;                   // +0x140  galaxy pan offset Y
    float    touchStartX;            // +0x144
    float    touchStartY;            // +0x148
    float    field_0x14c;            // +0x14c
    float    touchDeltaX;            // +0x150
    float    touchDeltaY;            // +0x154
    float    field_0x158;            // +0x158
    float    lastTouchX;             // +0x15c
    float    lastTouchY;             // +0x160
    int32_t  field_0x164;            // +0x164  (= 0)
    float    momentumFactor;         // +0x168  drag-momentum decay (1.0f / 0.9f)
    float    velocityX;              // +0x16c
    float    velocityY;              // +0x170
    uint8_t  dragging;               // +0x174
    uint8_t  pad_0x175[0x178 - 0x175];// +0x175
    uint32_t planetTexture;          // +0x178
    AbyssEngine::EaseInOut *easeX;   // +0x17c
    AbyssEngine::EaseInOut *easeY;   // +0x180
    AbyssEngine::EaseInOut *easeZ;   // +0x184
    float    yaw;                    // +0x188  system-view yaw (fixed 16.16-ish)
    float    pitch;                  // +0x18c  system-view pitch
    int32_t  field_0x190;            // +0x190  (= 0)
    Array<Vector *> *systemPositions;// +0x194  per-system screen positions
    Array<Vector *> *stationPositions;//+0x198  per-station screen positions
    int32_t  lastSelectedSystem;     // +0x19c
    int32_t  lastSelectedStation;    // +0x1a0
    int32_t  alpha;                  // +0x1a4
    int32_t  iconWidth;              // +0x1a8
    int32_t  missionIconWidth;       // +0x1ac
    AEGeometry *bgLayer0;            // +0x1b0  (mesh 0x41d2)
    AEGeometry *bgLayer1;            // +0x1b4  (mesh 0x41d3)
    AEGeometry *bgLayer2;            // +0x1b8  (mesh 0x41d4)
    AEGeometry *planetGeom;          // +0x1bc  system-view planet mesh
    float    spin;                   // +0x1c0  star-icon spin accumulator
    int32_t  centeredStation;        // +0x1c4  station index being centered (-1 none)
    int32_t  hitRadius;              // +0x1c8  touch-pick radius (status field 0x88)
    float    dragScale;             // +0x1cc  layout drag-scale factor
    int32_t  jumpCost;               // +0x1d0
    uint8_t  noRoute;                // +0x1d4
    uint8_t  pad_0x1d5[0x1d8 - 0x1d5];// +0x1d5
    int32_t  cargoAmount;            // +0x1d8
    int32_t  routeStart;             // +0x1dc  (setStart target stored here)
    int32_t  routeTarget;            // +0x1e0  (setStart start stored here)
    uint8_t  suppressNextClose;      // +0x1e4
    uint8_t  pad_0x1e5[0x1e8 - 0x1e5];// +0x1e5

    StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4);
    ~StarMap();

    int init(bool jumpMapMode, Mission *mission, bool param3, int param4);
    void initLights();
    void initStarSystem();

    void render();
    void render_tail();     // render tail: draw the optional jump-route overlay geometry (+0xf8)
    void renderBG();
    void draw();
    void drawKey();
    void drawOnScreenInfo(int index, bool stationMode);

    void update(int dt);

    uint32_t OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    void OnTouchEnd_tail();          // OnTouchEnd tail: open the help overlay
    int  touch_end(int x, int y);    // embedded (lounge) touch-release; nonzero => map dismissed

    void depart(bool jump);
    void depart_tail(void *app, int moduleId); // depart tail: hand the application to module `moduleId`
    void setStart(int start, int target);
    void setJumpMapMode(bool enabled, bool value);
    void askForJumpIntoAlienWorld();

    bool isInPlanetMode();
    uint8_t missionChanged();
};
#endif
