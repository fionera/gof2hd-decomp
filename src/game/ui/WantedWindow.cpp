#include "game/ui/WantedWindow.h"
#include "game/core/Globals.h"
#include "game/world/Galaxy.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/world/StarMap.h"
#include "game/mission/Status.h"
#include "game/mission/Mission.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "engine/render/ImagePart.h"
#include "engine/render/PaintCanvas.h"
#include "game/ui/Layout.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "game/ui/TouchButton.h"
#include "game/world/Wanted.h"
#include "game/core/String.h"
#include "game/menu/ModStation.h"


int WantedWindow::OnTouchMove(int x, int y) {
    if (this->showingMap != 0) {
        this->starMap->OnTouchMove(x, y);
        return 0;
    }

    Layout **layout = &Globals::layout;
    if ((((*layout)->field_0xc_leftMargin < y) &&
         (y < Globals::h - (*layout)->field_0x10_rightMargin) &&
         (x < Globals::w / 2)) ||
        (Globals::mouse_wheel != 0)) {
        int delta = y - this->lastDragY;
        this->dragDelta = delta;
        this->scrollDamping = 1.0f;
        this->scrollOffset += delta;
        this->lastDragY = y;
    }

    if (Globals::w / 2 < x) {
        this->scrollWindow->OnTouchMove(x, y);
    }

    for (uint32_t i = 0; i < this->buttons->size(); ++i) {
        (*this->buttons)[i]->OnTouchMove(x, y);
    }

    (*layout)->OnTouchMove(x, y);
    if (this->detailButton != nullptr) {
        Wanted *wanted = (*this->wantedList)[this->selectedWanted];
        if (wanted->isActive() != 0) {
            this->detailButton->OnTouchMove(x, y);
        }
    }

    uint32_t selected = this->getWantedAtPosition(x, y);
    int moved = y - this->touchStartY;
    if (moved < 0) {
        moved = -moved;
    }
    if (moved > 5) {
        selected = 0xffffffffu;
    }
    this->highlightedWanted = selected;
    return 0;
}

void WantedWindow::update(int dt) {
    if (this->showingMap != 0) {
        this->starMap->update(dt);
        return;
    }

    (*this->buttons)[1]->setAlwaysPressed(true);
    this->scrollWindow->update(dt);

    if (this->dragging == 0) {
        float delta = this->scrollDamping * this->scrollVelocity;
        float mag = -delta;
        if (0.0f < delta) {
            mag = delta;
        }
        this->scrollVelocity = delta;
        if (mag > 1.0f) {
            this->scrollOffset = (int) (delta + (float) this->scrollOffset);
        }
    }

    int scroll = this->scrollOffset;
    if (scroll > 0) {
        this->scrollDamping = 1.0f;
        this->scrollVelocity = (float) -scroll * 0.5f;
    }

    int limit = this->visibleHeight - this->contentHeight;
    if (limit <= -1) {
        if (scroll < limit) {
            this->scrollDamping = 1.0f;
            this->scrollVelocity = (float) (limit - scroll) * 0.5f;
        }
    } else {
        this->scrollOffset = 0;
        this->scrollVelocity = 0;
    }
}


int WantedWindow::OnTouchBegin(int x, int y) {
    if (this->showingMap != 0) {
        this->starMap->OnTouchBegin(x, y);
        return 0;
    }

    this->lastDragY = y;
    this->touchStartY = y;
    this->dragDelta = 0;
    this->dragging = 1;
    this->scrollWindow->OnTouchBegin(x, y);

    for (uint32_t i = 0; i < this->buttons->size(); ++i) {
        (*this->buttons)[i]->OnTouchBegin(x, y);
    }

    (Globals::layout)->OnTouchBegin(x, y);
    if (this->detailButton != nullptr) {
        Wanted *wanted = (*this->wantedList)[this->selectedWanted];
        if (wanted->isActive() != 0) {
            this->detailButton->OnTouchBegin(x, y);
        }
    }

    this->highlightedWanted = this->getWantedAtPosition(x, y);
    return 0;
}

