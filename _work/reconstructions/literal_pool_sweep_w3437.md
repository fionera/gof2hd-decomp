# Literal-pool sweep round 2 (w3437)

Status: research note + one landed source fix. Branch `wave87-w3437`, based on master
`8bf6a675` (byte_exact 4249, linked_exact 4496). All numbers below were measured in this
worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so`.

Scope: the functions `report.json` scores at `pct == 100.0` but not `byte` — the instruction
stream and every inter-function address already match, and only literal-pool words (plus the
branch immediates that reference shifted PLT slots) still differ. This is the round-2 pass over
`literal_pool_audit.md` (w3390) and `literal_pool_sweep_w3436.py`.

## Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

| metric | value |
|---|---|
| byte_exact | **4249** |
| linked_exact | 4496 |
| compared | 4524 |
| avg_match | 99.89 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 |
| data exact | 843 / 843 |
| imports | 207 |
| lint | CLEAN |
| drift | 0 field(s) across 0 class(es) |
| `.text` | 0x1337c8 @ 0x69440 |
| `.rodata` | 0x41b67 @ 0x1c9e10 |
| `.ARM.exidx` / `.ARM.extab` | 0x6f70 / 0x26298 |
| `.data.rel.ro` / `.got` / `.data` / `.bss` | 0x2fe8 / 0x430c / 0x296c / 0x69ac |

Every ELF section is size- and base-exact at the baseline.

Classification driver (committed in `_work/reconstructions/literal_pool_sweep_w3436.py`):

```sh
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3437_baseline_classify.json
```

Baseline result: **247** `pct==100 && !byte` functions (102 same-address, 145 moved);
runs `{insn: 997, pool: 317}`. Pool runs by section: `.rodata` 202, `.got` 52, `.bss` 48,
`.data` 11, `.text` 2, `.data.rel.ro` 2. No wrong callee: every resolved branch target is the
same symbol / same absolute address.

## Landed fix: `AutoPilotList` string literals (byte 4249 -> 4251)

### Evidence

`src/game/core/AutoPilotList.cpp` defined two file-static arrays:

```c++
static const char kApLit1[] = ": ";
static const char kApLit2[] = " ";
```

and passed them to `String(..., false)`. In the original `.rodata` there is **no** adjacent
`": \0 \0"` block (the 5-byte `kApLit1`/`kApLit2` pair): searching the original `.rodata` for
`3a 20 00 20 00` returns nothing. The original's `": "` and `" "` live only in the mergeable
string pool (`": "` first at `0x1ca641`, `" "` at `0x1ca470` in both binaries). The file-static
arrays are therefore an artefact of our source, not of the original.

Fix: use the literals inline (`String(": ", false)`, `String(" ", false)`) and delete the two
statics.

### Measured

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4251  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 4249 -> 4251 (lock with --update-baseline)
RATCHET PASS
```

* `_ZN13AutoPilotListC1EP5Level` and `_ZN13AutoPilotListC2EP5Level` (760 B each) become
  byte-exact; both were `pct==100.0 && !byte` before.
* `pct100 non-byte` 247 -> 245; pool runs 317 -> 313; `.rodata` pool runs 202 -> 198.
* Every ELF section size and base is unchanged (`.text 0x1337c8`, `.rodata 0x41b67`,
  `.bss 0x69ac`, ...).
* `.rodata` content diff (relinked unstripped + `gold -Map`):

  ```sh
  /tmp/w3437_relink_map.sh
  python3 _work/reconstructions/rodata_content_diff_map.py \
      --unstripped /tmp/w3437_unstripped.so --map /tmp/w3437_rodata.map
  ```

  differing bytes **13,340 -> 13,298** / 269,159; differing symbols **176 -> 174**; the only
  `NOT-EXACT` symbol is still `_ZL12kMissionCase` (190 B, no verbatim copy in the original).

The baseline is locked with `python3 tools/ratchet.py --build-dir cmake-build-match
--update-baseline` in the same commit.

## Negative / neutral A/B experiments

All are relinks or rebuilds of the same tree; only the named change differs. `byte`/`linked`
are the `verify` counts. "sections" notes any size change.

