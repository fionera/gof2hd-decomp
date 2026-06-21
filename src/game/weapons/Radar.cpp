#include "game/weapons/Radar.h"
#include "engine/render/PaintCanvas.h"
#include "platform/libc.h"
#include "game/ship/KIPlayer.h"   // for the virtual getPosition() call

namespace AbyssEngine {

// Game singletons (defined in the host engine; referenced by pointer here).
extern PaintCanvas* gPaintCanvas;

// ---- engine-opaque state read by raw byte offset ---------------------------
// Layout and Station are modelled elsewhere; their fields are read positionally
// (pulling in their full headers triggers placement-new redefinitions vs the STL).
static inline int layout_i32(void* layout, unsigned off) { return *(int*)((char*)layout + off); }
static inline uint8_t station_u8(void* station, unsigned off) { return *(uint8_t*)((char*)station + off); }

// The radar's lock-target, mission and per-frame canvas state live in engine
// singletons addressed through these slots in the shipped binary.
extern PaintCanvas** gRadarCanvasForDraw;
extern void* gRadarMissionSlot;
extern PaintCanvas** gRadarCanvasSlot;
extern void* gRadarLayoutSlot;
extern uint8_t* gRadarDrawCurrentLockFlag;

extern "C" int Radar_GetMissionState(void* mission);
extern "C" int Radar_GetMissionType(void* mission);

// Planet-dock station lookup goes through the persistent game Status singleton,
// whose accessors return id-typed views (kept opaque here to avoid the
// placement-new clash described above).
extern void* Radar_GetSystemStations();

int Radar::getTurretScopeWidth()
{
    return this->turretScopeHalfWidth << 1;
}

Radar::Radar(Level* level)
{
    this->field_0x174 = 0;
    this->field_0x178 = 0;
    this->field_0x17c = 0;
    this->field_0x180 = 0;
    this->field_0x164 = 0;
    this->field_0x168 = 0;
    this->field_0x16c = 0;
    this->field_0x170 = 0;
    this->radarPosX = 0;
    this->radarPosY = 0;
    this->radarPosZ = 0;
    this->field_0x160 = 0;

    this->transform = AbyssEngine::AEMath::Matrix();

    this->field_0x4 = 0;
    this->field_0x8 = 0;
    this->field_0x14 = 0;
    this->enabled = 1;
    this->field_0x58 = 0;
    this->labelStrings = 0;
    this->lockedAsteroid = 0;
    this->field_0x10 = 0;
    this->field_0x18 = 0;
    this->field_0x38 = 0;
    this->field_0x3c = 0;
    this->planetDockIndex = 0;
    this->field_0x20c = 0;
    this->field_0x1b4 = 0;
    this->field_0x1b8 = 0;
    this->field_0x1bc = 0;
    this->field_0x54 = 0;
    this->plasmaInRange = 0;
    this->field_0x120 = 0;
    this->onScreen = 0;
    this->field_0x1a8 = 0;
    this->level = level;

    void* layout = *(void**)gRadarLayoutSlot;
    if (layout != 0) {
        int width = layout_i32(layout, 0xac);
        int height = layout_i32(layout, 0xa8);
        this->screenWidth = width;
        this->halfScreenWidth = width >> 1;
        this->screenHeight = height;
        this->halfScreenHeight = height >> 1;
        this->originX = layout_i32(layout, 0xa0);
        this->originY = layout_i32(layout, 0xa4);
    }

    PaintCanvas* canvas = *gRadarCanvasSlot;
    unsigned int image = 0;
    canvas->Image2DCreate(0x4c7, image);
    this->radarImage = (int)image;
    int imageWidth = canvas->GetImage2DWidth(image);
    int imageHeight = canvas->GetImage2DHeight(image);
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->imageWidthSq = imageWidth * imageWidth;
    this->imageHeightSq = imageHeight * imageHeight;
    this->weightX = 1.0f / (float)(imageWidth * imageWidth);
    this->weightY = 1.0f / (float)(imageHeight * imageHeight);

    Array<String*>* strings = new Array<String*>();
    ArraySetLength<String*>(4, *strings);
    this->labelStrings = strings;
}

Radar::~Radar()
{
    Array<KIPlayer*>* players = this->players;
    if (players != 0) {
        delete players;
    }
    this->players = 0;
    this->lockLabel.~String();
}

uint8_t Radar::hasScanner()
{
    return this->scannerAvailable;
}

uint8_t Radar::isPlasmaInRange()
{
    return this->plasmaInRange;
}

bool Radar::stationLocked()
{
    void* station = this->lockedStation;
    if (station != 0) {
        return station_u8(station, 0x71) != 0;
    }
    return false;
}

void Radar::unlockAsteroid()
{
    this->lockedAsteroid = 0;
}

int Radar::getPlanetDockIndex()
{
    void* stations = Radar_GetSystemStations();
    return ((int*)stations)[this->planetDockIndex];
}

void Radar::update(KIPlayer* player)
{
    AEMath::Vector position = player->getPosition();   // actor vtable slot 0x28
    update(position);
}

void Radar::update(AEMath::Vector value)
{
    AEMath::Vector transformed = AEMath::MatrixTransformVector(this->transform, value);
    AEMath::Vector* current = (AEMath::Vector*)&this->radarPosX;
    *current = transformed;

    this->radarPosY = -this->radarPosY;
    this->radarPosZ = -this->radarPosZ;

    AEMath::Vector screen = value;
    // NOTE: in the shipped binary (Radar::update @ 0013dec0) GetScreenPosition
    // RETURNS the visibility flag (iVar4), which is stored into onScreen and
    // gates the elipsoidIntersect fallback. The corrected PaintCanvas signature
    // declares this overload `void`, so the flag can no longer be recovered at
    // the call site. Fixing this properly requires PaintCanvas::GetScreenPosition
    // to return int (another file). Arguments corrected (refs, drop &) here.
    int visible = (gPaintCanvas->GetScreenPosition(value, screen) != 0);
    this->onScreen = (uint8_t)visible;

    int screenX = (int)screen.x;
    this->screenX = screenX;
    int screenY = (int)screen.y;
    this->screenY = screenY;

    if (visible == 0) {
        *current = this->elipsoidIntersect(screenX, screenY, *current);
        this->screenX = (int)current->x;
        this->screenY = (int)current->y;
    }
}

long long Radar::draw(Player*, Hud*, int mode)
{
    if (this->enabled == 0) {
        return 0;
    }

    this->drawMode = 0;
    this->plasmaInRange = 0;

    PaintCanvas* canvas = *gRadarCanvasForDraw;
    canvas->SetColor((unsigned int)-1);

    void* mission = *(void**)gRadarMissionSlot;
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        this->drawMode = (uint8_t)mode;
    }

