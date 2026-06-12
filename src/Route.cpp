#include "gof2/Route.h"
#include "gof2/Waypoint.h"


extern "C" void ArrayReleaseClasses_Waypoint(Array<Waypoint *> *a);
extern "C" void *ArrayWaypoint_dtor(Array<Waypoint *> *a);
extern "C" void *ArrayKIPlayer_dtor(Array<KIPlayer *> *a);
extern "C" void *ArrayInt_dtor(Array<int> *a);
extern "C" int __aeabi_idiv(int n, int d);
extern "C" void ArraySetLengthKIPlayer(uint32_t n, void *a);
extern "C" void ArraySetLengthInt(uint32_t n, void *a);
extern "C" void Waypoint_ctor(void *wp, int x, int y, int z, Route *route);
extern "C" void ArrayAddWaypoint(void *wp, void *a);
extern "C" void ArrayAddInt(int v, void *a);
extern "C" void ArrayKIPlayer_ctor(void *a);
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count);
extern "C" Route *Route_ctor1(Route *self, int *coords, int count);

// Route::waypointDefined() -> whether the waypoint array has been allocated.
bool Route::waypointDefined() {
    Route *self = this;
    return self->field_0xc != 0;
}

// Route::length() -> number of waypoints.
int Route::length() {
    Route *self = this;
    return (int)self->field_0xc->size();
}

// Route::reset() -> reset every waypoint and rewind to the start.
void Route::reset() {
    Route *self = this;
    for (uint32_t i = 0; i < self->field_0xc->size(); i++)
        ((Waypoint *)((*self->field_0xc)[i]))->reset();
    self->field_0x0 = 0;
}

// Route::getWaypoint() -> waypoint at the current index.
Waypoint * Route::getWaypoint() {
    Route *self = this;
    return ((Route *)(self))->getWaypointAt(self->field_0x0);
}

// Route::getExactClone() -> a clone whose reached-waypoints are marked, sharing the current index.
Route * Route::getExactClone() {
    Route *self = this;
    Route *result = ((Route *)(self))->clone();
    for (uint32_t i = 0; i < result->field_0xc->size(); i++) {
        if ((*self->field_0xc)[i]->state != 0)
            ((Waypoint *)((*result->field_0xc)[i]))->reached();
    }
    result->field_0x0 = self->field_0x0;
    return result;
}

// Route::getDockingTarget() -> docking target at the current index, or 0.
KIPlayer * Route::getDockingTarget() {
    Route *self = this;
    Array<KIPlayer *> *targets = self->field_0x10;
    int index = self->field_0x0;
    if (targets != 0 && (int)targets->size() > index)
        return (*targets)[index];
    return 0;
}

// Route::getDockingTime(int) -> docking time at the given index, or 0.
int Route::getDockingTimeAt(int index) {
    Route *self = this;
    if (self->field_0x10 != 0) {
        Array<int> *times = self->field_0x14;
        if (index < (int)times->size())
            return (*times)[index];
    }
    return 0;
}

// Route::setNewCoords(Vector) -> overwrite the first waypoint's target coordinates.
void Route::setNewCoords(float x, float y, float z) {
    Route *self = this;
    Waypoint *wp = (*self->field_0xc)[0];
    wp->x = (int)x;
    wp->y = (int)y;
    wp->z = (int)z;
}

// Route::~Route() (D2). Returns this.
void *_ZN5RouteD2Ev(Route *self)
{
    Array<Waypoint *> *wps = self->field_0xc;
    if (wps != 0) {
        ArrayReleaseClasses_Waypoint(wps);
        Array<Waypoint *> *wps2 = self->field_0xc;
        if (wps2 != 0)
            ::operator delete(ArrayWaypoint_dtor(wps2));
    }
    self->field_0xc = 0;

    Array<KIPlayer *> *tgt = self->field_0x10;
    if (tgt != 0)
        ::operator delete(ArrayKIPlayer_dtor(tgt));
    self->field_0x10 = 0;

    Array<int> *times = self->field_0x14;
    if (times != 0)
        ::operator delete(ArrayInt_dtor(times));
    self->field_0x14 = 0;

    return self;
}

