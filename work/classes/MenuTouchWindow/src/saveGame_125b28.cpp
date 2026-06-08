#include "class.h"

extern "C" void *operator_new(uint32_t);
extern "C" void operator_delete(void *p);
extern "C" void _mtw_RecordHandler_ctor(void *rh);
extern "C" void _mtw_RecordHandler_recordStoreWrite(void *rh, int slot);
extern "C" void _mtw_RecordHandler_recordStoreWritePreview(void *rh, int slot);
extern "C" void *_mtw_RecordHandler_recordStoreReadPreview(void *rh);
extern "C" void *_mtw_RecordHandler_dtor(void *rh);
extern "C" void *_mtw_GameRecord_dtor(void *gr);
extern "C" void _mtw_createRecordButtons(void *self, bool b);
extern "C" void *_mtw_GameText_getText(void *gt, int id);
extern "C" void _mtw_ChoiceWindow_set(void *cw, void *s, bool b);
extern void *const gSaveGameText __attribute__((visibility("hidden")));

// MenuTouchWindow::saveGame(int slot)
struct MenuTouchWindow { void saveGame(int slot); };
void MenuTouchWindow::saveGame(int slot)
{
    void *rh = operator_new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    _mtw_RecordHandler_recordStoreWrite(rh, slot);
    _mtw_RecordHandler_recordStoreWritePreview(rh, slot);

    int *arr = (int *)i32(pp(this, 0xbc), 4);
    void *rec = *(void **)(arr + slot);
    int *cell;
    if (rec == 0) {
        cell = arr + slot;
    } else {
        operator_delete(_mtw_GameRecord_dtor(rec));
        cell = (int *)(i32(pp(this, 0xbc), 4) + slot * 4);
    }
    *cell = 0;

    void *preview = _mtw_RecordHandler_recordStoreReadPreview(rh);
    *(void **)(i32(pp(this, 0xbc), 4) + slot * 4) = preview;
    operator_delete(_mtw_RecordHandler_dtor(rh));

    _mtw_createRecordButtons(this, true);
    void *cw = pp(this, 0x104);
    void *s = _mtw_GameText_getText(*(void **)gSaveGameText, 0x32);
    _mtw_ChoiceWindow_set(cw, s, false);
    u8(this, 0x173) = 0;
    u8(this, 0x170) = 1;
}
