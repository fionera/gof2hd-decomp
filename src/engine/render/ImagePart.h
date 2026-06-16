#ifndef GOF2_IMAGEPART_H
#define GOF2_IMAGEPART_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// A drawable 2D image region: an image2d handle plus a cached size and a
// vertical offset, rendered through the global PaintCanvas.
class ImagePart {
public:
    int id;       // image2d handle
    int f_4;      // flags (draw ORs with 1)
    int pos_y;    // vertical offset applied at draw time
    int scale_x;  // cached image width
    int scale_y;  // cached image height

    // Demangles to ImagePart::ImagePart(unsigned, int, int); queries the
    // global PaintCanvas for the image's width/height.
    ImagePart(unsigned id, int field04, int posY);

    void draw(int x, int y, bool b);
};
#endif
