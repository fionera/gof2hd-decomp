#include "gof2/game/world/Route.h"
#include "gof2/game/world/Waypoint.h"

extern "C" int __aeabi_idiv(int n, int d);
extern "C" void Waypoint_ctor(void *wp, int x, int y, int z, Route *route);
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count);
extern "C" Route *Route_ctor1(Route *self, int *coords, int count);

// Route::waypointDefined() -> whether the waypoint array has been allocated.
bool Route::waypointDefined() {
    return this->field_0xc != 0;
}

// Route::length() -> number of waypoints.
int Route::length() {
    return (int)this->field_0xc->size();
}

// Route::reset() -> reset every waypoint and rewind to the start.
void Route::reset() {
    for (uint32_t i = 0; i < this->field_0xc->size(); i++)
        ((Waypoint *)((*this->field_0xc)[i]))->reset();
    this->field_0x0 = 0;
}

// Route::getWaypoint() -> waypoint at the current index.
Waypoint * Route::getWaypoint() {
    return ((Route *)(this))->getWaypointAt(this->field_0x0);
}

// Route::getExactClone() -> a clone whose reached-waypoints are marked, sharing the current index.
Route * Route::getExactClone() {
    Route *result = ((Route *)(this))->clone();
    for (uint32_t i = 0; i < result->field_0xc->size(); i++) {
        if ((*this->field_0xc)[i]->state != 0)
            ((Waypoint *)((*result->field_0xc)[i]))->reached();
    }
    result->field_0x0 = this->field_0x0;
    return result;
}

// Route::getDockingTarget() -> docking target at the current index, or 0.
KIPlayer * Route::getDockingTarget() {
    Array<KIPlayer *> *targets = this->field_0x10;
    int index = this->field_0x0;
    if (targets != 0 && (int)targets->size() > index)
        return (*targets)[index];
    return 0;
}

// Route::getDockingTime(int) -> docking time at the given index, or 0.
int Route::getDockingTimeAt(int index) {
    if (this->field_0x10 != 0) {
        Array<int> *times = this->field_0x14;
        if (index < (int)times->size())
            return (*times)[index];
    }
    return 0;
}

// Route::setNewCoords(Vector) -> overwrite the first waypoint's target coordinates.
void Route::setNewCoords(float x, float y, float z) {
    Waypoint *wp = (*this->field_0xc)[0];
    wp->x = (int)x;
    wp->y = (int)y;
    wp->z = (int)z;
}

// Route::~Route() (D2). Returns this.
void *_ZN5RouteD2Ev(Route *self)
{
    if (self->field_0xc != 0) {
        for (Waypoint *wp : *self->field_0xc)
            delete wp;
        self->field_0xc->clear();
        delete self->field_0xc;
    }
    self->field_0xc = 0;

    delete self->field_0x10;
    self->field_0x10 = 0;

    delete self->field_0x14;
    self->field_0x14 = 0;

    return self;
}

// Route::getDockingTarget(int) -> docking target at the given index, or 0.
KIPlayer * Route::getDockingTarget_i(int index) {
    Array<KIPlayer *> *targets = this->field_0x10;
    if (targets != 0 && index < (int)targets->size())
        return (*targets)[index];
    return 0;
}

// Route::update(Vector const&) -> tail-call update(v.x, v.y, v.z).
void Route::update(const Vector &v) {
    ((Route *)(this))->update_xyz(v.x, v.y, v.z);
}

// Route::Route(int *coords, int count) -- count triples of (x,y,z) define the waypoints.
Route *_ZN5RouteC2EPii(Route *self, int *coords, int count)
{
    self->field_0x4 = 0;
    self->field_0x0 = 0;
    self->field_0xc = new Array<Waypoint *>();
    self->field_0x10 = new Array<KIPlayer *>();
    self->field_0x14 = new Array<int>();
    uint32_t n = __aeabi_idiv(count, 3);
    self->field_0x10->resize(n);
    self->field_0x14->resize(n);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        self->field_0xc->push_back((Waypoint *)wp);
    }
    return self;
}

