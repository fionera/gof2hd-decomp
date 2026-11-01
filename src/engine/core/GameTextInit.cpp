#include "engine/core/GameText.h"

#include <cstdlib>
#include <cstring>
#include "engine/file/AEFile.h"

namespace {
}


GameText::GameText() {
    GameText::currentLang = 0xffff;
    this->textCount = 0;
    this->textTable = nullptr;
    this->fallbackText = String("Error Language");
}

GameText::~GameText() {
    this->release();
    delete[] this->textTable;
    this->textTable = nullptr;
}

void GameText::release() {
    if (this->textTable == nullptr)
        return;
    for (int i = 0; i < this->textCount; ++i) {
        delete this->textTable[i];
        this->textTable[i] = nullptr;
    }
}

short GameText::getLanguage() {
    return static_cast<short>(currentLang);
}

void GameText::setSubstituteArray(int *pairs, unsigned count) {
    if (count != 0) {
        if ((count & 1) != 0) return;
        ArrayRemoveAll<int>(this->substitutes);
    }
    for (; count != 0; --count) {
        int v = *pairs++;
        ArrayAdd(v, this->substitutes);
    }
}

AbyssEngine::String *GameText::getText(int key) {
    if (key == 5000) {
        static String privacyPolicy(currentLang == 1 ? "Datenschutzb." : "Privacy Policy", false);
        return &privacyPolicy;
    }
    if (key == 5001) {
        static String termsOfService(currentLang == 1 ? "Nutzungsb." : "Terms of S.", false);
        return &termsOfService;
    }

    for (unsigned int i = 0; i < this->substitutes.size(); i += 2) {
        if (this->substitutes[i] == key) {
            key = this->substitutes[i + 1];
            break;
        }
    }

    if (key >= 0 && key < this->textCount && this->textTable != nullptr &&
        this->textTable[key] != nullptr)
        return this->textTable[key];
    return &this->fallbackText;
}
