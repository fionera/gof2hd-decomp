# Level::createMission() reconstruction (agent ada8daff, 2026-07-01)

STATUS: authoritative analysis, NOT yet applied/byte-verified. All ~40 referenced
methods/fields confirmed to EXIST in headers (createScene, createStaticObject,
getFreelanceMission, getSecurityLevel, Standing::getEnemyRace, setJumper, setToSleep,
setMissionCrate, PlayerFixedObject::setMoving, Route::getDockingTarget/reachWaypoint,
Mission::isCampaignMission/getProductionGoodAmount/getClientRace/getAgent; Level fields
objectivesA/B@0x?, missionPtr@0x?, playerRoute@0x108, enemyRoute@0x110, friendCount@0x160,
field_164@0x164, field_168@0x168, hostileCount@0x16c, field_180@0x180).

Ghidra pseudocode is BROKEN (operator new @0x6eb24 treated no-return -> case bodies
dropped). Recovered via DeepOpen Java v1 (GOF2/Level.java:560-1005) + manual Thumb
disasm of 0xbffd8-0xc4146. Function @0xbffd8, 0x4314 bytes, mangled _ZN5Level13createMissionEv.

## 0x20CFE8 table (int[6], type-183 Route literal), byte-verified:
static const int t183Route[6] = {-55000, 0, -35000, -30000, 0, -50000};
(reached via PC-rel load @0xc10da: DAT_000c1158=0x0014bf0e; eff=0x20CFE8)

## Objective type census (r1 of ctor, verified addrs):
case1->7(@c16c8) case2->18,18(@c19a2/c19b8) case3/5->11,12(@c0434/c0450)
case6->1(@c1b16) case7->7(@c1c7e) case9->18,18(@c3732/c3748) case10->7(@c20f2)
case12->20,21(@c234a/c2360) case15->18,18 case183->3(@c1498) case184->28,29(@c26e6/c29d6)

## Raid thresholds (v2, DIFFER from Java v1 80/60/35/10): 90/65/35/10 for sec 0/1/2/3+ (@c04ba..c04ce)

## Field-name mappings (Java->repo, verified offsets):
friendCount@0x160=localFightersCnt; field_164@0x164=jumperCnt; field_168@0x168=bigShipsCnt;
hostileCount@0x16c=raidersCnt; field_180@0x180=raidRoute(Route*); playerRoute@0x108=missionRoute;
enemyRoute@0x110=enemyRoute; field_c8/cc/d0@0xc8=asteroidFieldCenter(float xyz, case4).

## DEFERRED before applying (must resolve + verify.py-gate each):
1. isEmpty special-case chain asm 0xc06f0-0xc0a9e (pirate-base bonuses, hardcore scaling,
   isAttackedByAliens, station-index 10/78/100-108 clamps, campaign overrides, 0x189 flag pair
   =0x0101). Reconstruction implements ONLY Java-v1 core + (==10||sum==0->friendCount=4) clamp.
2. Case 7 tail: v2 has NO timeLimit=121000 (0x1D8A8 absent); stores int 22 to this+0x130 +
   addSoundResourceToList. +0x130 semantics unresolved.
3. Cases 15/183(0xb7)/184(0xb8): v2-only, outline-only from manual decode. 15: 3-wpt enemyRoute
   (-70000+r(140000)/70000/100000/140000), this+0x28c=1, len count+2, two Obj18. 183: Route(t183Route,6),
   len14, 8 enemies createShip(model 0x2c)+setMaxHitpoints(250), 2 stations createStaticObject(0x495d/0x4974),
   14 friendly createShip(model 0x33), Obj(3). 184: 3-wpt, len4, 2 stations createStaticObject(0x4a88/0x498e)
   Waypoint(40000)/Waypoint(30000,40000) names getText(0xc88/0xc89), Obj(28)/Obj(29).
4. AEVector3D float-vs-int coord storage (cases 2/9 may need float Route construction).
5. createShip trailing bool args (hostile,group): used (...,1,0) idiom throughout, per-call unconfirmed.

