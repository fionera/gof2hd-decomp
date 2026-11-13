# MGame::OnUpdate — the last 10, wave 191 / w13555

Status: **no landed `src/` change** (negative sweep).  Worktree `wt/w13555`,
branch `wave191b-w13555`, base master `817cf1059`.  Target
`_ZN5MGame8OnUpdateEv` stays at **raw 10 / aligned 112 / 11,440 B,
insn 4003:4002** (the w189 `126800c09` shape).  Gate untouched:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Read with `mgame-onupdate-last16-wave163-w10919.md`,
`mgame-onupdate-last19-wave160-w10718.md`,
`mgame-onupdate-wave149-w9519.md`, `small-tails-wave189-w13307.md`.

## 1. Residual (6 runs, 10 bytes)

1. m23 `0x17e2b9..c5` (4 B): orig
   `ldr.w r2,[r9]; ldr r4,[pc,#316](=3600000); ldrd r2,r3,[r2,#256]; subs r0,r4,r0`
   ours `ldr.w r4,[r9]; ldr r5,[pc]; ldrd r2,r3,[r4,#256]; subs r0,r5,r0`.
2. m122 `0x17f190..1b1` (6 B): orig `mov r4,r0; ldr.w r0,[r9]; blx ccm; ...;
   add r2,r4` — ours `mov r5,r0; mov r0,r4; str r4,[sp,#120]; blx ccm; ...;
   add r2,r5`.  The `str` is the dead volatile store of the anchor.

The m122 shared tail (`b.w 17e26c` -> `ldr.w r0,[r9]; blx getPlayingTime;
ldr.w r2,[r9]; strd r0,r1,[r2,#256]`) is byte-exact.

## 2. Why the anchor is needed (re-derived)

`%1053` = the GOT base of `@Globals::status`.  Original keeps it in **r9**
from `0x17e088` (first `Globals::status` use of the m23 if-chain) to
`0x17e8e8`, and spills it to `[sp,#36]` at `0x17e31c` for the wingman-radio
array loop (`ldr.w r9,[sp,#36]` at `0x17e348`).  Removing the anchor makes
the plain source (`Globals::status` at the m122 ccm, no local) allocate
`%1053` to **r5**, split it (`mov r9,r5` at the 68824B slot, `mov r5,r9`
later) and shrink the function to **11,392 B** (raw 5,196) — size-fatal.
The w189 volatile-store anchor pins `%1053` to r9 and reproduces the
original's spill structure byte-for-byte everywhere else.

In the anchored build `%142` (the m23 `Globals::status` field load, MIR
48560B) is CSE'd with the anchor and lives 48560B..49088B in **r4**; that
is the whole residual: r4 is the anchor's register, so the m23 constant
goes to r5 and the m122 `radioIndex` goes to r5 / the ccm uses
`mov r0,r4` + the volatile `str`.

## 3. Sweep (all size 11,440 unless noted; raw via `tools/fn_diff.py`)

| shape | raw | note |
|---|---:|---|
| **HEAD (w189 anchor)** | **10** | best |
| `Status *const st` / `Status &st` / `Status *volatile st` store | 10 | identical code |
| `volatile int dummy = radioIndex;` instead of the `st` store | 10 | store moved after ccm; same 6 bytes |
| m23 field load made volatile (`*(Status *volatile *)&Globals::status`) | 10 | plain load still merged with the volatile load |
| m23 `3600000 < now - field` / `long long hour` local (m23+m24) | 10 | constant folded/sunk |
| `Level *level` before `nextInt` (`level_before`) | 33 | level hoisted to r4, const r1, `adds` |
| `Level *level` before `nextInt` + anchor (`level_then_head`) | 37 | level -> r8 |
| anchor before `nextInt`, ccm via `st` (`st_before_store_before_ccm`) | 15 | ldrd dest r3:r5 (anchor live past ldrd) |
| anchor before `nextInt`, store before `nextInt`, ccm plain | 361 | ldrd dest r3:r5, +2 B shift at m122 |
| store after ccm, ccm plain (`store_after_ccm_plain` / `sacc_*`) | 19 | m23 2 B (anchor r5) but m122 const->r1, level->r0, `adds` |
| volatile-load anchor before `nextInt`, used at ccm | 362 | m23 exact, GOT base r9; extra `ldr.w r4,[r9]` |
| volatile-load anchor after `nextInt` | 5196 | plain build (r5) |
| anchor before the m23 if-chain (store there), used at m122 | 6239 | size 11,340 |
| `Status *volatile st` before `nextInt` (store+reload at ccm) | 15 | m23 ldrd dest r3:r5; m122 store+reload = size-neutral |
| `volatile long long dummy = (long long) st` | 925 | size 11,444 |
| `Status **volatile dummy = &Globals::status` | 5212 | plain build |
| `AERandom *rnd` anchor / `Level *volatile` anchor | 930 / 345 | |
| `__builtin_expect(...,0)` on the `==122` select | 10 / 19 | no change |
| 60 permutations of the 5 m122 statements (anchor/store/radioIndex/radioBase/level) | 10 min | exhaustive; HEAD is the minimum |

## 4. What was learned

* `RAGreedy::enqueue` prioritises **global live ranges by size**
  (`Prio = (1u<<29) + Size`, larger first; llvm 7 `RegAllocGreedy.cpp`),
  so the GOT base (size ~60 kB) is assigned before the anchor (~528 B) and
  before the `tLDRpci` constant (~64 B).  The anchor therefore cannot move
  `%1053` directly; it changes the *spill/split structure* at the
  `[sp,#36]`/`[sp,#32]` sites (store vs `mov r9,r5`), which is what pins
  `%1053` to r9.
* The m23 constant is a `tgpr` (`tLDRpci %const.17`, MIR 48592B) and the
  anchor a `gpr` (MIR 48560B).  The anchor is larger, so it takes r4 and
  the constant takes r5.  Making the constant's interval larger than the
  anchor's (it is only ~64 B) would give the original's r4 constant and
  r5 anchor — but the constant is a literal that LLVM rematerialises at
  each use, so a `long long hour` local does not extend its interval.
* Any anchor that is *not* live at the m23 block (volatile load after
  `nextInt`, `&Globals::status` store, `volatile int`/`char`/`short`
  stores) loses the r9 assignment and falls back to the 11,392 B plain
  build.
* The m122 `store_after_ccm_plain` shape is the closest alternative
  (aligned 7): m23 is only 2 B off (anchor in r5) but the scheduler puts
  the constant in r1 and the level load directly in r0, giving
  `adds r2,r1,r4` instead of `add r2,r4; mov r0,r1` (17 B).
* `volatile int dummy = radioIndex;` (store after nextInt) gives exactly
  the same 10 bytes as HEAD — the anchor value is irrelevant as long as a
  volatile store keeps a value live across `nextInt` in r4.

## 5. Commands

```bash
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/home/debian/GalaxyOnFire2/scratch/w13555`:
`oracle.py`, `try2.py`, `lb3.py`, `sweep.py`, `perm.py`, `rawdiff.py`,
`dis.py`, `base_all.log` / `plain_all.log` (MIR), `mgame_cmd.txt`.
