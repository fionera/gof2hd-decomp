#include "class.h"

extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_move_guard;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_move_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_move_canvas;

extern "C" void ChoiceWindow_OnTouchMove(void *window, int x, int y);
extern "C" void Layout_OnTouchMove(void *layout, int x, int y);
extern "C" void TouchButton_OnTouchMove(void *button, int x, int y);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void AEGeometry_setRotation(void *geometry, float x, float y, float z);

static inline float absf_local(float v)
{
    return v < 0.0f ? -v : v;
}

void StarMap::OnTouchMove(int x, int y)
{
    Matrix matrix;

    if (field<uint8_t>(this, 0xa9) != 0) {
        ChoiceWindow_OnTouchMove(ptr_field(this, 0x5c), x, y);
        return;
    }
    if (field<uint8_t>(this, 0x138) != 0 || field<uint8_t>(this, 0x139) != 0) {
        return;
    }
    void *layout = *g_StarMap_move_layout;
    Layout_OnTouchMove(layout, x, y);
    if ((field<uint8_t>(this, 0xa8) != 0 && field<uint8_t>(this, 0x13a) != 0) ||
        field<uint8_t>(this, 0x13b) != 0) {
        return;
    }
    TouchButton_OnTouchMove(ptr_field(this, 0x4c), x, y);
    if (field<uint8_t>(this, 0x174) == 0) {
        return;
    }

    float fx = (float)x;
    float fy = (float)y;
    float lastX = field<float>(this, 0x15c);
    float lastY = field<float>(this, 0x160);
    field<int32_t>(this, 0x168) = 0x3f800000;
    field<float>(this, 0x15c) = fx;
    field<float>(this, 0x160) = fy;
    float dx = (fx - lastX) * field<float>(this, 0x1cc);
    float dy = field<float>(this, 0x1cc) * (fy - lastY);
    field<float>(this, 0x150) = dx;
    field<float>(this, 0x154) = dy;

    if (field<int32_t>(this, 4) == 0) {
        float speed = absf_local(dx + dy);
        if (speed > 10.0f) {
            speed = 10.0f;
        }
        float targetX = (float)field<int32_t>(this, 0x13c);
        float targetY = (float)field<int32_t>(this, 0x140);
        field<int32_t>(this, 0x13c) = (int)(dx + targetX);
        field<int32_t>(this, 0x140) = (int)(dy + targetY);
        field<float>(this, 0x1c0) = field<float>(this, 0x1c0) + speed;
        if (absf_local(field<float>(this, 0x144) - fx) > 3.0f ||
            absf_local(field<float>(this, 0x148) - fy) > 3.0f) {
            field<int32_t>(this, 0x19c) = -1;
            field<int32_t>(this, 0x60) = -1;
            field<int32_t>(this, 0x1d0) = 0;
        }
        void *canvas = *g_StarMap_move_canvas;
        PaintCanvas_CameraGetCurrent(canvas);
        __builtin_memcpy(&matrix, PaintCanvas_CameraGetLocal(canvas), 0x3c);
        PaintCanvas_CameraSetLocal(canvas, &matrix);
        return;
    }

    if (absf_local(field<float>(this, 0x144) - fx) <= 3.0f ||
        absf_local(field<float>(this, 0x148) - fy) <= 3.0f) {
        field<int32_t>(this, 0x1a0) = -1;
        field<int32_t>(this, 0x64) = -1;
    }

    float rotZ = dx * 16.0f;
    float rotX = dy * 16.0f;
    float pitch = field<float>(this, 0x18c) + rotX;
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }
    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    float yaw = (float)((int)(field<float>(this, 0x188) + rotZ) & 0xffff);
    float absZ = absf_local(rotZ);
    float absX = absf_local(rotX);
    field<float>(this, 0x150) = rotZ;
    field<float>(this, 0x154) = rotX;
    field<float>(this, 0x16c) = absZ > 3.0f ? rotZ : 0.0f;
    field<float>(this, 0x170) = absX > 3.0f ? rotX : 0.0f;
    field<float>(this, 0x188) = yaw;
    field<float>(this, 0x18c) = pitch;
    AEGeometry_setRotation(ptr_field(this, 0xa4), field<float>(this, 0x170), yaw, field<float>(this, 0x16c));
}
