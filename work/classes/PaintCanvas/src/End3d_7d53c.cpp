#include "class.h"

// AbyssEngine::PaintCanvas::End3d() — ends the current 3D pass (counterpart to Begin3d).
// Ghidra mis-decoded this tiny tail-call; it simply forwards to the internal end-3D helper.
extern "C" void paintcanvas_ext_end3d(AbyssEngine::PaintCanvas *);

void End3d(AbyssEngine::PaintCanvas *self)
{
    return paintcanvas_ext_end3d(self);
}