// Route::getDockingTarget(int) -> docking target at the given index, or 0.
KIPlayer * Route::getDockingTarget_i(int index) {
    Route *self = this;
    Array<KIPlayer *> *targets = self->field_0x10;
    if (targets != 0 && index < (int)targets->size())
        return (*targets)[index];
    return 0;
}

// Route::update(Vector const&) -> tail-call update(v.x, v.y, v.z).
void Route::update(const Vector &v) {
    Route *self = this;
    ((Route *)(self))->update_xyz(v.x, v.y, v.z);
}

extern "C" void ArrayWaypoint_ctor(void *a);   // Array<Waypoint*>::Array
extern "C" void ArrayKIPlayer_ctor(void *a);   // Array<KIPlayer*>::Array
extern "C" void ArrayInt_ctor(void *a);        // Array<int>::Array

// Route::Route(int *coords, int count) -- count triples of (x,y,z) define the waypoints.
Route *_ZN5RouteC2EPii(Route *self, int *coords, int count)
{
    self->field_0x4 = 0;
    self->field_0x0 = 0;
    void *wps = ::operator new(0xc);
    ArrayWaypoint_ctor(wps);
    self->field_0xc = (Array<Waypoint *> *)wps;
    void *tgts = ::operator new(0xc);
    ArrayKIPlayer_ctor(tgts);
    self->field_0x10 = (Array<KIPlayer *> *)tgts;
    void *times = ::operator new(0xc);
    ArrayInt_ctor(times);
    self->field_0x14 = (Array<int> *)times;
    uint32_t n = __aeabi_idiv(count, 3);
    ArraySetLengthKIPlayer(n, self->field_0x10);
    ArraySetLengthInt(n, self->field_0x14);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->field_0xc);
    }
    return self;
}

// Route::getLastWaypoint() -> last waypoint in the path.
Waypoint * Route::getLastWaypoint() {
    Route *self = this;
    return ((Route *)(self))->getWaypointAt((int)self->field_0xc->size() - 1);
}

// Route::translate(Vector const&) -> shift every waypoint's coords by the given vector.
void Route::translate(const Vector &v) {
    Route *self = this;
    Array<Waypoint *> *wps = self->field_0xc;
    float vx = v.x, vy = v.y, vz = v.z;
    for (uint32_t i = 0; i != wps->size(); i++) {
        Waypoint *wp = (*wps)[i];
        wp->x = (int)((float)wp->x + vx);
        wp->y = (int)((float)wp->y + vy);
        wp->z = (int)((float)wp->z + vz);
    }
}

extern "C" void Waypoint_activate(Waypoint *wp);   // veneer tail-call target

// Route::reachWaypoint(int) -> advance to the given waypoint, wrapping/resetting when looping.
void Route::reachWaypoint(int index) {
    Route *self = this;
    Array<Waypoint *> *wps = self->field_0xc;
    uint32_t len = wps->size();
    if (self->field_0x0 < (int)(len - 1)) {
        self->field_0x0 = index + 1;
    } else if (self->field_0x4 != 0) {
        self->field_0x0 = 0;
        for (uint32_t i = 0; i < len; i++) {
            ((Waypoint *)((*self->field_0xc)[i]))->reset();
            len = self->field_0xc->size();
        }
        ((Waypoint *)((*self->field_0xc)[0]))->setActive(true);
    }
    ((Waypoint *)((*self->field_0xc)[index]))->setActive(false);
    return Waypoint_activate((*self->field_0xc)[index]);
}

// Route::Route(int *coords, Array<KIPlayer*> *targets, int *times, int count)
Route *_ZN5RouteC2EPiPvPii(Route *self, int *coords, void *targets, int *times, int count)
{
    self->field_0x4 = 0;
    self->field_0x0 = 0;
    void *wps = ::operator new(0xc);
    ArrayWaypoint_ctor(wps);
    self->field_0xc = (Array<Waypoint *> *)wps;
    void *timesArr = ::operator new(0xc);
    ArrayInt_ctor(timesArr);
    self->field_0x10 = (Array<KIPlayer *> *)targets;
    self->field_0x14 = (Array<int> *)timesArr;
    for (int i = 0; i < count; i += 3)
        ArrayAddInt(*times++, self->field_0x14);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->field_0xc);
    }
    return self;
}


