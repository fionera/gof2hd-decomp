// Agent::~Agent() — real C++ destructor so the demangled symbol contains "~Agent".

typedef unsigned int uint32_t;

extern "C" __attribute__((nothrow)) void Agent_operator_delete_arr(void *p);

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gStringDtor __attribute__((visibility("hidden")));

struct VObj { dtor_fn vt[8]; };

struct Agent {
    void *name_str[3];   // 0x00 String (this+0x0)
    VObj *field_c;        // 0x0c  virtual; dtor at vt[1]
    unsigned char pad_10[0x88 - 0x10];
    void *imageParts;     // 0x88
    ~Agent() noexcept(false);
};

__attribute__((minsize)) Agent::~Agent() noexcept(false)
{
    if (this->imageParts != 0)
        Agent_operator_delete_arr(this->imageParts);

    VObj *o = this->field_c;
    this->imageParts = 0;
    if (o != 0) {
        (*(dtor_fn *)((char *)o->vt[0] + 4))(o);
        this->field_c = 0;
    }

    dtor_fn d = gStringDtor;
    d((char *)this + 0x78);
    d((char *)this + 0x6c);
    d((char *)this + 0x18);
    d(this);
}
