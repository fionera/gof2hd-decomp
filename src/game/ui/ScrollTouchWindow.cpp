#include "game/ui/ScrollTouchWindow.h"
#include "game/core/Globals.h"
#include "game/ui/ScrollTouchBox.h"
#include "game/ui/Layout.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"

void ScrollTouchWindow::OnTouchEnd(int x, int y) {
    this->scrollBox->OnTouchEnd(x, y);
    this->touchActive = 0;
}

void ScrollTouchWindow::OnTouchMove(int x, int y) {
    this->scrollBox->OnTouchMove(x, y);
    this->touchActive = 1;
}

void ScrollTouchWindow::setTextCentered(bool centered) {
    this->scrollBox->setTextCentered(centered);
}

void ScrollTouchWindow::setYPosition(int y) {
    this->scrollBox->setYPosition(y);
}

ScrollTouchWindow::~ScrollTouchWindow() {
    delete this->scrollBox;
    this->scrollBox = nullptr;
}

void ScrollTouchWindow::update(int dt) {
    this->scrollBox->update(dt);
}

void ScrollTouchWindow::OnTouchBegin(int x, int y) {
    this->scrollBox->OnTouchBegin(x, y);
}


void ScrollTouchWindow::draw() {
    PaintCanvas **canvasHolder = reinterpret_cast<PaintCanvas **>(&Globals::Canvas);
    PaintCanvas *canvas = *canvasHolder;
    int color = canvas->GetColor();

    int scrollOffset;
    int contentHeight;
    if (this->hasFrame == 0) {
        Layout *layout = Globals::layout;
        contentHeight = this->height - layout->field_0x2c_rowHeight * 2;
        scrollOffset = 0;
    } else {
        Layout *layout = Globals::layout;
        {
            String windowTitle(this->title);
            layout->drawWindow(windowTitle, this->x, this->y,
                               this->width, this->height, true);
        }
        layout = Globals::layout;
        contentHeight = this->height - layout->field_0x2c_rowHeight * 2;
        scrollOffset = -layout->windowTopInset;
    }

    this->scrollBox->draw();
    int scrollHeight = scrollOffset + contentHeight;
    float scale = (float) scrollHeight;
    float start = this->scrollBox->getRelativeScrollStartPos();
    float height = this->scrollBox->getRelativeScrollHeight();
    int startPx = (int) (start * scale);
    int heightPx = (int) (height * scale);

    if (startPx > 0 || heightPx >= 1) {
        Layout *layout = Globals::layout;
        int yOffset = (this->hasFrame == 0) ? 0 : layout->windowTopInset;
        layout->drawScrollBar((this->x + this->width) - layout->field_0x48 - layout->field_0x2c_rowHeight,
                              this->y + layout->field_0x2c_rowHeight + yOffset,
                              scrollHeight, startPx, heightPx);
    }

    (*canvasHolder)->SetColor((unsigned int) color);
}

static const char g_STW_empty_drawTextBG[] = "";

void ScrollTouchWindow::drawTextBG() {
    Layout *layout = Globals::layout;
    int x = this->x;
    int y = this->y;
    int w = this->width;
    int pad = layout->field_0x2c_rowHeight;
    float relHeight = this->scrollBox->getRelativeScrollHeight();
    int widthInset;
    int heightInset;
    if (relHeight > 0.0f) {
        widthInset = layout->field_0x48 + pad * 2;
        heightInset = pad * 2;
    } else {
        widthInset = pad;
        heightInset = pad * 2;
    }
    int h = this->height;
    layout->drawBox(5, x, pad + y, w - widthInset, h - heightInset, AbyssEngine::String(g_STW_empty_drawTextBG), 0);
}

void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text) {
    AbyssEngine::String tmp(text, false);
    this->scrollBox->setText(tmp);
    this->title = title;
}


ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->scrollBox = new ScrollTouchBox(
        Globals::layout->field_0x4c + x,
        Globals::layout->field_0x4c + y + (hasFrame ? Globals::layout->windowTopInset : 0),
        w - Globals::layout->field_0x4c * 2,
        h - Globals::layout->field_0x4c * 2 - (hasFrame ? Globals::layout->windowTopInset : 0));
    this->touchActive = 0;
    this->hasFrame = hasFrame;
}

void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text, int color) {
    AbyssEngine::String tmp(text, false);
    this->scrollBox->setText(tmp, color);
    this->title = title;
}


ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    Layout *layout = Globals::layout;
    int top = layout->windowTopInset;
    int border = layout->field_0x4c;
    this->scrollBox = new ScrollTouchBox(border + x, top + border + y, w - border * 2,
                                         (h - top) - border * 2);
    this->touchActive = 0;
    this->hasFrame = 1;
}
