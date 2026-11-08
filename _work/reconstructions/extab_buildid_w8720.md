# `.ARM.extab` byte ownership + `.note.gnu.build-id` flavour (w8720)

Status: research note, 2026-11-08. Worktree `wt/w8720`, base master
`1d10d0694` (wave 141). No `src/` change landed (see §4 for the measured
variants). Gate on the unchanged tree: `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `RATCHET PASS`.

Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

| section | differing bytes |
|---|---:|
| `.ARM.exidx` | **0** (byte-identical, 28,528 B) |
| `.ARM.extab` | **1,395** of 156,312 B (size/base exact) |
| `.rel.plt` | **0** (byte-identical, 29,392 B) |
| `.note.gnu.build-id` | 20 (the whole descriptor) |

## 1. Every differing `.ARM.extab` byte, by true owner

**Previous notes (w7620, w7742) misattribute records.** The `exidx_raw_audit.py`
grouping uses "nearest preceding `.dynsym` function", which names the
*previous* function: the exidx `prel31` target is the function address with
bit 0 set (Thumb), so the owner is the symbol at `fn | 1`, not the greatest
symbol `<= fn`. The record named `LevelScript::~LevelScript` is really
`LevelScript::process`, `Level::createGasClouds` is `Level::createMission`,
`ArrayAdd<KIPlayer*>` is `Level::createShip`, `MGame::startChargingJumpDrive`
is `MGame::OnTouchEnd`, `Hud::drawChallengeModeScore` is `Hud::draw`,
`Generator::createAgent` is `Generator::createMission`, `Radar::update` is
`Radar::draw`, `SpaceLounge::draw` is `SpaceLounge::drawLounge`,
`PendingProduct::~PendingProduct` is `Radar::Radar`, and
`Layout::OnTouchEnd` is `ModStation::ModStation`.

There are exactly **11** differing records (all equal-length; total 1,395 B),
and their true owners are:

| owner (exidx fn) | record B | extab diff B | body diff B (fdiff) |
|---|---:|---:|---:|
| `HangarWindow::render` (0x1490d8) | 948 | **762** | 1,740 / 9,700 |
| `LevelScript::process` (0x137d18) | 348 | 279 | 54,108 / 57,232 |
| `Level::createMission` (0xaffd8) | 956 | 138 | 5,763 / 17,172 |
| `Level::createShip` (0xc0214) | 336 | 70 | 1,411 / 2,764 |
| `MGame::OnTouchEnd` (0x17a144) | 280 | 40 | 4,464 / 8,080 |
| `Hud::draw` (0x163b90) | 476 | 37 | 1,362 / 9,332 |
| `Generator::createMission` (0x971b8) | 172 | 24 | 445 / 1,996 |
| `Radar::draw` (0x12e050) | 132 | 22 | 8,106 / 9,908 |
| `SpaceLounge::drawLounge` (0x170d38) | 168 | 14 | 175 / 1,948 |
| `Radar::Radar` (0x12d484) | 256 | **7** | **0 / 2,488 (byte-exact)** |
| `ModStation::ModStation` (0xd52d0) | 112 | 2 | 11 / 1,008 |
| **total** | | **1,395** | |

So **10 of the 11 records are owned by non-byte-exact bodies** and will close
when those bodies close. `Radar::Radar` is the **only** record owned by a
byte-exact body: its 7 bytes are the pure EH-shape delta analysed in
`extab_lsda_scope_w7742.md` (the two cleanup pads at offsets 2462/2464 are
swapped: rows 1/3/11/13/15 use 2462 in the original and 2464 in ours, rows
19/21 the reverse). The other 6 non-byte-exact functions
(`Layout::C1/C2`, `Status::missionCompleted`, `Gun::calcCharacterCollision`,
`MenuTouchWindow::OnTouchBegin`, `MGame::OnUpdate`) already have byte-exact
extab records. There is no other independent EH-shape residual.

## 2. `.note.gnu.build-id`: flavour is confirmed SHA-1, but it is *not* a
## function of `.text`/`.ARM.extab` alone

Rebuilding the gold link by hand (the checked-in `build.ninja` command, output
kept) and hashing it:

| file | size | stored build-id | SHA-1 whole, descriptor zeroed |
|---|---:|---|---|
| our unstripped gold output | 19,348,168 | `c47a15ad60f5a4cb55381cbbaab2bb17f270b430` | **`c47a15ad...` (match)** |
| our stripped `.so` | 2,187,328 | `c47a15ad...` (strip preserves the note) | `c4a36113...` |
| original stripped `.so` | 2,187,328 | `c011810d...` | `d7c05dc5...` |

So gold `--build-id` is SHA-1 of the **whole unstripped** output with the
20-byte descriptor zeroed (same as w7620). The original's `c011810d...` is the
hash of *its* unstripped output, not of the shipped stripped file.

**New result: the unstripped output contains the full `.debug_*` set and
`.symtab`/`.strtab`** (11 `.debug_*` sections, `.symtab` 5,832,800 B,
`.strtab` 283,878 B in our output), and the hash covers them. Our
`.debug_str` embeds the absolute worktree path
(`/home/debian/GalaxyOnFire2/wt/w8720/src/...`). The original's unstripped
file therefore contains the original build's source paths and line tables,
which we cannot reproduce from a different checkout. Consequence: the 20-byte
build-id will **not** simply follow `.text`/`.ARM.extab`; it can only match
when the entire unstripped link output — including `.debug_info`,
`.debug_line`, `.debug_str`, `.symtab` and `.strtab` — is byte-identical.
Unless the original build directory/line table is reproduced, the 20 bytes
are an irreducible delta even with a byte-exact `.text`/`.ARM.extab`.

(For the record: two consecutive manual gold links of the same object set are
byte-identical, so the linker itself is deterministic; the earlier
`f06be08e...` value in this worktree came from the first build's object set,
not from link non-determinism.)

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8720
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

The unstripped hash check: extract the `ld.gold` command from
`cmake-build-match/build.ninja` (the `COMMAND` line, `&&`-split, part 1),
replace the `libgof2hdaa.unstripped.so` output path, run it, then zero the
20-byte descriptor at `.note.gnu.build-id` and SHA-1 the whole file.

## 4. Source-shape experiments (all reverted; no extab win)

* `ModStation::ModStation` (2 extab B): the 11-byte body diff is a pure
  scheduler/RA tie around the `MatrixSetRotation(nearKey, pitch, yaw, ...)`
  call. The original loads yaw into `r3` and saves pitch in `r4`
  (`mov r4, r2`); ours loads yaw into `r4` and copies to `r3`
  (`mov r3, r4`), and schedules `str.w r8,[sp,#184]` before the load instead
  of after. Variants tried: `float` declaration order (both), `const int`,
  `const float`, separate declaration/assignment, `volatile` on one/both
  `int`s, `*(const volatile float*)` direct loads, `farKey` x = `pitch`
  (43 B, worse), `farKey` x = `pitch` + each of the above. Best stays
  **11 B / 1008 B**, extab **2 B**.
* `HangarWindow::render` (762 extab B): `String price("")` before
  `description` flips the register pair to the original's
  (description=r6, price=r5) but swaps the stack slots (price=sp+312,
  description=sp+300); raw stays 1740 and extab stays 1,395. `lay` moved
  before/after the Strings, and no-`lay`, are 1755/1770/2062/2059.
* `Radar::Radar` (7 extab B): extra braces around the `labelStrings[0]`
  `new String(...)` scope keep the body byte-exact (0 B) but leave the
  2462/2464 pad order unchanged (7 B). This confirms the w7742 BFI-tie
  analysis: the tie is not broken by a scope that does not change the
  machine BFI integer frequencies.
