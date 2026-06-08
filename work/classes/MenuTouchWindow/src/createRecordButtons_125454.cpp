#include "class.h"

// MenuTouchWindow::createRecordButtons(bool inSaveMode).
// Rebuilds the per-slot text rows (Array<Array<String*>>) shown in the load/save list and
// the OK/back buttons. r0=this, r1=inSaveMode.

extern "C" void *operator_new(uint32_t);
extern "C" void operator_delete(void *p);
extern "C" void _mtw_ArrayReleaseClasses_Str(void *arr);          // Array<String*> release
extern "C" void _mtw_ArrayReleaseClasses_StrArr(void *arr);       // Array<Array<String*>*> release
extern "C" void *_mtw_Array_Str_dtor(void *p);
extern "C" void *_mtw_Array_StrArr_dtor(void *p);
extern "C" void _mtw_Array_StrArr_ctor(void *p);
extern "C" void _mtw_Array_Str_ctor(void *p);
extern "C" void _mtw_ArraySetLength_StrArr(int n, void *arr);
extern "C" void _mtw_ArraySetLength_Str(int n, void *arr);
extern "C" void _mtw_String_ctor_empty(void *s);
extern "C" void _mtw_String_ctor_copy(void *s, void *src, bool b);
extern "C" void _mtw_String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void _mtw_String_ctor_from(void *s, int src);
extern "C" void _mtw_String_dtor(void *s);
extern "C" void *_mtw_String_op_plus(void *out, void *rhs);        // operator+
extern "C" void _mtw_Globals_longToTimeStringNoSeconds(long long t, void *out);
extern "C" void _mtw_Layout_formatCredits(int out);
extern "C" void *_mtw_GameText_getText(void *gt, int id);
extern "C" void *_mtw_TouchButton_dtor(void *p);
extern "C" void _mtw_TouchButton_ctor7(void *btn, void *label, int a, int x, int y, char type);
extern "C" void _mtw_TouchButton_ctorFull(void *btn, void *label, int a, int x, int y, int w,
                                          char type, char id);
extern "C" void _mtw_TouchButton_setPosition(void *btn, int x, int y);

// PC-relative holders.
extern void *const gCrbRowCount __attribute__((visibility("hidden")));   // *holder -> [0] = row count
extern void *const gCrbTimeVal  __attribute__((visibility("hidden")));   // *holder -> time/playtime value
extern void *const gCrbGameText __attribute__((visibility("hidden")));   // *holder -> GameText
extern const char gCrbDash[]    __attribute__((visibility("hidden")));   // "-" placeholder
extern void *const gCrbLayout   __attribute__((visibility("hidden")));   // *holder -> layout obj
extern void *const gCrbScreenW  __attribute__((visibility("hidden")));   // *holder -> [0] = screen width
extern void *const gCrbBackFlag __attribute__((visibility("hidden")));   // *holder -> [0]: byte, use 7-button form

