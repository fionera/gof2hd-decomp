#include "gof2/Radar.h"
#include "gof2/Layout.h"
#include "gof2/Station.h"


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
    return this->field_0x12c << 1;
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
    return this->field_0x1ab;
}

// ---- isPlasmaInRange_131056.cpp ----
uint8_t Radar::isPlasmaInRange()
{
    return this->field_0x130;
}

// ---- stationLocked_13103a.cpp ----
bool Radar::stationLocked()
{
    void *station = this->field_0x24;
    if (station != 0) {
        return station->field_0x71 != 0;
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
    this->field_0xc = 0;
}

// ---- getPlanetDockIndex_130974.cpp ----
int Radar::getPlanetDockIndex()
{
    SolarSystem *system = gStatus->getSystem();
    Array<Station *> *stations = system->getStations();
    return (int)stations->data[this->field_0x40];
}

// ---- draw_12e050.cpp ----
extern void *gRadarCanvasForDraw;
extern void *gRadarMissionSlot;

long long Radar::draw(Player *, Hud *, int mode)
{
    char scratch[0x120];
    (void)scratch;

    if (this->field_0x48 == 0) {
        return 0;
    }

    this->field_0x218 = 0;
    this->field_0x130 = 0;

    void *canvas = *(void **)gRadarCanvasForDraw;
    Radar_SetColor(canvas, -1);

    void *mission = *(void **)gRadarMissionSlot;
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        this->field_0x218 = (uint8_t)mode;
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

    this->field_0x174 = 0;
    this->field_0x178 = 0;
    this->field_0x17c = 0;
    this->field_0x180 = 0;
    this->field_0x164 = 0;
    this->field_0x168 = 0;
    this->field_0x16c = 0;
    this->field_0x170 = 0;
    this->field_0x154 = 0;
    this->field_0x158 = 0;
    this->field_0x15c = 0;
    this->field_0x160 = 0;

    Radar_StringDefault((char *)this + 0x18c);
    Radar_MatrixDefault((char *)this + 0x1d0);

    this->field_0x4 = 0;
    this->field_0x8 = 0;
    this->field_0x14 = 0;
    this->field_0x48 = 1;
    this->field_0x58 = 0;
    this->field_0x188 = 0;
    this->field_0xc = 0;
    this->field_0x10 = 0;
    this->field_0x18 = 0;
    this->field_0x38 = 0;
    this->field_0x3c = 0;
    this->field_0x40 = 0;
    this->field_0x20c = 0;
    this->field_0x1b4 = 0;
    this->field_0x1b8 = 0;
    this->field_0x1bc = 0;
    this->field_0x54 = 0;
    this->field_0x130 = 0;
    this->field_0x120 = 0;
    this->field_0x11c = 0;
    this->field_0x1a8 = 0;
    this->field_0x0 = level;

    void *layout = *(void **)gRadarLayoutSlot;
    if (layout != 0) {
        int width = layout->field_0xac;
        int height = layout->field_0xa8;
        this->field_0x21c = width;
        this->field_0x220 = width >> 1;
        this->field_0x224 = height;
        this->field_0x228 = height >> 1;
        this->field_0x22c = layout->field_0xa0;
        this->field_0x230 = layout->field_0xa4;
    }

    void *canvas = *(void **)gRadarCanvasSlot;
    Radar_Image2DCreate(canvas, 0x4c7, (char *)this + 0x1c4);
    int image = this->field_0x1c4;
    int imageWidth = Radar_GetImage2DWidth(canvas, image);
    int imageHeight = Radar_GetImage2DHeight(canvas, image);
    this->field_0x4c = imageWidth;
    this->field_0x50 = imageHeight;
    this->field_0x114 = imageWidth * imageWidth;
    this->field_0x118 = imageHeight * imageHeight;
    this->field_0x10c = 1.0f / (float)(imageWidth * imageWidth);
    this->field_0x110 = 1.0f / (float)(imageHeight * imageHeight);

    void *strings = Radar_operator_new(12);
    Radar_ArrayStringCtor(strings);
    this->field_0x188 = strings;
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
    int centerY = this->field_0x108;
    float dy = (float)(centerY - x);
    float dy2 = dy * dy;
    int centerX = this->field_0x104;
    float dx = (float)(centerX - y);
    float dx2 = dx * dx;
    float weightY = this->field_0x110;
    float weightX = this->field_0x10c;
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

    if (this->field_0x48 == 0) {
        return;
    }

    *gRadarDrawCurrentLockFlag = 1;

    if (this->field_0x14 == 0) {
        void *locked = this->field_0xc;
        if (locked == 0) {
            locked = this->field_0x38;
            if (locked == 0) {
                locked = this->field_0x4;
                if (locked == 0) {
                    locked = this->field_0x24;
                    if (locked == 0) {
                        *gRadarDrawCurrentLockFlag = 0;
                        return;
                    }
                }
            }
        }

        Radar_StringDefault(text40);
        if (locked == this->field_0x24) {
            Radar_StringAssign(text40, text34);
        }
        Radar_StringDtor(text40);
    } else {
        Radar_StringText(text34, this->field_0x18c, false);
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

    this->field_0x158 = -this->field_0x158;
    this->field_0x15c = -this->field_0x15c;

    int visible = Radar_GetScreenPosition(gPaintCanvas, positionStorage, positionStorage);
    this->field_0x11c = (uint8_t)visible;

    float *position = (float *)positionStorage;
    int screenX = (int)position[0];
    this->field_0xfc = screenX;
    int screenY = (int)position[1];
    this->field_0x100 = screenY;

    if (visible == 0) {
        Radar_elipsoidIntersect(transformed, this, screenX, screenY, *current);
        Radar_VectorAssign(current, transformed);
        this->field_0xfc = (int)current->x;
        this->field_0x100 = (int)current->y;
    }
}
