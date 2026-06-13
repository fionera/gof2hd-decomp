#include "gof2/game/menu/HackingGame.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/engine/render/PaintCanvas.h"

extern "C" unsigned __aeabi_uidiv(unsigned value, unsigned divisor);
extern "C" int __aeabi_idiv(int value, int divisor);
namespace AbyssEngine { namespace AERandom { int nextInt(void *random, int limit); } }

// ---- ~HackingGame ----
// The game state is a POD block of ints/bytes; nothing to free.
HackingGame::~HackingGame() {}

int HackingGame::getDockingIndex()
{
    return this->dockingIndex;
}

__attribute__((visibility("hidden"))) extern void **g_HackingGame_sound_left;

void HackingGame::rotateLeftCW(bool sound)
{
    if (this->wonTimer != 0)
        return;
    if (sound)
        ((FModSound *)(*g_HackingGame_sound_left))->play(0x8e2, 0, 0, 0.0f);
    return rotateLeftCW(this->working);
}

int HackingGame::getRewardAmount()
{
    return this->rewardAmount;
}

void HackingGame::rotateLeftCW(int *state)
{
    int a = state[0];
    int b = state[1];
    int c = state[3];
    int d = state[4];
    state[0] = c;
    state[1] = a;
    state[3] = d;
    state[4] = b;
    this->rotateTimer = 0;
    *(uint8_t *)&this->f_128 = 1;
}

int HackingGame::gameWon()
{
    if (this->wonTimer < 0x5dd)
        return 0;

    unsigned i = 0;
    do {
        if (i > 5)
            return 1;
        unsigned idx = i;
        ++i;
        if (this->current[idx] != this->target[idx])
            return 0;
    } while (true);
}

__attribute__((visibility("hidden"))) extern void **g_HackingGame_sound_right;

void HackingGame::rotateRightCW(bool sound)
{
    if (this->wonTimer != 0)
        return;
    if (sound)
        ((FModSound *)(*g_HackingGame_sound_right))->play(0x8e2, 0, 0, 0.0f);
    return rotateRightCW(this->working);
}

int HackingGame::gameWon(int *state)
{
    unsigned i = 0;
    do {
        if (i > 5)
            return 1;
        unsigned idx = i;
        ++i;
        if (state[idx] != this->target[idx])
            return 0;
    } while (true);
}

bool HackingGame::isRotating()
{
    if ((uint8_t)(this->f_128 & 0xff) != 0)
        return true;
    return (uint8_t)(this->f_128 >> 8) != 0;
}

int HackingGame::getRewardItem()
{
    return this->rewardItem;
}

void HackingGame::rotateRightCW(int *state)
{
    int a = state[1];
    int b = state[2];
    int c = state[4];
    int d = state[5];
    state[1] = c;
    state[2] = a;
    state[4] = d;
    state[5] = b;
    this->rotateTimer = 0;
    *((uint8_t *)&this->f_128 + 1) = 1;
}

typedef void (*ImageCreateFn)(void *canvas, uint16_t image, uint32_t *out);

__attribute__((visibility("hidden"))) extern void **g_HackingGame_ctor_canvas;
__attribute__((visibility("hidden"))) extern ImageCreateFn g_HackingGame_ctor_imageCreate;

HackingGame::HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex)
{
    this->type = type;
    int scaledType = type + type * 2;
    int savedRewardItem = rewardItem;
    int savedCanvas = canvas;
    int i = 0;
    uint32_t *slot = (uint32_t *)&this->tileImages[scaledType * 4 + 6];
    void **canvasHolder = g_HackingGame_ctor_canvas;
    while (true) {
        void *canvasObject = *canvasHolder;
        if (i == 6)
            break;
        ((PaintCanvas*)(canvasObject))->Image2DCreate((unsigned short)((uint16_t)(i + 0x1f4a)), (unsigned int*)(slot - 6));
        ((PaintCanvas*)(*canvasHolder))->Image2DCreate((unsigned short)((uint16_t)(i + 0x1f50)), (unsigned int*)(slot));
        ++slot;
        ++i;
    }

    ImageCreateFn create = g_HackingGame_ctor_imageCreate;
    create(*canvasHolder, 0x1f48, (uint32_t *)&this->bottomImage);
    create(*canvasHolder, 0x1f49, (uint32_t *)&this->topImage);
    create(*canvasHolder, 0x1f47, (uint32_t *)&this->mainImage);
    create(*canvasHolder, 0x1f46, (uint32_t *)&this->arrowActive);
    create(*canvasHolder, 0x1f44, (uint32_t *)&this->arrowIdle);
    create(*canvasHolder, 0x1f45, (uint32_t *)&this->markImage);

    this->difficulty = savedCanvas;
    this->rewardItem = savedRewardItem;
    this->rewardAmount = rewardAmount;
    this->dockingIndex = dockingIndex;
    reInit();
}

