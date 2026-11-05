# LevelScript::process — wave 119 / w6812 (statement-reorder hill-climb, round 4)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6812`, base master
`3981b32c5`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.471289         59.474784      (+0.003495)
orig / ours entries      20169 / 20042     20169 / 20043
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6812/score.py`, same definition as the wave-118/119 notes).
`verify`'s one-decimal report rounds both values to **59.5 %**; the raw ratio is the
discriminating number.

## Landed changes (all size-neutral statement reorders)

Source order below is the final file order; base line numbers are from the master file.

| # | base lines | change |
|---:|---|---|
| 1 | 2144/2146 | `m_pParticleGeom1->setPosition(dir * 10000.0f)` moved before the two `setDirection` calls |
| 2 | 3920/3923 | `int sideZ = Globals::rnd->nextInt(100);` moved before `KIPlayer *pirate = (*enemies)[i];` |
| 3 | 3957/3960 | `m_nScriptTimerA = 0;` moved before `m_tempVec += leader->getRightVector() * -500.0f;` |

Each is a reorder of independent statements; none adds/removes a call or symbol, so
`.rel.plt`, `.rel.dyn`, `.dynsym`, `.text` size and all section sizes are unchanged.

Running score after each round:

```
master                                      59.471289
+ (2144/2146)                               59.471826
+ (3920/3923)                               59.474784   <- landed (same score alone)
+ (3957/3960)                               59.474784   <- landed (same score alone)
```

## Method

1. Built and confirmed the master baseline (`RATCHET PASS`, byte 4505).
2. Built a fast scorer (`/dev/shm/w6812/fast.py`): compile only `LevelScript.cpp` with
   the exact `compile_commands.json` flags, gold-link a variant `.so` against the
   checked-in objects, strip/reorder-shstrtab, and score the target's normalized stream.
   ~2 s/compile+link, ~1.6 s/score; 8 workers in parallel.
3. Exhaustive searches from the landed base (all candidates are same-indent complete
   statements):
   - adjacent swaps (1822) — the wave-119/w6797 base is already at a local optimum;
   - distance-2 swaps (1397) — found change 1;
   - distance-3 swaps (1118) — found changes 2 and 3;
   - adjacent whole-block swaps (75 `if`/`for`/`while`/`switch` blocks) — no win;
   - `&&`/`||` operand swaps (18) — no win (clang canonicalises them);
   - all same-indent pairs inside case 125 (1457) — no win beyond the above.
4. Only size-57232 variants with `.rel.plt`/`.rel.dyn` byte-identical to the original
   were considered landable.

## Dead ends measured (recorded so they are not retried)

| variant | ratio | size | note |
|---|---:|---:|---|
| prologue call order `messages, player, enemies` (matches original disasm) | 58.723086 | 57304 | prologue matches but +72 B (EH landing pad grows); not landable |
| same + `Status *status` local first | 58.243781 | 57312 | status lands in r4, but entire allocation shifts |
| prologue `player, enemies, messages` | 59.469810 | 57232 | size-exact, slightly worse |
| case 125 `if (m_nState == 1)` -> `else if` (matches original `b.w` skip) | 57.689247 | 57248 | block layout moves the case far later |
| case 154 state 0: `setComputerControlled(true)` early / `m_nState++` late (matches original) | 59.164595 | 57248 | semantic fix, but +16 B and worse ratio |
| case 154 + `m_pCamera->setTarget(enemies[2]->geometry)` replacing 2nd `setSpeed(0)` | 59.164595 | 57248 | same |

The prologue order is the single biggest structural difference from the original
(original calls `getMessages`, `getPlayer`, `getEnemies`; master calls
`getEnemies`, `getPlayer`, `getMessages`), but every size-exact variant of it scores
lower because the register allocator then puts `Globals::status` in `r9` instead of
the original's `r4` and the EH landing pad grows. A future pass that can also pin the
status allocation (or shrink the landing pad by 72 B) should revisit it.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6812/score.py                         # 59.474784
```

`/dev/shm` helpers are scratch only; the landed source is the sole copy of the change.
`.rel.plt`/`.rel.dyn` remain byte-identical and the `.ARM.extab` size is unchanged.
