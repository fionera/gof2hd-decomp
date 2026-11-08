# PGO profile sweep on MGame/createMission/Layout/Radar (wave 158, w10414):
# no instrumented profile beats the no-profile `-Oz` build

Status: **research note only**, 2026-11-08. Worktree `wt/w10414`, base master
`9e4795206` (wave 157). Scratch `/dev/shm/w10414`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). No `src/`, `CMakeLists.txt`,
baseline, allowlist or doc change is landed; this note is the only commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 85,529 differing bytes, `.ARM.extab` 1,292,
`.rel.plt` byte-identical.

Targets (wave-157 tree, no profile):

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | **4391** | **903** | 8080 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | **5487** | **774** | 17172 |
| `Layout::Layout` | `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` | **5753** | **1984** | 7072 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | **8181** | **1278** | 9908 |

Raw = same-address differing bytes (`fdiff.py`); aligned = instruction-index
alignment (`align_diff.py`). `Layout` is profiled through its `C2` symbol (the
`C1` alias has the same body and address); `Radar::draw` is the 3-argument
`draw(Player*, Hud*, int)` overload.

## 0. Bottom line

1. **No instrumented profile improves any of the four functions.** Every
   profile tested is worse than the no-profile build on both raw and aligned
   diff, and every profile changes the function size. The original's block
   layout is the heuristic `-Oz` layout for these four bodies, not a profile
   layout.
2. **The complete single-counter-zero sweep (all counters, all four
   functions) found no size-exact variant and no variant better than baseline
   on either metric.** Best raw per function: MGame 7545 (ctr 22),
   createMission 16589 (ctr 81), Layout 6523 (ctr 1), Radar 9548 (ctr 70).
3. **`-mllvm -max-uses-for-sinking=0` is not the missing lever.** It changes
   createMission (16612/9239 -> 16637/9093) and Radar (9633/5940 ->
   9617/5838) but leaves MGame and Layout unchanged; all four are still far
   worse than baseline.
4. **The w10313 Status vector, applied as a prefix or cyclically, is worse.**
   It gives MGame 7721/3964/8140 (prefix) and 7629/4183/7956 (cycle), and
   similar regressions on the other three.
5. **Removing the `isDockingToStream` `__builtin_expect` does not help.**
   Plain no-expect build: 4405/915/8084 (vs 4391/903/8080). With a uniform
   profile: 7647/4153/8176.
6. **Block-placement / if-conversion flags are neutral or worse.**
   `-disable-block-placement` is a large regression; `-block-placement-exit-block-bias=0`,
   `-disable-early-ifcvt` and `-old-thumb2-ifcvt` are exactly neutral;
   `-block-placement-exit-block-bias=100` and the `-disable-ifcvt-triangle*`
   set are worse on MGame/createMission.
7. **Gate:** every profile that changes a target's size drops `byte_exact`
   (the address-shift artefact); `linked_exact` stays 4507. Best-per-function
   zero-sweep variants: MGame 4290, createMission 2502, Layout 2455,
   Radar 2041 `byte_exact` (all `linked_exact 4507`). No profile is landable.

## 1. Method

The w10314 generic harness is used unchanged except for the worktree path:

```sh
cd /home/debian/GalaxyOnFire2/wt/w10414
sed -e "s#REPO='/home/debian/GalaxyOnFire2/wt/w10314'#REPO='/home/debian/GalaxyOnFire2/wt/w10414'#" \
    -e "s#/dev/shm/w10314/run#/dev/shm/w10414/run#" \
    _work/reconstructions/profgen_w10314.py > /dev/shm/w10414/profgen.py
```

`Prof(tu, mangle)` instruments the TU with `-fprofile-instr-generate
-emit-llvm -S`, parses the function's 64-bit hash and counter count, writes a
text profile, merges it with `llvm-profdata`, recompiles the TU with
`-fprofile-instr-use=<profdata> -mllvm -profile-guided-section-prefix=false`,
and gold-links the full `.so` with only that object replaced. `measure()`
reports raw / aligned / size. The `other_same`/`other_total` check confirmed
that every non-target function in the TU is bit-identical to the canonical
object in every variant (e.g. MGame 46/46, Layout 50/50).

Counter counts: MGame 245, createMission 225, Layout 669, Radar 443.

## 2. Profile families

### 2.1 Uniform / extreme profiles

