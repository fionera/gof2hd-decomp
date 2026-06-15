#ifndef GOF2_IMAGEFACTORY_H
#define GOF2_IMAGEFACTORY_H
#include "gof2/common.h"
#include "gof2/engine/render/Sprite.h"

class ImagePart;

/// Builds and draws the composite glyph / item / ship images used by the UI.
/// Owns a Sprite for the animated composite layers and caches two glyph
/// image2d handles fetched from the global PaintCanvas.
class ImageFactory {
public:
    Sprite      *sprite;       ///< owned composite sprite (frame array)
    unsigned int itemImage;    ///< item glyph image2d handle (id 0x485)
    unsigned int shipImage;    ///< foreground glyph image2d handle (id 0x511)

    ImageFactory();
    ~ImageFactory();

    /// Randomizes a character then forwards to createChar(bool, int).
    void createChar_i(int param_1);

    /// Builds a 5-int char descriptor: a type slot followed by four random
    /// part indices. `type` selects a 4-entry row in the part-count table;
    /// type 3 rerolls to 0 or 2; type 0 maps to row 10 unless rand0 forced it.
    int *createChar(bool clz, bool rand0, int type);

    /// Public 2-argument overload; forwards to the 3-arg builder with rand0=false.
    int *createChar(bool clz, int type) { return createChar(clz, false, type); }

    void drawChar(Array<ImagePart *> *parts, int x, int y, int flag);
    void drawItem(int itemId, int x, int y);
    void drawItem4(int itemId, int frame, int x, int y);
    void drawShip(int shipId, int x, int y);
    Array<ImagePart *> *loadChar(int *param_1);
    void *loadImage(int row, int col, int frameBase);
    void reload();

    int getItemImageId(int param_1);
};
#endif
