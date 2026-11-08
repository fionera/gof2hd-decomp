# Small tail B wave 157 (w10218): SpaceLounge clamp colouring + Generator
# rem spill — no landed `src/` change

Status: **note only, no `src/` change**. Worktree `wt/w10218`, base master
`cf74f4ee6` (wave 155). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000). Scratch
`/dev/shm/w10218`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  Both bodies stay size-exact (1948 / 1996 B).

Read first: `small-tail-b-wave154-w10018.md`,
`status-generator-spacelounge-wave155-w10110.md`,
`spacelounge-drawlounge-wave130-w7682.md`,
`SpaceLounge-drawLounge-wave123-w7315.md`,
`spacelounge-drawlounge-wave117-w6517.md`.

Metrics: raw same-address diff bytes (`/dev/shm/w10218/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10218/align_diff.py`, path-adjusted
copy of the committed tool).

| fn | before raw/aligned/size | after raw/aligned/size |
|---|---:|---:|
| `SpaceLounge::drawLounge` (`_ZN11SpaceLounge10drawLoungeEv`) | 175 / 41 / 1948 | 175 / 41 / 1948 |
| `Generator::createMission` (`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`) | 393 / 52 / 1996 | 393 / 52 / 1996 |

## 1. SpaceLounge — the clamp is right, the pad/x colouring is the whole residual

