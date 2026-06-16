#ifndef GOF2_MISSION_H
#define GOF2_MISSION_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Agent;

using AbyssEngine::String;

// Galaxy on Fire 2 - a single mission (campaign or freelance): who issued it,
// where it takes place, its rewards, and the live won/failed/visible state.
class Mission {
public:
    void* vtable;
    uint8_t failed;
    uint8_t won;
    Agent* agent;
    int id;
    String name;                 // client name
    String targetName;
    int clientImage;
    int clientRace;
    int costs;                   // reward / costs slot (getReward/getCosts read this)
    int costsValue;              // +0x34 separate costs slot (setCosts writes this)
    int bonus;
    int targetStation;
    String targetStationName;
    String targetSystemName;
    int reward;                  // difficulty slot (getDifficulty reads this)
    uint8_t instantAction;
    int distance;
    int campaign;
    int productionGoodsA;        // production good index
    int productionGoodsB;        // production good amount
    int statusValue;
    uint8_t visible;

    Mission();
    Mission(int id);
    Mission(int id, int goods, int station);
    Mission(int id, const String* client, int clientImage, int clientRace,
            int costs, int station, int reward);
    ~Mission();

    void calcDistance();
    Mission* clone();
    void setCampaign_akw(int flag);
    void setWon_akw(int flag);
    String getClientName();
    String getDescription();
    String getName();
    String getTargetName();
    String getTargetStationName();
    String getTargetSystemName();
    bool isCampaignMission();
    bool isEmpty();
    uint8_t isInstantActionMission();
    uint8_t isVisible();
    void setInstantActionMission(bool v);
    void setProductionGoods(int a, int b);
    void* setTargetName(const String& rhs);
    void setTargetStation(int idx);
    void* setTargetSystemName(const String& rhs);
    void setVisible(bool v);

    int getType();
    void setType(int type);
    int getReward();
    void setReward(int reward);
    int getCosts();
    void setCosts(int costs);
    int getBonus();
    void setBonus(int bonus);
    int getDifficulty();
    int getDistance();
    int getStatusValue();
    void setStatusValue(int value);
    Agent* getAgent();
    void setAgent(Agent* agent);
    int getClientImage();
    int getClientRace();
    int getProductionGoodIndex();
    int getProductionGoodAmount();
    int getTargetStation();
    bool hasFailed();
    void setFailed(bool failed);
    bool hasWon();
    void setWon(bool won);
    void setCampaignMission(bool flag);
    bool isOutsideMission();
};
#endif
