#include "Radio.h"


extern "C" __attribute__((visibility("hidden"))) void *g_Radio_wantedRoot;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **g_Radio_imageFactoryCreate;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **g_Radio_imageFactoryLoad;
extern "C" __attribute__((visibility("hidden"))) int *g_Radio_imagePartTable[];
extern "C" __attribute__((visibility("hidden"))) GameText **g_Radio_gameText;
extern "C" __attribute__((visibility("hidden"))) String **g_Radio_fontNormal;
extern "C" __attribute__((visibility("hidden"))) String **g_Radio_fontWide;
extern "C" __attribute__((visibility("hidden"))) Layout **g_Radio_layoutForText;
extern "C" __attribute__((visibility("hidden"))) Globals **g_Radio_globals;
extern "C" __attribute__((visibility("hidden"))) char g_Radio_agentName[];
extern "C" __attribute__((visibility("hidden"))) void **g_Radio_layout;
extern "C" __attribute__((visibility("hidden"))) int **g_Radio_screenWidth;
extern "C" __attribute__((visibility("hidden"))) FModSound **g_Radio_drawSound;
extern "C" __attribute__((visibility("hidden"))) PaintCanvas **g_Radio_drawCanvas;
extern "C" __attribute__((visibility("hidden"))) Layout **g_Radio_drawLayout;
extern "C" __attribute__((visibility("hidden"))) void *g_Radio_drawWantedRoot;
extern "C" __attribute__((visibility("hidden"))) GameText **g_Radio_drawGameText;
extern "C" __attribute__((visibility("hidden"))) ImageFactory **g_Radio_drawImageFactory;
extern "C" __attribute__((visibility("hidden"))) Globals **g_Radio_drawGlobals;

// ---- isShowingMessage_1555d0.cpp ----
bool Radio::isShowingMessage()
{
    return F<void *>(this, 0x04) != 0;
}

// ---- lastMessageShown_15505a.cpp ----
uint8_t Radio::lastMessageShown()
{
    return F<uint8_t>(this, 0x2c);
}

// ---- _Radio_154fd4.cpp ----
Radio::~Radio()
{
    void *array = F<void *>(this, 0x0c);
    if (array != 0) {
        ArrayReleaseClasses_ImagePart(array);
        array = F<void *>(this, 0x0c);
        if (array != 0) {
            operator_delete(Array_ImagePart_dtor(array));
        }
    }
    F<void *>(this, 0x0c) = 0;

    void *parts = F<void *>(this, 0x10);
    if (parts != 0) {
        operator_delete__(parts);
    }
    F<void *>(this, 0x10) = 0;

    array = F<void *>(this, 0x08);
    if (array != 0) {
        ArrayReleaseClasses_String(array);
        array = F<void *>(this, 0x08);
        if (array != 0) {
            operator_delete(Array_String_dtor(array));
        }
    }
    F<void *>(this, 0x08) = 0;
}

// ---- getMessage_155050.cpp ----
RadioMessage *Radio::getMessage(int index)
{
    void *messages = F<void *>(this, 0x00);
    void **data = F<void **>(messages, 0x04);
    return (RadioMessage *)data[index];
}

// ---- setMessages_15501c.cpp ----
void Radio::setMessages(Array<RadioMessage *> *messages)
{
    Radio *self = this;
    Array<RadioMessage *> *list = messages;
    bool hasMessages = list != 0;
    F<Array<RadioMessage *> *>(this, 0x00) = messages;
    if (hasMessages) {
        for (uint32_t i = 0; i < list->length; ++i) {
            list->data[i]->setRadio(self);
        }
    }
}

// ---- update_155060.cpp ----
#define ALWAYS_INLINE __attribute__((always_inline)) inline

static ALWAYS_INLINE RadioMessage *radio_message_at(Radio *self, uint32_t index)
{
    void *messages = F<void *>(self, 0x00);
    void **data = F<void **>(messages, 0x04);
    return (RadioMessage *)data[index];
}

static ALWAYS_INLINE void release_string_lines(Radio *self)
{
    void *lines = F<void *>(self, 0x08);
    if (lines != 0) {
        ArrayReleaseClasses_String(lines);
        lines = F<void *>(self, 0x08);
        if (lines != 0) {
            operator_delete(Array_String_dtor(lines));
        }
    }
    F<void *>(self, 0x08) = 0;
}

static ALWAYS_INLINE void release_parts(Radio *self)
{
    void *parts = F<void *>(self, 0x10);
    if (parts != 0) {
        operator_delete__(parts);
    }
    F<void *>(self, 0x10) = 0;
}

static ALWAYS_INLINE Wanted *wanted_for_image(int imageId)
{
    void *rootHolder = g_Radio_wantedRoot;
    void *outer = F<void *>(rootHolder, 0x00);
    Array<Wanted *> *wanted = F<Array<Wanted *> *>(outer, 0x00);
    return wanted->data[imageId];
}

