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
#include "platform/libc.h"

using AbyssEngine::PaintCanvas;

// Byte-offset accessor for EXTERNAL opaque objects (the engine sound-config
// record) whose layout is not modelled here.
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }

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

__attribute__((visibility("hidden"))) extern int g_dw_briefingCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successCounts[];

int DialogueWindow::length() {
    Mission *mission = this->mission;
    if (mission != 0 && mission->isCampaignMission() != 0) {
        int *counts;
        if (this->kind == 1) {
            counts = g_dw_successCounts;
        } else {
            if (this->kind != 0) return 1;
            counts = g_dw_briefingCounts;
        }
        return counts[this->campaignMission] / 2;
    }
    if (this->kind == 0 && this->mission != 0 &&
        this->mission->getTargetStation() == 0x6c) {
        int result = 6;
        if (((int *)gStatus)[0x45] == 2) result = 0x12;
        return result;
    }
    return 1;
}

int DialogueWindow::nextPage() {
    int len = this->length();
    if (this->page < len - 1) {
        this->page = this->page + 1;
        this->loadContent();
        return 1;
    }
    return 0;
}

bool DialogueWindow::hasLevel() {
    return this->level != 0;
}

// Releases the face-part image array, the campaign briefing/success offset
// tables, the scroll window and the three touch buttons, then destroys the
// agent-name / body-text strings.
DialogueWindow::~DialogueWindow() {
    if (this->faceParts != 0) {
        for (ImagePart *part : *this->faceParts) delete part;
        this->faceParts->clear();
        delete this->faceParts;
        this->faceParts = 0;
    }

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

    this->agentName.dtor();
    this->bodyText.dtor();
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
    int len = this->length();
    return this->page == len - 1;
}

__attribute__((visibility("hidden"))) extern int g_dw_briefingDialogueIds[];

bool DialogueWindow::hasBriefingDialogue(int id) {
    if (id > 0xa1) return false;
    return g_dw_briefingDialogueIds[id] > 0;
}

__attribute__((visibility("hidden"))) extern int g_dw_successDialogueIds[];

bool DialogueWindow::hasSuccessDialogue(int id) {
    if (id > 0xa1) return false;
    return g_dw_successDialogueIds[id] > 0;
}

void DialogueWindow::set(Mission *mission, int kind, int campaign) {
    this->mission = mission;
    this->kind = kind;
    if (kind == 1) {
        mission->getAgent();
        mission->setWon(true);
    } else if (kind == 2) {
        Agent *agent = mission->getAgent();
        if (agent != 0 && agent->isGenericAgent() == 0) {
            agent->setOfferAccepted(false);
        }
        mission->setFailed(true);
    }

    this->page = 0;
    if (campaign == -1) {
        campaign = gStatus->getCurrentCampaignMission();
    }
    this->campaignMission = campaign;
    this->loadContent();
}

__attribute__((visibility("hidden"))) extern GameText **g_dw_gameTextLoad;
__attribute__((visibility("hidden"))) extern FModSound **g_dw_soundLoad;
__attribute__((visibility("hidden"))) extern ImageFactory **g_dw_imageFactoryLoad;
__attribute__((visibility("hidden"))) extern const char g_dw_emptyLoad[];
__attribute__((visibility("hidden"))) extern int g_dw_campaignBriefingTextIds[];
__attribute__((visibility("hidden"))) extern int g_dw_campaignSuccessTextIds[];
__attribute__((visibility("hidden"))) extern void *g_dw_defaultClientImage;

