#include "game/menu/HackingGame.h"
#include "engine/audio/FModSound.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"

HackingGame::~HackingGame() {
}

int HackingGame::getDockingIndex() {
    return this->dockingIndex;
}


extern FModSound **g_HackingGame_sound_left;

void HackingGame::rotateLeftCW(bool sound) {
    if (this->wonTimer != 0)
        return;
    if (sound)
        (*g_HackingGame_sound_left)->play(0x8e2, 0, 0, 0.0f);
    return rotateLeftCW(this->working);
}

int HackingGame::getRewardAmount() {
    return this->rewardAmount;
}

void HackingGame::rotateLeftCW(int *state) {
    int a = state[0];
    int b = state[1];
    int c = state[3];
    int d = state[4];
    state[0] = c;
    state[1] = a;
    state[3] = d;
    state[4] = b;
    this->rotateTimer = 0;
    this->rotatingLeft = true;
}

int HackingGame::gameWon() {
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


extern FModSound **g_HackingGame_sound_right;

void HackingGame::rotateRightCW(bool sound) {
    if (this->wonTimer != 0)
        return;
    if (sound)
        (*g_HackingGame_sound_right)->play(0x8e2, 0, 0, 0.0f);
    return rotateRightCW(this->working);
}

int HackingGame::gameWon(int *state) {
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

bool HackingGame::isRotating() {
    return this->rotatingLeft || this->rotatingRight;
}

int HackingGame::getRewardItem() {
    return this->rewardItem;
}

void HackingGame::rotateRightCW(int *state) {
    int a = state[1];
    int b = state[2];
    int c = state[4];
    int d = state[5];
    state[1] = c;
    state[2] = a;
    state[4] = d;
    state[5] = b;
    this->rotateTimer = 0;
    this->rotatingRight = true;
}

typedef void (*ImageCreateFn)(void *canvas, uint16_t image, uint32_t *out);


extern ImageCreateFn g_HackingGame_ctor_imageCreate;

HackingGame::HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex) {
    this->type = type;
    int scaledType = type + type * 2;
    int savedRewardItem = rewardItem;
    int savedCanvas = canvas;
    int base = scaledType * 4;
    for (int i = 0; i != 6; ++i) {
        unsigned int handle;
        gCanvas->Image2DCreate((uint16_t)(i + 0x1f4a), handle);
        this->tileImages[base + i] = handle;
        gCanvas->Image2DCreate((uint16_t)(i + 0x1f50), handle);
        this->tileImages[base + 6 + i] = handle;
    }

    ImageCreateFn create = g_HackingGame_ctor_imageCreate;
    create(gCanvas, 0x1f48, (uint32_t *) &this->bottomImage);
    create(gCanvas, 0x1f49, (uint32_t *) &this->topImage);
    create(gCanvas, 0x1f47, (uint32_t *) &this->mainImage);
    create(gCanvas, 0x1f46, (uint32_t *) &this->arrowActive);
    create(gCanvas, 0x1f44, (uint32_t *) &this->arrowIdle);
    create(gCanvas, 0x1f45, (uint32_t *) &this->markImage);

    this->difficulty = savedCanvas;
    this->rewardItem = savedRewardItem;
    this->rewardAmount = rewardAmount;
    this->dockingIndex = dockingIndex;
    reInit();
}

typedef int (*ImageMeasureFn)(void *canvas, int image);


extern int *g_HackingGame_render_screen_w_a;

extern int *g_HackingGame_render_screen_w_b;

extern int *g_HackingGame_render_screen_h;

extern ImageMeasureFn g_HackingGame_render_height_fn;

extern void **g_HackingGame_render_layout;

static inline int half_i(int value) {
    return value / 2;
}

static inline int layoutOffset(void *layout, uint32_t off) {
    return *(int32_t *) ((char *) layout + off);
}

void HackingGame::render2D() {
    int delta[13];

    gCanvas->SetColor((unsigned) (-1));

    bool solved = true;
    for (unsigned i = 0; i <= 5; ++i) {
        if (this->current[i] != this->target[i]) {
            solved = false;
            break;
        }
    }

    int typeOffset = this->type * 12;
    int baseImage = this->tileImages[typeOffset];
    int tileW = gCanvas->GetImage2DWidth((unsigned) (baseImage));
    int tileH = gCanvas->GetImage2DHeight((unsigned) (baseImage));

    for (int i = 0; i != 6; ++i) {
        delta[i * 2] = 0;
        delta[i * 2 + 1] = 0;
    }

    if (this->rotatingLeft) {
        float amount = (float) this->rotateTimer / 300.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        delta[3] = (int) (amount * (float) tileH);
        delta[0] = (int) (amount * (float) tileW);
        delta[7] = (int) (-(amount * (float) tileH));
        delta[8] = (int) (-(amount * (float) tileW));
    } else if (this->rotatingRight) {
        float amount = (float) this->rotateTimer / 300.0f;
        if (amount > 1.0f)
            amount = 1.0f;
        delta[5] = (int) (amount * (float) tileH);
        delta[2] = (int) (amount * (float) tileW);
        delta[9] = (int) (-(amount * (float) tileH));
        delta[10] = (int) (-(amount * (float) tileW));
    }

    if (solved) {
        if (this->wonTimer > 0x5dc)
            goto done;
    } else {
        int image = this->mainImage;
        int screenW = *g_HackingGame_render_screen_w_a;
        int width = gCanvas->GetImage2DWidth((unsigned) (image));
        int height = gCanvas->GetImage2DHeight((unsigned) (image));
        gCanvas->DrawImage2D((unsigned) image, half_i(screenW) - half_i(width),
                             half_i(*g_HackingGame_render_screen_h) - half_i(height));
    }

    {
        int topImage = this->topImage;
        int screenW = *g_HackingGame_render_screen_w_b;
        int topWidth = gCanvas->GetImage2DWidth((unsigned) (topImage));
        ImageMeasureFn measure = g_HackingGame_render_height_fn;
        int screenH = *g_HackingGame_render_screen_h;
        int titleH = measure(gCanvas, this->mainImage);
        int topH = measure(gCanvas, this->topImage);
        void **layout = g_HackingGame_render_layout;

        gCanvas->DrawImage2D((unsigned) topImage, half_i(screenW) - topWidth,
                             half_i(screenH) - half_i(titleH) - topH + layoutOffset(*layout, 0x30c));
        gCanvas->DrawImage2D((unsigned) (topImage), half_i(screenW),
                             half_i(screenH) - half_i(titleH) - topH + layoutOffset(*layout, 0x30c),
                             (unsigned char) (true));

        if (!solved) {
            int bottomImage = this->bottomImage;
            int bottomW = gCanvas->GetImage2DWidth((unsigned) (bottomImage));
            int bottomH = gCanvas->GetImage2DHeight((unsigned) (this->mainImage));
            gCanvas->DrawImage2D((unsigned) bottomImage, half_i(screenW) - bottomW,
                                 half_i(bottomH) + half_i(screenH) + layoutOffset(*layout, 0x314));
            gCanvas->DrawImage2D((unsigned) bottomImage, half_i(screenW),
                                 half_i(bottomH) + half_i(screenH) + layoutOffset(*layout, 0x314),
                                 (unsigned char) true);
        }

        float oneAndHalf = (float) tileW * 1.5f;
        for (unsigned i = 0; i != 6; ++i) {
            int stateIndex = this->current[i];
            bool flashSolved = solved && (((this->wonTimer / 200) & 1) == 0);
            int imageOffset = (flashSolved ? 6 : 0) + typeOffset + stateIndex;
            int image = this->tileImages[imageOffset];
            unsigned row = i / 3;
            unsigned col = i - row * 3;
            int y = (int) (((float) half_i(screenW) - oneAndHalf) + (float) (tileW * col) +
                           (float) delta[i * 2]);
            int imageH = gCanvas->GetImage2DHeight((unsigned) (this->mainImage));
            gCanvas->DrawImage2D((unsigned) image, y,
                                 row * tileH + half_i(*g_HackingGame_render_screen_h) - half_i(imageH) +
                                 delta[i * 2 + 1] + layoutOffset(*layout, 0x310));
        }

        int leftArrow = this->rotatingLeft ? this->arrowActive : this->arrowIdle;
        int arrowW = gCanvas->GetImage2DWidth((unsigned) (leftArrow));
        int arrowTitleH = gCanvas->GetImage2DHeight((unsigned) (this->mainImage));
        int arrowH = gCanvas->GetImage2DHeight((unsigned) (leftArrow));
        gCanvas->DrawImage2D((unsigned) leftArrow,
                             half_i(screenW) - half_i(tileW) - half_i(arrowW),
                             half_i(*g_HackingGame_render_screen_h) - half_i(tileH) -
                             half_i(arrowTitleH) - half_i(arrowH) + layoutOffset(*layout, 0x310));

        int rightArrow = this->rotatingRight ? this->arrowActive : this->arrowIdle;
        arrowW = gCanvas->GetImage2DWidth((unsigned) (rightArrow));
        arrowTitleH = gCanvas->GetImage2DHeight((unsigned) (this->mainImage));
        arrowH = gCanvas->GetImage2DHeight((unsigned) (rightArrow));
        gCanvas->DrawImage2D((unsigned) rightArrow,
                             half_i(screenW) + half_i(tileW) - half_i(arrowW),
                             half_i(*g_HackingGame_render_screen_h) - half_i(tileH) -
                             half_i(arrowTitleH) - half_i(arrowH) + layoutOffset(*layout, 0x310));

        if (!solved) {
            for (unsigned i = 0; i != 6; ++i) {
                unsigned row = i / 3;
                unsigned col = i - row * 3;
                int stateIndex = this->target[i];
                int image = this->tileImages[typeOffset + stateIndex];
                int imageH = gCanvas->GetImage2DHeight((unsigned) (this->mainImage));
                int y = (int) (((float) half_i(screenW) - oneAndHalf) + (float) (tileW * col));
                gCanvas->DrawImage2D((unsigned) image, y,
                                     row * tileH + half_i(*g_HackingGame_render_screen_h) + half_i(imageH) +
                                     layoutOffset(*layout, 0x318));
            }

            int mark = this->markImage;
            int markW = gCanvas->GetImage2DWidth((unsigned) (mark));
            int titleH2 = measure(gCanvas, this->mainImage);
            int markH = measure(gCanvas, mark);
            gCanvas->DrawImage2D((unsigned) mark,
                                 half_i(screenW) - half_i(tileW) - half_i(markW),
                                 half_i(*g_HackingGame_render_screen_h) + half_i(tileH) +
                                 half_i(titleH2) - half_i(markH) + layoutOffset(*layout, 0x318));
            gCanvas->DrawImage2D((unsigned) mark,
                                 half_i(screenW) + half_i(tileW) - half_i(markW),
                                 half_i(*g_HackingGame_render_screen_h) + half_i(tileH) +
                                 half_i(titleH2) - half_i(markH) + layoutOffset(*layout, 0x318));
        }
    }

done:
    return;
}

void HackingGame::reInit() {
    int local[6];

    int type = this->difficulty;
    this->rotateTimer = 0;
    this->wonTimer = 0;

    if (type == 1) {
        for (unsigned i = 0; i != 6; ++i)
            this->target[i] = i >> 1;
    } else if (type == 2) {
        for (int i = 0; i != 4; ++i)
            this->target[i] = i;
        this->target[4] = gRandom->nextInt(4);
        this->target[5] = gRandom->nextInt(4);
    } else if (type == 3) {
        for (int i = 0; i != 5; ++i)
            this->target[i] = i;
        this->target[5] = gRandom->nextInt(5);
    } else {
        for (int i = 0; i != 6; ++i)
            this->target[i] = i;
    }

    for (int i = 0x28; i != 0; --i) {
        int a = gRandom->nextInt(6);
        int b = gRandom->nextInt(6);
        int value = this->target[a];
        this->target[a] = this->target[b];
        this->target[b] = value;
    }

    for (int i = 0; i != 6; ++i) {
        int value = this->target[i];
        this->current[i] = value;
        this->working[i] = value;
    }

    for (unsigned i = 0; (int) i < this->difficulty * 2; ++i) {
        int count = gRandom->nextInt(2);
        for (int j = 0; j <= count; ++j) {
            if ((i & 1) == 0)
                rotateRightCW(false);
            else
                rotateLeftCW(false);
        }

        type = this->difficulty;
        if (i == (unsigned) (type * 2 - 1)) {
            for (int j = 0; j != 6; ++j)
                local[j] = this->working[j];
            if (solvableInNSteps(type, 0, 0, 0, local) != 0)
                i = 0;
        }
    }

    for (int i = 0; i != 6; ++i)
        this->current[i] = this->working[i];

    this->rotatingLeft = false;
    this->rotatingRight = false;
}

int HackingGame::solvableInNSteps(int steps, int depth, int leftCount, int rightCount, int *state) {
    int leftState[6];
    int rightState[6];

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
                this->rotatingLeft = true;
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
                this->rotatingRight = true;
                return solvableInNSteps(steps, depth + 1, 0, rightCount + 1, rightState);
            }
        } while (true);
    }

    return 0;
}


extern FModSound **g_HackingGame_update_sound;

int HackingGame::update(int dt) {
    if (this->rotatingLeft || this->rotatingRight) {
        int timer = this->rotateTimer + dt;
        this->rotateTimer = timer;
        if (timer > 300) {
            this->rotatingLeft = false;
            this->rotatingRight = false;
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
