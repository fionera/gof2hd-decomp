# `Level::createScene` — wave-119 w6961: the `-sink-insts-to-avoid-spills` mechanism

**Status:** no landable source win over **92.7573 %** (2252 B). Worktree
`wt/w6961`, branch `wave119-w6961` (base master `401857181`). Target
`_ZN5Level11createSceneEv`, `src/game/world/Level.cpp`. This note records a
new MIR-level diagnosis of the 93.2656 % flag basin and a fresh batch of
source measurements. Source untouched.

Continuation of `level-createscene-wave119-w6929.md` (92.7573 % local maximum),
`…-w6934.md` (the ctor + `-sink-insts-to-avoid-spills` 93.2656 % basin),
`…-w6903.md` (measurement tables) and `…-w6820.md` (MIR).

## TL;DR

* Baseline and best: **92.7573 % / 2252 B**, 791/783 instructions, first
  diff at normalised instruction 478 (parts-loop store schedule).
* **New:** the `-mllvm -sink-insts-to-avoid-spills` flag is a **MachineLICM**
  option, not a MachineSink/post-RA one (the w6934 note guessed "during/after
  register allocation"). Its entire effect is to stop Early MachineLICM from
  hoisting the loop-invariant **constant materialisations** (`t2MOVi
  192/304/-1/100/4/3/300/7`, `t2MOVi16 14348`, and `t2ADDri %stack.1`) from
  the loop preheader into the use blocks. That reduced preheader pressure is
  what flips greedy RA into the original's parts-loop allocation.
* The plain `Vector rot(0.0f, 3.1415927f, 0.0f)` form is **instruction-exact
  for the whole parts loop except the zero's register**: it emits
  `movs r4,#0` / `str r4` where the original has `mov.w sl,#0` / `str.w sl`,
  and it spills `k` (`str r6,[sp,#44]`). Same store order (y, x, z), same
  `add r1, sp, #56`. Only the 4-byte high-register encoding differs.
* The landed `- (size_t) g` trick gets the zero into `sl` but costs the
  `add r0,sp,#56; subs r1,r0,r5` pair (the post-RA ARM load-store optimiser
  then merges the x/y stores into `strd`). Folding the subtraction back to
  `&rot` while keeping the RA effect remains unsolved.
* Gate re-measured green: **byte_exact 4505/4524, linked_exact 4505/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN, drift 0, RATCHET PASS**, 27 sections, file
  2,187,328 B. No section or `.ARM.extab` change (source untouched).

## 1. The flag mechanism (MIR diff, new)

`clang -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level11createSceneEv`
with the plain ctor form, once without and once with
`-mllvm -sink-insts-to-avoid-spills`, gives 58 identical passes; the **first**
differing pass is *Early Machine Loop Invariant Code Motion*. The whole diff is
constant materialisations moving from the preheader into the use blocks:

```
bb.8 (preheader)                       bb.13/14 (parts loop body)
  %534 = t2MOVi 192        -->           %534 = t2MOVi 192   (bb.13)
  %545 = t2ADDri %stack.1  -->           %545 = t2ADDri %stack.1 (bb.14)
  %546 = t2MOVi 304        -->           %546 = t2MOVi 304   (bb.14)
  %549 = t2MOVi -1         -->           %549 = t2MOVi -1    (bb.14)
```

and the same for `t2MOVi 100` (ship-loop site B), `t2MOVi 4` (site C),
`t2MOVi 3` (site E), `t2MOVi 300` (site H), `t2MOVi 7` (mission-4 seat loop),
`t2MOVi16 14348`, and the mission-4 `192/304/-1` triple. Every later pass
differs only because RA has already diverged.

So the flag is not a post-RA scheduler knob: it is a **pre-RA MachineLICM
hoisting heuristic**. The w6934 note's "the difference is made during/after
register allocation" is wrong; the flag changes the pre-RA MIR (the
`Eliminate PHIs` dump differs by exactly these sunk instructions).

## 2. The parts loop is one register away

Plain ctor form (84.9524 %, 2248 B, first diff 28):

