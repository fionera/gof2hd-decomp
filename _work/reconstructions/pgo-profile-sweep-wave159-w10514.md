# PGO profile sweep on MGame/createMission/Layout/Radar (wave 159, w10514):
# the original is still the no-profile `-Oz` layout; residual is source shape

Status: **research note + harness only**, 2026-11-08. Worktree `wt/w10514`,
base master `cfde1f5f2` (wave 158). Scratch `/dev/shm/w10514`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). No `src/`,
`CMakeLists.txt`, baseline, allowlist or doc change is landed.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 85,372 differing bytes, `.ARM.extab`
1,284, `.rel.plt` byte-identical, `.note.gnu.build-id` 20. All 27 section
sizes exact.

This wave extends w10414 (`pgo-profile-sweep-wave158-w10414.md`) with NEW
axes: `-profile-guided-section-prefix=true` (the harness normally disables
it), `-max-uses-for-sinking=0` combined with the w10313 Status vector,
`-branch-fold-placement=false`, `-tail-dup-placement=false`,
`-block-placement-exit-block-bias` 1..200, `-arm-adjust-jump-tables`,
`-sink-common-insts`, `-simplifycfg-sink-common=false`,
`-disable-early-taildup`, and `-likely-branch-weight` 1..10000.

## 0. Bottom line

1. **No profile and no build flag improves any of the four functions on both
   metrics.** The original's block layout is the heuristic `-Oz` layout.
2. **`-profile-guided-section-prefix=true` is neutral-to-worse** (MGame
   7749/4118/8176 vs uniform 7650/4111/8176); it does not recover the
   original layout.
3. **`-max-uses-for-sinking=0` + the w10313 Status vector is worse** on all
   four (MGame 7629/4183/7956, createMission 16575/10167/17720, Layout
   6671/4196/7328, Radar 9645/5359/10688).
4. **`-block-placement-exit-block-bias` is the only flag that moves
   createMission** (5..150 -> 7822/876/17172), and it is worse; bias 1 and
   200 are exactly neutral.
5. **`-likely-branch-weight` only moves Radar** (the function with
   `__builtin_expect` hints); every value is worse than the 2000 default.
6. **NEW source-shape evidence (not PGO).** The first machine-block
   divergence is block 65 at `0x17ada8`: the original encodes `beq.n`
   (16-bit, short range) where we emit `beq.w`, because the original places
   the shared tail block `0x17afde` (`blx Hud::initHudMenu`) close enough for
   a short branch. The original also writes `cmp r0,#0; beq` where we write
   `cmp r0,#1; bne` (semantically equal for a `bool`) at `0x17ade2`, and the
   original's `0x17adc0` block is a tail-call thunk to `0x17afde` while ours
   inlines the call. These are source-order/shared-tail differences, not
   branch weights.
7. **Gate:** every profile changes the target size and therefore drops
   `byte_exact` by the address-shift artefact (`linked_exact` stays 4507).
   Uniform `MGame` profile (8176 B) gives `byte_exact 3137`. No profile is
   landable.

## 1. Baseline (canonical no-profile build)

`raw` = same-address differing bytes (`fdiff.py`); `aligned` =
instruction-index alignment (`align_diff.py`); `size` = function bytes.

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | **4387** | **901** | 8080 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | **5487** | **774** | 17172 |
| `Layout::Layout` | `_ZN6LayoutC2Ev` | **5739** | **1978** | 7072 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | **8181** | **1278** | 9908 |

## 2. Profile variants (linked .so, one TU replaced)