// Route::getLastWaypoint() -> last waypoint in the path.
Waypoint * Route::getLastWaypoint() {
    return ((Route *)(this))->getWaypointAt((int)this->field_0xc->size() - 1);
}

// Route::translate(Vector const&) -> shift every waypoint's coords by the given vector.
void Route::translate(const Vector &v) {
    Array<Waypoint *> *wps = this->field_0xc;
    float vx = v.x, vy = v.y, vz = v.z;
    for (uint32_t i = 0; i != wps->size(); i++) {
        Waypoint *wp = (*wps)[i];
        wp->x = (int)((float)wp->x + vx);
        wp->y = (int)((float)wp->y + vy);
        wp->z = (int)((float)wp->z + vz);
    }
}

// veneer tail-call target

// Route::reachWaypoint(int) -> advance to the given waypoint, wrapping/resetting when looping.
void Route::reachWaypoint(int index) {
    Array<Waypoint *> *wps = this->field_0xc;
    uint32_t len = wps->size();
    if (this->field_0x0 < (int)(len - 1)) {
        this->field_0x0 = index + 1;
    } else if (this->field_0x4 != 0) {
        this->field_0x0 = 0;
        for (uint32_t i = 0; i < len; i++) {
            ((Waypoint *)((*this->field_0xc)[i]))->reset();
            len = this->field_0xc->size();
        }
        ((Waypoint *)((*this->field_0xc)[0]))->setActive(true);
    }
    ((Waypoint *)((*this->field_0xc)[index]))->setActive(false);
    return ((Waypoint *)((*this->field_0xc)[index]))->activate();
}

// Route::Route(int *coords, Array<KIPlayer*> *targets, int *times, int count)
Route *_ZN5RouteC2EPiPvPii(Route *self, int *coords, void *targets, int *times, int count)
{
    self->field_0x4 = 0;
    self->field_0x0 = 0;
    self->field_0xc = new Array<Waypoint *>();
    self->field_0x10 = (Array<KIPlayer *> *)targets;
    self->field_0x14 = new Array<int>();
    for (int i = 0; i < count; i += 3)
        self->field_0x14->push_back(*times++);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        self->field_0xc->push_back((Waypoint *)wp);
    }
    return self;
}

// Route::clone() -> deep copy of the path; preserves docking targets/times when any are set.
Route * Route::clone() {
    Array<Waypoint *> *wps = this->field_0xc;
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
    Array<KIPlayer *> *tgt = this->field_0x10;
    if (tgt != 0) {
        bool any = false;
        for (uint32_t k = 0; k < tgt->size(); k++)
            if ((*tgt)[k] != 0)
                any = true;
        if (any) {
            Array<int> *times = this->field_0x14;
            int *timesCopy = (int *)::operator new[](times->size() * 4);
            for (uint32_t k = 0; k < times->size(); k++)
                timesCopy[k] = (*times)[k];
            Array<KIPlayer *> *targetsArr = new Array<KIPlayer *>();
            targetsArr->resize(this->field_0x10->size());
            for (uint32_t k = 0; k < this->field_0x10->size(); k++)
                (*targetsArr)[k] = (*this->field_0x10)[k];
            Route *r = (Route *)::operator new(0x18);
            Route_ctor2(r, coords, targetsArr, timesCopy, (int)this->field_0xc->size() * 3);
            r->field_0x4 = this->field_0x4;
            ::operator delete[](timesCopy);
            return r;
        }
    }
    Route *r = (Route *)::operator new(0x18);
    Route_ctor1(r, coords, wps->size() * 3);
    r->field_0x4 = this->field_0x4;
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
    char pos[12];
    Waypoint *wp = 0;
    if ((int)this->field_0xc->size() > index &&
        (wp = (*this->field_0xc)[index]) != 0) {
        void *k = (void *)(*this->field_0x10)[index];
        if (k != 0) {
            getPos(k, pos);
            wp->x = (int)*(float *)(pos + 0);
            getPos((void *)(*this->field_0x10)[index], pos);
            wp->y = (int)*(float *)(pos + 4);
            getPos((void *)(*this->field_0x10)[index], pos);
            wp->z = (int)*(float *)(pos + 8);
        }
    }
    return wp;
}

