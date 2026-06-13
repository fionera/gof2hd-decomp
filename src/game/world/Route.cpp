#include "gof2/game/world/Route.h"
#include "gof2/game/world/Waypoint.h"

extern "C" int __aeabi_idiv(int n, int d);
extern "C" void Waypoint_ctor(void *wp, int x, int y, int z, Route *route);
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count);
extern "C" Route *Route_ctor1(Route *self, int *coords, int count);

// Route::waypointDefined() -> whether the waypoint array has been allocated.
bool Route::waypointDefined() {
    return this->waypoints != 0;
}

// Route::length() -> number of waypoints.
int Route::length() {
    return (int)this->waypoints->size();
}

// Route::reset() -> reset every waypoint and rewind to the start.
void Route::reset() {
    for (uint32_t i = 0; i < this->waypoints->size(); i++)
        ((Waypoint *)((*this->waypoints)[i]))->reset();
    this->currentIndex = 0;
}

// Route::getWaypoint() -> waypoint at the current index.
Waypoint * Route::getWaypoint() {
    return ((Route *)(this))->getWaypointAt(this->currentIndex);
}

// Route::getExactClone() -> a clone whose reached-waypoints are marked, sharing the current index.
Route * Route::getExactClone() {
    Route *result = ((Route *)(this))->clone();
    for (uint32_t i = 0; i < result->waypoints->size(); i++) {
        if ((*this->waypoints)[i]->state != 0)
            ((Waypoint *)((*result->waypoints)[i]))->reached();
    }
    result->currentIndex = this->currentIndex;
    return result;
}

// Route::getDockingTarget() -> docking target at the current index, or 0.
KIPlayer * Route::getDockingTarget() {
    Array<KIPlayer *> *targets = this->dockingTargets;
    int index = this->currentIndex;
    if (targets != 0 && (int)targets->size() > index)
        return (*targets)[index];
    return 0;
}

// Route::getDockingTime(int) -> docking time at the given index, or 0.
int Route::getDockingTimeAt(int index) {
    if (this->dockingTargets != 0) {
        Array<int> *times = this->dockingTimes;
        if (index < (int)times->size())
            return (*times)[index];
    }
    return 0;
}

// Route::setNewCoords(Vector) -> overwrite the first waypoint's target coordinates.
void Route::setNewCoords(float x, float y, float z) {
    Waypoint *wp = (*this->waypoints)[0];
    wp->x = (int)x;
    wp->y = (int)y;
    wp->z = (int)z;
}

// Route::~Route() (D2). Returns this.
void *_ZN5RouteD2Ev(Route *self)
{
    if (self->waypoints != 0) {
        for (Waypoint *wp : *self->waypoints)
            delete wp;
        self->waypoints->clear();
        delete self->waypoints;
    }
    self->waypoints = 0;

    delete self->dockingTargets;
    self->dockingTargets = 0;

    delete self->dockingTimes;
    self->dockingTimes = 0;

    return self;
}

// Route::getDockingTarget(int) -> docking target at the given index, or 0.
KIPlayer * Route::getDockingTarget_i(int index) {
    Array<KIPlayer *> *targets = this->dockingTargets;
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
    self->loop = 0;
    self->currentIndex = 0;
    self->waypoints = new Array<Waypoint *>();
    self->dockingTargets = new Array<KIPlayer *>();
    self->dockingTimes = new Array<int>();
    uint32_t n = __aeabi_idiv(count, 3);
    self->dockingTargets->resize(n);
    self->dockingTimes->resize(n);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        self->waypoints->push_back((Waypoint *)wp);
    }
    return self;
}

// Route::getLastWaypoint() -> last waypoint in the path.
Waypoint * Route::getLastWaypoint() {
    return ((Route *)(this))->getWaypointAt((int)this->waypoints->size() - 1);
}

// Route::translate(Vector const&) -> shift every waypoint's coords by the given vector.
void Route::translate(const Vector &v) {
    Array<Waypoint *> *wps = this->waypoints;
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
    Array<Waypoint *> *wps = this->waypoints;
    uint32_t len = wps->size();
    if (this->currentIndex < (int)(len - 1)) {
        this->currentIndex = index + 1;
    } else if (this->loop != 0) {
        this->currentIndex = 0;
        for (uint32_t i = 0; i < len; i++) {
            ((Waypoint *)((*this->waypoints)[i]))->reset();
            len = this->waypoints->size();
        }
        ((Waypoint *)((*this->waypoints)[0]))->setActive(true);
    }
    ((Waypoint *)((*this->waypoints)[index]))->setActive(false);
    return ((Waypoint *)((*this->waypoints)[index]))->activate();
}

// Route::Route(int *coords, Array<KIPlayer*> *targets, int *times, int count)
Route *_ZN5RouteC2EPiPvPii(Route *self, int *coords, void *targets, int *times, int count)
{
    self->loop = 0;
    self->currentIndex = 0;
    self->waypoints = new Array<Waypoint *>();
    self->dockingTargets = (Array<KIPlayer *> *)targets;
    self->dockingTimes = new Array<int>();
    for (int i = 0; i < count; i += 3)
        self->dockingTimes->push_back(*times++);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], self);
        self->waypoints->push_back((Waypoint *)wp);
    }
    return self;
}

