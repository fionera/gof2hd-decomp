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

    Layout *layout = Globals::layout;
    if (((layout->field_0xc_leftMargin < y) &&
         (y < Globals::h - layout->field_0x10_rightMargin) &&
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

    layout->OnTouchMove(x, y);
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
        uint32_t u = 0x4650a903u;
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


void WantedWindow::OnTouchEnd(int x, int y) {
    if (this->showingMap != 0) {
        this->starMap->OnTouchEnd(x, y);
        uint32_t h;
        uint32_t w;
        uint32_t halfW = 0;
        if (Globals::iPad == 0) {
            h = Globals::h;
            w = Globals::w;
            this->windowX = 0;
            this->windowY = 0;
        } else {
            if (Globals::iPadHD == 0) {
                h = 1000;
                w = 0x514;
                if (Globals::iPadLarge == 0) {
                    h = 500;
                    w = 0x28a;
                }
                halfW = w >> 1;
            } else {
                h = 0x2bf;
                halfW = 0x1c9;
                w = 0x392;
            }
            this->windowX = (Globals::w >> 1) - halfW;
            this->windowY = (Globals::h >> 1) - (h >> 1);
        }
        this->windowWidth = w;
        this->windowHeight = h;
        this->showingMap = 0;
        return;
    }

    int delta = this->dragDelta;
    int pos = this->scrollOffset + delta;
    float velocity = 0.0f;
    int mag = delta < 0 ? -delta : delta;
    if (mag > 3) {
        velocity = (float) delta;
    }
    this->scrollDamping = 0.95f;
    this->dragging = 0;
    this->scrollOffset = pos;
    this->scrollOffsetSnapshot = pos;
    this->scrollVelocity = velocity;

    this->scrollWindow->OnTouchEnd(x, y);
    for (uint32_t i = 0; i < this->buttons->size(); ++i) {
        if ((*this->buttons)[i]->OnTouchEnd(x, y) != 0) {
            this->lastButtonHit = i;
        }
    }

    {
        uint32_t idx = this->getWantedAtPosition(x, y);
        this->selectedWanted = idx;
        this->highlightedWanted = idx;
        this->selectWanted((int) idx);
    }

    bool openMap = false;
    if (this->detailButton != nullptr) {
        Wanted *wanted = (*this->wantedList)[this->selectedWanted];
        if (wanted->isActive() != 0 &&
            this->detailButton->OnTouchEnd(x, y) != 0) {
            openMap = true;
        }
    }

    if (openMap) {
        ApplicationManager *app = Globals::appManager;
        ModStation *module = (ModStation *) app->GetApplicationModule(5);
        this->starMap = module->starMap;
        Wanted *wanted = (*this->wantedList)[this->selectedWanted];
        int lastSeen = wanted->getLastSeen();
        int stationIndex = Globals::galaxy->getStation(lastSeen);
        Station *station = (Station *) (long) stationIndex;
        delete this->mission;
        this->mission = nullptr;

        Mission *mission = new Mission(0, 0, wanted->getTravelsTo());
        this->mission = mission;

        StarMap *map = this->starMap;
        if (map == nullptr) {
            map = new StarMap(true, mission, false, -1);
            ((ModStation *) app->GetApplicationModule(5))->starMap = map;
            map = ((ModStation *) app->GetApplicationModule(5))->starMap;
            this->starMap = map;
        } else {
            map->init(true, mission, false, -1);
        }

        int system = station->getSystem();
        lastSeen = wanted->getLastSeen();
        map->setStart(system, lastSeen);
        delete station;
        this->showingMap = 1;
        (Globals::layout)->resetWindowDimensions();
    } else {
        Layout *layout = Globals::layout;
        if (layout->OnTouchEnd(x, y) != 0) {
            layout->resetWindowDimensions();
        } else if (layout->helpPressed() != 0) {
            String help;
            help.copy((Globals::gameText)->getText(0x27b), false);
            layout->initHelpWindow(help);
        }
    }
}


void WantedWindow::draw() {
    if (this->showingMap != 0) {
        this->starMap->draw();
        return;
    }

    Layout *layout = Globals::layout;
    PaintCanvas *canvas = Globals::Canvas;
    unsigned int font = (unsigned int) (uintptr_t) Globals::font;

    canvas->EnableClip(this->windowX,
                       this->windowY + layout->field_0xc_leftMargin +
                       layout->field_0x20_top + layout->field_0x5c,
                       this->windowWidth,
                       layout->field_0x2c_rowHeight + this->visibleHeight);

    float relStart = this->getRelativeScrollStartPos();
    float visf = (float) this->visibleHeight;
    float relHeight = this->getRelativeScrollHeight();
    int barSize = (int) (relHeight * visf);
    int barStart = (int) (relStart * visf);
    if (barSize >= 1 || barStart >= 0) {
        layout->drawScrollBar(((this->windowX + (this->windowWidth >> 1)) -
                               layout->field_0x2c_rowHeight) - layout->field_0x48,
                              this->windowY + layout->field_0x2c_rowHeight +
                              layout->field_0xc_leftMargin + layout->field_0x20_top +
                              layout->field_0x5c,
                              this->visibleHeight, barStart, barSize);
    }

    int y = this->windowY + layout->field_0xc_leftMargin + layout->field_0x20_top +
            layout->field_0x5c + layout->field_0x2c_rowHeight + this->scrollOffset;
    int inset = barSize < 1 ? 0 : layout->field_0x2c_rowHeight + layout->field_0x48;

    for (uint32_t i = 0; i < this->wantedList->size(); ++i) {
        int style = (i == this->selectedWanted || i == this->highlightedWanted) ? 4 : 3;
        String boxLabel("", false);
        layout->drawBox(style, layout->buttonInsetX + this->windowX, y,
                        (this->windowWidth >> 1) -
                        (inset + layout->buttonInsetX + layout->field_0x2c_rowHeight),
                        layout->field_0x70_rowHeight, boxLabel, 0);

        Wanted *wanted = (*this->wantedList)[i];
        canvas->SetColor(wanted->isActive() ? 0xffffffffu : 0xff808080u);
        String name = wanted->getName();
        int textY = y + layout->field_0x70_rowHeight / 2 - canvas->GetTextHeight(font) / 2;
        canvas->DrawString(font, name,
                           this->windowX + layout->buttonInsetX +
                           layout->field_0x44,
                           textY, false);

        int campaign = Globals::status->getCurrentCampaignMission();
        if ((i == 0 && campaign == 0x80) ||
            (i == 1 && Globals::status->getCurrentCampaignMission() == 0x82)) {
            String marked = wanted->getName();
            marked += String(" *", false);
            int textW = canvas->GetTextWidth(font, marked);
            canvas->DrawImage2D(this->bgImage,
                                this->windowX + layout->buttonInsetX +
                                layout->field_0x44 + textW,
                                textY);
        }

        y += layout->field_0x34 + layout->field_0x70_rowHeight;
    }

    canvas->DisableClip();
    canvas->SetColor(0xffffffffu);
    String header;
    header.copy((Globals::gameText)->getText(0xc93), false);
    layout->drawHeader(header);

    for (uint32_t i = 0; i < this->buttons->size(); ++i) {
        (*this->buttons)[i]->draw();
    }

    String leftHdr;
    leftHdr.copy((Globals::gameText)->getText(0xc95), false);
    layout->drawBox(1, layout->buttonInsetX + this->windowX,
                    this->windowY + layout->field_0xc_leftMargin + layout->field_0x20_top,
                    (this->windowWidth >> 1) - (layout->field_0x2c_rowHeight + layout->buttonInsetX),
                    layout->field_0x5c, leftHdr, 0);

    String leftBody("", false);
    layout->drawBox(5, layout->buttonInsetX + this->windowX,
                    this->windowY + layout->field_0xc_leftMargin + layout->field_0x20_top +
                    layout->field_0x5c + layout->field_0x2c_rowHeight,
                    (this->windowWidth >> 1) - (layout->field_0x2c_rowHeight + layout->buttonInsetX),
                    ((this->windowHeight -
                      (layout->field_0x20_top + layout->field_0xc_leftMargin +
                       layout->field_0x5c + layout->field_0x2c_rowHeight * 2)) -
                     layout->field_0x10_rightMargin) -
                    layout->field_0x24,
                    leftBody, 0);

    String rightHdr;
    rightHdr.copy((Globals::gameText)->getText(0xc95), false);
    layout->drawBox(1, this->windowX + (this->windowWidth >> 1) + layout->field_0x2c_rowHeight,
                    this->windowY + layout->field_0xc_leftMargin + layout->field_0x20_top,
                    ((this->windowWidth >> 1) - layout->field_0x2c_rowHeight) - layout->buttonInsetX,
                    layout->field_0x5c, rightHdr, 0);

    String rightBody("", false);
    layout->drawBox(5, this->windowX + (this->windowWidth >> 1) + layout->field_0x2c_rowHeight,
                    this->windowY + layout->field_0x2c_rowHeight + layout->field_0xc_leftMargin +
                    layout->field_0x20_top + layout->field_0x5c,
                    ((this->windowWidth >> 1) - layout->field_0x2c_rowHeight) - layout->buttonInsetX,
                    ((this->windowHeight -
                      (layout->field_0xc_leftMargin + layout->field_0x2c_rowHeight * 2 +
                       layout->field_0x20_top + layout->field_0x5c)) -
                     layout->field_0x10_rightMargin) -
                    layout->field_0x24,
                    rightBody, 0);

    if (this->imageParts != nullptr) {
        int charX = this->windowX + (this->windowWidth >> 1) + layout->field_0x2c_rowHeight;
        int charY = layout->field_0x5c + this->windowY + layout->field_0x2c_rowHeight +
                    layout->field_0xc_leftMargin + layout->field_0x20_top;
        (Globals::imageFactory)->drawChar(this->imageParts, charX, charY, false);
        int textX = layout->field_0x2d4 + charX + layout->field_0x2c_rowHeight;
        canvas->DrawString(font, this->nameText, textX, charY, false);

        String fromLine = String("from: ", false) +
                          *(Globals::gameText)->getText(0xc93) + this->fromText;
        canvas->DrawString(font, fromLine, textX, charY + layout->field_0x4 * 2, false);

        String toLine = String("to: ", false) +
                        *(Globals::gameText)->getText(0xc93) + this->toText;
        canvas->DrawString(font, toLine, textX, charY + layout->field_0x4 * 3, false);

        this->scrollWindow->draw();
    }

    if (this->detailButton != nullptr &&
        (*this->wantedList)[this->selectedWanted]->isActive() != 0) {
        this->detailButton->draw();
    }

    layout->drawFooter();
}


int WantedWindow::init() {
    this->scrollOffset = 0;
    this->lastDragY = 0;
    this->scrollOffsetSnapshot = 0;
    this->dragDelta = 0;

    this->wantedList = new Array<Wanted *>();

    Status *status = Globals::status;
    Array<Wanted *> *allWanted = status->getWanted();
    Layout *layout = Globals::layout;

    for (uint32_t i = 0; i < allWanted->size(); ++i) {
        int race = ((SolarSystem *) (long) status->getSystem())->getRace();
        Wanted *wanted = (*allWanted)[i];
        if (race == wanted->getBoard()) {
            race = ((SolarSystem *) (long) status->getSystem())->getRace();
            if (race != 0 || status->getCurrentCampaignMission() < 0x80) {
                race = ((SolarSystem *) (long) status->getSystem())->getRace();
                if (race == 0 && status->getCurrentCampaignMission() >= 0xa2) {
                    ArrayAdd(wanted, *(this->wantedList));
                }
            } else {
                ArrayAdd(wanted, *(this->wantedList));
            }
        }
    }

    uint32_t h;
    uint32_t w;
    uint32_t halfW = 0;
    if (Globals::iPad == 0) {
        h = Globals::h;
        w = Globals::w;
        this->windowX = 0;
        this->windowY = 0;
    } else {
        if (Globals::iPadHD == 0) {
            h = 1000;
            w = 0x514;
            if (Globals::iPadLarge == 0) {
                h = 500;
                w = 0x28a;
            }
            halfW = w >> 1;
        } else {
            h = 0x2bf;
            halfW = 0x1c9;
            w = 0x392;
        }
        this->windowX = (Globals::w >> 1) - halfW;
        this->windowY = (Globals::h >> 1) - (h >> 1);
    }

    this->windowWidth = w;
    this->windowHeight = h;
    this->selectedWanted = 0;

    uint32_t selected = 0;
    for (;;) {
        Array<Wanted *> *arr = this->wantedList;
        uint32_t count = arr->size();
        if (selected >= count) {
            selected = this->selectedWanted;
            if (selected == count - 1 && (*arr)[selected]->isActive() == 0) {
                selected = 0;
                this->selectedWanted = 0;
            } else {
                selected = this->selectedWanted;
            }
            break;
        }
        if ((*arr)[selected]->isActive() != 0) {
            this->selectedWanted = selected;
            arr = this->wantedList;
            count = arr->size();
            if (selected == count - 1 && (*arr)[selected]->isActive() == 0) {
                selected = 0;
                this->selectedWanted = 0;
            } else {
                selected = this->selectedWanted;
            }
            break;
        }
        ++selected;
    }

    this->highlightedWanted = selected;
    this->selectWanted((int) selected);

    Array<TouchButton *> *buttons = new Array<TouchButton *>();
    this->buttons = buttons;
    ArraySetLength(2, *buttons);

    GameText *text = Globals::gameText;
    {
        String *label = text->getText(0xc93);
        int helpOff = layout->getHelpButtonOffset();
        (*buttons)[1] = new TouchButton(this->windowX + (int) this->windowWidth - helpOff,
                                        this->windowY, *label, 3, 0x12, 0);
    }
    {
        String *label = text->getText(0x81);
        int helpOff = layout->getHelpButtonOffset();
        int width = (*buttons)[1]->getWidth();
        (*buttons)[0] = new TouchButton(this->windowX + (int) this->windowWidth - helpOff -
                                        width + layout->field_0x38,
                                        this->windowY, *label, 3, 0x12, 0);
    }

    (*buttons)[1]->setAlwaysPressed(true);
    layout->setWindowDimensions(this->windowX, this->windowY, this->windowWidth, this->windowHeight);

    layout = Globals::layout;
    this->contentHeight = (layout->field_0x34 + layout->field_0x70_rowHeight) *
                          (int) this->wantedList->size();
    this->visibleHeight =
            (((((this->windowHeight - layout->field_0x10_rightMargin) - layout->field_0xc_leftMargin) -
               layout->field_0x20_top) - layout->field_0x24) - layout->field_0x5c) +
            layout->field_0x2c_rowHeight;

    delete this->detailButton;
    this->starMap = nullptr;
    this->detailButton = nullptr;
    this->showingMap = 0;

    int activeWidth = layout->field_0xcc - layout->buttonInsetX - layout->field_0x4c;
    {
        String *label = text->getText(0x1a8);
        this->detailButton = new TouchButton(
            *label, 0,
            this->windowX + (this->windowWidth >> 1) + layout->field_0x2c_rowHeight,
            (((this->windowY - layout->field_0x2c_rowHeight) + this->windowHeight) -
             layout->field_0x10_rightMargin) - layout->field_0x24,
            activeWidth, 0x21, 4);
    }
    this->hangarUpdate = 0;
    this->lastButtonHit = 1;
    return 1;
}


WantedWindow::WantedWindow() {
    this->detailButton = nullptr;
    this->starMap = nullptr;
    this->imageParts = nullptr;
    PaintCanvas *canvas = Globals::Canvas;
    int h = canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
    this->wantedList = nullptr;
    this->mission = nullptr;
    this->scrollWindow = nullptr;
    this->buttons = nullptr;
    this->halfTextHeight = h / 2 - 1;
    unsigned int bgImageHandle;
    canvas->Image2DCreate(0x454, bgImageHandle);
    this->bgImage = bgImageHandle;
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
    int content = this->contentHeight;
    int visible = this->visibleHeight;
    if (content < visible) {
        uint32_t u = 0x4605e009u;
        float f;
        __builtin_memcpy(&f, &u, sizeof(f));
        return f;
    }
    int scroll = this->scrollOffset;
    int num;
    if (scroll >= 1) {
        num = visible - scroll;
    } else if (scroll >= visible - content) {
        num = visible;
    } else {
        num = scroll + content;
    }
    return (float) num / (float) content;
}


void WantedWindow::selectWanted(int idx) {
    delete this->imageParts;
    this->imageParts = nullptr;

    delete this->scrollWindow;
    this->scrollWindow = nullptr;

    Wanted *wanted = (*this->wantedList)[idx];
    this->imageParts = (Globals::imageFactory)->loadChar(wanted->getImageParts());

    this->nameText = wanted->getName();

    if (wanted->isActive() != 0) {
        Galaxy *galaxy = Globals::galaxy;
        Station *last = (Station *) (long) galaxy->getStation(wanted->getLastSeen());
        Station *travel = (Station *) (long) galaxy->getStation(wanted->getTravelsTo());
        Station *current = (Station *) (long) galaxy->getStation(wanted->getCurrentLocation());

        this->fromText = last->getName() + String("from ", false) + String(" ", false);
        this->toText = travel->getName() + String("to ", false) + String(" ", false);
        this->atText = current->getName() + String("at ", false) + String(" ", false);

        String reward;
        reward.Set((long long) (wanted->getReward()));
        this->rewardText = reward + String("reward ", false);

        delete last;
        delete travel;
        delete current;
    } else if (wanted->isTerminated() != 0) {
        this->fromText = String("terminated", false);
        this->toText = String("", false);
        this->atText = String("", false);
        this->rewardText = String("", false);
    } else {
        this->fromText = *(Globals::gameText)->getText(0xc9d);
        this->toText = *(Globals::gameText)->getText(0xc9d);
        String reward;
        reward.Set((long long) (wanted->getReward()));
        this->rewardText = reward + String("reward ", false);
        this->atText = *(Globals::gameText)->getText(0xc9d);
    }

    this->detailText = *(Globals::gameText)->getText(0xc9d);

    Layout *layout = Globals::layout;
    int y = this->windowY;
    int h = this->windowHeight;
    int top = y + layout->field_0xc_leftMargin + layout->field_0x20_top +
              layout->field_0x5c + layout->field_0x2c_rowHeight;
    int height = (((((y - layout->field_0x2c_rowHeight) + h) - top) -
                   layout->field_0x10_rightMargin) - layout->field_0x2d8) -
                 layout->field_0x24;
    if (wanted->isActive() != 0) {
        height = (height - layout->field_0x4c) - layout->field_0x30_rowHeight;
    }

    int pad = layout->field_0x2c_rowHeight;
    this->scrollWindow = new ScrollTouchWindow(
        this->windowX + (this->windowWidth >> 1) + pad,
        layout->field_0x2d8 + pad + top,
        ((this->windowWidth >> 1) - pad) - layout->buttonInsetX,
        height, false);

    String body;
    body = *(Globals::gameText)->getText(0xc9d);
    body += String("\n", false) + this->fromText;
    body += String("\n", false) + this->toText;
    body += String("\n", false) + this->atText;
    body += String("\n", false) + this->rewardText;

    String title("", false);
    this->scrollWindow->setText(title, body);
}
