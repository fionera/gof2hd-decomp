# PaintCanvas::DrawLine

Symbol: `_ZN11AbyssEngine11PaintCanvas8DrawLineEiiii`

The original function starts at `0x794d8` and is 436 bytes. The reconstructed
candidate is also 436 bytes and uses the real engine and GL calls rather than
the temporary `paintcanvas_ext_dl_*` wrappers.

The linked path constructs the 15-word matrix and calls
`SetWorldViewMatrix` before filling `lineVerts` and calling
`Engine::DrawLine2D`. The legacy path sets line width and disables texturing,
fills `lineVerts`, constructs the matrix, calls `SetWorldViewMatrix`, configures
the vertex client state, draws the two vertices, and restores texturing. The
matrix words at indices 0, 5, 10, 12, 13, and 14 are `0x3f800000`; all other
words are zero.

The only remaining verifier residual is in the prologue. The original loads
the Android linked-status global before the stack-canary global and tests it
with `cmp` plus `beq`. The current compiler loads the stack-canary global
before the linked-status global and emits `cbz`, with a compensating `nop`.
The bodies after that scheduling difference align.

A named local for the low byte of `g_android_gp_is_linked` was tested and did
not change this residual. Do not pursue an ODR-mismatched `extern bool`, a
`volatile` qualifier, inline assembly, attributes, or dummy side effects to
force the prologue: those are either incorrect, forbidden, or both.
