# w11311 (wave 168): `Level::createMission` — guard boolean / alienGuns /
# icmp-liveness sweep (no raw win; 5,435 / 767 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11311`, branch
`wave168-w11311`, base master `631c26f0b`.  Continues
`level-createmission-wave167-w11210.md` (guard `icmp` de-sinking; no win)
and `level-createmission-wave166-w11111.md` (frame map).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## The two guards (original)

Both station-loop guards are the same OR-form shape; the first is at
`0xb2e04`, the second at `0xb2f2a`:

```text
b2e04: subs.w r0, fp, r3     ; i - first
b2e08: mov    r1, r0
b2e0a: it     ne
b2e0c: movne  r1, #1         ; r1 = (i != first)
b2e0e: ldr    r2, [sp, #60]  ; alienWave (int, 32-bit slot)
b2e10: eor.w  r2, r2, #1     ; r2 = alienWave ^ 1
b2e14: orrs   r1, r2
b2e16: bne.w  b2f2a          ; -> else-if
b2e1a: <body1>
...
b2f2a: cmp    fp, r3
b2f2c: it     ne
b2f2e: movne  r0, #1
b2f30: ldr    r1, [sp, #56]  ; pirateFortress (int)
b2f32: eor.w  r1, r1, #1
b2f36: orrs   r0, r1
b2f38: bne.w  b30a4
```

The zero-init block at `b0f14` stores **32-bit** zeros to `sp+60`
(alienWave), `sp+56` (pirateFortress) and `sp+64` (bigAlienWave), so all
three are `int` in the original source.  The `eor` is therefore an
`xor i32 alienWave, 1` computed from the *reloaded* value inside the
loop, not a `xor i1` of the preheader `icmp ne` (which is what
`SinkCmpExpression`/InstCombine produce from our source).

## 1. Bool declarations — right guard shape, wrong size

Changing only the declaration (`int alienWave = 0` -> `bool alienWave =
false`) makes the first guard byte-for-byte the original OR form
(`subs; it ne; movne; ldr [sp,#60]; eor; orrs; bne`) with the `eor`
inline, because the `and` operand is then a load (not an `ICmpInst`) and
`FindMergedConditions` cannot split it.  But the bool zero-init is
cheaper and the whole function shrinks:

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| `bool alienWave` only | 17156 | 12403 | **818** | first guard exact, second split; -16 B |
| `bool pirateFortress` only | 17152 | 12471 | 974 | second guard exact, first split; -20 B |
| both bool | 17168 | 15965 | 3334 | both OR but negations hoisted to sp+116/sp+100 |
| both bool, `pirateFortress` declared first | 17156 | 15656 | 2012 | -16 B |
| `bool alienWave` declared first | 17164 | 12207 | 803 | -8 B |

The `bool alienWave` variant is the closest structural match yet
(aligned 818 vs 767) but the -16 B frame/encoding delta shifts every
following block, so raw explodes to 12,403.  The -14 B of the delta is
in the zero-init block itself: the original emits three `mov.w`/`str`
pairs (24 B); the bool variant emits one `str` for alienWave earlier
plus only the pirateFortress store here (18 B), and the rest is
16/32-bit encoding differences.  There is no single compensable block.

## 2. Source shapes that keep the int declaration

Every shape below was built with `link_so -j2` and measured with
`tools/fn_diff.py` (raw = same-address byte diff).

| # | guard source shape | size | raw | aligned |
|---|---|---:|---:|---:|
| HEAD | `i == first && alienWave` | 17172 | **5435** | **767** |
| a | `!((i != first) \| !alienWave)` (l3) | 17172 | 12459 | 1685 |
| b | `!((i != first) \| !(alienWave != 0))` | 17172 | 12459 | 1685 |
| c | `!((i != first) \| !(bool)alienWave)` | 17172 | 12459 | 1685 |
| d | `!((i != first) \| (alienWave ^ 1))` | 17180 | 14237 | 1772 |
| e | `!((i != first) \| ((alienWave & 1) ^ 1))` | 17180 | 14237 | 1772 |
| f | `!((i != first) \| ((alienWave & 1) == 0))` | 17180 | 14237 | 1772 |
| g | `!((i != first) \| (alienWave == 0))` (l1) | 17172 | 5446 | 790 |
| h | `!((i != first) \| !alienGuns)` | 17172 | 5446 | 790 |
| i | `!((i != first) \| !(alienGuns != 0))` | 17172 | 5446 | 790 |
| j | `i == first && alienWave == 1` | 17172 | 5446 | 790 |
| k | `i == first && alienWave > 0` | 17172 | 5446 | 790 |
| l | `i == first && alienGuns != 0` | 17164 | 13467 | 985 |
| m | `(i == first) & alienWave` | 17184 | 12411 | 2564 |
| n | `bool firstSlot = i == first;` used in both guards | 17172 | 5435 | 767 |
| o | `bool alienWaveFlag = alienWave != 0;` before loop | 17172 | 5435 | 767 |
| p | `int alienWaveFlag = alienWave;` before loop, `!alienWaveFlag` | 17172 | 12459 | 1685 |
| q | `int noAlien = alienWave ^ 1;` before loop | 17180 | 9114 | 2055 |

* The `xor` shapes (a-c) do produce the original OR form, but the `xor`
  is created next to the preheader `icmp ne alienWave, 0` (the one
  feeding `alienGuns`) and is spilled to **sp+116** (stationRace's slot),
  not recomputed inline from sp+60.  The guard then lands 12 B early and
  raw is 12,459.
* The i32-`or` shapes (d-f) are additionally folded to a `select`
  (`ldr [sp,#116]; it ne; movne; cmp; beq`) and cost +8 B.
* `alienGuns`-based shapes (h-j) simplify back to `alienWave` and stay
  split; using `alienGuns` as the condition itself (l) is -8 B.
* Hoisting the compare (`firstSlot`, n) or the bool (`alienWaveFlag`, o)
  is byte-identical to HEAD: the compare is still sunk and split.

## 3. Conclusion

The w11210 diagnosis stands and is now stronger: the original isel-time
condition is `or i32 (zext (i != first)), (xor i32 load alienWave, 1)`,
with the xor in the loop.  A `bool` declaration reproduces that guard
exactly (inline `eor` from sp+60) but changes the zero-init code and
frame colouring by -16 B; every int-preserving source shape either
leaves the `and` split (neutral/+11) or hoists the negation to sp+116
(raw 12 k).  No size-exact shape reduces raw below 5,435.

Nothing landed: tree left clean at the baseline (5,435 / 767 / 17,172 B).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11311/{Level.cpp.base,measure.sh,setguard.py,batch*.py,
dumpfn.py,orig.dis,ours.dis,ours2.dis,ours3.dis,all.log}` (`all.log` is the
`-print-after-all` dump, 883 MB; delete when done).