| # | experiment | byte | linked | section sizes | verdict |
|---|---|---:|---:|---|---|
| 0 | baseline | 4249 | 4496 | exact | — |
| 1 | AutoPilotList inline literals (landed) | **4251** | 4496 | exact | strict improvement |
| 2 | `g_Hud_raceBadge` `int[10]` (drop `alignas(16)`) | 4230 | 4496 | exact | worse |
| 3 | `g_Hud_raceBadge` `alignas(8) int[10]` | 4230 | 4496 | exact | worse |
| 4 | `LensFlare` tables `alignas(32)` | 3882 | 4496 | exact | catastrophic |
| 5 | `Status::missionCompleted` direct `switch (m->getType())` (drop `kMissionCase`) | 2942 | 4496 | `.rodata` shrinks | catastrophic |
| 6 | `g_ips_shipTypeMap[64]` (append the original's 32 missing entries) | 4232 | 4496 | `.rodata` 0x41be7 (+0x80) | worse |
| 7 | `MenuTouchWindow` table declaration swap | 4251 | 4496 | exact | neutral (clang emits by first use, not declaration order) |
| 8 | remove dead strings `g_dfSep`/`g_dfTail`/`gCrbDash` | 4251 | 4496 | exact | neutral |
| 9 | drop the `.bss*` line from `section_order.txt` | **4259** | 4496 | `.bss` 0x69b8 (+0xc) | byte gain, not strict |

Experiment 9 is the known `.bss` common-block placement issue (see below). It is **not landed**
because it breaks the "every ELF section size-exact" invariant for +8 byte_exact; the existing
`rodata_group_order.md` records the same variant A (byte 4115 -> 4179, `.bss` +12) and calls it
a separate `.bss`-layout pass.

## `.rodata` findings

First divergence after the landed fix: `0x1fc33d` (VMA; `.rodata` base `0x1c9e10`).

* `0x1fc33c` original: `g_levelXPTable` (Status.cpp, `int[0x15]` =
  `0,7,0x15,0x2a,0x46,0x69,0x93,0xc4,0xfc,0x13b,0x181,0x1ce,0x222,0x27d,0x2df,0x348,0x3b8,0x42f,0x4ad,0x532,0x672`),
  followed by a 16-float table at `0x1fc390..0x1fc3d0`.
* `0x1fc33c` ours: `_ZL12kMissionCase` (Status.cpp, `unsigned char[190]`), a
  `mission type -> case label` lookup table. The original has no such table in `.rodata`; its
  `Status::missionCompleted` uses in-text `tbh` jump tables (see
  `Status-missionCompleted-blockorder.md`). The 190-byte block is load-bearing for our codegen:
  removing it (experiment 5) drops byte_exact to 2942, so it cannot simply be deleted at the
  current source shape.
* `0x1fc3d0` original: `g_ips_shipTypeMap` — **64** ints:
  `3,0,8,3,2,0,3,0,9,1,0,8,2,0,0,0,2,0,2,3,3,2,0,8,8,8,0,0,0,8,3,2,`
  `8,0,0,2,0,0,0,1,0,1,1,2,1,3,3,3,3,1,1,0,8,1,1,0,3,2,0,0,8,1,3,1`.
  Ours declares only the first 32 entries (`Level.cpp:751`).
* `0x1fc4d0` original: `g_ips_skyColors` (`uint32_t[20]`, first word `0x00a020ff`).
  Ours emits `g_ips_shipTypeMap` at `0x1fc4d0` and `g_ips_skyColors` at `0x1fdc84`.
* The whole `Level.cpp` plain-`.rodata` block is therefore emitted in a different order and
  shifted: ours has `jumpgateMeshes` at `0x1fc490`, `g_ips_shipTypeMap` at `0x1fc4d0`,
  `g_ips_skyColors` at `0x1fdc84`; the original has `g_ips_shipTypeMap` at `0x1fc3d0` and
  `g_ips_skyColors` at `0x1fc4d0`. Enlarging the table alone (experiment 6) makes the shift
  worse, so the fix is a `Level.cpp` `.rodata` ordering pass (declaration/first-use order), not
  a table-size edit.

## `.bss` findings (48 pool runs / 29 functions)

The original's `.bss` is: regular `.bss` inputs in link order, then gold's common block at the
**tail**. Our build puts the common block at the **front** because `section_order.txt` ends with
`.bss*` (all `.bss` inputs get the trailing index, the common block keeps index 0).

Exported `.bss` symbols, original vs ours (current build):

| symbol | original | ours |
|---|---|---|
| `g_pActivity` | `0x216970` | `0x2169cc` |
| `stackX` | `0x217980` | `0x2169d0` |
| `stackY` | `0x217984` | `0x2169d4` |
| `wheelStack` | `0x217988` | `0x2169d8` |
| `S` | `0x21c278` | `0x217a00` |
| `P` | `0x21d278` | `0x218a00` |
| `g_android_leaderboard_scores` | `0x21d2c0` | `0x216970` |
| `g_pVM` | `0x21d318` | `0x2169c8` |

Two independent causes:

1. **Common-block placement.** Dropping the `.bss*` line from `section_order.txt` (scratch
   relink, no source change) moves the common block to the tail and gives byte_exact
   **4259** (linked 4496), but `.bss` grows `0x69ac -> 0x69b8` (+0xc). The +0xc is alignment
   padding before the tail common block plus the 4-byte `.bss.anchor`; the current build uses
   the anchor to fill the original's last 4 bytes. Not landed (see experiment 9).
2. **Intra-TU order in `VirtualInput.cpp`.** The original's `VirtualInput` `.bss` block is
   `g_delayedKeyCount, g_delayedKeys, g_speedUp, g_slowMotion, stackX, stackY, wheelStack,
   g_simEdgesInit, g_simTouchActive, g_input` (stackX at block+0x100c). Ours is
   `stackX, stackY, wheelStack, g_speedUp, g_slowMotion, g_delayedKeyCount, g_delayedKeys,
   g_simEdgesInit, g_simTouchActive, g_input` (stackX at block+0x0). Reordering the
   *declarations* does not change clang's emission order: clang emits file-scope globals first
   (declaration order), then internal scalar globals in first-use order, then internal struct
   globals last. Matching the original therefore needs a first-use/function-order change (or a
   section pragma), not a declaration reorder; the handoff's declaration reorder was neutral
   (4251).
3. **`BlowfishSubkeyFile` `S`/`P`.** The original places `S`/`P` at the tail of the regular
   `.bss` (`0x21c278`/`0x21d278`, immediately before the common block), i.e. the original's
   link order has `BlowfishSubkeyFile.cpp` last among the objects. Our `link_order.rsp` places
   it third (its `.text` is early, and `gen_link_order.py` infers link order from `.text`
   addresses). Moving it to the end of the object list in a scratch relink makes `.ARM.exidx`
   shrink by `0x650` (the empty-TU sentinels move), so this needs a `gen_link_order.py`
   exception plus an exidx-marker re-check, not a raw rsp edit.

## Proven vs hypothesis

Proven (measured):

* The `AutoPilotList` inline-literal change is a strict improvement: byte 4249 -> 4251,
  linked unchanged, every section size/base unchanged, lint CLEAN, drift 0, and it gains exactly
  the two constructors.
* The original has no adjacent `": \0 \0"` block in `.rodata`; `": "`/`" "` occur only in the
  mergeable string pool.
* The first `.rodata` divergence is `0x1fc33d`, caused by our 190-byte `kMissionCase` table.
* The original's `g_ips_shipTypeMap` is 64 ints and sits at `0x1fc3d0`, before
  `g_ips_skyColors` at `0x1fc4d0`; ours is 32 ints and is emitted after `jumpgateMeshes`.
* The original's common block is at the `.bss` tail (`g_android_leaderboard_scores` at
  `0x21d2c0`, `g_pVM` at `0x21d318`); ours is at the front.
