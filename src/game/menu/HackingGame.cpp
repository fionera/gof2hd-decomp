#include "game/menu/HackingGame.h"
#include "game/core/Globals.h"
#include "engine/audio/FModSound.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"

HackingGame::~HackingGame() {
}

int HackingGame::getDockingIndex() {
    return this->dockingIndex;
}


void HackingGame::rotateLeftCW(bool sound) {
    if (this->wonTimer != 0)
        return;
    if (sound)
        (Globals::sound)->play(0x8e2, 0, 0, 0.0f);
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


void HackingGame::rotateRightCW(bool sound) {
    if (this->wonTimer != 0)
        return;
    if (sound)
        (Globals::sound)->play(0x8e2, 0, 0, 0.0f);
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

HackingGame::HackingGame(int type, int difficulty, int rewardItem, int rewardAmount, int dockingIndex) {
    this->type = type;
    // Each hacking-game type has its own set of six tiles plus their highlighted versions.
    for (int i = 0; i != 6; ++i) {
        Globals::Canvas->Image2DCreate((uint16_t) (i + 8010), *(unsigned int *) &this->tileImages[type * 12 + i]);
        Globals::Canvas->Image2DCreate((uint16_t) (i + 8016), *(unsigned int *) &this->tileImages[type * 12 + 6 + i]);
    }
    Globals::Canvas->Image2DCreate((uint16_t) 8008, *(unsigned int *) &this->bottomImage);
    Globals::Canvas->Image2DCreate((uint16_t) 8009, *(unsigned int *) &this->topImage);
    Globals::Canvas->Image2DCreate((uint16_t) 8007, *(unsigned int *) &this->mainImage);
    Globals::Canvas->Image2DCreate((uint16_t) 8006, *(unsigned int *) &this->arrowActive);
    Globals::Canvas->Image2DCreate((uint16_t) 8004, *(unsigned int *) &this->arrowIdle);
    Globals::Canvas->Image2DCreate((uint16_t) 8005, *(unsigned int *) &this->markImage);

    this->difficulty = difficulty;
    this->rewardItem = rewardItem;
    this->rewardAmount = rewardAmount;
    this->dockingIndex = dockingIndex;
    reInit();
}







void HackingGame::render2D() {
    Globals::Canvas->SetColor(0xffffffffu);
    bool solved = gameWon(this->current);
    int tileWidth = Globals::Canvas->GetImage2DWidth(this->tileImages[this->type * 12]);
    int tileHeight = Globals::Canvas->GetImage2DHeight(this->tileImages[this->type * 12]);

    // Pixel offsets of the tiles that are currently sliding.
    int offset[6][2];
    for (int i = 0; i != 6; i++) {
        offset[i][0] = 0;
        offset[i][1] = 0;
    }
    if (this->rotatingLeft) {
        float progress = (float) this->rotateTimer / 300.0f;
        progress = progress < 1.0f ? progress : 1.0f;
        offset[0][0] = (int) (progress * (float) tileWidth);
        offset[1][1] = (int) (progress * (float) tileHeight);
        offset[3][1] = (int) (-progress * (float) tileHeight);
        offset[4][0] = (int) (-progress * (float) tileWidth);
    } else if (this->rotatingRight) {
        float progress = (float) this->rotateTimer / 300.0f;
        progress = progress < 1.0f ? progress : 1.0f;
        offset[1][0] = (int) (progress * (float) tileWidth);
        offset[2][1] = (int) (progress * (float) tileHeight);
        offset[4][1] = (int) (-progress * (float) tileHeight);
        offset[5][0] = (int) (-progress * (float) tileWidth);
    }

    if (solved) {
        if (this->wonTimer > 1500)
            return;
    } else {
        Globals::Canvas->DrawImage2D(this->mainImage,
                            Globals::w / 2 - Globals::Canvas->GetImage2DWidth(this->mainImage) / 2,
                            Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2);
    }

    Globals::Canvas->DrawImage2D(this->topImage,
                                 Globals::w / 2 - Globals::Canvas->GetImage2DWidth(this->topImage),
                                 Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 -
                                 Globals::Canvas->GetImage2DHeight(this->topImage) + Globals::layout->field_0x30c);
    Globals::Canvas->DrawImage2D(this->topImage, Globals::w / 2,
                                 Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 -
                                 Globals::Canvas->GetImage2DHeight(this->topImage) + Globals::layout->field_0x30c,
                                 1);
    if (!solved) {
        Globals::Canvas->DrawImage2D(this->bottomImage,
                                     Globals::w / 2 - Globals::Canvas->GetImage2DWidth(this->bottomImage),
                                     Globals::h / 2 + Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 +
                                     Globals::layout->field_0x314);
        Globals::Canvas->DrawImage2D(this->bottomImage, Globals::w / 2,
                                     Globals::h / 2 + Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 +
                                     Globals::layout->field_0x314,
                                     1);
    }

    float gridLeft = (float) tileWidth * 1.5f;
    for (unsigned char i = 0; i != 6; i++) {
        int image = this->current[i];
        if (solved && (this->wonTimer / 200) % 2 == 0)
            Globals::Canvas->DrawImage2D(this->tileImages[this->type * 12 + 6 + image], (int) ((float) (Globals::w / 2) - gridLeft + (float) ((unsigned char) (i % 3) * tileWidth) + (float) offset[i][0]), (i / 3 - 2) * tileHeight + Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 + Globals::layout->field_0x310 + offset[i][1]);
        else
            Globals::Canvas->DrawImage2D(this->tileImages[this->type * 12 + image], (int) ((float) (Globals::w / 2) - gridLeft + (float) ((unsigned char) (i % 3) * tileWidth) + (float) offset[i][0]), (i / 3 - 2) * tileHeight + Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 + Globals::layout->field_0x310 + offset[i][1]);
    }
    if (this->rotatingLeft)
        Globals::Canvas->DrawImage2D(this->arrowActive,
                                     Globals::w / 2 - tileWidth / 2 -
                                     Globals::Canvas->GetImage2DWidth(this->arrowActive) / 2,
                                     Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 - tileHeight - Globals::Canvas->GetImage2DHeight(arrowActive) / 2 + Globals::layout->field_0x310
                                     );
    else
        Globals::Canvas->DrawImage2D(this->arrowIdle,
                                     Globals::w / 2 - tileWidth / 2 -
                                     Globals::Canvas->GetImage2DWidth(this->arrowIdle) / 2,
                                     Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 - tileHeight - Globals::Canvas->GetImage2DHeight(arrowIdle) / 2 + Globals::layout->field_0x310
                                     );
    if (this->rotatingRight)
        Globals::Canvas->DrawImage2D(this->arrowActive,
                                     Globals::w / 2 + tileWidth / 2 -
                                     Globals::Canvas->GetImage2DWidth(this->arrowActive) / 2,
                                     Globals::h / 2 - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 - tileHeight + Globals::layout->field_0x310 - Globals::Canvas->GetImage2DHeight(arrowActive) / 2
                                     );
    else
        Globals::Canvas->DrawImage2D(this->arrowIdle,
                                     Globals::w / 2 + tileWidth / 2 -
                                     Globals::Canvas->GetImage2DWidth(this->arrowIdle) / 2,
                                     Globals::h / 2 - tileHeight - Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 - Globals::Canvas->GetImage2DHeight(arrowIdle) / 2 + Globals::layout->field_0x310
                                     );
    if (solved)
        return;

    // The target arrangement below.
    for (unsigned char i = 0; i != 6; i++) {
        Globals::Canvas->DrawImage2D(
                this->tileImages[this->type * 12 + this->target[i]],
                (int) ((float) (Globals::w / 2) - gridLeft + (float) ((unsigned char) (i % 3) * tileWidth)),
                tileHeight * (i / 3) + Globals::h / 2 + Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 +
                Globals::layout->field_0x318);
    }
    Globals::Canvas->DrawImage2D(this->markImage,
                                 Globals::w / 2 - tileWidth / 2 - Globals::Canvas->GetImage2DWidth(this->markImage) / 2,
                                 tileHeight + Globals::h / 2 + Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 -
                                 Globals::Canvas->GetImage2DHeight(this->markImage) / 2 + Globals::layout->field_0x318);
    Globals::Canvas->DrawImage2D(this->markImage,
                                 Globals::w / 2 + tileWidth / 2 - Globals::Canvas->GetImage2DWidth(this->markImage) / 2,
                                 tileHeight + Globals::h / 2 + Globals::Canvas->GetImage2DHeight(this->mainImage) / 2 -
                                 Globals::Canvas->GetImage2DHeight(this->markImage) / 2 + Globals::layout->field_0x318);
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
        this->target[4] = Globals::rnd->nextInt(4);
        this->target[5] = Globals::rnd->nextInt(4);
    } else if (type == 3) {
        for (int i = 0; i != 5; ++i)
            this->target[i] = i;
        this->target[5] = Globals::rnd->nextInt(5);
    } else {
        for (int i = 0; i != 6; ++i)
            this->target[i] = i;
    }

    for (int i = 0; i < 40; ++i) {
        int a = Globals::rnd->nextInt(6);
        int b = Globals::rnd->nextInt(6);
        int value = this->target[a];
        this->target[a] = this->target[b];
        this->target[b] = value;
    }

    for (int i = 0; i != 6; ++i) {
        int value = this->target[i];
        this->current[i] = value;
        this->working[i] = value;
    }

    for (int i = 0; i < this->difficulty * 2; ++i) {
        int count = Globals::rnd->nextInt(2);
        for (int j = 0; j <= count; ++j) {
            if ((i & 1) == 0)
                rotateRightCW(false);
            else
                rotateLeftCW(false);
        }

        type = this->difficulty;
        if (i == type * 2 - 1) {
            for (int j = 0; j != 6; ++j)
                local[j] = this->working[j];
            if (solvableInNSteps(type, 0, 0, 0, local))
                i = 0;
        }
    }

    for (int i = 0; i != 6; ++i)
        this->current[i] = this->working[i];

    this->rotatingLeft = false;
    this->rotatingRight = false;
}

bool HackingGame::solvableInNSteps(int steps, int depth, int leftCount, int rightCount, int *state) {
    int leftState[6];
    int rightState[6];

    for (int i = 0; i != 6; ++i) {
        int value = state[i];
        leftState[i] = value;
        rightState[i] = value;
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
                return false;

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

    return false;
}


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
        (Globals::sound)->play(0x8e1, 0, 0, 0.0f);
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
