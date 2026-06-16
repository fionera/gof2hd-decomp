#include "game/core/Radio.h"
#include "game/core/RadioMessage.h"
#include "game/world/Wanted.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/ImageFactory.h"
#include "engine/render/ImagePart.h"
#include "engine/audio/FModSound.h"
#include "game/ui/Layout.h"

// ---------------------------------------------------------------------------
// Cross-class engine entry points used through their own (not-yet-self-clean)
// headers. They are declared locally so this translation unit stays
// self-sufficient and strictly compilable while those headers are migrated.
// ---------------------------------------------------------------------------

// GameText::getText(int) — localized text table lookup. GameText.h currently
// double-defines String and cannot be co-included with common.h.
class GameText;
extern "C" String* _ZN8GameText7getTextEi(GameText* self, int key)
    asm("_ZN8GameText7getTextEi");

// Globals text/line helpers. Globals.h does not model these draw-time methods;
// the engine exposes them as free functions (see src/game/core/Globals.cpp).
void Globals_getLineArray(void* globals, String* font, String* text, int maxWidth,
                          Array<String*>* out);
void Globals_drawLines(void* globals, String* font, Array<String*>* lines,
                       int x, int y);
int Globals_getDialogueSoundId(void* globals, int textId, void* agent);

// Agent is constructed transiently to resolve the speaker's voice sample.
// Agent.h is not yet self-clean (missing forward decls), so the object is held
// opaquely and built/destroyed through the engine's real ctor/dtor symbols.
extern "C" void _ZN5AgentC1EjPN11AbyssEngine6StringEiiibiiii(
    void* self, unsigned kind, String* name, int p4, int p5, int p6,
    bool p7, int p8, int p9, int p10, int p11);
extern "C" void _ZN5AgentD1Ev(void* self);

// ---------------------------------------------------------------------------
// Engine globals (resolved at link time; left as-is per the port conventions).
// ---------------------------------------------------------------------------
extern "C" __attribute__((visibility("hidden"))) Array<Wanted*>** g_Radio_wantedRoot;
extern "C" __attribute__((visibility("hidden"))) ImageFactory** g_Radio_imageFactoryCreate;
extern "C" __attribute__((visibility("hidden"))) ImageFactory** g_Radio_imageFactoryLoad;
extern "C" __attribute__((visibility("hidden"))) int* g_Radio_imagePartTable[];
extern "C" __attribute__((visibility("hidden"))) GameText** g_Radio_gameText;
extern "C" __attribute__((visibility("hidden"))) String** g_Radio_fontNormal;
extern "C" __attribute__((visibility("hidden"))) String** g_Radio_fontWide;
extern "C" __attribute__((visibility("hidden"))) Layout** g_Radio_layoutForText;
extern "C" __attribute__((visibility("hidden"))) void** g_Radio_globals;
extern "C" __attribute__((visibility("hidden"))) char g_Radio_agentName[];
extern "C" __attribute__((visibility("hidden"))) Layout** g_Radio_layout;
extern "C" __attribute__((visibility("hidden"))) int** g_Radio_screenWidth;
extern "C" __attribute__((visibility("hidden"))) FModSound** g_Radio_drawSound;
extern "C" __attribute__((visibility("hidden"))) Layout** g_Radio_drawLayout;
extern "C" __attribute__((visibility("hidden"))) Array<Wanted*>** g_Radio_drawWantedRoot;
extern "C" __attribute__((visibility("hidden"))) GameText** g_Radio_drawGameText;
extern "C" __attribute__((visibility("hidden"))) ImageFactory** g_Radio_drawImageFactory;
extern "C" __attribute__((visibility("hidden"))) void** g_Radio_drawGlobals;

// Build a String from the engine's NUL-terminated agent-name byte buffer.
static String radio_string_from_cstr(const char* c)
{
    String r;
    for (const char* p = c; p && *p; ++p)
        r.s.push_back((char16_t)(unsigned char)*p);
    return r;
}

Radio::Radio()
{
    this->startTime = 0;
    this->displayDuration = 0;
    this->lastMessageShownFlag = 0;
    this->imagePartBuffer = 0;
    this->soundId = -1;
    this->messages = 0;

    Layout* layout = *g_Radio_layout;
    int width = layout->field_0x98;
    int screenWidth = **g_Radio_screenWidth;
    this->boxWidth = width;
    this->boxX = (screenWidth - width) >> 1;
    this->boxY = layout->field_0x9c;
}

Radio::~Radio()
{
    if (this->imageParts != 0) {
        for (ImagePart* e : *this->imageParts)
            delete e;
        this->imageParts->clear();
        delete this->imageParts;
    }
    this->imageParts = 0;

    delete[] this->imagePartBuffer;
    this->imagePartBuffer = 0;

    if (this->textLines != 0) {
        for (String* e : *this->textLines)
            delete e;
        this->textLines->clear();
        delete this->textLines;
    }
    this->textLines = 0;
}

bool Radio::isShowingMessage()
{
    return this->currentMessage != 0;
}

uint8_t Radio::lastMessageShown()
{
    return this->lastMessageShownFlag;
}

RadioMessage* Radio::getMessage(int index)
{
    return (*this->messages)[index];
}

void Radio::setMessages(Array<RadioMessage*>* messages)
{
    this->messages = messages;
    if (messages != 0) {
        for (uint32_t i = 0; i < messages->size(); ++i)
            (*messages)[i]->radio = this;
    }
}

void Radio::addMessage(int textId)
{
    // Lazily create the backing message list on first use, append a fresh
    // RadioMessage built from the story text id and link it back to this radio.
    if (this->messages == 0)
        this->messages = new Array<RadioMessage*>();

    RadioMessage* message = new RadioMessage(textId, 1, 5, 0);
    this->messages->push_back(message);
    message->radio = this;
}

