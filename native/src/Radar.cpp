#include "Radar.h"


extern "C" void Radar_SetColor(void *canvas, int color);
extern "C" int Radar_GetMissionState(void *mission);
extern "C" int Radar_GetMissionType(void *mission);
extern "C" void Radar_StringDefault(void *self);
extern "C" void Radar_MatrixDefault(void *self);
extern "C" void *Radar_operator_new(unsigned size);
extern "C" void Radar_ArrayStringCtor(void *self);
extern "C" void Radar_ArraySetLengthString(int length, void *self);
extern "C" void Radar_Image2DCreate(void *canvas, int id, void *out);
extern "C" int Radar_GetImage2DWidth(void *canvas, int image);
extern "C" int Radar_GetImage2DHeight(void *canvas, int image);
extern "C" void Radar_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void Radar_StringText(void *self, void const *text, bool copy);
extern "C" void Radar_StringAssign(void *self, void const *rhs);
extern "C" void Radar_StringDtor(void *self);
extern "C" void Radar_StringInt(void *self, int value);
extern "C" void Radar_StringText(void *self, char const *text, bool copy);
extern "C" void Radar_StringPlus(void *out, void const *lhs, void const *rhs);
extern "C" void Radar_StringSubString(void *out, void const *self, int start, int count);
extern "C" int __aeabi_idivmod(int numerator, int denominator);
extern "C" unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator);
extern "C" void Radar_MatrixTransformVector(void *out, Matrix const *matrix, void const *value);
extern "C" void Radar_VectorAssign(Vector *dst, void const *src);
extern "C" int Radar_GetScreenPosition(AbyssEngine::PaintCanvas *canvas, void const *value, void *screen);
extern "C" void Radar_elipsoidIntersect(void *out, Radar *self, int x, int y, Vector value);

// ---- getTurretScopeWidth_13104e.cpp ----
int Radar::getTurretScopeWidth()
{
    return F<int>(this, 0x12c) << 1;
}

// ---- _Radar_12de3c.cpp ----
Radar::~Radar()
{
    Array<KIPlayer *> *players = F<Array<KIPlayer *> *>(this, 0x34);
    if (players != 0) {
        players->~Array<KIPlayer *>();
        operator delete(players);
    }
    F<Array<KIPlayer *> *>(this, 0x34) = 0;
    ((AbyssEngine::String *)((char *)this + 0x18c))->~String();
}

// ---- hasScanner_13105c.cpp ----
uint8_t Radar::hasScanner()
{
    return F<uint8_t>(this, 0x1ab);
}

// ---- isPlasmaInRange_131056.cpp ----
uint8_t Radar::isPlasmaInRange()
{
    return F<uint8_t>(this, 0x130);
}

// ---- stationLocked_13103a.cpp ----
bool Radar::stationLocked()
{
    void *station = F<void *>(this, 0x24);
    if (station != 0) {
        return F<uint8_t>(station, 0x71) != 0;
    }
    return false;
}

// ---- update_12e01c.cpp ----
void Radar::update(KIPlayer *player)
{
    char position[sizeof(AbyssEngine::AEMath::Vector)];
    typedef void (*GetPosition)(void *, KIPlayer *);
    void **vtable = *(void ***)player;
    ((GetPosition)vtable[0x28 / 4])(position, player);
    update(*(AbyssEngine::AEMath::Vector *)position);
}

// ---- unlockAsteroid_131030.cpp ----
void Radar::unlockAsteroid()
{
    F<int>(this, 0x0c) = 0;
}

// ---- getPlanetDockIndex_130974.cpp ----
int Radar::getPlanetDockIndex()
{
    SolarSystem *system = gStatus->getSystem();
    Array<Station *> *stations = system->getStations();
    return (int)stations->data[F<int>(this, 0x40)];
}

// ---- draw_12e050.cpp ----
extern void *gRadarCanvasForDraw;
extern void *gRadarMissionSlot;

long long Radar::draw(Player *, Hud *, int mode)
{
    char scratch[0x120];
    (void)scratch;

    if (F<uint8_t>(this, 0x48) == 0) {
        return 0;
    }

    F<uint8_t>(this, 0x218) = 0;
    F<uint8_t>(this, 0x130) = 0;

    void *canvas = *(void **)gRadarCanvasForDraw;
    Radar_SetColor(canvas, -1);

    void *mission = *(void **)gRadarMissionSlot;
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        F<uint8_t>(this, 0x218) = (uint8_t)mode;
    }

    return 0;
}

// ---- Radar_12d484.cpp ----
extern void *gRadarCanvasSlot;
extern void *gRadarLayoutSlot;

