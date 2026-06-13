#ifndef GOF2_LISTITEMWINDOW_H
#define GOF2_LISTITEMWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- ListItemWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "ListItemWindow::...". Only argument types live in AbyssEngine. The recovered
// instance layout mixes the engine's 12-byte String and fixed-size Matrix at hard
// offsets (and several runtime-computed offsets such as the up/down/equal arrow
// image ids), so the body reaches fields through the typed byte-offset helpers
// below rather than a synthetic std-typed field list.

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Engine String is a 12-byte trivially-copyable value (ctor/dtor are engine calls).
struct Str { uint32_t a, b, c; };

// ListItemWindow instance: the four leading container members (the label/value
// String* arrays and the ship-stat int arrays) are modelled as real Array<T>*
// fields so the recovered code can drive them through std::vector operations; the
// remaining instance state is still reached via the typed byte-offset helpers.
class ListItemWindow {
public:
    // +0x0  label-row String* array (owns its elements).
    Array<AbyssEngine::String *> *labels;
    // +0x4  value-row String* array (owns its elements).
    Array<AbyssEngine::String *> *values;
    // +0x8  current ship-stat int array (ship items only).
    Array<int> *statsCur;
    // +0xc  previous ship-stat int array (ship items only).
    Array<int> *statsPrev;

    ListItemWindow();
    ~ListItemWindow();
    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    uint8_t shows3DShip();
    void render();
    void set(void *item, unsigned p2, unsigned p3, unsigned p4, unsigned p5, bool p6);
    void draw();
    void update(int frameTime);

    // C-ABI destructor helper: runs ~ListItemWindow and returns `this` so the
    // caller can free the storage (matches the ARM deleting-dtor thunk).
    void *dtor();
    // Touch-release entry point used by the owning screen; equivalent to
    // OnTouchEnd, returning an int status the original caller ignores.
    int touch_end(int x, int y);
};

// Field accessors via byte offset.

#endif
