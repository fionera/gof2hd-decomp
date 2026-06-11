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

// Byte-offset field accessors used throughout StarMap's methods.

static inline void *&ptr_field(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;

class StarMap {
public:
    // Raw storage for the parts of the layout accessed by byte offset (everything except
    // the named scratch Vector below).
    char storage_0x0[0x78];             // +0x00 .. +0x78
    Vector scratchVector;                  // +0x78  scratch screen-space Vector
    char storage_0x84[0x200 - 0x84];    // +0x84 .. (object tail)

    StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4);
    ~StarMap();

    int init(bool jumpMapMode, Mission *mission, bool param3, int param4);
    void initLights();
    void initStarSystem();

    void render();
    void draw();
    void drawKey();
    void drawOnScreenInfo(int index, bool stationMode);

    void update(int dt);

    uint32_t OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);

    void depart(bool jump);
    void setStart(int start, int target);
    void setJumpMapMode(bool enabled, bool value);
    void askForJumpIntoAlienWorld();

    bool isInPlanetMode();
    uint8_t missionChanged();
};
#endif
