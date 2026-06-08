#include "class.h"

// PlayerEgo::setTurretMode(bool enable)
//   Enters or leaves the gun-turret view. Refused unless the ship mounts a
//   turret (0x170) and is neither mining (0x1e4) nor mid-cutscene (0x180); a
//   pending maneuver (0x194) instead just restores the default camera. When
//   enabling it lazily builds the turret camera rig (camera + AEGeometry chain,
//   like the docking camera), aims it, and makes it current; when disabling it
//   stops shooting and resets the camera. Either way it toggles the turret
//   crosshair transform (0x30) and starts/stops the turret hum sound.

extern "C" void  PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
extern "C" void  LevelScript_resetCamera(void *level);
extern "C" void  PlayerEgo_stopShooting(PlayerEgo *self);
extern "C" void  Player_stopShooting(void *player);
extern "C" void  PaintCanvas_CameraCreate(void *canvas, unsigned int *out);
extern "C" int   Status_inAlienOrbit();
extern "C" void  PaintCanvas_CameraSetPerspective(unsigned int cam, float fov, float a, float b);
extern "C" void *AEGeometry_new(void *canvas);
extern "C" void  AEGeometry_setRotationOrder(void *geo, int order);
extern "C" void  AEGeometry_translate(void *geo, const void *vec);
extern "C" void  AEGeometry_addChild(void *parent, void *child);
extern "C" void  AEGeometry_rotate(void *geo, const void *vec);
extern "C" void  AEGeometry_setPosition(void *geo, const void *vec);
extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  AEGeometry_setMatrix(void *geo, const void *m);
extern "C" void *Status_getShip();
extern "C" int   Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" void *PaintCanvas_TransformGetTransform(unsigned int handle);
extern "C" int   Transform_SetVisible(void *tf, bool vis);
extern "C" void  FModSound_stop(int snd);
extern "C" int   FModSound_play(int snd, int id, void *a, int b);

__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasA;   // 0xaadbc
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasB;   // 0xaadc0
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasC;   // 0xaadc4
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasD;   // 0xaadc8
__attribute__((visibility("hidden"))) extern void **g_PE_tm_canvasE;   // 0xaadcc
__attribute__((visibility("hidden"))) extern void **g_PE_tm_transform; // 0xaadd0
__attribute__((visibility("hidden"))) extern int  *g_PE_tm_hum;        // 0xaadd4
extern const float g_PE_tm_fovNormal;  // 0xaad9c
extern const float g_PE_tm_fovAlien;   // 0xaada0

extern "C" void PlayerEgo_setTurretMode(PlayerEgo *self, int enable)
{
    if (C(self, 0x170) == 0 || P(self, 0x1e4) != 0 || C(self, 0x180) != 0) {
        // turret view unavailable -> restore default camera if a maneuver runs.
        if (P(self, 0x194) != 0) {
            PaintCanvas_CameraSetCurrent(*g_PE_tm_canvasA, U(P(self, 0x88), 0));
            LevelScript_resetCamera(P(self, 0xc));
        }
        return;
    }

    C(self, 0x1a0) = (unsigned char)enable;
    if (enable == 0) {
        PlayerEgo_stopShooting(self);
        PaintCanvas_CameraSetCurrent(*g_PE_tm_canvasE, U(P(self, 0x88), 0));
        LevelScript_resetCamera(P(self, 0xc));
    } else {
        if (P(self, 0x194) != 0)
            return;
        if (P(self, 0x178) == 0) {
            void **holder = g_PE_tm_canvasB;
            PaintCanvas_CameraCreate(*holder, &U(self, 0x174));
            unsigned int cam = (unsigned int)(unsigned long)*holder;
            float fov = (Status_inAlienOrbit() != 0) ? g_PE_tm_fovAlien : g_PE_tm_fovNormal;
            PaintCanvas_CameraSetPerspective(cam, fov, 0.0f, g_PE_tm_fovAlien);

            void *node = AEGeometry_new(*holder);
            P(self, 0x178) = node;
            AEGeometry_setRotationOrder(node, 2);
            AEGeometry_translate(node, (char *)self + 0x224);

            void *mid = AEGeometry_new(*holder);
            P(self, 0x19c) = mid;
            AEGeometry_translate(mid, (char *)mid + 0xc);
            AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x19c) + 0xc));

            void *leaf = AEGeometry_new(*holder);
            P(self, 0x17c) = leaf;
            AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x178) + 0xc));

            if (Ship_getFirstEquipmentOfSort(Status_getShip(), 0x23) != 0)
                AEGeometry_rotate(P(self, 0x178), (char *)self + 0x224);
        }
        AEGeometry_setPosition(P(self, 0x17c), (char *)self + 0x148);
        void *leaf = P(self, 0x17c);
        AEGeometry_setMatrix(leaf, AEGeometry_getMatrix(P(self, 0x8)));
        PaintCanvas_CameraSetCurrent(*g_PE_tm_canvasD, U(self, 0x174));
        Player_stopShooting(P(self, 0x0));
    }

    if (P(self, 0x30) != 0) {
        void *tf = PaintCanvas_TransformGetTransform(U(*g_PE_tm_transform, 0));
        int v = Transform_SetVisible(tf, enable != 0);
        if (enable == 0)
            FModSound_stop(*g_PE_tm_hum);
        else
            FModSound_play(*g_PE_tm_hum, 0x8cf, 0, v);
    }
}
