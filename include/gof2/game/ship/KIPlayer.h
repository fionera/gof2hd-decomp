#ifndef GOF2_KIPLAYER_H
#define GOF2_KIPLAYER_H
#include "gof2/common.h"
#include "gof2/math.h"

// Galaxy on Fire 2 - KIPlayer: the AI controller that wraps a Player ship and drives
// it through routes, navigation/docking space points, jumps and wingman commands.

class Player;
class Route;
class Gun;
class SpacePoint;
class Level;
class Hud;
class AEGeometry;

class KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    Player*             player;
    AEGeometry*         geometry;
    AEGeometry*         parentGeometry;
    int                 field_0x10;
    int                 field_0x14;
    String              name;
    int                 field_0x24;
    uint8_t             field_0x25;
    int                 shipGroup;
    int                 field_0x2c;
    int                 field_0x30;
    int                 field_0x34;
    int                 stealFlag;
    char                field_0x3f;
    int                 field_0x40;
    char                field_0x42;
    int                 field_0x44;
    int                 field_0x48;
    int                 field_0x4c;
    Array<int>*         cargo;
    Level*              level;
    float               posX;
    float               posY;
    float               posZ;
    uint32_t            field_0x64;
    uint8_t             field_0x68;
    uint8_t             field_0x69;
    uint8_t             field_0x6a;
    Route*              route;
    int                 field_0x70;
    uint8_t             field_0x72;
    uint16_t            field_0x73;
    uint8_t             field_0x75;
    uint8_t             field_0x76;
    AEGeometry*         crateGeometry;
    int                 shipGroupFlag;
    int                 field_0x80;
    int                 field_0x84;
    int                 state;
    int                 field_0x8c;
    int                 field_0x90;
    int                 field_0x94;
    int                 field_0x98;
    int                 field_0x9c;
    int                 field_0xa0;
    int                 field_0xa4;
    float               rotationSpeed;
    int                 type;
    uint8_t             sleepFlag;
    uint8_t             initActiveFlag;
    Route*              initialRoute;
    int                 field_0xbc;
    uint8_t             field_0xc0;
    Array<uint32_t>*    spacePointIds;
    int                 field_0xc8;
    Array<SpacePoint*>* spacePoints;
    int                 field_0xd0;
    uint32_t            jumpSphere;
    int                 field_0xd8;
    uint8_t             wingmanFlag;
    int                 wingmanCommand;
    int                 field_0xe4;
    KIPlayer*           wingmanTarget;
    uint8_t             jumperFlag;
    uint8_t             jumpDone;
    uint8_t             visibleFlag;
    int                 engineSoundEvent;
    uint32_t            field_0xfc;
    uint8_t             field_0x100;
    uint8_t             field_0x101;
    int                 field_0x104;
    int                 field_0x10c;
    int                 field_0x110;
    int                 field_0x114;
    int                 field_0x118;
    int                 field_0x11c;
    int                 field_0x120;

    KIPlayer(int faction, int group, Player* player, AEGeometry* geom,
             float x, float y, float z, bool active);
    ~KIPlayer();

    void PauseEngineSound();
    void PlayEngineSound();
    void ResumeEngineSound();
    void StopEngineSound();
    void addGun_a();
    void addGun_b();
    void awake();
    void captureCrate(Hud* hud);
    int  cargoAvailable();
    void createCrate(int type);
    SpacePoint* getNearestDockingPoint(Vector* dir);
    SpacePoint* getNearestNavigationPoint(Vector* dir, SpacePoint* target);
    Vector getPosition();
    int  getType();
    bool isDead();
    bool isDocked();
    bool isDying();
    uint8_t isEnemy();
    uint8_t isJumper();
    uint8_t isVisible();
    uint8_t isWingMan();
    void jump();
    void outerCollide(const Vector& v);
    // Polymorphic: PlayerJunk/PlayerAsteroid/PlayerCreature/PlayerTurret override this.
    // Level::render()/renderPause() dispatch through it for every actor in the rosters.
    virtual void render();
    void reset();
    void setActive();
    void setDead();
    void setEnemies(Array<Player*>* enemies);
    void setInitActive();
    void setJumpSphere(uint32_t sphere);
    void setJumper(bool b);
    // Polymorphic position setter dispatched through the actor vtable from Level's
    // spawn/respawn paths (createMission/updateOrbit/createSentryGuns).
    virtual void setPosition3(float x, float y, float z);
    void setPosition_vec(const Vector& v);
    void setRotationSpeed(float speed);
    void setRoute(Route* route);
    void setShipGroup(int param2, int flag, int cond);
    void setSpacePoints(Array<SpacePoint*>* pts);
    void setState(int state);
    void setToSleep();
    void setVisible(bool visible);
    void setWingman(bool b, int cmd);
    void setWingmanCommand(int cmd, KIPlayer* target);
    void translate(const Vector& v);
    Route* getRoute();
    Array<SpacePoint*>* getSpacePoints();
    // Polymorphic: PlayerFighter/PlayerEgo override this to also wire up their engine-trail
    // particle systems. Dispatched through the actor vtable right after construction.
    virtual void setLevel(Level* lvl);
    // Adds a specific gun to the underlying Player's weapon set (forwarded to Player::addGun).
    void addGun(Gun* gun);
    // Engages / disengages the auto-pilot lock onto the given target (engaged when target != null).
    void setAutoPilot(KIPlayer* target);

    // Collision-query virtuals inherited from the bounding-volume interface. A KIPlayer
    // does not expose a surface to project onto, so all three return a zeroed vector.
    Vector getProjectionVector(const Vector& v);
    Vector getCollisionNormal(const Vector& position);
    Vector projectCollisionOnSurface(const Vector& position);
};
#endif