// Route::clone() -> deep copy of the path; preserves docking targets/times when any are set.
Route * Route::clone() {
    Route *self = this;
    Array<Waypoint *> *wps = self->field_0xc;
    int n = (int)wps->size();
    int *coords = (int *)::operator new[](n * 3 * 4);
    int *p = coords;
    for (int i = 0; i != n; i++) {
        Waypoint *wp = (*wps)[i];
        p[0] = wp->x;
        p[1] = wp->y;
        p[2] = wp->z;
        p += 3;
    }
    Array<KIPlayer *> *tgt = self->field_0x10;
    if (tgt != 0) {
        bool any = false;
        for (uint32_t k = 0; k < tgt->size(); k++)
            if ((*tgt)[k] != 0)
                any = true;
        if (any) {
            Array<int> *times = self->field_0x14;
            int *timesCopy = (int *)::operator new[](times->size() * 4);
            for (uint32_t k = 0; k < times->size(); k++)
                timesCopy[k] = (*times)[k];
            void *targets = ::operator new(0xc);
            ArrayKIPlayer_ctor(targets);
            ArraySetLengthKIPlayer(self->field_0x10->size(), targets);
            Array<KIPlayer *> *targetsArr = (Array<KIPlayer *> *)targets;
            for (uint32_t k = 0; k < self->field_0x10->size(); k++)
                (*targetsArr)[k] = (*self->field_0x10)[k];
            Route *r = (Route *)::operator new(0x18);
            Route_ctor2(r, coords, targets, timesCopy, (int)self->field_0xc->size() * 3);
            r->field_0x4 = self->field_0x4;
            ::operator delete[](timesCopy);
            return r;
        }
    }
    Route *r = (Route *)::operator new(0x18);
    Route_ctor1(r, coords, wps->size() * 3);
    r->field_0x4 = self->field_0x4;
    return r;
}

// KIPlayer's vtable slot at +0x28 writes its current position (3 floats) to *out.
typedef void (*GetPosFn)(void *out);

static inline void getPos(void *kip, void *out)
{
    (*(*(GetPosFn **)kip + 0xa))(out);
}

// Route::getWaypoint(int) -> snap the waypoint's stored coords to its docking target's position.
// The on-stack position buffer makes the compiler emit the stack-protector canary.
Waypoint * Route::getWaypointAt(int index) {
    Route *self = this;
    char pos[12];
    Waypoint *wp = 0;
    if ((int)self->field_0xc->size() > index &&
        (wp = (*self->field_0xc)[index]) != 0) {
        void *k = (void *)(*self->field_0x10)[index];
        if (k != 0) {
            getPos(k, pos);
            wp->x = (int)*(float *)(pos + 0);
            getPos((void *)(*self->field_0x10)[index], pos);
            wp->y = (int)*(float *)(pos + 4);
            getPos((void *)(*self->field_0x10)[index], pos);
            wp->z = (int)*(float *)(pos + 8);
        }
    }
    return wp;
}

extern "C" float Waypoint_advance(Waypoint *wp, bool b);   // veneer tail-call target

