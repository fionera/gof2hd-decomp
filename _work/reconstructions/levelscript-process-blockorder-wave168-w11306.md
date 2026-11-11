# LevelScript::process — wave 168 / w11306: mission-78 switch exit block
# (source-level block order, not `__builtin_expect`)

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Worktree `wt/w11306`, branch `wave168-w11306`, base master
`631c26f0b` (wave 167).  Scratch `/dev/shm/w11306/`.

## Baseline (HEAD, unchanged, tree left clean)

```
process raw diffbytes    54,031
process aligned insns    17,764
orig / ours size         57232 / 57232   (size-exact)
branch direction mism.   15 / 736 conditional branches
.text diffbytes          82,526
.ARM.extab diffbytes     670
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

The 15 direction mismatches split into three groups:

| group | orig branches | target | count |
|---|---|---|---:|
| mission-78 (`0x138380`, `0x13b85a`..`0x13bd84`) | `bhi`/`blt` | `0x13c88c` | 7 |
| mission-0 exit (`0x13d378`..`0x13d554`) | `blt`/`beq` | `0x137f7c` | 4 |
| mission-tail (`0x13d7e0`, `0x140230`, `0x142b9c`, `0x142f12`) | mixed | various | 4 |

Two of the last four (`0x142b9c` `bne` vs ours `blt`, `0x142f12` `bcs` vs
ours `bcc`) are sequence-alignment artefacts (the tool pairs the orig loop
back-edge with the following `getHitpoints` test); the branch *condition* is
the same in both builds.  So the real residual is 13 branches.

## Root cause found: the mission-78 switch-exit block is dead code

`src/game/world/LevelScript.cpp` has, at the end of the mission-78
(`getCurrentCampaignMission() == 0x4e`) arm:

```cpp
        switch (m_nState) {
            case 1: ... case 12: ... case 11: { ... break; }
            if (m_nState >= 5 && m_nState <= 8) {          // line 1237
                m_pCamera->translate((float) -delta * 0.8f, 0.0f, 0.0f);
            }
        }                                                   // closes the switch
    } else if (... == 89) {
```

The `if` sits **inside** the switch body, after the last case label, so it is
unreachable: clang deletes it.  Consequently every `break` of the mission-78
switch falls straight through to the shared active tail at `0x137f76`, and
the function has no `subs r0,#5; cmp r0,#3` block at all.

The original has the block as the **switch exit block** at `0x13c88c`
(46 B, immediately after case 8's body which falls into it):

```
13c88c ldr.w r0,[sl,#28]        ; m_nState
13c890 subs  r0,#5
13c892 cmp   r0,#3
13c894 bhi.n 13c8ba
13c896 ldr   r0,[sp,#76]        ; delta
13c898 movs  r2,#0
13c89a vldr  s2,[pc,#92]        ; 0.8f
13c89e movs  r3,#0
13c8a0 negs  r0,r0
13c8a2 vmov  s0,r0
13c8a6 vcvt.f32.s32 s0,s0
13c8aa ldr.w r0,[sl,#20]        ; m_pCamera
13c8ae vmul.f32 s0,s0,s2
13c8b2 vmov  r1,s0
13c8b6 blx   translate
13c8ba ldrb.w r5,[sl,#17]       ; return m_bCinematicBreak != 0;
13c8be b.w   144f5a             ; shared epilogue (normalises r5)
```

So the mission-78 arm ends with an explicit `return m_bCinematicBreak != 0;`
(the return value is normalised in the shared epilogue), and the `if` is
after the switch.

## Measured source-shape sweep (full gold link, `-j2`)

Each row is a fresh build of the whole `.so`.

| # | shape | raw | aligned | size | dir.mism. |
|---|---|---|---:|---:|---:|
| 0 | HEAD (baseline) | **54,031** | **17,764** | **57,232** | 15 |
| A | move `if` after switch + `return m_bCinematicBreak != 0;` (faithful) | 55,449 | 20,722 | 57,368 (+136) | 14 |
| A2 | as A but `return m_bCinematicBreak;` | 55,449 | 20,722 | 57,368 (+136) | 14 |
| A3 | as A + drop case-12 `return` (fall into check) | 55,411 | 20,643 | 57,344 (+112) | — |
| B | move `if` after switch, **no** return | 55,496 | 20,294 | 57,252 (+20) | **7** |
| C | keep `if` inside, add `default:` label before it | 54,031 | 17,764 | 57,232 | 15 |
| D | keep dead `if` inside, add `return` after switch | 55,398 | 20,633 | 57,296 (+64) | — |
| E | move mission-0 `if (m_nState==16)` inside case 15 | 55,400 | 19,844 | 57,292 (+60) | 18 |

### Variant B is structurally right (7 mismatches, all mission-78 fixed)

Variant B's exit block reproduces the original's shape: case 8's body falls
into `ldr r0,[fp,#28]; subs #5; cmp #3; bhi`, and the translate is a
tail-merge into the mission-0 translate call site (`b.w 0x137e7e`).  All 7
mission-78 direction mismatches disappear.  The 7 remaining are the
mission-0 exit group (4) and the mission-tail group (3, two of them
artefacts).

But B is **+20 B** (57,252) and A is **+136 B** (57,368), so neither can be
landed: the function must stay 57,232 B and a size shift makes the raw
same-address metric meaningless (every byte after the insertion differs).

### Why the faithful shape costs bytes

* Adding the `return` alone (D) costs +64 B: the check-block return gets a
  local `cmp r5,#0; it ne; movne r5,#1` normalisation instead of jumping
  into the shared epilogue normalisation at `0x144cde` the way the original
  does (`0x13c8ba ldrb; b.w 0x144f5a`).  It also perturbs the global spill
  layout (prologue `strd r4,r0,[sp,#80]` becomes two `str`s, frame slots
  shift by 4).
* Moving the `if` out adds the ~44 B exit block on top (A = +136).
* Removing the case-12 `return` recovers 24 B (A3 = +112).

So our reconstruction is ~136 B *larger* than the original in the code that
this fix touches; the original's size-exactness is currently compensated by
other (unfixed) differences in the 57 KB body — notably the mission-78 case
bodies, which in the original contain five extra `TargetFollowCamera::translate`
calls (`0x13c30e`, `0x13c386`, `0x13c76e`, `0x13c820`, `0x13c8b6`) that ours
does not emit.  Landing this needs a companion reconstruction that recovers
those bytes; no size-neutral shape was found in the time-box.

## Mission-0 exit group (4 mismatches)

Original `0x13f902`: `if (m_nState == 16)` is reached only from case 15's
`isOver()` false edge (jump-threaded); every other case break goes straight
to the shared tail `0x137f7c`.  In ours the check is the common switch-exit
block at `0x13d5d4`, so all case breaks go there (`blt/beq -> 0x137f76`
become forward branches).  Moving the check inside case 15 (variant E) fixes
these four but introduces eight new mismatches in the mission-tail region and
costs +60 B, so it is not a win either.

## Branch-layout tool

`_work/reconstructions/branch_layout_w11220.py` (wave 167) still works; the
dumps were produced with the 20-line `dump.py` wrapper documented in
`build-provenance-round7-branch-weights.md` (paths adjusted to this
worktree).  Mismatch lists in this note are from that tool on the fresh
build after each variant.

## Files changed

* `_work/reconstructions/levelscript-process-blockorder-wave168-w11306.md` —
  this note.
* No `src/`, `CMakeLists.txt`, baseline or allowlist change; tree clean.
