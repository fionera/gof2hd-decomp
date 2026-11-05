# MGame::OnUpdate — idle-contact `Status*` r5/r9 residual (wave 112, w6019)

Status: **research pass, no `src/` change landed.** Worktree `wt/w6019`, branch
`wave112-w6019`, base = current master (`5d98bd391`). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8 (offset 0 = 0x17c8d8).
The function is already at its best measured shape: **41 diff bytes, one run,
size exact, 99.9 %**. This note extends waves 97 / 105 / 106 / 107 / 108 and the
wave-111 residual taxonomy with (a) a sharper greedy-RA mechanism (the
`CostPerUse` early-return in `tryAssign`), (b) the pre-RA MIR of the exact block,
and (c) 40 further measured source shapes.

## 1. Baseline (re-verified this session)

```
size 11440 11440  diffbytes 41  runs 1
0x17bb 0x17e3 41          (absolute 0x17e093 .. 0x17e0bb)
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2766-2772`). Original vs ours:

```
original                                    ours (current source)
17e08c ldr.w r9,[r0]   ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                       17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission       17e094 mov   r0,r5
17e098 cmp   r0,#23                        17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                        17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx   isMining                      17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                         17e0a2 blx   isMining
17e0a6 bne   17e176                        17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                       17e0a8 bne   17e176
17e0ac blx   getMission                    17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                       17e0ae blx   getMission
17e0b4 cmp   r0,#0                         17e0b2 blx   isEmpty
17e0b6 beq   17e176                        17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                       17e0b8 beq   17e176
17e0bc blx   getPlayingTime                 17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                       17e0bc blx   getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

Both blocks are 52 B (4+4+4+4 = 4+2+4+2+4), so the run is a pure byte
permutation of the register field plus the induced branch-offset shift.
From 0x17e176 (mission-24) to the end of the function the two disassemblies are
**byte-identical** (1749 instructions, 0 differences, checked with the
verifier's own disassembler), including the wingmen-radio loop where the GOT
base is spilled to `[sp,#36]` and r9 is reused for
`gWingmenRadioMissions`. Before 0x17e176 only 14 instructions differ, all of
them in the mission-23 block.

## 2. New this wave: the exact greedy-RA mechanism

`lib/CodeGen/RegAllocGreedy.cpp` (LLVM 7, `/dev/shm/wf110/llvm702src`) `tryAssign`:

```cpp
  Order.rewind();
  unsigned PhysReg;
  while ((PhysReg = Order.next()))
    if (!Matrix->checkInterference(VirtReg, PhysReg))
      break;
  if (!PhysReg || Order.isHint())
    return PhysReg;
  // missed-hint eviction ...
  unsigned Cost = TRI->getCostPerUse(PhysReg);
  if (!Cost)                       // <-- r0..r7 have CostPerUse 0
    return PhysReg;                //     returns immediately, no tryEvict
  unsigned CheapReg = tryEvict(VirtReg, Order, NewVRegs, Cost);
  return CheapReg ? CheapReg : PhysReg;
```

and `enqueue`:

```cpp
  if (ExtraRegInfo[Reg].Stage == RS_Split) {
    Prio = Size;                            // no high bits: dequeued LAST
  } else {
    Prio = (1u << 29) + Size;               // global ranges
    Prio |= (1u << 31);                     // ... and local ranges
    if (VRM->hasKnownPreference(Reg)) Prio |= (1u << 30);
  }
```

Consequences, confirmed against the 7.0.2 `-print-after-all` MIR of the current
source (`/dev/shm/w6019/pre_ra_onupdate.mir`, dump after *Live Register Matrix*):

* `%1053` is the `&Globals::status` GOT base of the idle block
  (`42016B %1053 = t2LDRi12 %1052, 0`). After regalloc it is the **single,
  unsplit** range `%2931`, live from slot `42016B` to the end of the function,
  with spill/reload `COPY`s only at `42856/44520/45704/47368` (the wingmen loop).
  It is an `RS_Split` range, so its queue priority is `Prio = Size` — **lower
  than every global range and lower than most local ranges**.
* `%128` is the `Status*` value (`42032B %128 = t2LDRi12 %1053, 0`), live
  `[42032, 42608)` across **four MBBs** (`bb.238..bb.241`). It is therefore a
  *global* range: `Prio = (1<<29) + 576`, i.e. it is dequeued **before**
  `%2931`, takes **r5** (CostPerUse 0, first free), and stays there.
* When `%2931` is dequeued, r5 is occupied by `%128`, so the first free
  register is a high one (r9, CostPerUse 1) and the `tryEvict` path runs — the
  GOT base ends in **r9**, which is the original's colour.
* With no `st`, nothing occupies r5 when `%2931` is dequeued, so `tryAssign`
  returns **r5** immediately (`Cost == 0`, no eviction) — the 11392 B /
  5148 diff / 213 run no-`st` shape.

So the anchor works because it is a **multi-MBB value** (global-range priority),
not because of its weight. That is sharper than wave 107's "tryAssign finds no
free register" phrasing: the anchor is not blocking a *scan*, it is winning the
*queue*.

### 2.1 Corollary — every `st` placement costs a `mov` in m23

`Globals::status` is a single invariant load in the idle block (the MIR keeps
one `%128`/`%1053` pair across all four calls), so machine CSE merges *any*
`Status *st = Globals::status;` in the block with the **ccm load**. Hence:

* `%128` always has ≥ 2 uses (ccm + `st`), so it cannot be coalesced into r0;
* the ccm therefore always emits `mov r0,r5`, never `ldr.w r0,[r9]`.

The only byte-neutral placement is the current one (`st` used at
`getPlayingTime`, where the 2-byte `mov` exactly replaces the 4-byte load that
the 4-byte `st` load added). Every other placement either adds or removes bytes
and/or recolours the whole function. This is why six waves of `st` sweeps all
bottom out at 41.

## 3. Variants measured this wave

`cmake --build cmake-build-match --target link_so` + objcopy `.text` diff
(`/dev/shm/w6019/try3.py`; the size column is the `nm` size of
`_ZN5MGame8OnUpdateEv`, so a size change is detected, not mis-read as diff
bytes). Baseline row is the current source.

| variant (what `st` is used at) | size | diff | runs |
|---|---:|---:|---:|
| **baseline: `st` at m23 `getPlayingTime`** | **11440** | **41** | **1** |
| `st` at m23 ccm + getPlayingTime | 11440 | 41 | 1 |
| `st` at m23 getPlayingTime, decls hoisted to idle-block top | 11440 | 41 | 1 |
| `st` at m23 getPlayingTime, decls hoisted into m23 body | 11440 | 41 | 1 |
| `this->dialogueWindow = new DialogueWindow(...)` directly | 11440 | 41 | 1 |
| `auto *st` | 11440 | 41 | 1 |
| `int variant` moved before `pauseOpen = 1` | 11440 | 126 | 4 |
| `st` at m23 getPlayingTime + alien `field_0x17c` | 11440 | **50** | **3** |
| `st` at m23 getPlayingTime + m122 `getPlayingTime` | 11440 | 4525 | 307 |
| `st` at m23 getPlayingTime + alien `getPlayingTime` | 11440 | 4480 | 306 |
| `st` at m23 getPlayingTime + wingmen ccm | 11440 | 4750 | 331 |
| `st` at m23 getPlayingTime + m24 `getPlayingTime` | 11440 | 4785 | 308 |
| `st` at m23 getPlayingTime + second `st2` at m24 pt | 11440 | 4785 | 308 |
| `st` at m23 getPlayingTime + m24 ccm | 11440 | 4822 | 305 |
| `st` at m23 getPlayingTime + hint-23 ccm | 11440 | 5020 | 290 |
| `st` at m23 getPlayingTime + hint-32 ship/ccm | 11440 | 5048 | 289 |
| `st` at m23 getPlayingTime + hint-32 mission | 11440 | 5048 | 289 |
| `st` at m23 getPlayingTime + m122 ccm (both) | 11440 | 4565 | 304 |
| `st` at m23 getPlayingTime + m23 body store (both operands) | 11440 | 6233 | 306 |
| `st` at m23 ccm + m23 body store | 11440 | 6233 | 306 |
| `st` at m23 ccm + pt + m23 body store | 11440 | 6233 | 306 |
| `st` at m23 getPlayingTime + m24 body store | 11440 | 6235 | 299 |
| `st` at m23 field only (not pt) | 11440 | 4985 | 308 |
| `st` at m23 pt + field | 11440 | 4985 | 308 |
| `st` at m23 ccm + field | 11440 | 4985 | 308 |
| `st` at m23 ccm + pt + field | 11440 | 4985 | 308 |
| `st` at m23 mission + pt | 11440 | 4986 | 304 |
| `st` at m23 ccm + mission + pt | 11440 | 4986 | 304 |
| `st` at m23 all four | 11440 | 5041 | 271 |
| `st` at m23 ccm + mission + field | 11440 | 5041 | 271 |
| `st` at m23 mission + field | 11440 | 5041 | 271 |
| `st` at m23 ccm only | 11440 | 5195 | 213 |
| `st` at m23 mission only | 11440 | 5172 | 241 |
| `st` at m23 ccm + mission | 11440 | 5172 | 241 |
| `st` at idle-block top, m23 condition all `Globals::status`, used at m24 ccm | 11440 | 230 | 9 |
| `st` declared before the outer `elapsedTime > 5000` if, used at m24 ccm | 11440 | 8771 | 534 |
| same, used at m24 getPlayingTime | 11440 | 8762 | 541 |
| same, used at m24 getMission | 11440 | 8766 | 539 |
| `st` declared right after `if (elapsedTime > 5000) {` | 11440 | 8771 | 535 |
| `Level *level = this->level;` cached across m122 + alien blocks | 11440 | 4402 | 346 |
| `GameText *text = Globals::gameText;` cached across m23 + m24 bodies | 11440 | 9027 | 383 |

Nothing beats 41. The best non-baseline shape by *run structure* is
`st + alien field_0x17c` (50 B / 3 runs): the m23 run is unchanged at 41 B and
it adds a 9-byte run at 0x17e2ea — proof that extending the anchor's live range
does keep r9 (the m23 block is untouched) but does not remove the `mov`.

## 4. Why the two obvious escapes do not work

* **The `Level*` / `GameText*` anchors** (`f_level`, `f_text`) are multi-MBB
  global ranges too and do force a global recolour, but they also change the
  call-site code (a cached pointer turns a second `ldr` into a `mov`), so the
  m23 block is *not* left clean — the recolour starts at `+0x2b0`.
* **The `llvm 7.0.0svn` assertions oracle** at
  `/dev/shm/wf110/llvm702build/bin/llc` is structurally faithful (it reproduces
  the baseline's `ldr r5,[r8] / mov r0,r5 / … / mov r0,r5` shape) but **not
  register-faithful**: it puts `this` in `r10` and the GOT base in `r8`, while
  the real 7.0.2 puts `this` in `fp` and the GOT base in `r9`. It cannot be used
  to predict the r9/r5 decision.

## 5. Remaining lever (outside this TU)

The only untested lever is wave 105's hypothesis 1: make the `&Globals::status`
GOT load non-rematerialisable (or otherwise change its GOT-load attributes) so
the allocator's split/rematerialisation choice changes. That lives in
`src/game/core/Globals.h` (`static Status *status;`) and affects all 331
`Globals::status` GOT-base sites in the binary, so it is a separate pass with
its own full-gate measurement. `status` is assigned in `Globals.cpp`
(`Globals::status = status;` at line 804), so a `Status *const` spelling is not
available.

## 6. Structural state (unchanged — no `src/` change)

```
sections            27 (exact)
file size           2,187,328 B (exact, both)
.text               0x1337c8 @ 0x69440   (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08  (size/base exact)
.ARM.extab          0x26298  @ 0x1a3b78  (size/base exact — the wave-110 4 B
                                          shortfall is gone after the
                                          Level::createShip pass)
.rodata             0x41c27 ours / 0x41b67 orig (+192 B rank table)
.got                0x430c   @ 0x20fcf4  (size/base exact)
```

No section, `.ARM.extab` or file-size delta was introduced by this pass.

## 7. Gate (note-only pass)

`cmake --build cmake-build-match --target check -j6` with the pristine source
restored:

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN)
drift:  0 field(s) across 0 class(es)
```

Before -> after: byte 4500 -> 4500, linked 4500 -> 4500. Target unchanged at
41 diff bytes / 99.9 % / 11440 B.

## 8. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6019/try3.py /dev/shm/w6019/v1{2..9}.json   # variant sweeps
python3 /dev/shm/w6019/measure.py                             # size/diff/runs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Scratch (not committed): `/dev/shm/w6019/` — `MGame.base.cpp` (pristine TU),
`try3.py` (variant runner with `nm` size check), `measure.py`, `cc.sh`
(`-mllvm -print-after-all`), `mir.txt` (full 7.0.2 pass dump),
`pre_ra_onupdate.mir` (pre-RA MIR of `OnUpdate`), `orig_dis.txt`,
`ours_dis.txt`, `v12.json`..`v19.json`.