| variant | MGame raw/al/size | createMission | Layout | Radar |
|---|---|---|---|---|
| uniform (all 1) | 7650 / 4111 / 8176 | 16612 / 9239 / 17492 | 6644 / 4150 / 7308 | 9633 / 5940 / 10596 |
| Status prefix | 7728 / 3971 / 8152 | 16623 / 10554 / 17588 | 6640 / 4149 / 7308 | 9629 / 6036 / 10504 |
| Status cycle | 7629 / 4183 / 7956 | 16649 / 10151 / 17648 | 6671 / 4196 / 7328 | 9636 / 5563 / 10600 |
| all 0 | 7360 / 2966 / 8132 | 16060 / 3476 / 17224 | 6632 / 4144 / 7292 | 9614 / 4951 / 10812 |
| uniform + section-prefix=true | 7749 / 4118 / 8176 | 16649 / 9260 / 17492 | 6644 / 4159 / 7308 | 9655 / 5945 / 10596 |
| uniform + sink0 | 7650 / 4111 / 8176 | 16637 / 9093 / 17492 | 6644 / 4150 / 7308 | 9617 / 5838 / 10368 |
| uniform + bias0 | 7650 / 4111 / 8176 | 16612 / 9239 / 17492 | 6644 / 4150 / 7308 | 9633 / 5940 / 10596 |
| uniform + no-branch-fold | 7623 / 4121 / 8192 | 16650 / 9249 / 17524 | 6636 / 4151 / 7308 | 9638 / 5942 / 10600 |
| uniform + no-tail-dup | 7576 / 4122 / 8184 | 16613 / 9245 / 17516 | 6644 / 4150 / 7308 | 9634 / 5940 / 10596 |
| uniform + likely=7/unlikely=1 | 7650 / 4111 / 8176 | 16612 / 9239 / 17492 | 6644 / 4150 / 7308 | 9633 / 5917 / 10596 |
| Status cycle + sink0 | 7629 / 4183 / 7956 | 16575 / 10167 / 17720 | 6671 / 4196 / 7328 | 9645 / 5359 / 10688 |

Targeted MGame profiles (dock counters 185..215 / tail 235..244 = 1000) are
byte-identical to uniform (7650/4111/8176); dock 190..205 and the epilogue
predecessors give 7651/4149/8192.

## 3. Plain build-flag variants (no profile)

| variant | MGame | createMission | Layout | Radar |
|---|---|---|---|---|
| baseline | 4387/901/8080 | 5487/774/17172 | 5739/1978/7072 | 8181/1278/9908 |
| bias 1 | 4387/901/8080 | 5487/774/17172 | 5739/1978/7072 | 8181/1278/9908 |
| bias 5..50 | 4387/901/8080 | 7822/876/17172 | 5739/1978/7072 | 8181/1278/9908 |
| bias 150 | 4644/1036/8076 | 7822/876/17172 | 5739/1978/7072 | 8181/1278/9908 |
| bias 200 | 4387/901/8080 | 5487/774/17172 | 5739/1978/7072 | 8181/1278/9908 |
| branch-fold=false | 5562/1401/8080 | 10344/850/17208 | 5766/1995/7072 | 8289/1287/9908 |
| tail-dup=false | 5370/2000/8076 | 7237/801/17192 | 5766/1995/7072 | 8263/1286/9908 |
| tail-dup-penalty=0 | 4387/909/8080 | 5487/774/17172 | 5739/1978/7072 | 8181/1278/9908 |
| adjust-jump-tables=false | 4387/901/8080 | 16018/831/17204 | 5739/1978/7072 | 8181/1278/9908 |
| sink-common-insts=true | 4387/901/8080 | 10755/903/17176 | 5857/2179/7080 | 8738/2130/9924 |
| simplifycfg-sink-common=false | 4395/904/8080 | 13269/910/17180 | 5947/2304/7096 | 8181/1278/9908 |
| disable-early-taildup | 4387/901/8080 | 5487/774/17172 | 5739/1978/7072 | 8322/1267/9912 |

`-likely-branch-weight` 1..10000 leaves MGame/createMission/Layout unchanged
except MGame at 1 (5099/3198 object metric) and moves Radar (size 9908 only
at 2000). No value beats baseline.

## 4. NEW: first structural divergence is source shape, not weights

Splitting both disassemblies into basic blocks (branch targets + terminators)
shows the first 65 blocks are identical in order and opcode signature. The
first divergence is block 65 at `0x17ada8`:

