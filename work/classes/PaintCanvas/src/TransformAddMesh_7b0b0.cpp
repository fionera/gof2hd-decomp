#include "class.h"

// AbyssEngine::PaintCanvas::TransformAddMesh(unsigned transformIndex, unsigned short meshId, bool b)
// Adds a mesh (by global mesh id) to a transform. Ghidra mis-decoded this tiny wrapper; modeled
// on TransformAddMeshId — it creates/resolves the mesh then forwards to the internal add helper.
extern "C" void paintcanvas_ext_transform_addmesh(AbyssEngine::PaintCanvas *, void *,
                                                  unsigned short, bool);

void TransformAddMesh(AbyssEngine::PaintCanvas *self, unsigned int transformIndex,
                      unsigned short meshId, bool b)
{
    if (transformIndex < *(unsigned int *)((char *)self + 0x158)) {
        void *t = (*(void ***)((char *)self + 0x15c))[transformIndex];
        return paintcanvas_ext_transform_addmesh(self, t, meshId, b);
    }
}
