# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 182 / w12615

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave182-w12615`, base master `57e085059` (wave-181 harvest 5).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

This pass targets the branch layout / EH-LSDA / mission-type-switch area.
Nothing from wave 181 is re-landed; all edits are on top of the wave-181
tree (raw 3647, `.text` 75,365, `.ARM.extab` 655).

## 1. Result

**One size-exact code win landed: raid-loop `setRotate`/`setSpeed` order.**

```text
                                     size            raw    aligned  insn
base 57e085059                       17172/17172     3647   956      6206/6197
landed (this pass)                   17172/17172     3646   951      6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 75,365 -> 75,364  (-1)
.ARM.extab   size   156,312 /   156,312   differing bytes    655 ->    655  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (cmp clean)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20
```

Net total-byte win **-1** (`.text` only).  `.text` and `.ARM.extab` stay
size- and base-exact (`.ARM.extab` @ `0x1a3b78`, 156,312 B).  The function is
still one of the 17 non-byte-exact bodies, so the ratcheted `byte_exact` count
is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edit

### 2a. Raid loop `inPirateLootOrbit` body (`~line 2012`)

```cpp
-                (*this->enemies)[i]->setSpeed(3.5f);
                 ((PlayerFighter *) (*this->enemies)[i])->setRotate(3);
+                (*this->enemies)[i]->setSpeed(3.5f);
                 Player *p = (*this->enemies)[i]->player;
                 p->setHitpoints(p->getMaxHitpoints() * 2);
```

The three calls touch independent state on the same ship, so the source order
is free.  Exhaustive 6-permutation sweep of `{setSpeed, setRotate, hitpoints}`
(all size-exact 17172):

```text
perm   raw  aligned
RSP    3646   951   <- landed
SRP    3647   956   (wave-181 base)
RPS    3654   950
SPR    3654   955
PSR    3654   941
PRS    3654   950
```

`RSP` is the unique raw minimum and also improves the instruction-alignment
metric by 5.  (The original emits `hitpoints` first, then `setSpeed`, then
`setRotate`; that source order `PSR` measures 3654, so the emitted schedule is
not simply the source order here.)

## 3. Mission-type switch / branch-layout findings (no further win)

The task's suggested angles were investigated exhaustively; none produced a
size-exact raw improvement over 3646, so only the §2 edit is landed.

### 3a. Switch dispatch and TBH table

`switch (mission->getType())` lowers to `subs r1,r0,#1; cmp r1,#14;
bhi .L183_184; tbh [pc,r1,lsl#1]` plus a `cmp #184 / cmp #183` chain for
types > 15.  The 15-entry TBH table at `0xb02ac` is **identical except for
three entries** (measured by decoding the halfword table):

```text
type   original   ours     delta
 1     0xb14b2    0xb14b6   +4
 2     0xb16d4    0xb16d6   +2
 4     0xb19c0    0xb19be   -2
```

All other case-body entry addresses are identical.  The deltas are
downstream of the 4 B code-size difference at the end of the type-15 body
(the `new Mission(4, *gameText->getText(1601), ...)` String-copy block and
the four `enemies[0..3]->setPosition` calls): the original's case-1/case-2
blocks are laid out `case1, case2`, ours `case2, case1`, and the original
shares the third `offset[2]` store of the `switch (k)` convoy-offset switch
at a common tail.  The compiler's block placement is not source-order
sensitive (verified by swapping the `case 1`/`case 2` source blocks: binary
unchanged).

### 3b. `switch (k)` convoy-offset store orders

2-D sweep of the `case 1` x `case 2` store permutations with `case0`/`default`
fixed at the wave-181 `201` order (36 builds, all measured with `link_so`):

```text
best size-exact results (raw, case1, case2):
  3647  (1,2,0)  (0,1,2)   <- wave-181 base order
  3647  (2,1,0)  (0,1,2)
  3648  (1,0,2)  (2,1,0)
  3649  (0,1,2)  (2,1,0)
```

All-`(1,0,2)` (the original's emitted per-case order) makes the compiler
collapse the switch to a table lookup and is **8 B short** (17164), so it is
not size-exact.  No permutation beats 3647.

### 3c. `raid` boolean block order

The first structural difference in the function is at `0xb04c0`: the
`raid = false` cold block and the `security == 0 -> 90` block are emitted in
the opposite order (original: `raid=false` first; ours: `security==0` first).
This is a 2-instruction block swap of equal size, so it does not shift the
rest of the function.  Every source spelling tried
(`if/else`, inverted condition, `bool raid=false; if (...)`, `&&`, ternary,
`switch (security)`, separate `raidThreshold` local) either canonicalises to
the same binary or changes the function size (17180/17188/17204), so it was
not landed.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12615
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3646 aligned=951 size=17172/17172`, `.text` diff 75,364,
`.ARM.extab` diff 655, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12615/`
(`measure.sh`, `diffmap.py`, `dis.py`, `insndiff.py`, `seqalign.py`,
`sweep12.py`, `sweep_raid.py`, `Level.cpp.base`).
