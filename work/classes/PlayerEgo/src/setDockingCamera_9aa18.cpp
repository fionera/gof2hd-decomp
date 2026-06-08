#include "class.h"

// PlayerEgo::setDockingCamera()
//   Lazily builds the docking camera rig (a PaintCanvas camera plus a small
//   chain of AEGeometry nodes used to orient/offset the view) on first use,
//   seeding a default offset vector at 0x224 when none is set, then positions
//   the leaf node at the dock offset (0x148), copies the ship matrix onto it,
//   and makes the camera current.

extern "C" void  PaintCanvas_CameraCreate(void *canvas, unsigned int *outHandle);
extern "C" int   Status_inAlienOrbit();
extern "C" void  PaintCanvas_CameraSetPerspective(unsigned int cam, float fov, float a, float b);
extern "C" void *AEGeometry_new(void *canvas);            // operator new + AEGeometry::AEGeometry
extern "C" void  AEGeometry_setRotationOrder(void *geo, int order);
extern "C" void  AEGeometry_translate(void *geo, const void *vec);
extern "C" void  AEGeometry_addChild(void *parent, void *child);
extern "C" void  AEGeometry_setPosition(void *geo, const void *vec);
extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void  AEGeometry_setMatrix(void *geo, const void *m);
extern "C" void  PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam); // 0x1ab2d8 veneer

__attribute__((visibility("hidden"))) extern void **g_PE_dc_canvas;   // 0xaab84 PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_PE_dc_canvas2;  // 0xaab8c PaintCanvas (current)
extern const float g_PE_dc_fovNormal;   // 0xaab68
extern const float g_PE_dc_fovAlien;    // 0xaab6c
extern const unsigned int g_PE_dc_defX; // 0xaab78 default offset lo
extern const unsigned int g_PE_dc_defY; // 0xaab7c default offset hi

extern "C" void PlayerEgo_setDockingCamera(PlayerEgo *self)
{
    if (P(self, 0x178) == 0) {
        void **holder = g_PE_dc_canvas;
        PaintCanvas_CameraCreate(*holder, &U(self, 0x174));
        unsigned int cam = (unsigned int)(unsigned long)*holder;

        float fov = (Status_inAlienOrbit() != 0) ? g_PE_dc_fovAlien : g_PE_dc_fovNormal;
        PaintCanvas_CameraSetPerspective(cam, fov, 0.0f, g_PE_dc_fovAlien);

        void *node = AEGeometry_new(*holder);
        P(self, 0x178) = node;
        AEGeometry_setRotationOrder(node, 2);

        // seed the default rig offset (0x224..0x22c) only when fully zero.
        if (F(self, 0x224) == 0.0f && F(self, 0x228) == 0.0f && F(self, 0x22c) == 0.0f) {
            U(self, 0x228) = g_PE_dc_defX;
            U(self, 0x22c) = g_PE_dc_defY;
        }
        AEGeometry_translate(P(self, 0x178), (char *)self + 0x224);

        void *mid = AEGeometry_new(*holder);
        P(self, 0x19c) = mid;
        AEGeometry_translate(mid, (char *)mid + 0xc);
        AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x19c) + 0xc));

        void *leaf = AEGeometry_new(*holder);
        P(self, 0x17c) = leaf;
        AEGeometry_addChild(P(self, 0x178), *(void **)((char *)P(self, 0x178) + 0xc));
    }

    AEGeometry_setPosition(P(self, 0x17c), (char *)self + 0x148);
    void *leaf = P(self, 0x17c);
    AEGeometry_setMatrix(leaf, AEGeometry_getMatrix(P(self, 0x8)));

    PaintCanvas_CameraSetCurrent(*g_PE_dc_canvas2, U(self, 0x174));
}
