#ifndef GOF2_WANTED_H
#define GOF2_WANTED_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"

class Wanted {
public:
    String name;
    int index;
    int board;
    int race;
    bool male;
    int shipId;
    int weapon;
    int hitpoints;
    int lootItemId;
    int lootAmount;
    int reward;
    int requiredBounties;
    int requiredMission;
    int numWingmen;
    int *imageParts;
    int lastSeen;
    int travelsTo;
    int currentLocation;
    bool terminated;
    bool active;

    Wanted(int index, String name, int board, int race, bool male,
           int shipId, int weapon, int hitpoints, int lootItemId, int lootAmount,
           int reward, int requiredBounties, int requiredMission, int numWingmen);

    ~Wanted();

    String getName();

    bool isActive();

    bool isTerminated();

    void setActive(bool v);

    void setTerminated(bool v);

    int getIndex();

    int getBoard();

    int getRace();

    bool isMale();

    int getShip();

    int getWeapon();

    int getHitpoints();

    int getLoot();

    int getLootAmount();

    int getReward();

    int getRequiredBounties();

    int getRequiredMission();

    int getNumWingmen();

    int getCurrentLocation();

    int getTravelsTo();

    int getLastSeen();

    int *getImageParts();

    void setImageParts(int *parts);

    void setRequiredMission(int v);

    void setCurrentLocation(int v);

    void setTravelsTo(int v);

    void setLastSeen(int v);
};
#endif
