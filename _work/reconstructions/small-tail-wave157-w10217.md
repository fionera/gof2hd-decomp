# Small tail wave 157 (w10217): MenuTouchWindow raw 29 -> 28; Gun / ModStation /
# MGame::OnUpdate re-probed (no further win)

Status: **one landed `src/` change** (`MenuTouchWindow::OnTouchBegin`, raw
29 -> 28 at aligned 15 -> 16). Worktree `wt/w10217`, base master `cf74f4ee6`
(wave 155). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave156-w10111.md`, `small-tail-wave155-w10017.md`,
`small-tail-b-wave154-w10018.md`.

Gate before/after (full `check`): **RATCHET PASS**, `byte_exact 4507 -> 4507`,
`linked_exact 4507 -> 4507`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, avg 99.93.  All four bodies stay
size-exact.

Metrics: raw same-address diff bytes (`/dev/shm/w10217/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10217/align_diff.py`, path-adjusted
copy of the committed tool).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` (`_ZN3Gun22calcCharacterCollisionEv`) | 463 | 463 | 57 | 57 | 1984 |
| `ModStation::ModStation` C1 (`_ZN10ModStationC1Ev`) | 43 | 43 | 12 | 12 | 1008 |
| `MenuTouchWindow::OnTouchBegin` (`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`) | 29 | **28** | **15** | 16 | 1896 |
| `MGame::OnUpdate` (`_ZN5MGame8OnUpdateEv`) | 37 | 37 | 109 | 109 | 11440 |

## 1. Gun::calcCharacterCollision — the r4/r5 swap is a Greedy-RA tie

HEAD remains **463 / 57 / 1984**.  The root is one virtual: the `i * 3`
index.  Greedy-RA MIR (`-mllvm -print-after-all`) shows

```
%526:rgpr = t2ADDrs %520:rgpr, %520:rgpr, 10   ; i*3, def at 2896B
%100:gprnopc = t2ADDrs %98:gprnopc, %526:rgpr, 18
%104:gprnopc = t2ADDrs %103:gprnopc, %526:rgpr, 18
```

and the Virtual Register Rewriter emits `$r5 = t2ADDrs $r11, $r11, 10`.
The original is `add.w r4, fp, fp, lsl #1`.  At the definition point the
block live-ins are only `$r8, $r9, $r11`; `$r4` is free and the `rGPR`
allocation order is r0..r7, so this is not ordinary interference — it is a
Greedy-RA order/hint tie.  The sret temp (`sp+44`) and subtraction temp
(`sp+104`) follow the same swap because the sret temp reuses the index
register after `mov r6,r5`.

New source shapes measured this wave (all 1984 B unless noted; all reverted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD (`Vector position` / `Vector velocity` named copies) | **463** | **57** |
| velocity loaded before position | 478 | 80 |
| `Vector *pos = data() + i; Vector *vel = ...` | 485 | 61 |
| `&data()[i]` pointer form | 485 | 61 |
| copy-constructor `Vector position(data()[i])` | 463 | 57 |
| `data()[(unsigned) i]` | 463 | 57 |
| `data()[3 * i]` / `[i * 3u]` / `[i * 3L]` / `[i + (i << 1)]` | 1763 | 368 (1940 B) |
| `unsigned i3 = i * 3; data()[i3 / 3]` | 1750 | 326 (1964 B) |
| `__builtin_expect` on the `i` loop condition | 1481 / 1746 | 191 / 490 |

The `3 * i` family loses the shared `t2ADDrs` and shrinks to 1940 B, so it
is not the original form.  A source shape that pins `%526` to r4 without
changing the 1984-byte body is still open.

## 2. ModStation::ModStation — 43/12 re-confirmed, no new shape

HEAD remains **43 / 12 / 1008**.  The residual is still the two
literal-pool `ldr` swaps (`YawIPad`/`Yaw`), `mov r0,r3` before `cmp r1,#0`,
and the PostRA schedule of `str r8,[sp,#184]` / `mov r4,r2` /
`ldr r3,[r0,r8,lsl#2]`.  The `volatile` yaw element load is the only lever
that gives the original select operand order; dropping it fixes the pool
order but the scheduler sinks the `Yaw` LDRLIT and hoists `mov r4,r2`
(44/18).  No new shape was found this wave; the w154/w156 sweeps are
saturated.

## 3. MenuTouchWindow::OnTouchBegin — LANDED: w9918 reload shape

Landed the w9918 shape: keep the inner `Layout &layout = *Globals::layout;`
but move the reference use from C (`field_0x20_top`) to B
(`field_0xc_leftMargin`), and make C a direct `Globals::layout->field_0x20_top`:

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - Canvas->GetImage2DWidth(...) &&
    y < Globals::layout->field_0x20_top +
        (layout.field_0xc_leftMargin + Canvas->GetImage2DHeight(...)))
