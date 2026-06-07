#include "class.h"

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
