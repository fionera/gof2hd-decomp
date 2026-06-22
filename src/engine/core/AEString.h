#ifndef GOF2_AESTRING_H
#define GOF2_AESTRING_H

#include <cstdint>

#define AESTRING_SHIM inline __attribute__((always_inline))

namespace AbyssEngine {
    class String {
    public:
        unsigned short *data;
        int length;

        String();

        String(const char *cstr, bool reverse = false);

        AESTRING_SHIM String(const String &other) : data(nullptr), length(0) { Set(other.data); }

        String(const uint16_t *wstr, bool reverse);

        String(const String &other, bool reverse);

        explicit String(char c);

        explicit String(int v);

        explicit String(float v);

        explicit String(long long v);

        virtual ~String();

        String &operator=(const String &other);

        String &operator=(const char *cstr) {
            Set(cstr);
            return *this;
        }

        String &operator=(const char16_t *wstr) {
            Set(reinterpret_cast<const unsigned short *>(wstr));
            return *this;
        }

        AESTRING_SHIM void push_back(unsigned short c) { appendWide(&c, 1); }

        String &operator+=(const String &other) {
            appendWide(other.data, other.length);
            return *this;
        }

        String &operator+=(const char &c);

        String &operator+=(const int &v);

        String &operator+=(const float &v);

        String &operator+=(const long long &v);

        void copy(const String *src, bool reverse) { ctor_copy(const_cast<String *>(src), reverse); }
        const char16_t *text() const { return reinterpret_cast<const char16_t *>(data); }
        uint32_t size() const { return (uint32_t) length; }

        void Set(const char *s);

        void Set(const unsigned short *s);

        void Set(float v);

        void Set(long long v);

        static int StrLen(const char *s);

        static int StrLen(const unsigned short *s);

        static int GetStringLength(const char *s);

        int Compare(const String &other);

        unsigned int Compare(const char *s);

        const unsigned short *GetAEWChar() const;

        operator unsigned short *();

        operator const unsigned short *() const;

        unsigned short *operator[](int i);

        const unsigned short *operator[](int i) const;

        AESTRING_SHIM unsigned int Compare_char(const char *s) { return Compare(s); }
        AESTRING_SHIM int Compare_str(String *other) { return Compare(*other); }

        void ConvertFromUTF8();

        char *GetAEChar() const;

        unsigned int IndexOf(const String &needle);

        unsigned int IndexOf(unsigned int start, const String &needle);

        AESTRING_SHIM unsigned int IndexOf_from(unsigned int start, const String *needle) {
            unsigned int slen = (unsigned int) this->length;
            unsigned int nlen = (unsigned int) needle->length;
            while (start < slen && nlen <= slen - start) {
                if (needle->data[0] == this->data[start]) {
                    unsigned int k = 0;
                    while (start + k < slen && this->data[start + k] == needle->data[k]) {
                        if (nlen <= k + 1)
                            return start;
                        k++;
                    }
                    start += k;
                } else {
                    start++;
                }
            }
            return 0xffffffff;
        }

        void PrintOut();

        void ReplaceChar(char from, char to);

        void ReplaceString(String find, String repl);

        void Reverse();

        AESTRING_SHIM void Set_char(const char *s) { Set(s); }
        AESTRING_SHIM void Set_float(float v) { Set(v); }

        AESTRING_SHIM void Set_longlong(long long v) { Set((long long) v); }
        AESTRING_SHIM void Set_wchar(const uint16_t *s) { Set((const unsigned short *) s); }

        void *Split(String sep);

        void SplitTags(String tag);

        AESTRING_SHIM int StrLen_char(const char *s) { return StrLen(s); }
        AESTRING_SHIM int StrLen_wchar(const uint16_t *s) { return StrLen((const unsigned short *) s); }

