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

int IMAGE_OFFSETS[104];
int IMAGE_OFFSETS_IPAD[104] = {
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
int IMAGE_OFFSETS_IPAD_HD[104] = {
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
int IMAGE_OFFSETS_IPAD_LARGE[104] = {
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

static int *g_IF_idTable = IMAGE_OFFSETS;

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

static PaintCanvas **g_IF_li_canvas = &Globals::Canvas;
static char *g_IF_flagA = (char *)&Globals::iPadHD;
static char *g_IF_flagB = (char *)&Globals::iPadLarge;
static int *g_IF_posTableA = IMAGE_OFFSETS_IPAD_HD;
static int *g_IF_posTableB = IMAGE_OFFSETS_IPAD_LARGE;
static int *g_IF_posTableC = IMAGE_OFFSETS;
static char *g_IF_flagC = (char *)&Globals::iPad;
static int *g_IF_posTableD = IMAGE_OFFSETS_IPAD;

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
    if (Globals::iPad != 0) {
        const int *src = IMAGE_OFFSETS_IPAD;
        if (Globals::iPadHD != 0)
            src = IMAGE_OFFSETS_IPAD_HD;
        else if (Globals::iPadLarge != 0)
            src = IMAGE_OFFSETS_IPAD_LARGE;
        for (int i = 0; i < 104; ++i)
            IMAGE_OFFSETS[i] = src[i];
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

    int w = (*canvasHolder)->GetImage2DWidth(ids[0]);
    int h = (*canvasHolder)->GetImage2DHeight(ids[0]);
    this->sprite = new Sprite((uint32_t *)ids, 6, w, h);

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
    int id = g_IF_idTable[row * 4 + col];
    if (id < 0)
        return nullptr;

    unsigned image = 0;
    (*g_IF_li_canvas)->Image2DCreate((unsigned short) ((short)id + (short)frameBase), image);

    int *posBase;

    int cell = row * 8 + col * 2;
    if (*g_IF_flagA != 0) {
        posBase = g_IF_posTableA;
    } else if (*g_IF_flagB != 0) {
        posBase = g_IF_posTableB;
    } else {
        posBase = g_IF_posTableC;
        if (*g_IF_flagC == 0)
            posBase = g_IF_posTableD;
    }

    int px = posBase[cell];
    int py = posBase[cell + 1];
    return new ImagePart(image, px, py);
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

void ImageFactory::createChar(int race) {
    int sexRoll = (*gCreateCharRng)->nextInt(2);
    this->createChar(sexRoll == 0, race);
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

