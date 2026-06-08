#include "class.h"

extern "C" void *operator_new(uint32_t);
extern "C" void operator_delete(void *p);
extern "C" void *_mtw_Array_GameRecord_dtor(void *p);
extern "C" void _mtw_RecordHandler_ctor(void *rh);
extern "C" void *_mtw_RecordHandler_readAllPreviewRecords(void *rh);
extern "C" void _mtw_RecordHandler_dtor(void *rh);

// PC-relative singleton holders used to lay out the scroll geometry.
extern void *const gPrevScreenW __attribute__((visibility("hidden")));   // *holder -> screen metrics, [0]=width
extern void *const gPrevLayout __attribute__((visibility("hidden")));    // *holder -> layout, +0x10/0xc/0x20/0x24/0x70
extern void *const gPrevRowCnt __attribute__((visibility("hidden")));    // *holder -> [0]=row count
typedef void (*RefreshFn)();
extern void *const gPrevRefreshThunk __attribute__((visibility("hidden")));

// MenuTouchWindow::loadPreviewRecords()
struct MenuTouchWindow { void loadPreviewRecords(); };
void MenuTouchWindow::loadPreviewRecords()
{
    i32(this, 0x194) = 0;
    // two zeroed Vector4 blocks at +0x215 and +0x20c
    for (int o = 0x215; o < 0x215 + 16; o += 4) i32(this, o) = 0;
    for (int o = 0x20c; o < 0x20c + 16; o += 4) i32(this, o) = 0;

    int *metrics = (int *)*(void **)gPrevScreenW;
    int *layout = (int *)*(void **)gPrevLayout;
    int *rowObj = (int *)*(void **)gPrevRowCnt;

    // content height = width - margins (+0x10,+0xc,+0x20,+0x24 of layout)
    i32(this, 0x228) = (((metrics[0] - layout[3]) - layout[2]) - layout[8]) - layout[9];
    // page height = rowCount * (rowHeight + this->0x1b4)
    i32(this, 0x22c) = rowObj[0] * (layout[0x1c] + i32(this, 0x1b4));

    void *rec = pp(this, 0xbc);
    if (rec != 0) {
        operator_delete(_mtw_Array_GameRecord_dtor(rec));
        i32(this, 0xbc) = 0;
    }
    void *rh = operator_new(0x2c);
    _mtw_RecordHandler_ctor(rh);
    pp(this, 0xbc) = _mtw_RecordHandler_readAllPreviewRecords(rh);
    _mtw_RecordHandler_dtor(rh);

    RefreshFn refresh = *(RefreshFn *)((char *)gPrevRefreshThunk);
    refresh();
}
