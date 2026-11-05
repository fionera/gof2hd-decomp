# `Generator::createMission` — wave 119 (w6835): guard-removal / `volatile amount` / null-block relocation sweep, no source win

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows; ours **168 B**, `cslen = 147`, 25 rows).

Status: **research note, no source change landed.**  Master keeps the tuned
**83.6088 % / 1996 B / 168 B record** shape (frame 112).

```
before (HEAD):  83.6088 %  1996 B  .ARM.extab 156308 B (orig 156312)  record 168  frame 112
after:          83.6088 %  1996 B  .ARM.extab 156308 B                 record 168  frame 112
```

Gate at the start and end of this pass: `RATCHET PASS`, `byte 4505`,
`linked 4505`, `data 843/843`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, file size 2,187,328 B.

Scratch (not part of the repo): `/dev/shm/w6835/` (`head.cpp` pristine HEAD,
`h.sh` build+score+record, `score.py`, `extab.py`, `diffins.py`, `region.py`,
`batch.py`, `sweep.py`, `run*.py` variant drivers).

---

## 1. Where the current 16.4 % delta lives

`verify.reconcile` keeps intra-function branch targets literal, so any block
that moves costs *every* branch to it.  The HEAD diff (`diffins.py`) is
dominated by four clusters:

1. **Frame 112 vs 96** (HEAD carries `volatile double padA` at `sp+96` and the
   `amount` alloca at `sp+92`; original frame is 96 with `amount` reusing
   `targetStation`'s `sp+28` slot).  Every `sub sp`, `[sp,#N]`, canary load and
   pc-relative literal offset differs; ~15 instructions.
2. **`pick == 12` rotation.**  Original `972b2 bne.n 972ce` (then-arm is the
   fallthrough); HEAD `972b2 beq.n 972bc` (else-arm fallthrough).  `expect(0)`
   is worth 2.75 points elsewhere, so the rotation cannot be fixed alone.
3. **Shared `type` store.**  HEAD `97354 str r0,[sp,#36]` (chained after the
   offer arm, `bb.32`), original `973f8 str r0,[sp,#36]` (chained after the
   `type = 13` arm, `bb.48`).  This is the MBP `hasBetterLayoutPredecessor`
   decision analysed in `generator-createmission-wave119-w6816.md`:
   `freq(bb.48) >= freq(bb.32)/4` is required; measured ratio 0.0155.
4. **Cold `getRoutes()==nullptr` block** (`bb.50`).  HEAD places it at
   `973f8..9740a`, immediately before `getName` (`973dc`), i.e. before the
   first invoke try-range, so its ordinary `nextInt(2)` is absorbed into the
   `(0, 606, 0, 0)` gap row.  The original places it at `97586` between two
   invoke try-ranges, producing the extra `(968, 32, 0, 0)` gap row (+4 B).

The literal pool is already identical and at the same address
(`0x9795c`: `0x42be0000`, `0x44960000`, `0x45abe000`), and the call multiset is
identical to the original (84 `blx`), so the remaining delta is pure
block placement + register allocation.

## 2. Removing the `(uint32_t) type < 15` guard (the w6816 lead)

`type` is 0..15, and `(1u << 15) & 0x4801 == 0`, so
`if (((1U << type) & 0x4801U) != 0)` is semantically identical to the guarded
form.  The original *does* emit the guard (`97358 cmp r1,#14; 9735a bhi`), so
removing it changes codegen:

| variant | pct | usize | record | note |
|---|---:|---:|---:|---|
| HEAD | **83.6088** | 1996 | 168 | |
| `expect(mask,1)` only (guard deleted) | 65.5125 | 1956 | 168 | store moves to `97412`, `type` register-resident |
| plain `if (mask)` (guard deleted) | 65.5125 | 1956 | 168 | |
| `expect(mask,0)` (guard deleted) | 59.0278 | 1972 | 168 | |
| `expect(type<15,1) && expect(mask,1)` | 65.5602 | 1960 | 168 | |
| `expect(type<15,1) && expect(mask,1)`, no padA | 59.7510 | 1956 | 168 | |
| `...` + plain `amount = 0` | 60.8033 | 1952 | 168 | |
| `...` + plain amount, no padA | 63.4483 | 1964 | 168 | |
| `...` + `volatile int padA` | 65.1903 | 1952 | 168 | |
| `...` + `volatile float padA` | 65.8368 | 1956 | 168 | |

Deleting the guard does move the shared store after `difficulty`
(`97412 str r5,[sp,#36]` in the rebuilt image), but the cold null block stays
at `973d8` (before `getName` at `973ee`), so the record stays 168.  The
predicted "moves the type13 store -> 172 B row" does not materialise.

Mask-expression spellings that keep the guard all measure the same or worse:
`type != 15` 80.1101/2000/168, `(uint32_t)type <= 14` 83.6088/1996/168,
`mask && type<15` 65.4219/1960/168, `bit` 65.5125/1956/168.

## 3. Lowering `freq(bb.32)` instead of raising `freq(bb.48)`

`bb.32` is the `getOffer() == 5` arm (50 %).  Hinting it cold moves the store
but also changes the layout:

| variant | pct | usize | record |
|---|---:|---:|---:|
| `expect(getOffer()==5, 0)` | 63.8947 | 1956 | 168 |
| `expect(getOffer()==5, 1)` | 40.0278 | 2020 | 168 |
| `expect(getOffer()==5, 0)`, no padA | 59.0187 | 1960 | 168 |
| `expect(getOffer()==5, 0)`, plain amount | 60.2076 | 1956 | 168 |

## 4. `volatile int amount` (faithful memory-resident amount)

The original keeps `amount` in memory and reuses `targetStation`'s `sp+28`
slot; `volatile int amount` reproduces the memory-resident stores but gets a
dedicated slot (`sp+92`).  With `padA` kept the score is very close to HEAD:

| variant | pct | usize | record |
|---|---:|---:|---:|
| `volatile int amount` + `volatile double padA` | 83.4711 | 1996 | 168 |
| `volatile int amount` + `padA` + `padA = level` | 83.4711 | 1996 | 168 |
| `volatile int amount` + `volatile float padA` | 79.6143 | 1992 | 168 |
| `volatile int amount` + `volatile long long padA` | 79.3127 | 1996 | 168 |
| `volatile int amount` + `volatile int padA` | 79.2842 | 1992 | 168 |
| `volatile int amount`, no padA | 71.9117 | 1984 | 168 |
| `volatile int amount` + `expect(pick==12,1)` | 80.7163 | 1996 | 168 |
| `volatile int amount` + `expect(pick==12,0)` plain | 80.7163 | 1996 | 168 |
| `volatile int amount` + `expect(offer==5,0)` | 63.8947 | 1956 | 168 |
| `volatile int amount` + `expect(type<15,0)` | 39.1667 | 2020 | 168 |
| `volatile int amount` + padA store in else / before switch | 80.9917 | 1996 | 168 |
| `volatile int amount` + padA store after switch | 83.3333 | 1996 | 168 |
| `volatile int amount` + padA store after itemId | 80.9917 | 1996 | 168 |

Region accounting (`region.py`, orig vs `volatile amount` frame-96 build,
1984 B): pick loop ours +8, campaign switch orig +8, offer arm ours +2,
`getRoutes` orig +4, switch orig +2, case15 orig +6, case0/2 orig +2 each,
distance `getSystem` ours +2 / orig +4, `idivmod` orig +2, `StringC1` ours +2,
epilogue orig +4, `ZdlPv` ours +2 → net +12.  The frame-96 shape is faithful
but the slot layout shifts one slot (`sp+16` guard vs orig `sp+20` guard,
`sp+20` agent vs `sp+24`, ...), which costs ~12 points.

## 5. Null-block relocation (the only lever that changes the record)

Hoisting `bool noRoutes` and moving the `nextInt(2)` tail after the first
invoke does move the block into/after the invoke region, but produces *real*
invoke rows (8 B) or removes a row, never the original's 4 B gap row:

| placement of the `noRoutes` tail | pct | usize | record |
|---|---:|---:|---:|
| HEAD (in place, before `getName`) | 83.6088 | 1996 | 168 |
| after `getName` (bool `noRoutes`) | 74.9658 | 2020 | **180** |
| after `getName` (`type==4 && race<4` heuristic) | 70.3297 | 2004 | **176** |
| after `difficulty` (before `itemId`) | 59.7966 | 2076 | 168 |
| after the campaign switch | 73.9876 | 2008 | **164** |
| after the reward chain | 68.4463 | 2020 | **176** |
| after `new Mission` | 74.0589 | 2012 | **180** |
| after `setCosts` | 77.9684 | 2008 | 168 |

`after_switch` is the interesting one: it *removes* a row (164 B).  None of
these is landable (body size moves and the score collapses).

## 6. Everything else tried (all ≤ HEAD)

* `pick == 12` forms: plain 80.8540/1996/168, `expect(pick!=12,0)` 80.8540,
  `expect(pick!=12,1)` 83.6088, `selected = true` hoisted 83.6088,
  `early_continue` 83.6088, ternary 52.6897/1988/168.
* Declaration permutations: `level` last 83.6088, `padA` first 83.6088, no
  blank line 83.6088, `int type` 83.6088, `amount` first 75.4821,
  `amount = 0` at decl 80.8540, `int race` 83.4711, `uint32_t guard` 56.5726,
  `int selected` 83.4711, `difficulty` hoisted to any position 83.6088.
* Local type tweaks: `uint32_t pick` 83.6088, `short`/`char pick`
  66.2088/2004, `short type` 57.8478/2008, `uint8_t type` 59.1569/1976,
  `int8_t type` 57.8478/2008, `int used i` 83.6088, `int32_t sum` 83.6088,
  `int found` 83.3333.
* `volatile` on other locals: `level` 79.4784/2004, `difficulty`
  68.5517/1992, `itemId` 47.4692/2000, `bonus` 72.0/1992, `costs`
  74.1403/1996, `fromSystem` 72.6272/2000, `guard` 52.864/1972,
  `selected` 59.7097/2044.
* `padA` store placement: after `fromSystemPtr` 83.4711, before `galaxy`
  83.4711, after reward 83.1956, before `new Mission` 81.9559, after
  `difficulty` 81.1295, at top 83.6088, `padA = level` 83.6088,
  `padA = selected` 83.6088, `padA = amount/race/targetStation/itemId`
  80.2478/2000, `padA = guard` 66.1146.
* Reward expression splits: `float dist` 81.9807/2000/176, `float base`
  77.686/1996/168, both 81.9807/2000/176, `toSystemPtr` 81.7056/2000/176,
  `Station *toStation` 79.9725/2000/176, `padA` moved before
  `fromSystemPtr` 83.4711.
* `__builtin_expect` on individual `if`s (0 and 1): `getIndex()==15`
  83.6088, campaign 70.5559/2004 and 67.7686, `type==12` 83.6088,
  `difficulty>=10` 83.6088, reward-rem 83.6088, `race>3` 53.1034/1992 and
  54.1667/2004, `used->data()[pick]` 60.4811/1988 and 80.9917,
  `sum==14` 59.8893/1964 and 83.3333, `sysIndex==25` 83.6088,
  `nextInt(2)==0` 83.6088.
* `while`/`for`/`do` rewrites, `used` block inversion, `noRoutes` bool,
  `void*` routes pointer, `if (pick==8)` label forms, `switch`→`if` chain:
  all 83.6088 (except `noRoutes_inv` 81.0548/1972 and reversed switch
  82.7824).
* `getOffer()` hoisted to a local, `type==12` moved after the offer block,
  `campaign < 16` → `<= 15`, offer-arm assignment order swapped: 83.6088.

## 7. Conclusion

The 4 B `.ARM.extab` shortfall is unchanged.  Two independent MBP decisions
gate it: `bb.49` (shared `type` store) must chain after `bb.48` instead of
`bb.32`, and `bb.50` (cold `getRoutes()==nullptr` block) must then be laid out
after the first invoke try-range.  Every source lever that moves either block
changes the body size and/or `type`'s register allocation enough to lose more
than the record gains.  The best measured shape remains HEAD:
**83.6088 % / 1996 B / 168 B record / frame 112**.

## 8. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6835/score.py                         # pct 83.6088, usize 1996
python3 /dev/shm/w6835/extab.py cmake-build-match/libgof2hdaa.so 0x971b8   # size 168
python3 /dev/shm/w6835/diffins.py                       # 727 vs 725 insns, 607 match
python3 /dev/shm/w6835/region.py                        # per-region byte deltas
```
