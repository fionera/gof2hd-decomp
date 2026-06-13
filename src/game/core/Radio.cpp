#include "gof2/game/core/Radio.h"
// NOTE: gof2/Layout.h is intentionally NOT included here. Radio only needs the
// minimal byte-offset Layout view declared in Radio.h, which is not layout-
// compatible with the full Layout.h definition.

// Layout::setDrawColor / drawBox are Layout methods (Layout not in this batch);
// call their mangled symbols directly via thin wrappers.
extern "C" void _ZN13Layout12setDrawColorEi(Layout *self, int color);
void Layout::setDrawColor(int color) {
    Layout *self = this; _ZN13Layout12setDrawColorEi(self, color); }
void Layout::drawBox(int style, int x, int y, int width, int height, String *title) {
    ((Layout *)(this))->drawBox(style, x, y, width, height, title); }

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

bool Radio::isShowingMessage()
{
    return this->currentMessage != 0;
}

uint8_t Radio::lastMessageShown()
{
    return this->lastMessageShownFlag;
}

Radio::~Radio()
{
    if (this->imageParts != 0) {
        for (ImagePart *e : *this->imageParts) delete e;
        this->imageParts->clear();
        delete this->imageParts;
    }
    this->imageParts = 0;

    if (this->imagePartBuffer != 0) {
        ::operator delete[](this->imagePartBuffer);
    }
    this->imagePartBuffer = 0;

    if (this->textLines != 0) {
        for (String *e : *this->textLines) delete e;
        this->textLines->clear();
        delete this->textLines;
    }
    this->textLines = 0;
}

RadioMessage *Radio::getMessage(int index)
{
    return (*this->messages)[index];
}

void Radio::setMessages(Array<RadioMessage *> *messages)
{
    Radio *self = this;
    Array<RadioMessage *> *list = messages;
    bool hasMessages = list != 0;
    this->messages = messages;
    if (hasMessages) {
        for (uint32_t i = 0; i < list->size(); ++i) {
            (*list)[i]->setRadio(self);
        }
    }
}

// ---- addMessage (ModStation supernova cutscene helper, OnTouchEnd @0xe79e4) ----
// The engine's supernova cutscene builds its RadioMessages inline inside
// ModStation::OnTouchEnd: for each story text id it heap-allocates a 0x28-byte
// RadioMessage(textId, 1, kind, sound) and stores it into the radio's message
// Array (raw layout: +0x0 = size, +0x4 = data). This recovers that step as a
// proper Radio method that lazily creates the array, appends a fresh message,
// and links it back to this radio.
extern "C" RadioMessage *RadioMessage_ctor(RadioMessage *self, int textId,
                                           int param2, int param3, int sound);

void Radio::addMessage(int textId)
{
    // Lazily create the backing message Array<RadioMessage *> on first use.
    Array<RadioMessage *> *list = this->messages;
    if (list == 0) {
        list = new Array<RadioMessage *>();
        this->messages = list;
    }

    // Heap-allocate and construct the RadioMessage from the story text id. The
    // unrolled cutscene uses (textId, 1, 5, 0) for the lead-in line; subsequent
    // lines vary only in the kind/sound fields, which default to the lead-in's.
    RadioMessage *message = (RadioMessage *)::operator new(0x28);
    RadioMessage_ctor(message, textId, 1, 5, 0);

    list->push_back(message);
    message->setRadio(this);
}

#define ALWAYS_INLINE __attribute__((always_inline)) inline

static ALWAYS_INLINE RadioMessage *radio_message_at(Radio *self, uint32_t index)
{
    return (*self->messages)[index];
}

static ALWAYS_INLINE void release_string_lines(Radio *self)
{
    if (self->textLines != 0) {
        for (String *e : *self->textLines) delete e;
        self->textLines->clear();
        delete self->textLines;
    }
    self->textLines = 0;
}

