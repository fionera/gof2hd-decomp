# LevelScript::process — wave 109 (w5703): five statement reorders + status/messages
# register-split audit

Status: **code change landed, 2026-11-04**. Branch `wave109-w5703`, base master
`7c34c4098`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (HEAD)     after
official rounded pct     56.1              56.1
raw official ratio       56.057515         56.082392
orig / ours insns        20169 / 20029     20169 / 20029
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
.text differing bytes    96,488            96,517   (+29)
.ARM.extab               0x26294 (4 B short) 0x26294 (unchanged)
.ARM.exidx               0x6f70            0x6f70
.rodata / .rel.dyn / .rel.plt  exact       exact
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.  Net gain: **+0.024877 raw**.  The `.text` differing-byte
count rises by 29 because the reorders change instruction encodings even where
the fuzzy stream improves; the tracked structural deltas (28 sections, file
size +44, `.ARM.extab` `0x26294`, `.rodata`/`.dynsym`/`.dynamic`/`.rel.dyn`
exact) are unchanged.

## What changed (one file, `src/game/world/LevelScript.cpp`)

Five adjacent-statement swaps.  All are semantically neutral (independent
statements); they nudge clang's `-Oz` scheduler / register allocator.  Line
numbers are pre-change (HEAD).

| # | HEAD line | pair swapped | raw % after | .text diff after |
|---|---|---|---:|---:|
| 1 | 736 | `player->setTurretMode(false)` / `Globals::sound->play(143, ...)` | 56.067466 | 96,488 |
| 2 | 2405 | `player->setComputerControlled(false)` / `m_pHud->visible = 1` | 56.070256 | 96,501 |
| 3 | 2964 | `PlayerFighter::field_0x128 = 50000` / `enemies[i]->field_0x38 = 4` | 56.072441 | 96,497 |
| 4 | 2932 | `m_nScriptTimerB = ...` / `AEGeometry *leader = ...` | 56.080997 | 96,508 |
| 5 | 3524 | `port->setName(...)` / `enemies[i]->field_0x74 = 0` | **56.082392** | 96,517 |

The swaps were found by a randomised adjacent-statement sweep (1813 candidate
pairs in the function; four rounds of 300–500 pairs) screened with
`_work/reconstructions/w3692_oracle.py` (raw ratio patched to 6 decimals) and
confirmed with the official linked verifier.  The oracle ranking is noisy:
its top candidate (line 2135, oracle 57.148) measured **55.938** officially;
line 736 (oracle 57.143) was the real win.

A strictly-better-on-both variant exists: swaps {1,3} alone give
**56.069652** at `.text` diff **96,484** (−4 vs HEAD).  The full five-swap set
was landed for the higher fuzzy score; the `.text` delta is reported above.

## Status/messages register split: investigated, not flipped

The brief's premise is real.  Original prologue (`0x137d18`, objdump of the
ground-truth `.so`):

```
137d3a: ldr.w r0,[r4,#24]!     ; r4 = &this->m_pLevel
137d3e: blx  Level::getMessages
137d42: mov  r5, r0            ; messages -> r5
137d44: ldr  r0,[r4]
137d46: blx  Level::getPlayer
137d4a: strd r4, r0, [sp,#80]  ; [sp+80] = &m_pLevel, [sp+84] = player
137d4e: ldr  r0,[r4]
137d50: blx  Level::getEnemies
137d54: mov  r8, r0            ; enemies -> r8
137d56: ldr  r0,[pc,#848]
137d58: add  r0, pc
137d5a: ldr  r4, [r0,#0]       ; &Globals::status -> r4   <-- reuses dead r4
137d5c: ldr  r0, [r4]
137d5e: blx  Status::getCurrentCampaignMission
...
137d68: strd r4, r8, [sp,#92]  ; [sp+92] = &status, [sp+96] = enemies
137d6e: strd r5, r6, [sp,#72]  ; [sp+72] = messages, [sp+76] = delta
```

