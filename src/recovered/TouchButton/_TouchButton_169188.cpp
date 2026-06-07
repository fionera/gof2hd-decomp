// TouchButton::~TouchButton() — real C++ destructor so the demangled symbol contains "~TouchButton".

typedef unsigned int uint32_t;

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gStringDtor __attribute__((visibility("hidden")));

struct TouchButton {
    unsigned char pad[0x100];
    ~TouchButton() noexcept(false);
};

__attribute__((minsize)) TouchButton::~TouchButton() noexcept(false)
{
    dtor_fn d = gStringDtor;
    d((char *)this + 0x2c);
    d((char *)this + 0x18);
    d((char *)this + 0xc);
}
