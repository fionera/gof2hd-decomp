# w10712 (wave 161): `Level::createMission` — alien-loop `this=r4` / `i=fp`
# register-map sweep; no source win landed (raw 5,461 / aligned 781 / 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10712`, based on master
`01c7f92d6` (wave 160). Continues
`level-createmission-wave160-w10612.md` (hoisted `end` + `&`-guarded tail) and
`…-wave160-w10613.md` (case-183 store order), and
`wave160-merge-tests.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B).  Ghidra
MCP unavailable in this session; NDK `objdump` is the authority.  Raw same-address
diff: `/dev/shm/w10712/fdiff.py`; instruction-index alignment:
`/dev/shm/w10712/align_diff.py`; register map: `/dev/shm/w10712/regmap.py`
(worktree-local copies, `OURS` pointed at this worktree's
`cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5461   align 781
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,518   .ARM.extab 1,258   .rel.plt 0 (byte-identical)
```

The landed source is w10612's `(i < end) & (i < recomputed)` tail (see
`level-createmission-wave160-w10612.md`).

## 1. Target register map (original, `0xb329a`–`0xb3394`)

```text
        this        i     gun   k*24   t     v(addr)  rotation   temp   alienGuns
ORIG    r4(reload)  fp    r6    r9     sl    r5       r4(reused) r8     sl(reload)
```

`this` is loaded into `r4` at `0xb3232` (before the `cmp alienGuns,#0`), used at
the body head `0xb329a` (`mov r0,r4`) and the two `enemies[i]` loads
(`0xb32a8`, `0xb32b6`), then `r4` is clobbered by `add r4,sp,#312` (rotation) at
`0xb32da`; the tail reloads `this` into `r4` at `0xb336e` for the bound
recompute (`add r2,r4,#352`).  `i` is `fp` (r11); `k` is a byte offset in `r9`
(`add r9,r9,#24`); `t` is `sl`; `v` is `r5`; `gun` is `r6`; `t[k][1]` is `r8`;
`alienGuns` is reloaded from `sp+52` into `sl` each iteration.

## 2. Measured shapes (all 17,172 B unless noted)

Baseline for all rows is pristine HEAD (`5461 / 781`).  `this/i` is the alien
loop map.

| variant | raw | align | size | this | i | k | t | v | rot | gun |
|---|---:|---:|---:|---|---|---|---|---|---|---|
| **HEAD (landed `(i<end)&(i<recomp)`)** | **5,461** | **781** | 17,172 | fp | r9* | r6 | r5 | r9 | sl | r4 |
| natural tail (`i < recompute`, no `end`) | 5,659 | 704 | 17,172 | fp | r9* | r6 | r5 | r6 | sl | r4 |
| pure hoist (`i < end`, no `&`) | 6,514 | 1,408 | 17,156 | r4 | sl | r6 | r5 | r9 | fp | r4 |
| **`__builtin_expect(alienGuns != 0, 1)` + natural tail** | 5,844 | 825 | 17,172 | r5 | **fp** | **r9** | r6 | r9 | sl | r4 |
| `__builtin_expect(alienGuns != 0, 1)` + landed `&` tail | 5,837 | 881 | 17,172 | r5 | r9* | sl | r6 | fp | sl | r4 |
| `__builtin_expect(alienGuns != 0, 1)` + pure hoist | 6,577 | 1,442 | 17,172 | r4 | r9 | r6 | r5 | sl | fp | r4 |
| natural tail, drop `(i<0)` table hack | 6,586 | 1,254 | 17,172 | r5 | r4* | r6 | — | sl | r8 | r4 |
| natural tail, `expect(i<bound, 0)` | 6,599 | 1,292 | 17,172 | — | r9 | r8 | — | — | — | r6 |

`*` = spilled (`i` at `sp+116`/`sp+120`).

### 2.1 The `(i < 0)` table-select hack is no longer size-load-bearing

The w10509 note reported that removing `(bigAlienWave + (i < 0))` shrinks the
function to 17,140/17,132 B.  On the current base (after w10612 + w10613) the
natural tail with plain `bigAlienWave` is **size-exact at 17,172 B**; the hack
is dead weight for size (it still changes the literal-pool/block layout and
therefore the raw metric: 5,659 → 6,586).  The original's table select is a
plain `cbz r0, ...` (`0xb32c8`), consistent with no hack.

### 2.2 `__builtin_expect(alienGuns != 0, 1)` moves `i` to `fp` and `k` to `r9`

