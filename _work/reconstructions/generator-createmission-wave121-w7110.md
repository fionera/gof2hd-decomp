# `Generator::createMission` — wave 121 (w7110): 172 B LSDA + 84.7 -> 88.0 %

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows, pad=0 rows at `[0] [7] [12] [16] [19]`).

```
before (HEAD bd061a489):  84.7000 %  1996 B  LSDA 176 B (cslen 152, 26 rows, 4 pad=0)
after:                    88.0000 %  1996 B  LSDA 172 B (cslen 151, 26 rows, 5 pad=0)
```

Full `check` after the change:

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (lint CLEAN, drift 0)
```

27 sections, file size 2,187,328 B.  `.text` `0x1337c8` and `.ARM.exidx` `0x6f70`
stay size- and base-exact.  `.ARM.extab` is **156,304 B** (`0x26290`, 8 B short)
because the only remaining record delta is `Level::createShip` -8 (328 vs 336);
`Generator::createMission` is now **exact at 172 B**.  Section content deltas fell:
`.text` 88,118 -> **87,809** differing bytes, `.ARM.exidx` 1,660 -> **1,327**,
`.ARM.extab` 135,597 -> **107,985**.

## 1. The landed shape (5 lines)

```diff
-    struct NoopGuard { ~NoopGuard() {} };
-    volatile double padA;
+    volatile long long padA;
-        if (__builtin_expect(pick == 10, 1)) { ... }
+        if (pick == 10) { ... }
-        if (__builtin_expect(pick == 12, 0)) { ... }
+        if (pick == 12) { ... }
-        if ((uint32_t) type < 15 && __builtin_expect(((1U << type) & 0x4801U) != 0, 1)) {
+        if ((uint32_t) type < 15 && ((1U << type) & 0x4801U) != 0) {
-        if (race < 4 && type == 13) {
+        if (__builtin_expect(race < 4 && type == 13, 1)) {  // lint: arm_builtin ...
-        int count; { NoopGuard g; count = Globals::rnd->nextInt(15); }
+        int count = Globals::rnd->nextInt(15);
```

The `count` guard (wave-119 w6948) is gone: it was only there to inflate the
record to 176 B and cancel `Level::createShip` -8.  The branch-weight hints move
from `pick == 10` / `pick == 12` / the `0x4801` mask to the `race < 4 && type == 13`
test; that single relocation of the hint is what reproduces the original's block
order (see §3).  `padA` widens from `double` to `long long` (still a volatile
temporary in the reward comma expression) — this is the register-allocation anchor
that keeps the reward/bonus/costs region matching.

## 2. Exact LSDA structure after the change

Parsed with `_work/reconstructions/exidx_table.py`; record size is
`round_up(21 + cslen, 4)`.

| | original | before (HEAD) | after |
|---|---:|---:|---:|
| call-site table length (`cslen`) | **151** | 152 | **151** |
| call-site entries | **26** | 26 | **26** |
| pad=0 entries | **5** | 4 | **5** |
| record size | **172** | 176 | **172** |

Ours after (start/len/landing-pad, offsets from function start):

```
[ 0] 0x000/0x24c/0x0      [ 1] 0x24c/0x01c/0x78c  [ 2] 0x27c/0x032/0x794
[ 3] 0x2da/0x012/0x754    [ 4] 0x364/0x006/0x78c  [ 5] 0x396/0x006/0x790
[ 6] 0x3ac/0x018/0x756    [ 7] 0x3c4/0x020/0x0    [ 8] 0x3e4/0x036/0x78c
[ 9] 0x462/0x032/0x78e    [10] 0x4f8/0x004/0x774  [11] 0x528/0x014/0x778
[12] 0x53c/0x064/0x0      [13] 0x5a0/0x008/0x772  [14] 0x5a8/0x00a/0x76e
[15] 0x5b2/0x02a/0x77a    [16] 0x5dc/0x018/0x0    [17] 0x5f4/0x04e/0x776
[18] 0x642/0x008/0x758    [19] 0x64a/0x022/0x0    [20] 0x66c/0x01e/0x78a
[21] 0x696/0x006/0x792    [22] 0x6bc/0x004/0x78e  [23] 0x6e0/0x006/0x76c
[24] 0x6e6/0x008/0x75e    [25] 0x6ee/0x0de/0x0
```

The row *shape* is now identical to the original: 26 rows, `cslen = 151`, and the
same five pad=0 rows (`[0]`, `[7]`, `[12]`, `[16]`, `[19]`).  The `[7]` row is the
cold `getRoutes() == nullptr` gap the wave-119 notes chased; the new hint placement
puts that block after `getName` at `+0x3c4` (original `+0x3c8`) between two invoke
try-ranges, so `EHStreamer::computeCallSiteTable` emits the extra 5-byte pad=0 row.

## 3. Why the hint relocation works

`MachineBlockPlacement::hasBetterLayoutPredecessor(bb.32, bb.49)` chains the shared
`str r0,[sp,#36]` spill block after the offer arm (`bb.32`) instead of the
`type = 13` arm (`bb.48`) unless `freq(bb.48) >= freq(bb.32)/4` (HotProb 0.8).
The wave-119 w6816 note measured the ratio at **0.0155** with the old hint set.
Removing the `pick == 10` / `pick == 12` / mask hints and putting the single
`__builtin_expect(race < 4 && type == 13, 1)` on the race test raises the
`bb.48` path weight enough that the spill block is chained after `bb.48`, which:

* moves the shared `str r0,[sp,#36]` to `+0x240` (original `+0x240`),
* moves the cold null block after `getName` (`+0x3c4`, original `+0x3c8`), giving
  the 172 B record, and
* removes the `pick == 12` arm rotation (`bne.n` then-arm fallthrough, as original).

The `count` guard is then no longer needed for the record size and is dropped.

## 4. Measured variants

Base = HEAD (84.7 %, 176 B).  All 1996 B unless noted.  `rec` = createMission LSDA
record bytes.

### 4.1 hint combinations on the HEAD shape (full 81-cell sweep)

| pick10 | pick12 | mask | race | pct | rec |
|---|---|---|---|---:|---:|
| expect(1) | expect(0) | expect(1) | none | **84.7** | 176 |
| none | none | none | expect(1) | **85.2** | 180 |
| none | expect(0) | none | expect(1) | 84.4 | 180 |
| none | expect(0) | expect(1) | expect(1) | 84.4 | 180 |
| expect(1) | expect(0) | none | none | 83.0 | 176 |
| expect(1) | none | expect(1) | none | 82.0 | 176 |
| expect(1) | expect(1) | expect(1) | none | 82.0 | 176 |

The `none/none/none/expect(1)` cell (85.2 %, 180 B) is the shape that first moved
the cold block; dropping the `count` guard from it gives **88.0 %, 172 B**.

### 4.2 padA type on the `none/none/none/expect(1)` + no-count-guard shape

| padA type | pct | rec |
|---|---:|---:|
| **`volatile long long` (landed)** | **88.0** | **172** |
| `volatile unsigned long long` / `volatile int64_t` / `volatile uint64_t` | 88.0 | 172 |
| `volatile double` | 86.7 | 172 |
| `volatile long double` | 86.7 | 172 |
| `volatile int` | 86.1 | 172 |
| `long long` (non-volatile) | 76.5 | 172 |
| none (frame 96) | 76.5 | 172 |

`amount` variants: plain `amount = 0` 79.0 (2000 B), `volatile int amount` 87.9,
`int amount = 0` at declaration 79.0 (2000 B).  `newType` temp 88.0 -> 87.2.
`Globals::galaxy->getStation` for the inner call 76.1.  `padA` store before
`fromSystem` 87.6; after `fromSystemPtr` 87.6.

## 5. Remaining residual (88.0 %, 235/727 aligned mismatches)

1. **Frame 112 vs 96.**  `padA` occupies `sp+96/100` and the canary `sp+108`;
   the original frame is 96 with the canary at `sp+92`.  Slot colouring is the
   largest remaining delta: ours puts `galaxy` at `sp+28` and `amount` at `sp+92`
   where the original puts `galaxy`/`bonus` at `sp+16` and `amount` at `sp+28`.
2. **`difficulty` in `r5` vs `r6`** (~9 instructions).  The type==8 arm keeps
   `tecLevel` in `r5` and defers the shared `str amount` to the merge; the original
   stores `amount` inline and moves `tecLevel` to `r6`.  Not reachable by any
   declaration-order / temp / volatile spelling tried.
3. **`galaxy`/`fromSystemPtr` swap.**  Ours keeps `galaxy` in a register and spills
   `fromSystemPtr`; the original spills `galaxy` to `sp+16` and keeps
   `fromSystemPtr` in `r6`, reloading `Globals::galaxy` for the inner `getStation`.
4. **`type > 14` branch target.**  Ours `bhi.n +0x242` (direct to `getName`);
   original `bhi.n +0x1d0` (through the race check).  Value-range propagation
   difference; not reachable via `bool doSearch` / signed `int type`.
5. Landing-pad branch targets differ by 2-4 bytes because the frame is 16 B larger.

## 6. Reproduction

```sh
git checkout -- src/game/mission/Generator.cpp   # then apply the 5-line diff above
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
python3 /dev/shm/w7110/score2.py                     # pct 88.0, usize 1996
python3 /dev/shm/w7110/rows2.py <orig> <ours>        # 172 B, cslen 151, 26 rows
```

Scratch variants: `/dev/shm/w7110/` (`best4.cpp` is the landed source;
`search1.py`..`search6.py` are the sweeps).  Not part of the repo.
