#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

extern "C" void Player_updateDamageRate(Player *self);
extern "C" void Player_StopEngineSound(Player *self);
extern "C" void MatrixGetPosition(void *out, float *matrix);
extern "C" float Vector_subF(Vector *out, Vector *a, Vector *b);
extern "C" void Vector_scale(Vector *out, float s, Vector *v);
extern "C" void Vector_div(Vector *out, Vector *v, float s);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" int FModSound_updateEvent3DAttributes(void *sound, int event, int eventId,
                                                 Vector *pos, Vector *vel, bool b);
extern "C" void FloatVectorMax(void *out, float a, float b, int c, int d);

extern "C" int **g_update_clock;       // DAT_b41a0
extern "C" char **g_update_flag;       // DAT_b41be
extern "C" float **g_update_speed;     // DAT_b41e0
extern "C" void (**g_update_transform)(void *, void *, int);  // DAT_b41e2 fn ptr table
extern "C" void **g_update_sound;      // DAT_b423c
extern "C" const float k_update_a;     // DAT_b42b8
extern "C" const float k_update_b;     // DAT_b42bc
extern "C" const float k_update_c;     // DAT_b42c0

extern "C" Vector *Player_update(Player *self, int dt, int doSound)
{
    void * volatile cookie = __stack_chk_guard;

    int b4 = self->field_b4 + dt;
    self->field_b4 = b4;
    if (b4 > 3000) {
        self->damaged = 0;
        self->field_b4 = 0;
    }

    Vector *result = 0;

    if (self->field_68 != 0) {
        int e = self->empData + dt;
        float ef = (float)self->empData;
        float ef2 = (float)e;
        int maxEmp = self->maxEmpPoints;
        float mf = (float)maxEmp;
        float v = (ef2 / ef) * mf;
        self->empData = e;
        self->empPoints = (int)v;
        if (maxEmp < (int)v) {
            self->field_68 = 0;
            self->empPoints = maxEmp;
            int *clk = *g_update_clock;
            *(int *)((char *)clk + 0x134) = *(int *)((char *)clk + 0x134) - 1;
            self->empData = 0;
        }
        Player_updateDamageRate(self);
    }

    char *flagObj = *g_update_flag;
    if (*(char *)(flagObj + 0xf) == 0 || doSound == 0 || self->field_f4 == -1) {
        if (self->field_108 != 0) {
            Player_StopEngineSound(self);
        }
    } else {
        float *transform = self->transform;
        void (*fn)(void *, void *, int) = *g_update_transform;
        float local[3];
        float spd = (float)(**g_update_speed);
        fn(*g_update_speed, local, (int)(long)transform);
        float tmpA[3], tmpB[3], tmpC[3];
        float d = (float)Vector_subF((Vector *)tmpA, (Vector *)self->position, (Vector *)local);
        Vector_scale((Vector *)tmpB, d, (Vector *)tmpA);
        Vector_div((Vector *)tmpC, (Vector *)tmpB, (float)dt);
        (void)spd;
        fn(tmpB, local, (int)(long)transform);
        int ev = FModSound_updateEvent3DAttributes(*g_update_sound, self->engineEvent,
                                                   self->field_f4, (Vector *)tmpB, (Vector *)tmpC, false);
        self->engineEvent = ev;
        self->field_108 = 1;
        fn(tmpA, local, (int)(long)transform);
        Vector_assign((Vector *)self->position, (Vector *)tmpA);
    }

    float speed = (float)dt;
    float nf = *(float *)&self->field_60 + speed * k_update_a * k_update_b;
    *(float *)&self->field_60 = nf;
    FloatVectorMax(&result, nf, k_update_c, 2, 0x20);

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
    return result;
}
