#include "class.h"

struct RadioMessage;
struct Station;

// PC-relative status/RNG holders + radio-table base pointers (each DAT_ resolves to a global
// table of {messageId, delay} pairs the original indexes into).
__attribute__((visibility("hidden"))) extern int  **g_crm_status;   // [DAT_000d59dc]
__attribute__((visibility("hidden"))) extern int   *g_crm_rngA;     // [DAT_000d59e0]
__attribute__((visibility("hidden"))) extern int   *g_crm_rngB;     // [DAT_000d59e8]
__attribute__((visibility("hidden"))) extern int   *g_crm_rngStorm; // [DAT_000d59e4]
__attribute__((visibility("hidden"))) extern int   *g_crm_counts8;  // [DAT_000d59f8] per-stage counts
__attribute__((visibility("hidden"))) extern int   *g_crm_table8;   // [DAT_000d59fc] {id,arg} table

extern "C" {
int  Status_getMission_crm();
int  Mission_isEmpty_crm();
int  Status_getStation_crm();
int  Station_getIndex_crm(Station *s);
int  AERandom_nextInt_crm(int rng);
void *Level_opnew_crm(unsigned size);
void ArrayRadio_ctor_crm(void *a);
void *ArrayRadio_dtor_crm(void *a);
void operator_delete_crm(void *p);
void RadioMessage_ctor_crm(RadioMessage *m, int id, int arg, int kind, int delay);
void ArrayAdd_Radio_crm(RadioMessage *m, void *a);
// Hands the finished radio-message queue to the player's comm system (vtable slot at player+0x18).
void Level_crm_dispatch(int ego, void *queue);
}