struct MenuTouchWindow { void createRecordButtons(bool inSaveMode); };
void MenuTouchWindow::createRecordButtons(bool inSaveMode)
{
    void *self = this;

    // Tear down any existing 2D row array.
    void *rows = pp(self, 0x100);
    if (rows != 0) {
        for (uint32_t i = 0; i < *(uint32_t *)rows; i++) {
            void *row = ((void **)i32(rows, 4))[i];
            if (row != 0) {
                _mtw_ArrayReleaseClasses_Str(row);
                int base = i32(pp(self, 0x100), 4);
                void *r2 = *(void **)(base + i * 4);
                int *cell;
                if (r2 == 0) cell = (int *)(base + i * 4);
                else { operator_delete(_mtw_Array_Str_dtor(r2)); cell = (int *)(i32(pp(self, 0x100), 4) + i * 4); }
                *cell = 0;
                rows = pp(self, 0x100);
            }
        }
        _mtw_ArrayReleaseClasses_StrArr(rows);
        if (pp(self, 0x100) != 0) operator_delete(_mtw_Array_StrArr_dtor(pp(self, 0x100)));
        pp(self, 0x100) = 0;
    }

    void *outer = operator_new(0xc);
    _mtw_Array_StrArr_ctor(outer);
    pp(self, 0x100) = outer;
    int rowCount = *(int *)*(void **)gCrbRowCount;
    _mtw_ArraySetLength_StrArr(rowCount, outer);

    void *timeHolder = *(void **)gCrbTimeVal;
    void *gtHolder   = *(void **)gCrbGameText;

    for (int i = 0; i < rowCount; i++) {
        void *row = operator_new(0xc);
        _mtw_Array_Str_ctor(row);
        *(void **)(i32(pp(self, 0x100), 4) + i * 4) = row;
        _mtw_ArraySetLength_Str(6, *(void **)(i32(pp(self, 0x100), 4) + i * 4));

        char s48[12]; _mtw_String_ctor_empty(s48);
        char s54[12];

        void *rec = pp(self, 0xbc);
        bool empty = (rec == 0) || (*(int *)(i32(rec, 4) + i * 4) == 0);

        if (empty) {
            _mtw_String_ctor_empty(s54);
            _mtw_Globals_longToTimeStringNoSeconds((long long)(int)(long)timeHolder, 0);
            void *e;
            int *rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4);

            e = operator_new(0xc); _mtw_String_ctor_copy(e, s54, false); ((void **)rowData)[0] = e;
            e = operator_new(0xc); _mtw_String_ctor_copy(e, _mtw_GameText_getText(*(void **)gtHolder, 0x1e6), false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[1] = e;

            e = operator_new(0xc);
            if (i == 0) _mtw_String_ctor_copy(e, _mtw_GameText_getText(*(void **)gtHolder, 0x1e6), false);
            else _mtw_String_ctor_cstr(e, gCrbDash, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[2] = e;

            e = operator_new(0xc); _mtw_String_ctor_cstr(e, gCrbDash, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[3] = e;

            e = operator_new(0xc); _mtw_String_ctor_cstr(e, gCrbDash, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[4] = e;

            e = operator_new(0xc); _mtw_String_ctor_cstr(e, gCrbDash, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[5] = e;
        } else {
            _mtw_String_ctor_empty(s54);
            int slot = *(int *)(i32(rec, 4) + i * 4);
            _mtw_Globals_longToTimeStringNoSeconds((long long)slot, *(void **)(slot + 0x10));
            void *e;
            int *rowData;

            e = operator_new(0xc); _mtw_String_ctor_copy(e, s54, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[0] = e;

            e = operator_new(0xc);
            _mtw_String_ctor_copy(e, (void *)(*(int *)(i32(pp(self, 0xbc), 4) + i * 4) + 0x194), false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[1] = e;

            e = operator_new(0xc);
            if (i == 0) _mtw_String_ctor_copy(e, _mtw_GameText_getText(*(void **)gtHolder, 0x1e6), false);
            else _mtw_String_ctor_cstr(e, gCrbDash, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[2] = e;

            void *credits = operator_new(0xc);
            _mtw_Layout_formatCredits((int)credits);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[3] = credits;

            // build "Kills: " + value string
            void *combined = operator_new(0xc);
            void *label = _mtw_GameText_getText(*(void **)gtHolder, 0x141);
            char s6c[12]; _mtw_String_ctor_cstr(s6c, gCrbDash, false);
            char s60[12]; _mtw_String_op_plus(s60, label);
            char s78[12]; _mtw_String_ctor_from(s78, *(int *)(i32(pp(self, 0xbc), 4) + i * 4) + 0x20);
            _mtw_String_op_plus(combined, s60);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[4] = combined;
            _mtw_String_dtor(s78);
            _mtw_String_dtor(s60);
            _mtw_String_dtor(s6c);

            e = operator_new(0xc);
            float rank = *(float *)(*(int *)(i32(pp(self, 0xbc), 4) + i * 4) + 0x11c);
            void *rankTxt;
            if (rank <= 0.0f) rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x207);
            else if (rank <= 0.5f) rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x207);
            else if (rank <= 1.0f) rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x207);
            else rankTxt = _mtw_GameText_getText(*(void **)gtHolder, 0x19);
            _mtw_String_ctor_copy(e, rankTxt, false);
            rowData = (int *)i32(*(void **)(i32(pp(self, 0x100), 4) + i * 4), 4); ((void **)rowData)[5] = e;
        }

        _mtw_String_dtor(s54);
        _mtw_String_dtor(s48);
    }

    // OK button (+0xc4)
    if (pp(self, 0xc4) != 0) { operator_delete(_mtw_TouchButton_dtor(pp(self, 0xc4))); pp(self, 0xc4) = 0; }
    int *layout = (int *)*(void **)gCrbLayout;
    char backForm = *(char *)*(void **)gCrbBackFlag;
    int extra = layout[0x42]; // [0x108]
    void *okBtn = operator_new(0xc8);
    int screenW = *(int *)*(void **)gCrbScreenW;
    int x = (screenW - i32(self, 0x198)) - layout[0xa]; // -[0x28]
    int y = layout[8] + layout[3] + extra;              // [0x20]+[0xc]
    void *okLabel = _mtw_GameText_getText(*(void **)gtHolder, backForm ? 0x1fa : 0x1f9);
    if (backForm)
        _mtw_TouchButton_ctor7(okBtn, okLabel, 7, x, y, 0x12);
    else
        _mtw_TouchButton_ctorFull(okBtn, okLabel, 0, x, y, layout[0xa9] /*+0x2a4*/, 0x12, 0x04);
    pp(self, 0xc4) = okBtn;

    _mtw_TouchButton_setPosition(okBtn,
        (screenW - i32(self, 0x198)) - layout[0xa],
        (layout[0x1c] + i32(self, 0x1b4)) * i32(self, 0x18c) + layout[3] + extra + layout[8] + i32(self, 0x194));

    // back button (+0xc8)
    if (pp(self, 0xc8) != 0) { operator_delete(_mtw_TouchButton_dtor(pp(self, 0xc8))); pp(self, 0xc8) = 0; }
    void *backBtn = operator_new(0xc8);
    void *backLabel = _mtw_GameText_getText(*(void **)gtHolder, 0x41);
    _mtw_TouchButton_ctor7(backBtn, backLabel, 7,
        (screenW - i32(self, 0x198)) - layout[0xa],
        layout[3] + extra + layout[8], 0x12);
    pp(self, 0xc8) = backBtn;
}