```
b4860: ldr   r0, [pc, #804]   ; pi
b4862: add   r1, sp, #56      ; &rot   (2-byte encoding)
b4864: str   r0, [sp, #60]    ; y
b4866: movs  r4, #0           ; zero in r4
b4868: mov   r0, r5           ; g
b486a: str   r4, [sp, #56]    ; x
b486c: str   r6, [sp, #44]    ; k spilled
b486e: str   r4, [sp, #64]    ; z
b4870: blx   setRotation
```

Original (28 B, 8 instructions):

```
b4860: ldr   r0, [pc, #808]   ; pi
b4862: add   r1, sp, #56      ; &rot
b4864: str   r0, [sp, #60]    ; y
b4866: mov.w sl, #0           ; zero in sl
b486a: mov   r0, r5           ; g
b486c: str.w sl, [sp, #56]    ; x
b4870: str.w sl, [sp, #64]    ; z
b4874: blx   setRotation
```

The store order, the `add r1, sp, #56` and the `mov r0, r5` position already
match. The only differences are (a) the zero's register (`r4` vs `sl`, which
is a 2-byte vs 4-byte encoding for the `mov` and both `str`s) and (b) the
`k` spill. The zero is live across the `setRotation` call because the same
constant feeds the following `PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f)`
(`strd r4,r4,[sp]` vs the original's `strd sl,sl,[sp]`), so it is a
callee-saved value in both builds.

The landed `- (size_t) g` trick gets the zero into `sl` (and `hangarPart`
into `r4`, `k` stays in `r6`) but costs `add r0,sp,#56; subs r1,r0,r5`
(4 bytes instead of the 2-byte `add r1,sp,#56`), which the post-RA ARM
load-store optimiser then hides by merging the x/y stores into
`strd sl,r0,[sp,#56]`. Net: 26 B vs the original's 28 B, and the whole
parts loop differs.

`clang -mllvm -arm-load-store-opt=false` (the ARM pass name is
`arm-load-store-opt`, not `arm-ldst-opt`) does split the `strd` again, but it
also changes the **pre-RA** load/store pass and drops the whole function into
Basin A: **73.9570 % / 2240 B / fd 6**.

## 3. Measurements (this session, all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib ratio).
`fd` is the first differing normalised instruction index. Landed baseline
`92.7573 % / fd 478`.

### 3.1 Flag / pass probes (per-file `COMPILE_OPTIONS`, reverted)

| variant | pct | size | fd |
|---|---|---|---|
| plain ctor + `-sink-insts-to-avoid-spills` | **93.2656** | 2252 | 548 |
| landed trick + `-sink-insts-to-avoid-spills` | 89.7078 | 2252 | 28 |
| plain ctor + `-disable-machine-licm` | 70.8812 | 2228 | 3 |
| plain ctor + `-hoist-cheap-insts=true` | 84.9524 | 2248 | 28 |
| landed trick + `-arm-load-store-opt=false` | 73.9570 | 2240 | 6 |

The 93.2656 % build is still TU-wide: `check` reports byte_exact
4505 -> 2357 because the +100 B `.text` growth shifts `.rodata` (the w6934
note's numbers still hold).

### 3.2 Parts-loop source probes (landed ship loop)

| variant | pct | size | fd |
|---|---|---|---|
| landed y/z/x + trick | **92.7573** | 2252 | 478 |
| `g->setRotation(*(Vector *)((char *)&rot - (size_t)g))` (no `rp`) | 92.7573 | 2252 | 478 |
| `(uintptr_t)&rot - (uintptr_t)g` | 92.7573 | 2252 | 478 |
| `(char *)&rot - (char *)g` | 92.7573 | 2252 | 478 |
| `rp` declared before the stores | 92.7573 | 2252 | 478 |
| `g->setRotation(rp[0])` | 92.7573 | 2252 | 478 |
| `Vector rot(0,pi,0)` plain (ctor/brace/copy/inline/`const`) | 84.9524 | 2248 | 28 |
| field y/z/x plain, field y/x/z plain, `Vector rot; rot.y=pi` plain | 84.8793 | 2248 | 28 |
| `Vector rot` declared before `new AEGeometry` | 85.4061 | 2256 | 28 |
| `Vector rot` declared outside the `k` loop (plain / ctor) | 84.3175 | 2256 | 28 |
| nested `{ Vector rot(...); g->setRotation(rot); }` block | 84.9524 | 2248 | 28 |
| `Vector rot(0.0f, pi, -0.0f)` (adds a 4-byte `mov.w`) | 88.1980 | 2252 | 460 |
| `PlayerStatic(..., rot.x, rot.x, rot.x)` | 84.6641 | 2260 | 28 |
| `#line` directive before the parts loop (3 values) | 84.9524 | 2248 | 28 |

The `-0.0f` probe is informative: a single extra 4-byte constant in the
parts-loop block flips the `kHangarMeshes` base from `r9` to `r8` and the
`Canvas` base the other way (first diff jumps back to 460), i.e. the block
contents, not just the source order, drive the allocation.

### 3.3 Ship-loop probes

| variant | pct | size | fd |
|---|---|---|---|
| landed (local `rnd` at B, C, retry) | **92.7573** | 2252 | 478 |
| `unsigned char *const spotUsed` | 92.7573 | 2252 | 478 |
| `AERandom *const rnd` | 92.7573 | 2252 | 478 |
| `AERandom **rndp = &rnd` + `(*rndp)->` at B/C/retry | 92.7573 | 2252 | 478 |
| `unsigned shipCount` | 92.7573 | 2252 | 478 |
| `char *spotUsed` | 92.7573 | 2252 | 478 |
| local `rnd` also at site H | 92.3761 | 2252 | 28 |
| local `rnd` also at site E | 91.8679 | 2252 | 28 |
| local `rnd` also at site D | 91.7408 | 2252 | 28 |
| `size_t capacity` / `unsigned capacity` | 92.6302 | 2252 | 478 |
| `rnd` declared just before the retry loop | 72.6349 | 2232 | 5 |
| `rnd` declared just before `spot` | 72.3243 | 2228 | 5 |
| `rnd` used at `shipCount` + B/C/retry (decl before capacity) | 70.7595 | 2236 | 6 |
| `unsigned char spotUsed[16]` on the stack | 68.2741 | 2228 | 4 |
| all-global `rnd` (no local) | 75.4286 | 2232 | 6 |
| all-global `rnd` + plain ctor parts loop | 75.0318 | 2228 | 6 |

Nothing bridges the two basins: the local `rnd` value must be live across
the ship loop to pin Basin B, and its existence is exactly what makes the
allocator spill `&Globals::rnd` instead of caching it.

## 4. Conclusion

The remaining gap is one register decision in each of the two regions:

* **Parts loop:** the plain ctor form already produces the original's
  instruction sequence; only the zero's register (`r4` vs `sl`) and the
  resulting `k` spill differ. The `- (size_t) g` trick is the only lever
  found that moves the zero to `sl`, and it necessarily adds the `subs`.
* **Ship loop:** the original reloads `Globals::rnd` at every site
  (`ldr.w r0,[r8]`) and keeps `&Globals::rnd` in `r8`; the landed source
  caches the value in `r5` and spills the address. All-global source
  reproduces the reload shape but collapses the whole function to Basin A.

A future pass should look for a source shape that makes the zero land in a
high register **without** an extra instruction (e.g. by changing what else
is live across the `setRotation` call), or a way to make the allocator spill
the cached `rnd` value instead of its address. Per-file `-mllvm` flags are
not landable: the only basin-flipping one (`-sink-insts-to-avoid-spills`)
changes 12 `Level.cpp` functions and shifts `.rodata`.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6961
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

MIR reproduction (plain ctor form in place):

```sh
clang++ <Level.cpp compile line> -o /tmp/pa.o \
  -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level11createSceneEv \
  -c src/game/world/Level.cpp > /tmp/pa_cp.txt 2>&1
# add -mllvm -sink-insts-to-avoid-spills for the flag build
```

Scratch harness (not committed): `/dev/shm/w6961/` — `Level.pristine.cpp`,
`createScene.base.cpp` (landed body), `try.py` / `batch.py` / `score.py`
(official verifier import; `batch.py` re-reads the ELF after each build),
`gen.py` / `mkbody.py` (parts-loop variant generator), `flagtest.sh`
(per-file `COMPILE_OPTIONS` A/B), `cmd_pa.sh` (`-print-after-all`),
`pa_cp.txt` / `pa_cpf.txt` (the two MIR dumps diffed in §1),
`v_*.cpp` (all measured variants).
