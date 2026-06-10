#include "gof2/Radio.h"
#include "gof2/Layout.h"

// Layout::setDrawColor is a Layout method (Layout not in this batch);
// call its mangled symbol directly via a thin wrapper.
extern "C" void _ZN13Layout12setDrawColorEi(Layout *self, int color);
static inline void Layout::setDrawColor(int color) {
    Layout *self = this; _ZN13Layout12setDrawColorEi(self, color); }

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
    return this->field_0x4 != 0;
}

// ---- lastMessageShown_15505a.cpp ----
uint8_t Radio::lastMessageShown()
{
    return this->field_0x2c;
}

// ---- _Radio_154fd4.cpp ----
Radio::~Radio()
{
    void *array = this->field_0xc;
    if (array != 0) {
        ArrayReleaseClasses_ImagePart(array);
        array = this->field_0xc;
        if (array != 0) {
            operator_delete(Array_ImagePart_dtor(array));
        }
    }
    this->field_0xc = 0;

    void *parts = this->field_0x10;
    if (parts != 0) {
        operator_delete__(parts);
    }
    this->field_0x10 = 0;

    array = this->field_0x8;
    if (array != 0) {
        ArrayReleaseClasses_String(array);
        array = this->field_0x8;
        if (array != 0) {
            operator_delete(Array_String_dtor(array));
        }
    }
    this->field_0x8 = 0;
}

// ---- getMessage_155050.cpp ----
RadioMessage *Radio::getMessage(int index)
{
    void *messages = this->field_0x0;
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
        for (uint32_t i = 0; i < list->size(); ++i) {
            list->data()[i]->setRadio(self);
        }
    }
}

// ---- update_155060.cpp ----
#define ALWAYS_INLINE __attribute__((always_inline)) inline

static ALWAYS_INLINE RadioMessage *radio_message_at(Radio *self, uint32_t index)
{
    void *messages = self->field_0x0;
    void **data = F<void **>(messages, 0x04);
    return (RadioMessage *)data[index];
}

static ALWAYS_INLINE void release_string_lines(Radio *self)
{
    void *lines = self->field_0x8;
    if (lines != 0) {
        ArrayReleaseClasses_String(lines);
        lines = self->field_0x8;
        if (lines != 0) {
            operator_delete(Array_String_dtor(lines));
        }
    }
    self->field_0x8 = 0;
}

static ALWAYS_INLINE void release_parts(Radio *self)
{
    void *parts = self->field_0x10;
    if (parts != 0) {
        operator_delete__(parts);
    }
    self->field_0x10 = 0;
}

static ALWAYS_INLINE Wanted *wanted_for_image(int imageId)
{
    void *rootHolder = g_Radio_wantedRoot;
    void *outer = F<void *>(rootHolder, 0x00);
    Array<Wanted *> *wanted = F<Array<Wanted *> *>(outer, 0x00);
    return wanted->data()[imageId];
}

