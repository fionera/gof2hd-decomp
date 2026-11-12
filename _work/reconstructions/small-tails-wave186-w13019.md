# Small tails — wave 186 / w13019: five residuals re-swept, no `src/` win

Status: **research note, no `src/` change landed.** Worktree `wt/w13019`,
branch `wave186-w13019`, base master `2d02567f6` (wave-185 notes).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13019`.

The five smallest residuals were re-diffed and re-swept.  New angles were
tried for each, but every candidate that fixes the remaining instructions
either changes the function size or re-schedules an adjacent block.  The
tree is left clean at the base.

## 0. Baseline (re-confirmed on the base link)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **15** | 111 | 4003/4002 |

Gate on exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS** — `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. VMA sensitivity of the `raw` metric (important for future sweeps)

`tools/fn_diff.py`'s `raw` count compares bytes at the same function offset.
The inline literal pools contain PC-relative words, so the raw count depends
on the function's VMA.  Editing any earlier TU (even when its size is
unchanged) can move the target function and inflate `raw` by hundreds of
bytes.  Example: with `MGame.cpp`'s m122 body edited, `Hud::draw` moved and
its raw count read 43 instead of 32; restoring `MGame.cpp` restored 32.
**Always restore every other target TU to the base before measuring a
function's raw count**, or compare against a build that keeps the same VMA.

## 2. `Hud::draw` — damage block can be made byte-exact, but costs 4 B

Moving the volatile anchor *after* the two damage `if`s makes the damage
block byte-identical to the original (two conditional `movne/strne` pairs
plus the dead `ldr.w r8,[sp,#108]`):

```cpp
if (hitFlags & 0x24)
    this->field_0x4b8 = 300;
if (hitFlags & 0x18)
    this->field_0x4b4 = 300;
Status **volatile p = &Globals::status;
```

The damage block then matches `0x164184..0x16419c` exactly, but the volatile
anchor emits a `str`/`ldr` pair and the function becomes 9336 B (`raw` 7111
or 7145).  Removing the anchor's use (`(void) p;`) does not remove the
volatile store; plain `(void) Globals::status;` removes the dead load and
gives 9328 B.  The landed unbraced anchor (9332 B, raw 32) remains the only
size-exact shape.

Other new damage shapes tried (all 9332 unless noted): anchor between the
ifs / before them, `Status *volatile`, `Status *const volatile`,
`__builtin_prefetch`, `(void) *(Status *** volatile)&Globals::status`,
`volatile int x = (intptr_t)&Globals::status`.  None is size-exact with the
damage block fixed.

## 3. `ModStation::ModStation()` — faithful farKey form is still 43

The landed 11-byte shape uses `farKey(yaw)` where `yaw = bits(yawBits)`,
i.e. the far key's first rotation is the *second* table value.  The original
binary copies the *first* table value (`pitchBits`, `r2`) into `r4` before
the nearKey call (`mov r4,r2` at `0xd546e`) and passes it to farKey.  The
semantically faithful source (natural `pitch`/`yaw` naming, farKey `pitch`)
gives raw 43 in every spelling tried:

| faithful-farKey shape | raw |
|---|---:|
| natural `float pitch = bits(pitchBits); float yaw = bits(yawBits);` | **43** |
| `const float` / references / `volatile` pointers | 43 |
| `const int *` table pointers (volatile loads) | 43 |
| `!iPad ? gHangarCamYaw : gHangarCamYawIPad` (both select sites) | 43 |
| `if/else` table selection | 43 |
| `int ipad = Globals::iPad;` before the select | 43 |
| `*(const float *)&pitchBits` directly in farKey | 43 |
| `float pitchCopy = pitch;` | 43 |

The 43 is the same 5-instruction tie: the allocator puts the long-lived
`pitchBits` in `r4`, swaps `yawIPad`/`yaw` into `r3`/`r4`, and hoists
`mov r4,r2` before the second table load.  No size-exact spelling was found
that keeps the original `r3 = yawIPad`, `r4 = yaw`, late `mov r4,r2`.

## 4. `Generator::createMission` — bonus block exact, frame/slot tie

A new shape makes the bonus block byte-exact (predicated 16-bit `subne`,
unconditional `rem` reload, `bonus` at `sp+16`, frame 96):

```cpp
int bonus;
...
int rawBonus = ...;
int remVal = rawBonus % 50;
int b = rawBonus + remVal;
volatile int rem = remVal;
int remCopy = rem;              // unconditional volatile read
if (b % 50 != 0)
    b = rawBonus - remCopy;
bonus = b;
```

The bonus block matches the original except that `rem` is spilled to
`sp+44` instead of `sp+32`.  The compiler does not colour the volatile slot
into the `&Globals::status` slot at `sp+32`; the function is 2000 B (4 B
over) and the exception-cleanup tail gains one `b.n` plus a `nop`.  The
landed 40-byte shape (1996 B) remains the best size-exact form.

Variants tried (all land at 2000 B / raw 90 unless noted): plain `rem`
(181/2000, `subne.w` in `r9`), `volatile rem` without the unconditional
read (716/2000, `cbz` branch), `int rem` + volatile pointer read (90),
comma-expression volatile read (90), declaration-order permutations (90),
`volatile int remVal` (716), `bonus` stored inside the `if` (716).

## 5. `MGame::OnUpdate` — m122 exact, extra volatile store

`Status *volatile st = Globals::status;` before `nextInt`, followed by a
direct `Globals::status->getCurrentCampaignMission()` and a `Level *level`
local before the ccm, makes the m122 body exact: `ldr.w r0,[r9]` after the
`nextInt` call and the level load after the ccm.  The volatile anchor leaves
one extra `str r2,[sp,#120]` before `nextInt` (no reload), so raw is 361 at
size 11440.  Removing the anchor drops the size to 11396/11392 (GOT base
splits to `r5`); a `volatile short`/`int` anchor changes the size to
11400/11396.  The landed 15-byte shape remains the best size-exact form.

The m23 64-bit idle compare (`ldrd r2,r3` vs `ldrd r3,r5`) is unchanged by
all m122 shapes; it is coupled to the volatile-anchor frame slot.

## 6. `MenuTouchWindow::OnTouchBegin` — local must stay used for leftMargin

The case-15 `Layout &layout` local must be used for `field_0xc_leftMargin`
to keep size 1896.  Any direct `Globals::layout->field_0xc_leftMargin` drops
to 1892 B (raw 362).  Using the local for `buttonInsetX` and leftMargin is
still raw 28; using it for `buttonInsetX` and top is raw 29.  The 28-byte
residual is the `r5`/`r6`/`r2`/`r3` rotation in the two scrollbar blocks,
unchanged by pointer/reference/volatile/ternary variants.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN5MGame8OnUpdateEv
# raw=32/11/11/28/40/15, sizes 9332/1008/1008/1896/1996/11440
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13019/`
(`sweep_mod4.py`, `sweep_hud_dmg.py`, `sweep_gen.py`, `sweep_gen2.py`,
`sweep_mg.py`, `sweep_mg23.py`, `sweep_mtw.py`, `idiff.py`, `*.base*.cpp`).
