# LevelScript::process — wave 183 / w12706: literal-pool / constant-island pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12706`, branch `wave183-w12706`, base master
`9555474d8` (wave-182 harvest, raw **52,412**, `.text` 75,015).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12706`.

**Result: no size-exact raw win landed; `src/` is at the base.**  The pass
maps the literal-pool layout, proves the pool *contents* are already correct,
and shows that every remaining pool-byte difference is a consequence of the
global code-layout/register-allocation delta, not of constant order.  The
diagnostic flag `-mllvm -arm-constant-island-max-iteration` has no effect.

## Baseline (verified on this worktree)

```
process raw diffbytes    52,412
process aligned insns    22,050
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728
.text diffbytes          75,015
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
symbol parity            5370/5370
RATCHET                  PASS
```

## Method

`scratch/w12706/pool.py` extracts the function body from either `.so` (dynsym
address/size, `.text` offset), disassembles it with the NDK r18b
`arm-linux-androideabi-objdump -D -b binary -m arm -M force-thumb`, and
resolves every PC-relative `ldr`/`vldr` literal load to its pool word.  A
linear sweep over a blob that contains embedded pools also decodes pool data as
instructions, so pool spans are reconstructed by clustering referenced word
addresses (gap <= 8 B) and merging overlaps; the resulting spans are a
superset of the real pools.  `scratch/w12706/pools_s.py` / `poolgroups.py`
parse the `clang -S` listing of the same TU (`process.s`) for the exact
`.p2align`/`.long`/`.short` island boundaries on our side.

`scratch/w12706/harness.py` compiles `LevelScript.cpp` with the exact
`compile_commands.json` command (optionally with extra `-mllvm` flags), runs
the exact `build.ninja` gold + GNU strip + `reorder_shstrtab.py` link into a
private `.so`, and measures with `measure_w11901.py` (same-address raw diff,
function size, `.text` diff, `.rel.plt` md5).  ~1.5 s per candidate.

## Pool layout

Our TU has **69 constant-island groups: 55 with `.long` literal constants and
15 with `.short` jump tables** (one group has both; from `process.s`).  The original has the same 15 `tbh` tables (same switch
structure; every table's non-default entries are at the same indices).  The
literal-pool starts (offset from function base) are:

| # | orig | ours | delta | # | orig | ours | delta |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | 0x038c | 0x03a8 | +28 | 28 | 0x78d4 | 0x7340 | -1428 |
| 1 | 0x0824 | 0x07fc | -40 | 29 | 0x7d68 | 0x7700 | -1640 |
| 2 | 0x0c78 | 0x0c68 | -16 | 30 | 0x81cc | 0x7b5c | -1648 |
| 3 | 0x10e4 | 0x102c | -184 | 31 | 0x8520 | 0x7ea8 | -1656 |
| 4 | 0x1604 | 0x13ec | -536 | 32 | 0x88dc | 0x8294 | -1608 |
| 5 | 0x19f0 | 0x17f8 | -504 | 33 | 0x8d04 | 0x8738 | -1484 |
| 6 | 0x1e24 | 0x1cd8 | -332 | 34 | 0x9188 | 0x8c1c | -1388 |
| 7 | 0x2244 | 0x21e8 | -92 | 35 | 0x9554 | 0x8f4c | -1544 |
| 8 | 0x25b4 | 0x2588 | -44 | 36 | 0x9974 | 0x9320 | -1620 |
| 9 | 0x290c | 0x2940 | +52 | 37 | 0x9dc4 | 0x9738 | -1676 |
| 10 | 0x2de0 | 0x2c90 | -336 | 38 | 0xa2cc | 0x9b64 | -1896 |
| 11 | 0x31ec | 0x2fec | -512 | 39 | 0xa608 | 0xa608 | **0** |
| 12 | 0x34b8 | 0x33d0 | -232 | 40 | 0xaa44 | 0xaa44 | **0** |
| 13 | 0x3838 | 0x381c | -28 | 41 | 0xad50 | 0xad5c | +12 |
| 14 | 0x3c10 | 0x39ec | -548 | 42 | 0xb0f4 | 0xb12c | +56 |
| 15 | 0x4084 | 0x3ba0 | -1252 | 43 | 0xb3e8 | 0xb6f0 | +776 |
| 16 | 0x4458 | 0x3de4 | -1652 | 44 | 0xb808 | 0xb12c | -1756 |
| 17 | 0x482c | 0x4224 | -1544 | 45 | 0xbc30 | 0xb334 | -2300 |
| 18 | 0x4bac | 0x4634 | -1400 | 46 | 0xc020 | 0xb6f0 | -2352 |
| 19 | 0x4fb8 | 0x4acc | -1260 | 47 | 0xc364 | 0xbaf4 | -2160 |
| 20 | 0x5560 | 0x4f64 | -1532 | 48 | 0xc74c | 0xbed8 | -2164 |
| 21 | 0x59c8 | 0x5424 | -1444 | 49 | 0xcb98 | 0xc210 | -2440 |
| 22 | 0x5e8c | 0x58d4 | -1464 | 50 | 0xcef8 | 0xc5dc | -2332 |
| 23 | 0x6294 | 0x5c88 | -1548 | 51 | 0xd300 | 0xc958 | -2472 |
| 24 | 0x66bc | 0x6124 | -1432 | 52 | 0xd74c | 0xcd9c | -2480 |
| 25 | 0x6b18 | 0x656c | -1452 | 53 | 0xdb74 | 0xcf78 | -3068 |
| 26 | 0x7074 | 0x6a10 | -1636 | 54 | 0xdf58 | 0xd338 | -3104 |
| 27 | 0x74a4 | 0x6f40 | -1380 | | | | |

Only two literal pools start at the same address (`0xa608`, `0xaa44`); the
first pool is 28 B late and the mid-function pools drift by 1.4–3.1 KB because
the code before them has different sizes.  Consequently the same-address raw
diff inside pool spans is **2,590 of 2,824 bytes** even though the pool
*sequences* are mostly identical.  Matching pool content at the same address
therefore requires matching the code size in front of each pool, which is the
known global register-allocation/block-placement delta, not a constant-order
problem.

## First pool: contents identical, placement +28 B

Original pool at `0x1380a4` (offset `0x38c`), ours at `0x1380c0`
(offset `0x3a8`).  Word-for-word (offsets from the pool start):

```
orig: d7fd2 d8400 37800000 40c90fdb c59c4000 3e4ccccd d83ec d83d2 [bf00bf00] 9999999a 3fb99999 c63b8000 ...
ours: d7fd2 d8400 37800000 40c90fdb c59c4000 3e4ccccd d83e6 d83cc  9999999a 3fb99999 c63b8000 ...
```

The order and the numeric constants are identical.  The only differences are

* the two `GOT_PREL` words (`d83ec`/`d83e6` and `d83d2`/`d83cc`), whose values
  are `symbol - (add_pc + 4 - pool_addr)` and therefore change with the
  address of the `add r0, pc` that consumes them; and
* the 4-byte alignment pad: the original pool starts at `0x38c` (4 mod 8), so
  the `double 0.1` needs a `bf00bf00` pad; our pool starts at `0x3a8`
  (0 mod 8), so the double is already 8-aligned and the pad disappears.

The +28 B first-block delta decomposes as:

* **+6 B prologue register allocation.**  The original keeps
  `&Globals::status` in `r4` and reloads the pointer: `ldr r4,[r0]; ldr
  r0,[r4]` (2+2 B).  Ours keeps `Globals::status` (the pointer) in `r4` and
  `&Globals::status` in `r9`: `ldr.w r9,[r0]; ldr.w r4,[r9]; mov r0,r4`
  (4+4+2 B).  The `r9` encoding is wide, and the extra `mov` is the cost.
  This is the same `this`=`fp` / `status`=`sl` swap already documented in
  wave 181 (w12505): the original has `this`=`sl`, canary=`fp`,
  `&status`=`r4`; ours has `this`=`fp`, canary=`sl`, `&status`=`r9`.
* **-4 B mission==1 block.**  The original orders the calls
  `LODManager::forceUpdate(delta,true)` -> `PlayerEgo::rotate` ->
  `getPosition` -> `setToSleep`; our source orders `rotate` -> `setToSleep`
  -> `getPosition` -> `forceUpdate`.  The original's order is 4 B *larger*
  here (the pool block is not the whole story).
* **+26 B shared tail / mission==0 state machine** from instruction
  scheduling and the same register-assignment delta (e.g. ours emits
  `mov r6,fp; mov fp,sl` where the original reuses `r4`/`sl` directly).

The `Status *status` local is not the cause of the `r9` choice: replacing it
with `Globals::status->` everywhere (or `Status *&status = Globals::status`)
does change the prologue to the original's *reload* shape, but the allocator
still picks `r9`, so raw moves 52,412 -> 52,413 (+1), not a win.

## Constant multiset is correct

Classifying every referenced pool word above `0x100000` as a constant
(excluding `GOT_PREL` offsets, function pointers and 16-bit jump-table
entries) and comparing the multisets of the two functions:

* 190 distinct constants in ours vs 191 in the original; the only
  count differences are duplicate entries the original needs for range:
  `0.8` (`3f4ccccd`) x3 vs x2, `-500` (`c3fa0000`) x3 vs x2, and the integer
  `-40000` (`ffff63c0`) x2 vs x1.  These are pool-placement duplicates, not
  missing/wrong literals.
* All the values that appear in only one binary are code words that the
  linear sweep mis-decoded as pool words (e.g. `6800ad46`, `f50d9d16`), not
  real constants.

So there is no wrong/missing constant to fix; the source's literal values and
their relative order are already faithful.

## `-mllvm -arm-constant-island-max-iteration` diagnostic

Compiled with the exact command plus `-mllvm -arm-constant-island-max-iteration=N`:

| N | raw | size | `.rel.plt` |
|---:|---:|---|---|
| 2 | 52,412 | 57232/57232 | 289b2a06… |
| 3 | 52,412 | 57232/57232 | 289b2a06… |
| 4 | 52,412 | 57232/57232 | 289b2a06… |
| 5 | 52,412 | 57232/57232 | 289b2a06… |
| 10 | 52,412 | 57232/57232 | 289b2a06… |
| 20 | 52,412 | 57232/57232 | 289b2a06… |
| 50 | 52,412 | 57232/57232 | 289b2a06… |
| 100 | 52,412 | 57232/57232 | 289b2a06… |

The pass converges after 2 iterations and the result is bit-identical
afterwards; `N=0` and `N=1` crash clang 7.0.2 (assertion).  The flag is a
no-op for this function and is not a candidate for landing.

## Source-order experiments (all size-exact, `.rel.plt`-identical)

Every candidate below was compiled + gold-linked + stripped with the real
recipe and measured with `measure_w11901.py`.

| candidate | raw | delta |
|---|---:|---:|
| base | 52,412 | — |
| mission==1: `forceUpdate` first, keep `setToSleep` before `getPosition` | 52,415 | +3 |
| mission==1: `forceUpdate` first + `getPosition` before `setToSleep` (original order) | 52,416 | +4 |
| mission==1: `getPosition` before `setToSleep` only | 52,413 | +1 |
| mission==0 state 0: hoist `m_nState = 1` above both `setPosition` calls | 52,416 | +4 |
| swap state-8 and state-9 `else if` arms (behaviour-neutral) | 52,412 | 0 (identical code) |
| `Status *status` -> `Globals::status->` everywhere (drop local) | 52,413 | +1 |
| `Status *&status = Globals::status` (reference, drop pointer local) | 52,413 | +1 |
| reference declared first (before `messages/player/enemies`) | 52,413 | +1 |
| local drop + mission==1 `forceUpdate` first | 52,416 | +4 |
| local drop + `getPosition` before `setToSleep` | 52,414 | +2 |
| drop `-mllvm -disable-postra-machine-sink` (diagnostic only) | 52,414 | +2 |

The state-8/9 swap producing byte-identical code shows the `else if` source
order is not what controls block placement here; the scheduler/RA does.

## Why the pools cannot be aligned by constant reordering

For a pool to contribute a raw win, its words must match the original's words
*at the same address*.  Because the code before each pool differs in size, the
pool starts differ (table above).  For the first pool the original's word at
our pool start (`0x3a8`) is its index 7 (`d83d2`, a relocation-dependent GOT
offset); to match it our pool would have to start with that word, i.e. the
first eight constants (`__stack_chk_guard` GOT, `Globals::status` GOT,
1.5258789e-5, 2*pi, -5000, 0.2, two `Globals::layout` GOTs) would have to be
first-used *after* the double 0.1.  Those are the very first constants used in
the prologue, so the only way to achieve it is to make the first block 28 B
shorter - i.e. reproduce the original's prologue register assignment and
scheduling, not reorder constants.

Similarly, in the well-aligned region `0xa2cc-0xb3e8` the two pools that do
start at the same address (`0xa608`, `0xaa44`) already have identical content
and differ only in relocation offsets (4/2/0x16/0x14 B), which are fixed by
the address of the consuming `add r0, pc`; and the pools at `0xad50`/`0xad5c`
(+12) are content-identical but shifted.

## Recommendation for the next pass

To make the first pool (and the whole pool chain) align, attack the prologue
register assignment: the original's `{this=sl, canary=fp, &status=r4}` versus
ours `{this=fp, canary=sl, &status=r9}`.  That single rotation is worth 6 B in
the prologue and, if it also fixes the shared-tail scheduling, would move the
first pool from `0x3a8` to `0x38c` and let the (already identical) first-pool
content match.  The `Status *status` local shape is not the lever (both the
pointer local and the reference local leave `r9`); a source construct that
changes the callee-saved interval assignment (e.g. a different first-use
order of `this` vs `Globals::status`) is the remaining unexplored angle.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12706
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52412 aligned= 22050 size= 57232/57232 insn= 20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52412 size=57232/57232 text=75015 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`pool.py`, `pools_s.py`, `poolgroups.py`, `poolseq.py`,
`poolcmp.py`, `locate.py`, `harness.py`, `measure.py`, `process.s`,
`pools_bin.txt`, `orig_pool.txt`, `ours_pool.txt`) live in
`/home/debian/GalaxyOnFire2/scratch/w12706/`; they are scratch, not tracked.
