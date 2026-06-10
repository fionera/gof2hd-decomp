#include "gof2/Route.h"
#include "gof2/Waypoint.h"


extern "C" void Waypoint_reset(Waypoint *wp);
extern "C" Waypoint *Route_getWaypointAt(Route *self, int index);
extern "C" Route *Route_clone(Route *self);
extern "C" void Waypoint_reached(Waypoint *wp);
extern "C" void ArrayReleaseClasses_Waypoint(Array<Waypoint *> *a);
extern "C" void *ArrayWaypoint_dtor(Array<Waypoint *> *a);
extern "C" void *ArrayKIPlayer_dtor(Array<KIPlayer *> *a);
extern "C" void *ArrayInt_dtor(Array<int> *a);
extern "C" void operator_delete(void *p);
extern "C" void Route_update_xyz(Route *self, float x, float y, float z);
extern "C" void *operator_new(unsigned int sz);
extern "C" int __aeabi_idiv(int n, int d);
extern "C" void ArraySetLengthKIPlayer(uint32_t n, void *a);
extern "C" void ArraySetLengthInt(uint32_t n, void *a);
extern "C" void Waypoint_ctor(void *wp, int x, int y, int z, Route *route);
extern "C" void ArrayAddWaypoint(void *wp, void *a);
extern "C" void Waypoint_setActive(Waypoint *wp, bool active);
extern "C" void ArrayAddInt(int v, void *a);
extern "C" void ArrayKIPlayer_ctor(void *a);
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count);
extern "C" Route *Route_ctor1(Route *self, int *coords, int count);
extern "C" int Route_getDockingTimeAt(Route *self, int index);

// ---- waypointDefined_11bee8.cpp ----
// Route::waypointDefined() -> whether the waypoint array has been allocated.
extern "C" bool Route_waypointDefined(Route *self)
{
    return self->field_0xc != 0;
}

// ---- length_11bf5c.cpp ----
// Route::length() -> number of waypoints.
extern "C" int Route_length(Route *self)
{
    return (int)F<Array<Waypoint *> *>(self, 0xc)->length;
}

// ---- reset_11bd94.cpp ----
// Route::reset() -> reset every waypoint and rewind to the start.
extern "C" void Route_reset(Route *self)
{
    for (uint32_t i = 0; i < F<Array<Waypoint *> *>(self, 0xc)->length; i++)
        Waypoint_reset(F<Array<Waypoint *> *>(self, 0xc)->data[i]);
    self->field_0x0 = 0;
}

// ---- getWaypoint_11bed8.cpp ----
// Route::getWaypoint() -> waypoint at the current index.
extern "C" Waypoint *Route_getWaypoint(Route *self)
{
    return Route_getWaypointAt(self, self->field_0x0);
}

// ---- getExactClone_11c074.cpp ----
// Route::getExactClone() -> a clone whose reached-waypoints are marked, sharing the current index.
extern "C" Route *Route_getExactClone(Route *self)
{
    Route *result = Route_clone(self);
    for (uint32_t i = 0; i < F<Array<Waypoint *> *>(result, 0xc)->length; i++) {
        if (*(uint8_t *)((char *)F<Array<Waypoint *> *>(self, 0xc)->data[i] + 0x130) != 0)
            Waypoint_reached(F<Array<Waypoint *> *>(result, 0xc)->data[i]);
    }
    result->field_0x0 = self->field_0x0;
    return result;
}

// ---- getDockingTarget_11bde8.cpp ----
// Route::getDockingTarget() -> docking target at the current index, or 0.
extern "C" KIPlayer *Route_getDockingTarget(Route *self)
{
    Array<KIPlayer *> *targets = F<Array<KIPlayer *> *>(self, 0x10);
    int index = self->field_0x0;
    if (targets != 0 && (int)targets->length > index)
        return targets->data[index];
    return 0;
}

// ---- getDockingTime_11be1c.cpp ----
// Route::getDockingTime(int) -> docking time at the given index, or 0.
extern "C" int Route_getDockingTimeAt(Route *self, int index)
{
    if (self->field_0x10 != 0) {
        Array<int> *times = F<Array<int> *>(self, 0x14);
        if (index < (int)times->length)
            return times->data[index];
    }
    return 0;
}

// ---- setNewCoords_11bdb8.cpp ----
// Route::setNewCoords(Vector) -> overwrite the first waypoint's target coordinates.
extern "C" void Route_setNewCoords(Route *self, float x, float y, float z)
{
    Waypoint *wp = F<Array<Waypoint *> *>(self, 0xc)->data[0];
    wp->field_0x124 = (int)x;
    wp->field_0x128 = (int)y;
    wp->field_0x12c = (int)z;
}

