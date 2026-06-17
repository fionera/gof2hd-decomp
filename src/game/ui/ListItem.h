#ifndef GOF2_LISTITEM_H
#define GOF2_LISTITEM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// A polymorphic entry in the various menu lists. Exactly one of its payload pointers is set by
// the matching constructor (text lines, Agent, BluePrint, Ship, Item, Mission or PendingProduct),
// plus up to two owned name Strings (title/subtitle) and a set of selection/slot flags.

class Agent;
class BluePrint;
class Item;
class Mission;
class PendingProduct;
class Ship;

class ListItem {
public:
    Array<AbyssEngine::String*>* lines;     // text lines
    Agent*                       agent;
    BluePrint*                   bluePrint;
    Ship*                        ship;
    // item / selectable / slot / buttonKind are also read by Hud.cpp and HangarList.cpp via
    // their legacy field_0xNN names; an anonymous union keeps both spellings available.
    union { Item* item; Item* field_0x10; };
    Mission*                     mission;
    PendingProduct*              pendingProduct;
    AbyssEngine::String*         name;       // owned String (title)
    AbyssEngine::String*         name2;      // owned String (subtitle)
    union { uint8_t selectable; uint8_t field_0x24; };
    union { int slot; int field_0x28; };     // slot index / medal tier (-1 sentinel)
    // itemId / inTabIndex are also read by Hud.cpp, HangarWindow.cpp and HangarList.cpp via
    // their legacy field_0xNN names; anonymous unions keep both spellings available.
    union { int itemId; int field_0x2c; };   // -1 sentinel
    union { int buttonKind; int field_0x30; }; // 0=sell, 1=move-to-cargo
    int                          imageIndex; // -1 sentinel / image index
    uint8_t                      textButton;
    union { int inTabIndex; int field_0x3c; }; // -1 sentinel / tab index
    union { int subTabIndex; int field_0x40; }; // -1 sentinel / tab index
    uint8_t                      text;       // text flag
    union { uint8_t craftable; uint8_t field_0x45; }; // blueprint craftable tint flag (read @HangarWindow render 0x1598e0)

    // Exactly one payload pointer is set per overload. The String overloads take the
    // engine String by pointer and deep-copy it into an owned title/subtitle String.
    ListItem(Agent* a);
    ListItem(Array<AbyssEngine::String*>* arr);
    ListItem(BluePrint* bp);
    ListItem(Item* it);
    ListItem(Mission* m);
    ListItem(PendingProduct* pp);
    ListItem(Ship* s);
    ListItem(const AbyssEngine::String* src);
    ListItem(const AbyssEngine::String* p1, const AbyssEngine::String* p2);
    ListItem(AbyssEngine::String* p1, AbyssEngine::String* p2);
    ListItem(AbyssEngine::String* src, bool b, int v);
    ListItem(const AbyssEngine::String* src, int v);
    ListItem(const ListItem& src);
    ListItem(int v);                                 // also the "slot" entry
    ListItem(int a, int b);
    ListItem(int a, int b, AbyssEngine::String* src);

    ~ListItem();

    bool    checkCredits();
    int     checkSlot();
    int     checkSort();
    // Tail helper of checkSort(): forwards the requested item sort to the player ship,
    // which answers whether a matching equipment slot is available.
    static int sortCmp(Ship* ship, int sort);
    int     getIndex();
    int     getNumLines();
    int     getPrice();
    void*   init();
    bool    isBluePrint();
    bool    isCargo();
    bool    isImage();
    bool    isItem();
    bool    isMission();
    bool    isMoveToCargoButton();
    bool    isPendingProduct();
    uint8_t isSelectable();
    bool    isSellButton();
    bool    isShip();
    bool    isSlot();
    bool    isTab();
    uint8_t isText();
    uint8_t isTextButton();
};
#endif