void WantedWindow::render3D() {
    if (this->showingMap != 0) {
        this->starMap->render();
    }
}

float WantedWindow::getRelativeScrollStartPos() {
    int pos = this->scrollOffset;
    if (pos > 0) {
        uint32_t u = 0x00000000u;
        float f;
        __builtin_memcpy(&f, &u, sizeof(f));
        return f;
    }
    return -(float) pos / (float) this->contentHeight;
}


uint32_t WantedWindow::getWantedAtPosition(int x, int y) {
    if (x >= this->windowX + (this->windowWidth >> 1)) {
        return 0xffffffffu;
    }

    Array<Wanted *> *list = this->wantedList;
    Layout *layout = Globals::layout;
    int numerator = y - this->windowY;
    numerator -= layout->field_0xc_leftMargin;
    numerator -= layout->field_0x20_top;
    numerator -= layout->field_0x5c;
    numerator -= this->scrollOffset;
    int idx = numerator / (layout->field_0x70_rowHeight + layout->field_0x34);
    if ((uint32_t) idx > (uint32_t)((int) list->size() - 1)) {
        return 0xffffffffu;
    }
    return (uint32_t) idx;
}


int WantedWindow::OnTouchEnd(int x, int y) {
    if (this->showingMap) {
        if (this->starMap->OnTouchEnd(x, y)) {
            // Back from the star map: restore the window frame.
            int width;
            int height;
            if (!Globals::iPad) {
                this->windowX = 0;
                this->windowY = 0;
                height = Globals::h;
                width = Globals::w;
            } else {
                int halfWidth;
                if (Globals::iPadHD) {
                    height = 703;
                    halfWidth = 457;
                    width = 914;
                } else {
                    height = 1000;
                    width = 1300;
                    if (!Globals::iPadLarge) {
                        height = 500;
                        width = 650;
                    }
                    halfWidth = (unsigned int) width >> 1;
                }
                int x = (Globals::w >> 1) - halfWidth;
                int y = (Globals::h >> 1) - ((unsigned int) height >> 1);
                this->windowX = x;
                this->windowY = y;
            }
            this->windowWidth = width;
            this->windowHeight = height;
            this->showingMap = 0;
        }
        return 0;
    }

    int delta = this->dragDelta;
    this->scrollDamping = 0.9f;
    this->dragging = 0;
    this->scrollOffset += delta;
    this->scrollOffsetSnapshot = this->scrollOffset;
    this->scrollVelocity = (delta < 0 ? -delta : delta) > 3 ? (float) delta : 0.0f;
    this->scrollWindow->OnTouchEnd(x, y);
    for (unsigned int i = 0; i < this->buttons->size(); ++i) {
        if ((*this->buttons)[i]->OnTouchEnd(x, y))
            this->lastButtonHit = i;
    }
    if ((int) this->highlightedWanted > -1) {
        int index = this->getWantedAtPosition(x, y);
        this->selectedWanted = index;
        this->highlightedWanted = index;
        this->selectWanted(index);
    }

    if (this->detailButton != nullptr && (*this->wantedList)[this->selectedWanted]->isActive() &&
        this->detailButton->OnTouchEnd(x, y)) {
        // Show the wanted person's last known position on the star map.
        this->starMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
        Station *station = (Station *) (intptr_t) Globals::galaxy->getStation(
                (*this->wantedList)[this->selectedWanted]->getLastSeen());
        StarMap *map;
        if (this->starMap != nullptr) {
            delete this->mission;
            this->mission = nullptr;
            this->mission = new Mission(0, 0, (*this->wantedList)[this->selectedWanted]->getTravelsTo());
            this->starMap->init(true, this->mission, false, -1);
            map = this->starMap;
        } else {
            delete this->mission;
            this->mission = nullptr;
            this->mission = new Mission(0, 0, (*this->wantedList)[this->selectedWanted]->getTravelsTo());
            ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap =
                    new StarMap(true, this->mission, false, -1);
            map = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
            this->starMap = map;
        }
        map->setStart(station->getSystem(), (*this->wantedList)[this->selectedWanted]->getLastSeen());
        delete station;
        this->showingMap = 1;
        Globals::layout->resetWindowDimensions();
        return 0;
    }

    if (Globals::layout->OnTouchEnd(x, y)) {
        Globals::layout->resetWindowDimensions();
        return 1;
    }
    if (Globals::layout->helpPressed())
        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(635), false));
    return 0;
}


