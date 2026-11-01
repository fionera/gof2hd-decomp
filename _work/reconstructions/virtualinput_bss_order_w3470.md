# VirtualInput `.bss` order (w3470)

Status: research note + landed source fix. Branch `wave87-w3470`, based on master
`b0f4540f`. All numbers were measured in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`,
image base 0x10000).

The measured baseline is **byte_exact 4408 / linked_exact 4496** (the handoff's
"4182" is stale; `check` below is the authority). This pass raises it to
**4413 / 4496** by reproducing the original's intra-TU `.bss` order for
`src/engine/input/VirtualInput.cpp`.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
```

Baseline `check`:

```
verify: byte 4408  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`drift`: `0 field(s) across 0 class(es)`. `diff`: `original: 5370  ours: 5370
extra: 0  missing: 0`. `lint`: `CLEAN`.

Baseline section table (unchanged by this pass):

```
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x6f78   @ 0x19cc08
.ARM.extab   0x2628c  @ 0x1a3b80
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

`.ARM.exidx`/`.ARM.extab` are already +8/-0xc versus the original at this baseline
(see `docs/IDENTICALITY.md`); this pass does not change either.

Baseline literal-pool classifier:

```sh
python3 _work/reconstructions/literal_pool_sweep_w3436.py \
    --json /tmp/w3470_base_classify.json
```

```
pct100 non-byte functions: 88  (same address 87, moved 1)
runs by kind: {'pool': 175, 'insn': 492}
functions by kind: {'pool': 66, 'insn': 24}
pool runs by (orig section, ours section):
  ('.rodata', '.rodata')                   148
  ('.bss', '.bss')                         27
pool functions by (orig section, ours section):
  ('.rodata', '.rodata')                   51
  ('.bss', '.bss')                         15
```

Five of the 15 `.bss` functions are VirtualInput: `SlowMotion`, `SpeedUp`,
`keyPressed`, `keyReleasedWithDelay`, `SendStoredKeyUpEvents`. The other ten are
other TUs (ModStation, Engine, SpaceLounge, StatusWindow, DialogueWindow,
GameText, NdkHooks) and are out of scope here.

## 2. Ground truth: original VirtualInput `.bss` order

The original is stripped, so the exported symbols come from `.dynsym` and the
local ones from the PIC destinations in the functions:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so |
    rg ' (stackX|stackY|wheelStack|g_pActivity)' | sort -k1,1
```

```
00216970 00000004 B g_pActivity
00217980 00000004 B stackX
00217984 00000004 B stackY
00217988 00000004 B wheelStack
```

The classifier resolves every differing `ldr/add pc` pool word in the five
VirtualInput functions to these original addresses:

```
g_delayedKeyCount  0x216974
g_delayedKeys      0x216978  (0x1000 bytes, ends 0x217978)
g_speedUp          0x217978
g_slowMotion       0x21797c
stackX             0x217980
stackY             0x217984
wheelStack         0x217988
g_simEdgesInit     0x21798c
g_simTouchActive   0x217990
g_input            0x217994  (16 bytes, ends 0x2179a4)
```

The whole block is `0x216974..0x2179a4` (0x1030 bytes) and the next exported
object is `ndk_APK_Path` at `0x2179a4`. The original order is therefore:

```
g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion,
stackX, stackY, wheelStack, g_simEdgesInit, g_simTouchActive, g_input
```

## 3. Why the tree emitted a different order

The baseline object (`nm -S --defined-only
cmake-build-match/CMakeFiles/gof2.dir/src/engine/input/VirtualInput.cpp.obj`)
had:

```
0x0000 stackX
0x0004 stackY
0x0008 wheelStack
0x000c g_speedUp
0x0010 g_slowMotion
0x0014 g_delayedKeyCount
0x0018 g_delayedKeys
0x1018 g_simEdgesInit
0x101c g_simTouchActive
0x1020 g_input
```

That is exactly clang 7.0.2's emission rule for this TU:

1. external-linkage definitions are emitted at their source position, before
   anonymous-namespace globals (so `stackX/Y/wheelStack` came first);
2. internal globals are emitted in first-use order, not declaration order
   (`g_speedUp`/`g_slowMotion` are first used in `keyPressed`, before
   `g_delayedKeyCount`/`g_delayedKeys` in `SendStoredKeyUpEvents`);
3. the `g_input` struct is split/optimized last (`g_input.0..3` at the tail).

The original block has the delayed-key pair first, then the speed flags, then
the three exported stack globals. That cannot be produced by a declaration
reorder alone (measured neutral in `literal_pool_sweep_w3437.md`).

## 4. Controlled A/B experiments

All rows are full `cmake --build cmake-build-match --target check -j8` runs of
this tree. No `--update-baseline` was used for the A/B rows.

| # | source change | byte | linked | `.bss` runs/functions | verdict |
|---|---|---:|---:|---:|---|
| 0 | baseline | 4408 | 4496 | 27 / 15 | — |
| E2 | move `stackX/Y/wheelStack` definitions after `keyReleasedWithDelay` | 4408 | 4496 | 27 / 15 | neutral |
| E3 | hidden-linkage `g_delayedKeyCount/g_delayedKeys` only | 4408 | 4496 | 27 / 15 | neutral |
| E4 | E3 + move `stackX/Y/wheelStack` after `keyReleasedWithDelay` | 4412 | 4496 | 17 / 11 | +4 |
| **E5** | **E4 + swap the two zero stores in the `Globals::keys[42]` branch** | **4413** | **4496** | **15 / 10** | **+5, landed** |

Object-level `.bss` order for the same experiments:

```
E2: g_speedUp, g_slowMotion, g_delayedKeyCount, g_delayedKeys,
    stackX, stackY, wheelStack, g_simEdgesInit, g_simTouchActive, g_input
