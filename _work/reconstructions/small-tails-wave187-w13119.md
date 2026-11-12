# Small tails — wave 187 / w13119: five residuals re-swept, no `src/` win

Status: **research note, no `src/` change landed.** Worktree `wt/w13119`,
branch `wave187-w13119`, base master `8b5d4281c` (wave-186 harvest 6).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13119`.

The five smallest residuals from wave 186 were re-diffed and re-swept with new
angles.  Every candidate that fixes the remaining instructions either changes
the function size, re-schedules an adjacent block, or re-colours the whole
function.  The tree is left clean at the base.

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

Byte-level residual split (function-relative offsets, `rawdiff.py`):

| function | differing runs |
|---|---|
| `Hud::draw` | `0x5f8..0x60c` damage block (19 B) + `0x1dbc..0x1eba` dock `r5`/`r6` swap (13 B) |
| `ModStation` ctor | 4 runs round the nearKey `MatrixSetRotation` (`0x186..0x19c`) + 2 literal-pool words |
| `MenuTouchWindow::OnTouchBegin` | `0x401..0x405` `ip`/`r4` (2 B) + `0x60f..0x678` scrollbar `r2/r3/r4/r5/r6` rotation (26 B) |
| `Generator::createMission` | frame 104 vs 96, `rem` slot `sp+44` vs `sp+32`, bonus block, cleanup tail |
| `MGame::OnUpdate` | `0x19e7..0x19f3` m23 `ldrd` pair (5 B) + `0x28b4..0x28be` m122 `str`/`ldr` (10 B) |

## 1. `ModStation::ModStation()` — literal-pool order is a compiler tie

The landed 11-byte shape uses `farKey(..., pitch, ...)` where `pitch` is
`bits(yawBits)`, so the farKey first argument is the *second* table value.
The semantically faithful form (`farKey(yaw, ...)` with `yaw = bits(pitchBits)`,
i.e. both rotations `(pitchBits, yawBits)`) is raw **43**.

Decoding the literal pool shows the two forms differ only in the *order* of
the two yaw-pointer loads and the corresponding literal-pool words:

```
original:  ldr r3,[pc] -> yawIPad (0x1fe058)   ; literal at d56b4
           ldr r4,[pc] -> yaw     (0x1fe080)   ; literal at d56b8
ours:      ldr r4,[pc] -> yaw                  ; literal at d56b4
           ldr r3,[pc] -> yawIPad              ; literal at d56b8
```

The final register assignment is identical (`r3 = yawIPad`, `r4 = yaw`); only
the load order is swapped.  clang 7 at `-Oz` always materialises the **false**
operand of `iPad ? yawIPad : yaw` first (the select is lowered to
`cmp; mov r0,r3; it eq; moveq r0,r4`).  Every spelling tried still loads
`yaw` (r4) first:

| faithful-farKey spelling | raw | loads |
|---|---:|---|
| `Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw` | 43 | r4,r3 |
| `!Globals::iPad ? gHangarCamYaw : gHangarCamYawIPad` | 43 | r4,r3 |
| `Globals::iPad == 0 ? gHangarCamYaw : gHangarCamYawIPad` | 43 | r4,r3 |
| `Globals::iPad < 1 ? gHangarCamYaw : gHangarCamYawIPad` | 43 | r4,r3 |
| `Globals::iPad ^ 1 ? gHangarCamYaw : gHangarCamYawIPad` | 44 | r4,r3 |
| `*(Globals::iPad ? &gHangarCamYawIPad : &gHangarCamYaw)` | 43 | r4,r3 |
| `if (Globals::iPad) yawTable = yawIPad; else yawTable = yaw;` | 43 | r4,r3 |
| `yawTable = yawIPad; if (!iPad) yawTable = yaw;` | 43 | r4,r3 |
| `switch (Globals::iPad) { case 0: yaw; default: yawIPad; }` | 43 | r4,r3 |
| `__builtin_expect(iPad, 0/1)` on the ternary | 43 | r4,r3 |
| `yawIPadTable` local, then `iPad ? yawIPadTable : yaw` | 43 | r4,r3 |

The compiler canonicalises the condition and always emits the else operand
first, so the original's true-first order is not reachable from source.  The
landed 11-byte form (farKey uses yawBits) remains the best size-exact shape.

## 2. `Hud::draw` — anchor is a global size ballast

The 19-byte damage-block residual is the unbraced `Status **volatile p =
&Globals::status;` anchor inside `if (hitFlags & 0x24)`.  It turns the block
into `mov r0,#300; it ne; strne r8,[sp,#0xd0]; str r0,[sl,#0x4b8]` (14 B)
instead of the original `itt ne; movne r0,#300; strne r0,[sl,#0x4b8]` (10 B).
Removing the anchor makes the damage block exact but drops the whole function
to **9328 B / 3216 insns** (a 10-instruction global re-colour), so the anchor
is not a local 4-byte ballast — it is a global register-allocation knob.

New placements tried this wave (all +4 B unless noted):