The original's right arm is the 8-byte `itt gt; addgt.w r1,r6,sl;
subgt r5,r0,r1` (reuses the int `pad*2` already in `sl` from `top`).  HEAD
(`+ (-width - pad - pad)`) instead emits the 4-instruction
`movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6; addgt.w r9,r1,r0`.
Every shape that produces the 2-instruction clamp also leaves `pad` in `fp`
(r11) and `x` in `r9`, i.e. the same colouring HEAD already has; the
original has `pad` in `r9` and `x` in `r5`.

Measured this wave (all real TU compile + checked-in gold link):

| right arm / shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD `+ (-width - pad - pad)` | **175** | **41** | **1948** |
| `- (width + pad * 2)` | 445 | 54 | 1948 |
| `+ (-width - pad * 2)` | 445 | 54 | 1948 |
| `- width - pad * 2` | 445 | 54 | 1948 |
| `- (pad * 2 + width)` | 445 | 54 | 1948 |
| `int pad2 = pad*2;` used for top+clamp | 445 | 54 | 1948 |
| `- (width + pad + pad)` | 176 | 42 | 1948 |
| `w - (inset + width + pad*2)` | 196 | 47 | 1948 |
| `int boxW = width + pad*2;` after clamp, used in drawBox/DrawRectangle | 1833 | 241 | 1968 |
| same, `boxW` before clamp | 509 | 93 | 1940 |
| same, `boxW` after `lineHeight` | 1833 | 241 | 1968 |
| `boxW` + `PaintCanvas *canvas` local | 1833 | 241 | 1968 |
| `boxW` + `short top` | 1833 | 241 | 1968 |
| `boxW` + `float topF` | 1833 | 241 | 1968 |
| `int top = (int) bottomRight.y - pad * 2;` | 1803 | 267 | 1956 |
| `pad` after `SetColor` / after `centerX` | 254 | 72 | 1948 |
| `const int pad` | 175 | 41 | 1948 |
| `unsigned pad` | 177 | 43 | 1948 |
| `short pad` | 1490 | 219 | 1964 |
| `x`/`top` declared before the name/role strings | 175 | 41 | 1948 |
| `top` moved after the strings | 1854 | 264 | 1976 |

The `boxW` family is the only source shape that reaches `pad=r9` + `x=r5`
(wave 117/123), but it rotates `pad2 -> fp`, `lineHeight -> r8` and spills
`top` (frame 120 -> 128), so it is 1968 B and raw 1833.

### Why the colouring cannot be reached

Wave 123/130 established the mechanism: the greedy allocator's target needs
`agent.weight > pad.weight > x.weight` so that `lineHeight` evicts `pad`
from `fp`, then `pad` evicts `x` from `r9` (and `x` falls to `r5`).  The
faithful MIR has `x (2.946e-4) > agent (2.267e-4) > pad (2.078e-4)`, so the
two conditions are mutually exclusive under any monotone weight scaling.
This wave re-confirmed that no clamp association, `pad2` local, `boxW`
local, declaration reorder, qualifier change (`const`/`unsigned`/`short`),
or `top` type changes the ordering.  HEAD 175/41/1948 remains the best
measured shape.

## 2. Generator — `rawBonus - rem` gets the right block but the frame grows

The original's bonus block is

```
9770c: adds r5, r1, r6      ; bonus = rem + rawBonus
9770e: str r1, [sp, #32]    ; spill rem
97710: movs r1, #50
97712: mov r0, r5
97714: blx __aeabi_idivmod  ; bonus % 50
97718: ldr r0, [sp, #32]    ; reload rem
9771a: cmp r1, #0
9771c: it ne
9771e: subne r5, r6, r0     ; bonus = rawBonus - rem
```

HEAD compiles `bonus = rawBonus + rawBonus - bonus;` to the spill-free
`rsbne r5, r5, r6, lsl #1` (no `str`/`ldr`).  Changing the source to
`bonus = rawBonus - rem;` reproduces the `subne` shape (raw 393 -> 304) but
the allocator keeps `rem` in `r9` (`mov r9, r1`) and spills `agent` instead:
`stack.7` (agent) stays live to the `new Mission` call at `0x97862`
(`ldr r0,[sp,#40]`), the frame grows 96 -> 104, and the function is 2000 B
(one extra `mov r9,r1` plus one alignment `nop`).

Measured this wave (all real TU compile + checked-in gold link):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD `rawBonus + rawBonus - bonus` | **393** | **52** | **1996** |
| `bonus = rawBonus - rem` | 304 | 112 | 2000 |
| `bonus = rawBonus - (bonus - rawBonus)` | 304 | 112 | 2000 |
| `bonus = bonus - rem - rem` | 304 | 112 | 2000 |
| `bonus -= rem * 2` (semantically identical) | 445 | 59 | **1996** |
| `bonus -= rem + rem` / `rem << 1` / `2 * rem` | 445 | 59 | 1996 |
| `volatile int rem` + HEAD body | 592 | **34** | 2000 |
| `volatile int rem` + `bonus -= rem * 2` | **300** | 59 | **1996** |
| `volatile int rem` + `bonus -= rem + rem` | 578 | 65 | 2000 |
| `volatile int rem; int remv = rem; bonus -= remv * 2` | 566 | 40 | 2000 |
| `volatile int rem` + `rawBonus - rem` | 702 | 91 | 2000 |
| `int rawRem; volatile int rem = rawRem; bonus = rawBonus + rawRem;` | 706 | 88 | 2000 |
| `short`/`unsigned`/`long`/`int8_t`/`int16_t rem` + `rawBonus - rem` | 304 | 112 | 2000 |
| `rem` address-taken (`*(&rem)`) | 304 | 112 | 2000 |
| `rem` at function scope / `bonusRem` local | 304 | 112 | 2000 |
| `__builtin_expect(bonus % 50 != 0, 0/1)` | 304 | 112 | 2000 |
| `if (2 * rem % 50 != 0)` | 645 | 58 | 1992 |
| `if (bonus % 50)` (no `!= 0`) | 304 | 112 | 2000 |
| `bonus = rawBonus - rem` + `agent->getImageParts()` hoisted | 1142 | 165 | **1996** |

### Why the frame cannot stay at 96 with the `rawBonus - rem` block

The allocator processes `agent` (global range, size ~8768) before `rem`
(local range across the second `idivmod`).  `agent` is in `r9` from the
prologue; `rem` is live across the second call.  In the original, `rem`
spills (`str r1,[sp,#32]`) and `agent` keeps `r9`.  With
`bonus = rawBonus - rem` the allocator instead splits `rem` into `r9`
(`%723 = COPY %724`) and spills `agent` to `stack.7`; `stack.7` then stays
live to the `new Mission` call, so the frame grows 96 -> 104.  The
`bonus -= rem*2` spelling avoids the frame growth (it reuses `rawBonus`'s
`r6` for `bonus`, keeping `rem` in `r5`), but then the block is
`add r6,r1; mov r5,r1; ... subne.w r6,r6,r5,lsl#1` (raw 445).  Hoisting
`agent->getImageParts()` to end `agent`'s live range earlier does keep the
frame at 96 (size 1996) but moves the call and is raw 1142.

The best size-exact shape found is `volatile int rem; ... bonus -= rem * 2;`
at **raw 300 / aligned 59 / 1996 B**: it improves raw (393 -> 300) and stays
size-exact, but the volatile reload inside the `if` body prevents
if-conversion (`cbz` instead of `it ne`), so aligned rises 52 -> 59.  No
size-exact shape measured improves *both* metrics.  HEAD 393/52/1996 is
retained.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10218
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10218/fdiff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w10218/align_diff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w10218/fdiff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 /dev/shm/w10218/align_diff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```

Scratch (not committed), `/dev/shm/w10218/`: `fdiff.py`, `align_diff.py`,
`score.sh`, `try_sl.py`, `try_sl2.py`, `try_sl3.py`, `try_gen.py`,
`SpaceLounge.cpp.base`, `Generator.cpp.base`, `sl_*.txt`, `gen_*.txt`,
`gen_all*.txt`, `gen_gra_*.txt`, `gen_vrr_*.txt`.

## 4. Files changed

* `_work/reconstructions/small-tail-b-spacelounge-generator-wave157-w10218.md`
  (this note).
