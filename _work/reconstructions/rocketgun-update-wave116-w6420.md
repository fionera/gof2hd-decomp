# RocketGun::update regression guard + next-exact recipe — wave 116 (w6420)

**Status:** research note (2026-11-05). Worktree `wt/w6420`, branch
`wave116-w6420`, base `d672d349f` (wave-115 harvest). **No `src/` change
landed**: the target is already byte-exact and every variant of the two named
next candidates measured at or below the committed HEAD.

| metric | before | after |
|---|---:|---:|
| `_ZN9RocketGun6updateEi` fuzzy | **100.0 %** | **100.0 %** |
| size | 1908 B | 1908 B |
| byte / linked exact | yes / yes | yes / yes |
| gate `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| data / imports / lint / drift | 843 / 207 / CLEAN / 0 | unchanged |
| sections / file size | 27 / 2,187,328 B | unchanged |
| `.ARM.extab` | 0x26294 | 0x26294 (4 B short) |
| `.rodata` | 0x41c27 (orig 0x41b67) | unchanged |

`check` on the untouched tree: **RATCHET PASS**.

---

## 1. Regression guard: RocketGun::update is still byte-exact

`python3 tools/verify/verify.py ... --show _ZN9RocketGun6updateEi`:

```text
RocketGun::update(int)
  orig 1908B @0x15ecd8  |  ours 1908B @0x15ecd8
  ... every line identical ...
