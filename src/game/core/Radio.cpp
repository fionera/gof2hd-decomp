#include "game/core/Radio.h"
#include "game/core/RadioMessage.h"
#include "game/mission/Status.h"
#include "game/world/Wanted.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/ImageFactory.h"
#include "engine/render/ImagePart.h"
#include "engine/audio/FModSound.h"
#include "engine/core/GameText.h"
#include "game/ui/Layout.h"
#include "game/ship/Agent.h"
#include "game/core/Globals.h"
#include "game/core/CharacterImages.h"


Radio::Radio() {
    this->messages = 0;
    this->currentMessage = 0;
    this->rows = 0;
    this->faceImages = 0;
    this->msgStartTime = 0;
    this->msgTime = 0;
    this->lastMessageWasShown = false;
    this->playSound = false;
    this->face = 0;
    this->soundId = -1;

    this->boxWidth = Globals::layout->field_0x98;
    this->boxX = (Globals::w - this->boxWidth) >> 1;
    this->boxY = Globals::layout->field_0x9c;
}

Radio::~Radio() {
    if (this->faceImages != 0) {
        ArrayReleaseClasses(*this->faceImages);
        delete this->faceImages;
    }
    this->faceImages = 0;

    delete[] this->face;
    this->face = 0;

    if (this->rows != 0) {
        ArrayReleaseClasses(*this->rows);
        delete this->rows;
    }
    this->rows = 0;
}

bool Radio::isShowingMessage() {
    return this->currentMessage != 0;
}

uint8_t Radio::lastMessageShown() {
    return this->lastMessageWasShown;
}

RadioMessage *Radio::getMessage(int index) {
    return (*this->messages)[index];
}

void Radio::setMessages(Array<RadioMessage *> *messages) {
    this->messages = messages;
    if (messages != 0) {
        for (unsigned int i = 0; i < messages->size(); ++i)
            (*messages)[i]->setRadio(this);
    }
}

void Radio::setCurrentMessage(RadioMessage *message) {
    this->currentMessage = message;
}

void Radio::update(long time, PlayerEgo *ego, LevelScript *script) {
    if (this->messages == 0)
        return;

    for (unsigned int i = 0; i < this->messages->size(); ++i) {
        if (!(*this->messages)[i]->triggered(time, ego, script))
            continue;

        int imageId = (*this->messages)[i]->getImageID();
        int race;
        bool generated;

        if (imageId >= 10000) {
            delete[] this->face;
            this->face = 0;
            this->face = new int[5];
            for (int j = 0; j < 5; ++j)
                this->face[j] = (*Globals::status->wanted)[imageId - 10000]->getImageParts()[j];
            generated = true;
            race = 0;
        } else if (imageId >= 63 || imageId == 21) {
            if (imageId == 64)
                race = 0;
            else if (imageId == 65)
                race = 2;
            else
                race = imageId == 21 ? 3 : 1;
            delete[] this->face;
            this->face = 0;
            generated = true;
            this->face = Globals::imageFactory->createChar(true, race);
        } else {
            delete[] this->face;
            this->face = 0;
            int *character = CHAR_IMAGES[imageId];
            this->face = new int[5];
            for (int j = 0; j < 5; ++j)
                this->face[j] = character[j];
            generated = this->face[0] != 10;
            race = generated ? this->face[0] : 0;
            if (imageId == 9)
                race = 8;
        }

        this->faceImages = Globals::imageFactory->loadChar(this->face);

        if (this->rows != 0) {
            ArrayReleaseClasses(*this->rows);
            delete this->rows;
        }
        this->rows = 0;
        this->rows = new Array<String *>();

        String text(*Globals::gameText->getText((*this->messages)[i]->getTextID()));

        void *font = imageId == 56 ? Globals::fontAlien : Globals::font; // lint: void_ptr (Globals font handles)
        if (imageId == 19)
            font = Globals::fontAlien;
        this->font = (unsigned int) (uintptr_t) font;

        Globals::globals->getLineArray(this->font, text,
                                       this->boxWidth - 10 - Globals::layout->field_0x2d4, this->rows);

        this->msgStartTime = time;
        this->msgTime = this->rows->size() * 2000 + 1500;
        this->playSound = true;

        Agent *agent = new Agent(0, String(""), 0, 0, race, generated, 0, 0, 0, 0);
        this->soundId = Globals::globals->getDialogueSoundId((*this->messages)[i]->getTextID(), agent);
        delete agent;
        break;
    }
}

void Radio::draw(int64_t time, PlayerEgo *ego, LevelScript *script) {
    if (this->currentMessage == 0) {
        update((long) time, ego, script);
        return;
    }
    if (this->msgStartTime + 2000 >= time)
        return;

    if (this->playSound && this->soundId >= 0) {
        Globals::sound->play(this->soundId, 0, 0, 0.0f);
        this->playSound = false;
    }

    Globals::Canvas->SetColor(0xffffffffu);
    int imageId = this->currentMessage->getImageID();
    Globals::layout->setDrawColor(-209);

    int width = this->boxWidth;
    int x = this->boxX;
    int y = this->boxY;
    Layout *layout = Globals::layout;
    unsigned int textHeight = this->rows->size() * layout->field_0x4;
    if ((unsigned int) layout->field_0x2d8 > textHeight)
        textHeight = layout->field_0x2d8;
    int height = textHeight + layout->windowTopInset + 10;

    if (imageId >= 10000)
        layout->drawBox(7, x, y, width, height, (*Globals::status->wanted)[imageId - 10000]->getName());
    else
        layout->drawBox(7, x, y, width, height, *Globals::gameText->getText(imageId + 1597));

    Globals::layout->setDrawColor(-1);
    Globals::imageFactory->drawChar(this->faceImages, this->boxX + 5,
                                    Globals::layout->windowTopInset + this->boxY + 5, false);
    Globals::globals->drawLines(this->font, this->rows, Globals::layout->field_0x2d4 + this->boxX + 7,
                                Globals::layout->windowTopInset + this->boxY + 7);

    if (this->playSound)
        this->playSound = false;

    if (this->messages != 0 && this->messages->size() != 0 &&
        this->msgStartTime + this->msgTime + 2000 < time) {
        if (this->currentMessage == (*this->messages)[this->messages->size() - 1])
            this->lastMessageWasShown = true;
        this->msgStartTime = 0;
        this->currentMessage->finish();
        this->currentMessage = 0;
    }
}
