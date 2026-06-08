#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void *operator_new(unsigned int sz);
extern "C" void operator_delete(void *p);
extern "C" unsigned short *operator_new_arr(unsigned int n);
extern "C" void operator_delete_arr(void *p);

extern "C" void AEGeometry_ctor(void *self, int resId, void *canvas, int flag);
extern "C" void AEGeometry_addChild(unsigned self);
extern "C" void AEGeometry_setLodMeshes(void *self, unsigned short *meshes, int *dist, int n);
extern "C" void AEGeometry_setLodMeshesWithMeshIds(void *self, unsigned short *meshes,
                                                   unsigned *ids, int *dist, int n);
extern "C" void AEGeometry_setLodChildMeshes(void *self, unsigned short *meshes);
extern "C" void AEGeometry_setScaling(float x, float y, float z);
extern "C" void AEGeometry_setLodLastVisibleDistance(void *self);

extern "C" void PaintCanvas_TransformCreate(void *canvas, unsigned *out);
extern "C" void PaintCanvas_TransformAddMesh(unsigned canvas, unsigned short t, int flag);
extern "C" void PaintCanvas_TransformAddMeshId(void *canvas, unsigned t, unsigned mesh);
extern "C" void PaintCanvas_TransformAddChild(void *canvas, unsigned parent, unsigned child);
extern "C" int PaintCanvas_TransformGetLocal(unsigned canvas);
extern "C" int PaintCanvas_TransformGetTransform(unsigned canvas);
extern "C" void PaintCanvas_MeshCreate(void *canvas, unsigned short mesh, unsigned *out, int flag);
extern "C" void PaintCanvas_MaterialCreate(void *canvas, unsigned short mat, unsigned *out);
extern "C" void PaintCanvas_MeshChangeResourceMaterial(void *canvas, unsigned mesh,
                                                       unsigned short mat);
extern "C" void MatrixSetTranslation(void *m, float x, float y, float z);
extern "C" int AERandom_nextInt(int rng);

// Per-group builders for the special param_1 == 0xf sub-cases that build articulated
// transform chains (capital ships). Each delegates to the engine the same way the target
// does; kept as helpers to preserve the recoverable dispatch without inlining ~1.7KB.
extern "C" void Globals_buildShipGroup0f(void *self, int param_2, void *canvas);

extern void *const gGSG_guard __attribute__((visibility("hidden")));     // DAT_000f51dc
extern void *const gGSG_canvas __attribute__((visibility("hidden")));    // shared canvas global
extern const unsigned short gGSG_resTable[] __attribute__((visibility("hidden")));   // DAT_000f520c
extern const unsigned short gGSG_meshTable[] __attribute__((visibility("hidden")));  // DAT_000f5210
extern const short gGSG_extraTable[] __attribute__((visibility("hidden")));          // DAT_000f5544
extern const unsigned gGSG_lodTable[] __attribute__((visibility("hidden")));         // DAT_000f5548 (3/group)
extern const unsigned gGSG_childTable[] __attribute__((visibility("hidden")));       // DAT_000f5550 (3/group)

