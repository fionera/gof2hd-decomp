#ifndef GOF2_ABSTRACTGUN_H
#define GOF2_ABSTRACTGUN_H

class AbstractGun {
public:
    virtual int isRocketGun();

    virtual int isBombGun();

    virtual int isMineGun();

    virtual int isAutoTurret();
};
#endif
