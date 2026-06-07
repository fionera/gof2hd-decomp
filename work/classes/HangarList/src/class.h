#ifndef WORK_CLASSES_HANGARLIST_SRC_CLASS_H
#define WORK_CLASSES_HANGARLIST_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Ship;
struct Item;
struct BluePrint;
struct PendingProduct;
struct ListItem;
struct String;
struct GameText;
struct Status;
struct Station;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t cap;
};

struct HangarList {
    HangarList();
    ~HangarList();

    void release();
    int init(Ship *ship, Array<Item *> *items, Array<Ship *> *ships,
             Array<BluePrint *> *blueprints);
    void initShipTab(Ship *ship);
    void initShopTab(Array<Item *> *items, Array<Ship *> *ships);
    void initBlueprintTab(Array<BluePrint *> *blueprints);
    void fillBuyList(ListItem *item);
    void fillIngredientsList(BluePrint *blueprint, bool flag);
    ListItem *getCurrentItem();
    ListItem *getCurrentItemAt(int index);
    Array<ListItem *> *getCurrentTabItems();
    uint32_t getCurrentLength();
};

template <class T>
static inline T &F(HangarList *self, unsigned off) {
    return *(T *)((char *)self + off);
}

template <class T>
static inline T &G(void *self, unsigned off) {
    return *(T *)((char *)self + off);
}

void *operator new(unsigned size);
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

#endif
