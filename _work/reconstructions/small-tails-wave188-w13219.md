# Small tails — wave 188 / w13219: five residuals re-swept, no `src/` win

Status: **research note, no `src/` change landed.** Worktree `wt/w13219`,
branch `wave188-w13219`, base master `ce16f45f1` (wave-187 harvest 5).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13219`.

The five smallest residuals were re-diffed and re-swept with new angles
(exhaustive statement ordering, pointer-table types, volatile/IR hints,
anchor placement, argument materialisation).  Every candidate that fixes the
remaining instructions either changes the function size, re-colours the whole
function, or re-schedules an adjacent block.  The tree is left clean at the
base.

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

**VMA sensitivity (re-confirmed).**  `raw` compares bytes at the same
function offset, so the inline literal pools make it VMA-dependent.  The five
target TUs must all be at their base source before measuring: an 8-byte
ModStation size change moved `Hud::draw` from `0x163b90` to `0x163b88` and
inflated its raw count from 32 to 481.  All measurements below were taken
with every other target TU at base.

## 1. `ModStation::ModStation()` — exhaustive statement ordering still 47/48

The landed 11-byte shape keeps the farKey first rotation at the *second*
table value (`pitch = bits(yawBits)`).  The semantically faithful form
(farKey first rotation = `bits(pitchBits)`) was re-measured at this base:
it is **47/15**, not the 43/12 quoted in waves 186–187 (that number does not
reproduce on `ce16f45f1`; the nearest 43 was probably a different base or a
different source detail).

New this wave:

* All 6 valid orders of the four value statements
  (`pitchBits`, `yawBits`, `pitch` cast, `yaw` cast; casts after their read)
  × both nearKey argument orders.  Result: `PYpy`/`PYyp`/`PpYy` with
  nearKey `(pitch, yaw)` all give **47/15**; the `YP..`/`Yy..` orders give
  752/40 at size 1000.  No order gives the landed 11.
* `const float` / `const float &` locals: 47/15.
* `const int *` / `const volatile int *` table pointers: 669/60 and 47/15.
* `__builtin_assume(Globals::iPad <= 1)`, `__builtin_expect(iPad,0/1)`:
  47/15.
* `Globals::iPad != 0` / `> 0` / `>= 1` / `!!` / `== 1` / `& 1`: 47/15
  (the plain ternary `Globals::iPad ? ... : ...` is uniquely best).
* `Matrix farKey;` declared at the top of the block: 582/109 at size 1000.
* order/zero/`int order` temporaries: 47/15.
* `pitch = yaw;` reassignment before farKey, or `float farPitch = yaw;`
  copy: 47/15 (clang coalesces and re-schedules).
* direct float table loads (`pitchTable[race]`), `__builtin_memcpy`,
  `reinterpret_cast`, union bit-pun: 667/59 (the volatile integer load is
  required to keep the value in `r2`).

Residual (function-relative, 11 B, 4 runs): the original loads the selected
yaw-table value straight into `r3` and copies `r2`→`r4` last
(`ldr.w r3; ...; mov r4,r2`), while every farKey-first spelling loads the
false select operand first and schedules `mov r4,r2` before `ldr r3`.
The landed 11-byte shape (farKey uses the second value) remains the only
size-exact form.

## 2. `MGame::OnUpdate()` — the 2-byte volatile anchor is a frame ballast

The m122 block is exactly original in the `Status *volatile st` shape except
for one 2-byte `str r2,[sp,#120]` (the volatile store) before `nextInt`:

```
original:  blx nextInt; mov r4,r0; ldr.w r0,[r9]; blx getCurrentCampaignMission
anchor:    str r2,[sp,#120]; blx nextInt; mov r4,r0; ldr.w r0,[r9]; blx ...
```

The store shifts the whole remainder by +2 until the literal pool (raw 361,
size 11440).  New angles tried to keep the ballast without the store:

| shape | raw | size |
|---|---:|---:|
| landed (anchor, call through `st`) | **15** | 11440 |
| anchor + direct `Globals::status->getCurrentCampaignMission()` | 361 | 11440 |
| direct call, no anchor | 5196 | 11392 |
| volatile read `*(Status *volatile *)&Globals::status` | 5196 | 11392 |
| anchor moved after `nextInt` | 5206 | 11396 |
| anchor at function tail | 5181 | 11408 |
| `int dummy; (void)dummy;` / array / address / init | 5196 | 11392 |
| `AbyssEngine::String dummy;` | 5440 | 11444 |
| `volatile Status *st` (pointee volatile) | 5196 | 11392 |

The store pins the String temporary slot at `sp+120`; without it the frame
is re-laid-out and the whole 11 KB function re-colours.  The m23 `ldrd`
pair (`r2:r3` vs `r3:r5`) is coupled to the same frame slot and is unchanged
by all m122 shapes.  The landed 15-byte shape remains best.

## 3. `Hud::draw()` — the damage anchor is a global register-allocation knob

The 19-byte damage block is broken by the unbraced
`Status **volatile p = &Globals::status;` inside `if (hitFlags & 0x24)`,
which makes the `mov`/`str` unconditional and adds a `strne r8,[sp,#0xd0]`.
New ballast forms tried this wave (all inside the same `if`):

| anchor | raw | size |
|---|---:|---:|
| landed unbraced `Status **volatile p` | **32** | 9332 |
| `__atomic_signal_fence` (no code) | 7339 | 9328 |
| `volatile Status **p` (pointee volatile) | 7339 | 9328 |
| `(void) *(Status **volatile *)&Globals::status` | 7339 | 9328 |
| `(void) Globals::status` | 7339 | 9328 |
| `volatile int dummy = 0` | 6563 | 9336 |
| braced `Status **volatile p` | 7127 | 9336 |
| global store `*(Status **volatile*)&Globals::status = ...` | 7339 | 9328 |
| `volatile short` / `volatile char` | 7369 | 9340 |
| `volatile int p = (int)&Globals::status` | 7127 | 9336 |
| `Status *volatile p = Globals::status` | 7276 | 9340 |

No no-code ballast reproduces the original allocation; the store is the
knob.  The dock `r5`/`r6` swap (13 B) is a separate global tie.  The landed
32-byte shape remains best.

## 4. `MenuTouchWindow::OnTouchBegin()` — case-8 hoisting is allocation-neutral

The 0x401 residual is `layout->field_0x154` (`this->listEntryWidth`) loaded
into `ip` (original) vs `r4` (ours).  New case-8 hoisting variants:

| shape | raw | aligned |
|---|---:|---:|
| landed | **28** | 16 |
| `int leftMargin = layout->field_0xc_leftMargin` | 574 | 76 |
| `int top = layout->field_0x20_top` | 28 | 16 |
| `int entryWidth = this->listEntryWidth` | 62 | 26 |
| `Layout &layoutRef = *Globals::layout` | 28 | 16 |
| `Globals::layout->...` direct | 28 | 16 |
| reordered sum (`listEntryWidth + leftMargin + top`) | 33 | 17 |

`ip` vs `r4` is a global allocation tie (r4 is live elsewhere in the
original).  The 26-byte case-15 scrollbar `r2/r3/r4/r5/r6` rotation is
unchanged.  The landed 28-byte shape remains best.

## 5. `Generator::createMission()` — note shape is +4 B in the cleanup tail

The wave-186 note shape was re-measured with the non-volatile `bonus` it
specifies (the landed source still has `volatile int bonus` and `bv`):

```
int rawBonus = ...;
int remVal = rawBonus % 50;
int b = rawBonus + remVal;
volatile int rem = remVal;
int remCopy = rem;              // unconditional volatile read
if (b % 50 != 0)
    b = rawBonus - remCopy;
bonus = b;
```

with `int bonus;` (non-volatile) and no `bv` anchor: **90/31 at size 2000**,
matching the wave-187 note.  The bonus block itself is exact except `rem`
at `sp+44` instead of the original `sp+32`; the +4 is one extra `.ARM.exidx`
landing pad plus the literal-pool alignment `nop`.  Variants with `bv`
before/after the block give 2004/2008.  The landed 40-byte / 1996 B shape
remains best.

## 6. Reproduction

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

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13219/`
(`cc.sh`, `rawruns.py`, `sweep_mod.py`, `brute_mod.py`, `sweep_mg.py`,
`sweep_hud.py`, `sweep_mtw.py`, `sweep_gen.py`, `*.base.cpp`).
