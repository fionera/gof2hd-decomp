# LevelScript::process — wave 171 / w11705: mission-78 tail + inlined-vs-called split

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Base master `41ef4c8b6` (wave 171).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11705/`.

Entry baseline (rebuilt from the clean tree):

```
raw 53,812   aligned 17,932   size 57,232/57,232   insn 20,724/20,830
gate: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. Landed: mission-78 escort fade order (raw-neutral, faithful)

The case-10 cold block (`0x13c82e`) fades `enemies[0]` then `enemies[1]`
(`ldr r0,[r0,#0]` then `ldr r0,[r0,#4]`).  Our source had them reversed.
Swapping to `[0]` then `[1]` reproduces the original call order exactly.
Measured raw 53,812 / aligned 17,932 / size 57,232 — byte-for-byte neutral
(the two instructions exist in both orders, only the `#0`/`#4` immediate
moves), but the block now matches the original instruction-for-instruction.

## 2. Dead mission-78 tail — re-measured on this base, still not landable

The original tail is shape B (post-switch):

```
13c88c  ldr  r0,[sl,#28]     ; m_nState
13c890  subs r0,#5
13c892  cmp  r0,#3
13c894  bhi  13c8ba
13c896  ... translate((float)-delta*0.8f, 0, 0) ...
13c8ba  ldrb r5,[sl,#17]     ; m_bCinematicBreak
13c8be  b.w  144f5a          ; shared bool epilogue
```

i.e. `if (state>=5 && state<=8) translate(...); return flag;` after the
switch.  Our source keeps the tail inside the switch braces (unreachable) and
the mission-78 branch falls through to the post-switch tail.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (tail dead inside switch) | **53,812** | **17,932** | **57,232** |
| A: tail after switch, branch falls through (no return) | 55,468 | 20,629 | 57,268 (+36) |
| B: tail after switch + `return m_bCinematicBreak != 0` | 55,364 | 21,090 | 57,392 (+160) |
| B + case-12 `m_nState=13`/return swap | 55,446 | 21,076 | 57,400 (+168) |
| A + case-12 swap | 55,477 | 20,628 | 57,268 (+36) |

Shape A is the cheapest (+36 on this base; w11605 measured +32 on the wave-170
base).  Shape B is worse than on the wave-170 base because our shared epilogue
(`0x144d00`) expects the flag already normalised, so every flag return
normalises inline (`cmp/it/movne`, 6 B); the original normalises once in the
shared epilogue (`0x144f5a`).  Both `return m_bCinematicBreak;` and
`return m_bCinematicBreak != 0;` compile identically.

Root cause of the raw churn: block placement.  In the original, mission-106's
case-2 block (`0x13be96`, 0xec B) sits between mission-78 case 11's break
(`0x13be92`) and mission-78's tail (`0x13c88c`); our mission-106 case-2 block
is placed ~0x1dd8 earlier (`0x1395a6`), so our live tail would land at
`0x13c41c`, 0x470 B before the original.  The tail cannot be placed correctly
until the mission-106/78 block layout matches; no size-exact live form exists
on this base.

## 3. Inlined-vs-called split — re-confirmed, plus two spurious call sites

The 245-name PLT callee set is identical.  `LevelScript::resetCamera` is a
PLT call 51x on both sides; `ArrayAdd<Player*>` is a PLT call 2x on both
(not inlined).  No TU move changes `process` inlining.

Call-count deltas (original vs ours, whole function):

```
  23 24  AEGeometry::setPosition(Vector const&)
  66 65  LODManager::forceUpdate(int,bool)      <- ours MISSES one
  15 16  Level::getStarSystem()
  23 24  Level::getPlayer()
   1  2  Status::getStanding()
   1  2  Standing::setStanding(int,int)
  41 42  PlayerEgo::getPosition()
   2  3  PlayerEgo::setAutoTurret(bool)
   1  2  PlayerEgo::autoTurretIsEnabled()
```

Two spurious call sites were isolated:

* **case 24 (`LevelScript.cpp:1825`)** `player->setAutoTurret(player->autoTurretIsEnabled());`
  does not exist in the original.  The original's wormhole block
  (`0x13a982..0x13a98e`) is just `landmarks[3]->reset(false); ...->open();`.
  Removing the line: raw 54,848 / aligned 17,809 / size 57,216 (-16), but
  `.rel.plt` rotates 9 entries.  The original's `.rel.plt` order is
  `autoTurretIsEnabled, setAutoTurret, open, ...`; our real pair site (case
  154, `0x13c742`) is placed *after* `open`, so the line is currently the
  PLT-order anchor.  Fixing it requires case-154's block to be placed before
  case-24's block (in the original they are `0x13a7fc` and `0x13a98e`).
* **case 42 (`LevelScript.cpp:2030`)** `if (delta == 0x7d) setStanding(0,100);`
  is a dead PLT-order anchor; the original has only the mission-50/51 body
  site (`0x139b06`).  Removing it: raw 54,652 / aligned 17,928 / size 57,224
  (-8), `.rel.plt` differs by 26 bytes (our real site is at `0x13b52a`,
  after `setRotationAroundTarget`'s first reference; original is `0x139b0e`,
  before `0x139d94`).

Both are block-placement workarounds, not source-shape errors.

The single missing `LODManager::forceUpdate` is in the mission-106 case-4/5
region (`0x13bfae` in the original: `resetCamera; forceUpdate(delta,false);
m_bCinematicBreak=0; m_nState=2; setActive(false); setToSleep();
setPosition(...); setVisible(false); b 144e36`).  Our corresponding block is
placed elsewhere; not isolated to a single source line.

## 4. Other residuals measured (not landed)

* **case 7 branch shape**: original `0x13bc18` is branchy
  (`blt tail; movs r0,#8; str.w r0,[sl,#28]; b tail`); ours is predicated
  (`itt ge; movge; strge`).  Adding `__builtin_expect(m_nScriptTime >= 27001, 0)`
  restores the branchy shape and is size-exact, but raw 53,812 -> 54,012
  (+200), aligned 17,932 -> 17,933.  Not landed.
* **case 5 timer store**: original `0x13b8d4` is `strd r0,r0,[sl,#144]`; ours
  is two `str.w` (8 B, +4).  Ten spellings (`0LL`, local `long long`,
  `-= self`, `*0`, pointer store, state-before-timer) all keep the split.
  clang does emit `strd r0,r0,[fp,#144]` for the identical
  `m_nScriptTimerA = 0; m_nState = 2;` in mission-106 case 1, so the split is
  a block-placement/scheduling artifact, not a type/alignment issue.
* **case 11 translate arg**: `(float)(delta*3)` is size-fatal on this base
  (raw 54,932 / aligned 17,922 / size 57,240, +8), unlike the wave-170 base
  where w11605 measured it size-exact.  The escort-fade swap is included in
  that measurement and changes nothing.
* **case 2 negation**: `(float)(-delta) * 0.5f` is size-fatal
  (raw 54,913 / aligned 17,874 / size 57,240, +8).

## Final (landed)

```
process raw 53,812 -> 53,812   aligned 17,932 -> 17,932   size 57,232
.text diffbytes 81,661 (unchanged)
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN
```

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=53812 aligned=17932 size=57232/57232 insn=20724/20830
```
