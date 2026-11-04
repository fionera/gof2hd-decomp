# w5014: `Level::createMission` (91.128167 %, 17172 B) — wave-100:
# 2-D turret-table indexing + inline second turret vector

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5014`, branch `wave100-w5014`,
base `44225192c`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
This pass changes `src/game/world/Level.cpp` only (the `alienGuns` loop).

## Baseline (wave-98 landed state)

```text
_ZN5Level13createMissionEv   fuzzy 90.988611982 %   size 17172 B
                             (report rounds to 91.0)
gate: byte 4500 / linked 4500, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4), lint CLEAN, drift 0, RATCHET PASS
```

## Result

```text
_ZN5Level13createMissionEv   fuzzy 91.128167038 %   size 17172 B
                             (report rounds to 91.1)
gate: byte 4500 / linked 4500, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4), lint CLEAN, drift 0, RATCHET PASS
```

The function size stays exactly 17172 B, so no later function moves. The score
rises 90.988612 → 91.128167 (+0.1396 pp). `byte_exact` / `linked_exact` stay
4500/4524. This is a real, size-neutral improvement.

## 1. The landed body

```cpp
            // `i < 0` is always false here (i is a non-negative index); it is a
            // codegen anchor that keeps the table select inside the loop, matching the
            // original's in-loop `cbz` and restoring the raid block placement.
            const float (*t)[6] = (bigAlienWave + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
            gun->geometry->setRotation(v);
            v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
            ((PlayerTurret *) gun)->setScaling(6.0f);
            gun->shipGroup = localRace;
            gun->cargo = nullptr;
```

Two changes relative to wave-98:

1. **The table is kept as a 2-D pointer** (`const float (*t)[6]`) and indexed
   `t[k][…]`, instead of decaying to `const float *t = table[k]`. This is the
   original's shape: the original computes the row address once as
   `add.w r0, r1, r9` with `r9 = k*24` and then loads `t[0..5]` at **positive**
   offsets 0,4,8,12,16,20. With the 1-D form clang instead strength-reduces to
   `base + 20 + k*24` and loads `t[0..4]` at negative offsets (−20…−4) with a
   register-offset load for `t[5]`, which is a different (and 8 B larger)
   selection.
2. **The second turret vector is constructed inline** in the
   `MatrixRotateVector` call instead of in a named `Vector turretPos`. That
   shares the `sp+200` source-vector slot between the two calls, exactly as the
   original does (original: both source vectors at `sp+200`, temp at `sp+212`).
   The named form gives each vector its own slot (`sp+200` / `sp+212`).

Either change alone is not landable:

| variant | size | single-TU exact % | note |
|---|---:|---:|---|
| wave-98 HEAD (1-D, named `turretPos`) | 17172 | 90.922594 | baseline |
| inline only (1-D, inline pos) | **17180** | 91.005116 | +8 B, shifts every later function |
| 2-D only (2-D, named `turretPos`) | 17180 | 90.924092 | +8 B, no gain |
| **2-D + inline (landed)** | **17172** | **91.062144** | size exact, best |

The full-link verifier (the authoritative one) reports 90.988612 → 91.128167.

## 2. Why the 2-D form matters

Original `alienGuns` loop (image base + 0x10000, so the `.so` address is
`0xb32c6`):

```text
b32c6 ldr   r0, [sp, #64]      ; bigAlienWave
b32c8 cbz   r0, b32d0
b32ca ldr   r1, [pc, #732]     ; kAlienStationTurrets
b32cc add   r1, pc
b32ce b.n   b32d4
b32d0 ldr   r1, [pc, #728]     ; kAlienOutpostTurrets
b32d2 add   r1, pc
b32d4 add.w r0, r1, r9         ; r9 = k * 24  -> &table[k][0]
b32d8 mov   sl, r1
b32da add   r4, sp, #312
b32dc ldr   r1, [r0, #8]       ; t[2]
b32de ldr   r2, [r0, #16]      ; t[4]
b32e0 str   r1, [sp, #116]
b32e2 str   r2, [sp, #204]
b32e4 add   r2, sp, #200
b32e6 ldr   r1, [r0, #12]      ; t[3]
b32e8 ldr.w r8, [r0, #4]       ; t[1]
b32ec ldr   r0, [r0, #20]      ; t[5]
b32ee str   r1, [sp, #200]
b32f0 mov   r1, r4
b32f2 str   r0, [sp, #208]
b32f4 mov   r0, r5
b32f6 blx   MatrixRotateVector
```

Landed (full-link, `r8 = k*24`, `r9 = &temp`, `sl = &rotation`):

```text
f28c ldr   r0, [pc, #744]      ; station
f28e add.w r9, sp, #212
f292 ldr   r6, [pc, #744]      ; outpost
f294 add.w sl, sp, #312
f298 ldr   r2, [sp, #116]
f29a add   r0, pc
f29c ldr   r1, [sp, #64]       ; bigAlienWave
f29e add   r6, pc
f2a0 cmp.w r1, r2, asr #31     ; anchor: bigAlienWave + (i < 0)
f2a4 it    ne
f2a6 movne r6, r0
f2a8 add.w r0, r6, r8          ; &table[k][0]  (positive offsets!)
f2ac ldrd  fp, r1, [r0, #4]    ; t[1], t[2]
f2b0 str   r1, [sp, #104]
f2b2 ldrd  r1, r2, [r0, #12]   ; t[3], t[4]
f2b6 strd  r1, r2, [sp, #200]
f2ba add   r2, sp, #200
f2bc ldr   r0, [r0, #20]       ; t[5]
f2be mov   r1, sl
f2c0 str   r0, [sp, #208]
f2c2 mov   r0, r9
f2c4 blx   MatrixRotateVector
```

The first `MatrixRotateVector` now shares `sp+200` with the second one (the
original does the same), and the row address is `table + k*24` with positive
offsets. The remaining differences in this loop are scheduling/register
allocation (`t[1]` in `fp` vs `r8`, `t[2]` spilled to `sp+104` vs `sp+116`) and
the branchless `it ne; movne` select vs the original's `cbz`; the `i < 0`
anchor is still required to keep the select in the loop (plain
`bigAlienWave ? … : …` hoists it and gives 17144 B / 88.4 %).

## 3. Variant measurements

Single-TU `score.sh` (exact, unrounded; see §5 for the full-link caveat):

| variant | size | exact % | note |
|---|---:|---:|---|
| wave-98 HEAD | 17172 | 90.922594 | 1-D + named pos |
| inline only | 17180 | 91.005116 | +8 B |
| 2-D only | 17180 | 90.924092 | +8 B |
| **2-D + inline (landed)** | **17172** | **91.062144** | |
| 2-D + inline + `const float *row = t[k]` | 17180 | 91.005116 | row local re-creates the +20 form |
| 2-D + inline, plain selector (no `i<0`) | 17144 | 88.4 (rounded) | select hoisted out of loop |
| 2-D + inline, `if (bigAlienWave+(i<0)) t = station;` | 17172 | 91.062144 | identical code |
| 2-D + inline, `(i<0)?outpost:(bigAlienWave?station:outpost)` | 17172 | — | `it lt`/`it eq` selects, no `cbz` |
| 2-D + inline, `(bigAlienWave \| (i<0))` | 17172 | — | branchless, same |
| 2-D + inline, `(bigAlienWave & ~(i<0))` | 17172 | — | branchless, same |
| fortress loop also 2-D (`kBaseTurrets`) | 17172 | 91.020139 | loop matches better, overall worse |
| `b3dd0` bound reorder `start+pirates+extraAliens` | 17164 | 90.798052 | proven order, −8 B, −0.26 pp |
| `b2e04`/`b2f2a` predicate variants (`bool first`, swap, `&`) | 17172 | 91.062144 | codegen unchanged |
| `!((i != first) \| !alienWave)` (forces merge) | 17160/17164/17176 | 84.4/86.2/85.3 | merge achieved, block layout wrecked |
| convoy `switch(k)` / nested if / `!k` / `k<1` | 17172 | 91.062144 | clang still canonicalises 0,1,2 |

## 4. What still differs

- `alienGuns` select is a branchless `it ne; movne`, not the original's `cbz`.
- `alienGuns` load scheduling: original loads `t[2]`→`sp+116`, `t[4]`→`sp+204`,
  `t[3]`→`sp+200`, keeps `t[1]` in `r8`, `t[5]`→`sp+208`, and loads `t[0]`
  lazily after `setRotation`; ours loads `t[1],t[2]` first (`ldrd`), spills
  `t[1]` to `sp+104`, and reloads `t[0]` from there.
- `convoy` if-chain: original tests `k == 2`, `k == 1`, then `cbnz` default
  (source order 2,1,0); ours canonicalises to 0,1,2 and lays the bodies out in
  the opposite order.
- `b2e04` / `b2f2a` (`i == first && alienWave`, `i == first && pirateFortress`):
  original merges to `(i != first) | !flag` with `eor #1; orrs; bne`; ours emits
  two branches. Forcing the merge changes block layout and costs 8–16 B.
- `b3dd0` loop-bound association: original `((start+pirates)+extraAliens)+…`;
  ours `(start+extraAliens)+pirates+…`. Proven but −8 B / −0.26 pp.
- EH-pad tail (`b41ea`–`b42cc`) block order and filler run-lengths; ours has a
  trailing `nop` and `_Unwind_Resume` at `b42d2` vs the original's `b42d0`.
- `stationPirates` loop: `route->getWaypoint()` loaded once in the original,
  twice in ours.
- Register-allocation-only clusters at original `b3dd0`, `b3c6c`, `b3b92`,
  `b3f8e`, `b3f9c`, `b4198`, etc. (stack-slot and callee-saved assignment).

## 5. Measurement caveat

`score.sh` (single-TU gold link of `Level.o`) and the official full link
disagree by ~0.066 pp on this function because the verifier masks/normalises
literal-pool words differently (PIC `add rX,pc` deltas are masked in the full
link; the single-TU link has different pool contents). Always confirm with the
full link:

```sh
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level13createMissionEv
```

The full-link exact score is computed by importing `tools/verify/verify.py` and
replacing `score()` with the unrounded `difflib` ratio (the report rounds to
0.1 pp).

## 6. Gate counts (before → after)

```text
byte_exact      4500 → 4500   (4500/4524)
linked_exact    4500 → 4500   (4500/4524)
size_exact      4524 → 4524
stub_zero_size     0 → 0
missing            0 → 0
wrong_type         0 → 0
extra              0 → 0
data_exact       843 → 843
imports          207 → 207
.text          0x1337c8 → 0x1337c8
.rodata        0x41b67  → 0x41b67
.ARM.exidx     0x6f70   → 0x6f70
.ARM.extab     0x26294  → 0x26294  (-4)
lint            CLEAN   → CLEAN
drift           0       → 0
RATCHET         PASS    → PASS
```

Scratch (not committed): `/dev/shm/w5014/` (`exact.py`, `loopdump.py`,
`convdump.py`, `batch.sh`, `v_arr2d*.cpp`, `v_fort2d.cpp`, `v_b3dd_*.cpp`,
`v_b2e_*.cpp`, `v_conv_*.cpp`, `v_ag_*.cpp`, `a2d.o`, `arr2d.so`, …).
