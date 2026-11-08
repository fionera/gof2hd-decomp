# w10913 (wave 163): `Level::createMission` — 4-byte stack-slot
# permutation, frame map + weight analysis (no size-exact flip found;
# raw 5,454 / aligned 786 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10913`, base `e51db2fc7`
(wave 162).  Continues `wave162-merge-tests.md`,
`level-createmission-wave162-w10812.md`,
`level-createmission-wave162-w10813.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).
Raw metric `/dev/shm/w10913/fdiff.py`, aligned metric
`/dev/shm/w10913/align_diff.py` (worktree-local copies).

## Baseline (re-measured, pristine HEAD)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5454   align 786
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Frame map (final `-mllvm -print-machineinstrs`, frame 496 B)

`[SP-N]` is relative to the frame top; `sp+N` is relative to the post
`sub sp,#400` stack pointer.  The guard-address spill is `%stack.73`
(pre-coloring) and the raid bool is `%stack.85`.

```text
fi#0   4  sp+396   __stack_chk_guard canary value (%stack.0)
fi#1   12 sp+260   Vector playerPos
fi#3   12 sp+248   RaidCoords3 raidCoords
fi#5   12 sp+384   (align 8)
fi#6   12 sp+372
fi#7   60 sp+312   (align 8)
fi#71  36 sp+272   (align 8)
fi#73..fi#100  4-byte spill slots, laid out sp+124 down to sp+16
   fi#73 = sp+124 ... fi#82 = sp+88, fi#83 = sp+84 ... fi#100 = sp+16
fi#148..fi#163  callee-saved save areas (sp+400..496)
```

SSC mapping (from the pre-/post-coloring dumps, `mapssc.py`):

```text
              pre-slot   ours colour   original colour
guard addr    %stack.73   83 (sp+84)   82 (sp+88)
raid bool     %stack.85   82 (sp+88)   83 (sp+84)
```

Original disassembly confirms: `b0024 str.w fp,[sp,#88]` (guard) and
`b04c0/b04d8 str r0/r1,[sp,#84]` (raid).  Ours is the exact mirror.
The permutation touches 19 instructions (2 guard refs + 17 raid refs).

## 2. Root cause: StackSlotColoring weight order (quantified)

`StackSlotColoring::ScanForSpillSlotRefs` sets
`li.weight += LiveIntervals::getSpillWeight(false, true, MBFI, MI)` =
the sum of the block frequencies of every instruction with an FI operand.
`stable_sort` by weight desc; equal weights keep the lower slot index
first (guard 73 < raid 85), so a tie would already give the original
order.  Our weights are *not* tied:

```text
guard  = freq(bb.1) + freq(bb.520)          = 0.6250 + 0.5688 = 1.1938
raid   = sum of 19 block freqs (bb.24..bb.259)                = 1.1980
gap    = 0.0042  (0.35%)
```

(`freq2.py`: CFG + successor probabilities from the pre-SSC
`-print-machineinstrs` dump, linear-system solution.  w10812's
`color_sim.py` got the same ordering with a coarser model: 1.1811 vs
1.3078.)  The guard store is in `bb.1` (the `mission != nullptr` block,
freq 0.625), the guard reload in `bb.520` (the shared normal-return
block, 0.5688).  The 19 raid refs are spread over the mission-setup
blocks, summing to 1.1980.