// ---- _Route_11bcf0.cpp ----
// Route::~Route() (D2). Returns this.
extern "C" void *_ZN5RouteD2Ev(Route *self)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    if (wps != 0) {
        ArrayReleaseClasses_Waypoint(wps);
        Array<Waypoint *> *wps2 = F<Array<Waypoint *> *>(self, 0xc);
        if (wps2 != 0)
            operator_delete(ArrayWaypoint_dtor(wps2));
    }
    F<Array<Waypoint *> *>(self, 0xc) = 0;

    Array<KIPlayer *> *tgt = F<Array<KIPlayer *> *>(self, 0x10);
    if (tgt != 0)
        operator_delete(ArrayKIPlayer_dtor(tgt));
    F<Array<KIPlayer *> *>(self, 0x10) = 0;

    Array<int> *times = F<Array<int> *>(self, 0x14);
    if (times != 0)
        operator_delete(ArrayInt_dtor(times));
    F<Array<int> *>(self, 0x14) = 0;

    return self;
}

// ---- getDockingTarget_11be00.cpp ----
// Route::getDockingTarget(int) -> docking target at the given index, or 0.
extern "C" KIPlayer *Route_getDockingTarget_i(Route *self, int index)
{
    Array<KIPlayer *> *targets = F<Array<KIPlayer *> *>(self, 0x10);
    if (targets != 0 && index < (int)targets->length)
        return targets->data[index];
    return 0;
}

// ---- update_11bf62.cpp ----
// Route::update(Vector const&) -> tail-call update(v.x, v.y, v.z).
extern "C" void Route_update(Route *self, const Vector &v)
{
    return Route_update_xyz(self, v.x, v.y, v.z);
}

// ---- Route_11bb74.cpp ----
extern "C" void ArrayWaypoint_ctor(void *a);   // Array<Waypoint*>::Array
extern "C" void ArrayKIPlayer_ctor(void *a);   // Array<KIPlayer*>::Array
extern "C" void ArrayInt_ctor(void *a);        // Array<int>::Array

// Route::Route(int *coords, int count) -- count triples of (x,y,z) define the waypoints.
extern "C" Route *_ZN5RouteC2EPii(Route *self, int *coords, int count)
{
    self->f_4 = 0;
    self->f_0 = 0;
    void *wps = operator_new(0xc);
    ArrayWaypoint_ctor(wps);
    self->f_c = wps;
    void *tgts = operator_new(0xc);
    ArrayKIPlayer_ctor(tgts);
    self->f_10 = tgts;
    void *times = operator_new(0xc);
    ArrayInt_ctor(times);
    self->f_14 = times;
    uint32_t n = __aeabi_idiv(count, 3);
    ArraySetLengthKIPlayer(n, self->f_10);
    ArraySetLengthInt(n, self->f_14);
    for (int i = 0; i < count; i += 3) {
        void *wp = operator_new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->f_c);
    }
    return self;
}

// ---- getLastWaypoint_11bede.cpp ----
// Route::getLastWaypoint() -> last waypoint in the path.
extern "C" Waypoint *Route_getLastWaypoint(Route *self)
{
    return Route_getWaypointAt(self, (int)F<Array<Waypoint *> *>(self, 0xc)->length - 1);
}

// ---- translate_11c1f6.cpp ----
// Route::translate(Vector const&) -> shift every waypoint's coords by the given vector.
extern "C" void Route_translate(Route *self, const Vector &v)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    float vx = v.x, vy = v.y, vz = v.z;
    for (uint32_t i = 0; i != wps->length; i++) {
        Waypoint *wp = wps->data[i];
        wp->field_0x124 = (int)((float)wp->field_0x124 + vx);
        wp->field_0x128 = (int)((float)wp->field_0x128 + vy);
        wp->field_0x12c = (int)((float)wp->field_0x12c + vz);
    }
}

// ---- reachWaypoint_11bef6.cpp ----
extern "C" void Waypoint_activate(Waypoint *wp);   // veneer tail-call target

// Route::reachWaypoint(int) -> advance to the given waypoint, wrapping/resetting when looping.
extern "C" void Route_reachWaypoint(Route *self, int index)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    uint32_t len = wps->length;
    if (self->f_0 < (int)(len - 1)) {
        self->f_0 = index + 1;
    } else if (self->f_4 != 0) {
        self->f_0 = 0;
        for (uint32_t i = 0; i < len; i++) {
            Waypoint_reset(F<Array<Waypoint *> *>(self, 0xc)->data[i]);
            len = F<Array<Waypoint *> *>(self, 0xc)->length;
        }
        Waypoint_setActive(F<Array<Waypoint *> *>(self, 0xc)->data[0], true);
    }
    Waypoint_setActive(F<Array<Waypoint *> *>(self, 0xc)->data[index], false);
    return Waypoint_activate(F<Array<Waypoint *> *>(self, 0xc)->data[index]);
}

// ---- Route_11bc56.cpp ----
extern "C" void ArrayWaypoint_ctor(void *a);   // Array<Waypoint*>::Array
extern "C" void ArrayInt_ctor(void *a);        // Array<int>::Array

