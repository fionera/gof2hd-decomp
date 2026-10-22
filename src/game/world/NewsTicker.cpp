#include "engine/core/ArrayReleaseClasses.h"
#include "game/world/NewsTicker.h"
#include "game/core/Globals.h"
#include "game/mission/Item.h"
#include "game/world/Galaxy.h"
#include "game/world/Station.h"
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
    while (attempts < 100 && added < wanted) {
        NewsItem *item = (*allItems)[Globals::rnd->nextInt(allItems->size())];
        SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
        if (__predict_true(system->getIndex() > 21) && item->id == 13) {
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
String NewsTicker::replaceTokens(String text) {
    if (Globals::status->stringHasToken(String(text, false), String("#PLANET_NAME", false))) {
        Station *station = Globals::globals->getRandomStation();
        text = Globals::status->replaceHash(String(text, false), String(station->getName(), false),
                                            String("#PLANET_NAME", false));
        if (Globals::status->stringHasToken(String(text, false), String("#SYSTEM_NAME", false))) {
            SolarSystem *system = (SolarSystem *) (intptr_t) Globals::galaxy->getSystem(station->getSystem());
            text = Globals::status->replaceHash(String(text, false), String(system->getName(), false),
                                                String("#SYSTEM_NAME", false));
        }
        delete station;
    }

    if (Globals::status->stringHasToken(String(text, false), String("#SYSTEM_NAME", false))) {
        SolarSystem *system;
        if (Globals::status->stringHasToken(String(text, false), String("#DRINK_NAME", false))) {
            system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
            text = Globals::status->replaceHash(String(text, false),
                                                String(*Globals::gameText->getText(system->getIndex() + 1406), false),
                                                String("#DRINK_NAME", false));
        } else {
            system = Globals::globals->getRandomSystemForDrinks();
        }
        text = Globals::status->replaceHash(String(text, false), String(system->getName(), false),
                                            String("#SYSTEM_NAME", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#CHILD_NAME", false))) {
        String name = Globals::globals->getRandomName(((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace(), true);
        int space = (int) name.IndexOf(String(" ", false));
        if (space >= 0) {
            name = name.SubString(0, space);
        }
        text = Globals::status->replaceHash(String(text, false), String(name, false), String("#CHILD_NAME", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#SHIP_NAME", false))) {
        int kind = Globals::rnd->nextInt(4);
        text = Globals::status->replaceHash(
                String(text, false),
                String(*Globals::gameText->getText(Globals::globals->getRandomEnemyFighter(kind) + 913), false),
                String("#SHIP_NAME", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#PLATFORM_NUMBER", false))) {
        unsigned int platformIndex = Globals::rnd->nextInt(6);
        const char *letter;
        switch (platformIndex) {
            case 0:
                letter = "A";
                break;
            case 1:
                letter = "B";
                break;
            case 2:
                letter = "C";
                break;
            case 3:
                letter = "D";
                break;
            default:
                letter = platformIndex == 4 ? "E" : "F";
                break;
        }
        String platform(letter, false);
        text = Globals::status->replaceHash(String(text, false), platform + String(Globals::rnd->nextInt(10)),
                                            String("#PLATFORM_NUMBER", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#CATASTROPHE", false))) {
        text = Globals::status->replaceHash(String(text, false),
                                            String(*Globals::gameText->getText(Globals::rnd->nextInt(5) + 3237), false),
                                            String("#CATASTROPHE", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#VICTIMS", false))) {
        text = Globals::status->replaceHash(
                String(text, false),
                String(Globals::layout->formatNumber((Globals::rnd->nextInt(99000) + 100000) / 1000 * 1000), false),
                String("#VICTIMS", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#BERGER_LASER", false))) {
        String laser(*Globals::gameText->getText(Globals::rnd->nextInt(4) + 1279), false);
        text = Globals::status->replaceHash(String(text, false), String(laser, false), String("#BERGER_LASER", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#VOSSK_SHIP_OR_ITEM", false))) {
        int roll = Globals::rnd->nextInt(100);
        String item;
        String price;
        if (roll < 40) {
            item = *Globals::gameText->getText(922);
        } else {
            Array<Item *> *candidates = new Array<Item *>();
            for (unsigned int i = 0; i < Globals::items->size(); ++i) {
                Item *candidate = (*Globals::items)[i];
                if (candidate->getAttribute(60) == 1 && candidate->getIngredients() == 0 &&
                    candidate->getType() != 4) {
                    ArrayAdd(candidate, *candidates);
                }
            }
            int itemIndex = (*candidates)[Globals::rnd->nextInt(candidates->size())]->getIndex();
            item = *Globals::gameText->getText(itemIndex + 1274);
            delete candidates;
        }
        price = Globals::layout->formatNumber((int) (Globals::status->getPlayingTime() / 1000000 + (roll < 40 ? 140 : 12)));
        text = Globals::status->replaceHash(String(text, false), String(item, false),
                                            String("#VOSSK_SHIP_OR_ITEM", false));
        text = Globals::status->replaceHash(String(text, false),
                                            String(price, false) + String(" ", false) + *Globals::gameText->getText(3242),
                                            String("#VOSSK_REVENUE", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#PROFESSION", false))) {
        text = Globals::status->replaceHash(String(text, false),
                                            String(*Globals::gameText->getText(Globals::rnd->nextInt(4) + 3243), false),
                                            String("#PROFESSION", false));
        int kind = Globals::rnd->nextInt(8);
        String name = Globals::globals->getRandomName(kind, true);
        text = Globals::status->replaceHash(String(text, false), String(name, false), String("#NAME", false));
        text = Globals::status->replaceHash(String(text, false),
                                            String(*Globals::gameText->getText(Globals::rnd->nextInt(4) + 3247), false),
                                            String("#ACTIVITY", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#RACE_NAME", false))) {
        text = Globals::status->replaceHash(String(text, false),
                                            String(*Globals::gameText->getText(Globals::rnd->nextInt(4) + 406), false),
                                            String("#RACE_NAME", false));
        int kind = Globals::rnd->nextInt(8);
        String name = Globals::globals->getRandomName(kind, true);
        text = Globals::status->replaceHash(String(text, false), String(name, false), String("#NAME", false));
        text = Globals::status->replaceHash(String(text, false),
                                            String(*Globals::gameText->getText(Globals::rnd->nextInt(5) + 3251), false),
                                            String("#CRIME", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#NIVELIAN_PLANET", false))) {
        Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
        int index = -1;
        while (index < 0) {
            SolarSystem *system = (*systems)[Globals::rnd->nextInt(systems->size())];
            if (system->getRace() == 2) {
                index = system->getIndex();
            }
        }
        Array<int> *stations = (Array<int> *) (*systems)[index]->getStations();
        Station *station = (Station *) (intptr_t) Globals::galaxy->getStation(
                (*stations)[Globals::rnd->nextInt(stations->size())]);
        text = Globals::status->replaceHash(String(text, false), String(station->getName(), false),
                                            String("#NIVELIAN_PLANET", false));
        String count = Globals::layout->formatNumber((Globals::rnd->nextInt(990000) + 1000000) / 1000 * 1000);
        text = Globals::status->replaceHash(String(text, false), String(count, false),
                                            String("#DEMONSTRATORS", false));
        delete station;
    }

    if (Globals::status->stringHasToken(String(text, false), String("#POLL_PERCENTAGE", false))) {
        text = Globals::status->replaceHash(String(text, false), String(Globals::rnd->nextInt(80) + 10),
                                            String("#POLL_PERCENTAGE", false));
        text = Globals::status->replaceHash(String(text, false),
                                            String(*Globals::gameText->getText(Globals::rnd->nextInt(6) + 3256), false),
                                            String("#POLL_TOPIC", false));
    }

    if (Globals::status->stringHasToken(String(text, false), String("#CASUALTIES", false))) {
        text = Globals::status->replaceHash(String(text, false),
                                            2 + String(String(Globals::rnd->nextInt(48)), false),
                                            String("#CASUALTIES", false));
    }

    return String(text, false);
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
bool NewsTicker::OnTouchEnd(int, int) {
    if (this->touched == 0) {
        return false;
    }
    this->touched = 0;
    return true;
}