void Radio::update(long time, PlayerEgo* ego, LevelScript* script)
{
    Array<RadioMessage*>* messages = this->messages;
    if (messages == 0)
        return;

    for (uint32_t i = 0; i < messages->size(); ++i) {
        RadioMessage* message = (*messages)[i];
        if (message->triggered((int64_t)time, ego, script) == 0) {
            messages = this->messages;
            continue;
        }

        int imageId = message->imageID;
        int* parts;
        int agentIndex;
        bool generated;

        if (imageId >= 10000) {
            delete[] this->imagePartBuffer;
            this->imagePartBuffer = new int[5];
            int wantedIndex = imageId - 10000;
            Wanted* wanted = (**g_Radio_wantedRoot)[wantedIndex];
            int* source = wanted->getImageParts();
            for (int j = 0; j != 5; ++j)
                this->imagePartBuffer[j] = source[j];
            parts = this->imagePartBuffer;
            agentIndex = 0;
            generated = true;
        } else if (imageId < 0x3f && imageId != 0x15) {
            delete[] this->imagePartBuffer;
            int* source = g_Radio_imagePartTable[imageId];
            this->imagePartBuffer = new int[5];
            for (int j = 0; j != 5; ++j)
                this->imagePartBuffer[j] = source[j];
            parts = this->imagePartBuffer;
            generated = parts[0] != 10;
            agentIndex = generated ? parts[0] : 0;
            if (imageId == 9)
                agentIndex = 8;
        } else {
            if (imageId == 0x40)
                agentIndex = 0;
            else if (imageId == 0x41)
                agentIndex = 2;
            else
                agentIndex = (imageId == 0x15) ? 3 : 1;
            delete[] this->imagePartBuffer;
            generated = true;
            parts = (*g_Radio_imageFactoryCreate)->createChar(1, agentIndex);
            this->imagePartBuffer = parts;
        }

        this->imageParts = (*g_Radio_imageFactoryLoad)->loadChar(parts);

        if (this->textLines != 0) {
            for (String* e : *this->textLines)
                delete e;
            this->textLines->clear();
            delete this->textLines;
        }
        this->textLines = new Array<String*>();

        int textId = message->textID;
        String text = *_ZN8GameText7getTextEi(*g_Radio_gameText, textId);

        String** fontHolder = g_Radio_fontWide;
        if (imageId != 0x38)
            fontHolder = g_Radio_fontNormal;
        if (imageId == 0x13)
            fontHolder = g_Radio_fontWide;
        this->font = *fontHolder;

        Layout* layout = *g_Radio_layoutForText;
        Globals_getLineArray(*g_Radio_globals, this->font, &text,
                             (this->boxWidth - 10) - layout->field_0x2d4,
                             this->textLines);

        this->startTime = (int64_t)time;
        this->soundPending = 1;
        this->displayDuration = (int)this->textLines->size() * 2000 + 1500;

        void* agent = ::operator new(0x98);
        String agentName = radio_string_from_cstr(g_Radio_agentName);
        _ZN5AgentC1EjPN11AbyssEngine6StringEiiibiiii(
            agent, 0, &agentName, 0, 0, agentIndex, generated, 0, 0, 0, 0);
        this->soundId = Globals_getDialogueSoundId(*g_Radio_globals,
                                                   message->textID, agent);
        _ZN5AgentD1Ev(agent);
        ::operator delete(agent);
        break;
    }
}

void Radio::draw(int64_t time, PlayerEgo* ego, LevelScript* script)
{
    (void)ego;
    (void)script;

    if (this->currentMessage == 0)
        return;
    if (this->startTime + 2000 >= time)
        return;

    if (this->soundPending != 0 && this->soundId >= 0) {
        (*g_Radio_drawSound)->play(this->soundId, 0, 0, 0.0f);
        this->soundPending = 0;
    }

    gCanvas->SetColor(0xffffffffu);
    int imageId = this->currentMessage->imageID;
    Layout* layout = *g_Radio_drawLayout;
    layout->setDrawColor(-0xd1);

    int width = this->boxWidth;
    int x = this->boxX;
    int y = this->boxY;
    uint32_t imageHeight = layout->field_0x4 * (uint32_t)this->textLines->size();
    uint32_t minHeight = layout->field_0x2d8;
    if (minHeight > imageHeight)
        imageHeight = minHeight;
    int boxHeight = imageHeight + layout->field_0x8 + 10;

    if (imageId >= 10000) {
        Wanted* wanted = (**g_Radio_drawWantedRoot)[imageId - 10000];
        String title = wanted->getName();
        layout->drawBoxStr(7, x, y, width, boxHeight, &title);
    } else {
        String title = *_ZN8GameText7getTextEi(*g_Radio_drawGameText, imageId + 0x63d);
        layout->drawBoxStr(7, x, y, width, boxHeight, &title);
    }

    layout->setDrawColor(-1);
    (*g_Radio_drawImageFactory)->drawChar(this->imageParts,
                                          this->boxX + 5,
                                          layout->field_0x8 + this->boxY + 5,
                                          false);
    Globals_drawLines(*g_Radio_drawGlobals, this->font, this->textLines,
                      layout->field_0x2d4 + this->boxX + 7,
                      layout->field_0x8 + this->boxY + 7);

    if (this->soundPending != 0)
        this->soundPending = 0;

    Array<RadioMessage*>* messages = this->messages;
    if (messages != 0 && messages->size() != 0) {
        if (this->startTime + (int64_t)this->displayDuration + 2000 < time) {
            if (this->currentMessage == (*messages)[messages->size() - 1])
                this->lastMessageShownFlag = 1;
            this->startTime = 0;
            this->currentMessage->finish();
            this->currentMessage = 0;
        }
    }
}
