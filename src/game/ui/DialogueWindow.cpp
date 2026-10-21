#include "game/ui/DialogueWindow.h"
#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/mission/Status.h"
#include "game/ship/Agent.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "engine/render/ImagePart.h"
#include "game/ui/Layout.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"
#include "game/mission/Mission.h"
#include "game/world/Standing.h"
#include "game/world/Level.h"
#include "game/core/String.h"
#include "game/core/Globals.h"
#include "game/ui/TouchButton.h"

struct EngineSoundConfig {
    char field_0x0[0xe];
    uint8_t autoAdvanceEnabled;
};

int DialogueWindow::OnTouchBegin(int x, int y) {
    if (this->choiceActive != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
    } else {
        this->scrollWindow->OnTouchBegin(x, y);
        this->prevButton->OnTouchBegin(x, y);
        this->nextButton->OnTouchBegin(x, y);
        this->moreButton->OnTouchBegin(x, y);
    }
    return 0;
}

static const int g_dw_briefingCounts[0xa2] = {
    4, 0, 10, 0, 2, 0, 0, 8, 0, 0, 0, 2, 0, 0, 2, 0, 4, 0, 0, 0,
    0, 4, 0, 2, 8, 0, 6, 0, 0, 2, 0, 0, 2, 0, 0, 0, 4, 0, 2, 0,
    0, 6, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 22, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int g_dw_successCounts[0xa2] = {
    0, 38, 6, 10, 4, 12, 2, 10, 18, 22, 24, 26, 16, 4, 0, 22, 6, 34, 16, 10,
    2, 2, 20, 28, 0, 6, 4, 22, 0, 2, 10, 20, 22, 18, 30, 20, 4, 16, 4, 20,
    0, 10, 8, 4, 8, 0, 20, 32, 16, 0, 0, 0, 0, 0, 16, 28, 4, 20, 24, 2,
    16, 16, 26, 4, 10, 34, 24, 16, 20, 0, 2, 14, 30, 18, 4, 26, 20, 40, 0, 0,
    12, 0, 16, 18, 0, 4, 8, 0, 16, 0, 18, 0, 14, 22, 14, 32, 20, 6, 24, 24,
    0, 20, 12, 20, 14, 0, 0, 0, 30, 8, 6, 10, 6, 10, 2, 8, 36, 12, 2, 22,
    0, 18, 36, 0, 30, 12, 12, 22, 8, 0, 18, 0, 14, 6, 16, 4, 12, 0, 48, 2,
    24, 20, 10, 12, 0, 2, 24, 16, 12, 10, 6, 16, 16, 22, 32, 8, 10, 0, 10, 22,
    6, 12,
};

int DialogueWindow::getMode() {
    return this->kind;
}

int DialogueWindow::length() {
    Mission *mission = this->mission;
    if (mission != 0 && mission->isCampaignMission() != 0) {
        if (this->kind == 0) {
            return g_dw_briefingCounts[this->campaignMission] / 2;
        }
        if (this->kind == 1) {
            return g_dw_successCounts[this->campaignMission] / 2;
        }
        return 1;
    }
    if (this->kind == 0 && this->mission != 0 &&
        this->mission->getTargetStation() == 0x6c) {
        int result = 6;
        if (Globals::status->field_114 == 2) result = 0x12;
        return result;
    }
    return 1;
}

int DialogueWindow::nextPage() {
    int currentPage = this->page;
    int len = this->length();
    if (currentPage < len - 1) {
        this->page += 1;
        this->loadContent();
        return 1;
    }
    return 0;
}

bool DialogueWindow::hasLevel() {
    return this->level != 0;
}

DialogueWindow::~DialogueWindow() {
    if (this->faceParts != 0) {
        ArrayReleaseClasses(*this->faceParts);
        delete this->faceParts;
    }
    this->faceParts = 0;

    delete[] this->briefingOffsets;
    this->briefingOffsets = 0;
    delete[] this->successOffsets;
    this->successOffsets = 0;

    delete this->scrollWindow;
    this->scrollWindow = 0;

    delete this->prevButton;
    this->prevButton = 0;
    delete this->nextButton;
    this->nextButton = 0;
    delete this->moreButton;
    this->moreButton = 0;
}

int DialogueWindow::OnTouchMove(int x, int y) {
    if (this->choiceActive != 0) {
        this->choiceWindow->OnTouchMove(x, y);
    } else {
        this->scrollWindow->OnTouchMove(x, y);
        this->prevButton->OnTouchMove(x, y);
        this->nextButton->OnTouchMove(x, y);
        this->moreButton->OnTouchMove(x, y);
    }
    return 0;
}

bool DialogueWindow::isFirstPage() {
    return this->page == 0;
}

int DialogueWindow::previousPage() {
    if (this->page <= 0) return 0;
    this->page = this->page - 1;
    this->loadContent();
    return 1;
}

bool DialogueWindow::isLastPage() {
    return this->page == this->length() - 1;
}

bool DialogueWindow::hasBriefingDialogue(int id) {
    if (id > 0xa1) return false;
    return g_dw_briefingCounts[id] > 0;
}

bool DialogueWindow::hasSuccessDialogue(int id) {
    if (id > 0xa1) return false;
    return g_dw_successCounts[id] > 0;
}

void DialogueWindow::set(Mission *mission, int kind, int campaign) {
    this->mission = mission;
    this->kind = kind;
    if (kind == 2) {
        Agent *agent = mission->getAgent();
        if (agent != 0 && agent->isGenericAgent() == 0)
            agent->setOfferAccepted(false);
        mission->setFailed(true);
    } else if (kind == 1) {
        mission->getAgent();
        mission->setWon(true);
    }
    this->page = 0;
    if (campaign == -1)
        campaign = Globals::status->getCurrentCampaignMission();
    this->campaignMission = campaign;
    this->loadContent();
}




static const char g_dw_emptyLoad[] = "";

static int g_dw_campaignBriefingTextIds[0x1000];

static int g_dw_campaignSuccessTextIds[0x1000];

static int *g_dw_defaultClientImage = nullptr;

void DialogueWindow::loadContent() {
    GameText *gameText = Globals::gameText;
    FModSound *sound = Globals::sound;

    this->nextButton->replaceTextKeepSize(*gameText->getText(0xb4));
    this->mirrorFace = 0;
    this->autoAdvanceTimer = 0;
    this->pauseLength = 0;
    sound->stop(this->voiceSound);
    this->voiceSound = -1;
    this->nextButton->setPressProgress(0);

    if (this->faceParts != 0) {
        ArrayReleaseClasses(*this->faceParts);
        delete this->faceParts;
        this->faceParts = 0;
    }

    Mission *mission = this->mission;
    int kind = this->kind;
    int page = this->page;
    int textId = -1;

    if (mission != 0 && mission->isCampaignMission() != 0) {
        int base;
        if (kind == 1) {
            base = this->successOffsets[this->campaignMission];
            textId = g_dw_campaignSuccessTextIds[base + page * 2 + 1];
        } else if (kind == 0) {
            base = this->briefingOffsets[this->campaignMission];
            textId = g_dw_campaignBriefingTextIds[base + page * 2 + 1];
        } else {
            textId = 0x10 + 0x63d;
        }
        this->clientImage = g_dw_defaultClientImage;
        this->agentName = *(gameText->getText(0x63d + (textId & 0xff)));
        this->bodyText = *(gameText->getText(textId));
    } else if (mission != 0) {
        if ((page & 1) != 0) {
            this->clientImage = g_dw_defaultClientImage;
            this->agentName = *(gameText->getText(0x63d));
            this->mirrorFace = 1;
        } else {
            this->clientImage = mission->getClientImage();
            this->agentName = mission->getClientName();
            this->mirrorFace = 0;
        }

        if (kind == 1 || kind == 0 || kind == 2) {
            Agent *agent = mission->getAgent();
            if (GameText::getLanguage() == 1 && agent != 0) {
                textId = this->pickGermanGenericTextBecauseWeSaved100EurosWithThat(kind, agent);
            } else {
                textId = 0x188 + Globals::rnd->nextInt(5);
            }
        } else {
            textId = 0x20f;
        }
        this->bodyText = *(gameText->getText(textId));

        if (kind == 1) {
            int standing = Globals::status->getStanding();
            ((Standing *) (intptr_t) standing)->applyMissionCompleted(mission->getClientRace());
        }
        if (mission->getTargetStation() == 0x6c && kind == 0) {
            textId = 0x1ca;
            this->bodyText = *(gameText->getText(textId));
        }
        if (mission->getType() == 0x0c && kind == 0) {
            textId = 0x174;
            this->bodyText = *(gameText->getText(textId));
        }
    } else {
        this->clientImage = g_dw_defaultClientImage;
        textId = 0x10;
        this->bodyText = *(gameText->getText(textId));
        this->agentName = *(gameText->getText(0x63d));
    }

    String style(g_dw_emptyLoad, false);
    String body;
    body.Set((this->bodyText).data);
    this->scrollWindow->setText(style, body, 0);

    this->prevButton->setVisible(this->page != 0);
    this->moreButton->setVisible(this->length() > 1);
    this->faceParts = (Globals::imageFactory)->loadChar(this->clientImage);

    if (this->isLastPage() != 0) {
        this->nextButton->replaceTextKeepSize(*gameText->getText(0xb5));
    }

    Agent *agent = mission == 0 ? (Agent *) 0 : mission->getAgent();
    int soundId = Globals::globals->getDialogueSoundId(textId, agent);
    this->voiceSound = soundId;
    if (soundId >= 0) {
        sound->play(soundId, 0, 0, 0);
        this->pauseLength = sound->getEventPauseLength(soundId);
    }
}

DialogueWindow::DialogueWindow() {
    this->init();
}

static EngineSoundConfig g_dw_soundConfig_storage;
static EngineSoundConfig *g_dw_soundConfig = &g_dw_soundConfig_storage;


void DialogueWindow::update(int dt) {
    if (this->scrollWindow != 0)
        this->scrollWindow->update(dt);
    if (this->choiceActive != 0)
        this->choiceWindow->update(dt);
    if (Globals::options[14] != 0 && this->voiceSound != -1) {
        Globals::sound->getPlayingProgress(this->voiceSound);
        if (Globals::sound->isPlaying(this->voiceSound) == 0 && this->isLastPage() == 0) {
            if (this->autoAdvanceTimer >= this->pauseLength)
                this->nextPage();
            this->autoAdvanceTimer = this->autoAdvanceTimer + dt;
        }
    }
}






int DialogueWindow::OnTouchEnd(int x, int y) {
    if (this->choiceActive != 0) {
        int result = this->choiceWindow->OnTouchEnd(x, y);
        if (result == 0) {
            this->choiceActive = 0;
            if (Globals::status->getCurrentCampaignMission() == 0x0f) {
                Globals::sound->play(0xa2, 0, 0, 0);
                Globals::sound->stop(Globals::sound->currentMusicEvent);
                Globals::sound->play(0x88, 0, 0, 0);
            }
            if (this->voiceSound != -1)
                Globals::sound->stop(this->voiceSound);
            return 1;
        } else if (result == 1) {
            this->choiceActive = 0;
        }
        return 0;
    }

    this->scrollWindow->OnTouchEnd(x, y);
    if (this->prevButton->OnTouchEnd(x, y) != 0) {
        (Globals::sound)->stop(this->voiceSound);
        this->previousPage();
    }
    if (this->nextButton->OnTouchEnd(x, y) != 0) {
        (Globals::sound)->stop(this->voiceSound);
        if (this->nextPage() == 0) {
            return 1;
        }
    }
    if (this->moreButton->OnTouchEnd(x, y) != 0) {
        this->choiceWindow->set(*Globals::gameText->getText(0x18c), true);
        this->choiceActive = 1;
    }
    return 0;
}

static const char g_dw_defaultAgentName[] = "";

void DialogueWindow::init() {
    this->briefingOffsets = new int[0xa2];
    this->successOffsets = new int[0xa2];

    int briefingSum = 0;
    int successSum = 0;
    for (int i = 0; i < 0xa2; i++) {
        this->briefingOffsets[i] = briefingSum;
        this->successOffsets[i] = successSum;
        briefingSum += g_dw_briefingCounts[i];
        successSum += g_dw_successCounts[i];
    }

    this->agentName = String(g_dw_defaultAgentName, false);

    this->mission = 0;
    this->choiceWindow = 0;
    this->level = 0;
    this->clientImage = 0;
    this->faceParts = 0;
    this->voiceSound = -1;
    this->autoAdvanceTimer = 0;
    this->mirrorFace = false;

    this->frameWidth = Globals::layout->field_0x54;
    this->frameHeight = Globals::layout->field_0x58;
    this->frameX = Globals::w / 2 - this->frameWidth / 2;
    this->frameY = Globals::h / 2 - this->frameHeight / 2;

    this->scrollWindow = new ScrollTouchWindow(
        this->frameX + Globals::layout->field_0x4c * 2 + Globals::layout->field_0x2d4,
        Globals::layout->windowTopInset + this->frameY,
        this->frameWidth - Globals::layout->field_0x4c * 2 - Globals::layout->field_0x2d4,
        this->frameHeight - Globals::layout->field_0x4c * 2 - Globals::layout->windowTopInset -
            Globals::layout->field_0x30_rowHeight,
        false);

    this->choiceWindow = new ChoiceWindow();

    this->prevButton = new TouchButton(*Globals::gameText->getText(0xb3), 5,
                                       this->frameX + Globals::layout->field_0x4c,
                                       this->frameY - Globals::layout->field_0x4c + this->frameHeight,
                                       Globals::layout->field_0x50, 0x21, 4);

    this->nextButton = new TouchButton(*Globals::gameText->getText(0xb4), 6,
                                       this->frameX + this->frameWidth - Globals::layout->field_0x4c,
                                       this->frameY - Globals::layout->field_0x4c + this->frameHeight,
                                       Globals::layout->field_0x50, 0x22, 4);

    this->moreButton = new TouchButton(*Globals::gameText->getText(0x18b), 0,
                                       this->frameX + this->frameWidth / 2,
                                       this->frameY + this->frameHeight - Globals::layout->field_0x4c,
                                       this->frameWidth - Globals::layout->field_0x4c * 4 -
                                           Globals::layout->field_0x50 * 2,
                                       0x24, 4);
    this->choiceActive = 0;
}

static const char g_dw_emptyString[] = "";




DialogueWindow::DialogueWindow(String *text, String *agentName, int *parts) {
    this->init();

    this->scrollWindow->setText(String(g_dw_emptyString, false), String(*text, false));

    this->moreButton->setVisible(false);
    this->prevButton->setVisible(false);

    this->faceParts = (Globals::imageFactory)->loadChar(parts);
    delete this->nextButton;
    this->nextButton = 0;

    this->nextButton = new TouchButton(*Globals::gameText->getText(0x20c), 0, this->frameX + this->frameWidth / 2,
                                       this->frameY + this->frameHeight - Globals::layout->field_0x4c,
                                       this->frameWidth - Globals::layout->field_0x4c * 4 - Globals::layout->field_0x50 * 2,
                                       0x24, 4);

    this->agentName = *(agentName);
    this->page = 0;
    this->voiceSound = -1;
    this->pauseLength = 0;
}

DialogueWindow::DialogueWindow(Mission *mission, Level *level, int kind) {
    this->init();
    this->level = level;
    this->set(mission, kind, -1);
}

static int g_dw_germanBriefingTexts[64];

static int g_dw_germanSuccessTexts[64];

static int g_dw_germanOtherTexts[64];

int DialogueWindow::pickGermanGenericTextBecauseWeSaved100EurosWithThat(int kind, Agent *agent) {
    int race = agent->getRace();
    int male;
    if (race < 10) {
        race = agent->getRace();
        male = agent->isMale();
        if (race == 3) {
            if (agent->getImageParts() == 0) {
                race = 3;
            } else {
                int *parts = agent->getImageParts();
                race = 0;
                if (*parts == 2) race = 3;
            }
        }
    } else {
        male = agent->isMale();
        race = 10;
    }

    int index;
    DialogueWindowGermanTextTable *texts;
    if (kind == 2) {
        index = Globals::rnd->nextInt(2);
        texts = (DialogueWindowGermanTextTable *) g_dw_germanSuccessTexts;
    } else if (kind != 0) {
        index = Globals::rnd->nextInt(2);
        texts = (DialogueWindowGermanTextTable *) g_dw_germanOtherTexts;
    } else {
        index = Globals::rnd->nextInt(2);
        texts = (DialogueWindowGermanTextTable *) g_dw_germanBriefingTexts;
    }

    int *picked = &(&texts->femaleVariantBase)[index];
    if (male != 0) {
        picked = &(&texts->maleRaceRow)[race * 2 + index];
    }
    return *picked;
}










void DialogueWindow::draw() {
    Globals::Canvas->SetColor((unsigned int) -1);
    Globals::layout->drawMask();
    Globals::layout->drawBox(7, this->frameX, this->frameY, this->frameWidth, this->frameHeight,
                             String(this->agentName, false));

    this->scrollWindow->draw();

    Layout *layout = Globals::layout;
    Globals::imageFactory->drawChar(this->faceParts, this->frameX + layout->field_0x4c,
                                    layout->field_0x4c + this->frameY + layout->windowTopInset, this->mirrorFace);

    this->prevButton->draw();
    this->nextButton->draw();
    this->moreButton->draw();

    if (this->choiceActive != 0) {
        this->choiceWindow->draw();
    }

    if (Globals::is_choice_window_visible == 0) {
        if (this->moreButton != 0) {
            Globals::other_buttons_x[2] = (int) this->moreButton->getPosition().x;
            Globals::other_buttons_y[2] = (int) this->moreButton->getPosition().y;
        }
        if (this->nextButton != 0) {
            Globals::other_buttons_x[3] = (int) this->nextButton->getPosition().x;
            Globals::other_buttons_y[3] = (int) this->nextButton->getPosition().y;
        }
        Globals::is_dialogue_window_visible = 1;
    }
}

void DialogueWindow::setLevel(Level *level) {
    this->level = level;
}
