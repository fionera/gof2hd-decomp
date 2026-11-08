# Small tail wave 158 (w10318): MenuTouchWindow B1-direct/B2-cached landed
# (raw 29 -> 28); Gun / ModStation / MGame::OnUpdate re-probed

Status: **one landed `src/` change** (`MenuTouchWindow::OnTouchBegin`, raw
29 -> 28, aligned 15 -> 16). Worktree `wt/w10318`, base master `08575efff`
(wave 156). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave157-w10217.md`, `small-tail-wave156-w10111.md`,
`small-tail-wave155-w10017.md`.

Gate before/after (full `check`): **RATCHET PASS**, `byte_exact 4507 -> 4507`,
`linked_exact 4507 -> 4507`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, avg 99.93.  All four bodies stay
size-exact (1984 / 1008 / 1896 / 11440).

Metrics: raw same-address diff bytes (`/dev/shm/w10318/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10318/align_diff.py`, path-adjusted
copy of the committed tool).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` (`_ZN3Gun22calcCharacterCollisionEv`) | 463 | 463 | 57 | 57 | 1984 |
| `ModStation::ModStation` C1 (`_ZN10ModStationC1Ev`) | 43 | 43 | 12 | 12 | 1008 |
| `MenuTouchWindow::OnTouchBegin` (`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`) | 29 | **28** | **15** | 16 | 1896 |
| `MGame::OnUpdate` (`_ZN5MGame8OnUpdateEv`) | 37 | 37 | 109 | 109 | 11440 |

## 1. MenuTouchWindow::OnTouchBegin — LANDED the w10217 B1-direct/B2-cached shape

The wave-156 master shape is `field_0x20_top` via the cached `Layout &`
(B1 cached) and `field_0xc_leftMargin` direct (B2 direct): 29 / 15.  The
w10217 shape swaps them (B1 direct, B2 via the cached reference): 28 / 16.
The landed one-line change is exactly that:

```cpp
Layout &layout = *Globals::layout;
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(...) &&
    y < Globals::layout->field_0x20_top + (layout.field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(...)))
```

The original reloads `Globals::layout` after the first `GetImage2DWidth` call
and uses the reloaded pointer for **both** `field_0xc_leftMargin` and
`field_0x20_top`:

```
12a212 ldr r2,[sp,#16]     ; &Globals::layout (spill)
12a218 ldr r2,[r2,#0]      ; Globals::layout  <- reload after the call
12a21e ldr r5,[r2,#12]     ; field_0xc_leftMargin
12a220 ldr r6,[r2,#32]     ; field_0x20_top
```

So the true original shape is B1 **and** B2 direct.  But with both direct the
compiler drops the inner `Layout &layout` reference entirely and stops
spilling `&Globals::layout`, shrinking the body 1896 -> 1892 and recolouring
the whole case-15 block (raw 358 / aligned 76).  The spill is kept only if at
least one of B1/B2 goes through the cached reference.  The two size-1896
shapes are therefore the two one-cached/one-direct variants; the original's
both-direct shape is not reachable from source without the body shrinking.

