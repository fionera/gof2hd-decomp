#include "class.h"

// PC-relative singleton holders.
extern void *const gAERandom __attribute__((visibility("hidden")));   // DAT_000b6100
extern void *const gStatus __attribute__((visibility("hidden")));     // DAT_000b6104
extern void *const gItemDb __attribute__((visibility("hidden")));     // DAT_000b6108

extern "C" {
void setActive(KIPlayer *self, int v);                        // 0x732f4
int AERandom_nextInt(void *rng, int bound);                  // 0x71848
void *Status_getShip(void);                                  // 0x71a58
int Ship_getFreeSpace(void *ship);                           // 0x722ec
void *Item_makeItem(int infoPtr);                            // 0x718c0
void Level_stealFriendCargo(void *level);                    // 0x73300
void *Status_getStanding(void);                              // 0x71b84
void Standing_applyStealCargo(void *standing, int faction);  // 0x7330c
int Item_getIndex(void *item);                               // 0x718d8
int Item_getAmount(void *item);                              // 0x7183c
int Ship_spaceAvailable(void *ship, int amount);             // 0x73318
void Status_crateCaptured(void *status, int amount);         // 0x73324
void Item_setUnsaleable(void *item, int v);                  // 0x73330
int Item_getType(void *item);                                // 0x718fc
void *Ship_getEquipment(void *ship);                         // 0x7333c
void Item_changeAmount(void *item, int amount);              // 0x71854
void Ship_addCargo(void *ship, void *item);                  // 0x72cdc
void Hud_catchCargo(void *hud, int index, int amount, int flagA, int flagB, int flagC, int flagD); // 0x72ce8 / 0x72cdc area
}

// KIPlayer::captureCrate(Hud* hud)
//   On a crate capture: rolls/limits the amount against ship free space, creates the item,
//   applies steal/standing penalties, moves the cargo into the ship's hold or equipment, bumps
//   the relevant statistics, and notifies the HUD.
extern "C" void KIPlayer_captureCrate(KIPlayer *self, void *hud)
{
    if ((unsigned)(self->f_88 - 3) < 2) {
        self->f_4c = 0;
        if (F<uint8_t>(self, 0x101) != 0)
            setActive(self, 0);
    }

    void *cargo = self->f_50;
    F<unsigned>(self, 0x78) = 0;
    if (cargo == 0)
        return;

    unsigned count = *(unsigned *)cargo;
    for (unsigned i = 0; i < count; i = i + 2) {
        int amount = *(int *)(*(char **)((char *)cargo + 4) + i * 4 + 4);
        if (amount < 1)
            continue;

        // randomise the captured amount unless we are in a "guaranteed" state.
        if ((unsigned)(self->f_88 - 3) >= 2)
            amount = AERandom_nextInt(*(void **)gAERandom, amount);

        void *status = *(void **)gStatus;

        // clamp to the ship's free space.
        Status_getShip();
        int free1 = Ship_getFreeSpace(Status_getShip());
        int amt = amount;
        if (free1 <= amount)
            amt = Ship_getFreeSpace(Status_getShip());
        if (amt < 1) {
            amount = 1;
        } else {
            int free2 = Ship_getFreeSpace(Status_getShip());
            if (free2 <= amount)
                amount = Ship_getFreeSpace(Status_getShip());
        }

        // resolve the item info and decrement the crate's remaining count.
        int itemId = *(int *)(*(char **)((char *)self->f_50 + 4) + i * 4);
        int infoPtr = *(int *)(*(char **)(*(char **)gItemDb + 4) + itemId * 4);
        void *item = Item_makeItem(infoPtr);
        int *slot = (int *)(*(char **)((char *)self->f_50 + 4) + i * 4);
        slot[1] = slot[1] - amount;
        if (item == 0)
            return;

        if (*(char *)((char *)self->f_4 + 0x5d) != 0)
            Level_stealFriendCargo(self->f_54);

        if (self->f_3c == 0)
            Standing_applyStealCargo(Status_getStanding(), self->f_28);

        // determine whether this is a special (illegal) cargo item.
        bool special = false;
        if (self->f_d0 != 0) {
            int idx = Item_getIndex(item);
            if (idx == 0x74)
                special = true;
            else
                special = (Item_getIndex(item) == 0x75);
        }

        void *ship = Status_getShip();
        int avail = Ship_spaceAvailable(ship, Item_getAmount(item));
        int hudIndex;
        int hudAmount;
        int flagA = special ? 1 : 0;
        int flagB;

        if (avail == 0) {
            if (special)
                self->f_68 = 1;
            hudIndex = Item_getIndex(item);
            hudAmount = Item_getAmount(item);
            flagB = 1;
            Hud_catchCargo(hud, hudIndex, hudAmount, flagA, 0, 0, flagB);
            return;
        }

        Status_crateCaptured(status, Item_getAmount(item));
        if (special)
            Item_setUnsaleable(item, 1);

        bool merged = false;
        if (Item_getType(item) == 1) {
            void *equip = Ship_getEquipment(Status_getShip());
            if (equip != 0) {
                unsigned ecount = *(unsigned *)equip;
                for (unsigned j = 0; j < ecount; j = j + 1) {
                    void *e = ((void **)(*(char **)((char *)equip + 4)))[j];
                    if (e != 0 && Item_getIndex(e) == Item_getIndex(item)) {
                        void *e2 = ((void **)(*(char **)((char *)equip + 4)))[j];
                        Item_changeAmount(e2, Item_getAmount(item));
                        merged = true;
                    }
                }
            }
        }
        if (!merged)
            Ship_addCargo(Status_getShip(), item);

        *(int *)((char *)self->f_54 + 0x1c) =
            Item_getAmount(item) + *(int *)((char *)self->f_54 + 0x1c);

        if (special) {
            F<uint8_t>(self, 0x69) = 1;
        } else if (self->f_28 == 9) {
            void *st = *(void **)gStatus;
            *(int *)((char *)st + 0xcc) = Item_getAmount(item) + *(int *)((char *)st + 0xcc);
        } else {
            int idx = Item_getIndex(item);
            if (idx >= 0x84 && Item_getIndex(item) < 0x9a) {
                void *st = *(void **)gStatus;
                char *tbl = *(char **)((char *)st + 0xac);
                *(char *)(*(char **)(tbl + 4) + Item_getIndex(item) - 0x84) = 1;
            }
        }

        hudIndex = Item_getIndex(item);
        hudAmount = Item_getAmount(item);
        Hud_catchCargo(hud, hudIndex, hudAmount, flagA, 0, 0, 0);
        return;
    }
}
