# HangarWindow::render — the 948-byte extab record is a row-39 length-encoding problem (wave 160, w10616)

Status: **landed source change** (the wave-159 w10516 shape, re-verified on
the wave-159 master base).
Worktree `wt/w10616`, base `08af4633b` (wave 159). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, function 9,700 B, `.ARM.extab` record
948 B / 156 rows (the largest record in the binary).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  All 27 section sizes exact, file size
2,187,328 B.

## Result

| metric | before (base) | after |
|---|---:|---:|
| raw same-address diff | 1,965 | **2,514** |
| instruction-index alignment | 291 | **485** |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff bytes | 676 | **151** |
| its record length / rows | 948 / 156 | **948 / 156** |
| total `.ARM.extab` diff bytes | 1,256 | **731** |
| total `.ARM.extab` section size | 0x26298 | **0x26298** |

The landed shape is the wave-159 one:

```cpp
if (item->isShip()) {
    volatile int *dummy;            // lifetime anchor
    (void) dummy;
    if (__builtin_expect(tab != 0, 1)) { // lint: arm_builtin anchor: HangarWindow::render; branch-weight hint
        if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
            volatile int padX[1];   // LSDA sink
            ... buttons[1] ...
            if (this->upgradeMode && tab == 1) { ... buttons[2]/[10] ... }
        }
    }
}
```

## New finding: the 676-byte diff is one ULEB length byte, not 676 bad rows

The LSDA call-site table is a serialised stream of 4 ULEB128 values per row
(`cs_enc == 0x01`).  The base and the original agree on **113 of the 156
rows**; only 43 rows differ, and their *encoded lengths* are identical for
42 of them.  The single exception is row 39:

```
row  orig (start,len,lp)      base (start,len,lp)     encoded
 39  (2508, 150, 9664)        (2508, 118, 9664)       7 B vs 6 B
```

`150` needs a 2-byte ULEB, `118` only 1 byte.  That one byte shifts every
subsequent row, which is why the byte diff is 676 even though the row values
are close.  Simulating the original with only row 39 replaced by the base
value gives **65 differing bytes**; replacing rows 39+40 gives **61**.  The
"aligned-row" diff (same encoded length per row) is only **72 bytes over 43
rows**.  So the target is not "fix 676 bytes", it is "make row 39's length
encode in two bytes (>=128)".

## Why the base row 39 is 118 and the original is 150

Row 39 is the contiguous call-site range with LP 9664.  It starts at the
`Ship::getIndex` invoke of the row-rendering ship branch (fn+2508) and ends
at the last invoke with that landing pad.  In the original this is the
`Status::getShip` invoke of the selected-item ship branch (fn+2658, length
150).  In the base the `getShip` invoke instead unwinds to LP 9514 (the
cleanup that destroys **two** String slots, sp+288 and sp+312), so row 39
stops at the `ListItem::isShip` invoke (fn+2626, length 118) and row 40
starts at `getShip`.

The reason `getShip` gets the two-String cleanup is the threaded `tab == 0`
branch.  The original emits

```
149b24: cbz r0, 149b96        ; tab == 0 -> the tab == 1 check
```

while clang 7 on the base threads the edge through the `tab == 1` block:

```
149b24: cmp r0, #0
149b26: beq.w 14a6d0         ; tab == 0 -> END (skips the tab == 1 check)
```

`cbz` is 2 B, `cmp;beq.w` is 6 B: this is the whole +4 B difference between
the two shapes in this region (the original's `getShip` operand order is
2 B longer, so the net is -2 B, the well-known "+2 LP shift").  Because the
base's `getShip` block is merged into the button-1 basic block, it inherits
the button-1 landing pad (sp+288+sp+312) instead of the name cleanup
(sp+312 only).  Preventing the threading is therefore the key to both the
row-39 length and the landing-pad identity.

## Trials this wave (raw / aligned / extab-record / size / rec-len / rows)

Every row is a full build + `fdiff.py` + `align_diff.py` +
`extab_record_map.py` measurement.  "base" is the unmodified wave-159 master
source.

