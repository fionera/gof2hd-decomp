#include "class.h"

struct Station;
struct StarSystem;
struct SolarSystem;
struct Engine;
struct PaintCanvas;

__attribute__((visibility("hidden"))) extern int    *g_csp_stack;   // [DAT_000be974]
__attribute__((visibility("hidden"))) extern Status **g_csp_status;  // [DAT_000be978]
__attribute__((visibility("hidden"))) extern unsigned *g_csp_canvas;  // [DAT_000be97c]

extern "C" {
int  Status_inAlienOrbit_csp();
int  Status_inSupernovaSystem_csp();
int  Status_inPlanetRingOrbit_csp();
int  Status_inStormOrbit_csp(Status *s);
int  Status_inFogSkyboxOrbit_csp();
int  Status_inEmptyOrbit_csp(Status *s);
int  Status_dlc1Won_csp();
int  Status_getSystem_csp();
int  Status_getStation_csp();
int  Status_getCurrentCampaignMission_csp();
int  SolarSystem_getIndex_csp();
int  SolarSystem_getTextureIndex_csp();
int  Station_getIndex_csp(Station *s);
int  Station_isAttackedByAliens_csp(Station *s);
int  aeabi_idivmod_csp(int a, int b);
void PaintCanvas_MeshCreate_csp(PaintCanvas *c, int mesh, unsigned *out, int flag);
void PaintCanvas_TextureCreate_csp(unsigned c, unsigned tex, int flag);
void *Level_opnew_csp(unsigned size);
void StarSystem_ctor_csp(StarSystem *s, int mode);
void ArrayKIPlayer_ctor_csp(void *a);
void ArraySetLength_KIPlayer_csp(unsigned n, void *a);
int  ApplicationManager_GetEngine_csp();
int  Engine_IsPostEffectActivated_csp(Engine *e);
int  PaintCanvas_MeshGetPointer_csp(PaintCanvas *c, unsigned mesh);
// Builds the skybox detail meshes (rings/storm/supernova flare/jumpgates), the station object and
// the agent gallery, plus all the SIMD light-direction / jumpgate-placement math the decompiler
// mangled. Profile selects the orbit context.
void Level_csp_buildDetail(Level *self);
void Level_csp_buildStarSystemScene(Level *self);
void Level_csp_buildStationAndGates(Level *self);
}

// Level::createSpace() — creates the skybox + star-system backdrop, the home station and the
// jumpgate/agent props for the current orbit.
void Level::createSpace()
{
    char *self = (char *)this;

    // skybox mesh/texture only need (re)building when not yet created (mesh handle at +4 == -1).
    if (*(unsigned *)(self + 4) == 0xffffffff) {
        Status **status = g_csp_status;
        int alien = Status_inAlienOrbit_csp();
        unsigned canvas = *g_csp_canvas;

        if (alien == 0) {
            Status_getSystem_csp();
            int sysVariant = aeabi_idivmod_csp(SolarSystem_getIndex_csp(), 3);
            PaintCanvas_MeshCreate_csp((PaintCanvas *)canvas, sysVariant + 0x45ba,
                                       (unsigned *)(self + 8), 0);
            Status_getSystem_csp();
            sysVariant = aeabi_idivmod_csp(SolarSystem_getIndex_csp(), 3);
            PaintCanvas_TextureCreate_csp(canvas, (sysVariant + 0x2766) & 0xffff, 0);
            // the rest (campaign/supernova/storm/ring detail) is built by the helper.
            Level_csp_buildDetail(this);

            Status_getSystem_csp();
            if (0xf < SolarSystem_getTextureIndex_csp()) {
                Engine *eng = (Engine *)ApplicationManager_GetEngine_csp();
                if (Engine_IsPostEffectActivated_csp(eng) != 0) {
                    int mp = PaintCanvas_MeshGetPointer_csp((PaintCanvas *)canvas,
                                                            *(unsigned *)(self + 4));
                    *(int *)(mp + 0x1c) = 0;
                }
            }
        } else {
            PaintCanvas_MeshCreate_csp((PaintCanvas *)canvas, 0x45bc, (unsigned *)(self + 8), 0);
            PaintCanvas_TextureCreate_csp(canvas, 0x2768, 0);
            PaintCanvas_MeshCreate_csp((PaintCanvas *)canvas, 0x4592, (unsigned *)(self + 4), 0);
            PaintCanvas_TextureCreate_csp(canvas, 0x275b, 0);
        }

        // randomized skybox spin (light direction), unless in fog orbit.
        if (Status_inFogSkyboxOrbit_csp() == 0) {
            // (*status) used for storm check inside helper; spin written into self+0x1a4..0x1ac.
            (void)status;
            Level_csp_buildStarSystemScene(this);
        } else {
            *(int *)(self + 0x1a4) = 0;
            *(int *)(self + 0x1a8) = 0;
            *(int *)(self + 0x1ac) = 0;
        }
    }

    int mode = *(int *)(self + 0xc0);
    if (mode == 4 || mode == 0x17) {
        StarSystem *ss = (StarSystem *)Level_opnew_csp(0x60);
        StarSystem_ctor_csp(ss, mode);
        *(StarSystem **)(self + 0xec) = ss;
        Level_csp_buildStarSystemScene(this);
        return;
    }

    // build the home station + jumpgates.
    Status **status = g_csp_status;
    (void)status;
    void *players = Level_opnew_csp(0xc);
    ArrayKIPlayer_ctor_csp(players);
    *(void **)(self + 0x100) = players;
    ArraySetLength_KIPlayer_csp(4, players);

    Level_csp_buildStationAndGates(this);
}
