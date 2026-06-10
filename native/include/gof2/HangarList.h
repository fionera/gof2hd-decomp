#ifndef GOF2_HANGARLIST_H
#define GOF2_HANGARLIST_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Ship;
struct Item;
struct BluePrint;
struct PendingProduct;
struct ListItem;
// String comes from gof2/common.h (AbyssEngine::String); do NOT redeclare it here.
struct GameText;
struct Status;
struct Station;









void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" void *operator_new(unsigned size);
extern "C" void operator_delete(void *ptr);

extern "C" void Array_ListItem_ctor(Array<ListItem *> *array);
extern "C" void *Array_ListItem_dtor(Array<ListItem *> *array);
extern "C" void Array_ArrayListItem_ctor(Array<Array<ListItem *> *> *array);
extern "C" void *Array_ArrayListItem_dtor(Array<Array<ListItem *> *> *array);
extern "C" void Array_Item_ctor(Array<Item *> *array);
extern "C" void *Array_Item_dtor(Array<Item *> *array);

extern "C" void ArrayReleaseClasses_ListItem(Array<ListItem *> *array);
extern "C" void ArrayReleaseClasses_ArrayListItem(Array<Array<ListItem *> *> *array);
extern "C" void ArraySetLength_ListItem(uint32_t length, Array<ListItem *> *array);
extern "C" void ArraySetLength_ArrayListItem(uint32_t length,
                                              Array<Array<ListItem *> *> *array);
extern "C" void ArrayAdd_Item(Item *item, Array<Item *> *array);
extern "C" void ArrayAdd_ListItem(ListItem *item, Array<ListItem *> *array);

extern "C" String *GameText_getText(GameText *texts, int id);

extern "C" ListItem *ListItem_ctor_String(ListItem *self, String *text);
extern "C" ListItem *ListItem_ctor_String_int(ListItem *self, String *text, int kind);
extern "C" ListItem *ListItem_ctor_Ship(ListItem *self, Ship *ship);
extern "C" ListItem *ListItem_ctor_Item(ListItem *self, Item *item);
extern "C" ListItem *ListItem_ctor_Slot(ListItem *self, uint32_t kind);
extern "C" ListItem *ListItem_ctor_BluePrint(ListItem *self, BluePrint *blueprint);
extern "C" ListItem *ListItem_ctor_PendingProduct(ListItem *self,
                                                  PendingProduct *product);
extern "C" ListItem *ListItem_ctor_ListItem(ListItem *self, ListItem *item);
extern "C" ListItem *ListItem_ctor_TextButton(ListItem *self, String *text,
                                               bool enabled, int mode);
extern "C" bool ListItem_isShip(ListItem *self);
extern "C" bool ListItem_isSlot(ListItem *self);

extern "C" int Item_getType(Item *item);
extern "C" int Item_getIndex(Item *item);
extern "C" void Item_setBlueprintAmount(Item *item, int amount);

extern "C" Array<Item *> *Ship_getEquipment(Ship *ship);
extern "C" Array<Item *> *Ship_getEquipmentByType(Ship *ship, uint32_t kind);
extern "C" Array<Item *> *Ship_getCargo(Ship *ship);
extern "C" int Ship_getSlots(Ship *ship, uint32_t kind);
extern "C" int Ship_getSlotTypes(Ship *ship);
extern "C" void Ship_adjustPrice(Ship *ship);

extern "C" Ship *Status_getShip(Status *status);
extern "C" Station *Status_getStation(Status *status);
extern "C" Array<PendingProduct *> *Status_getPendingProducts(Status *status);
extern "C" Array<Ship *> *Station_getShips(Station *station);

extern "C" Array<int32_t> *BluePrint_getIngredientList(BluePrint *blueprint);
extern "C" int BluePrint_getIndex(BluePrint *blueprint);
extern "C" int BluePrint_isUnlocked(BluePrint *blueprint);

extern "C" Status *g_HangarList_status;
extern "C" GameText **g_HangarList_gameText;
extern "C" Array<Item *> *g_HangarList_items;

// The hangar/shop/blueprint list model. field_0x0 holds an Array of per-tab Arrays of ListItem*;
// field_0x4 is the currently selected tab index; field_0x8 caches the current ListItem*.
struct HangarList {
    Array<Array<ListItem *> *>* field_0x0;  // +0x0  per-tab item lists
    uint32_t                    field_0x4;  // +0x4  current tab index
    ListItem*                   field_0x8;  // +0x8  current item

    HangarList();
    ~HangarList();

    void release();
    uint32_t getCurrentLength();
    Array<ListItem *> *getCurrentTabItems();
    ListItem *getCurrentItem();
    ListItem *getCurrentItemAt(int index);
    void initBlueprintTab(Array<BluePrint *> *blueprints);
    void fillIngredientsList(BluePrint *blueprint, bool flag);
    void fillBuyList(ListItem *item);
    void initShipTab(Ship *ship);
    void initShopTab(Array<Item *> *shopItems, Array<Ship *> *ships);
    int  init(Ship *ship, Array<Item *> *items, Array<Ship *> *ships,
              Array<BluePrint *> *blueprints);
};
#endif
