# w7677 (wave-130b): `Level::createMission` — call-site / local-reuse sweep (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7677`, based on master `d16f28dd6`
(the wave-130 tail-frame ballast was reverted upstream: it raised the fuzzy ratio
to 92.333086 % but added **+37 `.text` differing bytes**, so the mainline kept the
`volatile unsigned n` latch instead).  This pass continues
`level-createmission-wave130-w7677.md` and tests the *call-site / loop-local reuse*
hypothesis plus the two remaining spill-slot permutations.

Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra `0xbffd8`,
linked `.text` `0xaffd8`, 17172 B).  Build/flags unchanged (NDK r18b clang 7.0.2,
`-Oz`).

## Baseline (master `d16f28dd6`, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
.text diff_bytes 85507  (whole section)
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The metric the mainline actually ratchets on for this function is the **`.text`
differing-byte count** (85507), not the fuzzy ratio: the function is not
`byte_exact`, so any change that does not flip `byte_exact` is judged by whether
it shrinks the byte diff.  Every variant below was scored with the official
verifier normalisation (`/dev/shm/w7677/score.py`) *and* `tools/verify` ELF
section diff (`/dev/shm/w7677/textdiff.py`).

## 1. Case-2 convoy loop: local reuse (the new hypothesis)

The original re-reads `this->enemies` from `[r5,#248]` for every statement in the
case-2 convoy loop (0x1958/0x1978/0x198a), while the checked-in source caches it
in `Array<KIPlayer *> *enemies = this->enemies;` and keeps `convoyShip`.  Tried
removing the locals so each statement is `(*this->enemies)[convoyEnd + j]->...`:

| variant | % | size | insns | `.text` diff |
|---|---:|---:|---:|---:|
| **baseline (`enemies` + `convoyShip` locals)** | **92.326733** | 17172 | 6065 | **85507** |
| A: both locals removed | 90.151765 | 17188 | 6069 | 902802 |
| C: keep `enemies` only for the two stores, re-read `this->enemies` after | 90.151765 | 17188 | 6069 | 902802 |
| D: swap `cargo`/`crateCaptured` store order (locals kept) | 92.326733 | 17172 | 6065 | 85507 |

Removing the locals makes clang keep `this->enemies` live across the two virtual
calls and grows the function by 16 B (all following functions shift → 902802 diff
bytes).  The store order is re-scheduled by clang and is neutral.  The original's
extra `ldr.w r0,[r5,#248]` reloads are therefore a *rematerialisation* choice by
clang, not a source-local difference we can express without perturbing the whole
function.  **No win.**

## 2. `raid` / `Globals::status` spill-slot swap (`sp+84` vs `sp+88`)

Original: `raid` → `sp+84`, `Globals::status` (`fp`) → `sp+88`.
Ours: `raid` → `sp+88`, `Globals::status` → `sp+84` (17 sites).  Tried every
source-level lever that could reorder the two spill slots:

| variant | % | size | insns | `.text` diff |
|---|---:|---:|---:|---:|
| **baseline** | **92.326733** | 17172 | 6065 | **85507** |
| `bool raid;` moved to top of `createMission` | 91.485149 | 17172 | 6065 | 86967 |
| `bool raid = false;` at top of `createMission` | 91.485149 | 17172 | 6065 | 86967 |
| `int raid;` | 92.161716 | 17172 | 6065 | 85734 |
| `uint8_t raid;` | 92.161716 | 17172 | 6065 | 85734 |
| `bool raid = false;` + inverted `if` (single store) | 92.326733 | 17172 | 6065 | 85507 |
| assign ternary first, then `if(...) raid = false;` | 84.901460 | 17228 | 6072 | 906048 |

The declaration position is already optimal; the swap is a pure stack-slot
colouring tie-break (both slots live across the same range) and no source spelling
moved it.  **No win.**

## 3. Alien-turret table select: `cbz` diamond vs if-converted

Original (inside the loop, 0x32ee):

```text
032ee  ldr   r0, [sp, #64]      ; bigAlienWave
032f0  cbz   r0, 0x32f8
032f2  ldr   r1, [pc, #732]     ; kAlienStationTurrets
032f4  add   r1, pc
032f6  b.n   0x32fc
032f8  ldr   r1, [pc, #728]     ; kAlienOutpostTurrets
032fa  add   r1, pc
032fc  add.w r0, r1, r9
```

The checked-in source uses the anchor `(bigAlienWave + (i < 0)) ? ...`, which
produces an if-converted `cmp.w r1, r9, asr #31; it ne; movne`.  Removing the
anchor gives the original's `cbz` form **but** clang then hoists the select out of
the loop (the loop body no longer rematerialises it):

| variant | % | size | insns | `.text` diff |
|---|---:|---:|---:|---:|
| **baseline `(bigAlienWave + (i < 0)) ? ...`** | **92.326733** | 17172 | 6065 | **85507** |
| plain `bigAlienWave ? ...` | 88.413577 | 17140 | 6054 | 901024 |
| `if (bigAlienWave) t = ...; else t = ...;` | 88.413577 | 17140 | 6054 | 901024 |
| `(bigAlienWave + (i<0) - (i<0)) ? ...` | 88.413577 | 17140 | 6054 | 901024 |
| `(bigAlienWave \| ((i<0) & 0)) ? ...` | 88.413577 | 17140 | 6054 | 901024 |
| `(bigAlienWave != 0) ? ...` | 88.413577 | 17140 | 6054 | 901024 |

So the original keeps a loop-invariant select in-loop (clang rematerialises the
cheap diamond each iteration) while our clang hoists it.  The `(i<0)` anchor is
currently the only spelling that keeps it in-loop; it necessarily changes the
condition to the if-converted form.  **No win.**

## 4. `stationPirates` / `extraAliens` / `fortressGuns` slot types

Baseline slots: orig `sp+48/44/40`, ours `sp+44/40/48`.  Type variants:

| variant | % | size | insns | `.text` diff |
|---|---:|---:|---:|---:|
| **baseline `unsigned stationPirates`** | **92.326733** | 17172 | 6065 | **85507** |
| `int stationPirates` | 92.326733 | 17172 | 6065 | 85507 |
| `bool stationPirates` | 86.319782 | 17176 | 6072 | 882857 |
| `unsigned extraAliens` | 92.310231 | 17172 | 6065 | 85507 |

`int` is codegen-identical to `unsigned`; `bool` perturbs the whole function.
**No win.**

## 5. Conclusion

No source change in this pass lowered the `.text` differing-byte count or the
function count.  The checked-in `volatile unsigned n` latch baseline is kept.
The remaining residual is unchanged and backend-only:

1. alien-turret table select: original `cbz` diamond vs our if-converted form
   (requires keeping the select in-loop without making the condition if-converted);
2. `case4Tail`: original `fp = 1` (hostile) + `&Globals::rnd` reloaded from
   `sp+120`; ours keeps `&rnd` in `fp` and rematerialises `1`;
3. `raid`/`Globals::status` spill-slot swap `sp+84`/`sp+88` (17 sites);
4. `stationPirates`/`extraAliens`/`fortressGuns` slot permutation
   (`sp+48/44/40` vs `sp+44/40/48`);
5. case-183 test/body order; branchless `&&` sites; pirate-fortress
   `ldmia`-vs-`ldrd` SLP pair.

## 6. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7677/score.py
python3 /dev/shm/w7677/textdiff.py
cmake --build cmake-build-match --target check -j6
```

Scratch: `/dev/shm/w7677/{score.py,textdiff.py,batch.py,v1.py..v5.py,orig.asm,ours.asm}`.

## 7. Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
