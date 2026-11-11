# Radar::draw wave 169 (w11508) — campaign/`sl` lever re-probe, tail shapes

**Status:** analysis pass, no source change landed (2026-11-11). Worktree
`wt/w11508`, branch `wave169-w11508`, base `master` `00c403d5a`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave169-w11408.md` (branch-layout re-sweep, local optimum
7,486) and `radar-draw-wave169-w11407.md` (the five landed condition expects,
7,486 -> 7,127).

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | **7,127** |
| instruction-index alignment | **1,606** |
| instructions | 3,537 ours / 3,532 orig |
| `.text` differing bytes | 81,642 |
| `.ARM.extab` | section size 0x26298 in both; 712 differing bytes |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN) |

## 1. What the `sl`/`fp` value actually is (refines w11408 §1)

w11408 called it the "zero/campaign register choice". It is really
**`missionBlocksPlanets` vs `campaign`**:

* original `12ebda`: `mov r0,r5; blx Mission::getType; subs.w fp,r0,#172;
  it ne; movne.w fp,#1; b 12e0dc` — the out-of-line `if` body leaves
  `fp = (getType() != 172)`, i.e. `fp` is the `missionBlocksPlanets` local.
  The fall-through `12e0d8: mov.w fp,#0` is its `false` initialiser.
* original spills it at `12e3ba: str.w fp,[sp,#112]` and reloads it at
  `12e74c`, `12ea20`, `12eb3c`; `sl` is `campaign` (`12e0f8 mov sl,r0`,
  one use at `12e38a cmp.w sl,#2`) and only then becomes `stations`.
* ours puts **`missionBlocksPlanets` in `sl`** (`12eba6 subs.w sl,r0,#172`)
  and spills `campaign` to `sp+144` (`12e0f8 str r0,[sp,#144]`). The reload
  at `12e38c ldr r0,[sp,#144]` is the extra instruction that shifts the whole
  `12e3d0`-onward body by +4 B and rotates every stack slot above `sp+140`
  (`turretMode` sp+152 vs sp+148, `otherLock` sp+148 vs sp+144, `i` sp+152,
  `stations` sp+132 vs `sl`, ...). It also frees `fp` for the enemy-loop
  `locked` pointer (ours `12f74e mov r5,fp`; original `ldr r5,[sp,#152]`).

So the prize is unchanged and large (both values need a callee-saved reg in
the original), but the competition is `missionBlocksPlanets` (4+ uses) vs
`campaign` (1 use): the greedy allocator spills the single-use interval in
ours even though `fp` is free at `12e0f8`.

## 2. New campaign-placement probes (all reverted; base 7,127/1,606/9,908)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `turretMode` computed before `Mission *mission` | 7,205 | 1,620 | 9,908 |
| `alienOrbit` before `mission` | 7,233 | 1,670 | 9,908 |
| `campaign` before `mission` | 8,171 | 1,617 | 9,900 |
| three bools declared after `campaign`/getters | compile error (scope) | | |
| `campaign` declared just before its `if` | 8,677 | 1,682 | 9,908 |
| `campaign` declared after the transform | 9,148 | 1,646 | 9,912 |
| `campaign` declared after the getters | 7,204 | 1,611 | 9,908 |
| `campaign` second in the guard | 8,664 | 1,899 | 9,912 |
| guard parenthesised `campaign >= 2 && (...)` | 7,127 | 1,606 | 9,908 |
| `campaign > 1` / `2 <= campaign` / `!(campaign < 2)` / `campaign - 2 >= 0` | 7,127 | 1,606 | 9,908 |
| `(unsigned) campaign >= 2` | 7,128 | 1,606 | 9,908 |
| `campaign` `unsigned int` | 7,128 | 1,606 | 9,908 |
| `campaign` `long` | 7,127 | 1,606 | 9,908 |
| `missionBlocksPlanets` `unsigned char`/`uint8_t`/`char` | 7,188 | 1,670 | 9,912 |
| bool declaration order permutations / one-line decl | 7,127 | 1,606 | 9,908 |
| getter order swaps (enemies/landmarks, route/asteroids, gas/asteroids, reverse) | 7,131-7,143 | 1,608-1,624 | 9,908 |
| `plasmaInRange = lockConfirmed = 0` chain | 7,129 | 1,606 | 9,908 |
| `plasmaInRange/lockConfirmed = false` | 7,127 | 1,606 | 9,908 |
| zero stores moved after the mission block | 9,534 | 1,628 | 9,880 |
| `-mllvm -regalloc-csr-first-time-cost=0` (whole TU, CMakeLists) | 7,127 | 1,606 | 9,908 |

