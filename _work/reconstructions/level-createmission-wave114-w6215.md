# w6215: `Level::createMission` (92.145215 %, 17172 B) — wave-114:
# station-loop `orrs` / convoy chain / loop-shape sweep; no landable gain

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6215`, branch `wave114-w6215`,
base `d8557f7e2`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; addresses below are linked `.so` addresses, function
`0xaffd8`–`0xb431c`).

**No `src/` change is committed**: every variant measured below is neutral or a
regression on the committed wave-113 baseline. The deliverable is this note.
The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.145215 %   size 17172 B (size-exact)
                             shape (regs collapsed) 96.69967 %
                             orig 6055 insns / ours 6065 insns
                             314 raw diff regions (157 shape-diff regions)
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
ELF shape unchanged by this pass: 27 sections exact, file 2,187,328 B,
.ARM.extab 0x26298 (exact), .rodata 0x41b67/0x41c27 (ours +0xC0, the known
Status rank-table tail).
```

Every row below is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier normalisation (unrounded).  `orrs O/U` is the
count of `orrs` instructions in the original / our disassembly (the original has
three: wanted `0xb2ab0`, alienWave `0xb2e14`, pirateFortress `0xb2f36`).

## 1. Station-loop merged predicate (`orrs`, source line 1904)

Ours emits the short-circuit form

```text
ldr  r0, [sp, #60]      ; alienWave
cmp  r0, #0
beq.w else
cmp  fp, r3             ; i == first
bne.w else
```

while the original materialises the negated `and`:

```text
subs.w r0, fp, r3       ; i - first
mov    r1, r0
it     ne
movne  r1, #1           ; (i != first)
ldr    r2, [sp, #60]    ; alienWave
eor.w  r2, r2, #1       ; !alienWave
orrs   r1, r2
bne.w  else
```

The optimised IR is `%1123 = icmp ne i32 %alienWave, 0` (defined *outside* the
loop) and `%1410 = icmp eq i32 %i, %first` (inside), combined as
`and i1 %1123, %1410` — leaf 0 is `SETNE`, so `ShouldEmitAsBranches` returns
true and the two-branch form is emitted.  Every spelling below leaves the
operand order and the `orrs` count unchanged:

| variant (first condition, line 1904) | pct | size | orrs O/U |
|---|---:|---:|---|
| **baseline `i == first && alienWave`** | **92.145215** | 17172 | 3/1 |
| `alienWave != 0` | 92.145215 | 17172 | 3/1 |
| `(bool)(alienWave != 0)` | 92.145215 | 17172 | 3/1 |
| `(alienWave ? true : false)` | 92.145215 | 17172 | 3/1 |
| `!!alienWave` | 92.145215 | 17172 | 3/1 |
| `i - first == 0 && alienWave` | 92.145215 | 17172 | 3/1 |
| `!(i - first != 0) && alienWave` | 92.145215 | 17172 | 3/1 |
| `alienWave && i == first` (swapped) | 92.145215 | 17172 | 3/1 |
| `pirateFortress && i == first` (2nd cond) | 92.145215 | 17172 | 3/1 |
| `pirateFortress == 1` (2nd cond) | 92.145215 | 17172 | 3/1 |
| `alienWave == 1` | 91.914191 | 17172 | 3/1 |
| `(alienWave & 1)` | 91.914191 | 17172 | 3/1 |
| `(alienWave > 0)` | 91.914191 | 17172 | 3/1 |
| `(alienWave == true)` | 91.914191 | 17172 | 3/1 |

The four 91.914191 rows all produce the same alternative codegen (a `cmp #1` on
the wave flag) and still no `orrs`.  A standalone micro-test
(`if (i == first && aw) return g(i);`) keeps the source order
(`and i1 %4 = icmp eq, %5 = icmp ne`) yet still emits the two-branch form
(`cmp; bne; cbz`), so leaf-0-`SETEQ` alone is not sufficient — the merged form
only appears in the original's large-block context.  This confirms the wave-112
conclusion: the only source-reachable `orrs` form is `bool alienWave`, which
costs ~3.8 pp elsewhere.

## 2. Convoy `k` chain (source line 2192)

The original tests descending (`cmp #2; beq; cmp #1; beq; cbnz`) with bodies
laid out `case0, case1, case2, default`.  Our source is the descending if-chain
`if (k == 2) … else if (k == 1) … else if (k == 0) … else …`, but clang 7.0.2
lowers it to the ascending test order `cmp #0; beq; cmp #1; beq; cmp #2; bne`.

| variant | pct | size |
|---|---:|---:|
| **baseline if-chain 2,1,0** | **92.145215** | 17172 |
| `switch (k)` cases 2,1,0, default last | 92.145215 | 17172 |
| `switch (k)` cases 0,1,2, default last | 92.013201 | 17172 |

`switch` and the if-chain are the *same* codegen (both ascending), so the
original's descending test order is not source-reachable from clang 7.0.2.
(The wave-112 note's `switch` figure of 91.945866 was measured on an older
baseline; on the wave-113 baseline the switch form is neutral.)

## 3. alienGuns / fortressGuns setup (lines 1856–1857, 1980)

All neutral (92.145215 / 17172):

```text
int alienGuns = 0; if (alienWave) alienGuns = bigAlienWave == 1 ? 8 : 7;
int fortressGuns = 0; if (pirateFortress) fortressGuns = 5;
int guns = fortressGuns + alienGuns;          (operand order swap)
for (...; k++, i++)                            (increment order swap)
Vector v(0.0f, 0.0f, 0.0f);                    (vs aggregate init)
```

