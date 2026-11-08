# HangarWindow::render — 156-row extab variants and the section-size constraint (wave 159, w10516)

Status: **landed source change** (the wave-158 w10416 shape, re-verified and
swept against the `.ARM.extab` size constraint).
Worktree `wt/w10516`, base `cfde1f5f2` (wave 158). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, the largest `.ARM.extab` record
(676 of 1,284 differing bytes at wave 158).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Result

| metric | before (wave 158) | after |
|---|---:|---:|
| raw same-address diff | 1,965 | **2,514** |
| instruction-index alignment | 291 | **485** |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff bytes | 676 | **151** |
| its `.ARM.extab` record length / rows | 948 / 156 | **948 / 156** |
| total `.ARM.extab` diff bytes | 1,284 | **759** |
| total `.ARM.extab` section size | 0x26298 | **0x26298** |
| `.text` size | 0x1337c8 | **0x1337c8** |

All 27 section sizes stay exact and the file size stays 2,187,328 B.

## The landed shape (unchanged from w10416)

```cpp
if (item == this->selectedItem) {
    if (item->isShip()) {
        // Lifetime anchor: the original's LSDA has a cleanup region
        // covering the whole ship branch.  This unused local reproduces
        // its lifetime markers (and hence the cleanup blocks).
        volatile int *dummy;
        (void) dummy;
        if (__builtin_expect(tab != 0, 1)) { // lint: arm_builtin anchor: HangarWindow::render; branch-weight hint
            if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
                // LSDA sink: lifetime starts after getShip(), so the getShip
                // invoke unwinds to the outer cleanup (row 39) while
                // setPosition..upgrade unwind to this cleanup (row 40).
                volatile int padX[1];
                (*this->buttons)[1]->setPosition(...);
                ...
                if (this->upgradeMode && tab == 1) {   // moved inside
                    ... upgrade buttons ...
                }
            }
        }
    } else { ... }
}
```

## Row table (fn 0x1490d8, 156 rows, 948 B)

```
row  orig (start,len,lp)      ours
 36  (2336, 52, 9664)         (2336, 52, 9666)     LP +2
 37  (2412, 30, 9454)         (2412, 30, 9456)     LP +2
 38  (2456, 34, 9334)         (2456, 34, 9334)     exact
 39  (2508, 150, 9664)        (2508, 158, 9666)    getShip in outer cleanup
 40  (2696, 140, 9562)        (2680, 26, 9356)     comparison row
 41  (2854, 6, 9664)          (2718, 4, 9224)
 42  (2874, 26, 9356)         (2724, 32, 9328)
 43  (2912, 4, 9224)          (2776, 4, 9380)
 44  (2918, 32, 9328)         (2818, 140, 9588)    setPosition..upgrade
 45  (2992, 4, 9418)          (2992, 4, 9420)      LP +2
 46  (3008, 154, 9588)        (3008, 154, 9590)    LP +2
```

The remaining 151 differing bytes are the +2 LP shift (one extra `b.n`
cleanup trampoline at fn offset 9382) and the reordered comparison row.
The +2 shift is a MachineBlockPlacement artefact: ours lowers
`if (item->isShip())` as `cbz` (2 B) while the original keeps the ship body
inline and uses `cmp; beq.n` (4 B).  The `.text` size is nevertheless exact
because the extra trampoline displaces the final alignment `nop`.

## New sweep: why no cheaper 156-row variant exists

The original record is 948 bytes / 156 rows.  The `.ARM.extab` section size
is exact only when every record keeps its original length, so a variant that
changes the row count changes the section size (944 B / 155 rows or 956 B /
157 rows) and is a section-size regression even when `check` still passes.

Measured this wave (`/dev/shm/w10516/sweep26.py` .. `sweep30.py`; each row is
raw / aligned / extab-fn / fn size / record len / rows):

| variant | raw | al | extab | size | rec | rows |
|---|---:|---:|---:|---:|---:|---:|
| base (wave 158, no expect/sink) | 1,965 | 291 | 676 | 9,700 | 948 | 156 |
| **landed: expect + dummy + array padX + upgrade inside** | **2,514** | **485** | **151** | **9,700** | **948** | **156** |
| expect + dummy, no sink | 2,260 | 398 | 465 | 9,700 | 944 | 155 |
| expect only, no dummy/sink | 6,750 | 809 | 521 | 9,700 | 944 | 155 |
| no expect + dummy (== base metrics) | 1,965 | 291 | 676 | 9,700 | 948 | 156 |
| `__builtin_expect(tab == 0, 0)` | 8,028 | 922 | 795 | 9,568 | 948 | 156 |
| `__builtin_expect(tab, 1)` / `tab != 0u` / `(bool)tab` | 2,514 | 485 | 151 | 9,700 | 948 | 156 |
| dummy scalar `volatile int` / `char` / `void*` / `int(*)[1]` / non-volatile / 2 dummies | 2,514 | 485 | 151 | 9,700 | 948 | 156 |
| `padX[2]` instead of `padX[1]` | 2,514 | 485 | 151 | 9,700 | 948 | 156 |
| `||` form `tab != 3 || getShip() != ship` | 2,514 | 485 | 151 | 9,700 | 948 | 156 |
| explicit `switch (tab)` + dummy + sink | 2,276 | 410 | 697 | 9,700 | 956 | 157 |
| `if (expect(tab==0,0)) {} else if (...)` | 2,240 | 407 | 694 | 9,700 | 956 | 157 |
| hoist `Ship *cur = tab==3 ? getShip() : 0` | 6,518 | 925 | 711 | 9,704 | 956 | 157 |
| hoist `bool same = ...getShip()...` | 6,566 | 671 | 709 | 9,708 | 956 | 157 |
| whole-condition `expect(tab!=0 && (...), 1)` | 7,283 | 1,763 | 345 | 9,656 | 948 | 156 |
| whole-condition plain `tab!=0 && (...)` | 2,276 | 410 | 697 | 9,700 | 956 | 157 |
| `padX` after `setPosition` | 2,468 | 490 | 684 | 9,700 | 948 | 156 |
| `padX` as `volatile int *` | 2,514 | 485 | 151 | 9,700 | 948 | 156 |

The `switch`/`else-if` forms reach raw 2,240-2,276 but they emit a 956-byte
/ 157-row record (+8 B `.ARM.extab`).  The "expect only" form reaches raw
2,260 but emits a 944-byte / 155-row record (-4 B `.ARM.extab`).  Every form
that keeps the 948-byte / 156-row record and moves the row 39/40 split costs
exactly 2,514 / 485.  The w10416 shape is therefore the unique Pareto point
under the section-size constraint.

### Why the +2 shift is the remaining floor

The original's landing-pad region ends with a `nop` at fn offset 9698; ours
ends 2 bytes earlier because the extra `b.n` cleanup trampoline at 9382
consumes the alignment padding.  Removing it needs the ship body to be placed
inline (original) instead of the non-ship `switch` body (ours), which is a
MachineBlockPlacement decision not reachable from source order.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10516
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10516/fdiff.py _ZN12HangarWindow6renderEv       # 2514, 9700/9700
python3 /dev/shm/w10516/align_diff.py _ZN12HangarWindow6renderEv  # 485
python3 _work/reconstructions/extab_record_map.py                 # fn 151, total 759
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10516/`: `sweep26.py` .. `sweep30.py`,
`rows2.py`, `fdiff.py`, `align_diff.py`, `final.cpp`.
