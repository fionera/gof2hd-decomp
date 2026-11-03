#include "engine/render/AMeshMerger.h"
#include "engine/render/PaintCanvas.h"

void AMeshMerger::render() {
    this->canvas->DrawTransform(this->transformId, nullptr);
}

AMeshMerger::~AMeshMerger() {
}