| variant | MGame raw/aligned/size | createMission | Layout | Radar |
|---|---|---|---|---|
| uniform (all 1) | 7638 / 4106 / 8168 | 16612 / 9239 / 17492 | 6628 / 4154 / 7316 | 9633 / 5940 / 10596 |
| uniform + sink0 | 7638 / 4106 / 8168 | 16637 / 9093 / 17492 | 6628 / 4154 / 7316 | 9617 / 5838 / 10368 |
| all1000 + sink0 | 7685 / 4085 / 8060 | 16552 / 10046 / 17776 | 6590 / 4147 / 7264 | 9603 / 5088 / 10344 |
| all0, entry 1000 + sink0 | 7583 / 3924 / 7920 | 16606 / 6553 / 17248 | 6245 / 3912 / 7376 | 9594 / 5752 / 10176 |
| all1, entry 0 + sink0 | 7638 / 4106 / 8168 | 16720 / 9886 / 17492 | 6620 / 4136 / 7316 | 9612 / 5994 / 10324 |

### 2.2 Status-vector patterns

The w10313 116-counter vector (`status_instr_1432_relplt0_w10313.proftext`)
was applied as (a) a prefix (first 116 counters, rest 1) and (b) cyclically.

| variant | MGame | createMission | Layout | Radar |
|---|---|---|---|---|
| Status prefix | 7721 / 3964 / 8140 | 16623 / 10554 / 17588 | 6625 / 4153 / 7316 | 9629 / 6036 / 10504 |
| Status prefix + sink0 | 7721 / 3964 / 8140 | 16645 / 10562 / 17644 | 6625 / 4153 / 7316 | 9628 / 5863 / 10372 |
| Status cycle | 7629 / 4183 / 7956 | 16649 / 10151 / 17648 | 6697 / 4199 / 7336 | 9636 / 5563 / 10600 |
| Status cycle + sink0 | 7629 / 4183 / 7956 | 16575 / 10167 / 17720 | 6697 / 4199 / 7336 | 9645 / 5359 / 10688 |

Switch-group `4:3:2` / ramp profiles (source-order case blocks set to
4/3/2, default 1) were also tested on the two functions with switches:

| variant | MGame | createMission |
|---|---|---|
| switch ramp (4:3:2) | 7674 / 4222 / 8056 | 16636 / 9383 / 17472 |
| switch ramp + sink0 | 7674 / 4222 / 8056 | 16616 / 9371 / 17472 |
| switch all 4 | 7658 / 4059 / 8140 | 16690 / 9653 / 17488 |
| switch all 2 | 7710 / 3755 / 8120 | 16585 / 9291 / 17504 |

### 2.3 Sparse profiles (MGame)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| all 0, entry 1 | 7523 | 4815 | 7972 |
| all 0, entry 1000 | 7583 | 3924 | 7920 |
| all 0, entry 1, dock counters 180..200 = 1 | 7513 | 4817 | 7972 |
| all 0, entry 1000, dock counters 180..200 = 1000 | 7617 | 3928 | 7928 |
| all 1, dock counters 180..200 = 0 | 7634 | 4193 | 8164 |
| all 1, dock counters 180..200 = 1000 | 7638 | 4106 | 8168 |

### 2.4 Complete single-counter-zero sweep

All counters 1 except one counter 0. Full coverage (245 / 225 / 669 / 443
variants); no variant is size-exact and none beats baseline on both metrics.

| function | best raw variant | best aligned variant | size-exact variants |
|---|---|---|---|
| MGame | ctr 22: **7545 / 4117 / 8064** | ctr 148: 7623 / **3688** / 8200 | 0 |
| createMission | ctr 81: **16589 / 9238 / 17492** | ctr 79: 16640 / **8251** / 17452 | 0 |
| Layout | ctr 1: **6523 / 3915 / 7288** | ctr 172: 6650 / **3793** / 7328 | 0 |
| Radar | ctr 70: **9548 / 6025 / 10696** | ctr 309: 9582 / **5568** / 10372 | 0 |

Size distribution is dominated by the uniform-profile size (MGame 8168 x186,
createMission 17492 x147, Layout 7316 x622, Radar 10596 x234); the nearest
sizes to the original are still 16-216 B away.

### 2.5 Build-flag variants (plain, no profile)