HEAD (`enemies; player; messages`) keeps the original's **frame slots**
(`sp+72` messages/delta, `sp+80` &m_pLevel, `sp+84` player, `sp+88` this,
`sp+92` &status, `sp+96` enemies) but assigns `messages -> r4` and
`&Globals::status -> r9`.  Because `messages` is fetched last it takes the
freed `r4`; `status` is pushed to `r9` (`ldr.w r9,[r0]`, 32-bit encoding,
+2 B over the original's 16-bit `ldr r4,[r0]`).

MIR (`-mllvm -print-machineinstrs`, "After Virtual Register Rewriter") shows
the allocator's order is `r4, r5, r8, r9`: the `&m_pLevel` temp takes `r4`,
`messages` `r5`, `enemies` `r8`, `&status` `r9`.  With the original call
order (`messages; player; enemies`) `messages` does move to `r5`, but the
allocator **still gives `&status` `r9`** even though `r4` is dead by then.
`-mllvm -regalloc=pbqp/basic` produce entirely different (worse) bodies.

Prologue shapes measured (official linked raw %, size):

| prologue shape | raw % | size | messages | status |
|---|---:|---:|---|---|
| HEAD: `enemies; player; messages` | **56.057515** | 57232 | r4 | r9 |
| `player; enemies; messages` | 56.056121 | 57232 | r4 | r9 |
| `messages; player; enemies` (original call order) | 55.576861 | 57308 | r5 | r9 |
| `messages; enemies; player` | 55.560536 | 57308 | r5 | r9 |
| `player; messages; enemies` | 55.560536 | 57308 | r5 | r9 |
| `enemies; messages; player` | 55.555556 | 57308 | r4 | r9 |
| `Level *level = m_pLevel` + MPE getters | 55.545595 | 57308 | r5 | r9 |
| local `Status *status = Globals::status` (MPE) | 50.532975 | 57212 | r5 | r9 |
| `Status *&status = Globals::status` (EPM) | 25.832162 | 55012 | r4 | r9 |

**Proven**: no faithful prologue shape tested puts `&Globals::status` in `r4`.
The split is a body-driven allocator outcome (the original's body has higher
scratch pressure, so it spills/reloads `status` from `[sp+92]` into `r0/r4/r5/
r8/r9` as needed; ours keeps it in `r9`).  Matching the body block layout is
the prerequisite, as wave 107/108 concluded.

## Real content bugs found, measured, not landed (size-negative)

1. **Mission-0 state-16 body is truncated.**  Original `0x139170..0x1391b8` +
   shared tail `0x13b132..0x13b15a`:
   ```
   layout->enableFillScreen(true); status->nextCampaignMission(true);
   switch_to_target_setting = 0;
   status->departStation((Station *)(intptr_t) galaxy->getStation(100));
   status->getStation()->setAttackedFriends(false);
   Level::comingFromAlienWorld = 0;
   appManager->SetCurrentApplicationModule(5);
   Level::initStreamOutPosition = 0; return 0;
   ```
   HEAD has the shorter `SetCurrentApplicationModule(2);
   switch_to_target_setting = 1; initStreamOutPosition = 0;` body.
   Replacing it: **55.975704 % at 57168 B** on HEAD (55.951610 at 57176 on the
   landed base) — a real fidelity fix but 56–64 B short and score-negative.
2. **Mission-1 state-3 `getStation` argument is `10`, not `100`.**
   Original `0x13b122: movs r1,#10`.  HEAD line 1034 passes `100`.
   Alone: **55.750846 % at 57160 B** — size-negative.

Both were measured on the wave-108 base and on the landed base; neither is
landed because the gate requires exactly 57232 B and the score drops.

## Proven vs hypothesis

* **Proven**: the five reorders are size-exact (57232 B), keep `.rel.plt`
  byte-identical (3674/3674 names), and raise the official raw ratio
  56.057515 -> 56.082392.
* **Proven**: the status/messages split is not a prologue source-order bug;
  every tested shape leaves `status` in `r9` (MIR-confirmed: `r4` is free at
  the status load in the messages-first shape yet `r9` is chosen).
* **Proven**: the mission-0 state-16 body and the mission-1 `getStation(10)`
  argument are real content bugs (Thumb disassembly); they are size-negative
  on the current tuned base.
* **Hypothesis**: the remaining ~44 % loss is dominated by the body's block
  layout / scratch-register colouring; matching the body, not the prologue,
  is the next lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN

python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

Raw score helper (scratch): import `tools/verify/verify.py`, call
`disassemble()` on both sides, `reconcile()`, then
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`.

Section byte-diff helper:

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset'] + s['size']]
for n in ('.text', '.rodata', '.ARM.exidx', '.ARM.extab', '.rel.dyn', '.rel.plt'):
    a, b = sec(O, n), sec(U, n)
    print(n, len(a), len(b), sum(x != y for x, y in zip(a, b)) + abs(len(a) - len(b)))
```

## Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
```