| anchor placement | raw | size |
|---|---:|---:|
| unbraced inside `&0x24` (landed) | **32** | 9332 |
| after both damage `if`s, unused | 7111 | 9336 |
| after both damage `if`s, `(void)p` | 7145 | 9336 |
| after both damage `if`s, `p; p = &status` | 7111 | 9336 |
| before both damage `if`s | 7134 | 9336 |
| braced inside `&0x24` | 7091 | 9336 |
| unbraced inside `&0x18` | 7075 | 9336 |
| unbraced inside `&1` / `&2` | 8793 / 8758 | 9484 / 9664 |
| `volatile int p = (int)&status` after the ifs | 7111 | 9336 |
| `*(volatile int *)&field_0x4b8 = 300` (no anchor) | 7339 | 9328 |
| `*(volatile int *)&field_0x4b4 = 300` (no anchor) | 7339 | 9328 |
| volatile read of `field_0x4b8` after the ifs | 2945 | 9332 |

The dock-transfer `r5`/`r6` swap (13 B) also resisted: `top`-first declaration
order → raw 110, `PaintCanvas *canvas` local → raw 48, `top/y/barW` order →
raw 1489 (size 9328).  Base is best.

## 3. `MenuTouchWindow::OnTouchBegin` — two independent ties

* `0x401` (2 B): case-8 `y < ... + this->listEntryWidth` loads
  `listEntryWidth` into `ip` (original) vs `r4` (ours).  Reordering the
  addition (`listEntryWidth + leftMargin + top`), a `listEntryWidth` local,
  a `listEntryHeight` local, and parenthesising the sum all leave raw 28.
* `0x60f..0x678` (26 B): case-15 scrollbar block.  The `Layout &layout` local
  is load-bearing: using it for `buttonInsetX`+`leftMargin` keeps 1896 B/raw
  28; using it for `buttonInsetX`+`top` gives raw 29 (aligned 15); using it
  for `leftMargin`+`top` or for all three drops to 1892 B (raw 308/362);
  removing the local drops to 1892 B (raw 362); a `heapBufB` local drops to
  1892 B (raw 318).  The `r2/r3/r4/r5/r6` rotation is unchanged.

## 4. `Generator::createMission` — exact bonus block costs 4 B in the LSDA

The note-shape bonus block (non-volatile `bonus` at `sp+16`, `volatile int
rem` + unconditional `int remCopy = rem`, `if (b%50) b = rawBonus - remCopy`)
reproduces the original bonus block byte-for-byte except that `rem` lands at
`sp+44` instead of the original `sp+32` (the dead `&Globals::status` temp
slot).  It is **2000 B**, +4 from the landed 1996 B.  The +4 is not the frame
(both are 96 B here); it is one extra `.ARM.exidx` landing pad `b.n` plus the
literal-pool alignment `nop` in the exception-cleanup tail:

```
original tail:  ... three b.n landing pads; mov r4,r0; ... literal pool at 9795c
note-shape:     ... four  b.n landing pads; mov r4,r0; ... nop; literal pool at 97960
```

Variants tried (all 2000 B / raw 90 unless noted): `volatile int rem` assigned
later; `int rem` + `*(volatile int *)&rem`; volatile store via pointer + plain
read (2000 B, 744 insns, raw 650); volatile store + volatile read twice
(raw 599); `volatile long long rem` (2004 B); `volatile unsigned rem`;
`volatile int remCopy` (2004 B); `volatile int remVal` (raw 613); `bonus`
direct; `if (b % 50)` without `!= 0`; `remCopy` inside the `if` (raw 716);
`int bonus = 0`; `volatile int bonus` (raw 169).  Adding the old `bv` anchor
back gives 2004-2008 B.  The landed 1996 B / raw 40 shape stays.

## 5. `MGame::OnUpdate` — m23 pair and m122 spill are coupled

* m23 64-bit idle compare (5 B): original `ldrd r2,r3` / `sbcs r1,r3` /
  `mov.w r2,#0`; ours `ldrd r3,r5` / `sbcs r1,r5` / `mov.w r3,#0`.  The
  status pointer in `r2` is dead after the load in both, but the allocator
  picks `r3:r5` in ours.  `3600000LL`, `>= 3600001`, reversed comparison,
  `(long long)` casts and `3600000 < ...` all leave raw 15; a `long long last`
  local, `last + 3600000`, and `last < now - 3600000` change the size
  (11432/11436) and blow the raw count.
* m122 block (10 B): the landed `Status *volatile st` anchor emits
  `str r2,[sp,#120]; blx nextInt; mov r4,r0; ldr r0,[sp,#120]` (10 B) where
  the original has `blx; mov r4,r0; ldr.w r0,[r9]` (10 B).  Switching to a
  direct `Globals::status->getCurrentCampaignMission()` with the anchor kept
  makes the m122 body exact but the block becomes 12 B, shifting the whole
  remainder by 2 (raw 361, size 11440, insn 4002/4002).  Removing the anchor
  drops the function to 11392 B.  `volatile int` anchor, `s2 = st`, volatile
  read of `*st`, and anchor-after-`nextInt` were all worse.

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

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13119/`
(`idiff.py`, `rawdiff.py`, `sweep_mod*.py`, `sweep_hud*.py`, `sweep_mtw*.py`,
`sweep_gen*.py`, `sweep_mg*.py`, `*.base` copies).
