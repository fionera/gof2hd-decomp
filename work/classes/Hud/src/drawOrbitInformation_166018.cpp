#include "class.h"

// Hud::drawOrbitInformation() — when not in an alien orbit, draws the current station name and
// (once past the early campaign) the system name plus a security-level label colored from a
// small RGB table. r0=this.
extern "C" int  Status_inAlienOrbit();
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_SetColor4(void *canvas, int b1, int b2, int b3, int b4);
extern "C" int  PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, int anchor);
extern "C" void PaintCanvas_DrawString2(void *canvas, void *font, void *str, int x, char y);
extern "C" void *Status_getSystem();
extern "C" int  SolarSystem_hasNoOwner(void *sys);
extern "C" void *Status_getStation();
extern "C" void Station_getName(void *out, void *station);
extern "C" int  Status_getCurrentCampaignMission();
extern "C" int  SolarSystem_getSecurityLevel(void *sys);
extern "C" int  SolarSystem_getIndex(void *sys);
extern "C" void SolarSystem_getName(void *out, void *sys);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_ctor_copy(void *s, void *src, bool b);
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" void String_dtor(void *s);
extern "C" void *GameText_getText(void *gt, int id);

__attribute__((visibility("hidden"))) extern void **g_Hud_oiCanvas;  // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_oiLayout;  // *holder -> layout (+0x21c..+0x228)
__attribute__((visibility("hidden"))) extern void **g_Hud_oiStatus;  // *holder -> status (+0x114)
__attribute__((visibility("hidden"))) extern void **g_Hud_oiFont;    // *holder -> font String
__attribute__((visibility("hidden"))) extern void **g_Hud_oiGameText;// *holder -> GameText
extern const char g_Hud_oiSep[]    __attribute__((visibility("hidden")));
extern const unsigned char g_Hud_secColors[] __attribute__((visibility("hidden"))); // 4 x 12-byte RGB rows

extern "C" void Hud_drawOrbitInformation(Hud *self)
{
    if (Status_inAlienOrbit() != 0) return;

    void *canvas = *g_Hud_oiCanvas;
    int *layout = (int *)*g_Hud_oiLayout;
    PaintCanvas_SetColor(canvas, -1);
    int x = PaintCanvas_GetImage2DWidth(canvas) + layout[0x87]; // +0x21c

    Status_getSystem();
    if (SolarSystem_hasNoOwner(Status_getSystem()) == 0)
        PaintCanvas_DrawImage2D2(canvas, I(self, 0x1c4), 3);

    void *font = *g_Hud_oiFont;
    // station name
    {
        char name[12];
        Status_getStation();
        Station_getName(name, Status_getStation());
        PaintCanvas_DrawString2(canvas, font, name, x, (char)layout[0x88] /*+0x220*/);
        String_dtor(name);
    }
    PaintCanvas_SetColor(canvas, -1);

    if (Status_getCurrentCampaignMission() <= 0xf) return;

    void *sys = Status_getSystem();
    int sec = SolarSystem_getSecurityLevel(sys);
    Status_getSystem();
    int idx = SolarSystem_getIndex(Status_getSystem());
    int *status = (int *)*g_Hud_oiStatus;
    if (idx == 0x1a && status[0x45] /*+0x114*/ > 1) sec = 3;

    // system name + separator
    {
        char sysName[12], copy[12], sep[12], acc[12], full[12];
        Status_getSystem();
        SolarSystem_getName(sysName, Status_getSystem());
        String_ctor_copy(copy, sysName, false);
        String_ctor_cstr(sep, g_Hud_oiSep, false);
        String_concat(acc, copy, sep);
        void *txt = GameText_getText(*g_Hud_oiGameText, 0); // id resolved by table
        String_concat(full, acc, txt);
        PaintCanvas_DrawString2(canvas, font, full, x, (char)layout[0x89] /*+0x224*/);
        String_dtor(full);
        String_dtor(acc);
        String_dtor(sep);
        String_dtor(copy);
        String_dtor(sysName);
    }

    const unsigned char *row = g_Hud_secColors + sec * 0xc;
    PaintCanvas_SetColor4(canvas, row[0], row[4], row[8], 0xff);
    void *secTxt = GameText_getText(*g_Hud_oiGameText, sec);
    PaintCanvas_DrawString2(canvas, font, secTxt, x, (char)layout[0x8a] /*+0x228*/);
}
