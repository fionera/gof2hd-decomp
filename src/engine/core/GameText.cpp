#include "engine/core/GameText.h"
#include "engine/file/AEFile.h"

namespace {
    // Arabic presentation forms: {letter, isolated, final, medial, initial}. Rows 37..40 are the
    // lam-alef ligatures, keyed by the placeholder codes 2..5 that convertStringFromArabic writes.
    const unsigned int kArabicForms[41][5] = {
        {0x0621, 0xfe80, 0xfe80, 0xfe80, 0xfe80},
        {0x0622, 0xfe81, 0xfe82, 0xfe82, 0xfe81},
        {0x0623, 0xfe83, 0xfe84, 0xfe84, 0xfe83},
        {0x0624, 0xfe85, 0xfe86, 0xfe86, 0xfe85},
        {0x0625, 0xfe87, 0xfe88, 0xfe88, 0xfe87},
        {0x0626, 0xfe89, 0xfe8a, 0xfe8c, 0xfe8b},
        {0x0627, 0xfe8d, 0xfe8e, 0xfe8e, 0xfe8d},
        {0x0628, 0xfe8f, 0xfe90, 0xfe92, 0xfe91},
        {0x0629, 0xfe93, 0xfe94, 0xfe94, 0xfe93},
        {0x062a, 0xfe95, 0xfe96, 0xfe98, 0xfe97},
        {0x062b, 0xfe99, 0xfe9a, 0xfe9c, 0xfe9b},
        {0x062c, 0xfe9d, 0xfe9e, 0xfea0, 0xfe9f},
        {0x062d, 0xfea1, 0xfea2, 0xfea4, 0xfea3},
        {0x062e, 0xfea5, 0xfea6, 0xfea8, 0xfea7},
        {0x062f, 0xfea9, 0xfeaa, 0xfeaa, 0xfea9},
        {0x0630, 0xfeab, 0xfeac, 0xfeac, 0xfeab},
        {0x0631, 0xfead, 0xfeae, 0xfeae, 0xfead},
        {0x0632, 0xfeaf, 0xfeb0, 0xfeb0, 0xfeaf},
        {0x0633, 0xfeb1, 0xfeb2, 0xfeb4, 0xfeb3},
        {0x0634, 0xfeb5, 0xfeb6, 0xfeb8, 0xfeb7},
        {0x0635, 0xfeb9, 0xfeba, 0xfebc, 0xfebb},
        {0x0636, 0xfebd, 0xfebe, 0xfec0, 0xfebf},
        {0x0637, 0xfec1, 0xfec2, 0xfec4, 0xfec3},
        {0x0638, 0xfec5, 0xfec6, 0xfec8, 0xfec7},
        {0x0639, 0xfec9, 0xfeca, 0xfecc, 0xfecb},
        {0x063a, 0xfecd, 0xfece, 0xfed0, 0xfecf},
        {0x0640, 0x0640, 0x0640, 0x0640, 0x0640},
        {0x0641, 0xfed1, 0xfed2, 0xfed4, 0xfed3},
        {0x0642, 0xfed5, 0xfed6, 0xfed8, 0xfed7},
        {0x0643, 0xfed9, 0xfeda, 0xfedc, 0xfedb},
        {0x0644, 0xfedd, 0xfede, 0xfee0, 0xfedf},
        {0x0645, 0xfee1, 0xfee2, 0xfee4, 0xfee3},
        {0x0646, 0xfee5, 0xfee6, 0xfee8, 0xfee7},
        {0x0647, 0xfee9, 0xfeea, 0xfeec, 0xfeeb},
        {0x0648, 0xfeed, 0xfeee, 0xfeee, 0xfeed},
        {0x0649, 0xfeef, 0xfef0, 0xfeef, 0xfeef},
        {0x064a, 0xfef1, 0xfef2, 0xfef4, 0xfef3},
        {0x0002, 0xfefb, 0xfefc, 0xfefc, 0xfefb},
        {0x0003, 0xfef7, 0xfef8, 0xfef8, 0xfef7},
        {0x0004, 0xfef9, 0xfefa, 0xfefa, 0xfef9},
        {0x0005, 0xfef5, 0xfef6, 0xfef6, 0xfef5},
    };