static ALWAYS_INLINE void release_parts(Radio *self)
{
    if (self->imagePartBuffer != 0) {
        ::operator delete[](self->imagePartBuffer);
    }
    self->imagePartBuffer = 0;
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
    Array<RadioMessage *> *messages = this->messages;
    if (messages != 0) {
        for (uint32_t i = 0; i < messages->size(); ++i) {
            if ((*messages)[i]->triggered((int64_t)time, ego, script) != 0) {
                int imageId = radio_message_at(this, i)->getImageID();
                int *parts;
                int agentIndex;
                bool generated;

                if (imageId >= 10000) {
                    release_parts(this);
                    parts = (int *)::operator new[](0x14);
                    this->imagePartBuffer = parts;
                    int wantedIndex = imageId - 10000;
                    for (int j = 0; j != 5; ++j) {
                        int *source = wanted_for_image(wantedIndex)->getImageParts();
                        parts = (int *)this->imagePartBuffer;
                        parts[j] = source[j];
                    }
                    agentIndex = 0;
                    generated = true;
                } else {
                    if (imageId < 0x3f && imageId != 0x15) {
                        release_parts(this);
                        int *source = g_Radio_imagePartTable[imageId];
                        parts = (int *)::operator new[](0x14);
                        this->imagePartBuffer = parts;
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
                        this->imagePartBuffer = parts;
                    }
                }

                this->imageParts = (*g_Radio_imageFactoryLoad)->loadChar(parts);
                release_string_lines(this);

                this->textLines = new Array<String *>();

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
                this->font = font;

                Layout *layout = *g_Radio_layoutForText;
                Globals *globals = *g_Radio_globals;
                globals->getLineArray(font, &text,
                                      (this->boxWidth - 10) - layout->field_0x2d4,
                                      this->textLines);

                this->startTime = (int64_t)time;
                this->soundPending = 1;
                this->displayDuration = (int)this->textLines->size() * 2000 + 1500;

                Agent *agent = (Agent *)operator new(0x98);
                {
                    String agentName = RadioStringFromCstr(g_Radio_agentName);
                    new (agent) Agent(0, &agentName, 0, 0, agentIndex, generated, 0, 0, 0, 0);
                }

                int soundTextId = radio_message_at(this, i)->getTextID();
                this->soundId = globals->getDialogueSoundId(soundTextId, agent);
                agent->~Agent();
                ::operator delete(agent);
                break;
            }
            messages = this->messages;
        }
    }
}

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

    this->startTime = 0;
    this->displayDuration = 0;
    this->lastMessageShownFlag = 0;
    this->imagePartBuffer = 0;
    this->soundId = -1;
    this->messages = 0;

    Layout *layout = (Layout *)*layoutHolder;
    int x = layout->field_0x98;
    int screenWidth = **g_Radio_screenWidth;
    this->boxWidth = x;
    this->boxX = (screenWidth - x) >> 1;
    this->boxY = layout->field_0x9c;
}

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

    if (this->currentMessage == 0) {
        return;
    }

    if (this->startTime + 2000 < time) {
        if (this->soundPending != 0 && this->soundId >= 0) {
            (*g_Radio_drawSound)->play(this->soundId, 0, 0, 0.0f);
            this->soundPending = 0;
        }

        (*g_Radio_drawCanvas)->SetColor(0xffffffffu);
        int imageId = this->currentMessage->getImageID();
        Layout *layout = *g_Radio_drawLayout;
        ((Layout *)(layout))->setDrawColor(-0xd1);

        int width = this->boxWidth;
        int x = this->boxX;
        int y = this->boxY;
        uint32_t imageHeight = layout->field_0x4 * (uint32_t)this->textLines->size();
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
        (*g_Radio_drawImageFactory)->drawChar(this->imageParts,
                                              this->boxX + 5,
                                              layout->field_0x8 + this->boxY + 5,
                                              false);
        (*g_Radio_drawGlobals)->drawLines(this->font,
                                          this->textLines,
                                          layout->field_0x2d4 + this->boxX + 7,
                                          layout->field_0x8 + this->boxY + 7);

        if (this->soundPending != 0) {
            this->soundPending = 0;
        }

        Array<RadioMessage *> *messages = this->messages;
        if (messages != 0 && messages->size() != 0) {
            int duration = this->displayDuration;
            if (this->startTime + (int64_t)duration + 2000 < time) {
                if (this->currentMessage == (*messages)[messages->size() - 1]) {
                    this->lastMessageShownFlag = 1;
                }
                this->startTime = 0;
                this->currentMessage->finish();
                this->currentMessage = 0;
            }
        }
    }
}