```

The original reloads `Globals::layout` after the first `GetImage2DWidth` call
and uses it for both `field_0xc_leftMargin` and `field_0x20_top`:

```
12a212 ldr r2,[sp,#16]     ; &Globals::layout (spill)
12a218 ldr r2,[r2,#0]      ; Globals::layout  <- reload after the call
12a21e ldr r5,[r2,#12]     ; field_0xc_leftMargin
12a220 ldr r6,[r2,#32]     ; field_0x20_top
```

The landed shape makes C reload correctly (`ldr r5,[r2,#32]` after the
call), but B is served from the cached reference and hoisted before the call
(`ldr r4,[r6,#12]`), which is the one extra instruction-index mismatch
(`imm:ldr`).  Raw drops 29 -> 28; aligned rises 15 -> 16.

New shapes measured this wave (all 1896 B unless noted; all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (A,B direct; C ref; D direct) | 29 | **15** | 1896 |
| **LANDED w9918 (A,B ref; C direct; D direct)** | **28** | 16 | 1896 |
| volatile reload of `Globals::layout` for B | 33 | 21 | 1896 |
| `Layout *lay = Globals::layout;` reassigned after the call for B | 29 | 15 | 1896 |
| reference declared after the first call (`(Layout &)*Globals::layout`) | 358 | 76 | 1892 |
| A ref + D ref, B/C direct | 314 | 74 | 1888 |

The `ip`/`r4` tie at `0x129fd2` (case 8 `listEntryWidth` scratch) is
independent and unchanged.

## 4. MGame::OnUpdate — 37/109 remains the best raw shape

HEAD remains **37 / 109 / 11440**.  The two `mov r0,r5` are `%142`
(the `Status *st` value) in r5.  The whole body is byte-identical to the
original except the mission-122 `getCurrentCampaignMission` and
`getPlayingTime` receivers and the branch-target shifts.

New hypotheses measured this wave (all 11440 B unless noted; all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed (`st` for ccm + store) | **37** | **109** | 11440 |
| pure direct (no `st`) | 5148 | 597 | 11392 |
| `st` declared after `createRadioMessage`, used for store RHS | 5148 | 597 | 11392 |
| `st` for store LHS, calls direct | 5253 | 761 | 11492 |
| volatile reload for ccm, `st` for store RHS | 318 | **19** | 11440 |
| volatile reload for the condition `field_0x100` | 37 | 109 | 11440 |
| `Status *volatile st` | 331 | 116 | 11440 |
| `st` before the else-if chain, direct calls | 8719 | 1929 | 11392 |

The volatile-ccm shape (318/19) removes the ccm `mov` but the extra 2-byte
store `mov` shifts the rest of the 11440-byte body, so raw is much worse.
The landed shape keeps the body size and the original's global allocation
(`&Globals::status` in r9, `st` in r5); a source shape that keeps r9 without
a live `Status*` in r5 is still open.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10217
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10217/fdiff.py <mangled>
python3 /dev/shm/w10217/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10217/`: `fdiff.py`, `align_diff.py`,
`measure.sh`, `sweep_gun*.py`, `*.orig.asm`, `*.ours.asm`, `gun.mir`
(`-mllvm -print-after-all`), LLVM 7.0.2 sources (`RegAllocGreedy70.cpp`,
`CalcSpillWeights70.cpp`, `AllocationOrder70.cpp`, `ARMBaseRegisterInfo70.cpp`,
`ARMRegisterInfo70.td`).

## 6. Files changed

* `src/game/ui/MenuTouchWindow.cpp` — case-15 scrollbar test: C direct,
  B via the cached `Layout &` reference.  Size 1896 unchanged; raw 29 -> 28,
  aligned 15 -> 16.
* `_work/reconstructions/small-tail-wave157-w10217.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.
