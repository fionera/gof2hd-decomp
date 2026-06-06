// Player::~Player() — real C++ destructor so the demangled symbol contains "~Player".
// Self-contained layout (does not include class.h, which declares Player as a plain struct).

typedef unsigned int uint32_t;

struct Gun;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t cap;
};

extern "C" void ArrayReleaseClasses_GunArray(Array<Gun *> *array);
extern "C" void ArrayReleaseClasses_GunArrayArray(Array<Array<Gun *> *> *array);
extern "C" void *Array_GunArray_dtor(Array<Gun *> *array);
extern "C" void *Array_GunArrayArray_dtor(Array<Array<Gun *> *> *array);
extern "C" void *Array_Player_dtor(Array<void *> *array);
extern "C" void Player_operator_delete(void *p);

struct Player {
    Array<Array<Gun *> *> *guns;        // 0x00
    unsigned char pad[0x70];            // 0x04 .. 0x73
    Array<void *> *enemies;             // 0x74

    ~Player();
};

__attribute__((minsize)) Player::~Player()
{
    Array<Array<Gun *> *> *guns = this->guns;
    if (guns != 0) {
        unsigned int i = 0;
        int byteOff = 0;
        for (; i < guns->length; i++) {
            Array<Gun *> *slot = *(Array<Gun *> **)((char *)guns->data + byteOff);
            if (slot != 0) {
                ArrayReleaseClasses_GunArray(slot);
                Array<Gun *> *s2 = *(Array<Gun *> **)((char *)this->guns->data + byteOff);
                if (s2 == 0) {
                    *(void **)((char *)this->guns->data + i * 4) = 0;
                } else {
                    Player_operator_delete(Array_GunArray_dtor(s2));
                    *(void **)((char *)this->guns->data + byteOff) = 0;
                }
                guns = this->guns;
            }
            byteOff += 4;
        }
        ArrayReleaseClasses_GunArrayArray(guns);
        if (this->guns != 0) {
            Player_operator_delete(Array_GunArrayArray_dtor(this->guns));
        }
        this->guns = 0;
    }
    if (this->enemies != 0) {
        Player_operator_delete(Array_Player_dtor(this->enemies));
    }
    this->enemies = 0;
}
