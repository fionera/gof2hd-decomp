# MGame::OnTouchEnd (8080 B) — w5907 (wave 111): no gate-safe gain;
# the original really does have a separate `== 127` campaign case

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `8eea060cc`, branch
`wave111-w5907`.  Starting point was the wave-110 state (`char refinedAny`,
8080 B, 78.9189 %, `.rel.plt` diff 0).

## Result

**No gate-safe improvement found.**  HEAD stays at 8080 B / 78.9189 % /
`.rel.plt` diff 0 / `RATCHET PASS` (byte 4500, linked 4500, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).  This note records the
measurements and two new structural findings.

The hard constraint is the interaction of three things:

1. `.text` size must stay **exactly 8080 B**.  Any `.text` length change
   shifts `.rodata` and every in-function literal pool: `byte_exact`
   collapses 4500 → **3102** (measured on `s127`, 8100 B).  Size-exact is
   not itself a ratchet criterion, but `byte_exact` is, so it is binding.
2. The first-reference order of `resetStartSequenceOver` /
   `removeObjectives` / `goingToWaypoint` / `removeRoute` inside this
   function fixes the `.rel.plt` order (reset, removeObjectives,
   goingToWaypoint, removeRoute).  Rotating it moves the PLT stubs and
   changes every caller's `blx` offset; `MGame::successCheck` (currently
   byte-exact) then loses byte-exactness → byte 4499 → **RATCHET FAIL**.
3. The remaining 21 % is MachineBlockPlacement.  It is insensitive to
   source nesting/statement order (see §3) and only moves when the CFG
   shape or branch weights change — and every such change tried so far
   either changes the size or rotates `.rel.plt`.

## 1. New finding: the original has a *separate* `== 127` campaign case

The original contains exactly one `cmp r0, #127` (0x17bc34) and one
`cmp r0, #96` (0x17b71c).  The 127 test is a distinct `else if` placed
**between the `== 126` and `== 134` cases**:

```
17bc24  isCampaignMission; cbz -> ...
17bc2c  getCurrentCampaignMission
17bc34  cmp r0, #127
17bc36  beq.w 17b722            ; shared 96/127 body
17bc3a  isCampaignMission ...   ; == 134
```

So the original's campaign chain order is
`15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 127, 134, 144,
155, 161, 162`, and our HEAD's combined
`(== 96 || (isCampaignMission && == 127))` is **not** the faithful shape.
The faithful spelling is the `s127` transform used by the wave-110 worker
(move a duplicated 96 body to a new `== 127` case before 134):

```
} else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 96) {
    <96 body>
} else if (... == 134) {
} else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 127) {
    <96 body>
}
```

`s127` alone is **8100 B / 75.1620 % / plt 0** (fails size → byte 3102).
`s127 + disp` is 8080 B / 79.9063 % but plt 4 (byte 4499).  The 20-byte
difference is real: the original has 2775 instructions, our `s127` build
has 2781 (+6: `b.n`+`mov.w r8,#2`, `ldr r3,[pc]`+`ldr.w r4,[r8]`, and two
`nop`s).  Those six instructions are somewhere else in our source, not in
the 127 case; finding and removing them would make the faithful source
8080 B.

## 2. New finding: the mining-loop test is one instruction off

Original (0x17a5ca):

```
17a5ca  cmp.w r9, #0
17a5ce  beq.n 17a652        ; skip report when amount == 0
```

HEAD (same address):

```
17a5ca  cmp.w r9, #4294967295
17a5ce  bgt.n 17a652        ; skip report when amount >= 0
```

Both are semantically identical (`amount` is a down-counter initialised to
0), but the encoding differs (2 diff slots).  The original's test is
`amount != 0`; HEAD's is `-amount > 0` (i.e. `amount < 0` inverted).
Every source spelling that produces `cmp #0; beq` (`amount != 0`,
`amount`, `count != 0`, `count`, `unsigned count`, …) makes clang 7.0.2
strength-reduce the down-counter to an **up**-counter and drop 8 B
(8072 B / 70.98 %).  Spellings that keep the down-counter
(`count > 0`, `amount < 0`, `count >= 1`, `-amount > 0`, `amount <= -1`)
all emit `cmp #-1; bgt` and are code-identical to HEAD at 8080 B /
78.9189 %.  `bool refinedAny` also flips the counter (8076 B / 76.3518 %).

## 3. Measurements (all on top of wave-110 HEAD unless noted)