E3: stackX, stackY, wheelStack, g_delayedKeyCount, g_delayedKeys,
    g_speedUp, g_slowMotion, g_simEdgesInit, g_simTouchActive, g_input
E4: g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion,
    stackX, stackY, wheelStack, g_simEdgesInit, g_simTouchActive, g_input
E5: same as E4 (the assignment swap does not change `.bss`)
```

E4 already has the original `.bss` order. The last +1 is a literal-pool ordering
detail inside `keyPressed`: the two `strb #0` stores in the
`Globals::keys[42]` branch are byte-identical either way, but clang orders their
two pool words by source order. The original has `g_speedUp` first; the
baseline source had `g_speedUp = 0; g_slowMotion = 0;`, which emitted
`g_slowMotion` first. Swapping the two assignments makes the pool words match
without changing the instruction stream.

## 5. Landed change

`src/engine/input/VirtualInput.cpp`:

* `g_delayedKeyCount` / `g_delayedKeys` are now **hidden external** globals
  (`#pragma GCC visibility push(hidden)`), not anonymous-namespace statics.
  This makes clang emit them before the anonymous-namespace scalars, while
  keeping them out of `.dynsym` exactly as in the original. The mechanism is
  already used in `Level.cpp` and `Status.cpp`.
* `stackX` / `stackY` / `wheelStack` are defined after `keyReleasedWithDelay`
  (still external, still exported), so clang emits them after the internal
  speed flags but before `simulateTouch`'s first use of `g_simEdgesInit`.
* The two zero stores in the `Globals::keys[42]` branch are written
  `g_slowMotion = 0; g_speedUp = 0;` to reproduce the original's pool order.

No linker flags, section-ordering entries, padding anchors, `asm`, or
`__attribute__` were added.

## 6. Result

After E5:

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
```

```
verify: byte 4413  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 4408 -> 4413 (lock with --update-baseline)
RATCHET PASS
```

`drift`: `0 field(s) across 0 class(es)`. `diff`: `original: 5370  ours: 5370
extra: 0  missing: 0`. `lint`: `CLEAN`.

The five fixed functions are exactly:

```
_Z10SlowMotionv
_Z7SpeedUpv
_Z10keyPressedPN11AbyssEngine6EngineEi
_Z20keyReleasedWithDelayPN11AbyssEngine6EngineEi
_Z21SendStoredKeyUpEventsPN11AbyssEngine6EngineEi
```

Classifier after the fix: `.bss` pool runs 27 -> 15, `.bss` pool functions
15 -> 10; no new `pct==100 && !byte` rows. The remaining 15 runs / 10 functions
are other TUs.

Section sizes/bases are unchanged from the baseline table above; `.text`,
`.rodata`, `.data.rel.ro`, `.got`, `.data` and `.bss` are size- and base-exact
versus the original. The `.ARM.exidx`/`.ARM.extab` +8/-0xc baseline residue is
unchanged.

## 7. Reproduction

```sh
# baseline and final gate
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8

# original VirtualInput .bss witnesses
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so |
    rg ' (stackX|stackY|wheelStack|g_pActivity)' | sort -k1,1
python3 _work/reconstructions/literal_pool_sweep_w3436.py \
    --json /tmp/w3470_classify.json

# our final .bss order (unstripped scratch relink with -Map)
/tmp/w3470_relink_map.sh
$NM -S --defined-only /tmp/w3470_unstripped.so |
    rg 'g_delayed|g_speedUp|g_slowMotion|stackX|stackY|wheelStack|g_simEdges|g_simTouch|g_input' |
    sort -k1,1
```

`/tmp/w3470_relink_map.sh` is the exact `ld.gold` command from
`cmake-build-match/build.ninja` with `-Map=/tmp/w3470.map` and without the final
`strip`; it is scratch-only.

## 8. Proven vs hypothesis

Proven (command-backed):

* The original VirtualInput `.bss` block is
  `g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion, stackX, stackY,
  wheelStack, g_simEdgesInit, g_simTouchActive, g_input` at
  `0x216974..0x2179a4`.
* Clang 7.0.2 emits external definitions at their source position and
  anonymous-namespace globals in first-use order for this TU; moving the
  stack definitions and making the delayed pair hidden external reproduces the
  original block exactly.
* E5 is a strict improvement: byte 4408 -> 4413, linked unchanged, all section
  sizes/bases unchanged, lint CLEAN, drift 0, symbol parity 0/0, imports 207.
* The `keyPressed` pool-word swap is real: the instruction stream is identical,
  but the two pool entries are ordered by source assignment order.

Hypothesis (not required for the fix):

* The original source may have used a different TU split or `static` plus a
  different function order rather than hidden external linkage; the binary
  cannot distinguish those source shapes because the symbols are stripped. The
  landed shape is the smallest source-only change that reproduces every observed
  address and passes the gate.
* The other ten `.bss` pool functions belong to other TUs and need their own
  first-use/definition-order reconstruction.
