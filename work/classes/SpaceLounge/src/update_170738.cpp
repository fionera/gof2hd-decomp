#include "class.h"

extern "C" void SpaceLounge_update_map_tail(void *map, int dt);
extern "C" void SpaceLounge_update_ship_tail(void *list, int dt);
extern "C" void EaseInOutMatrix_Increase(void *ease, float dt);
extern "C" void EaseInOutMatrix_GetMaxValue(void *out, void *ease);
extern "C" void EaseInOutMatrix_GetValue(void *out, void *ease);
extern "C" int Matrix_equal(void *a, void *b);
extern "C" void *PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas);
extern "C" void PaintCanvas_CameraSetLocal(void *canvas, void *matrix);
extern "C" float Sinf(float value);
extern "C" int AERandom_nextInt(void *random, int limit);
extern "C" void *operator_new(unsigned int size);
extern "C" void EaseInOut_ctor(void *ease, float start, float end);
extern "C" float EaseInOut_GetValue(void *ease);
extern "C" float EaseInOut_GetMaxValue(void *ease);
extern "C" void EaseInOut_SetRange(void *ease, float start, float end);
extern "C" void EaseInOut_Increase(void *ease, float value);
extern "C" void MatrixSetRotation(void *matrix, float x, float y, float z);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void Matrix_mul_assign(void *lhs, void *rhs);
extern "C" void SpaceLounge_updateScreenPositions(SpaceLounge *self);
extern "C" void ScrollTouchWindow_update(void *scroll, int dt);
extern "C" void CutScene_update(void *cutscene);

extern "C" void *SpaceLounge_update_camera_slot_a;
extern "C" void *SpaceLounge_update_camera_slot_b;
extern "C" void *SpaceLounge_update_camera_slot_c;
extern "C" void *SpaceLounge_update_random_slot;
extern "C" int *SpaceLounge_update_width_slot;
extern "C" int *SpaceLounge_update_height_slot;
extern "C" int *SpaceLounge_update_top_slot;

extern "C" void SpaceLounge_update(SpaceLounge *self, int dt)
{
    char maxMatrix[60];
    char valueMatrix[60];

    if (dt > 50) {
        dt = 50;
    }
    if (UC(self, 0xb0) == 0) {
        return;
    }

    if (UC(self, 0x34) != 0) {
        return SpaceLounge_update_map_tail(P(self, 0x4), dt);
    }
    if (UC(self, 0x1c) != 0) {
        return SpaceLounge_update_ship_tail(P(self, 0xc), dt);
    }

    if (UC(self, 0xbc) == 0) {
        EaseInOutMatrix_Increase(P(self, 0x48), (float)dt);
    }

    void *cameraSlot;
    void *camera;
    void *current;
    if (UC(self, 0xbd) == 0) {
        EaseInOutMatrix_GetMaxValue(maxMatrix, P(self, 0x48));
        EaseInOutMatrix_GetValue(valueMatrix, P(self, 0x48));
        if (Matrix_equal(maxMatrix, valueMatrix) == 0) {
            UC(self, 0xbc) = 0;
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_c;
            camera = *(void **)cameraSlot;
            current = PaintCanvas_CameraGetCurrent(camera);
            EaseInOutMatrix_GetValue(valueMatrix, P(self, 0x48));
            PaintCanvas_CameraSetLocal(camera, valueMatrix);
        } else {
            goto idle_camera;
        }
    } else {
idle_camera:
        float step = (float)dt * 0.001f;
        if (step > 1.0f) {
            step = 1.0f;
        }
        if (step < 0.0f) {
            step = 0.0f;
        }
        UC(self, 0xbd) = 1;
        F(self, 0x104) = F(self, 0x104) + step;
        float wave = Sinf(F(self, 0x104));
        float maxRot = (float)I(self, 0x108) * 0.5f;

        if (UC(self, 0xbc) == 0) {
            cameraSlot = *(void **)&SpaceLounge_update_camera_slot_a;
            camera = *(void **)cameraSlot;
            current = PaintCanvas_CameraGetLocal(camera);
            (void)current;
            UC(self, 0xbc) = 1;
            int amount = AERandom_nextInt(*(void **)&SpaceLounge_update_random_slot, 10);
            UC(self, 0xc4) = 0;
            if (P(self, 0xc0) == 0) {
                void *ease = operator_new(0x10);
                EaseInOut_ctor(ease, 0.0f, (float)amount);
                P(self, 0xc0) = ease;
            } else {
                EaseInOut_SetRange(P(self, 0xc0), 0.0f, (float)amount);
            }
            I(self, 0x108) = 2;
        } else {
            float value = EaseInOut_GetValue(P(self, 0xc0));
            float maxValue = EaseInOut_GetMaxValue(P(self, 0xc0));
            float distance = value - maxValue;
            if (distance < 0.0f) {
                distance = -distance;
            }
            int amount = I(self, 0x108);
            if (distance < 0.25f) {
                void *random = *(void **)&SpaceLounge_update_random_slot;
                amount = AERandom_nextInt(random, 10);
                float next = (float)(5 - amount);
                UC(self, 0xc4) = value > next;
                EaseInOut_SetRange(P(self, 0xc0), value, next);
                I(self, 0x108) = AERandom_nextInt(random, 4) + 1;
                amount = I(self, 0x108);
            }
            if (UC(self, 0xc4) != 0) {
                amount = -amount;
            }
            EaseInOut_Increase(P(self, 0xc0), (float)(dt * amount));
        }

        float value = EaseInOut_GetValue(P(self, 0xc0));
        MatrixSetRotation(valueMatrix, value / 90.0f, 0.0f, value);
        MatrixSetTranslation(maxMatrix, 0.0f, 0.0f, wave * maxRot);
        Matrix_mul_assign(valueMatrix, B(self, 0xc8));
        cameraSlot = *(void **)&SpaceLounge_update_camera_slot_b;
        camera = *(void **)cameraSlot;
        current = PaintCanvas_CameraGetCurrent(camera);
        PaintCanvas_CameraSetLocal(camera, valueMatrix);
    }

    SpaceLounge_updateScreenPositions(self);
    if (I(self, 0x14) != 0) {
        ScrollTouchWindow_update(P(self, 0x60), dt);
    }
    CutScene_update(P(self, 0x44));

    if (UC(self, 0xb2) != 0) {
        int x = I(self, 0xb4);
        int y = I(self, 0xb8);
        int top = *(int *)SpaceLounge_update_top_slot;
        int height = *(int *)SpaceLounge_update_height_slot;
        int width = *(int *)SpaceLounge_update_width_slot;
        void *cutscene = P(self, 0x44);
        if (y < top) {
            if (x < 150) {
                F(cutscene, 0xc) -= 10.0f;
            } else if (x > width - 150) {
                F(cutscene, 0xc) += 10.0f;
            }
        } else if (y < height - top * 2) {
            if (x < 70) {
                F(cutscene, 0x4) -= 7.5f;
            } else if (x > width - 70) {
                F(cutscene, 0x4) += 7.5f;
            } else if (x > 100 && x < width - 100) {
                if (y < height / 2) {
                    F(cutscene, 0x10) -= (float)dt;
                } else {
                    F(cutscene, 0x10) += (float)dt;
                }
            }
        } else {
            if (x < 70) {
                F(cutscene, 0x8) -= (float)dt;
            } else if (x > width - 70) {
                F(cutscene, 0x8) += (float)dt;
            }
        }
    }
}
