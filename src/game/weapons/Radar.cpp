#include "game/weapons/Radar.h"
#include "engine/render/PaintCanvas.h"

#include "game/ship/KIPlayer.h"

namespace AbyssEngine {
    extern PaintCanvas *gPaintCanvas;

    extern PaintCanvas **gRadarCanvasForDraw;
    extern void *gRadarMissionSlot;
    extern PaintCanvas **gRadarCanvasSlot;
    extern void *gRadarLayoutSlot;
    extern uint8_t *gRadarDrawCurrentLockFlag;
}

extern "C" int Radar_GetMissionState(void *mission);

extern "C" int Radar_GetMissionType(void *mission);

extern void *Radar_GetSystemStations();

static inline int layout_i32(void *layout, unsigned off) {
    return *reinterpret_cast<int *>(static_cast<char *>(layout) + off);
}

static inline uint8_t station_u8(void *station, unsigned off) {
    return *reinterpret_cast<uint8_t *>(static_cast<char *>(station) + off);
}

int Radar::getTurretScopeWidth() {
    return this->turretScopeHalfWidth << 1;
}

Radar::Radar(Level *level) {
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

    this->transform = Matrix();

    this->lockedEnemy = nullptr;
    this->field_0x8 = nullptr;
    this->field_0x14 = 0;
    this->enabled = 1;
    this->field_0x58 = 0;
    this->labelStrings = nullptr;
    this->lockedAsteroid = nullptr;
    this->field_0x10 = 0;
    this->field_0x18 = 0;
    this->lockedGasCloud = nullptr;
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

    void *layout = *reinterpret_cast<void **>(gRadarLayoutSlot);
    if (layout != nullptr) {
        int width = layout_i32(layout, 0xac);
        int height = layout_i32(layout, 0xa8);
        this->screenWidth = width;
        this->halfScreenWidth = width >> 1;
        this->screenHeight = height;
        this->halfScreenHeight = height >> 1;
        this->originX = layout_i32(layout, 0xa0);
        this->originY = layout_i32(layout, 0xa4);
    }

    PaintCanvas *canvas = *gRadarCanvasSlot;
    unsigned int image = 0;
    canvas->Image2DCreate(0x4c7, image);
    this->radarImage = static_cast<int>(image);
    int imageWidth = canvas->GetImage2DWidth(image);
    int imageHeight = canvas->GetImage2DHeight(image);
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->imageWidthSq = imageWidth * imageWidth;
    this->imageHeightSq = imageHeight * imageHeight;
    this->weightX = 1.0f / static_cast<float>(imageWidth * imageWidth);
    this->weightY = 1.0f / static_cast<float>(imageHeight * imageHeight);

    Array<String *> *strings = new Array<String *>();
    ArraySetLength<String *>(4, *strings);
    this->labelStrings = strings;
}

Radar::~Radar() {
    Array<KIPlayer *> *players = this->players;
    if (players != nullptr) {
        delete players;
    }
    this->players = nullptr;
    this->lockLabel.~String();
}

uint8_t Radar::hasScanner() {
    return this->scannerAvailable;
}

uint8_t Radar::isPlasmaInRange() {
    return this->plasmaInRange;
}

bool Radar::stationLocked() {
    void *station = this->lockedStation;
    if (station != nullptr) {
        return station_u8(station, 0x71) != 0;
    }
    return false;
}

KIPlayer *Radar::getLockedEnemy() {
    return this->lockedEnemy;
}

KIPlayer *Radar::getLockedAsteroid() {
    return this->lockedAsteroid;
}

KIPlayer *Radar::getLockedGasCloud() {
    return this->lockedGasCloud;
}

void Radar::unlockAsteroid() {
    this->lockedAsteroid = nullptr;
}

int Radar::getPlanetDockIndex() {
    void *stations = Radar_GetSystemStations();
    return reinterpret_cast<int *>(stations)[this->planetDockIndex];
}

void Radar::update(KIPlayer *player) {
    Vector position = player->getPosition();
    update(position);
}

void Radar::update(Vector value) {
    Vector transformed = AbyssEngine::AEMath::MatrixTransformVector(this->transform, value);
    Vector *current = reinterpret_cast<Vector *>(&this->radarPosX);
    *current = transformed;

    this->radarPosY = -this->radarPosY;
    this->radarPosZ = -this->radarPosZ;

    Vector screen = value;

    int visible = (gPaintCanvas->GetScreenPosition(value, screen) != 0);
    this->onScreen = static_cast<uint8_t>(visible);

    int screenX = static_cast<int>(screen.x);
    this->screenX = screenX;
    int screenY = static_cast<int>(screen.y);
    this->screenY = screenY;

    if (visible == 0) {
        *current = this->elipsoidIntersect(screenX, screenY, *current);
        this->screenX = static_cast<int>(current->x);
        this->screenY = static_cast<int>(current->y);
    }
}

long long Radar::draw(Player *, Hud *, int mode) {
    if (this->enabled == 0) {
        return 0;
    }

    this->drawMode = 0;
    this->plasmaInRange = 0;

    PaintCanvas *canvas = *gRadarCanvasForDraw;
    canvas->SetColor(static_cast<unsigned int>(-1));

    void *mission = *reinterpret_cast<void **>(gRadarMissionSlot);
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        this->drawMode = static_cast<uint8_t>(mode);
    }

    return 0;
}

Vector Radar::elipsoidIntersect(int y, int x, Vector value) {
    int centerY = this->centerY;
    float dy = static_cast<float>(centerY - x);
    float dy2 = dy * dy;
    int centerX = this->centerX;
    float dx = static_cast<float>(centerX - y);
    float dx2 = dx * dx;
    float weightY = this->weightY;
    float weightX = this->weightX;
    float distance = weightY * dy2 + weightX * dx2;

    if (distance >= 0.0f) {
        float scale = (distance - std::sqrt(distance)) / distance;
        if (scale >= 0.0f && scale <= 1.0f) {
            value.y = static_cast<float>(static_cast<int>(static_cast<float>(x) + scale * dy));
            value.x = static_cast<float>(static_cast<int>(static_cast<float>(y) + scale * dx));
        }
    }

    return value;
}

void Radar::drawCurrentLock(Hud *) {
    if (this->enabled == 0) {
        return;
    }

    *gRadarDrawCurrentLockFlag = 1;

    String label;

    if (this->field_0x14 == 0) {
        void *locked = this->lockedAsteroid;
        if (locked == nullptr) {
            locked = this->lockedGasCloud;
            if (locked == nullptr) {
                locked = this->lockedEnemy;
                if (locked == nullptr) {
                    locked = this->lockedStation;
                    if (locked == nullptr) {
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

void Radar::calcDistance(float, float a, float b, float c, float d, float e) {
    long long dx = static_cast<long long>(a * 0.5f - d * 0.5f);
    long long dy = static_cast<long long>(c * 0.5f - e * 0.5f);
    long long dz = static_cast<long long>(b * 0.5f - e * 0.5f);
    long long total = dy * dy + dx * dx + dz * dz;
    int distance = static_cast<int>(std::sqrt(static_cast<float>(total) * 0.001f));
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

        unsigned kilometers = static_cast<unsigned>(distance) / 125u;
        String km;
        km.Set_longlong(static_cast<long long>(kilometers));
        text = km + text;
    }
}

extern "C" void Radar_ctor(Radar *r, Level *level) {
    new(r) Radar(level);
}

extern "C" void *Radar_dtor(Radar *r) {
    r->~Radar();
    return r;
}
