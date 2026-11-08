# MGame::OnTouchEnd — wave 141 (w8710): nested `if (!cutsceneActive)` re-roll is a 20-byte loss on the merged base

Status: analysis-only, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w8710`,
base `1d10d0694`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8710`. **No source change landed** (base restored; raw diff unchanged at 4464/8080).

Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.
Baseline reproduced exactly: **raw same-address diff 4464, size 8080/8080**.

## 1. Mission

w8613's open lead: rewrite the `cutsceneActive` / `menuTouchOpen` /
`autopilotMenuOpen` / `!field_0xc4` ladder as a nested
`if (!cutsceneActive) { menuTouchOpen } else { cutscene body }` (the w8514
nested-else trick one level higher), and find the `-4 B` companion that keeps
the re-rolled layout at 8080 B.  w8613 measured `cutsceneActive == true` at
8084 B / raw 4433 on the *pre-w8614* base and expected the nested form to be
the faithful, size-neutral way to reproduce that layout.

## 2. Result: the premise does not hold on the merged base

w8614 landed the 96/127 merge (`raw 5816 -> 4464`).  That merge changed the
campaign-chain layout, so the w8613 near-miss no longer reproduces.  Measured
on current master (`1d10d0694`):

| variant | size | raw diff |
|---|---:|---:|
| base | 8080 | **4464** |
| `cutsceneActive == true` | 8080 | 4693 |
| `cutsceneActive == 1` | 8080 | 4693 |
| `cutsceneActive != 0` / `!!cutsceneActive` / `== 0` forms | 8080 | 4464 (byte-identical) |
| nested `if (!cutsceneActive) { menuTouch } else { cutscene }` (form A) | **8060** | 6072 |
| nested `if (cutsceneActive) { cutscene } else { menuTouch }` (form B) | **8060** | 6072 |
| form A/B + `== true` / `== false` | 8060 | 6072 / 6073 |
| Ghidra-shaped ladder `if (!autopilotMenuOpen) { if (!field_0xc4) {...} } else {...}` | 8080 | 4464 (byte-identical) |
| swap `autopilotMenuOpen` / `!field_0xc4` else-if order | 8076 | 6948 |
| `if (menuTouchOpen)` moved before `if (cutsceneActive)` | 8116 | 5370 |
| invert `hudTouchFlags == 0` (empty then) | 8080 | 4464 (byte-identical) |
| invert `cutsceneActive` (empty then) | 8080 | 4464 (byte-identical) |
| `menuTouchOpen` / `starMapOpen` / `pauseOpen` / `choiceWindowOpen` `!= 0` forms | 8080 | 4464 (byte-identical) |

The nested forms are **20 bytes shorter**, not 4 bytes longer.  Clang
canonicalises the two nestings to the same CFG (`if (cutsceneActive) goto
else`), and the changed fall-through re-rolls the whole function from
`0x17a7e4` (the `hudTouchFlags & 0x40` dock block) instead of from
`0x17adc4` as the base does.  That early re-roll is why raw rises to 6072.

## 3. The `-4 B` companion exists, but the re-rolled layout is still worse

On the *unmerged* 96/127 base (`split96`, 8080 / 5790), the w8613 shape does
reproduce:

| variant | size | raw diff |
|---|---:|---:|
| split96 (no merge) | 8080 | 5790 |
| split96 + `cutsceneActive == true` | **8084** | **4559** |
| split96 + `cutsceneActive == true` + `touchesStream == true` | 8084 | 4551 |
| split96 + nested form A | 8084 | 5059 |
| split96 + `== true` + `!wasAutoPilot && isAutoPilot()` swap | **8080** | 4690 |

So the `-4 B` companion is the epilogue condition reorder
`if (this->player->isAutoPilot() && !wasAutoPilot)` ->
`if (!wasAutoPilot && this->player->isAutoPilot())` (it removes the 4-byte
`b.w` at the 0x200 branch ledger).  It keeps the re-rolled layout and lands
exactly on 8080 B.  But the resulting raw is 4690, still worse than the merged
base's 4464.  The merge (w8614) is simply a bigger win than the layout move.

## 4. Other levers re-confirmed on the merged base (do not re-run)

- `if (count > 0)` -> `if (count != 0)` / `if (count)`: 8076 / 6665 (re-rolls).
  `if (0 < count)`: byte-identical to base.
- `if (this->hudTouchFlags == 0)` -> `if (!this->hudTouchFlags)` / `== 0u` / `< 1`: identical.
- `if (this->cutsceneActive)` -> `!!`, `!= 0`, `== 0`: identical.
- `if (this->menuTouchOpen)` -> `!= false`, `== true`: identical.
- `if (this->starMapOpen)` -> `!= false`, `== true`: `== true` gives 8080/4884.
- `if (this->autopilotMenuOpen)` -> `!= false`: identical; `== true`: 8084/5709.
- `if (this->field_0xc4 == false)`: identical.
- `if (this->touchesStream == true)` on the merged base: identical.

## 5. Open question

The base is a hard local optimum: every single-token polarity change is
canonicalised away, and every structural re-roll either loses 20 B or moves
the divergence from `0x17adc4` back to `0x17a7e4`.  The remaining 4464 bytes
are the absolute-address shift of the hud tail group (`0x200` body carries its
own `blx` at `0x17adca`; original branches to a shared `blx` at `0x17afde`).
Reproducing the original's shared-tail placement *without* moving the early
blocks is the unsolved part.  A MIR-level block-placement dump of the merged
base (not done this wave) is the next concrete step.

## 6. Reproduction

```
cp /dev/shm/w8710/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8710/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4464 / 8080

python3 /dev/shm/w8710/sweep2.py          # polarity sweep
python3 /dev/shm/w8710/sweep4.py          # 96/127 split + nested/eqtrue
```

Scratch: `/dev/shm/w8710/{fdiff.py,heat.py,heat2.py,delta.py,calls.py,
sweep.py,sweep2.py,sweep3.py,sweep4.py,MGame.base.cpp,orig.dis,ours.dis,
B.dis,T.dis,ghidra_mgame.c}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
