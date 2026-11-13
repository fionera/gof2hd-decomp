# MGame::OnUpdate — volatile-store trigger and two near-misses, wave 191b / w13565

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13565`,
branch `wave191b-w13565`.  Target `_ZN5MGame8OnUpdateEv` stays at
**raw 10 / aligned 112 / 11,440 B, insn 4003:4002**; gate green
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN).

Follows `mgame-onupdate-wave191b-w13565.md` (the r9-trigger note) and
`mgame-onupdate-wave191-w13555.md`.  This note adds the exact trigger shape
and two new near-miss families.

## 1. The r9 trigger is a volatile *store* of the status pointer before nextInt

All shapes below replace only the m122 body; everything else is HEAD.

| shape | raw | size |
|---|---:|---:|
| **HEAD anchor** (`Status *st=Globals::status; nextInt; Status *volatile dummy=st;` ccm via `st`) | **10** | 11440 |
| `Status *volatile m = Globals::status;` then `nextInt` (ccm via `Globals::status`) | 361 | 11440 |
| same, store *after* nextInt | 5212 | 11396 |
| same, store after ccm / after createRadioMessage / at end | 5205 / 5211 / 5211 | 11396 |
| `Status *m = Globals::status; volatile Status *d = m;` before nextInt | 5196 | 11392 |
| `volatile Status *m = Globals::status;` (volatile pointee) | 5196 | 11392 |
| `Status *m = *reinterpret_cast<Status *volatile *>(&Globals::status);` (volatile load) | 5205 | 11396 |
| `volatile unsigned m = (unsigned)(uintptr_t)Globals::status;` before nextInt | 361 | 11440 |
| `Level *volatile m = this->level;` / `PlayerEgo *volatile m = this->player;` | 5201 / 5201 | 11400 / 11400 |
| `uint8_t *volatile m = hints;` | 5195 | 11404 |

So the trigger is **not** "any value live across nextInt": it is a volatile
store of the `@Globals::status` pointer value placed *before* the nextInt call,
with the pointer load GVN'd back to the m23 `field_0x100` load.  Moving the
store past nextInt drops back to the 11,392 B plain build.  A volatile *load*
(`*reinterpret_cast<Status *volatile *>(...)`) also drops to plain.

## 2. Near-miss A: `sac` — m122 ccm shape right, radioBase/level wrong

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
volatile unsigned dummy = static_cast<unsigned>(reinterpret_cast<intptr_t>(st));
Level *level = this->level;
level->createRadioMessage(28, radioIndex + radioBase);
```

raw **19**, size 11440.  `mov r4,r0` and `ldr.w r0,[r9]` are byte-exact; the
pointer is in r5 (m23 `ldr.w r5,[r9]`, 2 B off) and the store lands between
`movw` and `cmp`, which pushes radioBase to r1 and level to r0 and folds
`add r2,r4; mov r0,r1` into `adds r2,r1,r4` (17 B run at `0x17f19a`).
Moving the store after `Level *level` or after the call is worse
(342 / 898).  `volatile int` and `volatile uintptr_t` are identical; a
pointer store (`volatile Status *`) collapses to plain.

## 3. Near-miss B: `st_before_pt` — m23 and m122 both exact, m23 branch pays

Declare `Status *st = Globals::status;` before the m23 `if`, use
`st->getPlayingTime()` in the m23 condition, and
`volatile Status *dummy = st;` at the end of the m122 body: raw **41**,
size 11440, with m23 (`ldr.w r2,[r9]; ldr r4,[pc]; ldrd r2,r3,[r2,#256]`)
and m122 (`mov r4,r0; ldr.w r0,[r9]; ... add r2,r4`) both byte-exact.
The whole residual is one 41-byte run at `0x17e093..0x17e0bc`: the m23
branch's `getPlayingTime` receiver becomes `mov r0,r5` instead of
`ldr.w r0,[r9]` (the anchor is live across the duplicated m23 check).

## 4. Conclusion

The m23 `ldrd r2,r3,[r2,#256]` needs the status pointer in r2 *and* dead
after the ldrd; the r9 pin needs that same pointer kept live to a volatile
store.  Every shape that satisfies the second keeps the pointer live at the
ldrd (scv: `ldrd r3,r5`) or moves the load into a callee-saved register
(HEAD: `ldr.w r4,[r9]`).  HEAD's 10 bytes remain the local optimum.

## 5. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j2      # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13565/`
(`q.py`, `bdiff.py`, `c20.py`..`c37.py`, `body_*.txt`, `MGame.cpp.head`).
