# LevelScript::process — wave 191b / w13517: cross-control-boundary statement moves

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13517`, branch
`wave191b-w13517`, base master `5c99979c4` (raw **51,639**, aligned 22,078,
`.text` 71,416, `.ARM.extab` 577, `.rel.plt` md5 `289b2a06…`).

## Lens

The wave-189/190/191 d1..d12 sweeps only permute statements *inside* a
straight-line run; every control header (`if`/`for`/`while`/`switch`/…) is a
barrier.  This pass fills that gap: a parser splits the `process` body into
statement items (multi-line statements included) and swaps each simple
statement with the adjacent `if`/`for` block (both directions, 471
candidates, exact NDK r18b `-Oz` compile + gold link).  Ten cells are
size-exact, `.rel.plt`-identical raw wins; nine independent moves were landed
(the two adjacent case-147 cells compose into one edit).

## Landed changes (all semantics-preserving: the moved statement touches no
state read by the block it crosses)

| # | source (new order) | move | raw |
|---:|---|---|---:|
| 1 | 1257 | mission-89 `enemies[11]->moveForward(delta)` before `if (m_nState <= 2)` camera block | -4 |
| 2 | 3125 | `player->setFreeLookMode(false)` before `if (player->isInRocketControl())` | -3 |
| 3 | 3407 | `m_pRadar->enabled = 0` before the rocket-control `if`; `player->setFreeze(true)` after it | -3 |
| 4 | 4519 | case-154 `player->player->setVulnerable(false)` before the rocket-control `if` | -3 |
| 5 | 4134 | `m_nScriptTimerA += delta` after the `__builtin_expect(m_nState == 1, 1)` quota block | -2 |
| 6 | 665 | intro `m_pCamera->translate(...)` before `if (!Globals::sound->isPlaying(142))` | -1 |
| 7 | 3063 | `wcamera->setLocal(local)` after the 3-ship `moveForward` loop | -1 |
| 8 | 4533 | `player->setComputerControlled(true)` after `if (field_0xaa) setAutoTurret(false)` | -1 |
| 9 | 5335 | post-switch `m_nState = 1` before the escort `setAlwaysEnemy` loop | -1 |

The individual cells sum to -21; the composition measures **-19** (two cells
share a register-allocation ripple).  These are codegen-coincidence reorders
(the original disasm order is the base order at several sites, e.g. the
mission-89 `moveForward` is after the camera block in the original); they are
kept because the gate is the binary's same-address byte diff.

## Measured result

```
                         base (5c99979c4)   this pass
process raw diffbytes    51,639             51,620     (-19)
process aligned insns    22,078             22,120     (+42, not a gate metric)
orig / ours size         57232 / 57232      57232 / 57232  (exact)
orig / ours insns        20,830 / 20,723    20,830 / 20,724
.text diffbytes          71,416             71,397     (-19)
.ARM.extab diffbytes     577                577        (size/base exact, 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0                  0          (byte-identical)
.rodata diffbytes        485                485        (sh_size 269,644 unchanged)
.rel.plt                 md5 289b2a06…      md5 289b2a06…  (byte-identical, 29,392 B)
.note.gnu.build-id       20                 20
file size                2,187,328          2,187,328  (exact)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
drift                    0                  0
symbol parity            5370/5370          5370/5370
RATCHET                  PASS               PASS
```

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13517
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51620 size=57232/57232 text=71397 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harness (`parser.py`, `sweep.py`, `harness.py`) lives in
`/home/debian/GalaxyOnFire2/scratch/w13517/` (scratch, not tracked).
