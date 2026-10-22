#include "game/ui/ScrollTouchWindow.h"
#include "game/core/Globals.h"
#include "game/ui/ScrollTouchBox.h"
#include "game/ui/Layout.h"
#include "game/core/String.h"
#include "engine/render/PaintCanvas.h"

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->scrollBox = new ScrollTouchBox(
        Globals::layout->field_0x4c + x,
        y + (hasFrame ? Globals::layout->windowTopInset : 0) + Globals::layout->field_0x4c,
        w - Globals::layout->field_0x4c * 2,
        h - Globals::layout->field_0x4c * 2 - (hasFrame ? Globals::layout->windowTopInset : 0));
    this->touchActive = 0;
    this->hasFrame = hasFrame;
}

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    this->scrollBox = new ScrollTouchBox(Globals::layout->field_0x4c + x,
                                         Globals::layout->field_0x4c + y + Globals::layout->windowTopInset,
                                         w - Globals::layout->field_0x4c * 2,
                                         h - Globals::layout->windowTopInset - Globals::layout->field_0x4c * 2);
    this->touchActive = 0;
    this->hasFrame = 1;
}

ScrollTouchWindow::~ScrollTouchWindow() {
    delete this->scrollBox;
    this->scrollBox = nullptr;
}

void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text) {
    this->scrollBox->setText(text);
    this->title = title;
}

void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text, int color) {
    this->scrollBox->setText(text, color);
    this->title = title;
}

void ScrollTouchWindow::setYPosition(int y) {
    this->scrollBox->setYPosition(y);
}

void ScrollTouchWindow::draw() {
    int color = Globals::Canvas->GetColor();
    if (this->hasFrame)
        Globals::layout->drawWindow(this->title, this->x, this->y, this->width, this->height, true);
    int trackHeight = this->height - Globals::layout->field_0x2c_rowHeight * 2 -
                      (this->hasFrame ? Globals::layout->windowTopInset : 0);

    this->scrollBox->draw();
    float scale = (float) trackHeight;
    float start = this->scrollBox->getRelativeScrollStartPos();
    float size = this->scrollBox->getRelativeScrollHeight();
    int startPx = (int) (start * scale);
    int sizePx = (int) (size * scale);
    if (startPx > 0 || sizePx >= 1) {
        Layout *layout = Globals::layout;
        int barX = this->x + this->width - layout->field_0x48 - layout->field_0x2c_rowHeight;
        int y = this->y;
        int topInset = this->hasFrame ? layout->windowTopInset : 0;
        layout->drawScrollBar(barX, y + layout->field_0x2c_rowHeight + topInset, trackHeight, startPx, sizePx);
    }
    Globals::Canvas->SetColor((unsigned int) color);
}

void ScrollTouchWindow::setTextCentered(bool centered) {
    this->scrollBox->setTextCentered(centered);
}

void ScrollTouchWindow::drawTextBG() {
    Layout *layout = Globals::layout;
    int x = this->x;
    int width = this->width;
    int top = this->y + layout->field_0x2c_rowHeight;
    int widthInset;
    int heightInset;
    if (this->scrollBox->getRelativeScrollHeight() > 0.0f) {
        widthInset = Globals::layout->field_0x48 + Globals::layout->field_0x2c_rowHeight * 2;
        heightInset = Globals::layout->field_0x2c_rowHeight * 2;
    } else {
        widthInset = Globals::layout->field_0x2c_rowHeight;
        heightInset = Globals::layout->field_0x2c_rowHeight * 2;
    }
    layout->drawBox(5, x, top, width - widthInset, this->height - heightInset, String(""));
}

void ScrollTouchWindow::update(int dt) {
    this->scrollBox->update(dt);
}

void ScrollTouchWindow::OnTouchBegin(int x, int y) {
    this->scrollBox->OnTouchBegin(x, y);
}

void ScrollTouchWindow::OnTouchMove(int x, int y) {
    this->scrollBox->OnTouchMove(x, y);
    this->touchActive = 1;
}

void ScrollTouchWindow::OnTouchEnd(int x, int y) {
    this->scrollBox->OnTouchEnd(x, y);
    this->touchActive = 0;
}
