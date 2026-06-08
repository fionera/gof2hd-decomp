// PlayerEgo::~PlayerEgo() -- real C++ destructor so the demangled symbol contains "~PlayerEgo".
//
// Deletes every owned sub-object (the wrapped Player, the various AEGeometry
// nodes for hull / shield / cloak / turret / docking rigs, the Route, tractor
// and repair beams, the mining game, both explosions, the ease matrix) and
// nulls each handle, mirroring the target teardown order.

typedef unsigned int uint32_t;

extern "C" void *Player_dtor(void *);
extern "C" void *AEGeometry_dtor(void *);
extern "C" void *Route_dtor(void *);
extern "C" void *TractorBeam_dtor(void *);
extern "C" void *MiningGame_dtor(void *);
extern "C" void *Explosion_dtor(void *);
extern "C" void *EaseInOutMatrix_dtor(void *);
extern "C" void  Array_releaseRepairBeams(void *);     // ArrayReleaseClasses<RepairBeam*>
extern "C" void *ArrayRB_dtor(void *);                 // Array<RepairBeam*>::~Array
extern "C" void  operator_delete_(void *) noexcept;

struct PlayerEgo {
    unsigned char pad[0x400];
    ~PlayerEgo() noexcept(false);
};

static inline void *&PP(void *self, uint32_t off) { return *(void **)((char *)self + off); }

__attribute__((minsize)) PlayerEgo::~PlayerEgo() noexcept(false)
{
    void *self = this;
    if (PP(self, 0x0))   operator_delete_(Player_dtor(PP(self, 0x0)));
    PP(self, 0x0) = 0;
    if (PP(self, 0x4))   operator_delete_(AEGeometry_dtor(PP(self, 0x4)));
    PP(self, 0x4) = 0;
    if (PP(self, 0x8))   operator_delete_(AEGeometry_dtor(PP(self, 0x8)));
    PP(self, 0x8) = 0;
    if (PP(self, 0xdc))  operator_delete_(AEGeometry_dtor(PP(self, 0xdc)));
    PP(self, 0xdc) = 0;
    if (PP(self, 0x28))  operator_delete_(AEGeometry_dtor(PP(self, 0x28)));
    PP(self, 0x28) = 0;
    if (PP(self, 0xfc))  operator_delete_(Route_dtor(PP(self, 0xfc)));
    PP(self, 0xfc) = 0;
    if (PP(self, 0x178)) operator_delete_(AEGeometry_dtor(PP(self, 0x178)));
    PP(self, 0x178) = 0;
    if (PP(self, 0x17c)) operator_delete_(AEGeometry_dtor(PP(self, 0x17c)));
    PP(self, 0x17c) = 0;
    if (PP(self, 0x2c))  operator_delete_(AEGeometry_dtor(PP(self, 0x2c)));
    PP(self, 0x2c) = 0;
    if (PP(self, 0x30))  operator_delete_(AEGeometry_dtor(PP(self, 0x30)));
    PP(self, 0x30) = 0;
    if (PP(self, 0x34))  operator_delete_(AEGeometry_dtor(PP(self, 0x34)));
    PP(self, 0x34) = 0;
    if (PP(self, 0x19c)) operator_delete_(AEGeometry_dtor(PP(self, 0x19c)));
    PP(self, 0x19c) = 0;
    if (PP(self, 0x1b4)) operator_delete_(TractorBeam_dtor(PP(self, 0x1b4)));
    PP(self, 0x1b4) = 0;
    if (PP(self, 0x1e4)) operator_delete_(MiningGame_dtor(PP(self, 0x1e4)));
    PP(self, 0x1e4) = 0;
    if (PP(self, 0x8c))  operator_delete_(Explosion_dtor(PP(self, 0x8c)));
    PP(self, 0x8c) = 0;
    if (PP(self, 0x90))  operator_delete_(Explosion_dtor(PP(self, 0x90)));
    PP(self, 0x90) = 0;
    if (PP(self, 0x358)) operator_delete_(EaseInOutMatrix_dtor(PP(self, 0x358)));
    PP(self, 0x358) = 0;
    if (PP(self, 0x1b8)) {
        Array_releaseRepairBeams(PP(self, 0x1b8));
        if (PP(self, 0x1b8))
            operator_delete_(ArrayRB_dtor(PP(self, 0x1b8)));
    }
    PP(self, 0x1b8) = 0;
}