        AESTRING_SHIM void SubString(String *self, unsigned int start, unsigned int end) {
            clear();
            unsigned int slen = (unsigned int) self->length;
            if (start < end && start < slen) {
                unsigned int hi = end > slen ? slen : end;
                assignWide(self->data + start, (int) (hi - start));
            }
        }

        String SubString(unsigned int start, unsigned int end);

        void ToLowerCase();

        void ToUpperCase();

        void Trim();

        int ValueOf();

        AESTRING_SHIM String *addAssign_char(const char *c) {
            unsigned short ch = (unsigned short) *(const unsigned char *) c;
            appendWide(&ch, 1);
            return this;
        }

        AESTRING_SHIM String *addAssign_float(const float *v) {
            String tmp;
            tmp.ctor_float(*v);
            appendWide(tmp.data, tmp.length);
            return this;
        }

        AESTRING_SHIM String *addAssign_int(const int *v) {
            long long ext = (long long) *v;
            return addAssign_longlong(&ext);
        }

        AESTRING_SHIM String *addAssign_longlong(const long long *v) {
            String tmp;
            tmp.ctor_longlong(*v);
            appendWide(tmp.data, tmp.length);
            return this;
        }

        AESTRING_SHIM String *addAssign_str(String *other) {
            appendWide(other->data, other->length);
            return this;
        }

        AESTRING_SHIM String *assign(String *other) {
            *this = *other;
            return this;
        }

        AESTRING_SHIM void ctor() { clear(); }
        AESTRING_SHIM String *ctor_char(const char *s, bool reverse) {
            Set_char(s);
            if (reverse)
                Reverse();
            return this;
        }

        AESTRING_SHIM String *ctor_charval(char c) {
            Set_longlong((long long) c);
            return this;
        }

        AESTRING_SHIM String *ctor_copy(String *other, bool reverse) {
            Set(other->data);
            if (reverse)
                Reverse();
            return this;
        }

        AESTRING_SHIM String *ctor_float(float v) {
            Set_float(v);
            return this;
        }

        AESTRING_SHIM String *ctor_int(int v) {
            Set_longlong((long long) v);
            return this;
        }

        AESTRING_SHIM String *ctor_longlong(long long v) {
            Set_longlong(v);
            return this;
        }

        AESTRING_SHIM String *ctor_wchar(const uint16_t *s, bool reverse) {
            Set_wchar(s);
            if (reverse)
                Reverse();
            return this;
        }

        AESTRING_SHIM uint16_t *index(int i) {
            return reinterpret_cast<uint16_t *>((*this)[i]);
        }

        AESTRING_SHIM uint16_t *index_const(int i) {
            return const_cast<uint16_t *>(reinterpret_cast<const uint16_t *>(
                (*const_cast<const String *>(this))[i]));
        }

        static uint16_t *getWCharFromUtf8(char *utf8, int len);

        AESTRING_SHIM void clear() {
            if (data) delete[] data;
            data = nullptr;
            length = 0;
        }

        AESTRING_SHIM void assignWide(const unsigned short *src, int n) {
            if (data) delete[] data;
            data = nullptr;
            length = 0;
            if (!src || n < 0) return;
            unsigned short *nd = new unsigned short[n + 1];
            for (int i = 0; i < n; i++) nd[i] = src[i];
            nd[n] = 0;
            data = nd;
            length = n;
        }

        AESTRING_SHIM void appendWide(const unsigned short *src, int n) {
            if (!src || n <= 0) return;
            int newLen = length + n;
            unsigned short *nd = new unsigned short[newLen + 1];
            for (int i = 0; i < length; i++) nd[i] = data[i];
            for (int i = 0; i < n; i++) nd[length + i] = src[i];
            nd[newLen] = 0;
            if (data) delete[] data;
            data = nd;
            length = newLen;
        }
    };

    String operator+(const String &a, const String &b);

    String operator+(const int &a, const String &b);
}

using AbyssEngine::String;

int GetStringLength(AbyssEngine::String str);

#endif
