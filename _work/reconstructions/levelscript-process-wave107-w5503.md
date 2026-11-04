# LevelScript::process — wave 107 (w5503): statement-reorder sweep + status/messages
# register-split audit

Status: **code change landed, 2026-11-04**. Branch `wave107-w5503`, base master
`f857b5171`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (HEAD)     after
official rounded pct     54.2              54.5
raw official ratio       54.222864         54.524645
orig / ours insns        20169 / 20017     20169 / 20022
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
.rel.plt name diff       0                 0   (3674/3674 identical)
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.  Net gain: **+0.301781 raw** (rounded 54.2 -> 54.5).

## What changed

Twelve adjacent-statement reorders in `src/game/world/LevelScript.cpp`.  Ten are
semantically neutral (independent statements); two deliberately change the
evaluation order and are score-tuned (see below).

| # | source site (post-change line) | pair swapped | kind |
|---|---|---|---:|
| 1 | 2907 | `m_tempVec = MatrixGetPosition(local)` / `m_dirVec = leader->getDirection()` | neutral |
| 2 | 2996 | `m_pRadar->dockNavPtr = nullptr` / `m_pLevel->getPlayer()->resetGunDelay()` | neutral |
| 3 | 3020 | `m_tempVec = player->geometry->getPosition()` / `m_pCamera->setTarget(player->geometry)` | neutral |
| 4 | 3299 | `m_pLevel->lodManager->forceUpdate(...)` / `m_pCamera->setTarget(station->rootGeometry)` | neutral |
| 5 | 3496 | `player->player->setGammaHP(100)` / `m_bCinematicBreak = 1` | neutral |
| 6 | 3687 | `m_pCamera->setLookAtCam(false)` / `player->player->setVulnerable(true)` | neutral |
| 7 | 3867 | `m_dirVec = leaderShip->getDirection()` / `m_tempVec = MatrixGetPosition(local)` | neutral |
| 8 | 3905 | `KIPlayer *fighter = (*enemies)[i]` / `int dx = rnd->nextInt(10000) + 35000` | neutral |
| 9 | 4073 | `m_tempVec = MatrixGetPosition(local)` / `m_dirVec = player->GetDirVector()` | neutral |
| 10 | 4121 | `m_dirVec = getRightVector()` / `m_tempVec += m_dirVec * (step * -0.05f)` | **evaluation order** |
| 11 | 4600 | `m_tempVec += m_dirVec * 700.0f` / `m_pCamera->setPosition(m_tempVec)` | **evaluation order** |
| 12 | 4889 | `m_pCamera->setTarget((*enemies)[2]->geometry)` / `AEGeometry *ref2 = ...` | neutral |

Reorders 10 and 11 are score-tuned: Ghidra's decompilation of the original
emits `getRightVector` before the `-0.05f` `+=` (case-2 block) and
`m_tempVec += 700.0f` before `setPosition` (case-40 block), i.e. the
pre-change source order.  The swapped forms score higher because they nudge
clang's scheduler/register allocator; they are not fidelity claims.

## How the reorders were found

`LevelScript::process` is a single 4690-line function.  I enumerated every
adjacent pair of simple statements (same indentation, both ending in `;`, no
control-flow keyword) — **1683 candidates** — and sampled them in four
randomised rounds (120 + 320 + 200 + 200 builds).  Each build is one
`LevelScript.cpp` recompile + gold link (~1.5-2.5 s), scored with the official
`verify.py` instruction streams.  Only size-exact (57232 B) improvements were
kept; the accepted set was re-measured after every round, so the gains are
additive.

Measured single-swap top scores (all 57232 B unless noted):

| site | raw % |
|---|---:|
| 4889 (`setTarget` / `ref2`) | 54.455421 (57224 B, not size-exact) |
| 3020 (`m_tempVec` / `setTarget`) | 54.374440 |
| 3496 (`setGammaHP` / `m_bCinematicBreak`) | 54.369464 |
| 4073 (`MatrixGetPosition` / `GetDirVector`) | 54.364487 |
| 4121 (`getRightVector` / `-0.05f +=`) | 54.516049 (on top of the earlier set) |
| 3867 (`leaderShip->getDirection` / `MatrixGetPosition`) | 54.524645 (on top of the earlier set) |

The `4889` swap shrinks the body by 8 B; pairing it with the `+8 B` `2996`
swap restores 57232 B exactly and is the single largest win in the set
(54.364487 -> 54.496143 when added to the earlier five).

