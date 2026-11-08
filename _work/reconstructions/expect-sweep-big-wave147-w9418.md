# `__builtin_expect` sweep on the big residuals — wave 147 / w9418

Status: **one source change landed** (`LevelScript::process` mission-0 loop
statement order), 2026-11-08. Worktree `wt/w9418`, base master `cbe3b5f7f`
(wave 147). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`). Scratch: `/dev/shm/w9418/`.

Read first: `expect-sweep-wave147-w9318.md`, `radar-draw-wave147-w9309.md`,
`systemic-idioms-wave146-w9220.md`, `levelscript-process-wave146-w9204.md`.

## 0. Result

| | before | after |
|---|---:|---:|
| byte_exact / linked_exact | 4507 / 4507 | **4507 / 4507** |
| size-exact | 4524/4524 | 4524/4524 |
| data / imports / lint / drift | 843 / 207 / CLEAN / 0 | same |
| `.text` differing bytes | 86,160 | **86,153** |
| `.ARM.extab` differing bytes | 1,309 | 1,309 |
| `.rel.plt` / build-id | 4 / 20 | 4 / 20 |
| section sizes | all 27 exact | all 27 exact |

Landed diff (`src/game/world/LevelScript.cpp`, mission-0 intro loop):

```diff
-                enemyData[i]->geometry->setVisible(true);
                 enemyData[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
+                enemyData[i]->geometry->setVisible(true);
```

The original loop body at `0x137f34` calls `setDirection` **before**
`setVisible` (`0x137f4e blx setDirection`, `0x137f5e blx setVisible`); our
source had the reverse.  This is a witnessed statement order, not a metric
trick.

`LevelScript::process`: size 57,232 exact, raw **54,913 -> 54,906 (-7)**,
aligned **15,740 -> 15,732 (-8)**.

## 1. Method

Each variant was applied alone to a pristine base (one TU), rebuilt with
`link_so`, and measured with:

* raw same-address diff bytes (`/dev/shm/w9418/fdiff.py`),
* instruction-index alignment (`/dev/shm/w9418/align_diff.py`),
* size + section diff (`/dev/shm/w9418/secdiff.py`).

`align_diff.py` is extremely greedy on `LevelScript::process` (20,830
instructions); its absolute number moves by thousands for a one-byte edit, so
for that function the raw metric and the *first divergence offset* are the
reliable signals.  `bigdiff1.py` (first-diff printer) was used to locate the
first real statement-order difference.

## 2. Response table (size / raw / aligned)

### LevelScript::process (`_ZN11LevelScript7processEi`, base 57,232 / 54,913 / 15,740)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 57,232 | 54,913 | 15,740 |
| **loop: setDirection before setVisible (landed)** | **57,232** | **54,906** | **15,732** |
| same + drop `enemyData` local (`(*enemies)[i]`) | 57,232 | 55,088 | 16,208 |
| faithful accessor order `messages, player, enemies` | 57,300 | 55,257 | 15,381 |
| `expect(mission == 0, 0)` | 57,452 | 55,399 | 23,559 |
| `expect(mission == 0, 1)` | 57,248 | 55,622 | 23,585 |
| `expect(mission == 1, 0)` | 57,240 | 55,452 | 23,268 |
| `expect(mission == 1, 1)` | 57,420 | 55,535 | 23,555 |

The accessor order is the witnessed original prologue
(`getMessages`, `getPlayer`, `getEnemies` at `0x137d3e/46/50`) and it improves
the aligned metric by 359, but it is **+68 B** on this base (w9204 measured
+76 on the wave-145 base), so it cannot land without a -68 companion.  The
`expect` forms all change the size and blow up the greedy aligner; none is
landable.  `expect` on the mission dispatch is therefore **falsified** for
this function.

### Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`, base 7,072 / 5,704 / 2,304)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 7,072 | 5,704 | 2,304 |
| all 179 `Globals::iPadHD ?` ternaries -> `expect(...,1) ?` | 7,020 | 6,451 | 4,208 |
| all 11 `if (Globals::iPadHD)` chains -> `expect(...,1)` | 7,072 | 5,704 | 2,304 (no-op) |

