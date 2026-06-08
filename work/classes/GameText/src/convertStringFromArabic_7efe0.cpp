#include "class.h"

// GameText::convertStringFromArabic(String in) -> String (sret)
// Reshapes a logical-order Arabic string into the presentation forms the bitmap font expects.
// Walks the code points from the end toward the start; for each Arabic letter it picks the
// isolated / initial / medial / final glyph based on the joining behaviour of its neighbours,
// and collapses the LAM (U+0644) + ALEF family into the appropriate ligature, splicing the
// string around the merged pair. Substitution data lives in two extern tables:
//   gArabForms : rows of 5 u32 {base, isolated, final, initial, medial}, terminated implicitly
//                by the 0x334-byte span (41 rows).
//   gLamAlef   : rows of 5 u32 {base, ...} for the 10 LAM-ALEF combinations.
//
// out is sret; `pad` is the hidden alignment argument from the {String} by-value ABI.

extern "C" void  AEString_ctor_wide(void *s, const unsigned short *text, bool copy);
extern "C" void  AEString_dtor(void *s);
extern "C" unsigned short *AEString_data(void *s);                 // operator cast to ushort*
extern "C" unsigned short *AEString_index(void *s, unsigned int i);// operator[](i)
extern "C" void  AEString_SubString(void *out, void *s, unsigned int a, unsigned int b);
extern "C" void  AEString_append(void *dst, void *src);           // operator+=

void *operator new[](uint32_t);
void operator delete[](void *) noexcept;
extern "C" void *memcpy(void *, const void *, unsigned long);

// Substitution tables (see header comment for row layout).
__attribute__((visibility("hidden"))) extern const unsigned int gArabForms[];  // 41 rows * 5
__attribute__((visibility("hidden"))) extern const unsigned int gLamAlef[];     // first-letter map (10)
__attribute__((visibility("hidden"))) extern const unsigned int gLamAlefForms[];// 0x29 rows * 5

// True for code points that participate in Arabic joining as a "letter".
static inline bool isJoiner(unsigned short c) {
    return c >= 0x600 && c != 0x60c && c != 0x61f;
}

extern "C" void GameText_convertStringFromArabic(void *out, int pad, void *in)
{
    (void)pad;

    // Working copy of the wide buffer (length + NUL).
    String work;
    AEString_ctor_wide(&work, AEString_data(in), false);
    unsigned int len = u32(&work, 8);          // character count

    unsigned int cap = (len + 1) * 2;
    unsigned short *buf = (unsigned short *)operator new[](cap);
    memcpy(buf, AEString_data(&work), (unsigned long)(len * 2 + 2));

    int inLen = i32(in, 8);
    unsigned int hi = (unsigned int)(inLen - 1);
    unsigned int i = hi;

    while (true) {
        if (i > 0x7fffffff) {
            // Underflowed past 0: finished. Emit the reshaped buffer.
            AEString_ctor_wide(out, buf, false);
            if (buf != 0)
                operator delete[](buf);
            AEString_dtor(&work);
            return;
        }

        if (i != 0) {
            unsigned short prev = *AEString_index(in, i - 1);
            unsigned short cur  = *AEString_index(in, i);
            if (prev == 0x644) {
                // LAM + ALEF-family ligature.
                unsigned short form = 2;
                switch (cur) {
                case 0x622: form = 5; break;
                case 0x623: form = 3; break;
                case 0x627: form = 2; break;
                default: break;
                }
                buf[i] = form;

                // Splice: keep [0,i-1) and [i+1,end), join via SubString + operator+=.
                String merged;
                AEString_ctor_wide(&merged, buf, false);
                String head, tail;
                AEString_SubString(&head, &merged, 0, i - 1);
                AEString_SubString(&tail, &merged, i + 1, len);
                AEString_append(&head, &tail);
                AEString_dtor(&tail);

                if (buf != 0)
                    operator delete[](buf);

                unsigned int mLen = u32(&head, 8);
                unsigned int mCap = (mLen + 1) * 2;
                buf = (unsigned short *)operator new[](mCap);
                memcpy(buf, AEString_data(&head), (unsigned long)(mLen * 2 + 2));
                AEString_dtor(&head);
                AEString_dtor(&merged);
                --i;
            }

            // Map the LAM-ALEF result through the secondary form table.
            unsigned int k = 0;
            for (; k < 10; ++k)
                if (gLamAlef[k * 4] == (unsigned int)prev)
                    break;
            if (k < 10) {
                for (unsigned int r = 0; r < 0x29; ++r) {
                    const unsigned int *row = &gLamAlefForms[r * 5];
                    if (row[0] == (unsigned int)buf[i]) {
                        int form = 1;
                        if ((int)i < (int)hi && isJoiner(buf[i + 1]))
                            form = isJoiner(buf[i + 1]) ? 4 : 1;
                        buf[i] = (unsigned short)row[form];
                        break;
                    }
                }
            }
        } else {
            // i == 0: standalone first letter contextual form.
            for (unsigned int r = 0; r < 0x334 / 20; ++r) {
                const unsigned int *row = &gArabForms[r * 5];
                if (row[0] == (unsigned int)buf[0]) {
                    int form;
                    if (inLen == 1 || buf[1] < 0x600) {
                        form = 1;
                    } else {
                        unsigned short nxt = buf[1];
                        form = (nxt == 0x60c || nxt == 0x61f) ? 1 : 4;
                    }
                    buf[0] = (unsigned short)row[form];
                    break;
                }
            }
        }

        --i;
    }
}
