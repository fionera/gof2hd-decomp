#include "class.h"

// Arabic codepoint table: 0x29 rows x 5 u32 columns (PC-relative base address).
extern unsigned gArabicTable[] __attribute__((visibility("hidden")));

// GameText::isNonArabicString(unsigned short const*, unsigned) -> 1 if no char is in the table.
extern "C" int GameText_isNonArabicString(const unsigned short *param_1, unsigned param_2)
{
    unsigned short i = 0;
    bool keep;
    do {
        if (param_2 <= i) return 1;
        keep = true;
        unsigned short ch = param_1[i];
        unsigned *row = gArabicTable;
        for (int r = 0; r != 0x29; row += 5, ++r) {
            for (int c = 0; c != 5; ++c)
                keep = keep & (row[c] != ch);
        }
        ++i;
    } while (keep);
    return 0;
}
