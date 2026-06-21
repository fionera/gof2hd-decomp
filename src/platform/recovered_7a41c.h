#ifndef GOF2_RECOVERED_7A41C_H
#define GOF2_RECOVERED_7A41C_H

// Virtual on-screen keyboard / mouse-emulation glue recovered from the Android
// binary (ghidra_addr 0x7a41c..0x7b2d4). The desktop/Android builds drive the
// game with a software pointer and a table of on-screen virtual keys; these free
// helpers query that table, push synthetic key-up events and roll the mouse
// wheel / pointer. All symbols are ordinary global C++ free functions, so they
// mangle straight from their declarations (the lone exception is F(), a plain
// C-linkage 32-bit hash finaliser).

namespace AbyssEngine { class Engine; }

// 32-bit hash finaliser: folds the four bytes of `value` through a 1 KiB,
// four-column substitution table (the classic per-byte S-box mix used by the
// asset checksum). C linkage in the original, hence the bare `F` symbol.
extern "C" unsigned int F(unsigned int value);

// Reports the current pressed/held state of the named logical action (e.g.
// "Accelerate" / "Decelerate"); a plain global free function in the binary.
int GetKeyState(char *name);

// Software-pointer relative move: accumulates a delta into the emulated cursor.
// While the first virtual key is "captured" the x delta is written absolutely,
// otherwise both axes are integrated; the whole move is ignored unless pointer
// emulation is enabled.
void MouseInput(int dx, int dy);

// Mouse-wheel notch: very large up/down notches flip the in-game zoom request
// when the game module is active, and the fractional notch is folded into the
// pending wheel accumulator.
void MouseWheel(float delta, float residual);

// Binds a game key code (slot 0 = primary, slots 1..3 = alternates) onto every
// virtual key whose label matches `name`.
void SetKeyCode(const char *name, int slot, int code);

// True while the touch-window the player's directional key is driving counts as
// an active sub-menu/dialog window (so the input layer should suppress flight
// steering). `player` selects the directional virtual key.
int IsSubMenuActive(int player);

// True while any of the four on-screen arrow keys is held.
bool ArrowKeyPressed();

// True while any arrow key or the action key is held.
bool keyIsPressed();

// Decays each of the three wheel accumulators by a fixed factor every frame,
// clamping toward zero.
void LowerMouseWheel();

// Presses the virtual key `key`: maps it to a game key code, drives the matching
// on-screen button (starting a synthetic touch via OnTouchBegin) and applies the
// key's side effects to the global game state.
void keyPressed(AbyssEngine::Engine *engine, int key);

// Presses every virtual key whose name matches `name` (the press counterpart of
// keyEventReleased).
void keyEventPressed(AbyssEngine::Engine *engine, char *name);

// Releases the virtual key `key` (clears its pressed flag, ends the touch it was
// driving and runs its follow-up action).
void keyReleased(AbyssEngine::Engine *engine, int key);

// Queues `key` to be released on a later frame (see SendStoredKeyUpEvents); the
// queue is bounded to 0x401 entries.
void keyReleasedWithDelay(AbyssEngine::Engine *engine, int key);

// Drains the delayed key-up queue filled by keyReleasedWithDelay.
void SendStoredKeyUpEvents(AbyssEngine::Engine *engine);

// Releases every virtual key whose name matches `name`.
void keyEventReleased(AbyssEngine::Engine *engine, char *name);

// Per-frame keyboard-animation tick. The shipped Android build has no on-screen
// keyboard animation, so this is a no-op kept for the input dispatch table.
void KeyboardAnimationTimer(AbyssEngine::Engine *engine);

// Shows/hides the emulated pointer based on input activity and game state.
int ActualizeMouseVisibilty(int force);

// Re-lays-out the on-screen buttons after a resize. A no-op in the Android build
// (the layout is fixed), kept for the input dispatch table.
void actualizeButtonPositions(AbyssEngine::Engine *engine);

// Folds the current virtual-pointer / wheel state into synthetic touch events for
// the active game module.
void simulateTouch(AbyssEngine::Engine *engine);

#endif // GOF2_RECOVERED_7A41C_H
