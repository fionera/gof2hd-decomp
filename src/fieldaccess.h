#ifndef GOF2_FIELDACCESS_H
#define GOF2_FIELDACCESS_H

#include <cstdint>

template<class T>
static inline T &F(void *p, uint32_t off) {
    return *(T *) ((char *) p + off);
}

static inline float &f32(void *self, unsigned int offset) {
    return *(float *) ((char *) self + offset);
}

template<class T>
static inline T &field(void *self, uint32_t off) {
    return *(T *) ((char *) self + off);
}

static inline float &field_f32(void *self, uint32_t offset) {
    return *(float *) ((char *) self + offset);
}

static inline int &field_i32(void *self, uint32_t offset) {
    return *(int *) ((char *) self + offset);
}

static inline void *field_ptr(void *self, uint32_t offset) {
    return *(void **) ((char *) self + offset);
}

static inline uint8_t &field_u8(void *self, uint32_t offset) {
    return *(uint8_t *) ((char *) self + offset);
}

static inline short &i16(void *self, uint32_t off) { return *(short *) ((char *) self + off); }

static inline int &i32(void *self, unsigned int offset) {
    return *(int *) ((char *) self + offset);
}

static inline void *&pp(void *self, uint32_t off) { return *(void **) ((char *) self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *) ((char *) self + off); }

static inline uint32_t &u32(void *self, uint32_t offset) {
    return *(uint32_t *) ((char *) self + offset);
}

static inline uint8_t &u8(void *self, unsigned int offset) {
    return *(uint8_t *) ((char *) self + offset);
}

#endif // GOF2_FIELDACCESS_H