| variant | size | fuzzy | plt | note |
|---|---:|---:|---:|---|
| **HEAD** | **8080** | **78.9189** | **0** | gate-safe best |
| `s127` (faithful 127 case) | 8100 | 75.1620 | 0 | size fail |
| `disp` (campaign-first dispatch) | 8088 | 79.0848 | 4 | size + plt fail |
| `s127+disp` | 8080 | 79.9063 | 4 | byte 4499 → FAIL |
| `won_exp1+disp` | 8088 | 80.7928 | 9 | highest fuzzy seen |
| `won_exp1+s127+disp` | 8080 | 78.8326 | 9 | |
| `swap` (`if (!failed) { won } else { failed }`) | 8080 | 78.8468 | 0 | |
| `wonfirst2` (`if (!failed && won) … else if (failed)`) | 8080 | 78.8468 | 0 | same code as `swap` |
| `resetfirst`, `resetfirst2` (reset as first arm) | 8080 | 78.9189 | 0 | code-identical |
| `ia_hoist` (`bool instantAction = …`) | 8080 | 78.9189 | 0 | code-identical |
| `freelance_first`, `nested_camp` | 8080 | 78.9189 | 0 | code-identical |
| `flag` (resetSequence bool + separate `if`) | 8080 | 78.9189 | 0 | optimised away |
| `flag2` (flag + duplicated tail) | 8084 | 77.1753 | 0 | |
| `earlyreset` (early-return reset arm) | 8084 | 77.1753 | 0 | |
| `earlyreset+disp` | 8092 | 79.2507 | 4 | |
| `cm_hoist` | 8080 | 78.7748 | 0 | |
| `ia_hoist2` | 8092 | 79.3085 | 4 | |
| `ch400` (swap `&0x200`/`&0x400`) | 8064 | 71.9077 | 2 | |
| `swap4_80` (swap `&4`/`&0x80` blocks) | 8080 | 78.4505 | 2 | |
| `swap_hack_rocket` | 8080 | 78.7387 | 0 | |

`__builtin_expect` sweep on HEAD (all real C++): `hasfail0` 8068/72.67/9,
`camp42_0` 8084/74.09/8, `type12_0` 8080/76.93/4, `type3_0`
8080/78.9189/0, `type3_1` 8084/76.63/3, `cargo0` 8092/75.45/8,
`cargo1` 8080/78.80/3, `keepobj0` 8096/69.73/6, `keepobj1`
8080/72.95/0, `escort0` 8080/77.48/0, `escort1` 8080/78.77/0,
`goaway0` 8088/76.77/0, `goaway1` 8080/78.9189/0, `goaway2`
8088/76.95/0, `pause0/1` 8080/78.9189 (plt 7/6), `hudm1` 8080/78.9189/0,
`hud0_0` 8092/76.00/0, `hud0_1` 8076/77.14/0, `flags0` 8048/53.71/19,
`flags1` 8156/52.57/0, `wp0` 8088/76.95/0, `wp1` 8080/78.9189/0,
`won_exp0` 7832/60.52/13, `won_exp1` 8072/75.14/9, `failed_exp0`
8068/72.67/9, `failed_exp1` 7832/58.03/10.

## 4. Why block placement cannot be steered from the source here

The original's block order in the cutscene tail is
`won test → keepObjectives → mission ternary → instant-action dispatch →
campaign/freelance → hudTouchFlags==0 cluster → failed type-12 → reset →
freelance continuation → … → shared tail`.  HEAD's is
`failed type-12 → ternary → reset → campaign chain → …`.  The reset block
in particular sits at 0x17b226 in HEAD but at 0x17b0aa in the original.

Measured facts:

* Inverting the failed/won nesting (`swap`, `wonfirst2`), making reset the
  first arm (`resetfirst`, `resetfirst2`), an explicit `resetSequence`
  flag, and an early-return reset arm (`earlyreset`) **do not move the
  reset block at all** — `earlyreset+disp` still emits
  `blx resetStartSequenceOver` at 0x17b43a, exactly where `disp` puts it.
  MachineBlockPlacement decides it from the CFG/weights, not from source
  position.
* The only transforms that move it are CFG/weight changes
  (`disp`, `__builtin_expect`), and all of those rotate `.rel.plt`
  (reset ends up after `removeObjectives` → byte 4499).
* The remaining instruction stream is otherwise identical to the original
  (same 2775 instructions; only the mining `cmp`/`beq` and
  `lsls #24` vs `#31` differ).

## 5. Gate / section state

Unchanged from wave 110: 27 sections exact, file size 2,187,328 B,
`.ARM.extab` 0x26294 (original 0x26298, 4 B short — pre-existing),
`.rodata` exact, `.rel.plt` diff 0, imports 207, `RATCHET PASS`.

```bash
cd /home/debian/GalaxyOnFire2/wt/w5907
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w5907/sc.py                          # 8080 / 78.9189
python3 /dev/shm/w5907/plt.py                         # diff 0
```

Scratch (not tracked): `/dev/shm/w5907/` (`sc.py`, `plt.py`, `diffnorm.py`,
`blk3.py`, `multiset.py`, `try3.py`…`try10.py`, `comb.py`, `minex*.py`,
`disp*.py`, `expwon.py`, `micro.py`, `MGame.base.cpp`, `orig.asm`,
`base2.asm`).