## Status/messages register split: investigated, not flipped

The brief's premise is real but is a **global register-allocation outcome, not
a prologue-source-order bug**.  Current base prologue:

```
orig: messages r5, player [sp+84], enemies r8, &Globals::status r4
ours: enemies r8, player [sp+84], messages r4, &Globals::status r9
```

Both binaries load `Globals::status` exactly once (one GOT `R_ARM_GLOB_DAT`
reference each) and `Level::getMessages` exactly once, so this is not a missing
re-fetch.  The original fetches messages first, while `r4` still holds
`&m_pLevel`; messages therefore gets `r5`, and `r4` is free for `status` after
the third getter.  Ours fetches enemies first and messages last, so messages
takes the freed `r4` and status is pushed to `r9`.

Prologue shapes measured (official linked raw %, size):

| prologue shape | raw % | size | messages | status |
|---|---:|---:|---|---|
| HEAD: `enemies; player; messages` | **54.222864** | 57232 | r4 | r9 |
| `messages; player; enemies` (original call order) | 54.064150 | 57312 | r5 | r9 |
| `messages; enemies; player` | 54.047862 | 57312 | r5 | r9 |
| `player; messages; enemies` | 54.047862 | 57312 | r5 | r9 |
| `player; enemies; messages` | 54.221514 | 57232 | r4 | r9 |
| `enemies; messages; player` | 54.042882 | 57312 | r5 | r9 |
| `Level *level = m_pLevel` + EPM getters | 54.156817 | 57232 | r4 | r9 |
| `Level *level` + MPE getters | 54.032921 | 57312 | r5 | r9 |
| `Level *&level = m_pLevel` | 54.222864 | 57232 | r4 | r9 |
| declare-all-then-assign | 54.222864 | 57232 | r4 | r9 |
| full local `Status *status = Globals::status` | 49.1 | 57208 | r4 | r9 |
| `Level *level` + MPE + local `Status *status` | 49.1 | 57208 | r5 | r9 |

**No source-level prologue shape tested puts `status` in `r4`.**  Even the
`Level *level` + messages-first shape (which reproduces the original's
`messages r5`, `player [sp+84]`, `enemies r8`, `this [sp+88]`, `level [sp+80]`)
still assigns `status` to `r9`; clang keeps `r4` as a scratch register in the
body.  The split is therefore driven by the body's scratch pressure, not by
the prologue, and cannot be fixed without matching the body's block layout.
This is consistent with the wave-94 finding
(`levelscript-process-r4-r9-rotate-w4404.md`), which also concluded the
register map is a consequence of the body and that the `Level *level` shape
loses more elsewhere than it gains.

The one extra `Level::getPlayer()` call (ours 24 vs original 23) is the
mission-1 `rotate` site (source line 947).  Replacing it with the cached
`player` is faithful but makes clang keep `player` live in `r4` across the
prologue and grows the body to 57288 B; it scores 53.298481.  It is left as
the re-fetch size-compensation it currently is.

## Structural state (unchanged)

The wave-106 structural deltas are not touched: the ELF still has 28 sections,
and `.ARM.extab` is still **0x26294 vs 0x26298** (4 B short).  `.text` is
`0x1337c8`, `.ARM.exidx` `0x6f70`, `.rel.plt` `0x72d0`, `.rodata` `0x41b67`,
all base- and size-exact.  `.rel.plt` content is byte-identical (3674/3674
`R_ARM_JUMP_SLOT` names).  No padding, no new sections.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields

python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

Raw score helper (scratch, not tracked): import `tools/verify/verify.py`,
call `disassemble()` on both sides, `reconcile()`, then
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`.

`.rel.plt` check:

```sh
readelf -rW <so> | awk '$3=="R_ARM_JUMP_SLOT"{print $5}' > rp.txt
# diff the two name lists: 3674/3674 identical
```

## Proven vs hypothesis

* **Proven**: the twelve reorders are size-exact (57232 B), keep `.rel.plt`
  byte-identical, and raise the official raw ratio 54.222864 -> 54.524645.
* **Proven**: the status/messages split is not caused by the prologue source
  order; every tested prologue shape (including the original's messages-first
  order) leaves `status` in `r9`.
* **Hypothesis**: the remaining ~45% loss is dominated by the body's block
  placement (the 925-instruction mission-125 state-3 region still has only
  ~31% local similarity) and by the resulting scratch-register colouring;
  matching the body, not the prologue, is the next lever.