The if/else chains already have the HD arm as fall-through, so `expect` is a
literal no-op there.  The ternary pool is SLP/constant-order dominated (w8806);
`expect` on the ternaries only perturbs the vector grouping and shrinks the
function by 52 B.  **No expect win for Layout C1.**

### Generator::createMission (`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`, base 1,996 / 445 / 59)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 1,996 | **445** | **59** |
| `expect(agent->getOffer() == 5, 0)` | 1,968 | 1,831 | 261 |
| `expect(agent->getOffer() == 5, 1)` | 2,016 | 1,875 | 673 |
| `expect(type == 8, 0)` | 2,020 | 1,811 | 419 |
| `expect(used != nullptr, 1)` | 1,980 | 1,651 | 279 |

All four change the size and are far worse.  The 59 aligned diffs are
register-allocation ties (`r6` vs `[sp,#16]`, `r5` vs `r6`,
`subne.w r6,r6,r5,lsl#1` vs `subne r5,r6,r0`), not block layout — consistent
with w9318's conclusion.  **No expect win for createMission.**

### MGame::OnTouchEnd (`_ZN5MGame10OnTouchEndEiiPv`, base 8,080 / 4,127 / 906)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 8,080 | **4,127** | **906** |
| `expect(hudTouchFlags & 0x200, 0)` | 8,076 | 4,670 | 1,119 |
| `expect(hudTouchFlags & 0x200, 1)` | 8,036 | 6,076 | 1,283 |
| `expect(hudTouchFlags & 0x400, 0)` | 8,080 | 4,671 | 1,124 |
| `expect(hudTouchFlags & 0x400, 1)` | 8,016 | 6,040 | 1,246 |
| both (`0x200`=0, `0x400`=1) | 8,028 | 6,067 | 1,279 |
| `expect(hudMenuOpen, 1)` | 8,068 | 5,926 | 1,153 |
| `expect(hudTouchFlags != 0, 1)` | 8,148 | 7,354 | 1,773 |
| `expect(hudTouchFlags != 0, 0)` | 8,048 | 7,312 | 2,117 |

**No expect win for MGame.**  The first real divergence is at instruction
1182 (`0x17aeae`), the `hudTouchFlags & 0x800` / `useCloak` arm: the original
tail-merges only the `blx initHudMenu` call (`0x17adc0 ... b.n 0x17afde`,
`0x17afd8 movs r1,#2` falls through), while ours merges the whole
`ldrd/movs/blx` block at `0x17adca` and the `0x400` arm branches back to it.
Every expect variant either keeps the merge and worsens the metric or changes
the size.  The lever is the block-placement/weight of the shared
`initHudMenu` block, not the flag tests themselves.

## 3. Conclusion

The big functions do **not** respond to `__builtin_expect` on their hot/cold
guards:

* `LevelScript::process` — mission dispatch expects all change size and
  destroy the greedy alignment; the real win is a witnessed statement order in
  the mission-0 loop.
* `Layout::Layout` — the HD if/else chains are already correctly laid out
  (expect is a no-op); the residual is SLP constant pooling.
* `Generator::createMission` — pure register-allocation ties.
* `MGame::OnTouchEnd` — a tail-merge/block-placement difference on the shared
  `initHudMenu` call site, which the flag-weight knobs do not reach.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9418/fdiff.py _ZN11LevelScript7processEi   # 54,906 / 57,232
python3 /dev/shm/w9418/align_diff.py _ZN11LevelScript7processEi  # 15,732
python3 /dev/shm/w9418/secdiff.py                            # .text 86,153
cmake --build cmake-build-match --target check -j6           # RATCHET PASS
```

## 5. Open questions

1. `LevelScript::process` faithful accessor order (`messages, player,
   enemies`) is +68 B on this base and improves aligned by 359; it needs a
   -68 companion.  (w9204's open item 3, still open.)
2. `MGame::OnTouchEnd` shared `initHudMenu` tail: the original merges only the
   `blx`, ours merges `ldrd/movs/blx`.  A source shape that prevents the load
   CSE (e.g. two distinct `this->hud`/`this->level` expressions) is untested.
3. `Generator::createMission` 59 aligned diffs are register-allocation ties;
   no source lever found.
