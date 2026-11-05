# `Level::createScene` — wave-114 w6214: faithful all-global `Globals::rnd`
# source shape and an 8-site mask re-sweep; 90.5277 % / 2252 B still the max

**Status:** research note, 2026-11-05. Worktree `wt/w6214`, branch
`wave114-w6214`. Target `_ZN5Level11createSceneEv`.
**No source change landed** — the shipped wave-112 source (wave-113 confirmed
it) is still the best variant found. This commit contains only this note;
`src/` is untouched.

Follow-up to `level-createscene-wave113-w6113.md` (and the chain back through
waves 88–113). Wave 113 re-ran the full 128-mask sweep on the tricked source
and concluded 90.5277 % was the flag-free maximum. This pass attacked the
remaining diff from the other direction: reconstruct the *faithful* original
source shape (all `Globals::rnd->nextInt(...)`, no local `rnd` pointer, no
`Vector` address trick, retry loop counting up from -100) and search for a
source-only perturbation that reproduces the original ship-loop register
assignment without the `+1` instruction.

## TL;DR

* Baseline (shipped source): **90.5277 % / 2252 B**, 783/790 instructions,
  first diff at instruction 462 (parts-loop preheader `b.n` target
  `loc_610` vs `loc_614`, the known +4 shift from the trick's extra `add.w`).
* The original ship loop is provably the codegen of the **all-global**
  source: `r8 = &Globals::rnd` (GOT slot `0x2101f8`) and `ldr.w r0,[r8]` at
  every one of the 8 `nextInt` sites (shipCount + sites 0–6). A local
  `AERandom *rnd = Globals::rnd` caches the value in `r5` instead.
* But the all-global source alone scores **75.0 % / 2228 B** (no trick) or
  **75.5 % / 2232 B** (trick). Its prologue diverges at instruction 8: the
  stack-guard address is kept in `r5`/`sl` instead of being spilled to
  `[sp,#36]`, and `&Globals::status` lands in `r4` instead of `sl`. The
  local `rnd` pointer is what forces the guard spill and the `sl = &status`
  assignment that the original has.
* Consequently the early match and the ship-loop RA are in direct tension:
  the local pointer is required for instructions 0–461, but it is exactly
  what displaces `race`/`capacity`/`type` in the ship loop
  (`r5`/`r6`/`r8`/`fp` permutation). Every source-only attempt to have both
  failed; the best remains 90.5277 %.
* New measurements this pass (all with the exact NDK r18b clang 7.0.2 `-Oz`
  flags, gold link + strip + `reorder_shstrtab`, official single-function
  scorer):
  - faithful all-global, no trick, retry up: **75.0 % / 2228 B**
  - all-global + trick: **75.5 % / 2232 B**
  - all-global + trick + retry up / `< 30`: **75.5 % / 2232 B**
  - m47 base + retry up (`tries = -100; tries++`): **89.8 % / 2252 B**
  - m47 base + `while`-down: **90.5277 %** (identical code)
  - m47 base + `do/while`: 87.1 % / 2248 B
  - m47 base + 8-site mask with shipCount local: best 77.0 % (declaration
    has to move before `shipCount`, which breaks the 0–461 match)
  - branch-order permutations of the `missionPtr` dispatch: best 88.1 %
    (order 2, 4, 23); 74.0 % (23, 4, 2 = current), 61.2 % (4, 2, 23)
  - local aliases `heights`, `capacityTable`, `Station *st`, `ships`
    (ownHangar if/else): neutral 90.5277 %
  - local `enemies` / `status` / `globals`: 67.9 / 67.9 / 73.8 %
  - `spotUsed` as `char`/`bool`/`uint8_t`/`int8_t`: neutral 90.5277 %
  - `Vector` assignment-order permutations: xyz 90.5277 %, zyx 90.4 %,
    yxz/yxz/zxy/xzy 90.0–90.1 % (x/z assignment pairs emit `strd` and
    shift the loop-condition target by +2)
  - `Vector *rp = &rot; rp = ...`, `char *base = &rot`, `Vector rots[1]`:
    identical codegen, 90.5277 %
  - `AERandom **prnd = &rnd` / `AERandom *&rrnd = rnd`: identical codegen
  - `volatile` local `rnd` (outside or inside the loop): 78.0 / 76.7 %
  - post-loop RA perturbations (`this`/`Canvas` trick in the capacity
    index): 85.3 % / 2244 B
  - row-access forms (`const int (*row)[4]`, flat `const int *row`,
    `*(kHangarMeshes[race] + k)`): identical codegen
  - 120-variant random combination sweep over the ship-loop knobs: max
    90.5277 % (all top hits byte-identical to the shipped source)
* Gate green before and after: **byte_exact 4500/4524, linked_exact
  4500/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
  data_exact 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.**
  Structural state unchanged: 27 section headers, file size 2,187,328 B,
  `.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`,
  `.ARM.extab` `0x26298`.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6214
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (`/dev/shm/w6214`, not committed): `cc_scratch.sh` compiles
one copy of `Level.cpp` with the exact NDK r18b clang 7.0.2 `-Oz` flags,
`link_scratch.sh` re-runs the saved gold link + `strip --strip-all` +
`reorder_shstrtab.py` with the scratch `Level.o`, and `score.py` imports the
official verifier and prints the single-function percentage, sizes, and
first differing instruction. `align.py` prints difflib opcodes over the
normalized instruction stream.

## 2. Why the original is the all-global source

Original ship-loop setup (`0xb4924`):

```
b4924: ldr  r0, [pc, #632]        ; GOT slot of Globals::rnd
b4926: ldr  r1, [pc, #636]        ; GOT slot of kHangarCapacity
b4928: add  r0, pc
b492a: add  r1, pc
b492c: ldr.w r8, [r0]             ; r8 = &Globals::rnd
b4930: ldr  r0, [sp, #52]         ; sceneRace
b4932: ldr.w r6, [r1, r0, lsl #2] ; capacity -> r6
b4936: ldr.w r0, [r8]             ; Globals::rnd value
b493a: adds r1, r6, #1
b493c: blx  nextInt
```

and the same `ldr.w r0,[r8]` at `0xb49ae`, `0xb49be`, `0xb49ca`, `0xb49f6`,
`0xb4a46`, `0xb4a56`, `0xb4ad8`. That is exactly `Globals::rnd->nextInt(...)`
with the address of the global pointer hoisted once into `r8`. A local
`AERandom *rnd = Globals::rnd` caches the *value* and emits `mov r0,r5`.

## 3. Why the faithful source loses the early match

All-global source prologue vs original:

```
orig:  ldr r0,[pc,#928] ; add r0,pc ; ldr r0,[r0,#0] ; str r0,[sp,#36]
       ldr r0,[r0,#0] ; str r0,[sp,#68]
ours:  ldr r0,[pc,#920] ; add r0,pc ; ldr r5,[r0,#0] ; ldr r0,[r5,#0]
       str r0,[sp,#68] ; ... later str r5,[sp,#12]
```

The guard address is spilled immediately to `[sp,#36]` in the original but
kept in `r5` (and spilled later to `[sp,#12]`) in the all-global build. The
`missionPtr == 23` branch then uses `r4 = &Globals::status` instead of
`sl = &Globals::status`, and the parts-loop preheader computes the row into
`r8` instead of `r9`. The local `rnd` pointer (declared after `spotUsed`)
is the only source knob found that forces the original guard spill and
`sl = &status`; the `Vector` address trick then forces the parts-loop
`r8 = &Canvas` / `r9 = row` assignment.

## 4. The ship-loop permutation

With the shipped source (m47) the ship loop is:

| value | original | shipped |
|---|---|---|
| `&Globals::rnd` | `r8` | spilled `[sp,#44]` |
| `Globals::rnd` value | reload `[r8]` | cached `r5` |
| `capacity` | `r6` | `fp` |
| `race` / `tries` | `r5` | `r6` |
| `type` | `fp` | `r8` |
| `i` | `r9` | `r9` |
| `ship` | `sl` | `sl` |
| `spotUsed` | `r4` | `r4` |

Only the `r5`/`r6`/`r8`/`fp` group is permuted, and the extra
`ldr r0,[sp,#44]` / `ldr r0,[r0]` pair at the two global `rnd` sites
(sites 4 and 6) comes from spilling `&Globals::rnd`. Every attempt to free
`r5` for `race` (volatile local, pointer-to-pointer, in-loop declaration,
post-loop tricks, local aliases) either changes the early RA or adds
instructions; none beat 90.5277 %.

## 5. Bottom line

`Level::createScene` remains **90.5277 % / 2252 B** (783/790, first diff at
instruction 462). The faithful all-global source shape is **75.5 % /
2232 B**; the local `rnd` pointer is worth ~15 points of early match but
costs the entire ship-loop register assignment. The 93.27 % target remains
`-mllvm -sink-insts-to-avoid-spills` + no trick + `rnd` mask 35/37, which is
not landable per-TU. Worktree clean, gate green (byte 4500 / linked 4500 /
lint CLEAN / drift 0 / RATCHET PASS), sections unchanged.