// Level::createRadioMessage(int type, int sub) — builds a context-appropriate sequence of radio
// chatter lines for the current orbit/mission and queues them on the player's comms.
extern "C" void Level_createRadioMessage(Level *thisptr, int type, int sub)
{
    char *self = (char *)thisptr;
    unsigned r2 = (unsigned)sub;

    if (*(int *)(self + 0xf0) == 0 || *(int *)(*(int *)(self + 0xf0) + 0x18) == 0)
        return;

    int **statusHolder = g_crm_status;
    Status_getMission_crm();
    if (Mission_isEmpty_crm() == 0)
        return;

    // fresh message queue.
    if (*(void **)(self + 0x114) != 0) {
        operator_delete_crm(ArrayRadio_dtor_crm(*(void **)(self + 0x114)));
        *(int *)(self + 0x114) = 0;
    }
    void *queue = Level_opnew_crm(0xc);
    ArrayRadio_ctor_crm(queue);
    *(void **)(self + 0x114) = queue;

    // resolve the speaker portrait from the sub-parameter.
    int speaker;
    if (r2 == 0)       speaker = 0x40;
    else if (r2 == 2)  speaker = 0x41;
    else if (r2 == 3)  speaker = 0x15;
    else               speaker = (r2 == 8) ? 9 : 0x3f;

    int extraDelay = 0;
    unsigned id = 0x1ba;
    bool aborted = false;
    bool builtInline = false; // case already pushed its own messages

    switch (type) {
    case 0:
    case 1: {
        // local-station hail: skip if you're docked at the relevant station.
        int *stations = *(int **)(*statusHolder[0] + 0x90);
        bool atStation = false;
        if (stations != 0) {
            for (unsigned k = 0; k < (unsigned)*stations; k = k + 1) {
                int sidx = *(int *)(((int *)stations[1])[k]);
                Station *st = (Station *)Status_getStation_crm();
                if (sidx == Station_getIndex_crm(st)) { atStation = true; break; }
            }
        }
        if (atStation) { aborted = true; break; }
        int base = (type == 0) ? 0x1aa : 0x1ad;
        id = AERandom_nextInt_crm(*g_crm_rngA) + base;
        break;
    }
    case 3:
        *(char *)(self + 0x1b0) = 1;
        id = AERandom_nextInt_crm(*g_crm_rngStorm) + 0x1b3;
        break;
    case 4:
        *(char *)(self + 0x68) = 1;
        id = AERandom_nextInt_crm(*g_crm_rngStorm) + 0x1b6;
        break;
    case 5:  id = 0x1bb; extraDelay = 0; break;
    case 6:  id = 0x1bc; extraDelay = 0; break;
    case 9:  id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c1; break;
    case 10: id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c3; break;
    case 0xb: id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c5; break;
    case 0xc: id = 0x1c5; extraDelay = 0; break;
    case 0xd: id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1c7; break;
    case 0xe: {
        int *st = *(int **)(*statusHolder[0] + 0x90);
        id = 0x88f;
        if (st != 0) {
            for (int k = 0; k != *st; k = k + 1)
                if (-1 < *(int *)(st[1] + k * 4)) id = id - 2;
        }
        break;
    }
    case 0xf: {
        int *st = *(int **)(*statusHolder[0] + 0x90);
        id = 0x88e;
        if (st != 0) {
            for (int k = 0; k != *st; k = k + 1)
                if (-1 < *(int *)(st[1] + k * 4)) id = id - 2;
            if (id - 0x889 < 5) { extraDelay = 0; break; }
        }
        aborted = true;
        break;
    }
    case 8: {
        int stage = AERandom_nextInt_crm(*g_crm_rngB);
        int off = 0;
        for (int k = 0; k < stage; k = k + 1)
            off = off + ((int *)g_crm_counts8)[k];
        int cnt = ((int *)g_crm_counts8)[stage];
        int *tbl = (int *)((char *)g_crm_table8 + off * 8);
        for (int k = 0; k < cnt; k = k + 1) {
            int delay = (k == 0) ? 5000 : (k - 1);
            int arg = tbl[k * 2];
            RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
            int kind = (k == 0) ? 5 : 6;
            RadioMessage_ctor_crm(m, tbl[k * 2 + 1], arg, kind, delay);
            ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        }
        builtInline = true;
        break;
    }
    case 0x13: {
        RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
        int rng = *g_crm_rngStorm;
        RadioMessage_ctor_crm(m, AERandom_nextInt_crm(rng) + 0xaf4, 0, 5, 0x5dc);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, AERandom_nextInt_crm(rng) + 0xafa, 0, 6, 0);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        builtInline = true;
        break;
    }
    case 0x15: id = 0xc54; extraDelay = 0; break;
    case 0x16: id = 0xc55; extraDelay = 0; break;
    case 0x17: id = 0xc56; extraDelay = 0; break;
    case 0x18: id = 0xc57; extraDelay = 0; break;
    case 0x19: id = 0xc58; extraDelay = 0; break;
    case 0x1a: id = 0xc59; extraDelay = 0; break;
    case 0x1b: {
        RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, r2 * 2 + 0xc60, 6, 5, 0x5dc);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, r2 * 2 + 0xc61, 0, 6, 0);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
        builtInline = true;
        break;
    }
    case 0x1c:
        speaker = 0x26;
        id = r2;
        extraDelay = 0;
        break;
    case 7:
    default:
        if (type == 7 || type == 0xc || type == 0xe) {
            // fall through to single-message emit below using current id/speaker.
        } else {
            id = AERandom_nextInt_crm(*g_crm_rngB) + 0x1bd;
        }
        break;
    }

    if (aborted) {
        if (*(void **)(self + 0x114) != 0)
            operator_delete_crm(ArrayRadio_dtor_crm(*(void **)(self + 0x114)));
        int ego = *(int *)(self + 0xf0);
        *(int *)(self + 0x114) = 0;
        Level_crm_dispatch(*(int *)(ego + 0x18), 0);
        return;
    }

    if (!builtInline) {
        RadioMessage *m = (RadioMessage *)Level_opnew_crm(0x28);
        RadioMessage_ctor_crm(m, id, speaker, 5, extraDelay);
        ArrayAdd_Radio_crm(m, *(void **)(self + 0x114));
    }

    int ego = *(int *)(self + 0xf0);
    Level_crm_dispatch(*(int *)(ego + 0x18), *(void **)(self + 0x114));
}
