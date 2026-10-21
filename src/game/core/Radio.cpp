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

// Face descriptors of the story characters (5 image-part indices each), indexed by a radio
// message's image id. Values taken from the original's .data (CHAR_IMAGES @ 0x20df04).
static int CHAR_BRENT[5] = {0, 0, 0, 0, 0};
static int CHAR_GUNANT[5] = {0, 8, 8, 1, 8};
static int CHAR_NORRIS[5] = {2, 1, 1, 1, 1};
static int CHAR_MKKT_BKKT[5] = {0, 9, 9, 8, 9};
static int CHAR_TOMMY[5] = {4, 2, 0, 2, 2};
static int CHAR_CARLA[5] = {0, 2, 3, 3, 10};
static int CHAR_ERRKT[5] = {10, 0, 0, 1, 0};
static int CHAR_JEAN[5] = {1, 1, 0, 4, 0};
static int CHAR_PIRATE_CHIEF[5] = {0, 6, 8, 8, 7};
static int CHAR_PIRATE_1[5] = {0, 4, 8, 8, 10};
static int CHAR_PIRATE_2[5] = {0, 3, 5, 8, 6};
static int CHAR_SECURITY_GUY[5] = {0, 7, 7, 8, 5};
static int CHAR_SECURITY_WOMAN[5] = {0, 5, 4, 2, 7};
static int CHAR_KIDNAPPER[5] = {10, 1, 1, 2, 4};
static int CHAR_COMPUTER[5] = {2, 2, 2, 4, 2};
static int CHAR_INFO_PIC[5] = {11, 1, 0, 0, 1};
static int CHAR_STORY[5] = {11, 1, 0, 0, 2};
static int CHAR_TERRAN_OFFICER[5] = {11, 1, 0, 0, 3};
static int CHAR_ALIEN[5] = {0, 8, 10, 10, 4};
static int CHAR_KHADOR[5] = {9, 0, 0, 0, 0};
static int CHAR_MIDO[5] = {7, 1, 1, 0, 0};
static int CHAR_NIVELIAN_SECURITY[5] = {2, 0, 0, 1, 2};
static int CHAR_AGENT[5] = {2, 0, 0, 0, 0};
static int CHAR_TARET[5] = {0, 2, 10, 9, 2};
static int CHAR_MEKANT[5] = {1, 2, 4, 3, 1};
static int CHAR_ALICE[5] = {1, 3, 1, 5, 0};
static int CHAR_ALICE_MERC_1[5] = {10, 3, 3, 4, 6};
static int CHAR_PIRATE_KHADOR_1[5] = {0, 0, 5, 8, 6};
static int CHAR_PIRATE_KHADOR_2[5] = {0, 8, 8, 8, 0};
static int CHAR_PIRATE_KHADOR_3[5] = {0, 4, 8, 8, 4};
static int CHAR_TENNER[5] = {0, 0, 5, 8, 10};
static int CHAR_NETOR[5] = {0, 2, 10, 9, 2};
static int CHAR_CONVOY_CAPTAIN[5] = {7, 0, 0, 2, 1};
static int CHAR_TROT_LYKKT[5] = {0, 5, 4, 8, 3};
static int CHAR_SMUGGLER[5] = {4, 1, 1, 1, 1};
static int CHAR_CONNY[5] = {0, 4, 8, 8, 4};
static int CHAR_BARGAND_SURR[5] = {0, 2, 10, 9, 2};
static int CHAR_MOONSPROCKET[5] = {2, 4, 1, 4, 1};
static int CHAR_TRUNT_HARVAL[5] = {0, 8, 3, 8, 3};
static int CHAR_BRILLO[5] = {2, 2, 1, 3, 2};
static int CHAR_DRUNK_WOMAN[5] = {6, 0, 0, 2, 0};
static int CHAR_BARKEEPER[5] = {10, 3, 3, 1, 4};
static int CHAR_DEALER[5] = {0, 0, 5, 4, 6};
static int CHAR_MUSEUM_CURATOR[5] = {1, 0, 3, 1, 2};
static int CHAR_PRISM_PIRATE_1[5] = {2, 4, 1, 2, 3};
static int CHAR_PRISM_PIRATE_2[5] = {1, 0, 1, 1, 3};
static int CHAR_PRISM_PIRATE_3[5] = {7, 0, 0, 0, 0};
static int CHAR_INDUSTRIALIST[5] = {2, 4, 2, 0, 0};
static int CHAR_MINER[5] = {2, 0, 1, 1, 2};
static int CHAR_VOSSK_ON_BAKRRAM[5] = {2, 1, 0, 4, 0};
static int CHAR_BROKER_1[5] = {1, 0, 0, 3, 5};
static int CHAR_BROKER_2[5] = {0, 3, 3, 0, 5};
static int CHAR_BROKER_3[5] = {10, 0, 3, 3, 5};
static int CHAR_BROKER_4[5] = {0, 3, 6, 0, 8};
static int CHAR_HANS[5] = {10, 3, 0, 3, 3};
static int CHAR_CONNY_VOID[5] = {0, 10, 7, 2, 1};
static int CHAR_BARKEEPER_WRONG_1[5] = {12, 0, 0, 0, 0};
static int CHAR_BARKEEPER_WRONG_2[5] = {0, 0, 10, 0, 7};
static int CHAR_MIDO_FREIGHTER_CAPTAIN[5] = {0, 6, 0, 7, 4};
static int CHAR_GUNANTS_SON[5] = {2, 3, 1, 1, 4};
static int CHAR_DEEP_SCIENCE_ESCORT[5] = {2, 1, 4, 2, 0};
static int CHAR_BARKEEPER_2[5] = {0, 0, 4, 3, 5};
static int CHAR_KEITH[5] = {0, 10, 2, 9, 4};
static int *const CHAR_IMAGES[63] = {
    CHAR_BRENT, CHAR_GUNANT, CHAR_NORRIS, CHAR_MKKT_BKKT,
    CHAR_TOMMY, CHAR_CARLA, CHAR_ERRKT, CHAR_JEAN,
    CHAR_PIRATE_CHIEF, CHAR_PIRATE_1, CHAR_PIRATE_2, CHAR_SECURITY_GUY,
    CHAR_SECURITY_WOMAN, CHAR_KIDNAPPER, CHAR_COMPUTER, CHAR_INFO_PIC,
    CHAR_STORY, CHAR_TERRAN_OFFICER, CHAR_ALIEN, CHAR_KHADOR,
    CHAR_MIDO, CHAR_NIVELIAN_SECURITY, CHAR_AGENT, CHAR_TARET,
    CHAR_MEKANT, CHAR_ALICE, CHAR_ALICE_MERC_1, CHAR_PIRATE_KHADOR_1,
    CHAR_PIRATE_KHADOR_2, CHAR_PIRATE_KHADOR_3, CHAR_TENNER, CHAR_NETOR,
    CHAR_CONVOY_CAPTAIN, CHAR_TROT_LYKKT, CHAR_SMUGGLER, CHAR_CONNY,
    CHAR_BARGAND_SURR, CHAR_MOONSPROCKET, CHAR_TRUNT_HARVAL, CHAR_BRILLO,
    CHAR_DRUNK_WOMAN, CHAR_BARKEEPER, CHAR_DEALER, CHAR_MUSEUM_CURATOR,
    CHAR_PRISM_PIRATE_1, CHAR_PRISM_PIRATE_2, CHAR_PRISM_PIRATE_3, CHAR_INDUSTRIALIST,
    CHAR_MINER, CHAR_VOSSK_ON_BAKRRAM, CHAR_BROKER_1, CHAR_BROKER_2,
    CHAR_BROKER_3, CHAR_BROKER_4, CHAR_HANS, CHAR_CONNY_VOID,
    CHAR_BARKEEPER_WRONG_1, CHAR_BARKEEPER_WRONG_2, CHAR_MIDO_FREIGHTER_CAPTAIN, CHAR_GUNANTS_SON,
    CHAR_DEEP_SCIENCE_ESCORT, CHAR_BARKEEPER_2, CHAR_KEITH,
};

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
