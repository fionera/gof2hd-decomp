#include "class.h"

extern "C" void Radar_StringInt(void *self, int value);
extern "C" void Radar_StringText(void *self, char const *text, bool copy);
extern "C" void Radar_StringDtor(void *self);
extern "C" void Radar_StringPlus(void *out, void const *lhs, void const *rhs);
extern "C" void Radar_StringAssign(void *self, void const *rhs);
extern "C" void Radar_StringSubString(void *out, void const *self, int start, int count);
extern "C" int __aeabi_idivmod(int numerator, int denominator);
extern "C" unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator);

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
