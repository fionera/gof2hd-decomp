#include "class.h"

extern "C" int GameText_getLanguage();

void NewsTicker::update(int dt)
{
    if (this->f_28 != 0) {
        return;
    }

    float step = ((float)dt / 1000.0f) * 50.0f;
    if (GameText_getLanguage() == 9) {
        float x = this->f_0 + step;
        float width = (float)this->f_c;
        this->f_0 = x;
        if (x > width) {
            this->f_0 = (float)-this->f_10;
        }
    } else {
        float x = this->f_0 - step;
        float minX = (float)-this->f_10;
        this->f_0 = x;
        if (x < minX) {
            this->f_0 = 0;
        }
    }
}