void WantedWindow::draw() {
    if (this->showingMap) {
        this->starMap->draw();
        return;
    }

    Globals::Canvas->EnableClip(this->windowX,
                                this->windowY + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                Globals::layout->field_0x5c,
                                this->windowWidth, Globals::layout->field_0x2c_rowHeight + this->visibleHeight);
    float startPos = this->getRelativeScrollStartPos();
    float visible = (float) this->visibleHeight;
    float scrollHeight = this->getRelativeScrollHeight();
    int barHeight = (int) (scrollHeight * visible);
    int barStart = (int) (startPos * visible);
    if (barHeight > 0 || barStart > 0)
        Globals::layout->drawScrollBar(
                this->windowX + (this->windowWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                Globals::layout->field_0x48,
                this->windowY + Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0xc_leftMargin +
                Globals::layout->field_0x20_top + Globals::layout->field_0x5c,
                this->visibleHeight, barStart, barHeight);

    // The list of wanted persons on the left half.
    int rowY = this->windowY + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
               Globals::layout->field_0x5c + Globals::layout->field_0x2c_rowHeight + this->scrollOffset;
    int scrollBarWidth = barHeight >= 1 ? Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x48 : 0;
    for (unsigned int i = 0; i < this->wantedList->size(); i++) {
        Globals::layout->drawBox(i == this->selectedWanted || i == this->highlightedWanted ? 4 : 3,
                                 this->windowX + Globals::layout->buttonInsetX, rowY,
                                 (this->windowWidth >> 1) - (scrollBarWidth + Globals::layout->buttonInsetX +
                                                             Globals::layout->field_0x2c_rowHeight),
                                 Globals::layout->field_0x70_rowHeight, String("", false));
        Globals::Canvas->SetColor((*this->wantedList)[i]->isActive() ? 0xffffffffu : 0x777777ffu);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, (*this->wantedList)[i]->getName(),
                                    this->windowX + Globals::layout->buttonInsetX + Globals::layout->field_0x44,
                                    rowY + Globals::layout->field_0x70_rowHeight / 2 -
                                    Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2,
                                    false);
        if ((i == 0 && Globals::status->getCurrentCampaignMission() == 128) ||
            (i == 1 && Globals::status->getCurrentCampaignMission() == 130)) {
            // Mark the campaign target.
            Globals::Canvas->DrawImage2D(
                    this->bgImage,
                    this->windowX + Globals::layout->buttonInsetX + Globals::layout->field_0x44 +
                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                                  (*this->wantedList)[i]->getName() + String(" ", false)),
                    rowY + Globals::layout->field_0x70_rowHeight / 2 -
                    Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2);
        }
        rowY += Globals::layout->field_0x70_rowHeight + Globals::layout->field_0x34;
    }
    Globals::Canvas->DisableClip();
    Globals::Canvas->SetColor(0xffffffffu);

    Globals::layout->drawHeader(*Globals::gameText->getText(3219));
    for (unsigned int i = 0; i < this->buttons->size(); i++)
        (*this->buttons)[i]->draw();

    Globals::layout->drawBox(1, this->windowX + Globals::layout->buttonInsetX,
                             this->windowY + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                             (this->windowWidth >> 1) - (Globals::layout->field_0x2c_rowHeight +
                                                         Globals::layout->buttonInsetX),
                             Globals::layout->field_0x5c, *Globals::gameText->getText(3221));
    Globals::layout->drawBox(5, this->windowX + Globals::layout->buttonInsetX,
                             this->windowY + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                             Globals::layout->field_0x5c + Globals::layout->field_0x2c_rowHeight,
                             (this->windowWidth >> 1) - (Globals::layout->field_0x2c_rowHeight +
                                                         Globals::layout->buttonInsetX),
                             this->windowHeight - Globals::layout->field_0x20_top -
                             Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x5c -
                             2 * Globals::layout->field_0x2c_rowHeight -
                             Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                             String("", false));
    Globals::layout->drawBox(1, this->windowX + (this->windowWidth >> 1) + Globals::layout->field_0x2c_rowHeight,
                             this->windowY + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                             (this->windowWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                             Globals::layout->buttonInsetX,
                             Globals::layout->field_0x5c, *Globals::gameText->getText(3222));
    Globals::layout->drawBox(5, this->windowX + (this->windowWidth >> 1) + Globals::layout->field_0x2c_rowHeight,
                             this->windowY + Globals::layout->field_0x2c_rowHeight +
                             Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                             Globals::layout->field_0x5c,
                             (this->windowWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                             Globals::layout->buttonInsetX,
                             this->windowHeight - Globals::layout->field_0xc_leftMargin -
                             Globals::layout->field_0x2c_rowHeight * 2 - Globals::layout->field_0x20_top -
                             Globals::layout->field_0x5c -
                             Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                             String("", false));

    // Details of the selected wanted person on the right half.
    if (this->imageParts != nullptr) {
        int imageX = this->windowX + (this->windowWidth >> 1) + Globals::layout->field_0x2c_rowHeight;
        int imageY = this->windowY + Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0xc_leftMargin +
                     Globals::layout->field_0x20_top + Globals::layout->field_0x5c;
        Globals::imageFactory->drawChar(this->imageParts, imageX, imageY, false);
        int textX = Globals::layout->field_0x2d4 + imageX + Globals::layout->field_0x2c_rowHeight;
        int textY = imageY;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, this->nameText, textX, textY, false);
        textY += Globals::layout->field_0x4 * 2;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                    *Globals::gameText->getText(3226) + String(" ", false) + this->detailText,
                                    textX, textY, false);
        textY += Globals::layout->field_0x4;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                    *Globals::gameText->getText(3225) + String(" ", false) + this->rewardText,
                                    textX, textY, false);
        this->scrollWindow->draw();
    }
    if (this->detailButton != nullptr && (*this->wantedList)[this->selectedWanted]->isActive())
        this->detailButton->draw();
    Globals::layout->drawFooter();
}


