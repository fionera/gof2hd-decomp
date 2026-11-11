# HangarWindow::render — original ship-guard order, raw win (wave 176, w12014)

Worktree `wt/w12014`, branch `wave176-w12014`, base master `b21088985`
(wave-175 merge). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows.

## Result

Landed the original ship-guard block order in `src/game/ui/HangarWindow.cpp`:

```cpp
if (item->isShip()) {
    int upgTab = tab;
    if (tab != 0 && (tab != 3 || Globals::status->getShip() != this->selectedItem->ship)) {
        (*this->buttons)[1]->setPosition(...);
        (*this->buttons)[1]->setVisible(true);
        (*this->buttons)[1]->draw();
        buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
    }
    if (this->upgradeMode && upgTab == 1) {
        (*this->buttons)[2]->setVisible(true);
        (*this->buttons)[10]->setPosition(...);
        (*this->buttons)[10]->setVisible(true);
        (*this->buttons)[10]->draw();
    }
}
```

| metric | before (wave-175 base) | after |
|---|---:|---:|
| raw same-address diff | 2,242 | **1,965** |
| instruction-index alignment | 621 | **291** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,735 / 3,734 | **3,734 / 3,734** |
| its `.ARM.extab` record diff | 91 | **676** |
| LSDA cst_len | 927 / 927 | **926 / 927** |
| row 39 (orig / ours) | 150 / 152 | **150 / 118** |
| `.text` section diff | 80,166 | **79,889** |
| `.ARM.extab` section diff | 746 | **1,331** |
| total differing bytes | 80,932 | **81,240** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.rel.plt` byte-identical (0), `.ARM.exidx` 0,
`.rodata`/`.data`/`.bss` 0. File size exact.

The raw win is 277 bytes; the `.ARM.extab` record diff rises 585 bytes
because the row-39 LSDA length changes from 150 (two ULEB bytes) to 118
(one ULEB byte), which shifts the whole call-site table by one byte.  The
raw residual is the primary metric for this function; the row-39 record
shape is the remaining target (see below).

## Why the original order wins raw bytes

The wave-175 base used the nested guard

```cpp
if (__builtin_expect(tab != 0, 1)) {
    if (__builtin_expect(!(tab == 3 && getShip() == selectedItem->ship), 1)) {
        int upgTab = tab;
        BODY
        if (__builtin_expect(upgTab == 1, 0) && upgradeMode) UPG
    }
}
```

which makes clang lay the non-ship `switch` block out *before* the ship
body.  The original lays the ship body out first (`0x149b1e..0x149bec`)
and the non-ship switch at `0x149bf0`, so the base's 319-byte ship-branch
residual is removed.  The landed compound guard reproduces the original
sequence:

```
149b1a cmp  r0,#0                  ; isShip()
149b1c beq.n 149bf0                ; -> non-ship switch
149b1e ldr  r0,[sp,#108]           ; tab
149b20 ldrd r5,r4,[sp,#132]
149b24 cbz  r0,149b96              ; tab == 0 -> upgrade check
149b26 cmp  r0,#3
149b28 bne.n 149b44                ; tab != 3 -> body
149b2a ... getShip() ...
149b3a cmp  r4,r0
149b40 beq.w 14a6d0                ; same ship -> merge
149b44 body
149b94 ldr  r0,[sp,#108]           ; tab reload
149b96 cmp  r0,#1                  ; upgrade check
```

The `int upgTab = tab;` snapshot is the codegen anchor that makes clang
reload `tab` at the upgrade check (the original's `ldr r0,[sp,#108]` at
`0x149b94`) and keeps the function at exactly 9,700 B.

## The row-39 record tradeoff

The original's `getShip()` invoke unwinds to the `isShip` landing pad
(`9664`), so row 39 spans `isShip` + `getShip` (`0x149aa4..0x149b3a`,
length 150).  With the compound guard the `getShip()` invoke unwinds to
the buttons-body pad (`9512`), so row 39 ends after `isShip`
(`0x149aa4..0x149b1a`, length 118) and row 40 starts at `getShip`:

```
orig rows:  39 (2508,150,9664)  40 (2696,140,9562)  41..46 exact
ours rows:  39 (2508,118,9664)  40 (2658,178,9512)  41..46 exact
```

`150 > 127` needs two ULEB bytes, `118` fits in one, so the call-site
table shortens by one byte and every following row shifts.  That single
byte accounts for ~660 of the 676 record-diff bytes.

The only shape found that restores row 39 = 150 while keeping the
original block order is the compound guard with the `int upgTab`
declaration *inside* the body (the wave-175 `cin_snap` near-miss):

```cpp
if (tab != 0 && (tab != 3 || getShip() != selectedItem->ship)) {
    int upgTab = tab;
    BODY
    if (upgTab == 1 && upgradeMode) UPG
}
```

It gives row 39 = 150, cst 927/927, record diff 253, but the `tab == 0`
edge is `cmp r0,#0; beq.w <merge>` (6 B) instead of the original's
`cbz r0,149b96` (2 B), making the function 9,704 B — gate-fatal.  Every
attempt to shave those 4 bytes (goto/label, `do/while(0)`, explicit
`switch`, `__builtin_expect` on the tab check, declaration before/after
the guard) either keeps the 6-byte edge or changes the size/record
elsewhere.  With `__builtin_expect(tab != 0, 1)` the size returns to
9,700 B and the record to 93, but clang flips the block layout back to
the wave-175 base order and raw rises to 2,266 (no raw win).

Shapes measured in this pass (all size 9,700 unless noted):

| shape | raw | aligned | size | insn | rec |
|---|---:|---:|---:|---:|---:|
| wave-175 base (nested guard) | 2,242 | 621 | 9,700 | 3735 | 91 |
| **landed compound guard, flip + `upgradeMode` first** | **1,965** | **291** | **9,700** | **3734** | **676** |
| compound guard, plain operands | 1,990 | 306 | 9,700 | 3734 | 676 |
| nested outer guard (`if (tab != 0) { if (...) }`) | 1,990 | 306 | 9,700 | 3734 | 676 |
| `goto`/`switch`/`do-while` variants | 1,965-2,031 | 291-307 | 9,700 | 3734 | 676 |
| compound guard, declaration inside body (`cin_snap`) | 6,506 | 583 | **9,704** | 3737 | 253 |
| `cin_snap` + `expect(tab!=0,1)` + `expect(upg==1,0)` | 2,266 | 386 | 9,700 | 3734 | 93 |
| base + `expect` grid (best) | 2,242 | 621 | 9,700 | 3735 | 91 |

The operand flip (`Globals::status->getShip() != this->selectedItem->ship`)
and `this->upgradeMode && upgTab == 1` are the best raw ordering
(1,965 vs 1,990); both are semantically identical to the original.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
cmake --build cmake-build-match --target check -j2
```

The shape generators used for this sweep live in
`/home/debian/GalaxyOnFire2/scratch/w12014/` (`harness.py`, `gen.py`,
`sweep_*.py`); they rewrite the ship-branch block of
`src/game/ui/HangarWindow.cpp` from `scratch/w12014/HangarWindow.base.cpp`.