void DialogueWindow::loadContent() {
    GameText *gameText = *g_dw_gameTextLoad;
    FModSound *sound = *g_dw_soundLoad;

    this->nextButton->replaceTextKeepSize(*gameText->getText(0xb4));
    this->mirrorFace = 0;
    this->autoAdvanceTimer = 0;
    this->pauseLength = 0;
    sound->stop(this->voiceSound);
    this->voiceSound = -1;
    this->nextButton->setPressProgress(0);

    if (this->faceParts != 0) {
        for (ImagePart *part : *this->faceParts) delete part;
        this->faceParts->clear();
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
        this->agentName.assign(gameText->getText(0x63d + (textId & 0xff)));
        this->bodyText.assign(gameText->getText(textId));
    } else if (mission != 0) {
        if ((page & 1) != 0) {
            this->clientImage = g_dw_defaultClientImage;
            this->agentName.assign(gameText->getText(0x63d));
            this->mirrorFace = 1;
        } else {
            this->clientImage = (void *)(intptr_t)mission->getClientImage();
            this->agentName = mission->getClientName();
            this->mirrorFace = 0;
        }

        if (kind == 1 || kind == 0 || kind == 2) {
            Agent *agent = mission->getAgent();
            if (GameText::getLanguage() == 1 && agent != 0) {
                textId = this->pickGermanGenericTextBecauseWeSaved100EurosWithThat(kind, agent);
            } else {
                textId = 0x188 + gRandom->nextInt(5);
            }
        } else {
            textId = 0x20f;
        }
        this->bodyText.assign(gameText->getText(textId));

        if (kind == 1) {
            int standing = gStatus->getStanding();
            ((Standing *)(intptr_t)standing)->applyMissionCompleted(mission->getClientRace());
        }
        if (mission->getTargetStation() == 0x6c && kind == 0) {
            textId = 0x1ca;
            this->bodyText.assign(gameText->getText(textId));
        }
        if (mission->getType() == 0x0c && kind == 0) {
            textId = 0x174;
            this->bodyText.assign(gameText->getText(textId));
        }
    } else {
        this->clientImage = g_dw_defaultClientImage;
        textId = 0x10;
        this->bodyText.assign(gameText->getText(textId));
        this->agentName.assign(gameText->getText(0x63d));
    }

    String style(g_dw_emptyLoad, false);
    String body;
    body.ctor_copy(&this->bodyText, false);
    this->scrollWindow->setText(style, body, 0);

    this->prevButton->setVisible(this->page != 0);
    this->moreButton->setVisible(this->length() > 1);
    this->faceParts = (*g_dw_imageFactoryLoad)->loadChar((int *)this->clientImage);

    if (this->isLastPage() != 0) {
        this->nextButton->replaceTextKeepSize(*gameText->getText(0xb5));
    }

    Agent *agent = mission == 0 ? (Agent *)0 : mission->getAgent();
    int soundId = gGlobals->getDialogueSoundId(textId, agent);
    this->voiceSound = soundId;
    if (soundId >= 0) {
        sound->play(soundId, 0, 0, 0);
        this->pauseLength = sound->getEventPauseLength(soundId);
    }
}

DialogueWindow::DialogueWindow() {
    this->bodyText.ctor();
    this->agentName.ctor();
    this->init();
}

__attribute__((visibility("hidden"))) extern void *g_dw_soundConfig;
__attribute__((visibility("hidden"))) extern FModSound **g_dw_fmodSound;

void DialogueWindow::update(int dt) {
    if (this->scrollWindow != 0) {
        this->scrollWindow->update(dt);
    }
    if (this->choiceActive != 0) {
        this->choiceWindow->update(dt);
    }
    if (F<uint8_t>(g_dw_soundConfig, 0xe) != 0 && this->voiceSound != -1) {
        FModSound *sound = *g_dw_fmodSound;
        sound->getPlayingProgress(this->voiceSound);
        if (sound->isPlaying(this->voiceSound) == 0 && this->isLastPage() == 0) {
            if (this->autoAdvanceTimer >= this->pauseLength) {
                this->nextPage();
            }
            this->autoAdvanceTimer = this->autoAdvanceTimer + dt;
        }
    }
}

__attribute__((visibility("hidden"))) extern FModSound **g_dw_soundChoice;
__attribute__((visibility("hidden"))) extern FModSound **g_dw_soundVoice;
__attribute__((visibility("hidden"))) extern FModSound **g_dw_soundPrev;
__attribute__((visibility("hidden"))) extern FModSound **g_dw_soundNext;
__attribute__((visibility("hidden"))) extern GameText **g_dw_gameTextTouchEnd;

