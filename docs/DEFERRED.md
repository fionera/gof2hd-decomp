- [ ] src/game/weapons/ObjectGun.h: ObjectGun (the polymorphic base) should declare the full predicate trio as virtuals returning 0 -- `virtual int isBombGun();` and `virtual int isMineGun();` alongside the existing `virtual int isRocketGun();`. The Android binary's vtables (e.g. 0x223608, 0x21e030) place isRocketGun(0x1668c4 ->0), isBombGun(0x1668c8 ->0), isMineGun(0x1668cc ->1) in three consecutive slots, so MineGun::isMineGun is truly an override of a base virtual. We only model isRocketGun on the base today, so isMineGun is currently declared `virtual` directly on MineGun (correct symbol/mangling, returns 1) rather than `override`. Adding the base virtuals would let this become a proper override and keep the vtable slot ordering identical to the original.

## Coordinated: setPosition3 -> setPosition(float,float,float) family rename
Original binary names the actor 3-float positioner `setPosition(float,float,float)` (KIPlayer @0xb6394
+ overload `setPosition(Vector const&)` @0xb6360); our tree calls it `setPosition3` and `setPosition_vec`.
NOT a blind rename: some classes (PlayerEgo.h:344) ALREADY declare `setPosition(float,float,float)`
alongside `setPosition3` (redundant) -> those need the `setPosition3` decl/def DELETED and call sites
repointed, while classes with only `setPosition3` need a true rename. Do as one careful coordinated
pass (base KIPlayer + derived PlayerFighter/Jumpgate/WormHole/FixedObject/Ego + call sites in
CutScene/SpaceLounge/Level/SentryGun/TouchButton), build-gating every affected TU, verifying each
`_ZN..11setPositionEfff` matches. Also rename `setPosition_vec` -> `setPosition` (the Vector overload).