typedef int (*ImageMeasureFn)(void *canvas, int image);

__attribute__((visibility("hidden"))) extern void **g_HackingGame_render_canvas;
__attribute__((visibility("hidden"))) extern int *g_HackingGame_render_screen_w_a;
__attribute__((visibility("hidden"))) extern int *g_HackingGame_render_screen_w_b;
__attribute__((visibility("hidden"))) extern int *g_HackingGame_render_screen_h;
__attribute__((visibility("hidden"))) extern ImageMeasureFn g_HackingGame_render_height_fn;
__attribute__((visibility("hidden"))) extern void **g_HackingGame_render_layout;

static inline int half_i(int value)
{
    return value / 2;
}

// The render code reads four scroll/layout offsets (0x30c..0x318) from a *separate* layout
// object (g_HackingGame_render_layout), not from `this`. Keep that cross-object read explicit.
static inline int layoutOffset(void *layout, uint32_t off)
{
    return *(int32_t *)((char *)layout + off);
}

void HackingGame::render2D()
{
    char deltaBytes[52];
    int *delta = (int *)deltaBytes;

    void **canvasHolder = g_HackingGame_render_canvas;
    void *canvas = *canvasHolder;
    ((PaintCanvas*)(canvas))->SetColor((unsigned)(-1));

    bool solved = true;
    for (unsigned i = 0; i <= 5; ++i) {
        if (this->current[i] != this->target[i]) {
            solved = false;
            break;
        }
    }

    int typeOffset = this->type * 12;
    int baseImage = this->tileImages[typeOffset];
    int tileW = ((PaintCanvas*)(*canvasHolder))->GetImage2DWidth((unsigned)(baseImage));
    int tileH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(baseImage));

    for (int i = 0; i != 6; ++i) {
        delta[i * 2] = 0;
        delta[i * 2 + 1] = 0;
    }

    if ((uint8_t)(this->f_128 & 0xff) != 0) {
        float amount = (float)this->rotateTimer / 300.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        delta[3] = (int)(amount * (float)tileH);
        delta[0] = (int)(amount * (float)tileW);
        delta[7] = (int)(-(amount * (float)tileH));
        delta[8] = (int)(-(amount * (float)tileW));
    } else if ((uint8_t)(this->f_128 >> 8) != 0) {
        float amount = (float)this->rotateTimer / 300.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        delta[5] = (int)(amount * (float)tileH);
        delta[2] = (int)(amount * (float)tileW);
        delta[9] = (int)(-(amount * (float)tileH));
        delta[10] = (int)(-(amount * (float)tileW));
    }

    if (solved) {
        if (this->wonTimer > 0x5dc)
            goto done;
    } else {
        int image = this->mainImage;
        void *drawCanvas = *canvasHolder;
        int screenW = *g_HackingGame_render_screen_w_a;
        int width = ((PaintCanvas*)(drawCanvas))->GetImage2DWidth((unsigned)(image));
        int height = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(image));
        ((PaintCanvas*)(drawCanvas))->DrawImage2D((unsigned)image, half_i(screenW) - half_i(width), half_i(*g_HackingGame_render_screen_h) - half_i(height));
    }

    {
        int topImage = this->topImage;
        int screenW = *g_HackingGame_render_screen_w_b;
        void *drawCanvas = *canvasHolder;
        int topWidth = ((PaintCanvas*)(drawCanvas))->GetImage2DWidth((unsigned)(topImage));
        ImageMeasureFn measure = g_HackingGame_render_height_fn;
        int screenH = *g_HackingGame_render_screen_h;
        int titleH = measure(*canvasHolder, this->mainImage);
        int topH = measure(*canvasHolder, this->topImage);
        void **layout = g_HackingGame_render_layout;

        ((PaintCanvas*)(drawCanvas))->DrawImage2D((unsigned)topImage, half_i(screenW) - topWidth, half_i(screenH) - half_i(titleH) - topH + layoutOffset(*layout, 0x30c));
        ((PaintCanvas*)(drawCanvas))->DrawImage2D((unsigned)(topImage), half_i(screenW), half_i(screenH) - half_i(titleH) - topH + layoutOffset(*layout, 0x30c), (unsigned char)(true));

        if (!solved) {
            int bottomImage = this->bottomImage;
            int bottomW = ((PaintCanvas*)(*canvasHolder))->GetImage2DWidth((unsigned)(bottomImage));
            int bottomH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(this->mainImage));
            ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)bottomImage, half_i(screenW) - bottomW,
                                    half_i(bottomH) + half_i(screenH) + layoutOffset(*layout, 0x314));
            ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)bottomImage, half_i(screenW),
                                           half_i(bottomH) + half_i(screenH) + layoutOffset(*layout, 0x314), (unsigned char)true);
        }

        float oneAndHalf = (float)tileW * 1.5f;
        for (unsigned i = 0; i != 6; ++i) {
            int stateIndex = this->current[i];
            bool flashSolved = solved && ((__aeabi_idiv(this->wonTimer, 200) & 1) == 0);
            int imageOffset = (flashSolved ? 6 : 0) + typeOffset + stateIndex;
            int image = this->tileImages[imageOffset];
            unsigned row = __aeabi_uidiv((uint8_t)i, 3);
            unsigned col = (uint8_t)(i - row * 3);
            int y = (int)(((float)half_i(screenW) - oneAndHalf) + (float)(tileW * col) +
                          (float)delta[i * 2]);
            int imageH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(this->mainImage));
            ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)image, y,
                                    row * tileH + half_i(*g_HackingGame_render_screen_h) - half_i(imageH) +
                                        delta[i * 2 + 1] + layoutOffset(*layout, 0x310));
        }

        int leftArrow = (uint8_t)(this->f_128 & 0xff) != 0 ? this->arrowActive : this->arrowIdle;
        int arrowW = ((PaintCanvas*)(*canvasHolder))->GetImage2DWidth((unsigned)(leftArrow));
        int arrowTitleH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(this->mainImage));
        int arrowH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(leftArrow));
        ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)leftArrow,
                                half_i(screenW) - half_i(tileW) - half_i(arrowW),
                                half_i(*g_HackingGame_render_screen_h) - half_i(tileH) -
                                    half_i(arrowTitleH) - half_i(arrowH) + layoutOffset(*layout, 0x310));

        int rightArrow = (uint8_t)(this->f_128 >> 8) != 0 ? this->arrowActive : this->arrowIdle;
        arrowW = ((PaintCanvas*)(*canvasHolder))->GetImage2DWidth((unsigned)(rightArrow));
        arrowTitleH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(this->mainImage));
        arrowH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(rightArrow));
        ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)rightArrow,
                                half_i(screenW) + half_i(tileW) - half_i(arrowW),
                                half_i(*g_HackingGame_render_screen_h) - half_i(tileH) -
                                    half_i(arrowTitleH) - half_i(arrowH) + layoutOffset(*layout, 0x310));

        if (!solved) {
            for (unsigned i = 0; i != 6; ++i) {
                unsigned row = __aeabi_uidiv((uint8_t)i, 3);
                unsigned col = (uint8_t)(i - row * 3);
                int stateIndex = this->target[i];
                int image = this->tileImages[typeOffset + stateIndex];
                int imageH = ((PaintCanvas*)(*canvasHolder))->GetImage2DHeight((unsigned)(this->mainImage));
                int y = (int)(((float)half_i(screenW) - oneAndHalf) + (float)(tileW * col));
                ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)image, y,
                                        row * tileH + half_i(*g_HackingGame_render_screen_h) + half_i(imageH) +
                                            layoutOffset(*layout, 0x318));
            }

            int mark = this->markImage;
            int markW = ((PaintCanvas*)(*canvasHolder))->GetImage2DWidth((unsigned)(mark));
            int titleH2 = measure(*canvasHolder, this->mainImage);
            int markH = measure(*canvasHolder, mark);
            ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)mark,
                                    half_i(screenW) - half_i(tileW) - half_i(markW),
                                    half_i(*g_HackingGame_render_screen_h) + half_i(tileH) +
                                        half_i(titleH2) - half_i(markH) + layoutOffset(*layout, 0x318));
            ((PaintCanvas*)(*canvasHolder))->DrawImage2D((unsigned)mark,
                                    half_i(screenW) + half_i(tileW) - half_i(markW),
                                    half_i(*g_HackingGame_render_screen_h) + half_i(tileH) +
                                        half_i(titleH2) - half_i(markH) + layoutOffset(*layout, 0x318));
        }
    }