int DialogueWindow::OnTouchEnd(int x, int y) {
    if (this->choiceActive != 0) {
        int r = this->choiceWindow->OnTouchEnd(x, y);
        if (r == 1) {
            this->choiceActive = 0;
            return 0;
        }
        if (r != 0) return 0;
        this->choiceActive = 0;
        if (gStatus->getCurrentCampaignMission() == 0x0f) {
            FModSound *sound = *g_dw_soundChoice;
            sound->play(0xa2, 0, 0, 0);
            sound->stop(F<int>(sound, 0));
            sound->play(0x88, 0, 0, 0);
        }
        if (this->voiceSound != -1) {
            (*g_dw_soundVoice)->stop(this->voiceSound);
        }
        return 1;
    }

    this->scrollWindow->OnTouchEnd(x, y);
    if (this->prevButton->OnTouchEnd(x, y) != 0) {
        (*g_dw_soundPrev)->stop(this->voiceSound);
        this->previousPage();
    }
    if (this->nextButton->OnTouchEnd(x, y) != 0) {
        (*g_dw_soundNext)->stop(this->voiceSound);
        if (this->nextPage() == 0) {
            return 1;
        }
    }
    if (this->moreButton->OnTouchEnd(x, y) != 0) {
        String *text = (*g_dw_gameTextTouchEnd)->getText(0x18c);
        this->choiceWindow->set(*text, true);
        this->choiceActive = 1;
    }
    return 0;
}

__attribute__((visibility("hidden"))) extern int g_dw_briefingPartCounts[];
__attribute__((visibility("hidden"))) extern int g_dw_successPartCounts[];
__attribute__((visibility("hidden"))) extern const char g_dw_defaultAgentName[];
__attribute__((visibility("hidden"))) extern int *g_dw_screenWidth;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutInit;
__attribute__((visibility("hidden"))) extern int *g_dw_screenHeight;
__attribute__((visibility("hidden"))) extern GameText **g_dw_gameTextInit;

static inline int half_round_to_zero(int v) {
    return (v + ((unsigned)v >> 31)) >> 1;
}

int DialogueWindow::init() {
    this->briefingOffsets = new int[0xa2];
    this->successOffsets = new int[0xa2];

    int briefingSum = 0;
    int successSum = 0;
    for (int i = 0; i != 0xa2; ++i) {
        int briefingCount = g_dw_briefingPartCounts[i];
        int successCount = g_dw_successPartCounts[i];
        this->briefingOffsets[i] = briefingSum;
        this->successOffsets[i] = successSum;
        successSum += briefingCount;
        briefingSum += successCount;
    }

    this->agentName = String(g_dw_defaultAgentName, false);

    this->mission = 0;
    this->choiceWindow = 0;
    this->level = 0;
    this->clientImage = 0;
    this->faceParts = 0;
    this->voiceSound = -1;
    this->autoAdvanceTimer = 0;
    this->mirrorFace = 0;

    void *layout = *g_dw_layoutInit;
    int frameW = F<int>(layout, 0x54);
    int frameH = F<int>(layout, 0x58);
    this->frameWidth = frameW;
    this->frameHeight = frameH;
    this->frameX = half_round_to_zero(*g_dw_screenWidth) - half_round_to_zero(frameW);
    this->frameY = half_round_to_zero(*g_dw_screenHeight) - half_round_to_zero(frameH);

    int margin = F<int>(layout, 0x4c);
    this->scrollWindow = new ScrollTouchWindow(
        this->frameX + margin * 2 + F<int>(layout, 0x2d4),
        F<int>(layout, 0x8) + this->frameY,
        frameW - margin * 2 - F<int>(layout, 0x2d4),
        frameH - margin * 2 - F<int>(layout, 0x8) - F<int>(layout, 0x30),
        false);

    this->choiceWindow = new ChoiceWindow();

    GameText *gameText = *g_dw_gameTextInit;
    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    this->prevButton = new TouchButton(gameText->getText(0xb3), 5,
        this->frameX + margin, this->frameY - margin + this->frameHeight,
        F<int>(layout, 0x50), 0x21, 4);

    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    this->nextButton = new TouchButton(gameText->getText(0xb4), 6,
        this->frameX + this->frameWidth - margin,
        this->frameY - margin + this->frameHeight,
        F<int>(layout, 0x50), 0x22, 4);

    layout = *g_dw_layoutInit;
    margin = F<int>(layout, 0x4c);
    this->moreButton = new TouchButton(gameText->getText(0x18b), 0,
        this->frameX + half_round_to_zero(this->frameWidth),
        this->frameY + this->frameHeight - margin,
        this->frameWidth - margin * 4 - F<int>(layout, 0x50) * 2, 0x24, 4);
    this->choiceActive = 0;
    return 0;
}