Radar::Radar(Level *level)
{
    char text6c[12];
    char local60[12];
    char text54[12];
    char local48[12];

    F<int>(this, 0x174) = 0;
    F<int>(this, 0x178) = 0;
    F<int>(this, 0x17c) = 0;
    F<int>(this, 0x180) = 0;
    F<int>(this, 0x164) = 0;
    F<int>(this, 0x168) = 0;
    F<int>(this, 0x16c) = 0;
    F<int>(this, 0x170) = 0;
    F<int>(this, 0x154) = 0;
    F<int>(this, 0x158) = 0;
    F<int>(this, 0x15c) = 0;
    F<int>(this, 0x160) = 0;

    Radar_StringDefault((char *)this + 0x18c);
    Radar_MatrixDefault((char *)this + 0x1d0);

    F<int>(this, 0x04) = 0;
    F<int>(this, 0x08) = 0;
    F<int>(this, 0x14) = 0;
    F<uint8_t>(this, 0x48) = 1;
    F<int>(this, 0x58) = 0;
    F<int>(this, 0x188) = 0;
    F<int>(this, 0x0c) = 0;
    F<int>(this, 0x10) = 0;
    F<int>(this, 0x18) = 0;
    F<int>(this, 0x38) = 0;
    F<int>(this, 0x3c) = 0;
    F<int>(this, 0x40) = 0;
    F<int>(this, 0x20c) = 0;
    F<int>(this, 0x1b4) = 0;
    F<int>(this, 0x1b8) = 0;
    F<int>(this, 0x1bc) = 0;
    F<uint8_t>(this, 0x54) = 0;
    F<uint8_t>(this, 0x130) = 0;
    F<uint8_t>(this, 0x120) = 0;
    F<int>(this, 0x11c) = 0;
    F<uint8_t>(this, 0x1a8) = 0;
    F<Level *>(this, 0x00) = level;

    void *layout = *(void **)gRadarLayoutSlot;
    if (layout != 0) {
        int width = F<int>(layout, 0xac);
        int height = F<int>(layout, 0xa8);
        F<int>(this, 0x21c) = width;
        F<int>(this, 0x220) = width >> 1;
        F<int>(this, 0x224) = height;
        F<int>(this, 0x228) = height >> 1;
        F<int>(this, 0x22c) = F<int>(layout, 0xa0);
        F<int>(this, 0x230) = F<int>(layout, 0xa4);
    }

    void *canvas = *(void **)gRadarCanvasSlot;
    Radar_Image2DCreate(canvas, 0x4c7, (char *)this + 0x1c4);
    int image = F<int>(this, 0x1c4);
    int imageWidth = Radar_GetImage2DWidth(canvas, image);
    int imageHeight = Radar_GetImage2DHeight(canvas, image);
    F<int>(this, 0x4c) = imageWidth;
    F<int>(this, 0x50) = imageHeight;
    F<int>(this, 0x114) = imageWidth * imageWidth;
    F<int>(this, 0x118) = imageHeight * imageHeight;
    F<float>(this, 0x10c) = 1.0f / (float)(imageWidth * imageWidth);
    F<float>(this, 0x110) = 1.0f / (float)(imageHeight * imageHeight);

    void *strings = Radar_operator_new(12);
    Radar_ArrayStringCtor(strings);
    F<void *>(this, 0x188) = strings;
    Radar_ArraySetLengthString(4, strings);

    (void)text6c;
    (void)local60;
    (void)text54;
    (void)local48;
}

// ---- elipsoidIntersect_12df48.cpp ----
AbyssEngine::AEMath::Vector Radar::elipsoidIntersect(
    int y, int x, AbyssEngine::AEMath::Vector value)
{
    int centerY = *(volatile int *)((char *)this + 0x108);
    float dy = (float)(centerY - x);
    float dy2 = dy * dy;
    int centerX = *(volatile int *)((char *)this + 0x104);
    float dx = (float)(centerX - y);
    float dx2 = dx * dx;
    float weightY = *(volatile float *)((char *)this + 0x110);
    float weightX = *(volatile float *)((char *)this + 0x10c);
    float distance = weightY * dy2 + weightX * dx2;

    if (distance >= 0.0f) {
        float scale = (distance - gGlobals->sqrt(distance)) / distance;
        if (scale >= 0.0f && scale <= 1.0f) {
            value.y = (float)(int)((float)x + scale * dy);
            value.x = (float)(int)((float)y + scale * dx);
        }
    }

    return value;
}

// ---- drawCurrentLock_130998.cpp ----
extern uint8_t *gRadarDrawCurrentLockFlag;