## 3. Condition-expect probes outside w11407's `if (...)` sweep

w11407 wrapped every `if (...)`; the loop conditions and ternaries were not
covered. All reverted:

| variant | raw | size |
|---|---:|---:|
| `enabled` expect 0 (`if (__builtin_expect(!enabled,0))`) | 7,158 (aligned **1,598**) | 9,908 |
| `!__builtin_expect(enabled,1)` | 7,158 | 9,908 |
| `!__builtin_expect(enabled,0)` | 9,972 | 10,328 |
| `enabled == 0` | 7,127 | 9,908 |
| landmark loop cond expect 0 / 1 | 9,486 / 9,157 | 9,888 / 9,880 |
| planet loop cond expect 0 / 1 | 9,666 / 9,199 | 10,072 / 9,932 |
| enemy loop cond expect 0 / 1 | 9,761 / 9,833 | 10,296 / 10,384 |
| gas loop cond expect 0 / 1 | 9,849 / 9,807 | 10,172 / 10,204 |
| sparks loop cond expect 0 / 1 | 9,439 / 8,619 | 9,984 / 9,888 |
| asteroid loop cond expect 0 / 1 | 7,934 / 9,432 | 9,956 / 9,904 |
| cargo loop cond expect 0 / 1 | 7,127 | 9,908 |
| `offset`/`size`/`image`/`enemy==locked` ternary expects | 7,127 (neutral) | 9,908 |
| `route` / `waypoint` / `lockedStation` expect 1 | 7,127 (neutral) | 9,908 |
| `route` / `waypoint` / `lockedStation` expect 0 | 9,209 / 9,224 / 8,759 | 9,928 / 9,924 / 9,928 |
| `alienOrbit` expect 0 / 1 | 9,577 / 9,509 | 9,876 / 9,848 |

The `enabled` expect 0 is the only probe that improves *aligned* (1,606 ->
1,598) while worsening raw (+31); raw is the ground truth, so not landed.

## 4. Exception-cleanup pads (re-confirmed)

The extra pad is still the enemy-loop `lockLabel = calcDistance(...)` temp
(line ~795): ours `12efa8 add r6,sp,#220`, original `12efca add r5,sp,#232`.
The named `AbyssEngine::AEMath::Vector delta` (line ~789) keeps `sp+232` live
across that call in ours, so stack colouring pushes the temp to `sp+220` and
the generic cleanup chain gains one `mov r4,r0` entry (`1306d8`). Removing or
scoping `delta` (inline `VectorLength(this->egoPos - this->targetPos)`, or an
inner `{}` scope) does free `sp+232` and shrinks the function to **9,888 B**
(20 B smaller, pad merged) but raw jumps to **8,694** (aligned 1,613) because
the whole tail re-lays out; size-exactness would also need +20 B of
compensation. Reverted.

## 5. Conclusion

The base remains a strong local optimum at raw **7,127** / aligned **1,606** /
9,908 B, `.rel.plt` 0, gate green. The single dominant lever is still the
`missionBlocksPlanets`/`campaign` callee-saved assignment at `12e0d8`/`12e0f8`;
no source spelling tried (declaration position/type/order, guard spelling,
getter order, bool order, CSR-cost knob) moves it. Everything else (the +4 B
body shift, the `cbz`/`beq.n` placement, the sound-ladder block order, the
7-vs-6 cleanup pads) is downstream of that one choice.

## 6. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11508/{m.sh,exp.py,adiff.py,sbs.py,regions.py}
```
