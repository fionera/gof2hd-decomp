#include "game/ui/ScrollTouchBox.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/GameText.h"
#include "game/core/Globals.h"
#include "engine/render/Engine.h"

ScrollTouchBox::ScrollTouchBox(int x, int y, int width, int height) {
    this->touchStartY = 0;
    this->lines = nullptr;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->textWidth = width;
    this->dragging = 0;
    this->scrollOffset = 0;
    this->centered = 0;
    this->contentHeight = 0;
    this->lastDelta = 0;
    this->damping = 0.0f;
    this->velocity = 0.0f;
    this->font = (unsigned int) (uintptr_t) Globals::font;
}

ScrollTouchBox::~ScrollTouchBox() {
    if (this->lines != 0) {
        ArrayReleaseClasses(*this->lines);
        delete this->lines;
        this->lines = 0;
    }
}

void ScrollTouchBox::setText(AbyssEngine::String text, int font) {
    if (this->lines != nullptr) {
        ArrayReleaseClasses(*this->lines);
        delete this->lines;
        this->lines = nullptr;
    }

    this->font = (unsigned int) font;
    this->lines = new Array<String *>();
    Globals::globals->getLineArray(font, text, this->textWidth, this->lines);
    this->contentHeight = (int) this->lines->size() * Globals::layout->field_0x4;
    if (this->contentHeight > this->height) {
        // Too long: re-wrap leaving room for the scroll bar, plus a trailing blank line.
        this->width = this->textWidth - Globals::layout->field_0x48;
        if (this->lines != nullptr) {
            ArrayReleaseClasses(*this->lines);
            delete this->lines;
            this->lines = nullptr;
        }
        this->lines = new Array<String *>();
        Globals::globals->getLineArray(font, text, this->width, this->lines);
        ArrayAdd(new String("\n", false), *this->lines);
        this->contentHeight = (int) this->lines->size() * Globals::layout->field_0x4;
    }

    this->scrollOffset = 0;
    this->lastDelta = 0;
    this->damping = 0.0f;
    this->velocity = 0.0f;
    this->touchStartY = 0;
    this->lastTouchY = 0;
    this->dragging = 0;
}

void ScrollTouchBox::setText(AbyssEngine::String text) {
    setText(text, (int) (uintptr_t) Globals::font);
}

void ScrollTouchBox::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void ScrollTouchBox::setYPosition(int y) {
    this->y = y;
}

void ScrollTouchBox::draw() {
    // Languages whose glyphs reach below the base line get a little extra room on the last line.
    bool tallGlyphs;
    switch ((unsigned short) GameText::getLanguage()) {
        case 10:
        case 11:
        case 14:
        case 15:
            tallGlyphs = true;
            break;
        default:
            tallGlyphs = false;
            break;
    }

    Globals::Canvas->SetColor(0xffffffff);
    if (this->lines != nullptr) {
        for (unsigned int i = 0; i < this->lines->size(); i++) {
            unsigned int count = this->lines->size();
            int lastOffset = 0;
            if (i == count - 1 && tallGlyphs)
                lastOffset = Globals::retinaDisplay ? -8 : -4;

            int lineY = Globals::layout->field_0x4 * (int) i + this->y + this->scrollOffset;
            if (count == 1 ||
                (this->y <= lineY &&
                 lineY + lastOffset <= this->height + this->y - Globals::Canvas->GetTextHeight(this->font))) {
                int x;
                if (GameText::getLanguage() == 9) {
                    AbyssEngine::Engine::enableReverseFlag = 0;
                    int left = this->x;
                    int width = this->width;
                    unsigned int font = this->font;
                    String *line = (*this->lines)[i];
                    PaintCanvas *canvas = Globals::Canvas;
                    if (this->centered)
                        x = left + (width >> 1) - (canvas->GetTextWidth(font, *line) >> 1);
                    else
                        x = left + width - canvas->GetTextWidth(font, *line);
                    canvas->DrawString(font, *line, x, lineY, false);
                } else {
                    x = this->x;
                    unsigned int font = this->font;
                    String *line = (*this->lines)[i];
                    PaintCanvas *canvas = Globals::Canvas;
                    if (this->centered) {
                        int width = this->width;
                        x = x + (width >> 1) - (canvas->GetTextWidth(font, *line) >> 1);
                    }
                    canvas->DrawString(font, *line, x, lineY, false);
                }
            }
        }
    }
    AbyssEngine::Engine::enableReverseFlag = 1;
}

void ScrollTouchBox::setTextCentered(bool centered) {
    this->centered = centered;
}

float ScrollTouchBox::getRelativeScrollStartPos() {
    int pos = this->scrollOffset;
    if (pos > 0)
        return 0.0f;
    return -(float) pos / (float) this->contentHeight;
}

float ScrollTouchBox::getRelativeScrollHeight() {
    if (this->height > this->contentHeight)
        return 0.0f;
    if (this->scrollOffset > 0)
        return (float) (this->height - this->scrollOffset) / (float) this->contentHeight;
    if (this->scrollOffset < this->height - this->contentHeight)
        return (float) (this->scrollOffset + this->contentHeight) / (float) this->contentHeight;
    return (float) this->height / (float) this->contentHeight;
}

void ScrollTouchBox::update(int dt) {
    if (this->contentHeight < this->height)
        return;

    if (!this->dragging) {
        this->velocity = this->damping * this->velocity;
        if ((this->velocity > 0.0f ? this->velocity : -this->velocity) > 1.5f)
            this->scrollOffset = (int) (this->velocity + (float) this->scrollOffset);
    }

    // Spring back when scrolled past either end.
    int range = this->contentHeight - this->height;
    if (this->scrollOffset >= 1) {
        this->damping = 1.0f;
        this->velocity = (float) -this->scrollOffset * 0.5f;
    } else if (this->scrollOffset < -range) {
        this->damping = 1.0f;
        this->velocity = (float) -(this->scrollOffset + range) * 0.5f;
    }
}

bool ScrollTouchBox::touchIsInside(int x, int y) {
    int left = this->x;
    int top = 0;
    if (left <= x)
        top = this->y;
    if (left > x || top > y)
        return false;

    bool inside = false;
    if (x < left + this->width)
        inside = y < top + this->height;
    return inside;
}

void ScrollTouchBox::OnTouchBegin(int x, int y) {
    if (touchIsInside(x, y)) {
        this->touchStartY = y;
        this->lastTouchY = y;
        this->lastDelta = 0;
        this->dragging = 1;
    }
}

void ScrollTouchBox::OnTouchMove(int x, int y) {
    if (this->dragging != 0 && this->contentHeight > this->height) {
        int delta = y - this->lastTouchY;
        this->lastTouchY = y;
        this->lastDelta = delta;
        this->damping = 1.0f;
        this->scrollOffset = delta + this->scrollOffset;
    }
}

void ScrollTouchBox::OnTouchEnd(int x, int y) {
    if (this->dragging != 0) {
        int delta = this->lastDelta;
        float speed = 0.0f;
        if ((delta < 0 ? -delta : delta) > 3)
            speed = (float) delta;
        this->damping = 0.9f;
        this->dragging = 0;
        this->scrollOffset = delta + this->scrollOffset;
        this->velocity = speed;
    }
}
