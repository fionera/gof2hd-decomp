#ifndef GOF2_ABSTRACTGUN_H
#define GOF2_ABSTRACTGUN_H

// Methods are inline (no out-of-line key function) so AbstractGun's vtable is NOT
// homed here. The original emits vtables only for the 7 concrete guns; AbstractGun
// itself is never instantiated, so it has _ZTI (base typeinfo) but no _ZTV.
class AbstractGun {
public:
    virtual int isRocketGun() { return 0; }

    virtual int isBombGun() { return 0; }

    virtual int isMineGun() { return 0; }

    virtual int isAutoTurret() { return 0; }
};
#endif
