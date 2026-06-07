#include "class.h"

extern "C" __attribute__((visibility("hidden"))) FModSound **g_Radio_drawSound;
extern "C" __attribute__((visibility("hidden"))) PaintCanvas **g_Radio_drawCanvas;
extern "C" __attribute__((visibility("hidden"))) Layout **g_Radio_drawLayout;
extern "C" __attribute__((visibility("hidden"))) void *g_Radio_drawWantedRoot;
extern "C" __attribute__((visibility("hidden"))) GameText **g_Radio_drawGameText;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **g_Radio_drawImageFactory;
extern "C" __attribute__((visibility("hidden"))) Globals **g_Radio_drawGlobals;

#define ALWAYS_INLINE __attribute__((always_inline)) inline

static ALWAYS_INLINE Wanted *draw_wanted_for_image(int imageId)
{
    void *rootHolder = g_Radio_drawWantedRoot;
    void *outer = F<void *>(rootHolder, 0x00);
    Array<Wanted *> *wanted = F<Array<Wanted *> *>(outer, 0x00);
    return wanted->data[imageId - 10000];
}

void Radio::draw(int64_t time, PlayerEgo *ego, LevelScript *script)
{
    (void)ego;
    (void)script;

    if (F<RadioMessage *>(this, 0x04) == 0) {
        return;
    }

    if (F<int64_t>(this, 0x18) + 2000 < time) {
        if (F<uint8_t>(this, 0x2d) != 0 && F<int>(this, 0x30) >= 0) {
            (*g_Radio_drawSound)->play(F<int>(this, 0x30), 0, 0, 0.0f);
            F<uint8_t>(this, 0x2d) = 0;
        }

        (*g_Radio_drawCanvas)->SetColor(0xffffffffu);
        int imageId = F<RadioMessage *>(this, 0x04)->getImageID();
        Layout *layout = *g_Radio_drawLayout;
        layout->setDrawColor(-0xd1);

        int width = F<int>(this, 0x38);
        int x = F<int>(this, 0x3c);
        int y = F<int>(this, 0x40);
        uint32_t imageHeight = F<int>(layout, 0x04) * F<uint32_t>(F<void *>(this, 0x08), 0x00);
        uint32_t minHeight = F<uint32_t>(layout, 0x2d8);
        if (minHeight > imageHeight) {
            imageHeight = minHeight;
        }
        int boxHeight = imageHeight + F<int>(layout, 0x08) + 10;

        if (imageId >= 10000) {
            String title = draw_wanted_for_image(imageId)->getName();
            Layout_drawBox(layout, 7, x, y, width, boxHeight, &title);
        } else {
            String title(*(*g_Radio_drawGameText)->getText(imageId + 0x63d), false);
            Layout_drawBox(layout, 7, x, y, width, boxHeight, &title);
        }

        layout->setDrawColor(-1);
        (*g_Radio_drawImageFactory)->drawChar(F<Array<ImagePart *> *>(this, 0x0c),
                                              F<int>(this, 0x3c) + 5,
                                              F<int>(layout, 0x08) + F<int>(this, 0x40) + 5,
                                              false);
        (*g_Radio_drawGlobals)->drawLines(F<String *>(this, 0x34),
                                          F<Array<String *> *>(this, 0x08),
                                          F<int>(layout, 0x2d4) + F<int>(this, 0x3c) + 7,
                                          F<int>(layout, 0x08) + F<int>(this, 0x40) + 7);

        if (F<uint8_t>(this, 0x2d) != 0) {
            F<uint8_t>(this, 0x2d) = 0;
        }

        Array<RadioMessage *> *messages = F<Array<RadioMessage *> *>(this, 0x00);
        if (messages != 0 && messages->length != 0) {
            int duration = F<int>(this, 0x28);
            if (F<int64_t>(this, 0x18) + (int64_t)duration + 2000 < time) {
                if (F<RadioMessage *>(this, 0x04) == messages->data[messages->length - 1]) {
                    F<uint8_t>(this, 0x2c) = 1;
                }
                F<int64_t>(this, 0x18) = 0;
                F<RadioMessage *>(this, 0x04)->finish();
                F<RadioMessage *>(this, 0x04) = 0;
            }
        }
    }
}