| variant | MGame | createMission | Layout | Radar |
|---|---|---|---|---|
| `-disable-block-placement` | 7426 / 3890 / 7960 | 16429 / 6495 / 17336 | 6116 / 4005 / 7120 | 9557 / 4358 / 9896 |
| `-block-placement-exit-block-bias=0` | **4391 / 903 / 8080** | **5487 / 774 / 17172** | **5753 / 1984 / 7072** | **8181 / 1278 / 9908** |
| `-block-placement-exit-block-bias=100` | 4644 / 1029 / 8076 | 7825 / 882 / 17172 | 5753 / 1984 / 7072 | 8181 / 1278 / 9908 |
| `-disable-early-ifcvt` | **4391 / 903 / 8080** | **5487 / 774 / 17172** | **5753 / 1984 / 7072** | **8181 / 1278 / 9908** |
| `-old-thumb2-ifcvt` | **4391 / 903 / 8080** | **5487 / 774 / 17172** | **5753 / 1984 / 7072** | **8181 / 1278 / 9908** |
| `-disable-ifcvt-triangle*` | 4673 / 942 / 8080 | 6044 / 819 / 17172 | 5753 / 1984 / 7072 | 8397 / 1366 / 9904 |

### 2.6 Expect-removal source experiment (MGame)

A copy of `MGame.cpp` with `__builtin_expect(this->player->isDockingToStream(), 1)`
replaced by the plain call (the w10314 open question):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| plain, no profile | 4405 | 915 | 8084 |
| uniform profile | 7647 | 4153 | 8176 |
| all0, entry 1000 | 7583 | 3924 | 7920 |
| all0, entry 1 | 7523 | 4815 | 7972 |

The plain form is 4 B larger and 14 raw / 12 aligned worse than the committed
expect form; adding a profile does not move the shared dock block to the
original's asteroid-arm placement.

## 3. Gate

| build | byte_exact | linked_exact | size_exact | data | imports |
|---|---:|---:|---:|---:|---:|
| canonical (no profile) | **4507** | **4507** | **4524** | **843** | **207** |
| uniform MGame profile (8168 B) | 3106 | 4507 | 4524 | 843 | 207 |
| zero-sweep MGame ctr 22 (8064 B) | 4290 | 4507 | 4524 | 843 | 207 |
| zero-sweep createMission ctr 81 (17492 B) | 2502 | 4507 | 4524 | 843 | 207 |
| zero-sweep Layout ctr 1 (7288 B) | 2455 | 4507 | 4524 | 843 | 207 |
| zero-sweep Radar ctr 70 (10696 B) | 2041 | 4507 | 4524 | 843 | 207 |

`linked_exact` is unchanged because the instruction streams of the other
4,523 functions are unchanged; `byte_exact` falls only because the target's
size change shifts later addresses/literal pools. No profile rises
`byte_exact`.

## 4. Conclusion and open questions

The four bodies are already in the original's heuristic `-Oz` block order.
A profile always adds `function_entry_count` and branch weights, which moves
at least one block and changes the size; for these functions the move is
always away from the original. This confirms and extends the w10314 result
with a complete zero sweep, the Status-vector patterns, sink0, sparse
profiles, expect removal, and block-placement/if-conversion flags.

Open questions (source-shape, not profile):

1. The residual raw diff is dominated by source-order / shared-tail placement
   (MGame pre-dock epilogue, createMission raid/alien arms, Layout SLP group
   bases, Radar register permutation). The profile cannot express these.
2. A hand-injected `!prof` vector (as in the wave-154 Status experiment) is
   the only mechanism that has reproduced a source-order jump table; doing it
   without inline IR would need a source-level `__builtin_expect` pattern or
   a compiler flag that emits the same metadata.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10414
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# generic harness (worktree-local copy)
python3 /dev/shm/w10414/profgen.py src/game/menu/MGame.cpp _ZN5MGame10OnTouchEndEiiPv

# complete zero sweep (one process per function / counter range)
python3 /dev/shm/w10414/sweep0.py <tu> <mangle> zero
python3 /dev/shm/w10414/sweep_range.py <tu> <mangle> zero <start> <end>

# Status vector patterns
python3 /dev/shm/w10414/statusvec.py <tu> <mangle>

# gate on a candidate .so
python3 /dev/shm/w10414/profgen.py ...  # or verify_so()
```

The scratch harnesses (`profgen.py`, `sweep0.py`, `sweep_range.py`,
`statusvec.py`, `patterns.py`, `sparse.py`, `flags.py`, `flags2.py`) are
verbatim copies of the committed w10314 harness plus the worktree-path
substitution; they are not part of the repo.

## 6. Files changed

* `_work/reconstructions/pgo-profile-sweep-wave158-w10414.md` (this note).
* `_work/reconstructions/profgen_w10414.py` (the w10314 harness with the
  `wt/w10414` path substitution; the sweep drivers are scratch-only).
* No source, CMake, baseline, allowlist or doc change.
