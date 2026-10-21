#include "engine/render/ImageFactory.h"
#include "engine/core/AERandom.h"
#include "game/core/Globals.h"
#include "engine/core/AbyssEngine.h"
#include "engine/render/ImagePart.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Sprite.h"

static PaintCanvas **g_reload_canvas = &Globals::Canvas;
static PaintCanvas **g_drawChar_canvas = &Globals::Canvas;
static PaintCanvas **g_IF_drawShip_canvas = &Globals::Canvas;
static PaintCanvas **g_drawItem_canvas = &Globals::Canvas;
static PaintCanvas **g_IF_drawItem4_canvas = &Globals::Canvas;

int IMAGE_OFFSETS[13][4][2] = {
    16, 45, 16, 26, 32, 75, 16, 0,
    16, 52, 16, 25, 32, 75, 16, 0,
    16, 45, 16, 24, 32, 75, 16, 0,
    32, -20, 32, -20, 32, 0, 16, -75,
    32, 75, 16, 17, 32, 75, 16, 0,
    16, -20, 32, -20, 32, 0, 16, -75,
    32, 75, 16, 42, 16, 0, 16, 0,
    16, 39, 16, 21, 32, 75, 16, 0,
    16, -20, 32, -20, 32, 0, 16, -75,
    32, 75, 32, 0, 32, 0, 32, 0,
    16, 46, 16, 26, 32, 75, 16, 0,
    16, 0, 16, 0, 16, 0, 16, 0,
    32, 75, 32, 0, 32, 0, 32, 0,
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


static AbyssEngine::AERandom **gCreateChar2Rng1 = &Globals::rnd;
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
static AbyssEngine::AERandom **gCreateChar2Rng2 = &Globals::rnd;
static AbyssEngine::AERandom **gCreateCharRng = &Globals::rnd;


int ImageFactory::getItemImageId(int itemId) {
    int base = 0xef0;
    if (itemId < 0xb0) base = 0x898;
    return base + itemId;
}

ImageFactory::~ImageFactory() {
    delete this->sprite;
    this->sprite = nullptr;
}

ImageFactory::ImageFactory() {
    this->sprite = nullptr;
    if (Globals::retinaDisplay || Globals::n9) {
        // Large-screen layout: 13 image slots x 4 parts x (x, y).
        const int largeScreenOffsets[13][4][2] = {
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
        for (int i = 0; i < 13; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 2; ++k)
                    IMAGE_OFFSETS[i][j][k] = largeScreenOffsets[i][j][k];
    }
    this->reload();
}

void ImageFactory::reload() {
    unsigned *ids = new unsigned[6];
    PaintCanvas **canvasHolder = g_reload_canvas;
    (*canvasHolder)->Image2DCreate(0x4fa, ids[0]);
    (*canvasHolder)->Image2DCreate(0x4fb, ids[1]);
    (*canvasHolder)->Image2DCreate(0x4f7, ids[2]);
    (*canvasHolder)->Image2DCreate(0x4f8, ids[3]);
    (*canvasHolder)->Image2DCreate(0x4f9, ids[4]);
    (*canvasHolder)->Image2DCreate(0x4fc, ids[5]);

    if (this->sprite != nullptr) {
        delete this->sprite;
    }
    this->sprite = nullptr;

    this->sprite = new Sprite(
        (uint32_t *) ids, 6,
        (*canvasHolder)->GetImage2DWidth(ids[0]),
        (*canvasHolder)->GetImage2DHeight(ids[0]));

    (*canvasHolder)->Image2DCreate(0x485, this->itemImage);
    (*canvasHolder)->Image2DCreate(0x511, this->shipImage);
}

void ImageFactory::drawChar(Array<ImagePart *> *parts, int x, int y, bool flag) {
    PaintCanvas **sl = g_drawChar_canvas;
    (*sl)->SetColor(0xffffffffu);
    (*sl)->DrawImage2D(this->itemImage, x, y);
    for (unsigned i = 0; i < parts->size(); ++i) {
        ImagePart *part = (*parts)[i];
        if (part != nullptr) part->draw(x, y, flag);
    }
    (*sl)->DrawImage2D(this->shipImage, x, y);
}

void ImageFactory::drawShip(int shipId, int x, int y) {
    PaintCanvas **holder = g_IF_drawShip_canvas;
    unsigned classIcon = 0xffffffffu;
    (*holder)->SetColor(0xffffffffu);
    this->sprite->setFrame(5);
    this->sprite->setPosition(x, y);
    this->sprite->draw(1.0f, 1.0f);
    (*holder)->Image2DCreate((unsigned short) (shipId + 0x971), classIcon);
    (*holder)->DrawImage2D(classIcon, x, y);
}

void ImageFactory::drawItem(int itemId, int x, int y) {
    PaintCanvas **holder = g_drawItem_canvas;
    unsigned icon = 0xffffffffu;
    (*holder)->SetColor(0xffffffffu);
    int base = 0xef0;
    if (itemId < 0xb0) base = 0x898;
    (*holder)->Image2DCreate((unsigned short) (base + itemId), icon);
    (*holder)->DrawImage2D(icon, x, y);
}

void *ImageFactory::loadImage(int row, int col, int frameBase) { // lint: void_ptr exported method return type baked into ImageFactory::loadImage signature
    // First image id of each portrait part (row: face type, col: part); -1 = part not used.
    const int imageIds[13][4] = {
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
    int id = imageIds[row][col];
    if (id < 0)
        return nullptr;

    unsigned image = 0;
    Globals::Canvas->Image2DCreate((unsigned short) (id + frameBase), image);
    return new ImagePart(image,
                         Globals::iPadHD ? IMAGE_OFFSETS_IPAD_HD[row][col][0]
                         : Globals::iPadLarge ? IMAGE_OFFSETS_IPAD_LARGE[row][col][0]
                         : Globals::iPad ? IMAGE_OFFSETS_IPAD[row][col][0]
                         : IMAGE_OFFSETS[row][col][0],
                         Globals::iPadHD ? IMAGE_OFFSETS_IPAD_HD[row][col][1]
                         : Globals::iPadLarge ? IMAGE_OFFSETS_IPAD_LARGE[row][col][1]
                         : Globals::iPad ? IMAGE_OFFSETS_IPAD[row][col][1]
                         : IMAGE_OFFSETS[row][col][1]);
}

void ImageFactory::drawItem(int itemId, int frame, int x, int y) {
    PaintCanvas **holder = g_IF_drawItem4_canvas;
    unsigned icon = 0xffffffffu;
    (*holder)->SetColor(0xffffffffu);
    this->sprite->setFrame(frame);
    this->sprite->setPosition(x, y);
    this->sprite->draw(1.0f, 1.0f);
    int base = 0xef0;
    if (itemId < 0xb0) base = 0x898;
    (*holder)->Image2DCreate((unsigned short) (base + itemId), icon);
    (*holder)->DrawImage2D(icon, x, y);
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

int *ImageFactory::createChar(int race) {
    int sexRoll = (*gCreateCharRng)->nextInt(2);
    return this->createChar(sexRoll == 0, race);
}

int *ImageFactory::createChar(bool isMale, int race) {
    if (race == 3) {
        int reroll = (*gCreateChar2Rng1)->nextInt(4);
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
        desc[i + 1] = (*gCreateChar2Rng2)->nextInt(partCounts[i]);
    return desc;
}