* Dropping `.bss*` gives byte 4259 at the cost of `.bss` +0xc.
* The negative A/B table above (Hud, LensFlare, Status, Level, MenuTouchWindow, dead strings).

Hypothesis (supported, not closed):

* The `.bss` +0xc under the no-`.bss*` variant is entirely alignment padding before the tail
  common block plus the `.bss.anchor`; if the 8-byte gap could be removed, `.bss` would be
  exact and the +8 byte_exact could be landed as a strict improvement.
* The original's `VirtualInput` `.bss` order can be reproduced by reordering the functions that
  first reference the globals (clang emits internal scalars in first-use order), which would
  make the 29 `.bss`-pool functions match once the common block is at the tail.
* The original's link order puts `BlowfishSubkeyFile.cpp` last; its early `.text` is produced by
  the section-ordering file, not by link order.

## Reproduction commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# classification (pct==100 && !byte)
python3 _work/reconstructions/literal_pool_sweep_w3436.py --json /tmp/w3437_classify.json

# .rodata content diff (unstripped relink + gold map)
/tmp/w3437_relink_map.sh
python3 _work/reconstructions/rodata_content_diff_map.py \
    --unstripped /tmp/w3437_unstripped.so --map /tmp/w3437_rodata.map

# .bss* A/B: copy the generated ordering file, delete the '.bss*' line, relink with the
# same gold command line (only --section-ordering-file changes), then verify:
grep -v '^\.bss\*$' cmake-build-match/section_order.txt > /tmp/order_no_bss.txt
# (relink script: same gold argv as the libgof2hdaa.so rule, -Map=... -o /tmp/no_bss.so)
python3 tools/verify/verify.py --build-dir /tmp/verify_no_bss \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/no_bss.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

## Note after harvest

The landed change is only `src/game/core/AutoPilotList.cpp` plus `tools/verify_baseline.json`
(4251/4496) and this note. No linker flags, section-order file, or other source files were
changed. The `.bss` common-block placement and the `Level.cpp` `.rodata` order are the two
highest-value follow-ups; both have measured A/B numbers above.