## FULL C++ BODY: see agent transcript ada8daffb106c841b.output (the ~400-line reconstruction).
## Per-case literal-pool constants (verified): 0xc0384=80000,0xc06d8=100000,0xc15f8=100000,
0xc15fc=75000,0xc19ec=-20000,0xc1d90=80000,0xc1da8=100000,0xc1dac=200000,0xc1db0=150000,
0xc2100=80000,0xc2104=120000,0xc2478=140000,0xc247c=-70000,0xc2480=70000,0xc2484=100000,
0xc2acc=140000,0xc2ad0=-70000,0xc2ad4=70000,0xc2ad8=100000.

# ============================================================
# AGENT 2 (a3bb61e75) — resolution of the 5 deferred items (byte-verified via llvm-objdump Thumb + ELF rel.plt/GOT)
# ============================================================
# Function 0xbffd8-0xc4145. Slot model: r10=this; status=*(Status**)[sp+0x6c]; rnd=*(AERandom**)[sp+0x64].

## Item 5 — createShip trailing args: ALL 39 sites = (hostile=1, group=0). Prior (...,1,0) correct everywhere.
## Item 4 — cases 2 & 9: integer-bit Route(int*,n) (compiler routes some coords through vcvt but stores INT word). No float Route anywhere in fn.
## Item 2 — case 7 tail (CORRECTED): v2 DOES set timeLimit=121000 (loads 120000 @0xc1da4 + adds 0x3e8). The `22` = arg to addSoundResourceToList.
##   this->objectivesA = new Objective(7, enemies->length - pirateCnt, this);
##   this->timeLimit = 121000;
##   Globals::globals->addSoundResourceToList(22);

## Item 1 — isEmpty count-adjustment chain 0xc0680-0xc0a9e (v2-only; Java v1 has only idx==10||sum==0->friendCount=4):
##   options = Globals::options (GOT 0x21016c); its FLOAT field @+0x2c = difficulty multiplier.
##   0xc0680: if(raid[slot54] && status->getCurrentCampaignMission()>=0x20 && rnd->nextInt(100)<=7){friendCount=9; raiders[hostileCount]=9;}
##   0xc06a6: slot30=status->getStation()->stationHasPirateBase(); float pb=slot30?5+(options->f_2c-0.5)*5:0; pirateBaseBonus=(int)pb;
##            if(status->getSystem() && getSystem()->hasPirateBase()){ pirateCnt=rnd->nextInt(1)+2; if(hardCoreMode())pirateCnt=rnd->nextInt(3)+pirateCnt*2; slot54=0; hostileCount=0; }
##   0xc0744: if(!Level::comingFromAlienWorld){ if(getStation()->isAttackedByAliens()){ if(getCurrentCampaignMission()!=0x2a){
##              field_168=max(field_168,2); hostileCount=rnd->nextInt(4)+2; if(landmarks[0x100]) raidRoute[0x180]->setNewCoords(landmark->getPos()); slot54=1; slot44=9; }}}
##   0xc07ac: wanted=status->getWantedInCurrentOrbit(); if(wanted){ if(friendCount>=3)friendCount=2; slot5c=1;} else slot5c=0; slot74=wanted;
##   0xc07d2: float f=0; cm=getCurrentCampaignMission(); if(cm>=0x65&&cm<=0x90) f=(float)cm/144.0*15+5; t=(int)f<<hardCoreMode();
##            extra=0; if(t>=1 && rnd->nextInt(100)<t) extra=(rnd->nextInt(3)+2)<<hardCoreMode(); slot2c=extra;
##   0xc084a: m=slot78; if(m && m->getType()==0xd && getStation()->f_0xf1==0 && m->getTargetStation()==getStation()->getIndex()){
##              field_164=field_168=0; hostileCount=0; slot54=0; slot1c=(getStation()->f_0xf0?7:6); goto skip160clamp; }
##   0xc089c: if(getStation()->hasAttackedFriends()){ friendCount=min(friendCount,7); *(uint16_t*)(this+0x189)=0x0101; }  // alarmRequested@0x189 + field_18a@0x18a
##   0xc08be: if(friendCount+field_164+field_168+hostileCount+pirateCnt==0) friendCount=4;  // skip160clamp:
##   0xc08d8: idx=getStation()->getIndex(); if(idx in{100,101,108}||idx==10){friendCount=0;slot54=0;field_164=field_168=0;}
##            else if(idx in{102,103,104}){friendCount=0;field_164=field_168=0; hostileCount=rnd->nextInt(5)+3; slot54=1; slot44=8;}
##   0xc0932: if((cm==0x24||cm==0x25) && getSystem() && getSystem()->getIndex()==5){hostileCount=0;slot54=0;friendCount=0;}
##   NOTE base block: station-index 78(0x4e) zeroing jumper/bigShips @0xc0614. raid thresholds 90/65/35/10 @0xc04b4.
##   STILL-COARSE TAIL: 0xc096a-0xc0a9e = inBlackMarketSystem/inPirateLootOrbit (raiders+=6/10, hardcore x) @0xc0992-0xc0a2a
##                      + station-index==0x6c and ==getCampaignMission targetStation loop @0xc0a2a-0xc0a9e. Needs full C++ before byte-precise landing.