// Route::clone() -> deep copy of the path; preserves docking targets/times when any are set.
Route * Route::clone() {
    Array<Waypoint *> *wps = this->waypoints;
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
    Array<KIPlayer *> *tgt = this->dockingTargets;
    if (tgt != 0) {
        bool any = false;
        for (uint32_t k = 0; k < tgt->size(); k++)
            if ((*tgt)[k] != 0)
                any = true;
        if (any) {
            Array<int> *times = this->dockingTimes;
            int *timesCopy = (int *)::operator new[](times->size() * 4);
            for (uint32_t k = 0; k < times->size(); k++)
                timesCopy[k] = (*times)[k];
            Array<KIPlayer *> *targetsArr = new Array<KIPlayer *>();
            targetsArr->resize(this->dockingTargets->size());
            for (uint32_t k = 0; k < this->dockingTargets->size(); k++)
                (*targetsArr)[k] = (*this->dockingTargets)[k];
            Route *r = (Route *)::operator new(0x18);
            Route_ctor2(r, coords, targetsArr, timesCopy, (int)this->waypoints->size() * 3);
            r->loop = this->loop;
            ::operator delete[](timesCopy);
            return r;
        }
    }
    Route *r = (Route *)::operator new(0x18);
    Route_ctor1(r, coords, wps->size() * 3);
    r->loop = this->loop;
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
    if ((int)this->waypoints->size() > index &&
        (wp = (*this->waypoints)[index]) != 0) {
        void *k = (void *)(*this->dockingTargets)[index];
        if (k != 0) {
            getPos(k, pos);
            wp->x = (int)*(float *)(pos + 0);
            getPos((void *)(*this->dockingTargets)[index], pos);
            wp->y = (int)*(float *)(pos + 4);
            getPos((void *)(*this->dockingTargets)[index], pos);
            wp->z = (int)*(float *)(pos + 8);
        }
    }
    return wp;
}

// veneer tail-call target

// Route::update(float, float, float) -> if close enough to the active waypoint, advance to the next.
float Route::update_xyz(float x, float y, float z) {
    int idx = this->currentIndex;
    Array<Waypoint *> *wps = this->waypoints;
    if (idx >= (int)wps->size())
        return x;
    if ((*this->dockingTargets)[idx] != 0)
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
    ((Waypoint *)((*this->waypoints)[this->currentIndex]))->reached();
    int cur = this->currentIndex;
    Array<Waypoint *> *w = this->waypoints;
    int next = cur + 1;
    this->currentIndex = next;
    uint32_t len = w->size();
    if (this->loop != 0 && (int)(len - 1) <= cur) {
        this->currentIndex = 0;
        for (uint32_t i = 0; i < len; i++) {
            ((Waypoint *)((*w)[i]))->reset();
            w = this->waypoints;
            len = w->size();
        }
        next = this->currentIndex;
    }
    if (next < (int)len)
        return ((Waypoint *)((*w)[next]))->advance(true);
    return x;
}

// Route::getDockingTime() -> docking time at the current index.
int Route::getDockingTime() {
    return ((Route *)(this))->getDockingTimeAt(this->currentIndex);
}

// ---- Route(int*, int) — build a route from a flat [x,y,z, x,y,z, ...] coord array.
// ctor1: coords + count(=3*numWaypoints). Allocates empty docking-target/time arrays.
Route::Route(int *coords, int count) {
    this->loop = 0;
    this->currentIndex = 0;
    this->waypoints = new Array<Waypoint *>();
    this->dockingTargets = new Array<KIPlayer *>();
    this->dockingTimes = new Array<int>();
    uint32_t n = __aeabi_idiv(count, 3);
    this->dockingTargets->resize(n);
    this->dockingTimes->resize(n);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], this);
        this->waypoints->push_back((Waypoint *)wp);
    }
}

// ---- Route(int*, Array<KIPlayer*>*, int*, int) — coords + pre-built docking targets + times.
// ctor2: the docking-target array is adopted as-is; docking times are copied in.
Route::Route(int *coords, Array<KIPlayer *> *targets, int *times, int count) {
    this->loop = 0;
    this->currentIndex = 0;
    this->waypoints = new Array<Waypoint *>();
    this->dockingTargets = targets;
    this->dockingTimes = new Array<int>();
    for (int i = 0; i < count; i += 3)
        this->dockingTimes->push_back(times[i / 3]);
    for (int i = 0; i < count; i += 3) {
        void *wp = ::operator new(0x138);
        Waypoint_ctor(wp, coords[i], coords[i + 1], coords[i + 2], this);
        this->waypoints->push_back((Waypoint *)wp);
    }
}

// ---- ~Route() — release the waypoint array (deep) and the docking target/time arrays.
Route::~Route() {
    if (this->waypoints != 0) {
        for (Waypoint *wp : *this->waypoints)
            delete wp;
        this->waypoints->clear();
        delete this->waypoints;
    }
    this->waypoints = 0;
    delete this->dockingTargets;
    this->dockingTargets = 0;
    delete this->dockingTimes;
    this->dockingTimes = 0;
}

// ---- getCurrent() — index of the active waypoint.
int Route::getCurrent() {
    return this->currentIndex;
}

// ---- setLoop(bool) — toggle whether the route restarts after the final waypoint.
void Route::setLoop(bool loop) {
    this->loop = loop;
}

// ---- C-ABI ctor/dtor shims ----
// Route's clone helpers allocate the Route storage and then call one of these in
// place. They forward to the real constructors/destructor on the given object and
// return it so the caller can chain (and, for the dtor, hand the storage to
// operator delete).

// Route_ctor1 — coords + count(=3*numWaypoints), empty docking-target/time arrays.
extern "C" Route *Route_ctor1(Route *self, int *coords, int count)
{
    new (self) Route(coords, count);
    return self;
}

// Route_ctor2 — coords + adopted docking-target array + copied docking times.
extern "C" Route *Route_ctor2(Route *self, int *coords, void *targets, int *times, int count)
{
    new (self) Route(coords, (Array<KIPlayer *> *)targets, times, count);
    return self;
}
