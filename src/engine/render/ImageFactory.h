#ifndef GOF2_IMAGEFACTORY_H
#define GOF2_IMAGEFACTORY_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "engine/render/Sprite.h"

class ImagePart;
class Sprite;

class ImageFactory {
public:
    Sprite *sprite;
    unsigned int itemImage;
    unsigned int shipImage;

    ImageFactory();

    ~ImageFactory();

    void createChar(int race);

    int *createChar(bool isMale, int race);

    void drawChar(Array<ImagePart *> *parts, int x, int y, bool flag);

    void drawItem(int itemId, int x, int y);

    void drawItem(int itemId, int frame, int x, int y);

    void drawShip(int shipId, int x, int y);

    Array<ImagePart *> *loadChar(int *desc);

    void *loadImage(int row, int col, int frameBase);

    // lint: void_ptr exported method return type baked into ImageFactory::loadImage signature

    void reload();

    int getItemImageId(int itemId);
};
#endif
