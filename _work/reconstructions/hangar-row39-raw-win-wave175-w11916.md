# HangarWindow::render — row-39/record vs raw, wave 175 (w11916)

Worktree `wt/w11916`, branch `wave175-w11916`, base master `5f599a137`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows.

Entry state (wave-174 landing, commit `49dd03200`):

| metric | value |
|---|---:|
| raw same-address diff | 2,242 |
| instruction-index alignment | 621 |
| function size | 9,700 / 9,700 |
| instructions | 3,735 / 3,734 |
| its `.ARM.extab` record diff | 91 |
| LSDA cst_len | 927 / 927 |
| LSDA rows | 156 / 156 |
| row 39 (orig / ours) | 150 / 152 |

`check` green on the base: byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.rel.plt` byte-identical.

## Result

**No shape was found with record diff <= 100 and raw < 2,242.**  `src/` is left
at the base; only this note is committed.  The best record-<=100 raw stays the
wave-174 operand flip (2,242 / 91), and the best raw overall stays the
`outer` family (1,950 / record 676).

The two requirements are mutually exclusive in every shape tried: the original
block order (ship body first, `tab == 0` edge to the near upgrade check) is
only reachable with `getShip()` in the *outer* cleanup scope, and every such
shape either loses the 2-byte `cbz` (9,704 B) or sinks `getShip()` into the
buttons-body landing pad (row 39 = 118, cst 926, record 676).

## Why the `outer` family scores record 676 (the cst shift)

The `outer` shape (ship body first, upgrade check after it) is size-exact
9,700 and raw 1,950-1,990, but its LSDA record differs by 676 bytes.  The
root cause is a single ULEB128 byte:

```
original  row39 = (2508, 150, 9664)   length 150 = 0x96 -> 2 ULEB bytes
outer     row39 = (2508, 118, 9664)   length 118 = 0x76 -> 1 ULEB byte
```

150 > 127 needs two bytes; 118 fits in one.  The missing byte shortens the
whole call-site table by one (`cst_len` 926 vs 927) and shifts every following
row by one byte, which alone accounts for ~660 of the 676 differing record
bytes.  Row 40 also changes shape (`(2654,182,9512)` vs `(2696,140,9562)`), but
rows 41..135 keep their original contents apart from the one-byte shift.

`getShip()` lands in a *different* landing pad in `outer`: the call-site table
puts it in the buttons-body pad (`%1765`, pad 9512) instead of the `isShip`
body pad (`%452`, pad 9664).  IR evidence:

```
outer_flip:   iship_unwind=452  getship_unwind=1765  DIFF
```

so row 39 stops right after `isShip` (0x149b1a) instead of after `getShip`
(0x149b3a).  The region is 118 bytes instead of 150.

## The near-miss: compound guard with the upgrade check inside

Putting the upgrade check *inside* the compound guard body

```cpp
if (tab != 0 && !(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
    int upgTab = tab;
    ... buttons[1] body ...
    if (upgTab == 1 && this->upgradeMode) { ... buttons[2]/[10] ... }
}
```

gives `getship_unwind=452` (same pad as `isShip`), row 39 = 150, cst 927/927
and **the original block order**:

```
orig  149b1a cmp  r0,#0           ours  149b1a cmp  r0,#0
      149b1c beq.n 149bf0               149b1c beq.n 149bf4
      149b1e ldr  r0,[sp,#108]          149b1e ldr  r0,[sp,#108]
      149b20 ldrd r5,r4,[sp,#132]       149b20 ldrd r5,r4,[sp,#132]
      149b24 cbz  r0,149b96   (2 B)     149b24 cmp  r0,#0
      149b26 cmp  r0,#3                 149b26 beq.w 14a6d4   (6 B)
      149b28 bne.n 149b44               149b2a cmp  r0,#3
      149b2a ldr  r0,[pc,#340]          149b2c bne.n 149b48
      149b2c ldr  r1,[r4,#104]          149b2e ldr  r0,[pc,#340]
      149b2e add  r0,pc                 149b30 ldr  r1,[r4,#104]
      149b30 ldr  r0,[r0,#0]            149b32 add  r0,pc
      149b32 ldr  r4,[r1,#12]           149b34 ldr  r0,[r0,#0]
      149b34 ldr  r0,[r0,#0]            149b36 ldr  r4,[r1,#12]
      149b36 blx  getShip               149b38 ldr  r0,[r0,#0]
      149b3a cmp  r4,r0                 149b3a blx  getShip
      149b3c ldrd r5,r4,[sp,#132]       149b3e cmp  r4,r0
      149b40 beq.w 14a6d0               149b40 ldrd r5,r4,[sp,#132]
      149b44 body                       149b44 beq.w 14a6d4
                                        149b48 body
```

Everything is byte-identical except the `tab == 0` edge: the original branches
to the *near* upgrade check (0x149b96, 2-byte `cbz`), ours branches to the far
merge (0x14a6d4, `cmp`+`beq.w`, 6 bytes).  The 4 extra bytes make the function
9,704 B, so the shape is gate-fatal.  Its record is 254 (cst 927/927, row 39 =
150) and raw 6,511 (the whole tail is shifted by 4).

Measured:

| shape | raw | aligned | size | insn | record | cst |
|---|---:|---:|---:|---:|---:|---|
| base (wave-174 landed) | **2,242** | 621 | 9,700 | 3735 | **91** | 927/927 |
| `outer` + `expect(tab!=0,1)` + `expect(upg==1,0)` | 1,950 | 290 | 9,700 | 3734 | 676 | 927/926 |
| `outer` + `expect(tab!=0,1)` | 1,954 | 294 | 9,700 | 3734 | 676 | 927/926 |
| `outer` (no expects) | 1,970 | 294 | 9,700 | 3734 | 676 | 927/926 |
| compound guard, upgrade inside, no expects | 6,511 | 611 | **9,704** | 3736 | 254 | 927/927 |
| compound guard, upgrade inside, operand flip | 6,506 | 583 | **9,704** | 3737 | 253 | 927/927 |
| compound guard, upgrade outside | 1,970 | 294 | 9,700 | 3734 | 676 | 927/926 |
| nested guard + `expect(tab!=0,1)` only | 2,266 | 386 | 9,700 | 3734 | 93 | 927/927 |
| `if (!isShip) { switch } else { ship }` (`swap`) | 2,362 | 631 | 9,700 | 3736 | 93 | 927/927 |
| wave-163 `cbz` shape (volatile + assumes) | 1,979 | 551 | 9,700 | 3736 | 676 | 927/926 |

`expect(item->isShip(), 1)` was also tried: it restores the original block
order but changes the whole function's register allocation (size 9,696 or
9,700 with a 29-instruction increase, raw 8,249-8,691, 158 rows), so it is not
a win.

## Where the remaining raw bytes are

For the base shape the 2,242 differing bytes are concentrated in three
regions:

| region | bytes | cause |
|---|---:|---|
| `0x1496d8..0x1498d7` | 63 | `this`/`item` swapped between `sl` and `r9` in the ship-drawing block |
| `0x149ad8..0x149cd7` | 321 | flipped ship-branch block order (guard + non-ship body placement) |
| `0x14a7d8..0x14aed7` | ~1,590 | tail register allocation (`r9` vs `r4` for `&this->buttons`, `Globals::options` spilled to `[sp,#124]` in the original vs kept in `r9` in ours) |
| `0x14b1d8..0x14b4d7` | 405 | EH landing-pad thunk table |
| `0x14b5d8..0x14b6d7` | 12 | LSDA thunk bytes |

The tail residual is independent of the ship-branch shape: `outer_flip`
(whose ship branch is byte-identical to the original apart from the 4-byte
`tab == 0` edge) shows the same ~1,590-byte tail diff.  Its instruction
sequence matches the original (verified with a mnemonic-normalised
`SequenceMatcher`); only the register/spill choices differ, so it is not
reachable from a source-level shape change.

## Shapes tried (all size-exact unless noted)

* nested guard, compound guard (`!(tab==3 && ...)` and `tab!=3 || ...`),
  `outer`, `mid`, `or`, `ie`, `cf`, `ho`, `nt`, `presame`, `sv`, `hn` families;
* `expect` values `{None,0,1,2,3}` on the outer guard and the inner guard,
  `expect` on the upgrade check, `expect` on `item->isShip()`;
* operand flips on the ship compare, the `tab != 0` test, the upgrade check
  and the `isShip` call;
* `goto`, `switch`, `do/while(0)`, block scope, `bool` temporaries, hoisted
  `Ship *myShip`, `int upgTab` anchors at every position inside/outside the
  guard;
* wave-163 volatile-read + `__builtin_assume` jump-threading-cost trick, alone
  and combined with the compound guard;
* dummy `getShip()` calls in the `isShip` body scope to lengthen row 39.

None produced record <= 100 with raw < 2,242 at 9,700 B.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
cmake --build cmake-build-match --target check -j2
```

The shape generators used for this sweep live in
`/home/debian/GalaxyOnFire2/scratch/w11916/` (`gen.py`, `gen9.py`-`gen27.py`,
`sweep*.py`, `rec.py`, `recdiff.py`, `seqall.py`); they rewrite the ship-branch
block of `src/game/ui/HangarWindow.cpp` from
`scratch/w11916/HangarWindow.base.cpp`.
