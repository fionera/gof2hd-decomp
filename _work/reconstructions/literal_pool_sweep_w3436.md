# Literal-pool sweep (w3436)

Branch `wave87-w3436`, based on master `28057ed7`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`,
image base 0x10000). Driver script: `_work/reconstructions/literal_pool_sweep_w3436.py`
(committed with this note; it reuses `tools/verify/verify.py`'s pool/jump-table scanner and
`elf.Elf32`).

## Result

| | byte_exact | linked_exact | pct==100 && !byte |
|---|---:|---:|---:|
| baseline (this tree before the pass) | 4233 | 4495 | 263 |
| after this pass | **4248** | **4495** | **248** |

One source change landed: `alignas(16)` on `Hud.cpp`'s `g_Hud_raceBadge`. It is a strict
**+15 with 0 lost functions**, and no section size/base changed:

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

`lint` CLEAN, `drift` 0 (`=== DRIFT: 0 field(s) across 0 class(es) ===`). The `check` target
prints `GAIN byte_exact 4233 -> 4248 (lock with --update-baseline)` and `RATCHET PASS`.

## Commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8        # baseline: byte 4233 / linked 4495
# ... edit src/game/ui/Hud.cpp line 43 (see A/B table) ...
cmake --build cmake-build-match --target check -j8        # after:    byte 4248 / linked 4495
cmake --build cmake-build-match --target drift -j8        # 0 drifted fields

# classification of the remaining pct==100 && !byte rows
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/literal_sweep_after.json

# .rodata content diff (needs an unstripped relink with -Map; see /tmp/w3436_relink_map.sh)
/tmp/w3436_relink_map.sh /tmp/w3436_align16_unstripped.so /tmp/w3436_align16.map
python3 _work/reconstructions/rodata_content_diff_map.py \
    --unstripped /tmp/w3436_align16_unstripped.so --map /tmp/w3436_align16.map
```

`/tmp/w3436_relink_map.sh` is the same plain gold link the build uses, with `-Map` and without
`--strip-all`; it is scratch only, not part of the tree. Its body is:

