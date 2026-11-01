# Literal-pool sweep round 3 (w3449)

Status: research note + one landed source fix. Branch `wave87-w3449`, based on master
`cfbd140e`. All numbers below were measured in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`,
image base 0x10000).

Scope: the `report.json` rows scored `pct == 100.0` but not `byte` — instruction stream and
every inter-function address already match, and only literal-pool words (plus branch
immediates that reference shifted PLT slots) still differ. This is the round-3 pass over
`literal_pool_audit.md`, `literal_pool_sweep_w3436.py`, `literal_pool_sweep_w3436.md` and
`literal_pool_sweep_w3437.md`.

## Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

| metric | value |
|---|---|
| byte_exact | **4254** |
| linked_exact | 4496 |
| compared | 4524 |
| avg_match | 99.89 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 |
| data exact | 843 / 843 |
| imports | 207 |
| lint | CLEAN |
| drift | 0 field(s) across 0 class(es) |
| `.text` | 0x1337c8 @ 0x69440 |
| `.ARM.exidx` / `.ARM.extab` | 0x6f70 / 0x26298 |
| `.rodata` | 0x41b67 @ 0x1c9e10 |
| `.data.rel.ro` / `.got` / `.data` / `.bss` | 0x2fe8 / 0x430c / 0x296c / 0x69ac @ 0x216970 |

Every ELF section is size- and base-exact at the baseline.

Classification driver (committed in `_work/reconstructions/literal_pool_sweep_w3436.py`):

```sh
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3449_baseline_classify.json
```

Baseline result: **242** `pct==100 && !byte` functions (97 same-address, 145 moved);
runs `{insn: 997, pool: 307}`. Pool runs by section: `.rodata` 197, `.got` 52, `.bss` 48,
`.data` 7, `.text` 2, `.data.rel.ro` 1.

## Landed fix: `.bss` COMMON block / VirtualInput `g_input` (byte 4254 -> 4262)

### Evidence

Eight VirtualInput functions differ only in one `.bss` literal-pool word:

```
_Z10MouseInputii, _Z11UseJoysticki, _Z11keyReleasedPN11AbyssEngine6EngineEi,
_Z12CaptureMousei, _Z12keyIsPressedv, _Z13simulateTouchPN11AbyssEngine6EngineE,
_Z14GetUseJoystickv, _Z9HideMousev
```

The classifier resolves the differing PIC word to the original variable address
`0x217994` for every one of them; in the baseline the same variable is at `0x2179b0`.
`0x217994` is VirtualInput's `InputState g_input` (16-byte struct, four ints).

The baseline `.bss` layout starts with the C runtime **COMMON block**:

```
0x216970 g_android_leaderboard_scores   (COMMON, from GOF2HD2012.c)
0x216990 g_android_current_achievements (COMMON)
0x21699c AccIndex                       (COMMON)
0x2169a4 SubkeyFile                     (COMMON)
...
0x2169cc g_pActivity                    (regular .bss, NFC.cpp)
0x2169d0 stackX                         (regular .bss, VirtualInput.cpp)
...
0x2179b0 g_input                        <- 0x1c too late
```

The original has the opposite order — every code TU's regular `.bss` first, the COMMON block
last:

```
0x216970 g_pActivity                    (regular)
0x217994 g_input                        (regular)
...
0x21d2c0 g_android_leaderboard_scores   (COMMON)
0x21d318 g_pVM                          (COMMON)
```

`gen_section_order.py` used to append `.bss*`, which gives every regular `.bss` input the
trailing ordering index. Gold emits the COMMON block (an `Output_section_data` with index 0)
*before* the indexed inputs, so the COMMON globals landed at the front. Leaving `.bss`
unlisted makes gold emit the COMMON block after the regular inputs at the same index,
reproducing the original.

### A/B table

Each row is a controlled relink of the same objects (`/tmp/w3449_relink*.sh` is the plain
gold link with `-Map`, unstripped; scratch only). `check`/`verify` numbers are from
`tools/verify/verify.py`; section numbers are `readelf -SW`.

| experiment | change | byte | linked | `.bss` | other sections |
|---|---|---:|---:|---|---|
| baseline | — | 4254 | 4496 | 0x69ac (exact) | exact |
| E2 | remove `.bss*` only | **4262** | 4496 | 0x69b8 (**+0xc**) | exact |
| E3 | E2 + S/P in a new data TU (normal link order) | 4262 | 4496 | 0x69a8 (-4) | exact |
| E5 | E3 + data TU linked after the archives | 4262 | 4496 | 0x69b0 (+4) | exact |
| V1 | `#pragma clang section bss=".bss.blowfish"` on S/P, `.bss.blowfish` listed, layoutPad still in `.bss.anchor` | 4262 | 4496 | 0x69b0 (+4) | exact |
| **V2 (landed)** | V1 + layoutPad moved to regular `.bss` | **4262** | **4496** | **0x69ac (exact)** | **exact** |
| data_sp (handoff) | S/P data TU + archive inserted after `libc++abi.a` | 4262 | 4496 | 0x69ac (exact) | exact |

