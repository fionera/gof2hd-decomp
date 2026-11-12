# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13530b

Second pass of worker w13530 on `src/game/world/Level.cpp`, branch
`wave191b-w13530`, base `78cb0be98` (the w13530 crate/cargo store-order win).
Target function VMA `0xaffd8`, size **17,172 B**. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a).

```text
                                     size            raw    aligned  insn
base 78cb0be98 (committed)           17172/17172     2657    871     6206/6197
this pass (no new win)               17172/17172     2657    871     6206/6197
```

No new size-exact raw win was found; the tree is left at `78cb0be98`.
This note records the mechanism behind the two biggest residuals so the next
worker does not have to re-derive them.

## 1. Case-1 `enemyRace` reload is a **post-RA MachineLICM** hoist

The original reloads `enemyRace` at the top of the case-1 attacker loop
(`0xb1608  ldr r5,[sp,#116]`); ours hoists the same load into the loop
preheader (`0xb158c`) and then needs no `nop` before the literal pool, so the
whole case-1 tail (and the case-2 prologue after it) is shifted by **-2 B**.

This was previously assumed to be a register-allocator spill-placement
decision.  It is not.  Dumping MIR with
`clang++ <compile_commands command> -mllvm -print-machineinstrs`
(378 MB, keep it out of `/tmp`) shows the load moving between passes:

| pass | block of `ldr r5, [sp,#116]` |
|---|---|
| After Greedy Register Allocator | `bb.376` (loop body) |
| After Virtual Register Rewriter | `bb.376` |
| After Stack Slot Coloring | `bb.376` |
| After Machine Copy Propagation Pass | `bb.376` |
| **After Machine Loop Invariant Code Motion** | **`bb.371` (preheader)** |
| After PostRA Machine Sink | `bb.371` (not sunk back) |

So the hoist is `MachineLICM::HoistRegionPostRA()` (LLVM 7
`lib/CodeGen/MachineLICM.cpp`).  Its candidate filter is:

```cpp
if (Candidate.FI != INT_MIN && StoredFIs.count(Candidate.FI)) continue;
if (!PhysRegClobbers.test(Def) && !TermRegs.test(Def)) {
  for (used reg MO of MI)
    if (PhysRegDefs.test(Reg) || PhysRegClobbers.test(Reg)) { Safe = false; }
  if (Safe) HoistPostRA(MI, Def);
}
```

i.e. the load is *not* hoisted only when the frame index is stored somewhere
in the loop, the def physreg (`r5`) is defined twice in the loop, the
preheader terminator reads `r5`, or a register used by the load is defined in
the loop.  In our MIR the loop body `bb.376` has `liveins: $r4, $r6, $r8, $r9,
$r10` (no `$r5`, no `$sp`), so every check passes and the load is hoisted.
The original must have failed one of them.  Since the original's loop body
also defines `r5` exactly once (the load itself) and the preheader terminator
is a plain `b.n`, the most plausible difference is that the original's
`enemyRace` frame object is **not a spill-slot object**
(`MFI->isSpillSlotObjectIndex(FI)` false), which would make
`isLoadFromStackSlot` fail and the load a non-candidate.  Nothing tried so far
reproduces that without changing the frame size.

### Sweep (all size 17,172, all raw 2,657 — neutral)

`int race = enemyRace;` in the body; `const int &race`; `const int *race =
&enemyRace` + `*race`; `KIPlayer *ship = createShip(...)`; `int race` placed
after `attackerCount`, after `new Array`, after `ArraySetLength`; `while`
loop; `++i`; `attackerCount > i`; `setAlwaysEnemy(1)`; `int i` hoisted out of
the `for`; `race = enemyRace` at loop top; `race = enemyRace` at loop bottom;
`struct { int race; }` holder + reference; `int raceBox[2]` + reference.