__attribute__((visibility("hidden"))) extern const char g_dw_emptyString[];
__attribute__((visibility("hidden"))) extern ImageFactory **g_dw_imageFactoryCtor;
__attribute__((visibility("hidden"))) extern GameText **g_dw_gameTextCtor;
__attribute__((visibility("hidden"))) extern void **g_dw_layoutCtor;

DialogueWindow::DialogueWindow(String *text, String *agentName, int *parts) {
    this->bodyText.ctor();
    this->agentName.ctor();
    this->init();

    String blank(g_dw_emptyString, false);
    String copy;
    copy.ctor_copy(text, false);
    this->scrollWindow->setText(blank, copy);

    this->moreButton->setVisible(false);
    this->prevButton->setVisible(false);

    this->faceParts = (*g_dw_imageFactoryCtor)->loadChar(parts);
    delete this->nextButton;
    this->nextButton = 0;

    GameText *gameText = *g_dw_gameTextCtor;
    void *layout = *g_dw_layoutCtor;
    int margin = F<int>(layout, 0x4c);
    int x = this->frameX + this->frameWidth / 2;
    int y = this->frameY + this->frameHeight - margin;
    int width = this->frameWidth - margin * 4 - F<int>(layout, 0x50) * 2;
    this->nextButton = new TouchButton(gameText->getText(0x20c), 0, x, y, width, 0x24, 4);

    this->agentName.assign(agentName);
    this->voiceSound = -1;
    this->page = 0;
    this->pauseLength = 0;
}

DialogueWindow::DialogueWindow(Mission *mission, Level *level, int kind) {
    this->bodyText.ctor();
    this->agentName.ctor();
    this->init();
    this->level = level;
    this->set(mission, kind, -1);
}

__attribute__((visibility("hidden"))) extern int g_dw_germanBriefingTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanSuccessTexts[];
__attribute__((visibility("hidden"))) extern int g_dw_germanOtherTexts[];

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
    int *texts;
    if (kind == 2) {
        index = gRandom->nextInt(2);
        texts = g_dw_germanSuccessTexts;
    } else if (kind != 0) {
        index = gRandom->nextInt(2);
        texts = g_dw_germanOtherTexts;
    } else {
        index = gRandom->nextInt(2);
        texts = g_dw_germanBriefingTexts;
    }

    int *picked = (int *)((char *)texts + index * 4 + 0x48);
    if (male != 0) {
        picked = (int *)((char *)texts + race * 8 + index * 4);
    }
    return *picked;
}

__attribute__((visibility("hidden"))) extern int *g_dw_drawGuard;
__attribute__((visibility("hidden"))) extern Layout **g_dw_layoutDraw;
__attribute__((visibility("hidden"))) extern ImageFactory **g_dw_imageFactoryDraw;
__attribute__((visibility("hidden"))) extern int *g_dw_drawPositionsReady;
__attribute__((visibility("hidden"))) extern int *g_dw_moreButtonX;
__attribute__((visibility("hidden"))) extern int *g_dw_moreButtonY;
__attribute__((visibility("hidden"))) extern int *g_dw_nextButtonX;
__attribute__((visibility("hidden"))) extern int *g_dw_nextButtonY;
__attribute__((visibility("hidden"))) extern int *g_dw_drawReadyFlag;