void Radar::drawCurrentLock(Hud *)
{
    char text34[12];
    char text40[12];
    char text4c[12];
    char text58[12];
    char text64[12];
    char text70[12];
    char text7c[12];
    char text88[12];

    if (F<uint8_t>(this, 0x48) == 0) {
        return;
    }

    *gRadarDrawCurrentLockFlag = 1;

    if (F<int>(this, 0x14) == 0) {
        void *locked = F<void *>(this, 0x0c);
        if (locked == 0) {
            locked = F<void *>(this, 0x38);
            if (locked == 0) {
                locked = F<void *>(this, 0x04);
                if (locked == 0) {
                    locked = F<void *>(this, 0x24);
                    if (locked == 0) {
                        *gRadarDrawCurrentLockFlag = 0;
                        return;
                    }
                }
            }
        }

        Radar_StringDefault(text40);
        if (locked == F<void *>(this, 0x24)) {
            Radar_StringAssign(text40, text34);
        }
        Radar_StringDtor(text40);
    } else {
        Radar_StringText(text34, F<void *>(this, 0x18c), false);
    }

    Radar_StringDtor(text34);
    (void)text4c;
    (void)text58;
    (void)text64;
    (void)text70;
    (void)text7c;
    (void)text88;
}

// ---- calcDistance_130704.cpp ----
static const float kDistanceScale = 0.001f;
static const char kMetersSuffix[] = "";
static const char kLeadingZero[] = "";
static const char kKmSuffix[] = "";
static const char kSpace[] = "";

void Radar::calcDistance(float, float a, float b, float c, float d, float e)
{
    char s84[12];
    char s78[12];
    char s6c[12];
    char s60[12];
    char s54[12];
    char s48[12];

    float extra = e;
    long long dx = (long long)(a * 0.5f - d * 0.5f);
    long long dy = (long long)(c * 0.5f - extra * 0.5f);
    long long dz = (long long)(b * 0.5f - e * 0.5f);
    long long total = dy * dy + dx * dx + dz * dz;
    int distance = (int)gGlobals->sqrt((float)total * kDistanceScale);
    int meters = distance << 3;
    void *out = this;

    Radar_StringInt(s48, meters);
    Radar_StringText(s54, kMetersSuffix, false);
    Radar_StringPlus(out, s48, s54);
    Radar_StringDtor(s54);
    Radar_StringDtor(s48);

    if (distance >= 125) {
        int remainder = meters % 1000;
        if (remainder < 100) {
            Radar_StringText(s48, kLeadingZero, false);
            Radar_StringAssign(out, s48);
        } else {
            Radar_StringInt(s48, remainder);
            Radar_StringAssign(out, s48);
        }
        Radar_StringDtor(s48);

        Radar_StringSubString(s48, out, 0, 1);
        Radar_StringAssign(out, s48);
        Radar_StringDtor(s48);

        unsigned kilometers = __aeabi_uidiv((unsigned)distance, 125u);
        Radar_StringInt(s6c, (int)kilometers);
        Radar_StringText(s78, kKmSuffix, false);
        Radar_StringPlus(s60, s6c, s78);
        Radar_StringPlus(s54, s60, out);
        Radar_StringText(s84, kSpace, false);
        Radar_StringPlus(s48, s54, s84);
        Radar_StringAssign(out, s48);

        Radar_StringDtor(s48);
        Radar_StringDtor(s84);
        Radar_StringDtor(s54);
        Radar_StringDtor(s60);
        Radar_StringDtor(s78);
        Radar_StringDtor(s6c);
    }
}

// ---- update_12de60.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;


void Radar::update(Vector value)
{
    Vector transformedValue;
    void *transformed = &transformedValue;
    char positionStorage[12];
    __builtin_memcpy(positionStorage, &value, 12);

    Radar_MatrixTransformVector(transformed, (Matrix const *)((char *)this + 0x1d0), positionStorage);
    Vector *current = (Vector *)((char *)this + 0x154);
    Radar_VectorAssign(current, transformed);

    F<float>(this, 0x158) = -F<float>(this, 0x158);
    F<float>(this, 0x15c) = -F<float>(this, 0x15c);

    int visible = Radar_GetScreenPosition(gPaintCanvas, positionStorage, positionStorage);
    F<uint8_t>(this, 0x11c) = (uint8_t)visible;

    float *position = (float *)positionStorage;
    int screenX = (int)position[0];
    F<int>(this, 0xfc) = screenX;
    int screenY = (int)position[1];
    F<int>(this, 0x100) = screenY;

    if (visible == 0) {
        Radar_elipsoidIntersect(transformed, this, screenX, screenY, *current);
        Radar_VectorAssign(current, transformed);
        F<int>(this, 0xfc) = (int)current->x;
        F<int>(this, 0x100) = (int)current->y;
    }
}