```sh
B=cmake-build-match
NDK=.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
"$GOLD" -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  --section-ordering-file=$B/section_order.txt -Map=/tmp/w3436_align16.map \
  -o /tmp/w3436_align16_unstripped.so \
  $B/crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @$B/exidx_markers/link_order.rsp \
  --whole-archive $B/exidx_markers/archives/libz_gof2.a \
                 $B/exidx_markers/archives/libzip_gof2.a --no-whole-archive \
  $B/exidx_markers/archives/libcrypto_gof2.a \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  ../_work/bins/apk_libs/libfmodex.so ../_work/bins/apk_libs/libfmodevent.so \
  $B/exidx_markers/archives/libc++abi.a -l m \
  $B/exidx_markers/archives/libandroid_support.a \
  $B/exidx_markers/archives/libgcc.a -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

## The fix: `g_Hud_raceBadge` must not sit in the shared 4-byte `.rodata.cst16` merge block

### Original-binary evidence

`Hud::init` (`_ZN3Hud4initEv`, `0x1604e4`, size `0xd88`) reads the race table at `0x16119a`:

```
16119a: 4930        ldr  r1, [pc, #192]   ; literal at 0x16125c = 0x000a25b6
16119e: 4479        add  r1, pc           ; r1 = 0x1611a2 + 0xa25b6 = 0x203758
1611a0: f831 1020   ldrh.w r1, [r1, r0, lsl #2]   ; r0 = SolarSystem::getRace()
```

The table at `0x203758` is:

```
0x203758: a6 04 00 00 a3 04 00 00 a5 04 00 00 a4 04 00 00
          = int[4]{1190, 1187, 1189, 1188}
```

It is a 32-bit `int[4]` (stride 4 via `lsl #2`); `ldrh.w` only narrows the value to the
`unsigned short` image-id parameter of `PaintCanvas::Image2DCreate`. The effective PC of
`add rX, pc` is `instruction + 4` without clearing bit 1: cross-checked on
`Hud::drawOrbitInformation`, whose `add r0, pc` at `0x1661b6` + literal `0x9d5c6` lands on
`g_Hud_securityColors` at `0x203780`, whose first entry is the source's `{255, 42, 0}`.

Original `.rodata` around the table:

```
0x203750: 74 47 75 6e 00 00 00 00  a6 04 00 00 a3 04 00 00   "tGun\0\0\0\0" then table
0x203760: a5 04 00 00 a4 04 00 00  ff ff ff ff ff ff ff ff
0x203770: ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff
0x203780: ff 00 00 00 2a 00 00 00  00 00 00 00 ff 00 00 00   g_Hud_securityColors
```

### Why the baseline was +0x10

The baseline `Hud.cpp.obj` has `.rodata.cst16` with section alignment 4 (`2**2`). gold merges
it into the shared 4-byte-aligned constants block at `0x1fb7d8`, size `0x130`
(`/tmp/w3436.map`). The original's corresponding block is `0x120` — the original does **not**
have this table in it. The extra 16 bytes push every later `.rodata` input section `+0x10`
past the original.

`alignas(16)` raises the object section alignment to 16 (`2**4`). gold then gives the table
its own 16-byte-aligned merge block at `0x203770` (size `0x10`,
`/tmp/w3436_align16.map`), and the shared block shrinks to `0x120`, matching the original.
`g_Hud_securityColors` moves from `0x203778` to `0x203780`, the original's address. From
`0x1fb8c8` onward the align16 `.rodata` matches the original byte-for-byte in this region.

### The 15 functions

All 15 were `pct==100 && !byte`; each has one or more PIC pool words whose resolved
destination was `+0x10` in the baseline and is exact after the change. Measured with the
classification script (orig destination == align16 destination; baseline = orig + 0x10):

| function | pool destination (orig == after) | baseline |
|---|---|---|
| `RepairBeam::update` | `0x1fbf80` | `0x1fbf90` |
| `SpaceLounge::OnTouchEnd` | `0x1fb8d8` | `0x1fb8e8` |
| `SpaceLounge::SpaceLounge` (C1/C2) | `0x1fb8c8`, `0x1fb8d8` | `0x1fb8d8`, `0x1fb8e8` |
| `Hud::drawOrbitInformation` | `0x203780` (`g_Hud_securityColors`) | `0x203778` |
| `Agent::getModPricePercentage` | `0x1fb8e8` | `0x1fb8f8` |
| `Player::playShootSound` | `0x1fbb50`, `0x1fbf00` | `0x1fbb60`, `0x1fbf10` |
| `Player::stopShootSound` | `0x1fbb50` | `0x1fbb60` |
| `Player::calcWeaponSounds` | `0x1fbb50` | `0x1fbb60` |
| `Status::nextCampaignMission` | `0x1fc330` | `0x1fc340` |
| `Station::getHiddenBlueprintIndex` | `0x1fbf6c` | `0x1fbf7c` |
| `Station::stationHasHiddenBlueprint` | `0x1fbf6c` | `0x1fbf7c` |
| `CutScene::replacePlayerShip` | `0x1fb934` | `0x1fb944` |
| `PlayerEgo::approachDockingPoint` | `0x1fba48` | `0x1fba58` |
| `PlayerEgo::update` | `0x1fba34` | `0x1fba44` |

The other 14 destinations are the `.rodata` tables that follow the shared merge block;
shrinking the block by 16 bytes moves them onto the original's addresses.

### A/B table (all `cmake --build cmake-build-match --target check -j8`)

| variant at `src/game/ui/Hud.cpp:43` | byte | linked | verdict |
|---|---:|---:|---|
| `static const int g_Hud_raceBadge[4]` (baseline) | 4233 | 4495 | PASS |
| `static const int g_Hud_raceBadge[10] = {...,-1...}` | 4229 | 4495 | FAIL |
| `static const volatile int g_Hud_raceBadge[4]` | 3842 | 4494 | FAIL |
| `alignas(8) static const int g_Hud_raceBadge[4]` | 4170 | 4495 | FAIL |
| **`alignas(16) static const int g_Hud_raceBadge[4]`** | **4248** | **4495** | **PASS, GAIN** |
| `alignas(32) static const int g_Hud_raceBadge[4]` | 3882 | 4495 | FAIL |
| `alignas(16) static const int g_Hud_raceBadge[10]` | 4229 | 4495 | FAIL |

`alignas(8)` is worse because the 8-byte-aligned group is placed in the first merge-constants
block at `0x1e7da8` (size `0x20`), so everything after `0x1e7da8` shifts `+0x10` instead of
only fixing the `0x1fb7d8` block. `alignas(32)`/`volatile` change codegen or place the table too late;
`int[10]` adds 24 bytes of `-1` and shifts later content.

## Remaining 248 `pct==100 && !byte` functions

```
pct100 non-byte functions: 248  (same address 103, moved 145)
runs by kind: {'insn': 998, 'pool': 317}
functions by kind: {'pool': 136, 'insn': 143}
pool runs by (orig section, ours section):
  ('.rodata', '.rodata')                   202
  ('.got', '.got')                          52
  ('.bss', '.bss')                          48
  ('.data', '.data')                        11
  ('.text', '.text')                         2
  ('.data.rel.ro', '.data.rel.ro')           2
pool functions by (orig section, ours section):
  ('.rodata', '.rodata')                    77
  ('.got', '.got')                          38
  ('.bss', '.bss')                          29
  ('.data', '.data')                         6
  ('.text', '.text')                         2
  ('.data.rel.ro', '.data.rel.ro')           2
```

### Instruction runs are link-layout artifacts

Of the 998 instruction-word runs, **983 resolve to the same symbol on both sides** (the branch
immediate differs only because the caller/callee address differs), 14 resolve to `#0x193e64`
(orig) vs `#0x193e40` (ours) — the same local `__cxa_*` helper at a different address — and 1
is a commutative-register `adds r2,r6,r4` vs `adds r2,r4,r6` in `MenuTouchWindow::draw`.
There is no wrong callee in this set.

Top same-target runs: `RadioMessage::RadioMessage(int,int,int,int)` 319,
`Array<RadioMessage*>::Array()` 67, `ArraySetLength<RadioMessage*>` 66, `_zip_error_set` 39,
`abs #0x5eff8` 35, `#0x68d30` 20.

### Pool runs

All 317 pool runs are "pic-same": the destination is the same logical datum, only the address
moved. By destination section pair: `.rodata` 202, `.got` 52, `.bss` 48, `.data` 11, `.text` 2,
`.data.rel.ro` 2. The `.got` runs are the same slot after a function base moved; `.bss`/`.data`
are the same global at a different local offset (the `.bss` order issue documented in
`bss_audit.md` / `data_layout_audit_2.md` §5: original `g_pActivity` is `.bss+0`, ours starts
with `g_android_leaderboard_scores` and puts `g_pActivity` at `0x2169cc`; `SlowMotion`'s pool
word points to `g_slowMotion` at `0x21797c` orig vs `0x2169e0` ours).

### `.rodata` after the fix

`rodata_content_diff_map.py` against the align16 unstripped relink:

```
differing bytes 13340 / 269159 (4.96%)
runs 5165  first VMA 0x1fc33d  last VMA 0x20377f
align: equal 6621 delete 556 insert 710 replace 45 (unequal 45); moved 763
differing symbols 176; no verbatim copy in original 1
  NOT-EXACT _ZL12kMissionCase  ours 0x1fc33c size 190
```

The first `.rodata` diff moved from `0x1fb8c8` to `0x1fc33d`, and the differing byte count fell
from 14,515 to 13,340. The remaining `.rodata` work is the per-TU table ordering already
described in `rodata_content_diff_map.md` / `rodata_group_order.md`; `kMissionCase` is the one
symbol with no verbatim copy in the original (a switch-vs-table codegen difference in
`Status.cpp`).

Other sections (same measurement): `.data.rel.ro` 900 differing bytes, `.data` 1626, `.got` 0,
`.text` 173,420 (per-function body/register allocation, not this sweep's scope), `.bss` order
as above.

## Proven vs hypothesis

Proven (command-backed):

- The +15 is real and lossless: controlled A/B via `cmake --build ... --target check`; the
  report diff shows 15 gained, 0 lost.
- The original's `g_Hud_raceBadge` is `int[4]{1190,1187,1189,1188}` at `0x203758`, read by
  `ldrh.w r1,[r1,r0,lsl #2]` in `Hud::init`.
- The baseline shared `.rodata.cst16` merge block is `0x130` at `0x1fb7d8`; after
  `alignas(16)` it is `0x120` and the table is a separate `0x10` block at `0x203770`
  (gold `-Map`).
- All 15 flips are explained by a `+0x10` pool-destination shift.
- The remaining 248 are all pic-same (same logical datum); no wrong function pointer.

Hypothesis / open:

- The original's placement of `g_Hud_raceBadge` at `0x203758` (8-byte aligned, followed by
  24 bytes of `0xff` padding before `g_Hud_securityColors`) is not yet reproduced; ours is at
  `0x203770` with no padding. Landing it at `0x203758` would need 24 bytes of padding to come
  from somewhere without changing `.rodata` size; not solved here.
- The `.bss` ordering (original `g_pActivity` first) is the next largest pool class; the
  `data_layout_audit_2.md` §5 ordering work is the likely fix.
- `alignas(16)` is a placement hint, not a source-layout reconstruction; the original source
  may not have had it. It is justified by the gate, not by a dump witness.