New shapes measured this wave (all 1896 B unless noted; all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD wave-156 (B1 ref, B2 direct) | 29 | **15** | 1896 |
| **LANDED w10217 (B1 direct, B2 ref)** | **28** | 16 | 1896 |
| both direct (inner ref dead) | 358 | 76 | 1892 |
| A ref, B1 direct, B2 direct | 358 | 76 | 1892 |
| A ref, B1 direct, B2 ref | 28 | 16 | 1896 |
| A ref, B1 ref, B2 direct | 29 | 15 | 1896 |
| B1 ref + B2 ref | 304 | 73 | 1892 |
| C via ref (B1 direct, B2 ref) | 287 | 74 | 1892 |
| C via ref (B1 ref, B2 direct) | 314 | 74 | 1888 |
| `int h = Globals::h;` before the case-1/2 test | 28 | 16 | 1896 |
| `y + rightMargin < Globals::h` | 37 | 20 | 1896 |
| `Globals::h > y + rightMargin` | 36 | 19 | 1896 |
| second-if operand order swapped | 42 | 22 | 1896 |
| second-if `x - width < inset` | 268 | 39 | 1896 |

The independent `ip`/`r4` tie at `0x129fd2` (case-1/2 `Globals::h`) and the
`r5`/`r4` tie at `0x12a246` (second-if `buttonInsetX`) are unchanged by every
shape above; they are Greedy-RA choices, not source-order artifacts.

Trade-off: raw improves 29 -> 28 but aligned rises 15 -> 16.  The raw metric
is the byte-matching metric; byte_exact is unchanged either way (the function
is not exact).  This is the same shape wave 157 (w10217) landed; wave 149 had
reverted it to 29/15.  If the coordinator prefers aligned, revert this one
hunk and keep 29/15.

## 2. Gun::calcCharacterCollision — 463/57 re-confirmed; r4/r5 tie still open

HEAD remains **463 / 57 / 1984**.  The residual is still the single
Greedy-RA choice: `i*3` (and the `getPosition` sret temp) gets r5 here and
r4 in the original; the subtraction temp gets the other.  New shapes
measured this wave (all 1984 B unless noted; all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **463** | **57** | 1984 |
| `enemyPosition` loaded before position/velocity | 600 | 106 | 1984 |
| `int radius` before the Vector copies | 1774 | 200 | 1968 |
| default-construct then assign position/velocity | 1706 | 289 | 1952 |
| `(void) radius` after the radius init | 518 | 62 | 1984 |
| hoist `const unsigned posCount = positions.count` | 1681 | 152 | 1988 |
| `const volatile unsigned posCount` | 1804 | 167 | 1996 |

The `posCount` hoists were measured on a sweep that accidentally patched all
four `for (unsigned i = 0; i < this->positions.count; ++i)` loops in Gun.cpp
(they are textually identical); the numbers above are from the corrected
single-site rebuild for the first two and the all-four build for the last
two, so treat them as indicative only.  Forcing `i*3` into r4 without
changing the 1984-byte body is still open.

## 3. ModStation::ModStation — 43/12 re-confirmed, no new shape

HEAD remains **43 / 12 / 1008**.  The residual is the literal-pool order
(`YawIPad`/`Yaw` ldr swap), `mov r0,r3` before `cmp r1,#0`, and the PostRA
schedule of `str r8,[sp,#184]` / `mov r4,r2` / `ldr r3,[r0,r8,lsl#2]`.
The `volatile` yaw element load is still the only lever that gives the
original select operand order.  New shapes measured this wave (all 1008 B
and 43/12 unless noted; all reverted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD | **43** | **12** |
| local `bool ipad = Globals::iPad` | 43 | 12 |
| `(Globals::iPad != 0) ? ... : ...` | 43 | 12 |
| default + `if (Globals::iPad) yawTable = ...` | 43 | 12 |
| named `yawIPad` / `yawNonIpad` locals | 43 | 12 |
| `volatile const int *pitchPtr` / `yawPtr` locals | 43 | 12 |
| `float pitch` declared before `float yaw` | 43 | 12 |
| yaw load before pitch load | 43 | 12 |

## 4. MGame::OnUpdate — 37/109 remains the best raw shape

HEAD remains **37 / 109 / 11440**.  The two `mov r0,r5` are the `Status *st`
value in r5.  The whole body is byte-identical to the original except the
mission-122 `getCurrentCampaignMission`/`getPlayingTime` receivers, the
condition's `ldr r2`/`ldr r5` colouring, and the branch-target shifts.

New shapes measured this wave (all 11440 B unless noted; all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed (`st` for ccm + store) | **37** | **109** | 11440 |
| pure direct (no `st`) | 5148 | 597 | 11392 |
| `Status *&st = Globals::status` (reference) | 5148 | 597 | 11392 |
| `Status **st = &Globals::status; (*st)->...` | 5148 | 597 | 11392 |
| `st` before the else-if chain, `st` for ccm+store | 890 | 162 | 11440 |
| `st` for ccm, store direct | 914 | 109 | 11436 |
| `st` for store RHS, ccm direct | 318 | **19** | 11440 |

The pure-direct family confirms the register-allocation mechanism: without
a live `Status*` value the allocator puts `&Globals::status` in r5 and every
`ldr.w r0,[r9]` becomes a 2-byte `ldr r0,[r5]`, shrinking the body 48 B.
The landed `st` shape occupies r5 and forces `&Globals::status` into r9,
matching the original's size and r9 colouring, at the cost of two `mov r0,r5`
(the ccm and store receivers).  The 318/19 shape (store via `st`) removes the
ccm mov and the condition colouring but the store's `mov` shifts the shared
tail, so raw is worse.  A source shape that keeps `&Globals::status` in r9
without a live `Status*` in r5 is still open.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10318
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10318/fdiff.py <mangled>
python3 /dev/shm/w10318/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10318/`: `fdiff.py`, `align_diff.py`,
`sbs.py`, `measure.sh`, `mod_sweep*.py`, `mtw_sweep*.py`, `gun_sweep*.py`,
`*.base.cpp`, `MenuTouchWindow.head.cpp`.

## 6. Files changed

* `src/game/ui/MenuTouchWindow.cpp` — case-15 scrollbar test: B1
  (`field_0x20_top`) direct, B2 (`field_0xc_leftMargin`) via the cached
  `Layout &` reference.  Size 1896 unchanged; raw 29 -> 28, aligned 15 -> 16.
* `_work/reconstructions/small-tail-wave158-w10318.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.