// Route::Route(int *coords, Array<KIPlayer*> *targets, int *times, int count)
extern "C" Route *_ZN5RouteC2EPiPvPii(Route *self, int *coords, void *targets, int *times, int count)
{
    self->f_4 = 0;
    self->f_0 = 0;
    void *wps = operator_new(0xc);
    ArrayWaypoint_ctor(wps);
    self->f_c = wps;
    void *timesArr = operator_new(0xc);
    ArrayInt_ctor(timesArr);
    self->f_10 = targets;
    self->f_14 = timesArr;
    for (int i = 0; i < count; i += 3)
        ArrayAddInt(*times++, self->f_14);
    for (int i = 0; i < count; i += 3) {
        void *wp = operator_new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->f_c);
    }
    return self;
}

// ---- clone_11c0b6.cpp ----
extern "C" void *operator_new__(unsigned int sz);   // operator new[]
extern "C" void operator_delete__(void *p);          // operator delete[]

// Route::clone() -> deep copy of the path; preserves docking targets/times when any are set.
extern "C" Route *Route_clone(Route *self)
{
    Array<Waypoint *> *wps = F<Array<Waypoint *> *>(self, 0xc);
    int n = (int)wps->length;
    int *coords = (int *)operator_new__(n * 3 * 4);
    int *p = coords;
    for (int i = 0; i != n; i++) {
        Waypoint *wp = wps->data[i];
        p[0] = wp->field_0x124;
        p[1] = wp->field_0x128;
        p[2] = wp->field_0x12c;
        p += 3;
    }
    Array<int> *tgt = F<Array<int> *>(self, 0x10);
    if (tgt != 0) {
        bool any = false;
        for (uint32_t k = 0; k < tgt->length; k++)
            if (tgt->data[k] != 0)
                any = true;
        if (any) {
            Array<int> *times = F<Array<int> *>(self, 0x14);
            int *timesCopy = (int *)operator_new__(times->length * 4);
            for (uint32_t k = 0; k < times->length; k++)
                timesCopy[k] = times->data[k];
            void *targets = operator_new(0xc);
            ArrayKIPlayer_ctor(targets);
            ArraySetLengthKIPlayer(F<Array<void *> *>(self, 0x10)->length, targets);
            for (uint32_t k = 0; k < F<Array<void *> *>(self, 0x10)->length; k++)
                ((void **)*(void **)((char *)targets + 4))[k] = F<Array<void *> *>(self, 0x10)->data[k];
            Route *r = (Route *)operator_new(0x18);
            Route_ctor2(r, coords, targets, timesCopy, (int)F<Array<Waypoint *> *>(self, 0xc)->length * 3);
            r->f_4 = self->f_4;
            operator_delete__(timesCopy);
            return r;
        }
    }
    Route *r = (Route *)operator_new(0x18);
    Route_ctor1(r, coords, wps->length * 3);
    r->f_4 = self->f_4;
    return r;
}

// ---- getWaypoint_11be34.cpp ----
// KIPlayer's vtable slot at +0x28 writes its current position (3 floats) to *out.
typedef void (*GetPosFn)(void *out);

static inline void getPos(void *kip, void *out)
{
    (*(*(GetPosFn **)kip + 0xa))(out);
}

// Route::getWaypoint(int) -> snap the waypoint's stored coords to its docking target's position.
// The on-stack position buffer makes the compiler emit the stack-protector canary.
extern "C" Waypoint *Route_getWaypointAt(Route *self, int index)
{
    char pos[12];
    Waypoint *wp = 0;
    if ((int)F<Array<Waypoint *> *>(self, 0xc)->length > index &&
        (wp = F<Array<Waypoint *> *>(self, 0xc)->data[index]) != 0) {
        void *k = F<Array<void *> *>(self, 0x10)->data[index];
        if (k != 0) {
            getPos(k, pos);
            wp->field_0x124 = (int)*(float *)(pos + 0);
            getPos(F<Array<void *> *>(self, 0x10)->data[index], pos);
            wp->field_0x128 = (int)*(float *)(pos + 4);
            getPos(F<Array<void *> *>(self, 0x10)->data[index], pos);
            wp->field_0x12c = (int)*(float *)(pos + 8);
        }
    }
    return wp;
}

// ---- update_11bf70.cpp ----
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
    float dx = x - (float)wp->field_0x124;
    if (!(dx < 2000.0f) || !(dx > -2000.0f))
        return x;
    float dy = y - (float)wp->field_0x128;
    if (!(dy < 2000.0f) || !(dy > -2000.0f))
        return x;
    float dz = z - (float)wp->field_0x12c;
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

// ---- getDockingTime_11be16.cpp ----
// Route::getDockingTime() -> docking time at the current index.
extern "C" int Route_getDockingTime(Route *self)
{
    return Route_getDockingTimeAt(self, self->f_0);
}
