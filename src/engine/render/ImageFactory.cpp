#include "engine/render/ImageFactory.h"
#include "engine/core/AERandom.h"
#include "game/core/Globals.h"
#include "engine/core/AbyssEngine.h"
#include "engine/render/ImagePart.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Sprite.h"

int IMAGE_OFFSETS_IPAD_HD[13][4][2] = {
    16, 84, 16, 48, 32, 141, 16, 0,
    16, 96, 16, 45, 32, 141, 16, 0,
    16, 82, 16, 44, 32, 141, 16, 0,
    32, -36, 32, -36, 32, 0, 16, -141,
    32, 141, 16, 31, 32, 141, 16, 0,
    16, -36, 32, -36, 32, 0, 16, -141,
    32, 141, 16, 80, 16, 0, 16, 0,
    16, 72, 16, 39, 32, 141, 16, 0,
    16, -36, 32, -36, 32, 0, 16, -141,
    32, 141, 32, 0, 32, 0, 32, 0,
    16, 85, 16, 48, 32, 141, 16, 0,
    32, 141, 32, 0, 32, 0, 32, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

int IMAGE_OFFSETS_IPAD_LARGE[13][4][2] = {
    16, 120, 16, 68, 32, 200, 16, 0,
    16, 137, 16, 65, 32, 200, 16, 0,
    16, 118, 16, 64, 32, 200, 16, 0,
    32, -52, 32, -52, 32, 0, 16, -200,
    32, 200, 16, 44, 32, 200, 16, 0,
    16, -52, 32, -52, 32, 0, 16, -200,
    32, 200, 16, 115, 16, 0, 16, 0,
    16, 104, 16, 56, 32, 200, 16, 0,
    16, -52, 32, -52, 32, 0, 16, -200,
    32, 200, 32, 0, 32, 0, 32, 0,
    16, 121, 16, 68, 32, 200, 16, 0,
    16, 0, 16, 0, 16, 0, 16, 0,
    32, 200, 32, 0, 32, 0, 32, 0,
};

int IMAGE_OFFSETS_IPAD[13][4][2] = {
    16, 75, 16, 43, 32, 125, 16, 0,
    16, 86, 16, 41, 32, 125, 16, 0,
    16, 74, 16, 40, 32, 125, 16, 0,
    32, -33, 32, -33, 32, 0, 16, -125,
    32, 125, 16, 28, 32, 125, 16, 0,
    16, -33, 32, -33, 32, 0, 16, -125,
    32, 125, 16, 72, 16, 0, 16, 0,
    16, 65, 16, 35, 32, 125, 16, 0,
    16, -33, 32, -33, 32, 0, 16, -125,
    32, 125, 32, 0, 32, 0, 32, 0,
    16, 76, 16, 43, 32, 125, 16, 0,
    16, 0, 16, 0, 16, 0, 16, 0,
    32, 125, 32, 0, 32, 0, 32, 0,
};

int IMAGE_OFFSETS[13][4][2] = {
    {{16, 45}, {16, 26}, {32, 75}, {16, 0}},
    {{16, 52}, {16, 25}, {32, 75}, {16, 0}},
    {{16, 45}, {16, 24}, {32, 75}, {16, 0}},
    {{32, -20}, {32, -20}, {32, 0}, {16, -75}},
    {{32, 75}, {16, 17}, {32, 75}, {16, 0}},
    {{16, -20}, {32, -20}, {32, 0}, {16, -75}},
    {{32, 75}, {16, 42}, {16, 0}, {16, 0}},
    {{16, 39}, {16, 21}, {32, 75}, {16, 0}},
    {{16, -20}, {32, -20}, {32, 0}, {16, -75}},
    {{32, 75}, {32, 0}, {32, 0}, {32, 0}},
    {{16, 46}, {16, 26}, {32, 75}, {16, 0}},
    {{16, 0}, {16, 0}, {16, 0}, {16, 0}},
    {{32, 75}, {32, 0}, {32, 0}, {32, 0}},
};

// Image resource of each portrait part: [row][part], -1 = part not available.
static const int kPartImageIds[13][4] = {
    {5000, 5011, 5022, 5033},
    {5044, 5048, 5053, 5059},
    {5068, 5073, 5078, 5083},
    {-1, -1, -1, -1},
    {5088, 5091, 5094, 5099},
    {-1, -1, -1, -1},
    {5103, 5105, 5108, -1},
    {5113, 5115, 5117, 5120},
    {-1, -1, -1, -1},
    {5122, -1, -1, -1},
    {5126, 5130, 5134, 5139},
    {5146, -1, -1, -1},
    {5148, -1, -1, -1},
};

// Part offsets used on retina / N9 screens.
static const int kRetinaOffsets[13][4][2] = {
    {{16, 75}, {16, 43}, {32, 125}, {16, 0}},
    {{16, 86}, {16, 41}, {32, 125}, {16, 0}},
    {{16, 74}, {16, 40}, {32, 125}, {16, 0}},
    {{32, -33}, {32, -33}, {32, 0}, {16, -125}},
    {{32, 125}, {16, 28}, {32, 125}, {16, 0}},
    {{16, -33}, {32, -33}, {32, 0}, {16, -125}},
    {{32, 125}, {16, 72}, {16, 0}, {16, 0}},
    {{16, 65}, {16, 35}, {32, 125}, {16, 0}},
    {{16, -33}, {32, -33}, {32, 0}, {16, -125}},
    {{32, 125}, {32, 0}, {32, 0}, {32, 0}},
    {{16, 76}, {16, 43}, {32, 125}, {16, 0}},
    {{16, 0}, {16, 0}, {16, 0}, {16, 0}},
    {{32, 125}, {32, 0}, {32, 0}, {32, 0}},
};

// Portrait part offset of the current screen class (component k: 0 = x, 1 = y).
#define PART_OFFSET(row, part, k)                                                           \
    (Globals::iPadHD ? IMAGE_OFFSETS_IPAD_HD[row][part][k]                                  \
                     : Globals::iPadLarge ? IMAGE_OFFSETS_IPAD_LARGE[row][part][k]          \
                                          : Globals::iPad ? IMAGE_OFFSETS_IPAD[row][part][k] \
                                                          : IMAGE_OFFSETS[row][part][k])

static const int gCreateChar2Table[] = {
    11, 11, 11, 11,
     4,  5,  6,  9,
     5,  5,  5,  5,
     2,  2,  2,  2,
     3,  3,  5,  4,
     0,  0,  0,  0,
     2,  3,  5,  0,
     2,  2,  3,  2,
     1,  1,  1,  1,
     1,  1,  1,  1,
     4,  4,  5,  7,
};
ImageFactory::ImageFactory() {
    this->sprite = nullptr;
    if (Globals::retinaDisplay || Globals::n9) {
        for (int i = 0; i < 13; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 2; k++)
                    IMAGE_OFFSETS[i][j][k] = kRetinaOffsets[i][j][k];
    }
    this->reload();
}

void ImageFactory::reload() {
    unsigned *ids = new unsigned[6];
    Globals::Canvas->Image2DCreate(0x4fa, ids[0]);
    Globals::Canvas->Image2DCreate(0x4fb, ids[1]);
    Globals::Canvas->Image2DCreate(0x4f7, ids[2]);
    Globals::Canvas->Image2DCreate(0x4f8, ids[3]);
    Globals::Canvas->Image2DCreate(0x4f9, ids[4]);
    Globals::Canvas->Image2DCreate(0x4fc, ids[5]);

    if (this->sprite != nullptr) {
        delete this->sprite;
    }
    this->sprite = nullptr;

    this->sprite = new Sprite(
        (uint32_t *) ids, 6,
        Globals::Canvas->GetImage2DWidth(ids[0]),
        Globals::Canvas->GetImage2DHeight(ids[0]));

    Globals::Canvas->Image2DCreate(0x485, this->itemImage);
    Globals::Canvas->Image2DCreate(0x511, this->shipImage);
}

ImageFactory::~ImageFactory() {
    delete this->sprite;
    this->sprite = nullptr;
}

int *ImageFactory::createChar(int race) {
    int sexRoll = Globals::rnd->nextInt(2);
    return this->createChar(sexRoll == 0, race);
}

int *ImageFactory::createChar(bool isMale, int race) {
    if (race == 3) {
        int reroll = Globals::rnd->nextInt(4);
        race = (reroll != 0) ? 2 : 0;
    }
    int row = race;
    const int *table = gCreateChar2Table;
    if (race == 0) row = 10;
    if (isMale) row = race;
    if (row == 5) row = 0;
    int *desc = new int[5];
    desc[0] = row;

    const int *partCounts = &table[row * 4];
    for (int i = 0; i != 4; ++i)
        desc[i + 1] = Globals::rnd->nextInt(partCounts[i]);
    return desc;
}

Array<ImagePart *> *ImageFactory::loadChar(int *desc) {
    if (desc == nullptr) return nullptr;
    Array<ImagePart *> *parts = new Array<ImagePart *>();
    ArraySetLength<ImagePart *>(4, *parts);
    int race = *desc++;
    for (int col = 0; col != 4; ++col) {
        int frameBase = desc[col];
        if (frameBase != -1) {
            (*parts)[col] = (ImagePart *) this->loadImage(race, col, frameBase);
        }
    }
    ImagePart *tmp = (*parts)[0];
    (*parts)[0] = (*parts)[2];
    (*parts)[2] = tmp;
    return parts;
}

void *ImageFactory::loadImage(int row, int part, int frame) { // lint: void_ptr exported method return type baked into ImageFactory::loadImage signature
    if (kPartImageIds[row][part] < 0)
        return nullptr;
    unsigned int image = 0;
    Globals::Canvas->Image2DCreate((unsigned short) (kPartImageIds[row][part] + frame), image);
    return new ImagePart(image, PART_OFFSET(row, part, 0), PART_OFFSET(row, part, 1));
}

void ImageFactory::drawChar(Array<ImagePart *> *parts, int x, int y, bool flag) {
    Globals::Canvas->SetColor(0xffffffffu);
    Globals::Canvas->DrawImage2D(this->itemImage, x, y);
    for (unsigned i = 0; i < parts->size(); ++i) {
        ImagePart *part = (*parts)[i];
        if (part != nullptr) part->draw(x, y, flag);
    }
    Globals::Canvas->DrawImage2D(this->shipImage, x, y);
}

void ImageFactory::drawItem(int itemId, int frame, int x, int y) {
    Globals::Canvas->SetColor(0xffffffffu);
    this->sprite->setFrame(frame);
    this->sprite->setPosition(x, y);
    this->sprite->draw(1.0f, 1.0f);
    unsigned int image = 0xffffffffu;
    Globals::Canvas->Image2DCreate((unsigned short) getItemImageId(itemId), image);
    Globals::Canvas->DrawImage2D(image, x, y);
}

int ImageFactory::getItemImageId(int itemId) {
    int base = 0xef0;
    if (itemId < 0xb0) base = 0x898;
    return base + itemId;
}

void ImageFactory::drawItem(int itemId, int x, int y) {
    Globals::Canvas->SetColor(0xffffffffu);
    unsigned int image = 0xffffffffu;
    Globals::Canvas->Image2DCreate((unsigned short) getItemImageId(itemId), image);
    Globals::Canvas->DrawImage2D(image, x, y);
}

void ImageFactory::drawShip(int shipId, int x, int y) {
    Globals::Canvas->SetColor(0xffffffffu);
    this->sprite->setFrame(5);
    this->sprite->setPosition(x, y);
    this->sprite->draw(1.0f, 1.0f);
    unsigned int image = 0xffffffffu;
    Globals::Canvas->Image2DCreate((unsigned short) (shipId + 0x971), image);
    Globals::Canvas->DrawImage2D(image, x, y);
}
