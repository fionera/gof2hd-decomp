#include "game/world/Route.h"
#include "game/world/Waypoint.h"

// Route::waypointDefined() -> whether the waypoint array has been allocated.
bool Route::waypointDefined() {
    return this->waypoints != nullptr;
}

// Route::length() -> number of waypoints.
int Route::length() {
    return (int)this->waypoints->size();
}

// Route::reset() -> reset every waypoint and rewind to the start.
void Route::reset() {
    for (uint32_t i = 0; i < this->waypoints->size(); i++)
        (*this->waypoints)[i]->reset();
    this->currentIndex = 0;
}

// Route::getWaypoint() -> waypoint at the current index.
Waypoint *Route::getWaypoint() {
    return getWaypointAt(this->currentIndex);
}

// Route::getExactClone() -> a clone whose reached-waypoints are marked, sharing the current index.
Route *Route::getExactClone() {
    Route *result = clone();
    for (uint32_t i = 0; i < result->waypoints->size(); i++) {
        if ((*this->waypoints)[i]->state != 0)
            (*result->waypoints)[i]->reached();
    }
    result->currentIndex = this->currentIndex;
    return result;
}

// Route::getDockingTarget() -> docking target at the current index, or null.
KIPlayer *Route::getDockingTarget() {
    Array<KIPlayer *> *targets = this->dockingTargets;
    int index = this->currentIndex;
    if (targets != nullptr && (int)targets->size() > index)
        return (*targets)[index];
    return nullptr;
}

// Route::getDockingTarget(int) -> docking target at the given index, or null.
KIPlayer *Route::getDockingTarget(int index) {
    Array<KIPlayer *> *targets = this->dockingTargets;
    if (targets != nullptr && index < (int)targets->size())
        return (*targets)[index];
    return nullptr;
}

// Route::getDockingTime(int) -> docking time at the given index, or 0.
int Route::getDockingTime(int index) {
    if (this->dockingTargets != nullptr) {
        Array<int> *times = this->dockingTimes;
        if (index < (int)times->size())
            return (*times)[index];
    }
    return 0;
}

// Route::getWaypoint(int) -> snap the waypoint's stored coords to its docking target's position.
Waypoint *Route::getWaypoint(int index) {
    return getWaypointAt(index);
}

// Route::getDockingTimeAt(int) -> docking time at the given index, or 0.
int Route::getDockingTimeAt(int index) {
    if (this->dockingTargets != nullptr) {
        Array<int> *times = this->dockingTimes;
        if (index < (int)times->size())
            return (*times)[index];
    }
    return 0;
}

// Route::setNewCoords(x, y, z) -> overwrite the first waypoint's target coordinates.
void Route::setNewCoords(float x, float y, float z) {
    Waypoint *wp = (*this->waypoints)[0];
    wp->x = (int)x;
    wp->y = (int)y;
    wp->z = (int)z;
}

// Route::setNewCoords(Vector) -> overwrite the first waypoint's target coordinates.
void Route::setNewCoords(Vector v) {
    setNewCoords(v.x, v.y, v.z);
}

// Route::getDockingTarget_i(int) -> docking target at the given index, or null.
KIPlayer *Route::getDockingTarget_i(int index) {
    Array<KIPlayer *> *targets = this->dockingTargets;
    if (targets != nullptr && index < (int)targets->size())
        return (*targets)[index];
    return nullptr;
}

// Route::update(Vector const&) -> forward to update_xyz(v.x, v.y, v.z).
void Route::update(const Vector &v) {
    update_xyz(v.x, v.y, v.z);
}

// Route::update(x, y, z) -> if close enough to the active waypoint, advance to the next.
float Route::update(float x, float y, float z) {
    return update_xyz(x, y, z);
}

