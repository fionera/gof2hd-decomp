# Small tail wave 164 (w11017): Generator bonus fold landed

Status: **1 `src/` change landed** (`Generator::createMission`).  Worktree
`wt/w11017`, branch `wave164-w11017`, base master `b3d3a1f68` (wave 163).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11017`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0),
all 25 section sizes/offsets exact.

Metrics: raw same-address diff bytes (`tools/fn_diff.py`), instruction-index
alignment (`/dev/shm/w11017/diff.py`) and global `.text` differing bytes
(`/dev/shm/w11017/textdiff.py`).

## Landed: `Generator::createMission` bonus fold

Master (`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`) was
`raw 162 / aligned 64 / 1996 B`.  The original's bonus block is

```
adds r5, r1, r6        ; bonus = rawBonus + rem   (rem still in r1)
str  r1, [sp, #32]     ; volatile store of rem
...
ldr  r0, [sp, #32]     ; volatile reload of rem
cmp  r1, #0
it   ne
subne r5, r6, r0       ; bonus = rawBonus - rem
str  r5, [sp, #16]     ; bonus spilled into galaxy's slot
```

i.e. `bonus = rawBonus - rem`, not `bonus -= rem * 2`.  The master shape
(`bonus -= rem * 2`) is a `sub.w r6, r6, r0, lsl #1` and keeps `bonus` in
`r6` until the `new Mission` call.

The landed shape:

```cpp
volatile int bonus;
if ((type | 4) == 12) {
    bonus = 0;
} else {
    int rawBonus = ...;
    volatile int rem = rawBonus % 50;
    int b = rawBonus + rem;
    if (b % 50 != 0) {
        b = rawBonus - rem;
    }
    bonus = b;
}
```

Measured: `raw 162 -> 127`, `aligned 64 -> 107`, `1996 B` exact,
`.text` differing bytes `84,652 -> 84,617` (**-35**), `.ARM.extab`
`725 -> 719` (**-6**).  The frame grows `96 -> 104` (the `volatile` bonus
gets its own 8-byte slot instead of sharing `galaxy`'s `sp+16`), which is
what costs the aligned count; raw and `.text` are the primary metrics and
both improve.

### Generator sweep (all measured)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base (`bonus -= rem * 2`) | 162 | 64 | 1996 |
| `bonus = rawBonus - rem` (plain bonus) | 709 | 89 | 2000 |
| `bonus = rawBonus - rem` (`init0_if`) | 709 | 89 | 2000 |
| `bonus = rawBonus - rem`, `init0_if`, `-= rem*2` | 162 | 64 | 1996 |
| plain `rem`, `bonus = rawBonus - rem` | 181 | 116 | 2000 |
| local `b`, volatile rem, `b = rawBonus - rem`, plain bonus | 709 | 89 | 2000 |
| local `b`, nonvolatile rem, `b = rawBonus - rem` | 181 | 116 | 2000 |
| local `b`, volatile rem, `b -= rem*2` | 162 | 64 | 1996 |
| local `b`, volatile rem, `b = rawBonus - rem`, **volatile bonus** | **127** | **107** | **1996** |
| local `b`, volatile rem, `b = rawBonus - rem`, `*(volatile int*)&bonus` | 127 | 107 | 1996 |
| local `b`, volatile rem, `b = rawBonus - rem`, `volatile int &bonusRef` | 127 | 107 | 1996 |
| volatile bonus + `bonus -= rem*2` | 665 | 144 | 2004 |
| volatile bonus + `bonus = rawBonus - rem` (no local b) | 663 | 145 | 2000 |
| volatile bonus + local `b`, explicit else | 654 | 142 | 2000 |
| volatile bonus + local `b`, ternary | 127 | 107 | 1996 |
| volatile bonus + local `b`, `b % 50` (no `!= 0`) | 127 | 107 | 1996 |
| volatile bonus + local `b`, nonvolatile rem | 162 | 97 | 2000 |
| volatile bonus placement (top/before galaxy/after fromsys/default) | 127 | 107 | 1996 |
| `volatile int bonus = 0` + local b | 724 | 164 | 2000 |
| `volatile int b` | 694 | 149 | 2000 |

The `127/107/1996` attractor is stable across the pointer/reference store
variants and the ternary/`b % 50` spellings.  The faithful shape
(`bonus = rawBonus - rem`, non-volatile bonus) is `709/89/2000` — 4 B
larger, hence gate-fatal; the volatile-bonus shape is the only size-exact
win.

## Not landed

### `ModStation::ModStation` C1/C2 — `43 / 12 / 1008` unchanged
Residual is the farKey yaw-table literal-pool order: original pool
`[pitch, YawIPad, Yaw]` with `r3=YawIPad, r4=Yaw`; master pool
`[pitch, Yaw, YawIPad]` with the same `r3/r4` pair.  The nearKey block
(`d53ce..d5436`) is byte-identical.  Swapping the ternary to
`!iPad ? Yaw : YawIPad` fixes the pool order but swaps the register pair.
All wave-163 shapes re-confirmed at `43/12` (or worse); no source-only
shape gets both.  Pure Greedy-RA / pool-materialisation tie.

### `MenuTouchWindow::OnTouchBegin` — `28 / 16 / 1896` unchanged
Two RA ties: case-15 `Globals::layout->buttonInsetX` lands in `ip` (orig)
vs `r4` (ours), and the later `heapBufB[field_0x1e0]` block swaps
`r2/r3/r6`.  Wave-163 sweep shapes re-confirmed (`Layout *layout` 28/16,
no-layout-local 362/76/1892, `uint32_t buf` 351/76/1888, etc.).

### `MGame::OnUpdate` — `16 / 6 / 11440` unchanged
Original loads `this->level` *after* `getCurrentCampaignMission()` into
`r1` (`mov r0, r1`); master loads before into `r5`.  Declaring
`Level *level` after the ccm makes clang fold it to
`this->level->createRadioMessage(...)` and the whole function shrinks
`11440 -> 11392` with `5196/597` (a global RA cascade, not a local fix).
The 16-byte residual is the `level` load order + `radioBase` landing in
`r1` vs `r2`; every reorder/`const`/`unsigned`/inline-ternary shape stays
`16/6`.

### `Gun::calcCharacterCollision` — `463 / 57 / 1984` unchanged
`i*3` in `r4` (orig) vs `r5` (ours) and the `playerPosition`/`enemyPosition`
stack slots (`sp+44`/`sp+104`) swapped.  Re-tried `*(data()+i)`,
`reinterpret_cast<Vector*>(data())[i]`, reference locals (1920 B,
gate-fatal).  Pure RA tie (wave-163 note is exhaustive).

### `SpaceLounge::drawLounge` — `175 / 41 / 1948` unchanged
Right-edge clamp: original `itt gt; addgt r1,r6,sl; subgt r5,r0,r1`; master
reassociates to `0 - 2*pad - width + (w-inset)`.  Wave-163 shapes
re-confirmed; the exact clamp costs the `r5/r9` allocation tie.

## Files changed

* `src/game/mission/Generator.cpp` (bonus fold).
* `_work/reconstructions/small-tail-wave164-w11017.md` (this note).