This is the only lever found that reproduces two of the target's seven
callee-saved assignments.  It changes the block-frequency metadata of the
alien block, which changes the greedy allocator's spill weights.  It does NOT
move `this` to `r4`: `this` keeps a long live range from the loop preheader to
the tail and is assigned `r5`, while `gun` takes `r4`.  In the original `this`
is split into two short intervals (preheader+body head, and tail) because `r4`
is clobbered by `&rotation`; the allocator's choice of `sl` for `&rotation` in
our shape is what keeps `this` alive across the loop.

### 2.3 Sweeps that did not change the map

All measured size-exact unless noted:

* all 24 declaration orders of `{rotation, v, k, base}` — natural tail stays
  `this=fp/i=r9*`; with `expect_ag` stays `this=r5/i=fp`; pure hoist stays
  `this=r4/i=sl` (or `i=r9` for VKBR/KVBR/VBRK/VBKR).
* `Level *self = this;` / `Level &self = *this;` (body or whole block) — the
  compiler CSEs them; codegen byte-identical to the plain `this` shape.
* `Array<KIPlayer*> *enemies = this->enemies;` cache — natural 6,543/1,183,
  `this=r5/i=r9/k=fp`; with `expect_ag` 6,579/1,230, `this=sl/i=r9/k=fp`.
* `KIPlayer *const gun`, `auto gun`, `Vector p` local for the first rotate,
  `cargo`-before-`shipGroup`, `int i` outside the `for`, `++i`, `i += 1`,
  `i = i + 1`, `unsigned i`, `unsigned k`, `Vector v;`, `Matrix()`,
  `(void) base`, `(void) k` — all byte-identical or no map change.
* loop-condition rewrites (`bound > i`, `i - bound < 0`, unsigned compare,
  `i <= bound - 1`) — byte-identical codegen.
* `__builtin_expect` on the inner `bigAlienWave` ternary (`!=0` or `==1`) with
  `expect_ag` — map unchanged (`this=r5/i=fp`), raw 6,497–6,500.
* `__builtin_expect(i < bound, 0)` — 6,599/1,292; `…, 1` — 15,986/2,451.
* `volatile int end2` — 6,609/1,417, `this=None/i=r9`.
* pure-hoist `expect_ag` — `this=r4/i=r9`, 6,577/1,442 (does not move `i` to
  `fp` because `this` does not reach the tail).

## 3. Why the target is still not reproduced

The allocator tie-break is between keeping `this` in a callee-saved register
across the loop (ours: `fp`/`r5`) and rematerialising it from `sp+124` at the
body head and tail (original: two short `r4` intervals).  The original's
`&rotation` is materialised into `r4` (clobbering `this`), whereas ours keeps
`&rotation` in `sl` (or `fp` in the pure hoist).  No source form tried this
wave flips that choice: the two are mutually circular (choosing `r4` for
`&rotation` splits `this`; splitting `this` is what frees `r4` for
`&rotation`).  The branch-weight lever (`expect_ag`) moves `i`/`k` but not
`this`.

## 4. Metrics and gate (before -> after)

No source change landed; the tree is byte-identical to HEAD.

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5461 -> 5461
                           align  781 -> 781
.text diff             85,518 -> 85,518
.ARM.extab diff         1,258 -> 1,258
.rel.plt                    0 -> 0   (byte-identical)
file size             2,187,328 -> 2,187,328 (exact)
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
```

`check` is green.

## 5. Open questions

1. The `expect_ag` shape (`this=r5`, `i=fp`, `k=r9`, raw 5,844/825) is the
   closest map match.  A follow-up should try to force `&rotation` into `r4`
   (e.g. by a source form that makes `rotation`'s address have a shorter live
   range) so `this` splits and takes `r4`, keeping `i` in `fp`.
2. The `(i<0)` hack can be dropped at no size cost on the current base; the
   original's `cbz` table select is then reproduced.  Recovering the raw
   regression (5,659 -> 6,586) needs the individual `ldr`/`str` vector
   construction of the original (`0xb32dc`–`0xb32f2`) instead of our
   `ldrd`/`strd`.
3. `alienGuns` is reloaded from `sp+52` every iteration in the original; ours
   keeps it in a callee-saved register (`r8`/`r9`).  That is another RA
   difference to attack.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10712/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10712/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10712/regmap.py
python3 /dev/shm/w10712/map2.py
```

Scratch: `/dev/shm/w10712/{fdiff.py,align_diff.py,regmap.py,map2.py,
sweep_var.py,batch1..9.py,Level.base.cpp,orig_full.asm,ours_full_natural.asm}`.
