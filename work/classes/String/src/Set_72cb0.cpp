#include "class.h"

// Local 12-byte String storage used for temporaries on the stack.
struct SBuf { uint32_t a, b, c; };

extern "C" {
void operator_delete__(void *p);
// computeFloatString(float v, int base, int *outExp, int *outNeg) -> wide buffer.
uint16_t *String_computeFloatString(float v, int base, int *outExp, int *outNeg);
void String_Set_wchar(String *self, const uint16_t *s);
void String_Set_char(String *self, const char *s);
String *String_ctor_char(String *self, const char *s, bool rev);
void String_dtor(String *self);
// operator+(String const&, String const&) - the free-function concatenation helper.
void String_concat(String *out, String *a, String *b);
void String_addAssign_str(String *self, String *other);
void String_assign_op(String *self, String *src);
// SubString(uint, uint) -> String*  (out, this, start, end via r2/r3 in target)
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
}

// Static format fragments (PC-relative literals in the target).
static const char kZeroDot[] = "0.";
static const char kZero[]    = "0";
static const char kDot[]     = ".";
static const char kExp[]     = "E";

// AbyssEngine::String::Set(float) - format a float into this string.
extern "C" void String_Set_float(String *self, float v)
{
    int exp = 0;
    int neg = 0;
    uint16_t *digitsW = String_computeFloatString(v, 10, &exp, &neg);

    // Accumulator string built up on the stack; start empty.
    SBuf acc;
    String_Set_char((String *)&acc, "");

    if (exp < 1) {
        // Leading "0." plus |exp| zeros.
        String_Set_char((String *)&acc, kZeroDot);
        for (int i = 0; exp < i; i--) {
            SBuf tmp;
            String_ctor_char((String *)&tmp, kZero, false);
            String_addAssign_str((String *)&acc, (String *)&tmp);
            String_dtor((String *)&tmp);
        }
    }

    {
        SBuf tmp;
        String_ctor_char((String *)&tmp, (const char *)digitsW, false);
        String_addAssign_str((String *)&acc, (String *)&tmp);
        String_dtor((String *)&tmp);
    }

    if (exp > 0) {
        SBuf head, dot, left, tail, full;
        String_SubString((String *)&head, (String *)&acc, 0, (unsigned int)exp);
        String_ctor_char((String *)&dot, kDot, false);
        String_concat((String *)&left, (String *)&head, (String *)&dot);
        String_SubString((String *)&tail, (String *)&acc, (unsigned int)exp, 0);
        String_concat((String *)&full, (String *)&left, (String *)&tail);
        String_assign_op((String *)&acc, (String *)&full);
        String_dtor((String *)&full);
        String_dtor((String *)&tail);
        String_dtor((String *)&left);
        String_dtor((String *)&dot);
        String_dtor((String *)&head);
    }

    if (neg != 0) {
        SBuf sign, full;
        String_ctor_char((String *)&sign, kExp, false);
        String_concat((String *)&full, (String *)&sign, (String *)&acc);
        String_assign_op((String *)&acc, (String *)&full);
        String_dtor((String *)&full);
        String_dtor((String *)&sign);
    }

    operator_delete__(digitsW);
    String_Set_wchar(self, *(uint16_t **)((char *)&acc + 0x4));
    String_dtor((String *)&acc);
}
