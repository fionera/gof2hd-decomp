# `.ARM.extab` LSDA scopes: `Radar::Radar` block-placement analysis (w7720, third pass)

Status: research note, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7720`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

No source change landed this pass (gate stays at the w7720b baseline:
`.ARM.extab` differing bytes **1417**, records **12**, byte_exact/linked_exact
**4506/4506**, size 4524, data 843, imports 207, lint CLEAN, drift 0,
`.text` 85,379, `.rel.plt` 0).  This note records the decoded structure and the
new hypotheses that were tested, so the next wave does not repeat them.

## `Radar::Radar` (`0x12d484`, 2488 B, body byte-exact)

The only remaining record owned by a byte-exact body.  Raw record diff is 7
bytes, all in the call-site table:

```
row  orig pad  ours pad
 1     2462      2464     (String::String member ctor)
 3     2462      2464     (early labelStrings invoke)
11     2462      2464     (labelStrings invoke)
13     2462      2464     (labelStrings invoke)
15     2462      2464     (labelStrings invoke)
19     2464      2462     (Level::getEnemyGuns)
21     2464      2462     (first Image2DCreate group)
20     2466      2466     (enemyGuns loop body: isRocketGun/setRadar)
```

Pads 2462/2464/2466 are three consecutive identical `b.n 0x9a4` trampolines
(0x99e/0x9a0/0x9a2) that all destroy the `String` member at `this+0x10` and
resume.  The **body is byte-identical** (verified against the linked `.so`:
0 differing bytes over 2488), so this is purely a layout-order swap of two
identical landing-pad blocks.

### Why the order is what it is

`MachineBlockPlacement.cpp` (`selectBestCandidateBlock`, LLVM 7 release/7.x)
lays out EH pads **least-probable first**:

```cpp
    // For ehpad, we layout the least probable first ...
    if (BestBlock && (IsEHPad ^ (BestFreq >= CandidateFreq)))
      continue;
```

The two pads are machine blocks `bb.210` (39 predecessors, the post-
`getEnemyGuns` / Image2DCreate scope) and `bb.211` (16 predecessors, the early
outer scope).  `-mllvm -print-bfi` gives:

```
BB210: float = 0.00000047683, int = 21     <- enemyGuns scope
BB211: float = 0.0000023842,  int = 107    <- outer scope
```

21 < 107, so the enemyGuns pad (bb.210) is placed at the lower address 2462
and the outer pad (bb.211) at 2464.  That is **ours**.  The original has the
outer scope at 2462, i.e. the original's two pad frequencies must be ordered
the other way (outer lower than enemyGuns).  The frequencies are computed from
the CFG + invoke unwind probability (1/1048576, uniform), so the original's IR
must differ in a way that changes the *relative* block frequencies without
changing the emitted machine code.

### Source variants tested this pass (all measured on the isolated TU)

All of these keep the 2488-byte body unless `same=False`:

| variant | body same | rows (1,3,11,13,15,19,21) |
|---|---|---|
| base | yes | 2464,2464,2464,2464,2464,2462,2462 |
| `enemyGuns` declared at ctor top, assigned in place | no | 2418,2416,2392,2424,2390,2468,2466 |
| `enemyGuns` declared+initialised at ctor top | yes | unchanged |
| `playerGuns` declared at ctor top, assigned in place | no | 2418,2416,2392,2424,2390,2468,2466 |
| both declared at ctor top | no | 2418,2416,2392,2424,2390,2466,2464 |
| `enemyGuns` declared before `playerGuns` decl | no | 2460,2460,2460,2460,2460,2462,2462 |
| `enemyGuns` declared before `playerGuns` if | yes | unchanged |
| swap `playerGuns`/`enemyGuns` blocks | yes | unchanged |
| `auto`/const-ref/`=0`/`=nullptr` forms | mostly yes | unchanged |
| `{ }` around labelStrings / playerGuns / whole ctor | yes | unchanged |
| `{ }` around enemyGuns block, end before Image2DCreate | no (14 B) | **2462,2462,2462,2462,2462,2368,2464** |
| `if (Array<...> *enemyGuns = getEnemyGuns())` | no (14 B) | **2462,2462,2462,2462,2462,2368,2464** |
| `{ }` around enemyGuns block, end after Image2DCreate | yes | unchanged |
| dummy `int`/`volatile int` at top / before labelStrings / before playerGuns / before enemyGuns | no | 2420..2474 range |

Key negative results:
* Moving statements (labelStrings block after the gun blocks, gun blocks
  swapped, etc.) is reordered back by the scheduler: **same IR, same rows**.
* Moving a *declaration* to the ctor top adds a lifetime-marker scope, which
  changes the cleanup chain and the body (the `eg_top` family).
* `{ }` scopes with no destructor inside are free (body unchanged) and do not
  move the pads.

### Closest variants (14-byte body delta, early rows already correct)

`eg_scope2` and `if_decl` both produce exactly the original's early rows
(2462) and keep the 2488-byte size, but change rows 19/21 to
**2368 / 2464** instead of 2464 / 2464.  The 14 differing body bytes are all
in the cleanup chain at offsets 2358..2390:

```
2358, 2362, 2368, 2370..2374, 2384..2386, 2388..2390
```

Row 19 is the `Level::getEnemyGuns` invoke; in these variants it unwinds to
the `String`-temporary cleanup pad 2368 instead of the enemyGuns pad, because
the enemyGuns lifetime scope now ends before the Image2DCreate group.  The
original keeps `getEnemyGuns` and the Image2DCreate group in the **same**
scope (both 2464), so the enemyGuns scope must extend past the Image2DCreate
group.  **Next hypothesis:** find a declaration form whose lifetime scope
starts at `getEnemyGuns` and extends to the end of the ctor while still
pushing the outer function-level scope *after* it in the EH stack — i.e. flip
the two pads without truncating the enemyGuns scope.  A `for`/`switch`
init-statement or a declaration in a compound statement that closes after the
Image2DCreate group is the untested shape.

## `Level::createScene` (`0xb42ec`, 2252 B) — not a pure swap

The 9-byte record diff is a systematic **+2** shift of the pads below 2164
(orig 2150/2148/2146/2144/2152/2158/2156/2142/2154 vs ours
2148/2146/2144/2142/2150/2156/2154/2140/2152), while pads >= 2164 are equal.
The trampolines at 0x85c..0x86c are `b.n` pairs with alternating targets
(0x88c vs 0x87a), so the original's rows point at the *next* trampoline and
some cleanup targets differ.  This is not the two-identical-trampoline swap;
it needs the source scope structure of the early `missionPtr == 23` block.
The body is **not** byte-exact (236 differing bytes), so this record will
close with the body.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7720
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1417, records 12
```

Scratch harnesses: `/dev/shm/w7720/cc_radar.sh`, `objlsda.py`,
`radtry9.py`..`radtry20.py`, `/tmp/mi.txt` (`-mllvm -print-machineinstrs`),
`/tmp/bfi2.txt` (`-mllvm -print-bfi`), `mbp.cpp`/`ehs.cpp`/`bpi.cpp`
(LLVM 7 release/7.x sources fetched for the placement rule).