// Route::update(float, float, float) -> if close enough to the active waypoint, advance to the next.
float Route::update_xyz(float x, float y, float z) {
    Route *self = this;
    int idx = self->field_0x0;
    Array<Waypoint *> *wps = self->field_0xc;
    if (idx >= (int)wps->size())
        return x;
    if ((*self->field_0x10)[idx] != 0)
        return x;
    Waypoint *wp = (*wps)[idx];
    float dx = x - (float)wp->x;
    if (!(dx < 2000.0f) || !(dx > -2000.0f))
        return x;
    float dy = y - (float)wp->y;
    if (!(dy < 2000.0f) || !(dy > -2000.0f))
        return x;
    float dz = z - (float)wp->z;
    if (!(dz < 2000.0f) || !(dz > -2000.0f))
        return x;

    ((Waypoint *)(wp))->setActive(false);
    ((Waypoint *)((*self->field_0xc)[self->field_0x0]))->reached();
    int cur = self->field_0x0;
    Array<Waypoint *> *w = self->field_0xc;
    int next = cur + 1;
    self->field_0x0 = next;
    uint32_t len = w->size();
    if (self->field_0x4 != 0 && (int)(len - 1) <= cur) {
        self->field_0x0 = 0;
        for (uint32_t i = 0; i < len; i++) {
            ((Waypoint *)((*w)[i]))->reset();
            w = self->field_0xc;
            len = w->size();
        }
        next = self->field_0x0;
    }
    if (next < (int)len)
        return Waypoint_advance((*w)[next], true);
    return x;
}

// Route::getDockingTime() -> docking time at the current index.
int Route::getDockingTime() {
    Route *self = this;
    return ((Route *)(self))->getDockingTimeAt(self->field_0x0);
}

// ---- Route(int*, int) — build a route from a flat [x,y,z, x,y,z, ...] coord array.
// ctor1: coords + count(=3*numWaypoints). Allocates empty docking-target/time arrays.
Route * Route::ctor(int *coords, int count) {
    Route *self = this;
    self->field_0x4 = 0;
    self->field_0x0 = 0;
    void *wps = ::operator new(0xc);
    ArrayWaypoint_ctor(wps);
    self->field_0xc = (Array<Waypoint *> *)wps;
    void *tgts = ::operator new(0xc);
    ArrayKIPlayer_ctor(tgts);
    self->field_0x10 = (Array<KIPlayer *> *)tgts;
    void *times = ::operator new(0xc);
    ArrayInt_ctor(times);
    self->field_0x14 = (Array<int> *)times;
    uint32_t n = __aeabi_idiv(count, 3);
    ArraySetLengthKIPlayer(n, self->field_0x10);
    ArraySetLengthInt(n, self->field_0x14);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->field_0xc);
    }
    return self;
}

// ---- Route(int*, Array<KIPlayer*>*, int*, int) — coords + pre-built docking targets + times.
// ctor2: the docking-target array is adopted as-is; docking times are copied in.
Route * Route::ctorWithTargets(int *coords, Array<KIPlayer *> *targets, int *times, int count) {
    Route *self = this;
    self->field_0x4 = 0;
    self->field_0x0 = 0;
    void *wps = ::operator new(0xc);
    ArrayWaypoint_ctor(wps);
    self->field_0xc = (Array<Waypoint *> *)wps;
    void *timesArr = ::operator new(0xc);
    ArrayInt_ctor(timesArr);
    self->field_0x10 = targets;
    self->field_0x14 = (Array<int> *)timesArr;
    for (int i = 0; i < count; i += 3)
        ArrayAddInt(times[i / 3], self->field_0x14);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        ArrayAddWaypoint(wp, self->field_0xc);
    }
    return self;
}

// ---- ~Route() — release the waypoint array (deep) and the docking target/time arrays.
Route * Route::dtor() {
    Route *self = this;
    if (self->field_0xc != 0) {
        ArrayReleaseClasses_Waypoint(self->field_0xc);
        if (self->field_0xc != 0)
            ::operator delete(ArrayWaypoint_dtor(self->field_0xc));
    }
    self->field_0xc = 0;
    if (self->field_0x10 != 0)
        ::operator delete(ArrayKIPlayer_dtor(self->field_0x10));
    self->field_0x10 = 0;
    if (self->field_0x14 != 0)
        ::operator delete(ArrayInt_dtor(self->field_0x14));
    self->field_0x14 = 0;
    return self;
}

// ---- getCurrent() — index of the active waypoint.
int Route::getCurrent() {
    return this->field_0x0;
}

// ---- setLoop(bool) — toggle whether the route restarts after the final waypoint.
void Route::setLoop(bool loop) {
    this->field_0x4 = loop;
}