## Item 3 — cases 15/183/184 (dispatch: tbh@0xc02a8 types1-15 idx=type-1; default check@0xc10c6: 0xb8->0xc2700, 0xb7->0xc10d2; type15->0xc2368):
## CASE 183(0xb7): enemyRoute=Route(t183Route{-55000,0,-35000,-30000,0,-50000},6); len14; [0..7] createShip(10,0,0x2c,wp0,1,0)+setAlwaysEnemy+setMaxHitpoints(250);
##   [8] createStaticObject(NULL,0x495d,false)+setMoving(false)+setDockingType(2)+setName(getText(0xc88))+setAlwaysFriend; [9] createStaticObject(&wpB,0x4974,true)+setDockingType(1)+setHitpoints(9999999);
##   [10..13] createShip(0,0,0x33,fmtWp0,1,0)+setRoute(clone)+setAlwaysFriend + per-ship FLOAT diamond pos (bits @0xc15bc..); ego->setPosition(...); timeLimit=181000; objectivesB=new Objective(3,181000,this).  (coords marked -ish need verify)
## CASE 184(0xb8): enemyRoute=Route(int[9],3); len4; [0]createStaticObject(&wpA(z=40000),0x4a88,true); [1]createStaticObject(...,0x498e,..) Waypoint(30000,40000) names getText(0xc88/0xc89);
##   objectivesB=new Objective(28,m->getProductionGoodAmount(),m->getProductionGoodIndex(),this); objectivesA=new Objective(29,...,this).
## CASE 15: enemyRoute=Route(int[3],3)(coord -70000+rnd(140000)); field_28c=1; enemyShipCnt=2+(int)(diff/10*2); enemyRace=Standing::getEnemyRace(m->getClientRace()); len=field_28c+n+2; two Objective(18,..).

## Level.h field notes: 0x130=timeLimit(named,ok); 0x189=alarmRequested + 0x18a=field_18a(set to 1 as halfword 0x0101); 0x28c=miningPlant(set=1 in case15 - verify/rename).
## STATUS: ~90% byte-verified. REMAINING for byte-exact landing: full C++ for isEmpty tail 0xc096a-0xc0a9e; exact coords cases 15/183(diamond)/184; then assemble + verify.py per-case gate.