// Route::getLastWaypoint() -> last waypoint in the path.
Waypoint *Route::getLastWaypoint() {
    return getWaypointAt((int)this->waypoints->size() - 1);
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

// Route::reachWaypoint(int) -> advance to the given waypoint, wrapping/resetting when looping.
void Route::reachWaypoint(int index) {
    Array<Waypoint *> *wps = this->waypoints;
    uint32_t len = wps->size();
    if (this->currentIndex < (int)(len - 1)) {
        this->currentIndex = index + 1;
    } else if (this->loop != 0) {
        this->currentIndex = 0;
        for (uint32_t i = 0; i < len; i++) {
            (*this->waypoints)[i]->reset();
            len = this->waypoints->size();
        }
        (*this->waypoints)[0]->setActive(true);
    }
    (*this->waypoints)[index]->setActive(false);
    (*this->waypoints)[index]->activate();
}

// Route::clone() -> deep copy of the path; preserves docking targets/times when any are set.
Route *Route::clone() {
    Array<Waypoint *> *wps = this->waypoints;
    int n = (int)wps->size();
    int *coords = new int[n * 3];
    int *p = coords;
    for (int i = 0; i != n; i++) {
        Waypoint *wp = (*wps)[i];
        p[0] = wp->x;
        p[1] = wp->y;
        p[2] = wp->z;
        p += 3;
    }
    Array<KIPlayer *> *tgt = this->dockingTargets;
    if (tgt != nullptr) {
        bool any = false;
        for (uint32_t k = 0; k < tgt->size(); k++)
            if ((*tgt)[k] != nullptr)
                any = true;
        if (any) {
            Array<int> *times = this->dockingTimes;
            int *timesCopy = new int[times->size()];
            for (uint32_t k = 0; k < times->size(); k++)
                timesCopy[k] = (*times)[k];
            Array<KIPlayer *> *targetsArr = new Array<KIPlayer *>();
            targetsArr->resize(this->dockingTargets->size());
            for (uint32_t k = 0; k < this->dockingTargets->size(); k++)
                (*targetsArr)[k] = (*this->dockingTargets)[k];
            Route *r = new Route(coords, targetsArr, timesCopy, (int)this->waypoints->size() * 3);
            r->loop = this->loop;
            delete[] timesCopy;
            return r;
        }
    }
    Route *r = new Route(coords, (int)wps->size() * 3);
    r->loop = this->loop;
    return r;
}

// KIPlayer's vtable slot writes its current position (3 floats) to *out. KIPlayer is
// only ever held by pointer here, so its position is read through the vtable rather
// than pulling in the full type.
typedef void (*GetPosFn)(void *out);

static inline void getPos(void *kip, void *out) {
    (*(*(GetPosFn **)kip + 0xa))(out);
}

// Route::getWaypointAt(int) -> snap the waypoint's stored coords to its docking target's position.
Waypoint *Route::getWaypointAt(int index) {
    char pos[12];
    Waypoint *wp = nullptr;
    if ((int)this->waypoints->size() > index &&
        (wp = (*this->waypoints)[index]) != nullptr) {
        void *k = (void *)(*this->dockingTargets)[index];
        if (k != nullptr) {
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

// Route::update_xyz(x, y, z) -> if close enough to the active waypoint, advance to the next.
float Route::update_xyz(float x, float y, float z) {
    int idx = this->currentIndex;
    Array<Waypoint *> *wps = this->waypoints;
    if (idx >= (int)wps->size())
        return x;
    if ((*this->dockingTargets)[idx] != nullptr)
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

    wp->setActive(false);
    (*this->waypoints)[this->currentIndex]->reached();
    int cur = this->currentIndex;
    Array<Waypoint *> *w = this->waypoints;
    int next = cur + 1;
    this->currentIndex = next;
    uint32_t len = w->size();
    if (this->loop != 0 && (int)(len - 1) <= cur) {
        this->currentIndex = 0;
        for (uint32_t i = 0; i < len; i++) {
            (*w)[i]->reset();
            w = this->waypoints;
            len = w->size();
        }
        next = this->currentIndex;
    }
    if (next < (int)len)
        return (*w)[next]->advance(true);
    return x;
}

// Route::getDockingTime() -> docking time at the current index.
int Route::getDockingTime() {
    return getDockingTimeAt(this->currentIndex);
}

// Route(int*, int) — build a route from a flat [x,y,z, x,y,z, ...] coord array.
// count = 3 * numWaypoints. Allocates empty docking-target/time arrays.
Route::Route(int *coords, int count) {
    this->loop = 0;
    this->currentIndex = 0;
    this->waypoints = new Array<Waypoint *>();
    this->dockingTargets = new Array<KIPlayer *>();
    this->dockingTimes = new Array<int>();
    uint32_t n = count / 3;
    this->dockingTargets->resize(n);
    this->dockingTimes->resize(n);
    for (int i = 0; i < count; i += 3)
        this->waypoints->push_back(new Waypoint(coords[i], coords[i + 1], coords[i + 2], this));
}

// Route(int*, Array<KIPlayer*>*, int*, int) — coords + pre-built docking targets + times.
// The docking-target array is adopted as-is; docking times are copied in.
Route::Route(int *coords, Array<KIPlayer *> *targets, int *times, int count) {
    this->loop = 0;
    this->currentIndex = 0;
    this->waypoints = new Array<Waypoint *>();
    this->dockingTargets = targets;
    this->dockingTimes = new Array<int>();
    for (int i = 0; i < count; i += 3)
        this->dockingTimes->push_back(times[i / 3]);
    for (int i = 0; i < count; i += 3)
        this->waypoints->push_back(new Waypoint(coords[i], coords[i + 1], coords[i + 2], this));
}

// ~Route() — release the waypoint array (deep) and the docking target/time arrays.
Route::~Route() {
    if (this->waypoints != nullptr) {
        for (Waypoint *wp : *this->waypoints)
            delete wp;
        this->waypoints->clear();
        delete this->waypoints;
    }
    this->waypoints = nullptr;
    delete this->dockingTargets;
    this->dockingTargets = nullptr;
    delete this->dockingTimes;
    this->dockingTimes = nullptr;
}

// getCurrent() — index of the active waypoint.
int Route::getCurrent() {
    return this->currentIndex;
}

// setLoop(bool) — toggle whether the route restarts after the final waypoint.
void Route::setLoop(bool loop) {
    this->loop = loop;
}
