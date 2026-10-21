#include "game/mission/Mission.h"
#include "game/core/Globals.h"
#include "game/world/Galaxy.h"
#include "game/world/SolarSystem.h"
#include "game/mission/Status.h"
#include "engine/core/GameText.h"
#include "game/world/Station.h"


uint8_t Mission::isInstantActionMission() {
    return this->instantAction;
}

String Mission::getTargetStationName() {
    return String(this->targetStationName, false);
}

bool Mission::isCampaignMission() {
    return this->campaign != 0;
}

String Mission::getDescription() {
    return String("");
}

String Mission::getTargetSystemName() {
    return String(this->targetSystemName, false);
}

String Mission::getName() {
    if (this->campaign != 0) {
        return String("");
    }
    return *Globals::gameText->getText(this->id + 0x162);
}

void Mission::setProductionGoods(int a, int b) {
    this->productionGoodsA = a;
    this->productionGoodsB = b;
}

String &Mission::setTargetName(String rhs) {
    return this->targetName = rhs;
}

String Mission::getClientName() {
    return String(this->name, false);
}

bool Mission::isVisible() {
    return this->visible;
}

bool Mission::isEmpty() {
    return this->id == -1;
}

String Mission::getTargetName() {
    return String(this->targetName, false);
}

void Mission::setInstantActionMission(bool v) {
    this->instantAction = v;
}

String &Mission::setTargetSystemName(const String &rhs) {
    return this->targetSystemName = rhs;
}

void Mission::setVisible(bool v) {
    this->visible = v;
}

Mission *Mission::clone() {
    Mission *m = new Mission(this->id, AbyssEngine::String(this->name, false), this->clientImage,
                             this->clientRace, this->costs, this->targetStation,
                             this->reward);
    m->instantAction = this->instantAction;
    return m;
}

void Mission::setTargetStation(int idx) {
    this->targetStation = idx;
    this->targetStationName =
        ((Station *) (long) Globals::galaxy->getStation(this->targetStation))->getName();
}

Mission::Mission(int id, AbyssEngine::String client, int *clientImage, int clientRace,
                 int costs, int station, int reward) {
    this->id = id;
    this->name = client;
    this->clientImage = clientImage;
    this->clientRace = clientRace;
    this->costs = costs;
    this->targetStation = station;
    this->targetStationName = ((Station *) (long) Globals::galaxy->getStation(station))->getName();
    this->reward = reward;
    this->targetSystemName = String("");
    this->campaign = 0;
    this->visible = 1;
    this->instantAction = 0;
    this->failed = 0;
    this->won = 0;
    this->agent = 0;
    this->bonus = 0;
}

Mission::Mission(int id) {
    this->name = String("");
    this->visible = 0;
    this->campaign = 0;
    this->agent = 0;
    this->id = id;
    this->clientImage = 0;
    this->clientRace = 0;
    this->costs = 0;
    this->reward = 0;
    this->instantAction = 0;
    this->statusValue = 0;
    this->failed = 0;
    this->won = 0;
    this->bonus = 0;
    this->targetStation = 0;
}

void Mission::calcDistance() {
    Station *st = (Station *) (long) Globals::galaxy->getStation(this->targetStation);
    Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
    this->distance = (int) Globals::galaxy->distance((*systems)[Globals::status->getStation()->getSystem()],
                                                     (*systems)[st->getSystem()]);
    delete st;
}

Mission::Mission(int id, int goods, int station) {
    this->targetStation = station;
    this->costs = goods;
    this->clientImage = 0;
    this->agent = 0;
    this->id = id;
    this->targetStationName =
        station >= 0 ? ((Station *) (long) Globals::galaxy->getStation(station))->getName() : String("");
    this->targetSystemName = String("");
    this->campaign = 1;
    this->visible = 1;
    this->failed = 0;
    this->won = 0;
    this->instantAction = 0;
    this->statusValue = 0;
    this->bonus = 0;
}

Mission::~Mission() {
}

Mission::Mission() {
    this->name = String("");
    this->visible = 0;
    this->campaign = 0;
    this->agent = 0;
    this->id = -1;
    this->clientImage = 0;
    this->clientRace = 0;
    this->costs = 0;
    this->reward = 0;
    this->instantAction = 0;
    this->statusValue = 0;
    this->failed = 0;
    this->won = 0;
    this->bonus = 0;
    this->targetStation = 0;
}

int Mission::getType() {
    return this->id;
}

int Mission::getReward() {
    return this->costs;
}

void Mission::setReward(int reward) {
    this->costs = reward;
}

int Mission::getCosts() {
    return this->costs;
}

void Mission::setCosts(int costs) {
    this->costsValue = costs;
}

int Mission::getBonus() {
    return this->bonus;
}

void Mission::setBonus(int bonus) {
    this->bonus = bonus;
}

int Mission::getDifficulty() {
    return this->reward;
}

int Mission::getDistance() {
    return this->distance;
}

int Mission::getStatusValue() {
    return this->statusValue;
}

void Mission::setStatusValue(int value) {
    this->statusValue = value;
}

Agent *Mission::getAgent() {
    return this->agent;
}

void Mission::setAgent(Agent *agent) {
    this->agent = agent;
}

int *Mission::getClientImage() {
    return this->clientImage;
}

int Mission::getClientRace() {
    return this->clientRace;
}

int Mission::getProductionGoodIndex() {
    return this->productionGoodsA;
}

int Mission::getProductionGoodAmount() {
    return this->productionGoodsB;
}

int Mission::getTargetStation() {
    return this->targetStation;
}

unsigned char Mission::hasFailed() { return this->failed; }

void Mission::setFailed(bool failed) {
    this->failed = failed ? 1 : 0;
}

unsigned char Mission::hasWon() { return this->won; }

void Mission::setWon(bool won) {
    this->won = won ? 1 : 0;
}

void Mission::setCampaignMission(bool flag) {
    this->campaign = flag ? 1 : 0;
}

bool Mission::isOutsideMission() {
    return true;
}

void Mission::setType(int type) {
    this->id = type;
}

void Mission::setDifficulty(int difficulty) {
    this->reward = difficulty;
}

Mission *Mission::empty;