void Radio::update(long time, PlayerEgo *ego, LevelScript *script)
{
    Array<RadioMessage *> *messages = F<Array<RadioMessage *> *>(this, 0x00);
    if (messages != 0) {
        for (uint32_t i = 0; i < messages->size(); ++i) {
            if (messages->data()[i]->triggered((int64_t)time, ego, script) != 0) {
                int imageId = radio_message_at(this, i)->getImageID();
                int *parts;
                int agentIndex;
                bool generated;

                if (imageId >= 10000) {
                    release_parts(this);
                    parts = (int *)operator_new__(0x14);
                    this->field_0x10 = parts;
                    int wantedIndex = imageId - 10000;
                    for (int j = 0; j != 5; ++j) {
                        int *source = wanted_for_image(wantedIndex)->getImageParts();
                        parts = (int *)this->field_0x10;
                        parts[j] = source[j];
                    }
                    agentIndex = 0;
                    generated = true;
                } else {
                    if (imageId < 0x3f && imageId != 0x15) {
                        release_parts(this);
                        int *source = g_Radio_imagePartTable[imageId];
                        parts = (int *)operator_new__(0x14);
                        this->field_0x10 = parts;
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
                        this->field_0x10 = parts;
                    }
                }

                F<Array<ImagePart *> *>(this, 0x0c) = (*g_Radio_imageFactoryLoad)->loadChar(parts);
                release_string_lines(this);

                void *lines = operator new(0x0c);
                Array_String_ctor(lines);
                this->field_0x8 = lines;

                GameText *gameText = *g_Radio_gameText;
                int textId = radio_message_at(this, i)->getTextID();
                String text = RadioStringCopy(*gameText->getText(textId));

                String **fontHolder = g_Radio_fontWide;
                if (imageId != 0x38) {
                    fontHolder = g_Radio_fontNormal;
                }
                if (imageId == 0x13) {
                    fontHolder = g_Radio_fontWide;
                }
                String *font = *fontHolder;
                this->field_0x34 = font;

                Layout *layout = *g_Radio_layoutForText;
                Globals *globals = *g_Radio_globals;
                globals->getLineArray(font, &text,
                                      (this->field_0x38 - 10) - layout->field_0x2d4,
                                      F<Array<String *> *>(this, 0x08));

                this->field_0x18 = (int64_t)time;
                this->field_0x2d = 1;
                this->field_0x28 = F<int>(this->field_0x8, 0x00) * 2000 + 1500;

                Agent *agent = (Agent *)operator new(0x98);
                {
                    String agentName = RadioStringFromCstr(g_Radio_agentName);
                    new (agent) Agent(0, &agentName, 0, 0, agentIndex, generated, 0, 0, 0, 0);
                }

                int soundTextId = radio_message_at(this, i)->getTextID();
                this->field_0x30 = globals->getDialogueSoundId(soundTextId, agent);
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

    this->field_0x18 = 0;
    this->field_0x28 = 0;
    this->field_0x2c = 0;
    this->field_0x10 = 0;
    this->field_0x30 = -1;
    this->field_0x0 = 0;

    Layout *layout = (Layout *)*layoutHolder;
    int x = layout->field_0x98;
    int screenWidth = **g_Radio_screenWidth;
    this->field_0x38 = x;
    this->field_0x3c = (screenWidth - x) >> 1;
    this->field_0x40 = layout->field_0x9c;
}

// ---- draw_155378.cpp ----
#define ALWAYS_INLINE __attribute__((always_inline)) inline

static ALWAYS_INLINE Wanted *draw_wanted_for_image(int imageId)
{
    void *rootHolder = g_Radio_drawWantedRoot;
    void *outer = F<void *>(rootHolder, 0x00);
    Array<Wanted *> *wanted = F<Array<Wanted *> *>(outer, 0x00);
    return wanted->data()[imageId - 10000];
}

void Radio::draw(int64_t time, PlayerEgo *ego, LevelScript *script)
{
    (void)ego;
    (void)script;

    if (this->field_0x4 == 0) {
        return;
    }

    if (this->field_0x18 + 2000 < time) {
        if (this->field_0x2d != 0 && this->field_0x30 >= 0) {
            (*g_Radio_drawSound)->play(this->field_0x30, 0, 0, 0.0f);
            this->field_0x2d = 0;
        }

        (*g_Radio_drawCanvas)->SetColor(0xffffffffu);
        int imageId = this->field_0x4->getImageID();
        Layout *layout = *g_Radio_drawLayout;
        ((Layout *)(layout))->setDrawColor(-0xd1);

        int width = this->field_0x38;
        int x = this->field_0x3c;
        int y = this->field_0x40;
        uint32_t imageHeight = layout->field_0x4 * F<uint32_t>(this->field_0x8, 0x00);
        uint32_t minHeight = layout->field_0x2d8;
        if (minHeight > imageHeight) {
            imageHeight = minHeight;
        }
        int boxHeight = imageHeight + layout->field_0x8 + 10;

        if (imageId >= 10000) {
            String title = draw_wanted_for_image(imageId)->getName();
            ((Layout *)(layout))->drawBox(7, x, y, width, boxHeight, &title);
        } else {
            String title = RadioStringCopy(*(*g_Radio_drawGameText)->getText(imageId + 0x63d));
            ((Layout *)(layout))->drawBox(7, x, y, width, boxHeight, &title);
        }

        ((Layout *)(layout))->setDrawColor(-1);
        (*g_Radio_drawImageFactory)->drawChar(F<Array<ImagePart *> *>(this, 0x0c),
                                              this->field_0x3c + 5,
                                              layout->field_0x8 + this->field_0x40 + 5,
                                              false);
        (*g_Radio_drawGlobals)->drawLines(this->field_0x34,
                                          F<Array<String *> *>(this, 0x08),
                                          layout->field_0x2d4 + this->field_0x3c + 7,
                                          layout->field_0x8 + this->field_0x40 + 7);

        if (this->field_0x2d != 0) {
            this->field_0x2d = 0;
        }

        Array<RadioMessage *> *messages = F<Array<RadioMessage *> *>(this, 0x00);
        if (messages != 0 && messages->size() != 0) {
            int duration = this->field_0x28;
            if (this->field_0x18 + (int64_t)duration + 2000 < time) {
                if (this->field_0x4 == messages->data()[messages->size() - 1]) {
                    this->field_0x2c = 1;
                }
                this->field_0x18 = 0;
                this->field_0x4->finish();
                this->field_0x4 = 0;
            }
        }
    }
}
