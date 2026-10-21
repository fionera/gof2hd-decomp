#include "game/world/NewsTicker.h"
#include "game/core/Globals.h"
#include "game/ui/Layout.h"
#include "engine/core/AbyssEngine.h"
#include "game/world/SolarSystem.h"
#include "game/world/NewsItem.h"
#include "game/mission/Status.h"
#include "engine/core/GameText.h"
#include "engine/core/AERandom.h"
#include "engine/file/FileRead.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"


NewsTicker::NewsTicker(int x, int y, int width, int faction, int level) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->tickerText = String("", false);
    this->textWidth = 0;

    FileRead *reader = new FileRead();
    Array<NewsItem *> *allItems = reader->loadTicker();
    delete reader;

    Array<NewsItem *> *items = new Array<NewsItem *>();
    int wanted = Globals::rnd->nextInt(1) + 2;

    for (uint32_t i = 0; i < allItems->size(); ++i) {
        NewsItem *item = (*allItems)[i];
        if (item->minLevel > 0 && item->minLevel <= level && item->maxLevel >= level && item->data[faction]) {
            ArrayAdd(item->clone(), *items);
        }
    }

    int added = 0;
    int attempts = 0;
    bool repeatLast = false;
    while (added < wanted && attempts < 100) {
        NewsItem *item = (*allItems)[Globals::rnd->nextInt(allItems->size())];
        SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
        if (system->getIndex() > 21 && item->id == 13) {
            continue;
        }
        if (item->maxLevel > 160 && item->minLevel <= level && Globals::rnd->nextInt(100) < 50 &&
            item->data[faction] && !item->used) {
            if (item->flag) {
                // field_160/field_164 hold the 64-bit time the last flagged news item was shown
                if (Globals::status->getPlayingTime() - *(int64_t *) &Globals::status->field_160 < 600000) {
                    repeatLast = true;
                    continue;
                }
                *(int64_t *) &Globals::status->field_160 = Globals::status->getPlayingTime();
                Globals::status->field_174 = item->id;
            }
            ArrayAdd(item->clone(), *items);
            item->used = 1;
            ++added;
        }
        ++attempts;
    }
    if (repeatLast) {
        ArrayAdd((*allItems)[Globals::status->field_174]->clone(), *items);
    }

    String separator("    +++    ", false);
    for (uint32_t i = 0; i < items->size(); ++i) {
        NewsItem *item = (*items)[i];
        String replaced = replaceTokens(String(*Globals::gameText->getText(item->id + 3262), false));
        if (item->flag) {
            if (item->used) {
                Globals::status->string_168 = replaced;
            } else {
                replaced = Globals::status->string_168;
            }
        }
        this->tickerText += replaced + separator;
    }

    this->textWidth = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->tickerText);
    if (this->textWidth < width) {
        this->tickerText += String(this->tickerText, false);
        this->textWidth = Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, this->tickerText);
    }

    if (GameText::getLanguage() == 9) {
        width = -this->textWidth;
    }
    this->touched = 0;
    this->lastTouchX = 0;
    this->scrollOffset = (float) width;

    ArrayReleaseClasses(*items);
    delete items;
    ArrayReleaseClasses(*allItems);
    delete allItems;
}

NewsTicker::~NewsTicker() {}

void NewsTicker::draw() {
    Globals::Canvas->SetColor(0x6f);
    Globals::Canvas->FillRectangle(this->x, this->y - 2, this->width,
                                   Globals::h - this->y + 2 - Globals::layout->field_0x10_rightMargin);
    Globals::Canvas->EnableClip(this->x, this->y, this->width, Globals::h);
    Globals::Canvas->SetColor(0x777777ff);
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, this->tickerText,
                                (int) (this->scrollOffset + (float) this->x), this->y, false);

    if (GameText::getLanguage() == 9) {
        if (this->scrollOffset > (float) this->textWidth) {
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, this->tickerText,
                                        (int) (this->scrollOffset + (float) this->x), this->y, false);
        }
    } else if (this->scrollOffset < (float) (this->textWidth - this->width)) {
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, this->tickerText,
                                    (int) (this->scrollOffset + (float) this->x + (float) this->textWidth),
                                    this->y, false);
    }

    Globals::Canvas->DisableClip();
}

void NewsTicker::update(int dt) {
    if (this->touched != 0) {
        return;
    }

    float step = ((float) dt / 1000.0f) * 50.0f;
    if (GameText::getLanguage() == 9) {
        this->scrollOffset += step;
        if (this->scrollOffset > (float) this->width) {
            this->scrollOffset = (float) -this->textWidth;
        }
    } else {
        this->scrollOffset -= step;
        if (this->scrollOffset < (float) -this->textWidth) {
            this->scrollOffset = 0;
        }
    }
}

int NewsTicker::getHeight() {
    return Globals::h + 2 - Globals::layout->field_0x10_rightMargin - this->y;
}

uint8_t NewsTicker::OnTouchBegin(int x, int y) {
    if (this->x <= x && x <= this->x + this->width && this->y <= y) {
        if (y <= Globals::h + 2 - Globals::layout->field_0x10_rightMargin) {
            this->touched = 1;
            this->lastTouchX = x;
        }
    }
    return this->touched;
}

bool NewsTicker::OnTouchMove(int x, int) {
    if (this->touched != 0) {
        float delta = (float) (this->lastTouchX - x);
        this->scrollOffset -= delta;
        float maxX = (float) Globals::w;
        if (maxX < this->scrollOffset) {
            this->scrollOffset = maxX;
        }
        this->lastTouchX = x;
    }
    return this->touched != 0;
}

uint32_t NewsTicker::OnTouchEnd(int, int) {
    if (this->touched == 0) {
        return 0;
    }
    this->touched = 0;
    return 1;
}

String NewsTicker::replaceTokens(String text) {
    return text;
}