## 4. alienGuns loop statement order (lines 1957–1973)

| variant | pct | size |
|---|---:|---:|
| **baseline** | **92.145215** | 17172 |
| `setScaling` moved to the very end (after `cargo`) | 92.079208 | 17172 |
| `shipGroup` before `setPosition` | 92.128713 | 17172 |
| `setMaxHitpoints` after `setRotation` | 92.046205 | 17172 |
| `setMaxHitpoints` deleted (control) | 84.691521 | 17196 |

This re-confirms wave-113: the tuned `setScaling`-after-first-`MatrixRotateVector`
position is a local optimum.

## 5. alienGuns vector construction (line 1964)

The original loads `t[k][3]` and `t[k][4]` separately
(`ldr r1,[r0,#12]` / `ldr r2,[r0,#16]`, stored to `sp+200` / `sp+204`
separately); ours merges them into `ldrd r1,r2,[r0,#12]` / `strd r1,r2,[sp,#200]`.
No source spelling tried prevents the merge:

| variant | pct | size |
|---|---:|---:|
| **baseline `Vector{t[k][3], t[k][4], t[k][5]}`** | **92.145215** | 17172 |
| `Vector(t[k][3], t[k][4], t[k][5])` ctor | 92.145215 | 17172 |
| `const float *row = t[k]; Vector{row[3], row[4], row[5]}` | 92.145215 | 17172 |
| flat pointer `const float *t` + `t[k*6+3]` | 92.145215 | 17172 |
| `float t3 = t[k][3], t4 = t[k][4], t5 = t[k][5]; Vector{t3,t4,t5}` | 92.145215 | 17172 |
| reverse component order `Vector{t[k][5], t[k][4], t[k][3]}` | 92.112211 | 17172 |
| `v.x = t[k][3]; v.y = t[k][4]; v.z = t[k][5]; v = MatrixRotateVector(rotation, v);` | 92.046205 | 17172 |
| `Vector tv = {…}; v = MatrixRotateVector(rotation, tv);` | 88.503755 | 17164 |
| `Vector tv; tv.x = …; tv.y = …; tv.z = …;` | 88.503755 | 17164 |

## 6. Structural facts established this pass

* **The swapped frame slots are `raid` and the `__stack_chk_guard` address.**
  `fp` is loaded at `0xaffe8`–`0xafff0` from the GOT entry `0x20fd04`
  (`R_ARM_GLOB_DAT __stack_chk_guard`), i.e. `fp = &__stack_chk_guard`; it is
  spilled with `str.w fp,[sp,#88]` (original) / `str.w fp,[sp,#84]` (ours) and
  reloaded once at `0xb02ca`.  The 17-use slot is `raid`: the `raid = raidRoll <
  (security == 0 ? 90 : …)` select stores it with `str r1,[sp,#84]` (original) /
  `str r1,[sp,#88]` (ours) at `0xb04d8`.  So the pair is simply swapped, and it
  is a `RegAllocGreedy` / stack-slot-coloring artefact (no declaration move tried
  flips it; `int raid` / `unsigned char raid` are 91.929361).  `Globals::status`
  is at `sp+108` and `this` at `sp+124` in **both** builds.  (The wave-112 note's
  "orig `raid` sp+84 / `status` sp+88" is half right: `raid` is at `sp+84`, but
  `sp+88` holds the stack-guard address, not `status`.)
* **alienGuns loop counter allocation.**  Original: `i` in `r4`, `k*24` in
  `r6`.  Ours: `i` in `r6`, `k*24` in `sl`.  Pure register assignment; the
  bound arithmetic (`alienGuns + friendCount + field_164 + field_168`) is
  identical.
* **`*bigShipsCount > 0` test (line 1837).**  Original: `cmp r0,#0; ble`
  (signed `x <= 0`); ours: `cmp r0,#1; blt` (`x < 1`).  The original also tests
  `localRace == 0` *first* (`cmp.w r9,#0; bne` at `0xb0e9c`/`0xb0eaa`) and then
  `*bigShipsCount` (`cmp r0,#0; ble` at `0xb0eac`/`0xb0eae`); writing
  `localRace == 0 && *bigShipsCount > 0` in our source merges the two into a
  single `ands` and costs ~18 pp (74.03 %, wave-112).
* **Convoy body layout.**  Our ascending test order also re-lays the bodies
  `case2, case0, case1, default`; the original is `case0, case1, case2,
  default`.  Not source-reachable (see §2).
* **`pirateEscorts` slot.**  Original `sp+48`, ours `sp+44` (also regalloc).

## 7. Conclusion

The residual is dominated by `RegAllocGreedy` register / spill-slot assignment
in a 6 k-instruction function.  The two source-reachable shape clusters
(station-loop `orrs` and the convoy descending chain) are blocked by clang
7.0.2's `InstCombine` operand ordering and switch lowering respectively, and
every spelling tried for them is neutral or worse.  No variant beat the
committed baseline's **92.145215 % / 17172 B**.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6215/score2.py _ZN5Level13createMissionEv
```

Scratch tooling (not committed): `/dev/shm/w6215/score2.py` (official verifier
normalisation, unrounded), `shapescore.py` (register-collapsed score),
`diffmap2.py` (diff regions + per-0x200 histogram), `showreg.py` (side-by-side
region dump), `countorrs.py` (`orrs` count), `tryvar.py`/`b*.py` (variant
apply/link/score harness), `mt/` (clang 7.0.2 micro-tests).
