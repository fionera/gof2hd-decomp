#include "engine/render/LensFlare.h"
#include "game/core/Globals.h"
#include <cmath>


static const int kFlareRed[5] = {200, 200, 255, 255, 255};
static const int kFlareGreen[5] = {200, 255, 200, 255, 255};
static const int kFlareBlue[5] = {255, 200, 200, 255, 200};

LensFlare::LensFlare(PaintCanvas *canvas) {
    this->images = new uint32_t[3];
    for (int i = 0; i < 3; i++)
        Globals::Canvas->Image2DCreate((unsigned short) (i + 0x508), this->images[i]);
    this->width = canvas->GetWidth();
    this->height = canvas->GetHeight();
    this->canvas = canvas;
}

LensFlare::~LensFlare() {
    delete this->images;
    this->images = nullptr;
}

void LensFlare::render2D(float x, float y, float depth, int colorIndex) {
    int r;
    int g;
    int b;
    if ((unsigned) colorIndex < 5) {
        b = kFlareBlue[colorIndex];
        g = kFlareGreen[colorIndex];
        r = kFlareRed[colorIndex];
    } else {
        r = 255;
        g = 255;
        b = 255;
    }

    if (depth < 0.0f && x > (float) -this->width && x < (float) (this->width * 2) &&
        y > (float) -this->height && y < (float) (this->height * 2)) {
        float cx = (float) (this->width >> 1);
        float cy = (float) (this->height >> 1);
        float dx = cx - x;
        float dy = cy - y;
        float dist = Globals::globals->sqrt(dx * dx + dy * dy);
        float dirX = -(dx * 65536.0f);
        float dirY = -(dy * 65536.0f);
        this->intensity = (colorIndex == 5 ? 80.0f : 64.0f) *
                          (1.0f - dist / (float) (this->height >> 1));
        if (dist > 0.0f) {
            dirX /= dist;
            dirY /= dist;
        }

        Globals::Canvas->SetColor(r, g, b, (unsigned char) (120.0f - (50.0f - this->intensity)));
        float offX = dist * dirX;
        float offY = dist * dirY;
        Globals::Canvas->DrawImage2D(this->images[0], (int) (cx + offX * 0.5f / 65536.0f),
                                     (int) (cy + offY * 0.5f / 65536.0f), 17, 68);

        Globals::Canvas->DrawImage2D(this->images[0], (int) (cx + offX * 0.25f / 65536.0f),
                                     (int) (cy + offY * 0.25f / 65536.0f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[0]) * 0.75f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[0]) * 0.75f),
                                     17, 68, 0);

        float nearX = offX * 0.75f / 65536.0f;
        float nearY = offY * 0.75f / 65536.0f;
        Globals::Canvas->DrawImage2D(this->images[1], (int) (cx + nearX), (int) (cy + nearY),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[1]) * 0.5f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[1]) * 0.5f),
                                     17, 68, 0);

        Globals::Canvas->SetColor(r, g, b, (unsigned char) (90.0f - (50.0f - this->intensity)));
        if (50.0f - this->intensity < 90.0f) {
            Globals::Canvas->DrawImage2D(this->images[0], (int) (cx + offX * 0.125f / 65536.0f),
                                         (int) (cy + offY * 0.125f / 65536.0f),
                                         (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[0]) * 1.25f),
                                         (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[0]) * 1.25f),
                                         17, 68, 0);
        }

        Globals::Canvas->SetColor(r, g, b, (unsigned char) (120.0f - (50.0f - this->intensity)));
        Globals::Canvas->DrawImage2D(this->images[1], (int) (cx + offX / 11.0f / 65536.0f),
                                     (int) (cy + offY / 11.0f / 65536.0f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[1]) * 0.5f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[1]) * 0.5f),
                                     17, 68, 0);

        Globals::Canvas->SetColor(r, g, b, (unsigned char) (120.0f - (50.0f - this->intensity)));
        Globals::Canvas->DrawImage2D(this->images[2], (int) (cx - nearX), (int) (cy - nearY),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[2]) * 2.0f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[2]) * 2.0f),
                                     17, 68, 0);

        Globals::Canvas->SetColor(r, g, b, (unsigned char) (120.0f - (50.0f - this->intensity)));
        Globals::Canvas->DrawImage2D(this->images[2], (int) (cx - offX * 0.2f / 65536.0f),
                                     (int) (cy - offY * 0.2f / 65536.0f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[2]) * 0.5f),
                                     (int) ((float) Globals::Canvas->GetImage2DWidth(this->images[2]) * 0.5f),
                                     17, 68, 0);

        if (this->intensity > 0.0f) {
            unsigned int saved = this->canvas->GetColor();
            this->canvas->SetColor(((g << 16) | (b << 8) | (r << 24)) + (int) this->intensity);
            this->canvas->FillRectangle(0, 0, this->width, this->height);
            this->canvas->SetColor(saved);
        }
    }
}

void LensFlare::update(int) {
    this->intensity = 0.0f;
}