done:
    return;
}

__attribute__((visibility("hidden"))) extern void **g_HackingGame_reinit_random2;
__attribute__((visibility("hidden"))) extern void **g_HackingGame_reinit_random3;
__attribute__((visibility("hidden"))) extern void **g_HackingGame_reinit_random_shuffle;

void HackingGame::reInit()
{
    char localBytes[24];
    int *local = (int *)localBytes;

    int type = this->difficulty;
    this->rotateTimer = 0;
    this->wonTimer = 0;

    if (type == 1) {
        for (unsigned i = 0; i != 6; ++i)
            this->target[i] = i >> 1;
    } else if (type == 2) {
        for (int i = 0; i != 4; ++i)
            this->target[i] = i;
        void **random = g_HackingGame_reinit_random2;
        this->target[4] = AbyssEngine::AERandom::nextInt(*random, 4);
        this->target[5] = AbyssEngine::AERandom::nextInt(*random, 4);
    } else if (type == 3) {
        for (int i = 0; i != 5; ++i)
            this->target[i] = i;
        this->target[5] = AbyssEngine::AERandom::nextInt(*g_HackingGame_reinit_random3, 5);
    } else {
        for (int i = 0; i != 6; ++i)
            this->target[i] = i;
    }

    void **random = g_HackingGame_reinit_random_shuffle;
    for (int i = 0x28; i != 0; --i) {
        int a = AbyssEngine::AERandom::nextInt(*random, 6);
        int b = AbyssEngine::AERandom::nextInt(*random, 6);
        int value = this->target[a];
        this->target[a] = this->target[b];
        this->target[b] = value;
    }

    for (int i = 0; i != 6; ++i) {
        int value = this->target[i];
        this->current[i] = value;
        this->working[i] = value;
    }

    for (unsigned i = 0; (int)i < this->difficulty * 2; ++i) {
        int count = AbyssEngine::AERandom::nextInt(*random, 2);
        for (int j = 0; j <= count; ++j) {
            if ((i & 1) == 0)
                rotateRightCW(false);
            else
                rotateLeftCW(false);
        }

        type = this->difficulty;
        if (i == (unsigned)(type * 2 - 1)) {
            for (int j = 0; j != 6; ++j)
                local[j] = this->working[j];
            if (solvableInNSteps(type, 0, 0, 0, local) != 0)
                i = 0;
        }
    }

    for (int i = 0; i != 6; ++i)
        this->current[i] = this->working[i];

    this->f_128 = 0;
}

