// Station::~Station() — real C++ destructor so the demangled symbol contains "~Station".

typedef unsigned int uint32_t;

struct Ship; struct Item; struct Agent; struct Mission; struct Status;

extern "C" void ArrayReleaseClasses_Ship(void *arr) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_Item(void *arr) __attribute__((nothrow));
extern "C" void *Array_Ship_dtor(void *arr) __attribute__((nothrow));
extern "C" void *Array_Item_dtor(void *arr) __attribute__((nothrow));
extern "C" void *Array_Agent_dtor(void *arr) __attribute__((nothrow));
extern "C" void Station_operator_delete(void *p) __attribute__((nothrow));

extern "C" Mission *Status_getCampaignMission(Status *s) __attribute__((nothrow));
extern "C" Mission *Status_getFreelanceMission(Status *s) __attribute__((nothrow));
extern "C" Agent *Mission_getAgent(Mission *m) __attribute__((nothrow));
extern "C" int Agent_isStoryAgent(Agent *a) __attribute__((nothrow));
extern "C" void *Agent_dtor(Agent *a) __attribute__((nothrow));

// base destructor (String member dtor), tail-called directly.
extern "C" void Station_baseDtor(void *self) __attribute__((nothrow));

// Status singleton: global slot holds P (kept in a reg); *P is reloaded per call.
extern Status **const gStatusForDtor __attribute__((visibility("hidden")));
#define STATUS (*gStatusForDtor)

template <class T>
struct Array { uint32_t length; T *data; uint32_t cap; };

struct Station {
    unsigned char pad_0[0x28];
    Array<Item *> *items;   // 0x28
    Array<Ship *> *ships;   // 0x2c
    Array<Agent *> *agents; // 0x30
    ~Station() noexcept(false);
};

__attribute__((minsize)) Station::~Station() noexcept(false)
{
    if (this->ships != 0) {
        ArrayReleaseClasses_Ship(this->ships);
        if (this->ships != 0)
            Station_operator_delete(Array_Ship_dtor(this->ships));
        this->ships = 0;
    }
    if (this->items != 0) {
        ArrayReleaseClasses_Item(this->items);
        if (this->items != 0)
            Station_operator_delete(Array_Item_dtor(this->items));
        this->items = 0;
    }
    Array<Agent *> *agents = this->agents;
    if (agents != 0) {
        for (uint32_t i = 0; i < agents->length; i++) {
            Agent *a = agents->data[i];
            Agent *campA = Status_getCampaignMission(STATUS) == 0
                               ? (Agent *)0
                               : Mission_getAgent(Status_getCampaignMission(STATUS));
            Agent *freeA = Status_getFreelanceMission(STATUS) == 0
                               ? (Agent *)0
                               : Mission_getAgent(Status_getFreelanceMission(STATUS));
            if (a != 0 && a != campA && a != freeA && Agent_isStoryAgent(a) == 0)
                Station_operator_delete(Agent_dtor(a));
            agents = this->agents;
        }
        if (agents != 0)
            Station_operator_delete(Array_Agent_dtor(agents));
        this->agents = 0;
    }
    Station_baseDtor(this);
}
