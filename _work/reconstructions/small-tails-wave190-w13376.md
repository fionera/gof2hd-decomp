# Small tails — wave 190 (w13376): MGame m122 anchor, Generator bonus, ModStation farKey

Worker `w13376`, branch `wave190-w13376`, base master `0f54626b3`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13376`.

Status: **research note, no additional `src/` change landed.**  The
`Level::createShip` rebase is in `level-createship-wave190-w13376.md`.
Three small tails were re-attacked with new source shapes; every candidate
either changes the function size or loses raw to the landed body.

## 0. Baseline (all at the committed wave-190 source)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **10** | 112 | 4003/4002 |

## 1. `MGame::OnUpdate` — m122 can be made instruction-exact, but the anchor store shifts it

The landed anchor is

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
Status *volatile dummy = st;
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
Level *level = this->level;
level->createRadioMessage(28, radioIndex + radioBase);
```

Changing the `radioBase` expression to `Globals::status->getCurrentCampaignMission()`
(so m122 reloads the pointer with `ldr r0,[r9]`, exactly as the original)
**fixes the m23/m24 64-bit-compare register pair and makes the m122 block
instruction-exact except for the 2-byte `str r5,[sp,#120]` dummy store**:

```
17f190 mov  r4,r0          ; original: same
17f192 ldr  r0,[r9]        ; original: same
17f196 str  r5,[sp,#120]   ; ours only (dummy anchor)
17f198 blx  getCurrentCampaignMission
```

The 2-byte store shifts the whole m122 block and the following code by 2,
so raw rises to **348** (aligned **18**, size 11440 exact, insns 4002/4002).
The `st`/dummy pair is load-bearing for the function size: removing `st`
entirely gives size **11392** (-48) and raw 5196.  Moving the dummy store
before `nextInt` gives raw 361 and re-breaks m24.  A reference
(`Status *&st = Globals::status`) also gives size 11392.  So the base
**10 / 112 / 11440** remains the best raw shape; the 348-byte variant is
an instruction-alignment candidate, not a raw win.

## 2. `Generator::createMission` — the bonus anchor store is the only residual

The 40 raw bytes are two regions: the `rawBonus % 50` bonus block
(`0x97718`, 9 B) and the exception cleanup table it shifts (`0x97918`,
21 B).  The original bonus block is

```
adds r5,r1,r6 ; b = rawBonus + remVal
str  r1,[sp,#32] ; remVal
movs r1,#50 ; mov r0,r5 ; blx idivmod
ldr  r0,[sp,#32] ; cmp r1,#0
it   ne
subne r5,r6,r0 ; b = rawBonus - remVal
str  r5,[sp,#16] ; bonus = b
```

Ours adds the `bv = rawBonus` volatile anchor store and lowers the `if` as
`cbz` + `subs` instead of `it ne` + `subne`.  Swept this wave (all size
1996 unless noted):

| bonus shape | raw | aligned | size |
|---|---:|---:|---:|
| landed | **40** | 21 | 1996 |
| `if (b % 50)` / ternary / `if/else` forms | 40 | 21 | 1996 |
| `bv = b` | 45 | 31 | 1996 |
| `bv` after `bonus = b` | 41 | 21 | 1996 |
| `volatile int bv2 = rawBonus;` local | 44 | 25 | 1996 |
| `volatile int bv2 = b;` local | 49 | 35 | 1996 |
| `__builtin_expect(b % 50 != 0, 1)` (with anchor) | 40 | 21 | 1996 |
| anchor removed + `expect(...,1)` | 582 | 126 | 1996 |
| `rem` non-volatile | 659 | 143 | 2004 |
| `bv` before `int b` / before `rem` | 621 / 627 | 77 / 76 | 2004 |

No size-exact shape removes the anchor store; 40 stays.

## 3. `ModStation::ModStation` — farKey yaw register tie

The 11 raw bytes are the farKey `MatrixSetRotation` yaw load: original
`ldr.w r3,[r0,r8,lsl#2]` (yaw in r3) with `mov r4,r2` (pitch in r4); ours
`ldr.w r4,[r0,r8,lsl#2]` with `mov r3,r4`.  Reusing the cached
`yawTable[race]` instead of the direct `(Globals::iPad ? ...)[race]`
expression gives size **968** (-40), not exact.  The landed
`pitch`/`yaw` naming and direct ternary reload remain the unique
size-1008 / raw-11 shape (consistent with w13339's `local_yawfar` cells).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13376
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5MGame8OnUpdateEv \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# -> MGame 10/112/11440, Generator 40/21/1996, ModStation 11/5/1008
```

Scratch: `MGame_base.cpp`, `Generator_base.cpp`, `ModStation_base.cpp`,
`gen_sweep.py`/`gen_sweep2.py`/`gen_sweep3.py`, `regions.py`, `sec.py`.

## 5. Files changed

* this note only.