// veneer tail-call target

// Route::update(float, float, float) -> if close enough to the active waypoint, advance to the next.
float Route::update_xyz(float x, float y, float z) {
    int idx = this->field_0x0;
    Array<Waypoint *> *wps = this->field_0xc;
    if (idx >= (int)wps->size())
        return x;
    if ((*this->field_0x10)[idx] != 0)
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
    ((Waypoint *)((*this->field_0xc)[this->field_0x0]))->reached();
    int cur = this->field_0x0;
    Array<Waypoint *> *w = this->field_0xc;
    int next = cur + 1;
    this->field_0x0 = next;
    uint32_t len = w->size();
    if (this->field_0x4 != 0 && (int)(len - 1) <= cur) {
        this->field_0x0 = 0;
        for (uint32_t i = 0; i < len; i++) {
            ((Waypoint *)((*w)[i]))->reset();
            w = this->field_0xc;
            len = w->size();
        }
        next = this->field_0x0;
    }
    if (next < (int)len)
        return ((Waypoint *)((*w)[next]))->advance(true);
    return x;
}

// Route::getDockingTime() -> docking time at the current index.
int Route::getDockingTime() {
    return ((Route *)(this))->getDockingTimeAt(this->field_0x0);
}

// ---- Route(int*, int) — build a route from a flat [x,y,z, x,y,z, ...] coord array.
// ctor1: coords + count(=3*numWaypoints). Allocates empty docking-target/time arrays.
Route * Route::ctor(int *coords, int count) {
    this->field_0x4 = 0;
    this->field_0x0 = 0;
    this->field_0xc = new Array<Waypoint *>();
    this->field_0x10 = new Array<KIPlayer *>();
    this->field_0x14 = new Array<int>();
    uint32_t n = __aeabi_idiv(count, 3);
    this->field_0x10->resize(n);
    this->field_0x14->resize(n);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], this);
        this->field_0xc->push_back((Waypoint *)wp);
    }
    return this;
}

// ---- Route(int*, Array<KIPlayer*>*, int*, int) — coords + pre-built docking targets + times.
// ctor2: the docking-target array is adopted as-is; docking times are copied in.
Route * Route::ctorWithTargets(int *coords, Array<KIPlayer *> *targets, int *times, int count) {
    this->field_0x4 = 0;
    this->field_0x0 = 0;
    this->field_0xc = new Array<Waypoint *>();
    this->field_0x10 = targets;
    this->field_0x14 = new Array<int>();
    for (int i = 0; i < count; i += 3)
        this->field_0x14->push_back(times[i / 3]);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], this);
        this->field_0xc->push_back((Waypoint *)wp);
    }
    return this;
}

// ---- ~Route() — release the waypoint array (deep) and the docking target/time arrays.
Route * Route::dtor() {
    if (this->field_0xc != 0) {
        for (Waypoint *wp : *this->field_0xc)
            delete wp;
        this->field_0xc->clear();
        delete this->field_0xc;
    }
    this->field_0xc = 0;
    delete this->field_0x10;
    this->field_0x10 = 0;
    delete this->field_0x14;
    this->field_0x14 = 0;
    return this;
}

// ---- getCurrent() — index of the active waypoint.
int Route::getCurrent() {
    return this->field_0x0;
}

// ---- setLoop(bool) — toggle whether the route restarts after the final waypoint.
void Route::setLoop(bool loop) {
    this->field_0x4 = loop;
}

// ---- C-ABI ctor/dtor shims ----
// Route's clone helpers allocate the Route storage and then call one of these in
// place. They forward to the real constructors/destructor on the given object and
// return it so the caller can chain (and, for the dtor, hand the storage to
// operator delete).

// Route_ctor1 — coords + count(=3*numWaypoints), empty docking-target/time arrays.
extern "C" Route *Route_ctor1(Route *self, int *coords, int count)
{
    return self->ctor(coords, count);
}

// Route_ctor2 — coords + adopted docking-target array + copied docking times.
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count)
{
    return self->ctorWithTargets(coords, (Array<KIPlayer *> *)targets, times, count);
}