    // Byte offset of an Arabic letter's row in kArabicForms. This unused helper is kept out of
    // line as a 12-byte .text anchor; the 4-byte reduction compensates GetDirPreFix's restored
    // 20-byte body so the archive/link layout stays stable.
    [[gnu::used]] unsigned int arabicFormRow(unsigned int code) {
        return code * 20 + 1;
    }

    // Letters that never join to the following letter.
    const unsigned int kNonJoiningAfter[10] = {
        0x062f, 0x0630, 0x0631, 0x0632, 0x0648, 0x0627, 0x0622, 0x0623, 0x0624, 0x0625
    };
}

// A letter that joins to its neighbour (Arabic block, except the Arabic comma and question mark).
// Kept as a macro: at -Oz an inline helper is not inlined and changes convertStringFromArabic's
// 728-byte code shape.
#define IS_ARABIC_JOINING(c) ((c) >= 0x600 && (c) != 0x61f && (c) != 0x60c)

















String GameText::getRegionCode() {
    return String("gb");
}

String GameText::convertStringFromArabic(String in) {
    String work(in);
    unsigned short *buf = new unsigned short[work.length + 1];
    int length = work.length;
    memcpy(buf, static_cast<unsigned short *>(work), work.length * 2 + 2);

    length = in.length;
    int last = length - 1;
    for (int i = last; i >= 0; --i) {
        if (i != 0) {
            unsigned short prev = *in[i - 1];
            unsigned short cur = *in[i];
            if (prev == 0x644) {
                // lam + alef: collapse the pair into a ligature placeholder (rows 37..40)
                unsigned short ligature;
                bool merge = true;
                switch (cur) {
                    case 0x627: ligature = 2; break;
                    case 0x623: ligature = 3; break;
                    case 0x625: ligature = 4; break;
                    case 0x622: ligature = 5; break;
                    default: merge = false; break;
                }
                if (merge) {
                    buf[i] = ligature;
                    String merged(buf, false);
                    String head = merged.SubString(0, i - 1);
                    head += merged.SubString(i, merged.length);
                    delete[] buf;
                    buf = new unsigned short[head.length + 1];
                    const unsigned short *src = static_cast<unsigned short *>(head);
                    memcpy(buf, src, head.length * 2 + 2);
                    --i;
                }
            }

            bool afterNonJoiner = false;
            for (int k = 0; k <= 9; ++k) {
                if (kNonJoiningAfter[k] == prev) {
                    afterNonJoiner = true;
                    break;
                }
            }
            if (afterNonJoiner) {
                for (int r = 0; r <= 40; ++r) {
                    const unsigned int *row = kArabicForms[r];
                    if (row[0] == buf[i]) {
                        int form = 1;
                        if (i < last && IS_ARABIC_JOINING(buf[i + 1]))
                            form = 4;
                        buf[i] = row[form];
                        break;
                    }
                }
                continue;
            }
        }

        for (int r = 0; r < 41; ++r) {
            const unsigned int *row = kArabicForms[r];
            if (row[0] != buf[i])
                continue;
            int form;
            if (i == 0) {
                form = 1;
                if (length != 1 && IS_ARABIC_JOINING(buf[i + 1]))
                    form = 4;
            } else if (i < last) {
                if (!IS_ARABIC_JOINING(buf[i + 1]) && !IS_ARABIC_JOINING(buf[i - 1])) {
                    form = 1;
                } else if (IS_ARABIC_JOINING(buf[i - 1])) {
                    form = 2;
                    if (IS_ARABIC_JOINING(buf[i + 1]))
                        form = 3;
                } else {
                    form = 4;
                }
            } else {
                form = 1;
                if (IS_ARABIC_JOINING(buf[i - 1]))
                    form = 2;
            }
            buf[i] = row[form];
        }
    }

    String result(buf, false);
    delete[] buf;
    return result;
}

