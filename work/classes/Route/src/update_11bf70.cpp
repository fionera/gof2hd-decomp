#include "class.h"

extern "C" void Waypoint_setActive(Waypoint *wp, bool active);
extern "C" void Waypoint_reached(Waypoint *wp);
extern "C" void Waypoint_reset(Waypoint *wp);
extern "C" float Waypoint_advance(Waypoint *wp, bool b);   // veneer tail-call target

// Route::update(float, float, float) -> if close enough to the active waypoint, advance to the next.
extern "C" float Route_update_xyz(Route *self, float x, float y, float z)
{
    int idx = self->f_0;
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    if (idx >= (int)wps->length)
        return x;
    if (F<Array<void *> *>(self, 0x10)->data[idx] != 0)
        return x;
    Waypoint *wp = wps->data[idx];
    float dx = x - (float)*(int32_t *)((char *)wp + 0x124);
    if (!(dx < 2000.0f) || !(dx > -2000.0f))
        return x;
    float dy = y - (float)*(int32_t *)((char *)wp + 0x128);
    if (!(dy < 2000.0f) || !(dy > -2000.0f))
        return x;
    float dz = z - (float)*(int32_t *)((char *)wp + 0x12c);
    if (!(dz < 2000.0f) || !(dz > -2000.0f))
        return x;

    Waypoint_setActive(wp, false);
    Waypoint_reached(F<Array<Waypoint *> *>(self, 0xc)->data[self->f_0]);
    int cur = self->f_0;
    Array<Waypoint *> *w = F<Array<Waypoint *> *>(self, 0xc);
    int next = cur + 1;
    self->f_0 = next;
    uint32_t len = w->length;
    if (self->f_4 != 0 && (int)(len - 1) <= cur) {
        self->f_0 = 0;
        for (uint32_t i = 0; i < len; i++) {
            Waypoint_reset(w->data[i]);
            w = F<Array<Waypoint *> *>(self, 0xc);
            len = w->length;
        }
        next = self->f_0;
    }
    if (next < (int)len)
        return Waypoint_advance(w->data[next], true);
    return x;
}
