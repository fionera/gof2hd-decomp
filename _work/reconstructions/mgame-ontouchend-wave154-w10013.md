# MGame::OnTouchEnd — wave 154 (w10013): `won` guard is a bool-return-type artifact

Status: analysis note, **no source change landed**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10013`, base `4417db0b4` (wave 153 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10013`.
Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 153) | this wave |
|---|---:|---:|
| size | 8080 | 8080 |
| raw same-address diff | 4391 | **4391** |
| aligned (align_diff) | 903 | **903** |

## 1. Mission hypotheses (all three falsified)

The wave-153 note asked for a `do/while` with `won` in the condition, a `bool`
result local, or a `switch (won)`.  All were measured on the exact base:

| variant | size | raw | aligned | guard at 0x17ade2 |
|---|---:|---:|---:|---|
| `switch (won) { case true: ... default: ... }` (if/else replaced) | 8080 | 4391 | 903 | `cmp #1; bne` |
| `bool won; do { won = A \|\| B == true; } while (false);` | 8080 | 4391 | 903 | `cmp #1; bne` |
| `bool won = A; if (!won) won = B;` (bool result local) | 8060 | 6069 | 1871 | `cmp #0; beq` |
| `bool won; do { won = A \|\| B; } while (false);` | 8060 | 6069 | 1871 | `cmp #0; beq` |
| `bool won = false; do { if (A) {won=true;break;} won=B; } while(false);` | 8060 | 6069 | 1871 | `cmp #0; beq` |

Every `!= 0`/truthiness spelling canonicalises to the same IR as plain `A || B`
(8060); every `== true` spelling canonicalises back to the landed 8080 layout.
`switch (won)` and `do/while(false)` are fully folded by InstCombine.

## 2. New hypothesis: `Mission::hasWon()` returns `bool`, not `unsigned char`

`Mission::hasWon()` in the original is `ldrb r0,[r0,#5]; bx lr` — identical for
`bool` and `unsigned char`, so the accessor body cannot distinguish the two.  But
the *caller* can: if the return type is `bool`, InstCombine knows the value is
0/1 and `hasWon() == true` folds to plain truthiness, emitting the original
`cmp r0,#0 ; beq.w` instead of our `cmp r0,#1 ; bne.w`.

Measured with `bool won;` and `bool Mission::hasWon()` (member `won` also
`bool`, so the accessor body stays `ldrb; bx lr`):

| | size | raw | aligned | guard |
|---|---:|---:|---:|---|
| `bool hasWon()` + `== true` | 8076 | 6062 | 1081 | **`cmp #0; beq.w`** |
| `bool hasWon()` + `if (failed)` | 8064 | 6057 | 2000 | `cmp #0; beq.w` |

The guard is fixed, but the function is 4 B short and the whole block layout
re-lays (raw/aligned both worse).  More importantly the return-type change is
**not local**: full `check` drops `byte_exact` **4507 -> 4487** (20 other
functions that compare `hasWon()` against `true`/`false` change codegen).  So the
original's `hasWon()` really is `unsigned char`; the `cmp #0` at 0x17add6 must
come from a truthiness spelling of `won`, and the missing 20 B is elsewhere.

## 3. `(hasWon() & 1) != 0`: raw win, but breaks `MGame::dockEvent`

Forcing a low-bit test keeps the 8080 size and swaps the compare for
`lsls r0,r0,#31 ; bne.n`:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `A \|\| ((B & 1) != 0)` / `& 1` / `<< 31` | 8080 | **4371** | 904 |
| `(B & 0x80) != 0` | 8096 | 4704 | 1118 |
| `(B & 0xff) != 0` / `(B & 0x7f) != 0` | 8060 | 6069 | 1871 |

Raw improves by 20 but aligned regresses by 1 (the `lsls; bne.n` is 4 B vs the
original 6 B `cmp; beq.w`, so the body/else blocks swap fall-through order).
The 2-byte shift also moves MGame.cpp's literal pool and breaks
`_ZN5MGame9dockEventEii` (100.0 % fuzzy but no longer byte-exact): full `check`
gives `byte_exact` **4506**, RATCHET FAIL.  Not landable without also fixing
`dockEvent`.

## 4. `__builtin_expect` on the comparison does nothing

`__builtin_expect(B, 1)`, `expect(B != 0, 1)`, `expect(B, 0)`,
`expect(B != 0, 0)` all leave the 8060 flip; `expect(B == true, 1)` leaves the
8080 layout.  The `== true` shape, not the branch weight, drives the layout.

## 5. Open questions

1. The original's `won` guard is truthiness (`cmp #0; beq`) yet the original is
   8080, while every truthiness spelling we emit is 8060.  The missing 20 B is
   *not* at the guard (both guards are 6 B): it is a block-layout/pool difference
   elsewhere.  The next wave should diff the original against the `A || B`
   (8060) build block-by-block and locate the 4 extra GOT pool entries the
   original carries (tail pool 48 B vs 32 B).
2. `bool hasWon()` proves the original's accessor is `unsigned char`; the
   `== true` in our source is a layout crutch.  A future wave could try
   `hasWon() != 0` plus a size-neutral dummy use that restores the 20 B.
3. The mining guard is unchanged: `count >= 0` (`cmp #0; bgt`) vs the original
   `cmp #0; beq`; no free `+amount` Basic use was found in waves 151-153.

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10013/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4391 / 8080
python3 /dev/shm/w10013/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 903
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10013/{fdiff.py,align_diff.py,structdiff.py,diff2.py,
diff3.py,try_variant.py,try_switch.py,ours8080.so,ours8060.so,ours8076.so}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