So a *source* fix must raise the guard weight by >0.0042 (or lower the
raid weight) **without changing the code**, i.e. by perturbing the
branch probabilities (the original's PGO weights).  Frame/alignment
changes cannot do it: they only shift the whole spill region.

## 3. New levers measured this wave

All size 17,172 B unless noted.  `guard` is the `str.w fp,[sp,#N]`
offset; `sp84/sp88` are the reference counts.  Target is
`guard=88, sp84=17, sp88=2`.

| candidate | size | guard | sp84 | sp88 | flip? |
|---|---:|---:|---:|---:|---|
| HEAD (baseline) | 17172 | 84 | 2 | 17 | no |
| `char buf[4]; buf[0]=0;` early | 17172 | 84 | 2 | 17 | no (dead store removed) |
| `char buf[4]` before `getMission` | 17172 | 84 | 2 | 17 | no |
| `long long aligner; if (aligner) return;` | 17172 | 84 | 2 | 17 | no |
| `double aligner=0; if (aligner!=0) return;` | 17172 | 84 | 2 | 17 | no |
| `struct { long long x; } aligner={0};` | 17172 | 84 | 2 | 17 | no |
| `int raid` / `char raid` | 17172 | 84 | 2 | 17 | no |
| `double raiders` | 17172 | 84 | 2 | 17 | no |
| `long long raidRoll` | 17172 | 84 | 2 | 17 | no |
| `volatile char buf[4]; (void)buf[0];` | **17180** | 88 | 4 | 2 | frame +8; raid at sp+92 |
| `volatile char buf[4]; buf[0]=0;` | **17188** | 88 | 4 | 2 | frame +8; raid at sp+92 |
| `volatile int v; (void)v;` | **17180** | 88 | 4 | 2 | frame +8 |
| `__builtin_expect(inBlackMarketSystem(),0)` | **17180** | **88** | **17** | **2** | **YES, +8** |
| `__builtin_expect(inPirateLootOrbit(),0)` | **17180** | **88** | **17** | **2** | **YES, +8** |
| `__builtin_expect(getStation()->getIndex()==108,0)` | **17212** | 88 | 17 | 2 | YES, +40 |
| `__builtin_expect(inBlackMarketSystem(),1)` | 17180 | 32 | 4 | 2 | RA changed |
| `__builtin_expect(inSupernovaSystem(),0)` | 17172 | 84 | 2 | 17 | no |
| `__builtin_expect(mission->isEmpty(),0)` | 17232 | 124 | 6 | 15 | no |
| `__builtin_expect(mission->isEmpty(),1)` | 17356 | 96 | 4 | 6 | no |
| `__builtin_expect(mission==nullptr,0)` | 17192 | 120 | 4 | 2 | RA changed |
| `__builtin_expect(nameLeader,0/1)` | 17172 | 84 | 2 | 17 | no |
| `__builtin_expect(convoy,0)` | 17224 | 84 | 2 | 17 | no |
| `__builtin_expect(blueprint,0)` | 17168 | 84 | 2 | 17 | no |

Full single-line `if` sweep (`sweep.py`, every condition in the
function, both polarities): exactly three flips — `inBlackMarketSystem`,
`inPirateLootOrbit`, `getStation()->getIndex()==108` — all size-odd.
No size-exact flip exists among them.

### Why the black-market/pirate-loot branches flip it

Both branches sit *after* the last raid reference (`bb.259`), so making
them unlikely raises the fall-through frequency into `bb.520` (the guard
reload block) **without** changing any raid-ref block frequency.  That
is the only source-reachable way to move the guard/raid weight ratio.
The cost is a register-allocation change (`movs r6,#0` ->
`mov.w r8,#0`, `movlt r6,#1` -> `movlt.w r8,#1`), +8 B.

### Why the frame/alignment levers do not work

`volatile char buf[4]` (or any extra local that survives) grows the
frame by 8 B and shifts every stack offset uniformly; the guard goes to
sp+88 but the raid goes to sp+92, so the *relative* order is unchanged.
The spill slots `fi#73..fi#100` are laid out monotonically (higher
colour = lower address), so only the SSC colour order can swap them.

## 4. Conclusion / open questions

1. The permutation is a `StackSlotColoring` weight-order artefact with a
   gap of only 0.0042.  The original's PGO branch weights must put the
   guard at or above the raid.
2. The only size-exact source lever is a branch-weight perturbation
   *after* the last raid ref; the two branches that work both cost +8 B
   through RA.  A size-exact flip needs a branch whose weight change
   flips the SSC order without moving `r6` -> `r8`.
3. Next levers: (a) a late branch with a *milder* weight change (e.g.
   restructure `if (nameLeader) {...} else return;` or the switch tail
   so the fall-through into `bb.520` increases slightly); (b) a
   post-RA weight lever that adds a third guard-slot reference without
   adding code; (c) reproduce the original's PGO branch weights.
4. Baseline kept (best size-exact variant).

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5454 -> 5454
                           align  786 -> 786
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
RATCHET PASS
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10913/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10913/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10913/{fdiff.py,align_diff.py,cc.sh,measure.sh,
freq2.py,mapssc.py,sweep.py,tryvar.py,t1..t8.py,pmi.txt,ssc.cpp,li.cpp,
bfi.cpp,bfi.h}`.