```

`report.json`: `pct 100.0, byte True, linked True, osize 1908, usize 1908`
(635/635 instructions).

The wave-115 winning shape is still present verbatim in
`src/game/weapons/RocketGun.cpp`; all six load-bearing choices survived the
wave-115 harvest:

1. plain `if (this->hasGeometry != 0 && this->gun->delayActive != 0)` (no
   `__builtin_expect`),
2. `AEGeometry *geom = this->geometry;` cached before `setDirection`,
3. `const float dt = (float) elapsed;` declared **after** the `hitSmall`
   launch block, immediately before the loop,
4. `(*this->trailTimers)[cur] = 0;` **after** `resetSystem`,
5. plain `Vector cross = VectorNormalize(VectorCross(...))` (no `V3`
   cross-buffer hack),
6. `Matrix &trail = (*this->trailMatrices)[i];` — `operator[]`, not
   `data()[i]`.

Choice 6 is the load-bearing one: `operator[]` keeps the element access as a
single address interval, which makes `%413` (the cluster temp LEA) evict
`this` from `r5`; `this` then colours to `sl`, `elapsed` keeps `r4`, and
`player` takes `r5` — the original colouring. With `data()[i]` the same
function is 93.8 % (wave-115 note). The other five remove the remaining
size/pool deltas.

**Guard result:** no regression; nothing to change.

---

## 2. Next candidate: `SpaceLounge::drawLounge` (95.7 %, 1948 B)

This is the only one of the two named candidates that actually uses
`Array<T>` container accesses. Its element reads are already written in the
winning `(*ptr)[i]` form:

```cpp
Agent *agent = (*this->agents)[this->hoverAgent];
Vector topLeft = *(*this->agentRects)[this->hoverAgent * 2];
Vector bottomRight = *(*this->agentRects)[this->hoverAgent * 2 + 1];
...
(*this->buttons)[i]->setVisible(false);
```

So the RocketGun lever is **already spent** here. Measured variants (full
gold link + per-function `verify.py` score, `autojunk=False`):

| variant | pct | size |
|---|---:|---:|
| HEAD (`(*ptr)[i]`, `short pad3` clamp) | **95.7** | 1948 |
| `agentRects->data()[...]` (both reads) | 95.7 | 1948 |
| `agents->data()[hoverAgent]` | 95.7 | 1948 |
| local `int idx = hoverAgent * 2` / `idx*2` forms | 95.7 | 1948 |
| local `Array<Vector*>* rects` / `Vector** rects = data()` | 95.7 | 1948 |
| `agent` declared after `pad`; `pad` first | 95.7 | 1948 |
| faithful clamp `-(width + pad * 2)` (drop `short pad3`) | 94.0 | 1948 |
| faithful clamp + single `centerX` | 94.0 | 1948 |
| faithful single `centerX` (drop `int x = centerX`, keep `short pad3`) | 95.7 | 1948 |
| `top = bottomRight.y - (float)(pad * 2)` | 95.7 | 1948 |
| `top` via `(float)pad * 2.0f` / `- pad - pad` / `2.0f*pad` | 81.8 | 1968 |
| `(width + (pad3 << 1))` / `2 * pad3` | 95.7 | 1948 |
| `(width + pad3 + pad3)` | 92.4 | 1952 |

The `data()` variants compile to **identical bytes** — unlike RocketGun, the
container syntax has no RA effect in this function. The residual is exactly
the wave-114 finding: the clamp region's greedy-RA weight gate
(`pad` 2.159e-04 cannot evict `x` 2.946e-04 from `r9`), plus the tail EH
landing-pad selectors. The faithful clamp is *structurally* right but scores
1.7 points lower because the tuned `short pad3` sign-extend changes the
colouring. No source-reachable variant beat 95.676047 %.

## 3. `Hud::draw` (95.5 %, 9332 B)

`Hud::draw` has **no `Array<T>` element access at all** in its body (the
`eventQueue` member is only read through `eventQueueDirty`), so the
`operator[]`/`data()` lever does not apply. Its residual is EH landing-pad
merging plus RA (wave-115 note). Measured variants:

| variant | pct | size |
|---|---:|---:|
| HEAD (`short frameH`, `int transferred`, `short barW`, `int barH`) | **95.5** | 9332 |
| drop `Globals::status &&` from the second-bar guard (original has none) | 83.3 | 9264 |
| `int frameH` (faithful; removes `sxth r0,r0`) | 93.7 | 9328 |
| `int barW` (faithful; removes `sxth.w r2,fp`) | 93.2 | 9324 |
| `int frameH` + `int barW` | 93.3 | 9328 |
| `int frameH` + `short barH` | 89.1 | 9324 |

The original really does load `frameH`/`barW` without a sign-extend
(`ldr r0,[sp,#96]; vmov s2,r0` and `vmov s0,r8` at 0x1658f6 / 0x1659ca), so
the faithful types are `int`; but the `short` declarations are load-bearing
for the RA and score ~2 points higher. Removing the `Globals::status &&`
guard is also faithful (original `if (0 < *(int*)(this+0x4b8))` at
0x1642f2) yet collapses the body by 68 B and 12 points — the guard is an
accidental RA anchor, not a logic difference.

---

## 4. Concrete recipe

1. **Find the container accesses.** Grep the target body for `Array<` members
   and `data()` / `[i]`. The lever only exists if the source uses
   `ptr->data()[i]`; rewrite it as `(*ptr)[i]` (both `Array::operator[]`
   overloads are plain `data_[i]`, so the source is semantically identical).
2. **Confirm the lever is live.** Swap `(*ptr)[i]` back to `data()[i]` and
   rebuild. If the bytes change, the syntax is load-bearing (RocketGun); if
   they are identical, the lever is spent (SpaceLounge) and the residual is
   RA/EH, not container syntax.
3. **Keep the faithful statement shape** — plain `if`, a local cache only
   where the original has one, stores in the original order, `dt` declared
   where the original declares it.
4. **Measure both shapes.** The fuzzy metric is a `SequenceMatcher` over the
   *normalized instruction stream including register names*, so greedy-RA
   colouring dominates. A structurally faithful shape can score lower than a
   tuned one (SpaceLounge clamp: 94.0 faithful vs 95.7 tuned; Hud types:
   93.2–93.7 faithful vs 95.5 tuned). Keep the higher score only if the
   source is still defensible C++.

## 5. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi
```

Scratch harness lived in `/dev/shm/w6420/` (`score.py` per-function scorer,
`try_sl*.py`, `try_hud.py`); none of it is in the repo.