void DialogueWindow::draw() {
    gCanvas->SetColor((unsigned int)-1);
    Layout *layout = *g_dw_layoutDraw;
    layout->drawMask();
    String title;
    title.ctor_copy(&this->agentName, false);
    layout->drawBox(7, this->frameX, this->frameY, this->frameWidth, this->frameHeight, title, 1);
    title.dtor();

    this->scrollWindow->draw();

    layout = *g_dw_layoutDraw;
    int margin = F<int>(layout, 0x4c);
    (*g_dw_imageFactoryDraw)->drawChar(this->faceParts,
        this->frameX + margin, this->frameY + margin + F<int>(layout, 0x8), this->mirrorFace);

    this->prevButton->draw();
    this->nextButton->draw();
    this->moreButton->draw();

    if (this->choiceActive != 0) {
        this->choiceWindow->draw();
    }

    if (*g_dw_drawPositionsReady == 0) {
        Vector pos;
        if (this->moreButton != 0) {
            pos = this->moreButton->getPosition();
            F<int>(g_dw_moreButtonX, 0x08) = (int)pos.x;
            pos = this->moreButton->getPosition();
            F<int>(g_dw_moreButtonY, 0x08) = (int)pos.y;
        }
        if (this->nextButton != 0) {
            pos = this->nextButton->getPosition();
            F<int>(g_dw_nextButtonX, 0x0c) = (int)pos.x;
            pos = this->nextButton->getPosition();
            F<int>(g_dw_nextButtonY, 0x0c) = (int)pos.y;
        }
        *g_dw_drawReadyFlag = 1;
    }
}

// Attach the campaign Level whose briefing/success text this window is showing.
// hasLevel() reports whether a level was bound this way.
void DialogueWindow::setLevel(Level *level) {
    this->level = level;
}

// ---- ABI shims -------------------------------------------------------------
// ModStation builds and drives its embedded DialogueWindow through a handful of
// other-unit-inlined entry points; each forwards to the recovered members. The
// raw object pointer is allocated by ModStation, so the constructors run via
// placement-new on the supplied storage.
extern "C" void DialogueWindow_ctor_ou(DialogueWindow *dw) {
    new (dw) DialogueWindow();
}

extern "C" void DialogueWindow_ctor_mission_ou(DialogueWindow *dw, void *mission,
                                               int level, int kind) {
    new (dw) DialogueWindow((Mission *)mission, (Level *)(intptr_t)level, kind);
}

extern "C" void DialogueWindow_ctor_msg_ou(DialogueWindow *dw, int titleStr,
                                           int bodyStr, int *parts) {
    new (dw) DialogueWindow((String *)(intptr_t)titleStr, (String *)(intptr_t)bodyStr, parts);
}

// set()/update() reached from the inlined station logic.  setMission passes the
// "won" kind (1) and lets the window resolve the current campaign mission (-1).
extern "C" void DialogueWindow_setMission_ou(void *dw, void *mission, int kind) {
    ((DialogueWindow *)dw)->set((Mission *)mission, kind, -1);
}

extern "C" void DialogueWindow_update_ou(int dw) {
    ((DialogueWindow *)(intptr_t)dw)->update(0);
}

extern "C" int DialogueWindow_OnTouchEnd_ote(int dw, int xy) {
    return ((DialogueWindow *)(intptr_t)dw)->OnTouchEnd(xy, xy);
}

extern "C" void DialogueWindow_dtor_ote(DialogueWindow *dw) {
    dw->~DialogueWindow();
}

// Render: ModStation passes itself; the live DialogueWindow pointer lives at
// ModStation + 0x84 (m_pDialogueWindow).  Draw it through the real member.
extern "C" void DialogueWindow_draw_r2d(void *modStation) {
    DialogueWindow *dw = *(DialogueWindow **)((char *)modStation + 0x84);
    if (dw != 0) dw->draw();
}
