#include "class.h"

__attribute__((visibility("hidden"))) extern const char *PlayerAsteroid_qualityNames[];
__attribute__((visibility("hidden"))) extern const char PlayerAsteroid_qualityDefault[];
__attribute__((visibility("hidden"))) extern const char PlayerAsteroid_qualityFour[];

String PlayerAsteroid::getQualityString()
{
    int quality = F<int>(this, 0x150);
    unsigned int index = quality - 5U;
    const char *text;
    if (index < 3U) {
        text = PlayerAsteroid_qualityNames[index];
    } else {
        text = PlayerAsteroid_qualityFour;
        if (quality != 4) {
            text = PlayerAsteroid_qualityDefault;
        }
    }
    return String(text, false);
}