| variant | raw | al | extab | size | rec | rows |
|---|---:|---:|---:|---:|---:|---:|
| base (padX=0, `if (tab!=0)`, upgrade outside) | 1,965 | 291 | 676 | 9,700 | 948 | 156 |
| **landed: dummy + expect(tab!=0,1) + padX[1] + upgrade inside** | **2,514** | **485** | **151** | **9,700** | **948** | **156** |
| no dummy at all | 6,731 | 734 | 730 | 9,700 | 936 | 154 |
| combine `tab!=0 && !(...)` / `||` form | 6,731 | 734 | 730 | 9,700 | 936 | 154 |
| `padX` scalar volatile int/char/short/float | 7,317 | 728 | 733 | 9,708 | 948 | 156 |
| `padX` non-volatile int / void* / array / uninitialised | 1,965 | 291 | 676 | 9,700 | 948 | 156 |
| dummy before/inside the second if | 6,606 | 838 | 706 | 9,708 | 948 | 156 |
| dummy inside the first if | 8,618 | 772 | 783 | 9,676 | 948 | 156 |
| `if (tab==1 && upgradeMode)` (condition order swap) | 8,685 | 2,061 | 594 | 9,640 | 944 | 155 |
| `if (tab-1==0 ...)` / `(unsigned)tab==1` / `1==tab` | 8,685 | 2,061 | 594 | 9,640 | 944 | 155 |
| `expect(tab==0,1)` on first if | 7,353 | 852 | 739 | 9,664 | 952 | 157 |
| `expect(tab!=0,1)` alone (upgrade outside) | 2,260 | 398 | 465 | 9,700 | 944 | 155 |
| `expect(item->isShip(),1)` | 9,145 | 3,417 | 825 | 9,552 | 944 | 155 |
| goto `if (tab==0) goto check;` | 2,016 | 298 | 676 | 9,700 | 948 | 156 |
| do/while(0) around upgrade | 7,341 | 776 | 770 | 9,576 | 948 | 156 |
| inverted `if (tab==0) {} else {...}` | 2,016 | 298 | 676 | 9,700 | 948 | 156 |
| hoist `bool sameShip` / `Ship *curShip` | 6,694 | 678 | 723 | 9,712 | 968 | 159 |
| second `if (item->isShip())` around upgrade | 8,766 | 1,444 | 790 | 9,656 | 940 | 155 |
| **call after padX: `Globals::status->getCredits()`** | **6,558** | **641** | **256** | **9,704** | **948** | **156** |
| call after padX: `Globals::status->getShip()` | 6,558 | 641 | 256 | 9,704 | 948 | 156 |
| call after padX: `hangarList->getCurrentTab()` | 6,668 | 585 | 268 | 9,708 | 948 | 156 |
| call after padX: `layout->drawBG()` | 6,668 | 585 | 268 | 9,708 | 948 | 156 |
| call after padX: `Canvas->SetColor(0xffffffff)` | 6,712 | 837 | 281 | 9,712 | 948 | 156 |
| call after padX: `Canvas->GetImage2DWidth(hintImage)` | 6,699 | 839 | 281 | 9,712 | 948 | 156 |
| call after padX: `appManager->GetApplicationModule(5)` | 8,947 | 704 | 334 | 9,716 | 948 | 156 |
| call after padX: `getRelativeScrollStartPos()` (inlined) | 1,965 | 291 | 676 | 9,700 | 948 | 156 |

### The near-miss: a 4-byte call extends row 39 and cuts the record to 256

`volatile int *padX = 0; Globals::status->getCredits();` immediately after
`if (item->isShip())` extends row 39 to **134 bytes** (>=128, so the ULEB
length is 2 bytes) and drops the record diff to **256** with the 948/156
record intact.  The call costs only the 4-byte `blx` because
`Globals::status` is already live in r4 for the `getShip` comparison.  The
function grows to 9,704 B, which is gate-fatal (size is sacred).  If the
`cmp;beq.w` threading could be turned into the original's 2-byte `cbz`,
that 4-byte saving would make this variant size-exact with a 256-byte
record diff.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10616
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j32
python3 /dev/shm/w10616/fdiff.py _ZN12HangarWindow6renderEv       # 2514, 9700/9700
python3 /dev/shm/w10616/align_diff.py _ZN12HangarWindow6renderEv  # 485
python3 _work/reconstructions/extab_record_map.py --ours cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j32               # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10616/`: `measure.sh`, `rows.py`,
`rows_all.py`, `rowsim.py`, `mk*.py`, `fdiff.py`, `align_diff.py`.
