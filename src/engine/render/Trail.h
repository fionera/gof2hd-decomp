#ifndef GOF2_TRAIL_H
#define GOF2_TRAIL_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// PaintCanvas is declared at global scope (and re-exported into AbyssEngine) by
// its own header; forward-declare it the same way for pointer-only use here.
class PaintCanvas;

namespace AbyssEngine {

using ::PaintCanvas;

// Trail: a flat triangle-strip "ribbon" mesh trailing a moving object (engine
// trails, exhaust, etc.). It owns a PaintCanvas mesh + transform and a pair of
// vertex buffers (absolute and head-relative) that are shifted forward each
// update.
class Trail {
public:
    uint32_t field_0x00;     // zeroed in ctor, otherwise unused
    uint32_t field_0x04;     // zeroed in ctor, otherwise unused
    uint32_t field_0x08;     // zeroed in ctor, otherwise unused
    int width;               // half-width of the trail strip
    uint32_t meshId;         // PaintCanvas mesh id
    uint32_t transformId;    // PaintCanvas transform id
    int *points;             // absolute vertex points buffer (x,y,z triples)
    int *relativePoints;     // vertex points relative to the head buffer
    int pointCount;          // number of ints in the point buffers
    int segments;            // number of trail segments

    Trail(int type, int segments);
    ~Trail();

    void update(const AbyssEngine::AEMath::Vector &a, const AbyssEngine::AEMath::Vector &b);
    void update(float ax, float ay, float az, float bx, float by, float bz);
    void update(const AbyssEngine::AEMath::Matrix &a, const AbyssEngine::AEMath::Matrix &b,
                const AbyssEngine::AEMath::Vector &v);

    void render();
    void translate(const AbyssEngine::AEMath::Vector &delta);
    void setWidth(int width);
    void changeType(int type);
    void reset(AbyssEngine::AEMath::Vector value);
};

} // namespace AbyssEngine

// The trail render / colour calls dispatch through engine function-pointer
// globals in the shipped binary; modelled here as external entry points.
extern "C" void Trail_renderTransform(AbyssEngine::PaintCanvas *canvas, uint32_t transform, int mode);
extern "C" void Trail_transformSetColor(AbyssEngine::PaintCanvas *canvas, uint32_t transform, uint32_t color);

__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasRender;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType1;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType2;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType3;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasTypeDefault;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType5;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType8;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasCtor;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasUpdate;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasSetWidth;

#endif