# ============================================================
# AGENT 3 (a71556f) — FINAL byte-verified: isEmpty tail + cases 15/183/184 exact. NOTHING DEFERRED.
# ============================================================
## isEmpty tail 0xc096a-0xc0a9e (exact):
##  cm=getCurrentCampaignMission(); if(cm in{42,43} && !inAlienOrbit()){raidActive=0; raidersCnt=0;}
##  if(status->inBlackMarketSystem()){friendCount=0; raidersCnt=rnd->nextInt(4)+6; if(hardCoreMode())raidersCnt=rnd->nextInt(3)+raidersCnt*2; bigShipsCnt=0; jumperCnt=0; raidActive=1; raidType=8;}
##  if(status->inPirateLootOrbit()){friendCount=0; raidersCnt=rnd->nextInt(4)+10; if(hardCoreMode())raidersCnt=rnd->nextInt(3)+raidersCnt*2; bigShipsCnt=0; jumperCnt=0; raidActive=1; raidType=8;}
##  if(getStation()->getIndex()==108){ if(status->field_114==0) goto station108_handler@0xc0f3e; raidActive=0; raidersCnt=0; }
##  campaignStationMatch=false; if(getCampaignMission() && getCampaignMission()->getType()==163 && status->field_90){ for(i=0;i<field_90->length;i++) if(field_90->data[i]==getStation()->getIndex()){campaignStationMatch=true;break;} }
##  (station108_handler@0xc0f3e: friendCount=0; field_0x288=1; jumperCnt=bigShipsCnt=0; raidersCnt=hardCoreMode()?8:6; new enemies Array...)
## Case183 diamond floats (ship idx r6-10, KIPlayer::setPosition(Vector&) vtable+0x44):
##  idx0={20000,-3000,-50000} idx1={10000,-7000,-30000} idx2={25000,0,-36000} idx3={12000,6000,-45000}
##  ego->setPosition(-30000,5000,-100000); field_c8=ego->getPosition(); timeLimit=181000; objectivesA=new Objective(3,181000,this).
## Case184 0xc2700: enemyRoute=Route({rnd(140000)-70000,0,70000, rnd(140000)-70000,0,100000, rnd(140000)-70000,0,140000},3);
##  getMission()->getDifficulty()[discard]; getStanding()->getEnemyRace(getClientRace())[discard]; enemies len=4;
##  [0]=createStaticObject(new Waypoint(-30000,0,40000,0),0x4a88,true)+setMoving(false)+setDockingType(2)+setName(String(gameText->getText(0xc88),false));
##  [1]=createStaticObject(new Waypoint(30000,0,40000,0),0x498e,true)+setMoving(false)+setDockingType(1)+setName(...getText(0xc89));
##  [2],[3]=createShip(clientRace,0,getRandomEnemyFighter(clientRace),wp,true,false)+setRoute(clone)+setAlwaysFriend(true)+setNeverAttack(true);
##  objectivesA=new Objective(29, mission->getProductionGoodAmount(), mission->getProductionGoodIndex(), this). [SINGLE objective]
## Case15 0xc2368: enemyRoute=Route(same-shape int[9],3); enemyShipCnt: d=(float)getDifficulty(); t=d/10; t+=t; base=(int)t+1; nf=base*(options->difficulty@0x2c+0.5); enemyShipCnt=(int)nf;
##  enemyRace=getStanding()->getEnemyRace(getClientRace()); field_28c=1(CONFIRMED off 0x28c); enemies len=field_28c+enemyShipCnt+2;
##  for(i<enemyShipCnt){model=Globals::globals->getRandomEnemyFighter(enemyRace); w=enemyRoute->getWaypoint(rnd->nextInt(enemyRoute->length())); s=createShip(enemyRace,0,model,w,true,false); enemies->data[i]=s; s->player->setAlwaysEnemy(true);}
##  mining-plant: createStaticObject(field_d8,0x4a88,true) at enemies[field_28c], setDockingType(1),setMoving(false),setPosition(field_c8); +2-ship friendly formation(setAlwaysFriend+setNeverAttack).
##  objectivesA=new Objective(28, getProductionGoodAmount(), getProductionGoodIndex(), this). [SINGLE objective]
## FIXES: raiders is ASSIGNMENT not +=. cases 15/183/184 each ONE Objective(28/3/29)->objectivesA@0x28 (prior census 18,18 & 28,29 WRONG).
## shared tail 0xc29da: this->objectivesA = <built Objective> (cases 15,183,184 all store to 0x28).