`(bigAlienWave + (i < 0) + (i < 0) + 0)` is also neutral (2,657) but removing
either `(i < 0)` term changes the function to **17,168 B** (raw 4,101/4,408),
i.e. those terms are load-bearing for size even though they are semantically
dead (`i >= 0`).

`volatile int enemyRace` forces the reload but grows the frame to **17,232 B**
(raw 16,401) — still fatal.

## 2. Case-183 `offset` switch: all 24 case permutations measured

Current source order is `case 0, case 1, default, case 2`; clang emits the
blocks as `case 0, case 2, case 1, default`, so the literal pool order is
`25000,-36000, -7000,10000,-30000, 6000,12000,-45000` instead of the
original's `-7000,10000,-30000, 25000,-36000, 6000,12000,-45000` (38 B in
`0xb1418..0xb1460`).  All 24 permutations of the four case bodies were built
(each with an explicit `break`):

| permutation | raw |
|---|---:|
| `01d2` (current) | **2,657** |
| `0d12` | 2,657 |
| `012d`, `021d`, `02d1`, `0d21` | 2,658 |
| any permutation starting with `2` | 5,369 |

The block order is a MachineBlockPlacement choice; source order alone does not
control it.

## 3. Alien-gun block: `setRotation` is interleaved in the binary

Original `0xb32f6..0xb3328`:

```
blx MatrixRotateVector      ; v = MatrixRotateVector(rotation, {t[k][3..5]})
blx Vector::operator=       ; v = tmp
ldr r0,[gun,#8]             ; gun->geometry
blx AEGeometry::setRotation ; setRotation(v)   <-- uses the FIRST rotation
... build {t[k][0..2]} ...
blx MatrixRotateVector      ; v = MatrixRotateVector(rotation, {t[k][0..2]})
blx Vector::operator=
```

Our source rotates twice and only then calls `setRotation`, so it passes the
**second** rotation.  Writing the source in the binary's order

```cpp
v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
gun->geometry->setRotation(v);
v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
```

reproduces the instruction order but is size-exact at raw **2,667** / aligned
867 (the block is then shifted by the `this`-in-`r4`-vs-`r8` differences), so
it is not landable on its own.  It is a genuine semantic divergence from the
binary and should be revisited once the tail register allocation matches.

## 4. Tail: `this` in `r4` (binary) vs `r8` (ours)

From `0xb322e` (start of the `alienGuns` block) the binary keeps `this` in
`r4` and uses `sl` for `alienGuns`, while ours keeps `this` in `r8`, uses `r6`
for `alienGuns`, and additionally loads `Globals::rnd` into `r4` at `0xb3234`
(dead on the `alienGuns != 0` path, live on the `alienGuns == 0` path into the
`pirateFortress` block; the binary instead reloads `rnd` at `0xb35da` inside
the fortress loop).  Everything from `0xb322e` to the epilogue is structurally
identical but uses a different register set, which is where ~1.9 KB of the
2,657-byte raw residual lives:

| region | raw bytes |
|---|---:|
| `0x1400` (case-183 switch) | 38 |
| `0x1600` (case 1/2 boundary) | 328 |
| `0x1800` (case-2 prologue) | 59 |
| `0x2000` (case-10 Objective tail) | 6 |
| `0x2a00`, `0x2c00` | 18 |
| `0x3200`–`0x4200` (tail) | 2,207 |

Other measured (not landed) variants: `gun = (*this->enemies)[i]` after the
`createStaticObject` store (matches the binary's data flow) = 2,658/867;
plus the same in the fortress block = 2,664/864; case-2 `goods` moved after
the `Route` = 2,676/863; case-2 `escorts` after `new Array` = 17,196 B (size
fatal).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13530
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2657 aligned=871 size=17172/17172`, `.text` diff 71,249,
`.ARM.extab` diff 1,216, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13530/` (`exp.py`, `run.py`,
`cmp2.py`, `sbs.py`, `diffmap.py`, `v1.py`..`v12.py`, `mir.txt`).
