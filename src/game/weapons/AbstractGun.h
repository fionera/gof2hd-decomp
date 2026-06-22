#ifndef GOF2_ABSTRACTGUN_H
#define GOF2_ABSTRACTGUN_H

class AbstractGun {
public:
    // (the +0x0 vptr is the compiler-managed C++ vtable; the class is polymorphic)

    virtual int isRocketGun();

    virtual int isBombGun();

    virtual int isMineGun();

    virtual int isAutoTurret();
};
#endif
