#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void Radar_MatrixTransformVector(void *out, Matrix const *matrix, void const *value);
extern "C" void Radar_VectorAssign(Vector *dst, void const *src);
extern "C" int Radar_GetScreenPosition(AbyssEngine::PaintCanvas *canvas, void const *value, void *screen);
extern "C" void Radar_elipsoidIntersect(void *out, Radar *self, int x, int y, Vector value);

void Radar::update(Vector value)
{
    Vector transformedValue;
    void *transformed = &transformedValue;
    char positionStorage[12];
    __builtin_memcpy(positionStorage, &value, 12);

    Radar_MatrixTransformVector(transformed, (Matrix const *)((char *)this + 0x1d0), positionStorage);
    Vector *current = (Vector *)((char *)this + 0x154);
    Radar_VectorAssign(current, transformed);

    this->f_158 = -this->f_158;
    this->f_15c = -this->f_15c;

    int visible = Radar_GetScreenPosition(gPaintCanvas, positionStorage, positionStorage);
    this->f_11c = (uint8_t)visible;

    float *position = (float *)positionStorage;
    int screenX = (int)position[0];
    this->f_fc = screenX;
    int screenY = (int)position[1];
    this->f_100 = screenY;

    if (visible == 0) {
        Radar_elipsoidIntersect(transformed, this, screenX, screenY, *current);
        Radar_VectorAssign(current, transformed);
        this->f_fc = (int)current->x;
        this->f_100 = (int)current->y;
    }
}