V2 is the landed change: it is the only variant that is a *strict* improvement (byte +8,
linked unchanged, every section size/base unchanged) without a link-order/archive hack.

### Landed change (3 files)

`src/engine/file/BlowfishSubkeyFile.cpp` — the S/P arrays move to a private input section:

```c++
#pragma clang section bss=".bss.blowfish"
unsigned int S[4][256];
unsigned int P[18];
#pragma clang section bss=""
```

`src/engine/core/AbyssEngine.cpp` — `levelTextAnchor`'s `layoutPad` static drops its
`.bss.anchor` pragma, so it is emitted with the rest of that TU's `.bss` (the `.text` body is
untouched; `.text` stays 0x1337c8 @ 0x69440).

`tools/gen_section_order.py` — `.bss*` is replaced by `.bss.blowfish`; `.bss.anchor` stays as
a trailing sentinel for the empty-TU marker insertion (no object emits it any more).

### Result

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
```

```
verify: byte 4262  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 4254 -> 4262 (lock with --update-baseline)
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
SYMBOL PARITY ✓
```

Section table after the change (all size/base exact):

```
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26298  @ 0x1a3b78
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

Classification after the change: **234** `pct==100 && !byte` (89 same-address, 145 moved);
runs `{insn: 997, pool: 296}`; `.bss` pool runs 48 -> 37, `.bss` pool functions 29 -> 21.
The 8 fixed functions are exactly the VirtualInput list above; **no function regressed**
(diff of the two classification JSONs).

The `.rodata` content diff is unchanged by the fix (S/P are `.bss`):

```sh
python3 _work/reconstructions/rodata_content_diff_map.py \
    --unstripped /tmp/w3449_v2.so --map /tmp/w3449_v2.map
# differing bytes 13266 / 269159 (4.93%), first VMA 0x1fc390, last VMA 0x20377f
```

### `.bss` symbol layout after the change

```
0x216970 g_pActivity                      (regular, NFC.cpp)
0x217994 g_input                          (regular, VirtualInput.cpp)   <- now matches
0x21bfe4 levelTextAnchor::layoutPad       (regular, AbyssEngine.cpp)
0x21c278 g_android_leaderboard_scores     (COMMON)
0x21c2d0 g_pVM                            (COMMON)
0x21c2d4 S                                (regular, .bss.blowfish)
0x21d2d4 P                                (regular, .bss.blowfish)
```

Note: the original puts S/P *before* the COMMON block (`S 0x21c278`, `P 0x21d278`,
COMMON `0x21d2c0`); V2 puts them after it. The total `.bss` is still exact because the
`layoutPad` static fills the 4-byte gap the swap leaves. Getting S/P before the COMMON block
needs the S/P TU to be the *last* regular `.bss` input, i.e. linked after the archives
(the `data_sp` variant), which is a link-command change and was not landed.

## Proven vs still hypothesis

Proven (measured):

- The baseline `.bss` starts with the COMMON block because `.bss*` gives the regular inputs a
  trailing index; gold emits the COMMON block first in that configuration.
- Leaving `.bss` unlisted moves the COMMON block to the tail and moves VirtualInput's
  `g_input` from `0x2179b0` to `0x217994`, making the 8 listed functions byte-exact
  (4254 -> 4262, no regressions, linked unchanged).
- All ELF section sizes/bases stay exact in V2; `.rodata` content is unchanged.
- The `data_sp` variant (S/P in a data TU linked after the archives) also reaches 4262 with
  exact sections, but only via a link-order/archive change.

Hypothesis (not proven):

- The original's S/P-before-COMMON order is reachable from source alone by making the
  S/P TU the last regular `.bss` input. `E3`/`E5` show the data-TU link position controls
  that, but no source-only placement was found that both puts S/P before the COMMON block and
  keeps every section exact.
- The remaining `.bss` pool deltas (37 runs / 21 functions) are the other code TUs' regular
  `.bss` ordering, not yet reconstructed against the original link order.

## Commands used

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3449_baseline_classify.json

# after the landed change
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3449_v2_classify.json

# .bss symbol comparison (scratch unstripped relink with -Map)
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -S --defined-only /tmp/w3449_v2.so | grep -E ' S$| P$|g_pActivity|g_android_leaderboard|g_pVM|layoutPad|g_input'
```

`/tmp/w3449_relink.sh` is the same plain gold link the build uses, with `-Map` and without
`--strip-all`; it is scratch only, not part of the tree.
