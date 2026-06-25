#ifndef GOF2_ABSTRACTGUN_H
#define GOF2_ABSTRACTGUN_H




class AbstractGun {
public:
    virtual int isRocketGun() { return 0; }

    virtual int isBombGun() { return 0; }

    virtual int isMineGun() { return 0; }

    virtual int isAutoTurret() { return 0; }
};
#endif