int WantedWindow::init() {
    this->scrollOffset = 0;
    this->lastDragY = 0;
    this->scrollOffsetSnapshot = 0;
    this->dragDelta = 0;
    this->scrollDamping = 0.0f;
    this->scrollVelocity = 0.0f;
    this->touchStartY = 0;
    this->dragging = 0;

    // Only the wanted persons posted on this system's board are listed.
    this->wantedList = new Array<Wanted *>();
    Array<Wanted *> *allWanted = Globals::status->getWanted();
    int detailWidth = Globals::layout->field_0xcc - Globals::layout->buttonInsetX - Globals::layout->field_0x4c;
    for (unsigned int i = 0; i < allWanted->size(); i++) {
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() != (*allWanted)[i]->getBoard())
            continue;
        if ((((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 0 &&
             Globals::status->getCurrentCampaignMission() > 127) ||
            (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() != 0 &&
             Globals::status->getCurrentCampaignMission() >= 162))
            ArrayAdd((*allWanted)[i], *this->wantedList);
    }

    int width;
    int height;
    if (!Globals::iPad) {
        this->windowX = 0;
        this->windowY = 0;
        height = Globals::h;
        width = Globals::w;
    } else {
        int halfWidth;
        if (Globals::iPadHD) {
            height = 703;
            halfWidth = 457;
            width = 914;
        } else {
            height = 1000;
            width = 1300;
            if (!Globals::iPadLarge) {
                height = 500;
                width = 650;
            }
            halfWidth = (unsigned int) width >> 1;
        }
        this->windowX = (Globals::w >> 1) - halfWidth;
        this->windowY = (Globals::h >> 1) - ((unsigned int) height >> 1);
    }
    this->windowWidth = width;
    this->windowHeight = height;

    // Preselect the first active wanted person.
    this->selectedWanted = 0;
    for (unsigned int i = 0; i < this->wantedList->size(); i++) {
        if ((*this->wantedList)[i]->isActive()) {
            this->selectedWanted = i;
            break;
        }
    }
    if (this->selectedWanted == this->wantedList->size() - 1 &&
        !(*this->wantedList)[this->selectedWanted]->isActive())
        this->selectedWanted = 0;
    this->highlightedWanted = this->selectedWanted;
    this->selectWanted(this->selectedWanted);

    this->buttons = new Array<TouchButton *>();
    ArraySetLength(2, *this->buttons);
    (*this->buttons)[1] = new TouchButton(*Globals::gameText->getText(3219), 3,
                                          this->windowX + this->windowWidth -
                                          Globals::layout->getHelpButtonOffset(),
                                          this->windowY, 18);
    (*this->buttons)[0] = new TouchButton(*Globals::gameText->getText(129), 3,
                                          this->windowX + this->windowWidth -
                                          Globals::layout->getHelpButtonOffset() -
                                          (*this->buttons)[1]->getWidth() + Globals::layout->field_0x38,
                                          this->windowY, 18);
    (*this->buttons)[1]->setAlwaysPressed(true);
    Globals::layout->setWindowDimensions(this->windowX, this->windowY, this->windowWidth, this->windowHeight);

    this->contentHeight = (Globals::layout->field_0x34 + Globals::layout->field_0x70_rowHeight) *
                          this->wantedList->size();
    this->visibleHeight = this->windowHeight - Globals::layout->field_0x10_rightMargin -
                          Globals::layout->field_0xc_leftMargin - Globals::layout->field_0x20_top -
                          Globals::layout->field_0x24 - Globals::layout->field_0x5c +
                          Globals::layout->field_0x2c_rowHeight;

    if (this->detailButton != nullptr)
        delete this->detailButton;
    this->starMap = nullptr;
    this->detailButton = nullptr;
    this->showingMap = 0;
    this->hangarUpdate = 0;
    this->detailButton = new TouchButton(
            *Globals::gameText->getText(424), 0,
            this->windowX + (this->windowWidth >> 1) + Globals::layout->field_0x2c_rowHeight,
            this->windowY - Globals::layout->field_0x2c_rowHeight + this->windowHeight -
            Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
            detailWidth, 33, 4);
    this->hangarUpdate = 0;
    this->lastButtonHit = 1;
    return 1;
}


WantedWindow::WantedWindow() {
    this->detailButton = nullptr;
    this->starMap = nullptr;
    this->imageParts = nullptr;
    PaintCanvas **canvas = &Globals::Canvas;
    int h = (*canvas)->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
    this->wantedList = nullptr;
    this->halfTextHeight = h / 2 - 1;
    this->mission = nullptr;
    this->scrollWindow = nullptr;
    this->buttons = nullptr;
    (*canvas)->Image2DCreate(0x454, reinterpret_cast<unsigned int &>(this->bgImage));
    this->init();
}

WantedWindow::~WantedWindow() {
    if (this->imageParts != nullptr) {
        ArrayReleaseClasses(*this->imageParts);
        delete this->imageParts;
    }
    this->imageParts = nullptr;

    if (this->buttons != nullptr) {
        ArrayReleaseClasses(*this->buttons);
        delete this->buttons;
    }
    this->buttons = nullptr;

    delete this->detailButton;
    this->detailButton = nullptr;

    delete this->wantedList;
    this->wantedList = nullptr;

    delete this->mission;
    this->mission = nullptr;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;
}

void WantedWindow::setHangarUpdate(bool needsUpdate) {
    this->hangarUpdate = needsUpdate;
}

unsigned char WantedWindow::hangarNeedsUpdate() { return this->hangarUpdate; }

float WantedWindow::getRelativeScrollHeight() {
    if (this->contentHeight < this->visibleHeight)
        return 0.0f;
    if (this->scrollOffset > 0)
        return (float) (this->visibleHeight - this->scrollOffset) / (float) this->contentHeight;
    if (this->scrollOffset < this->visibleHeight - this->contentHeight)
        return (float) (this->scrollOffset + this->contentHeight) / (float) this->contentHeight;
    return (float) this->visibleHeight / (float) this->contentHeight;
}


void WantedWindow::selectWanted(int idx) {
    delete this->imageParts;
    this->imageParts = nullptr;
    delete this->scrollWindow;
    this->scrollWindow = nullptr;

    this->imageParts = Globals::imageFactory->loadChar((*this->wantedList)[idx]->getImageParts());
    this->nameText = (*this->wantedList)[idx]->getName();

    if ((*this->wantedList)[idx]->isActive()) {
        Station *lastSeen = (Station *) (intptr_t) Globals::galaxy->getStation((*this->wantedList)[idx]->getLastSeen());
        Station *travelsTo =
                (Station *) (intptr_t) Globals::galaxy->getStation((*this->wantedList)[idx]->getTravelsTo());
        Station *current =
                (Station *) (intptr_t) Globals::galaxy->getStation((*this->wantedList)[idx]->getCurrentLocation());
        SolarSystem *lastSeenSystem = (SolarSystem *) (intptr_t) Globals::galaxy->getSystem(lastSeen->getSystem());
        SolarSystem *travelsToSystem = (SolarSystem *) (intptr_t) Globals::galaxy->getSystem(travelsTo->getSystem());
        SolarSystem *currentSystem = (SolarSystem *) (intptr_t) Globals::galaxy->getSystem(current->getSystem());
        this->fromText = lastSeen->getName() + String(" (", false) + lastSeenSystem->getName() + String(")", false);
        this->toText = travelsTo->getName() + String(" (", false) + travelsToSystem->getName() + String(")", false);
        this->atText = current->getName() + String(" (", false) + currentSystem->getName() + String(")", false);
        this->rewardText = String((*this->wantedList)[idx]->getReward()) + String("$", false);
        delete lastSeen;
        delete travelsTo;
        delete current;
    } else if ((*this->wantedList)[idx]->isTerminated()) {
        this->fromText = String(" -- ", false);
        this->toText = String(" --", false);
        this->atText = String(" --", false);
        this->rewardText = String(" --", false);
    } else {
        this->fromText = *Globals::gameText->getText(3229);
        this->toText = *Globals::gameText->getText(3229);
        this->rewardText = String((*this->wantedList)[idx]->getReward()) + String("$", false);
        this->atText = *Globals::gameText->getText(3229);
    }
    int detailId = (*this->wantedList)[idx]->isTerminated() ? 3227 : 3228;
    this->detailText = *Globals::gameText->getText(detailId);

    int top = this->windowY + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
              Globals::layout->field_0x5c + Globals::layout->field_0x2c_rowHeight;
    int height = this->windowY - Globals::layout->field_0x2c_rowHeight + this->windowHeight - top -
                 Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x2d8 - Globals::layout->field_0x24;
    if ((*this->wantedList)[idx]->isActive())
        height = height - Globals::layout->field_0x4c - Globals::layout->field_0x30_rowHeight;
    this->scrollWindow = new ScrollTouchWindow(
            this->windowX + (this->windowWidth >> 1) + Globals::layout->field_0x2c_rowHeight,
            Globals::layout->field_0x2d8 + (Globals::layout->field_0x2c_rowHeight + top),
            (this->windowWidth >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX,
            height, false);

    String body("", false);
    body = *Globals::gameText->getText(3223);
    body += String("\n", false) + this->fromText;
    body += String("\n\n", false) + *Globals::gameText->getText(3224);
    body += String("\n", false) + this->toText;
    body += String("\n\n", false) + *Globals::gameText->getText((*this->wantedList)[idx]->getIndex() + 3174);
    this->scrollWindow->setText("", body);
}
