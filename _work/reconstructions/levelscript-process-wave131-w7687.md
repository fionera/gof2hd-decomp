# LevelScript::process — wave 131 / w7687 (per-case diff map; two real semantic bugs found, both size-shrinking)

Status: **no code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `7c6933c2d`.
Scratch helpers in `/dev/shm/w7687/` (not tracked).

## Result

```
                         before (master)   after (no code change)
raw ratio (this pass)    59.88             59.88   (w7668 scorer: 60.09)
official verify pct      60.1              60.1
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams.

## New hypothesis tested: per-case diff map over the high-TBH bodies

Built a per-case map: decode the main-switch compare chain and both `tbh` tables on
both sides (`/dev/shm/w7687/tbh.py`, `percase.py`, `diffregions.py`), then score each
case body region (entry → next body entry) with difflib. The high-TBH body **order is
identical** on both sides (confirmed again: 131,135,145,142,144,139 and
64,69,73,67,70), so the remaining per-case differences are intra-body, not ordering.

Worst per-case ratios (orig/ours insns in the address-range approximation):

```
case 91   8.0%   39/11     (body split across cold blocks)
case 40   8.1%  135/38
case 102  8.7%  105/33
case 125  9.3%   37/372
case 154 12.0%  169/130
case 64  45.2%  474/429
case 70  55.9%  260/116
case 145 56.7%  203/210
case 135 67.0%  109/109   <- same length, essentially identical apart from registers
case 67  68.0%   98/99
```

The low ratios are dominated by cold-block placement and register allocation, not
statement order: e.g. case 135 is instruction-for-instruction identical apart from
`r0/r1` and the two loop-increment adds being swapped.

## Semantic bug 1 — case 131 state 2 is missing the `dir * 2.01` term

Original case-131 state 2 (`0x13c8fc`, the `if (m_nState != 2) { if (m_nState == 1) … }
…` fall-through body) is **the same camera-creep body as state 1**:

```
13c942  blx  PlayerEgo::GetDirVector
13c94e  blx  Vector::operator=(scratch, dir)
13c95c  vldr s0,[pc]        ; 2.01
13c970  blx  operator*(tmp, scratch, delta*2.01)
13c980  blx  operator+=(tempVec, tmp)
13c982  ldr  r1,[fp,#8]      ; player->geometry
13c98c  blx  getRightVector
13c994  blx  operator=(scratch, right)
13c996  vldr s0,[pc]        ; 0.03
13c9aa  blx  operator*
13c9b0  blx  operator+=
13c9b8  blx  GetUpVector
13c9d8  blx  operator*
13c9de  blx  operator+=
```

Our source (case 131, state 2) had:

```cpp
*tempVec = AEMath::MatrixGetPosition(local);
*scratch = player->GetDirVector();
*scratch = player->geometry->getRightVector();   // <-- overwrites dir before use
*tempVec += *scratch * (delta * 2.01f);          // uses right, not dir
*tempVec += *scratch * (delta * 0.03f);
*scratch = player->GetUpVector();
*tempVec += *scratch * (delta * 0.01f);
```

i.e. the `GetDirVector` result is dead and the right vector is applied twice. The
faithful body is identical to state 1:

```cpp
*tempVec = AEMath::MatrixGetPosition(local);
*scratch = player->GetDirVector();
*tempVec += *scratch * (delta * 2.01f);
*scratch = player->geometry->getRightVector();
*tempVec += *scratch * (delta * 0.03f);
*scratch = player->GetUpVector();
*tempVec += *scratch * (delta * 0.01f);
```

Measured: **57224 B (-8)**, raw ratio 59.09 (global layout shifts; the local state-2
body becomes structurally identical to the original apart from r4/r5/r9/fp register
colouring). This is a real cinematic-camera bug.

## Semantic bug 2 — case 157 setVisible target is the wrong object

Original `0x145430..0x145440`:

```
145430  ldr  r0,[sl,#196]   ; this+0xc4 = m_pGeometry3
145434  movs r1,#1
145436  blx  AEGeometry::setVisible(true)
14543a  ldr  r0,[sl,#192]   ; this+0xc0 = m_pGeometry2
14543e  movs r1,#0
145440  blx  AEGeometry::setVisible(false)
```

Our source (case 157, `m_pGeometry3->setVisible(true);`) had
`(*enemies)[stationIndex]->setVisible(false);` — a KIPlayer store, not a geometry
store. The faithful statement is `m_pGeometry2->setVisible(false);`.
Measured alone: **-4 B** (the `enemies[i]` load is one instruction longer than the
`this+0xc0` load).

## Why neither landed: the size compensator wall

`byte_exact` is a raw-byte gate over all 4524 functions. Shrinking `process` by 8 B
moves every following symbol, and the ratchet measured **byte_exact 4506 -> 3693**
(same linked_exact 4506). So a size-exact 57232 B result is mandatory.

Measured combinations (all on top of master 57232):

| variant | size | raw % |
|---|---:|---:|
| base | 57232 | 59.88 (this scorer) |
| case-131 state-2 reorder | 57224 | 59.09 |
| + case-157 `m_pGeometry2` | 57220 | 59.09 |
| + case-40 state-1 `translate(0,0,-2*delta)` (wave-129) | 57244 | — |
| `Vector dir = player->GetDirVector()` local instead of `*scratch` | 57452 | 54.78 |

The case-40 translate is **+24 B** (not the +16..+24 estimate), so it overshoots by 12.
No genuine +8/+12 statement was found in the time box. Both fixes are correct and
should be landed together with a size-neutralising companion in a later pass.

## Open lead

Land the two semantic fixes (net -12 B) together with a **+12 B** faithful companion.
Candidates not yet exhausted: the original has one more `TargetFollowCamera::setPosition
(float,float,float)` call than ours (8 vs 7) and two more `translate` calls (57 vs 55);
either is likely the missing statement. The `translate` probe costs +24, so the missing
`setPosition(f,f,f)` is the better bet.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7687/percase.py                       # per-case diff map
python3 /dev/shm/w7687/diffregions.py                   # largest diff blocks
```

Scratch helpers: `tbh.py`, `percase.py`, `diffregions.py`, `calls.py`, `callset.py`,
`dis2.py`, `dumpours.py`, `LevelScript.base.cpp`.
