# w7710 (wave-132): `Level::createMission` — case-183 offset-switch source order (`.text` 85379 -> 85376)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7710`, based on master `70e451ffe`.
Continues `level-createmission-wave131-w7700.md`, `…-wave130b-w7677.md`,
`…-wave128-w7653.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; Ghidra `0xbffd8`, linked `.text` `0xaffd8`, 17172 B).
Build/flags unchanged (NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.376238 % (report 92.3)   size 17172/17172 B
                             6055 orig / 6065 ours insns
.text diff_bytes 85379  (whole section)
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## New hypothesis: case-183 convoy `offset[3]` if-chain order

The wave-128/130b/131 notes listed the case-183 test/body order (2,1,0) as
"backend-only".  Re-reading the original disassembly shows the test order is
**2,1,0** but the body layout is **0,1,2,default**:

```text
b1408: bic.w r1, r5, #0x80000000
b140c: cmp   r1, #2
b1418: beq   b1438            ; case 2
b141a: cmp   r1, #1
b141c: beq   b142c            ; case 1
b141e: cbnz  r1, b1444        ; default
b1420: ...                    ; case 0 body
b142c: ...                    ; case 1 body
b1438: ...                    ; case 2 body
b1444: ...                    ; default body
```

The checked-in source wrote the chain `if (k == 2) … else if (k == 1) … else if
(k == 0) … else …`, which clang 7.0.2 canonicalises to test order **0,1,2** and
body layout **2,0,1,default**.  Writing the chain in ascending case order
(`if (k == 0) … else if (k == 1) … else if (k == 2) … else …`) makes clang emit
the original's test order **2,1,0**:

```text
b140c: cmp   r1, #2
b1418: beq   b142c            ; case 2
b141a: cmp   r1, #1
b141c: beq   b1438            ; case 1
b141e: cbnz  r1, b1446        ; default
b1420: ...                    ; case 0 body
b142c: ...                    ; case 2 body
b1438: ...                    ; case 1 body
b1446: ...                    ; default body
```

The body layout is still 0,2,1,default (the original is 0,1,2,default), but the
test order now matches and the whole 0x13b0–0x1470 region drops from **46 to 25
differing bytes**.  The 21-byte regional gain is partly offset by an 18-byte
literal-pool shift in 0x1500 (the case-3/5 block's `ldr [pc,#…]` offsets), so the
whole-function `.text` count falls only **85379 -> 85376**.  The official
verifier's rounded score stays **92.3** (raw 92.376238 -> 92.227723) because the
pool shift re-aligns the instruction diff; the report row is unchanged.

## Variants measured this session (fresh `link_so` + verifier normalisation)

| variant | raw % | `.text` |
|---|---:|---:|
| **baseline `if (k==2) … else if (k==1) … else if (k==0) …`** | **92.376238** | **85379** |
| **`if (k==0) … else if (k==1) … else if (k==2) …` (landed)** | **92.227723** | **85376** |
| `if (k==0) … else if (k==2) … else if (k==1) …` | 92.227723 | 85376 |
| `if (k==1) … else if (k==0) … else if (k==2) …` | 92.244224 | 85384 |
| `if (k==1) … else if (k==2) … else if (k==0) …` | 92.244224 | 85384 |
| `if (k==2) … else if (k==0) … else if (k==1) …` | 92.376238 | 85379 |
| `if (k==2) … else if (k==1) … else if (k==0) …` (baseline) | 92.376238 | 85379 |
| `switch (k) { case 2/1/0/default }` | 92.376238 | 85379 |
| `switch (k) { case 0/1/2/default }` | 92.227723 | 85376 |
| `if (k==0) … else if (k==1) … else if (k==2) … else if (k==3) …` | 92.301345 | 85388 (6064 insns) |
| ascending chain + `offset[1],offset[0],offset[2]` default body | 89.983498 | 902874 (size grew) |
| ascending chain + case-183 `enemies[8]` shipGroup/field_0x74 swap | 92.227723 | 85379 |
| ascending chain + case-183 `enemies[9]` swap | 92.211221 | 85382 |
| ascending chain + both swaps | 92.211221 | 85385 |

Also re-tested the wave-128/130b/131 residuals against the new chain:

| variant | raw % | `.text` |
|---|---:|---:|
| alien-turret faithful order (setRotation, setPosition, setScaling, cargo, shipGroup) | 92.242944 | 85529 (6063 insns) |
| alien-turret faithful order + shipGroup before cargo | 92.259449 | 85529 (6063 insns) |
| alien-turret faithful order, plain `bigAlienWave ? …` (no anchor) | 88.433914 | 901031 (17164 B) |
| fortress cargo-before-shipGroup | 92.326733 | 85379 |
| ascending chain + alien-turret faithful order | 92.094405 | 85526 (6063 insns) |
| ascending chain + fortress cargo-before-shipGroup | 92.178218 | 85376 |
| case4Tail faithful `this->enemies->size()` re-read (no `volatile n`) | 91.390838 | 832369 (17164 B) |
| hoist `Matrix rotation/Vector v/base/int k` out of both turret `if`s | 68.024071 | 886923 (17176 B) |

The alien-turret faithful order is closer in instruction count (6063 vs 6055) but
the register permutation (`this` r4-vs-r5, `gun` r6-vs-r4, `k*24` r9-vs-r8,
`&v` r5-vs-r9, table sl-vs-r6) is unchanged, and the literal-pool shift costs
more bytes than the case-183 gain, so it is not landed.

## Conclusion

The case-183 `offset[3]` if-chain is a genuine source-order lever: ascending case
order reproduces the original's **2,1,0** test order and halves the regional
differing-byte count (46 -> 25).  The whole-function `.text` count improves by
3 bytes (85379 -> 85376) at the original 17172 B; the official report score stays
92.3 and every gate count is unchanged.  The remaining residual is the same
backend set as wave-131 (alien-turret select/register permutation, `case4Tail`
`fp=1`/`&rnd` tie-break, `raid`/`status` and `stationPirates`/`extraAliens`
spill-slot permutations, case-183 body layout 0,2,1 vs 0,1,2).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7710/score2.py        # 92.227723, 6055/6065
python3 /dev/shm/w7710/textdiff.py      # 85376
python3 /dev/shm/w7710/rangediff.py 0x13b0 0x1470   # 25 differing bytes
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch: `/dev/shm/w7710/{score2.py,textdiff.py,rangediff.py,chunkdiff.py,runv.py,
batch1..11.py,orig.asm,ours.asm,gh_decomp.md}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85379 -> 85376
```