int HackingGame::solvableInNSteps(int steps, int depth, int leftCount, int rightCount, int *state)
{
    char leftBytes[24];
    char rightBytes[24];
    int *leftState = (int *)leftBytes;
    int *rightState = (int *)rightBytes;

    for (int i = 0; i != 6; ++i) {
        int value = state[i];
        rightState[i] = value;
        leftState[i] = value;
    }

    unsigned i = 0;
    do {
        if (i > 5)
            return 1;
        unsigned idx = i;
        ++i;
        if (state[idx] != this->target[idx])
            break;
    } while (true);

    if (depth < steps) {
        unsigned move = 0;
        do {
            if (move > 1)
                return 0;

            if (leftCount < 3 && move == 0) {
                int a = leftState[0];
                int b = leftState[1];
                int c = leftState[3];
                int d = leftState[4];
                leftState[0] = c;
                leftState[1] = a;
                leftState[3] = d;
                leftState[4] = b;
                this->rotateTimer = 0;
                *(uint8_t *)&this->f_128 = 1;
                return solvableInNSteps(steps, depth + 1, leftCount + 1, 0, leftState);
            }

            ++move;
            if (rightCount <= 2) {
                int a = rightState[1];
                int b = rightState[2];
                int c = rightState[4];
                int d = rightState[5];
                rightState[1] = c;
                rightState[2] = a;
                rightState[4] = d;
                rightState[5] = b;
                this->rotateTimer = 0;
                *((uint8_t *)&this->f_128 + 1) = 1;
                return solvableInNSteps(steps, depth + 1, 0, rightCount + 1, rightState);
            }
        } while (true);
    }

    return 0;
}

__attribute__((visibility("hidden"))) extern FModSound **g_HackingGame_update_sound;

int HackingGame::update(int dt)
{
    if ((uint8_t)(this->f_128 & 0xff) != 0 || (uint8_t)(this->f_128 >> 8) != 0) {
        int timer = this->rotateTimer + dt;
        this->rotateTimer = timer;
        if (timer > 300) {
            this->f_128 = 0;
            this->rotateTimer = 0;
            for (int i = 0; i != 6; ++i)
                this->current[i] = this->working[i];
        }
    }

    int wonTimer;
    for (unsigned i = 0; i < 6; ++i) {
        if (this->current[i] != this->target[i])
            goto running;
    }

    wonTimer = this->wonTimer;
    if (wonTimer == 0) {
        (*g_HackingGame_update_sound)->play(0x8e1, 0, 0, 0.0f);
        wonTimer = this->wonTimer;
    }
    wonTimer += dt;
    if (wonTimer > 0x5dc) {
        this->wonTimer = wonTimer;
        return 0;
    }
    this->wonTimer = wonTimer;
running:
    return 1;
}