// Globals::getShipGroup(int kind, int variant, bool wireframe) -> AEGeometry* via sret (this).
extern "C" void Globals_getShipGroup(void *self, int kind, int variant, int wireframe)
{
    int *guardP = *(int **)gGSG_guard;
    volatile int saved = *guardP;
    void **canvasP = *(void ***)gGSG_canvas;

    if (kind == 0xf) {
        Globals_buildShipGroup0f(self, variant, *canvasP);
        goto done;
    }
    if (kind == 0xe || kind == 0xd) {
        void *geom = operator_new(0xc0);
        int resId = (kind == 0xe) ? 0x37e7 : 0x4275;
        AEGeometry_ctor(geom, resId, *canvasP, 0);
        unsigned t0 = 0xffffffff;
        PaintCanvas_TransformCreate(*canvasP, &t0);
        PaintCanvas_TransformAddMesh((unsigned)(long)*canvasP, (unsigned short)t0, 1);
        AEGeometry_addChild((unsigned)(long)geom);
        unsigned t1 = 0xffffffff;
        PaintCanvas_TransformCreate(*canvasP, &t1);
        PaintCanvas_TransformAddMesh((unsigned)(long)*canvasP, (unsigned short)t1, 1);
        AEGeometry_addChild((unsigned)(long)geom);
        unsigned short lodMeshes[2] = {0, 0};
        int dist[2];
        dist[0] = (kind == 0xe) ? 35000 : 35000;
        dist[1] = (kind == 0xe) ? 60000 : 45000;
        AEGeometry_setLodMeshes(geom, lodMeshes, dist, 2);
        unsigned short childMeshes[1] = {0};
        AEGeometry_setLodChildMeshes(geom, childMeshes);
        if (kind == 0xe) {
            AEGeometry_setScaling(1.0f, 1.0f, 1.0f);
        }
        goto done;
    }

    // Generic path: per-ship table-driven build indexed by `kind`.
    {
        void *geom = operator_new(0xc0);
        AEGeometry_ctor(geom, gGSG_resTable[kind], *canvasP, 1);
        unsigned short mesh = gGSG_meshTable[kind];
        unsigned mainT = 0xffffffff;
        unsigned mainMesh = 0xffffffff;
        if (mesh != 0xffff) {
            PaintCanvas_MeshCreate(*canvasP, mesh, &mainMesh, 1);
            PaintCanvas_TransformCreate(*canvasP, &mainT);
            PaintCanvas_TransformAddMeshId(*canvasP, mainT, mainMesh);
            AEGeometry_addChild((unsigned)(long)geom);
            *(unsigned *)((char *)geom + 0x20) = mainMesh;
        }
        if (!wireframe) {
            unsigned short mat = (unsigned short)((short)kind + 0x7dc8);
            unsigned matH = 0xffffffff;
            PaintCanvas_MaterialCreate(*canvasP, mat, &matH);
            PaintCanvas_MeshChangeResourceMaterial(*canvasP, *(unsigned *)((char *)geom + 0x1c),
                                                   mat);
        }
        short extra = gGSG_extraTable[kind];
        if (extra != -1) {
            unsigned t = 0xffffffff;
            PaintCanvas_TransformCreate(*canvasP, &t);
            PaintCanvas_TransformAddMesh((unsigned)(long)*canvasP, (unsigned short)t,
                                         (int)(unsigned char)(char)extra);
            AEGeometry_addChild((unsigned)(long)geom);
        }
        if (wireframe) {
            if (kind != 0x27 && kind != 0x29) {
                unsigned t = 0xffffffff;
                PaintCanvas_TransformCreate(*canvasP, &t);
                PaintCanvas_TransformAddMesh((unsigned)(long)*canvasP, (unsigned short)t,
                                             (int)(char)(-0x14 + (char)kind));
                AEGeometry_addChild((unsigned)(long)geom);
            }
        } else {
            unsigned t = 0xffffffff;
            PaintCanvas_TransformCreate(*canvasP, &t);
            PaintCanvas_TransformAddMesh((unsigned)(long)*canvasP, (unsigned short)t,
                                         (int)(char)(0x50 + (char)kind));
            AEGeometry_addChild((unsigned)(long)geom);
        }

        // Count up to 2 LOD meshes for this ship.
        const unsigned *lod = &gGSG_lodTable[kind * 3];
        unsigned count = 0;
        for (int i = 0; i != 2; i++) {
            if (lod[i] != 0xffff) count++;
        }
        if (count != 0) {
            unsigned short *meshes = operator_new_arr(count << 1);
            unsigned *ids = (unsigned *)operator_new_arr(count * 4);
            int *dist = (int *)operator_new_arr(count * 4);
            int d = 5000;
            unsigned *idp = ids;
            const unsigned *src = lod;
            for (unsigned i = 0; i < count; i++) {
                unsigned m = *src;
                dist[i] = d;
                meshes[i] = (unsigned short)m;
                if (!wireframe) {
                    PaintCanvas_MeshCreate(*canvasP, (unsigned short)m, idp, 1);
                    PaintCanvas_MeshChangeResourceMaterial(*canvasP, *idp,
                                                           (unsigned short)((short)kind + 0x7dc8));
                }
                d += 8000;
                idp++;
                src++;
            }
            if (wireframe) {
                AEGeometry_setLodMeshes(geom, meshes, dist, count);
            } else {
                AEGeometry_setLodMeshesWithMeshIds(geom, meshes, ids, dist, count);
            }

            const unsigned *childSrc = &gGSG_childTable[kind * 3];
            int childCount = 0;
            for (int i = 0; i != 2; i++) {
                if (childSrc[i] != 0xffff) childCount++;
            }
            if (childCount != 0) {
                unsigned short *childMeshes = operator_new_arr(childCount << 1);
                for (int i = 0; i != childCount; i++) {
                    childMeshes[i] = (unsigned short)childSrc[i];
                }
                AEGeometry_setLodChildMeshes(geom, childMeshes);
                operator_delete_arr(childMeshes);
            }
            operator_delete_arr(meshes);
            operator_delete_arr(dist);
        }
        AEGeometry_setLodLastVisibleDistance(geom);
    }

done:
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