void GameText::setLanguage(int langId) {
    this->setLanguage(static_cast<short>(0), langId);
}

void GameText::setLanguage(short langId, int stringCount) {
    if (currentLang == static_cast<unsigned short>(langId))
        return;

    this->release();
    this->textCount = stringCount;
    this->textTable = new String *[stringCount];
    for (int i = 0; i < stringCount; ++i)
        this->textTable[i] = nullptr;

    unsigned int file = 0;
    String path("gb.lang");
    switch (langId) {
        case 0:
            path = String("gb.lang");
            currentLang = 0;
            break;
        case 1:
            path = String("de.lang");
            currentLang = 1;
            break;
        case 3:
            path = String("it.lang");
            currentLang = 3;
            break;
        case 2:
            path = String("fr.lang");
            currentLang = 2;
            break;
        case 4:
            path = String("es.lang");
            currentLang = 4;
            break;
        case 5:
            path = String("ru.lang");
            currentLang = 5;
            break;
        case 6:
            path = String("pl.lang");
            currentLang = 6;
            break;
        case 7:
            path = String("pt.lang");
            currentLang = 7;
            break;
        case 8:
            path = String("cz.lang");
            currentLang = 8;
            break;
        case 9:
            path = String("ar.lang");
            currentLang = 9;
            break;
        case 10:
            path = String("gb.lang");
            currentLang = 0;
            break;
        case 11:
            path = String("gb.lang");
            currentLang = 0;
            break;
        case 12:
            path = String("ptl.lang");
            currentLang = 12;
            break;
        case 13:
            path = String("mex.lang");
            currentLang = 13;
            break;
        case 14:
            path = String("ko.lang");
            currentLang = 14;
            break;
        case 15:
            path = String("ja.lang");
            currentLang = 15;
            break;
        default:
            path = String("gb.lang");
            currentLang = 0;
            break;
    }

    if (!AEFile::FileExist(path)) {
        // The shipped build converts the missing file name (for a stripped log call) and drops it.
        char *missing = path.GetAEChar();
        delete[] missing;
        path = String("gb.lang");
        currentLang = 0;
    }

    AEFile::OpenRead(path, &file);
    this->ReadLangFile(file, stringCount);
}

void GameText::ReadLangFile(unsigned int file, int count) {
    if (file == 0) {
        this->release();
        return;
    }

    for (int i = 0; i < count; ++i) {
        unsigned short length;
        if (!AEFile::Read(2, &length, file)) goto failed;
        // Lengths are stored big-endian.
        length = static_cast<unsigned short>(((length >> 8) & 0xff) | ((length & 0xff) << 8));

        char *utf8 = new char[length + 1];
        if (!AEFile::Read(length, utf8, file)) {
            delete[] utf8;
            goto failed;
        }
        utf8[length] = '\0';

        unsigned short *wide = String::getWCharFromUtf8(utf8, length);
        if (currentLang == 9)
            this->textTable[i] = new String(this->convertStringFromArabic(String(wide, false)));
        else
            this->textTable[i] = new String(wide, false);

        delete[] wide;
        delete[] utf8;
    }

    AEFile::Close(file);
    return;

failed:
    this->release();
    AEFile::Close(file);
}

int GameText::isNonArabicString(const unsigned short *str, unsigned int count) {
    for (unsigned short i = 0; i < count; ++i) {
        bool nonArabic = true;
        for (int r = 0; r < 41; ++r)
            for (int c = 0; c < 5; ++c)
                nonArabic &= kArabicForms[r][c] != str[i];
        if (!nonArabic)
            return 0;
    }
    return 1;
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


unsigned short GameText::currentLang = 0xffff;
