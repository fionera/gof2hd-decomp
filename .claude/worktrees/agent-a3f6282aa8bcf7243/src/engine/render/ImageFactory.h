#ifndef GOF2_IMAGEFACTORY_H
#define GOF2_IMAGEFACTORY_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/Sprite.h"

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

    /// Picks a random sex (50/50), then builds a random character of `race`.
    void createChar(int race);

    /// Builds a 5-int char descriptor: a race slot followed by four random
    /// part indices. `race` selects a 4-entry row in the part-count table;
    /// race 3 (Midorian) rerolls to 0 or 2; race 0 (Terran) maps to row 10
    /// (Woman) unless `isMale` is set; race 5 (Cyborg) collapses to row 0.
    int *createChar(bool isMale, int race);

    void drawChar(Array<ImagePart *> *parts, int x, int y, bool flag);
    void drawItem(int itemId, int x, int y);
    void drawItem(int itemId, int frame, int x, int y);
    void drawShip(int shipId, int x, int y);
    Array<ImagePart *> *loadChar(int *desc);
    void *loadImage(int row, int col, int frameBase);
    void reload();

    int getItemImageId(int itemId);
};
#endif
