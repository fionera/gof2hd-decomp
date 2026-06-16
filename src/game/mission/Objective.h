#ifndef GOF2_OBJECTIVE_H
#define GOF2_OBJECTIVE_H
#include "common.h"

class Level;
class KIPlayer;

namespace AbyssEngine { struct String; }

// Galaxy on Fire 2 — Objective: one mission goal evaluated against the in-flight
// Level. Objectives nest (an objective may own a list of child objectives) and
// each carries a type discriminator plus up to two integer parameters.
class Objective {
public:
    int type;                              // objective type discriminator
    int value;                             // value / index parameter
    int calcValue;                         // calc value / end-of-range parameter
    Level* level;                          // owning level
    Array<Objective*>* children;           // child objectives
    AbyssEngine::String* achievedText;     // achieved-text message
    int storedValue;                       // captured value when type == 0xd

    Objective(int type, int value, Level* level);
    Objective(int type, int value, int calcValue, Level* level);
    ~Objective();

    Objective* addObjective(Objective* objective);
    void setAchievedText(AbyssEngine::String* text);
    bool isSurvivalObjective();
    bool getCalcValue();
    unsigned int achieved(int value);
};
#endif
