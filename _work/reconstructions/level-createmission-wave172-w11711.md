# w11711 (wave 172): `Level::createMission` — operand-order swap gives the
# `and` IR form (exact 18-byte guard, insn 6197/6197) but raw regresses to
# 7,147; no shape beat the 5,425 baseline.  Nothing landed.

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11711`, branch
`wave172-w11711`, base master `41ef4c8b6` (wave 171).  Continues
`level-createmission-wave171-w11611.md`, `level-createmission-wave166-w11111.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5425   align 936   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. NEW: the source operand order decides `and` vs `or`

w11611 kept `if (i == first && alienWave)`.  InstCombine canonicalises
`icmp-first && bool` to `or (i != first), !alienWave` with the `xor`
hoisted to the loop preheader (LICM) and the branch polarity inverted
(`beq` instead of `bne`).  That is why w11611's guard was 2 B short
(`mov r1,r0` coalesced) and why both-bool without the swap was 15.6k raw.

Writing the **bool operand first** — `if (alienWave && i == first)` —
keeps the `and i1 (phi), (icmp)` form.  Verified with a minimal repro:

```text
bool g3(bool a, int i, int f) { if (a && i == f) ... }  ->  and i1 %4, %0
bool g4(bool a, int i, int f) { if (i == f && a) ... }  ->  or i1 %4, !%0
```

With `bool alienWave`, `bool pirateFortress` and both guards written
bool-first, the first guard becomes **byte-identical** to the original:

```text
b2e04 subs.w r0, fp, r3 ; mov r1,r0 ; it ne ; movne r1,#1
b2e0e ldr r2,[sp,#60] ; eor.w r2,r2,#1 ; orrs r1,r2 ; bne.w
```

and the second guard is the original's `cmp/movne r0` + inline `eor`
form.  Instruction count is exactly **6197/6197**.

## 2. Why raw still regresses: the zero stores disappear

`bool alienWave` lets LLVM prove the value is 0 on the pirate branch and
delete the `alienWave = 0` / `bigAlienWave = 0` stores the original keeps
at `b0f1e`/`b0f26` (word stores to sp+60/sp+64).  The block at `b0f30`
(the station-108 block) then sits 14 B earlier, and the whole
`b0f3e..b1a00` region (~2.8 KB) shifts; a second 2 B RA difference
(`adds r1,#80` vs the original `add.w r3,r1,#80`, pre-existing) shifts
the else block.  Net raw 7,147 despite the exact guard.

## 3. Measured matrix (all `link_so -j2` + `tools/fn_diff.py`)

| shape | size | raw | aligned | insn |
|---|---:|---:|---:|---:|
| HEAD (`int alienWave`) | 17172 | **5425** | **936** | 6204/6197 |
| bool alienWave only + convoy 1,0,2 (w11611) | 17172 | 6780 | 1009 | 6197/6197 |
| **both bool, bool-first, convoy 1,0,2** | **17172** | 7147 | 962 | **6197/6197** |
| both bool, bool-first, convoy 2,0,1 | 17156 | 12403 | 965 | 6203/6197 |
| all three bool, bool-first, convoy 2,0,1 | 17180 | 5816 | 950 | 6212/6197 |
| all three bool, bool-first, convoy 0,1,2 | 17172 | 11373 | 1005 | 6201/6197 |
| both bool, `i==first &&` first only / second only | 17172 | 7147 | 962 | 6202/6197 |
| `int` + explicit `!= 0` / bool locals (`bool x = alienWave`) | 17172 | 5425 | 936 | 6204/6197 |

Declaration-order sweep (both bool, bool-first, convoy 1,0,2):
`bigAlienWave,alienWave,pirateFortress` (aBP) = 7147 is the only
size-exact order; BPa 10378, BaP 8310 (17180), PBa 12184 (17180),
PaB 12495 (17188), aPB 10104 (17180).
Convoy-default store order: only 1,0,2 keeps 17,172 B with both bool.

## 4. Result

Nothing landed: every guard-exact shape regresses raw (best 5,816 at
size 17,180; best size-exact 7,147).  The remaining blocker is the
eliminated zero stores / block layout, not the guard itself.  Next
pass should try to keep the `alienWave = 0` / `bigAlienWave = 0` stores
alive (e.g. a non-constant phi incoming) while retaining the bool-first
`and` form.