void Radio::update(long time, PlayerEgo *ego, LevelScript *script)
{
    Array<RadioMessage *> *messages = F<Array<RadioMessage *> *>(this, 0x00);
    if (messages != 0) {
        for (uint32_t i = 0; i < messages->length; ++i) {
            if (messages->data[i]->triggered((int64_t)time, ego, script) != 0) {
                int imageId = radio_message_at(this, i)->getImageID();
                int *parts;
                int agentIndex;
                bool generated;

                if (imageId >= 10000) {
                    release_parts(this);
                    parts = (int *)operator_new__(0x14);
                    F<int *>(this, 0x10) = parts;
                    int wantedIndex = imageId - 10000;
                    for (int j = 0; j != 5; ++j) {
                        int *source = wanted_for_image(wantedIndex)->getImageParts();
                        parts = F<int *>(this, 0x10);
                        parts[j] = source[j];
                    }
                    agentIndex = 0;
                    generated = true;
                } else {
                    if (imageId < 0x3f && imageId != 0x15) {
                        release_parts(this);
                        int *source = g_Radio_imagePartTable[imageId];
                        parts = (int *)operator_new__(0x14);
                        F<int *>(this, 0x10) = parts;
                        for (int j = 0; j != 5; ++j) {
                            parts[j] = source[j];
                        }
                        generated = parts[0] != 10;
                        agentIndex = 0;
                        if (generated) {
                            agentIndex = parts[0];
                        }
                        if (imageId == 9) {
                            agentIndex = 8;
                        }
                    } else {
                        if (imageId == 0x40) {
                            agentIndex = 0;
                        } else if (imageId == 0x41) {
                            agentIndex = 2;
                        } else {
                            agentIndex = 1;
                            if (imageId == 0x15) {
                                agentIndex = 3;
                            }
                        }
                        release_parts(this);
                        generated = true;
                        parts = (*g_Radio_imageFactoryCreate)->createChar(1, agentIndex);
                        F<int *>(this, 0x10) = parts;
                    }
                }

                F<Array<ImagePart *> *>(this, 0x0c) = (*g_Radio_imageFactoryLoad)->loadChar(parts);
                release_string_lines(this);

                void *lines = operator new(0x0c);
                Array_String_ctor(lines);
                F<void *>(this, 0x08) = lines;

                GameText *gameText = *g_Radio_gameText;
                int textId = radio_message_at(this, i)->getTextID();
                String text(*gameText->getText(textId), false);

                String **fontHolder = g_Radio_fontWide;
                if (imageId != 0x38) {
                    fontHolder = g_Radio_fontNormal;
                }
                if (imageId == 0x13) {
                    fontHolder = g_Radio_fontWide;
                }
                String *font = *fontHolder;
                F<String *>(this, 0x34) = font;

                Layout *layout = *g_Radio_layoutForText;
                Globals *globals = *g_Radio_globals;
                globals->getLineArray(font, &text,
                                      (F<int>(this, 0x38) - 10) - F<int>(layout, 0x2d4),
                                      F<Array<String *> *>(this, 0x08));

                F<int64_t>(this, 0x18) = (int64_t)time;
                F<uint8_t>(this, 0x2d) = 1;
                F<int>(this, 0x28) = F<int>(F<void *>(this, 0x08), 0x00) * 2000 + 1500;

                Agent *agent = (Agent *)operator new(0x98);
                {
                    String agentName(g_Radio_agentName, false);
                    new (agent) Agent(0, &agentName, 0, 0, agentIndex, generated, 0, 0, 0, 0);
                }

                int soundTextId = radio_message_at(this, i)->getTextID();
                F<int>(this, 0x30) = globals->getDialogueSoundId(soundTextId, agent);
                agent->~Agent();
                operator_delete(agent);
                break;
            }
            messages = F<Array<RadioMessage *> *>(this, 0x00);
        }
    }
}

// ---- Radio_154f7c.cpp ----
struct RadioBlock16 {
    uint64_t a;
    uint64_t b;
};

struct RadioPair {
    uint32_t a;
    uint32_t b;
};


Radio::Radio()
{
    void **layoutHolder = g_Radio_layout;

    F<RadioPair>(this, 0x18) = RadioPair{0, 0};
    F<uint32_t>(this, 0x28) = 0;
    F<uint16_t>(this, 0x2c) = 0;
    F<uint32_t>(this, 0x10) = 0;
    F<int>(this, 0x30) = -1;
    F<RadioBlock16>(this, 0x00) = RadioBlock16{0, 0};

    void *layout = *layoutHolder;
    int x = F<int>(layout, 0x98);
    int screenWidth = **g_Radio_screenWidth;
    F<int>(this, 0x38) = x;
    F<int>(this, 0x3c) = (screenWidth - x) >> 1;
    F<int>(this, 0x40) = F<int>(layout, 0x9c);
}

// ---- draw_155378.cpp ----
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
