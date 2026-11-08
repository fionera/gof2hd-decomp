# MGame::OnTouchEnd — wave 157 (w10211): `cmp #0; beq` costs the 8080 layout in *both* `== 1` guards

Status: analysis note, **no source change landed**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10211`, base `cf74f4ee6` (wave 155 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10211`.
Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 155) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4391 | **4391** |
| aligned (align_diff) | 903 | **903** |

## 1. There are *two* `cmp #1; bne` guards, not one

The wave-155/156 notes only tracked the `won` guard.  The disassembly has a
second identical artifact at `0x17ab36`:

| site | original | ours (base) | source |
|---|---|---|---|
| `0x17ab36` | `cmp r4,#0 ; beq.w won` | `cmp r4,#1 ; bne.w won` | `if (failed == 1)` (line 1516) |
| `0x17ade2` | `cmp r0,#0 ; beq.w else` | `cmp r0,#1 ; bne.w else` | `won = ...->hasWon() == true` (line 1555) |

`failed` is an `int`; `hasFailed()` returns `unsigned char` (0/1), so
`failed == 1` and `failed != 0` are semantically identical.  Both `== 1`
spellings are **layout crutches**: they keep the branch at the default 50:50,
which is the only weight that reproduces the original's block order.  Changing
either one to truthiness (`!= 0`, `if (failed)`, `if (failed == 0)` + body
swap) gives the zero heuristic's **20:12 (62.5 %/37.5 %)** weight and re-lays
the function.

## 2. Measured spellings (all on the exact wave-155 base)

| variant | size | raw | aligned | guard |
|---|---:|---:|---:|---|
| base (`failed==1`, `won==true`) | 8080 | **4391** | **903** | `cmp #1; bne` ×2 |
| `won = A \|\| B` (truthy) | 8060 | 6069 | 1871 | `cmp #0; beq` |
| `won = A != 0 \|\| B != 0` | 8060 | 6069 | 1835 | `cmp #0; beq` |
| `won = A \|\| (B & 1) != 0` (AND exception) | 8080 | **4371** | 904 | `lsls #31; bne body` |
| `won = A \|\| (B & 1)` / `A != 0 \|\| (B&1)!=0` | 8080 | 4371 | 904 | `lsls #31; bne body` |
| `bool won=A; if(!won) won=(B&1)!=0; if(won)` | 8080 | 4371 | 904 | `lsls #31; bne body` |
| `(A & 1) \|\| B == true` | 8076 | 6118 | 1135 | — |
| `if (failed != 0)` / `if (failed)` / `if (failed > 0)` | 8076 | 4711 | 1968 | `cmp #0; beq` |
| `if (failed == 0) {won} else {failed}` (body swap) | 8076 | 4711 | 1968 | `cmp #0; beq` |
| `if ((failed & 1) != 0)` (AND exception) | 8080 | 4958 | 987 | `lsls r4,#31; bne` |
| `__builtin_expect(won, 1)` | 8096 | 4786 | 863 | — |
| `__builtin_expect(won, 0)` | 7832 | 5145 | 2162 | — |

## 3. IR branch weights (LLVM 7 `-mllvm -print-bpi`)

Base vs truthy differ in exactly one branch, repeated once per codegen run:

```
base  (== true):  %689 = icmp eq i8 %688, 1 ; br %689, %690(body), %1381(else)
                  0x40000000/0x80000000 = 50.00% / 50.00%
truthy (A || B):  %689 = icmp eq i8 %688, 0 ; br %689, %1381(else), %690(body)
                  0x30000000/0x80000000 = 37.50% / 0x50000000 = 62.50%
```

`calcZeroHeuristics` (`BranchProbabilityInfo.cpp:653`) gives `X == 0` 20:12;
`X == 1` matches no heuristic and stays 50:50.  The original was PGO-built, so
its `cmp #0` carries a profile weight that happens to place blocks exactly like
50:50.

## 4. The AND exception is the only 8080 `cmp #0` lever, but it breaks the PLT

`calcZeroHeuristics` has an escape hatch: if the compare LHS is `and` with a
power-of-2 mask it returns false (default 50:50).  `(B & 1) != 0` therefore
keeps the 8080 layout and improves raw **4391 -> 4371 (-20)**, but:

1. the backend lowers `(x & 1) != 0` to `lsls #31; bne body` (4 B) instead of
   `cmp #0; beq else` (6 B), and the *else* block becomes fall-through;
2. the else block's `blx resetStartSequenceOver` is then the first relocation,
   so gold's `.plt` order changes: `resetStartSequenceOver` moves ahead of
   `dockToStream/useCloak/maneuverTouchEnd/isInstantActionMission`
   (`0x683a0..0x683d0`).  `MGame::dockEvent`'s single `blx dockToStream`
   target shifts `0x683a0 -> 0x683ac`, so `check` reports
   **byte_exact 4507 -> 4506, RATCHET FAIL**.  (aligned 903 -> 904 too.)

Making the *body* fall-through (successor 0) while keeping `icmp ne (and),0`
would restore the PLT order; every source spelling tried (`if (!won) won=...`,
`A != 0 || (B&1) != 0`, `if (won & 1)`) is canonicalised by InstCombine back to
`icmp eq (and),0; br else,body`, so the backend still falls through to else.

## 5. Mining guard (`count >= 0`, line 1394)

Original `0x17a5ca` is `cmp r9,#0 ; beq` (`amount != 0`); ours is
`cmp r9,#0 ; bgt` (`count >= 0`).  `count != 0` / `amount != 0` is an
`ICmpZero` use of the same SCEV as `String(count)` and LSR flips the IV
(8060, raw 6590, aligned 1835).  `count >= 0 && amount != 0` keeps 8080 but
canonicalises to `cmp r9,#-1 ; bgt` (raw 4393, aligned 904).  No spelling
found that emits `beq` without the LSR flip.

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10211/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4391 / 8080
python3 /dev/shm/w10211/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 903
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10211/{fdiff.py,align_diff.py,try.py,trym.py,tryf.py,
tryb.py,tryfile.py,ops2.py,widths.py,bpi_base.txt,bpi_truthy.txt,c1..c19.so,
f1..f6.so,g1..g4.so,h1.so,m1..m10.so,n1..n4.so,w1..w9.so,swap_*.txt}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