    return 0;
}

AEMath::Vector Radar::elipsoidIntersect(int y, int x, AEMath::Vector value)
{
    int centerY = this->centerY;
    float dy = (float)(centerY - x);
    float dy2 = dy * dy;
    int centerX = this->centerX;
    float dx = (float)(centerX - y);
    float dx2 = dx * dx;
    float weightY = this->weightY;
    float weightX = this->weightX;
    float distance = weightY * dy2 + weightX * dx2;

    if (distance >= 0.0f) {
        float scale = (distance - std::sqrt(distance)) / distance;
        if (scale >= 0.0f && scale <= 1.0f) {
            value.y = (float)(int)((float)x + scale * dy);
            value.x = (float)(int)((float)y + scale * dx);
        }
    }

    return value;
}

void Radar::drawCurrentLock(Hud*)
{
    if (this->enabled == 0) {
        return;
    }

    *gRadarDrawCurrentLockFlag = 1;

    String label;

    if (this->field_0x14 == 0) {
        void* locked = this->lockedAsteroid;
        if (locked == 0) {
            locked = this->field_0x38;
            if (locked == 0) {
                locked = this->field_0x4;
                if (locked == 0) {
                    locked = this->lockedStation;
                    if (locked == 0) {
                        *gRadarDrawCurrentLockFlag = 0;
                        return;
                    }
                }
            }
        }
    } else {
        label = this->lockLabel;
    }
}

void Radar::calcDistance(float, float a, float b, float c, float d, float e)
{
    long long dx = (long long)(a * 0.5f - d * 0.5f);
    long long dy = (long long)(c * 0.5f - e * 0.5f);
    long long dz = (long long)(b * 0.5f - e * 0.5f);
    long long total = dy * dy + dx * dx + dz * dz;
    int distance = (int)std::sqrt((float)total * 0.001f);
    int meters = distance << 3;

    String text;
    text.Set_longlong(meters);

    if (distance >= 125) {
        int remainder = meters % 1000;
        if (remainder < 100) {
            text.Set_char("");
        } else {
            text.Set_longlong(remainder);
        }

        String head;
        head.SubString(&text, 0, 1);
        text = head;

        unsigned kilometers = (unsigned)distance / 125u;
        String km;
        km.Set_longlong((long long)kilometers);
        text = km + text;
    }
}

} // namespace AbyssEngine

// ---- ABI shims -------------------------------------------------------------
// MGame owns its Radar by raw pointer (allocated with operator new), so it
// constructs and destroys it through these C entry points that wrap the real
// special members. The destructor returns the object pointer so the caller can
// hand it straight to operator delete (mirroring the binary's call sequence).
extern "C" void Radar_ctor(AbyssEngine::Radar* r, Level* level)
{
    new (r) AbyssEngine::Radar(level);
}

extern "C" void* Radar_dtor(AbyssEngine::Radar* r)
{
    r->~Radar();
    return r;
}
