# MGame::OnTouchEnd — wave 149 (w9510): the sepif +4 is the literal-pool nop, and the `isDockingToStream` branch-weight companion

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9510`,
base `fece77a64` (wave 148 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9510`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 148) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4127 | **4393** |
| aligned (align_diff) | 906 | **904** |
| `.text` differing bytes | 86,225 | **86,491** |
| wingman tail | `b.n 17b9fc` (T) | **`b.w 17aeb2` (join test)** |

## 1. What the +4 actually is

`sepif` (the faithful CFG: the wingman body branches to the join test J, not the epilogue T)
is 8084 B.  The +4 is exactly two 2-byte items at the end of the wingman body:

```
base  17bf4a: e557       b.n 17b9fc     (2)
sepif 17bf4a: f7fe bfb2  b.w 17aeb2     (4)   +2
sepif 17bf66: bf00       nop            (2)   +2  (literal pool must be 4-aligned)
```

The original has the same `b.w` + `nop` pair (`0x17bf4a b.w 17afe2`, `0x17bf62 nop`,
pool at `0x17bf64`).  So the original is **not** smaller in the tail; it is 4 bytes
smaller *elsewhere before the pool*.  The block-level diff localises that saving:

* original `0x17bf56..0x17bf60`: `ldr.w r2,[fp,#128]; ldr.w r0,[fp,#88]; ldr r1,[r2,#36];
  b.n 17bcf4` — the `lockedStation` `dockToAsteroid` call is a **tail branch** into the
  shared call block owned by the `isDockingToAsteroid` site (`0x17bcf4 blx dockToAsteroid`).
* ours (base *and* sepif) `0x17bf54..0x17bf62`: the same three loads then
  `blx dockToAsteroid; b.n 17bae0` — the call is inline here (4 B) plus the branch (2 B),
  i.e. +4 vs the original.  The `isDockingToAsteroid` site instead branches *forward*
  to this block (`b.n 17bf5e`), so the merge direction is reversed.

So the genuine 4-byte companion is the `dockToAsteroid` tail-merge ownership, not the
mining counter.  Fixing it keeps the literal pool at the original `0x17bf64` and stops
the post-pool tail from shifting.

## 2. The 0x17a5c4 `count > 0` residual (the wave-148 candidate)

Original `0x17a5ca`: `cmp.w r9,#0; beq.n` (`amount == 0`).  Ours: `cmp.w r9,#-1; bgt.n`
(`amount >= 0`; clang proves `count = -amount >= 0`).  Every `!= 0` spelling produces the
original `cmp #0; beq` but flips the decrementing induction variable to an incrementing
one and re-rolls register allocation (r9<->sl swap) over the whole function:

| variant (with sepif) | size | raw | aligned |
|---|---:|---:|---:|
| `count > 0` (base) | 8084 | 4407 | 916 |
| `count != 0` / `amount != 0` / `if (count)` / `if (amount)` / `goto` / `do{}while(0)` / ternary | **8060** | 6579 | 1842 |
| `__builtin_expect(count != 0, 1)` | 8004 | 7456 | 2233 |
| `__builtin_expect(count != 0, 0)` | **8080** | 7296 | 2389 |

The IR is identical (`icmp eq i32 %amount, 0; br eq -> skip`); the flip happens in
codegen.  `expect(...,0)` recovers 8080 but the layout is far worse, so the count fix is
left out of this landing.

## 3. The landed companion: `__builtin_expect(isDockingToStream(), 1)`

The `hudTouchFlags & 0x40` dock chain has two `dockToAsteroid` sites that LLVM tail-merges.
The merge ownership is a MachineBlockPlacement/branch-weight decision.  A branch-weight
hint on the `isDockingToStream` test flips it enough to absorb the 4 bytes while keeping
the literal pool at `0x17bf64`:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base (`else if` chain) | 8080 | **4127** | 906 |
| `sepif` | 8084 | 4407 | 916 |
| `sepif` + `expect(isDockingToStream(),1)` (**landed**) | **8080** | **4393** | **904** |
| `sepif` + `expect(isDockingToStream(),0)` | 8084 | 4674 | 942 |
| `sepif` + `expect(isDockingToAsteroid(),0/1)` | 8080 | 4607/4606 | 923 |
| `sepif` + `expect(isDockingToDockingPoint(),0)` | 8084 | 4684 | 927 |
| `sepif` + `expect(isDockingToDockingPoint(),1)` | 8084 | 4671 | 907 |
| `sepif` + `expect(isDockingToStream(),1)` + swap dock branches | 8080 | 4393 | 904 |
| `sepif` + `expect(isDockingToStream(),1)` + `if (failed)` | 8076 | 4713 | 1969 |
| `sepif` + `expect(isDockingToStream(),1)` + `expect(isDockingToAsteroid(),1)` | 8080 | 4606 | 923 |

`expect(isDockingToStream(),1)` is the only knob that lands exactly 8080 with the faithful
`b.w` tail and does not move the literal pool.  The aligned count improves 906 -> 904;
raw rises 4127 -> 4393 (+266) because the `isDockingToStream` block itself moves
(`0x17bf4e` -> `0x17bf32` and the `hudEvent(6)` tail is inlined), which is the price of
the merge-ownership flip.  This is 27 raw better than the w9412 landed variant (4420/925).

## 4. Honest assessment

The landed change is structurally faithful (wingman body -> join test J) and size-exact,
and the aligned metric improves by 2.  Raw regresses by 266 from the base because the
block-placement flip relocates the `isDockingToStream` block; the literal pool and the
whole post-pool tail stay at the original addresses, so the regression is confined to the
pre-pool cold cluster.  The `count > 0` residual is confirmed but is not landable at 8080
without a codegen-level induction-variable flip.

## 5. Reproduction

```
cp /dev/shm/w9510/MGame.sepif.expstream1.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9510/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4393 / 8080
python3 /dev/shm/w9510/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 904
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9510/{fdiff.py,align_diff.py,cmp2.py,blocks2.py,blocksize.py,
widthdiff.py,sizedelta.py,measure.sh,ir.sh,MGame.*.cpp,base.txt,sepif.txt,countne.txt,
expstream1.txt,orig.txt}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