```
orig  17ada8: ldr.w r0,[fp,#20]
      17adac: cmp   r0,#0
      17adae: beq.n 17aeac          ; 16-bit short branch
ours  17ada8: ldr.w r0,[fp,#20]
      17adac: cmp   r0,#0
      17adae: beq.w 17aef8          ; 32-bit: target too far
```

The original's shared tail block `0x17afde` (`ldrd r0,r2,[fp,#116];
movs r1,#1; blx Hud::initHudMenu`) sits close enough for `beq.n`; ours
inlines `blx Hud::initHudMenu` at `0x17adca` and jumps elsewhere. At
`0x17ade2` the original is `cmp r0,#0; beq.w` while we emit
`cmp r0,#1; bne.w` (equivalent for `bool`, different canonical form).
Source line 1555 (`hasWon() == true`) is the site; removing `== true`
(V1) gives 6069/1871/8060, and line 1557 `!= false` -> `?` (V2) is
neutral 4387/901/8080, so the current form is the best of the three.

The `count >= 0` site at `0x17a5ce` is also a real divergence: original
`cmp r9,#0; beq.n` (skip when `amount == 0`), ours `cmp r9,#0; bgt.n`.
`count != 0` gives 6590/1835/8060 and `count > 0` gives 4389/902/8080;
`count >= 0` (4387/901/8080) is the best of the three.

## 5. Gate

| build | byte_exact | linked_exact | size_exact | data | imports |
|---|---:|---:|---:|---:|---:|
| canonical (no profile) | **4507** | **4507** | **4524** | **843** | **207** |
| uniform MGame profile (8176 B) | 3137 | 4507 | 4524 | 843 | 207 |

`linked_exact` is unchanged because the other 4,523 instruction streams are
unchanged; `byte_exact` falls only because the target's size change shifts
later addresses/literal pools. No profile rises `byte_exact`.

## 6. Conclusion and open questions

The four bodies are already in the original's heuristic `-Oz` block order.
Every profile adds `function_entry_count` and branch weights, moves at least
one block, and changes the size. The residual raw diff is dominated by
source-order / shared-tail placement (MGame epilogue chain, createMission
raid/alien arms, Layout SLP group bases, Radar register permutation), which
a profile cannot express.

Open questions (source shape, not profile):

1. `MGame::OnTouchEnd` `0x17adc0`/`0x17afde`: the original tail-call thunk
   to `Hud::initHudMenu` vs our inlined call. Reconstruct the source shape
   that makes clang emit the thunk.
2. `MGame::OnTouchEnd` `0x17a5ce`: original `beq` (`count == 0`) vs our
   `bgt` (`count >= 0`); the `count != 0` form changes the whole loop layout
   (8060 B), so the branch is coupled to a larger source-shape change.
3. `MGame::OnTouchEnd` `0x17ade2`: `cmp #0; beq` vs `cmp #1; bne` on the
   `hasWon() == true` expression.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10514
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# generic harness (worktree-local copy of profgen_w10414.py)
python3 _work/reconstructions/profgen_w10514.py \
  src/game/menu/MGame.cpp _ZN5MGame10OnTouchEndEiiPv

# fast object-only sweep (no link) and linked table
python3 - <<'PY'
import sys; sys.path.insert(0,'_work/reconstructions')
import fast_w10514 as fast
p=fast.get('src/game/menu/MGame.cpp','_ZN5MGame10OnTouchEndEiiPv')
n=p.meta['ncounters']
for name,v in [('uniform',[1]*n),('all0',[0]*n)]:
    print(name,p.build(v,'w10514_'+name))
PY
```

Status control: the w10313 vector
(`status_instr_1432_relplt0_w10313.proftext`) with
`-mllvm -max-uses-for-sinking=0` reproduces 1299/583/1432 exactly, so the
harness is faithful.

## 8. Files changed

* `_work/reconstructions/pgo-profile-sweep-wave159-w10514.md` (this note).
* `_work/reconstructions/profgen_w10514.py` (profgen_w10414 with the
  `wt/w10514` path substitution).
* `_work/reconstructions/fast_w10514.py` (object-only sweep helper).
* No source, CMake, baseline, allowlist or doc change.
