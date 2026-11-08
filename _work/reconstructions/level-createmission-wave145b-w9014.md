# w9014 (wave-145b): `Level::createMission` — alien-gun setPosition temp
# (`Vector p`) + declaration reorder: 17172 B, raw 5592 -> 5505

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9014`, based on master
`eb13ada9b` (wave 144). Continues `level-createmission-wave145-w9014.md`
(committed as `a21822e66`), `…-wave144-w8912.md`, `…-wave143-*.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9014/fdiff.py`).

## Result (landed)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5592 -> 5505  (-87)
byte_exact 4507 -> 4507 (4507/4524)   linked_exact 4507 -> 4507
size 4524/4524   data 843/843   imports 207   lint CLEAN   drift 0
parity 5370/5370   RATCHET PASS
```

The size stays exactly 17172 B (mandatory: a 17168 B build drops `byte_exact`
to 4506, a 17164 B build to 3087 because the verify tool compares at fixed
addresses).

## Source change (`src/game/world/Level.cpp`, alien-gun block only)

```diff
     if (alienGuns != 0) {
-        Matrix rotation;
         Vector v = {0.0f, 0.0f, 0.0f};
-        Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
+        Matrix rotation;
         int k = 0;
+        Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
         ...
             gun->geometry->setRotation(v);
-            v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
+            v = p;
             gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
```

Two independent effects, measured separately on pristine HEAD:

| change | size | raw |
|---|---:|---:|
| pristine HEAD | 17172 | 5592 |
| `Vector v` declared before `Matrix rotation` (and `k` before `base`) | 17172 | 5584 |
| `Vector p = MatrixRotateVector(...); v = p;` (decl order unchanged) | 17172 | 5513 |
| **both (landed)** | **17172** | **5505** |

### Why the `Vector p` temp helps

The original's second rotation writes the result into the call-return temp at
`sp+212` (`b331e mov r0,r5; blx MatrixRotateVector`), copies it into `v`
(`sp+372`) with `Vector::operator=`, then `setPosition` reads `v` (`sp+372`)
and `base` (`sp+224`) and stores the sum into `sp+212` again. The inline
`v = MatrixRotateVector(...)` form makes clang fold the temp into `v`; naming
the rotate result `p` and then assigning `v = p` restores the
`rotate -> temp -> operator= -> v` chain and the `vldr s0,[sp,#372]` /
`vldr s6,[sp,#224]` pairing at the `setPosition` call site. The landed form
uses `v` (not `p`) in the `setPosition` argument, so the call reads the same
`sp+372` slot as the original; using `p` there scores identically (5505) but
is less faithful.

### Faithfulness caveat (declaration order) — METRIC-ONLY in part

The declaration reorder `Vector v; Matrix rotation; int k; Vector base;` is a
**metric-only** part of the win: the original's asm initialises the `Matrix`
first (`b323c..b326a`), then `v` (`b326c..b3272`), then calls `getPosition`
for `base` (`b3274..b3284`), whereas this ordering emits `v`, `Matrix`,
`base`. It does, however, put `base` in `sp+224` and `v` in `sp+372`, which is
the original's stack-slot assignment (pristine HEAD swaps them). Pristine
declaration order + the `Vector p` change alone is 17172/5513 (still -79);
the coordinator may prefer that if strict source order is wanted.

## What was tried and did not help (new this wave)

All on top of the landed variant unless noted; size/raw.

* `Vector p` declared *outside* the loop: 17316/16579 (stack frame grows).
* `v = MatrixRotateVector(...); Vector p = v; setPosition(base+p)`: 5584.
* `setPosition(p.x+base.x, ...)` instead of `Vector{base.x+p.x, ...}`: 17176/5773.
* `setPosition(...); v = p;` (assignment after the call): 17176/5783.
* `Vector p = ...; gun->setPosition(base+p)` without `v = p;`: 17160/5946.
* Move `setScaling` after `setPosition` with the `p` temp: 17168/5943.
* `((PlayerTurret *) gun)->setScaling(6.0f)` (drop the `(*enemies)[i]` reload)
  with the `p` temp: 17168/5633.
* `cargo`/`shipGroup` swap with the `p` temp: 17172/5507.
* `Status *status` local in the raid block with the `p` temp: 17172/5530.
* `Vector base` before `Vector v` in the *pirate* block: 17172/5533.
* Pirate-loop `Vector p` temp for its `setPosition`: 17172/5510.
* Pirate-loop `Vector r` temp for its `setRotation`: 17160/6445.
* Pirate-loop second induction variable `k` (`t[k*6+...]`): 17172/5506.
* `Vector p` temp in the sieged-station turret loop: 17380/16627.
* `Matrix`/`v` declaration swap in the sieged-station block: 17172/5544.
* All 24 permutations of `{Matrix rotation, Vector v, Vector base, int k}` on
  top of the `p` temp: best is `v M k b` / `v M b k` / `v k M b` / `k v M b`
  = 5505; `M v k b` (pristine) = 5513; `M k b v` / `k M b v` = 5510.
* `volatile` forms remain +8 B (`volatile bool hard` 17176/5947,
  `volatile` pirate `t` 17180/6406, `volatile` pirate `i` 17168/6328).

## Remaining hunks (landed build)

Alien loop `b329a..b3394` is still 250/250 differing at the same address and
the pirate loop `b33fe..b34dc` 216/222; both are dominated by register-map
differences (`this` r4/r5, `i` fp vs spilled, `t` sl vs r6, the `cbz` table
select vs `it ne; movne`) that are not reachable from source without changing
the surrounding allocation. The largest remaining hunks in the function are
outside the two loops (`b1600..b19ea` 1004 B, `b3bac..b3e6a` 704 B,
`b2e04..b2fd0` 462 B) — untouched by this pass.

## Commands

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9014/fdiff.py _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j6
```

Scratch: `/dev/shm/w9014/{sweepA..sweepJ.py,Level.pv.cpp,Level.best1.cpp,
Level.setpos2.cpp,Level.pristine.cpp,Level.cur17168.cpp}`.
