# MGame::OnTouchEnd — wave 151 (w9713): the mining guard `cmp #0` is an LSR-flip, not a spelling problem

Status: analysis note, **no source change landed**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9713`, base `db9658a51` (wave 150 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w9713`.
Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 150) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4392 | **4392** |
| aligned (align_diff) | 904 | **904** |
| guard | `cmp.w r9,#2 ; bcc` | unchanged |

## 1. The guard and the LSR flip

Original `0x17a5ca`:

```
17a5ca: f1b9 0f00   cmp.w r9, #0
17a5ce: d040        beq.n 17a652
```

Landed base (`amount != 0 && amount != 1`):

```
17a5ca: f1b9 0f02   cmp.w r9, #2
17a5ce: d340        bcc.n 17a652
```

The mission suggested `int n = amount; if (n != 0)`, a `do/while`, or
`amount >= 1`.  All three were measured on the exact wave-150 base and all
re-lay the whole function to **8060 B**:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `int n = amount; int count = -n; if (n != 0)` | 8060 | 6590 | 1835 |
| `if (amount != 0)` | 8060 | 6590 | 1835 |
| `if (count != 0)` | 8060 | 6590 | 1835 |
| `if (amount >= 1)` | 8060 | 6026 | 1034 |
| `if (amount >= 0)` / `if (amount > -1)` | 8060 | 6025 | 1033 |
| `if (!amount)` | 8076 | 6528 | 2058 |
| `if (amount == 0) {} else { ... }` | 8060 | 6590 | 1835 |
| `switch (amount) { case 0: break; default: ... }` | 8060 | 6590 | 1835 |
| `do { if (amount == 0) break; ... } while (0)` | 8060 | 6590 | 1835 |
| `while (amount != 0) { ...; break; }` | 8060 | 6590 | 1835 |
| `goto` guard / ternary | 8060 | 6590 | 1835 |
| `(long long)amount != 0` / `(unsigned long long)amount != 0` | 8060 | 6590 | 1835 |
| `unsigned amount` (all spellings) | 8060 | 6590 | 1835 |
| `short amount` | 8032 | 7510 | 2295 |
| `long long amount` | 8064 | 7222 | 2391 |

The 8060 layout is the **LSR flip**: `LoopStrengthReduce` sees the equality
`icmp eq amount, 0`, creates an `LSRUse::ICmpZero` on `-amount`, and rewrites
the loop induction variable from the decrementing `amount` (`sub.w r9,r9,#1`)
to the incrementing count.  The flipped codegen is

```
17a5ca: f8dd 9018   ldr.w r9, [sp, #24]
17a5ce: f1b8 0f00   cmp.w r8, #0
17a5d2: d03d        beq.n 17a650
```

i.e. it *does* contain `cmp #0; beq`, but on the count register and with an
extra reload, and the rest of the function is re-laid.  The negate is not the
driver: a diagnostic variant with `String(0)` instead of `String(count)` still
flips to 8060.  The `ICmpZero` use alone is enough.

The landed `amount != 0 && amount != 1` works because InstCombine folds it to
`icmp ugt amount, 1` (non-equality), so LSR does not create an `ICmpZero` use
and the decrementing IV survives.  Every non-equality spelling that is
equivalent for `amount <= 0` canonicalises to the same `ugt amount, 1` and
therefore reproduces the landed 4392/904:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `(amount >> 1) != 0` | 8080 | 4392 | 904 |
| `(amount & -2) != 0` | 8080 | 4392 | 904 |
| `(amount \| 1) != 1` | 8080 | 4392 | 904 |
| `(amount & 0xfffffffe) != 0` | 8080 | 4392 | 904 |
| `amount != 0 && amount != 1` (landed) | 8080 | 4392 | 904 |
| `amount != 1 && amount != 0` | 8080 | 4392 | 904 |
| `!(amount == 0 \|\| amount == 1)` | 8080 | 4392 | 904 |
| `switch { case 0: break; case 1: break; default: ... }` | 8080 | 4392 | 904 |

`amount != 0 && amount != 2` / `!= -1` / `!= INT_MIN` give 8084/8084/8084
(`add r0,r9,#1; cmp r0,#2`), and `amount != 0 && amount != 3` gives 8096.

## 2. Shapes that keep the decrementing IV but do not emit `cmp #0`

A bit-test on the low half keeps the decrementing IV and emits `beq`:

```cpp
if ((amount & 0x8000) != 0) { ... }
```

```
17a5ca: f419 4f00   tst.w r9, #32768
17a5ce: d040        beq.n 17a652
```

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `(amount & 0x8000) != 0` | 8080 | **4393** | **903** |
| `(amount & 0x8000) == 0x8000` | 8080 | 4393 | 903 |
| `(amount & 0x8000) > 0` / `>= 1` | 8080 | 4393 | 903 |
| `(amount & 0x80000000) != 0` | 8080 | 4393 | 904 (`cmp #-1; bgt`) |
| `(amount & 0xe0000000) != 0` | 8080 | 4392 | 904 (`cmp #0x20000000; bcc`) |
| `(amount & 0xff000000) != 0` | 8080 | 4393 | 904 (`cmp #0x1000000; bcc`) |

`(amount & 0x8000) != 0` is the only 8080 shape that has the original's
`beq` branch and the original's decrementing `r9`, but it trades one raw byte
(3 differing bytes in the 4-byte test vs 2 in the base) for one aligned
instruction.  It is also only equivalent to `amount != 0` for
`|amount| < 32768`, so it is not semantically exact.  It was not landed.

## 3. Other new hypotheses (all re-lay or regress)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `__builtin_expect(amount != 0, 0)` | 8076 | 7353 | 2347 |
| `__builtin_expect(amount != 0, 1)` | 8004 | 7474 | 2341 |
| `__builtin_expect(amount != 0 && amount != 1, 0)` | 8076 | 7338 | 1474 |
| `__builtin_expect(amount != 0 && amount != 1, 1)` | 8028 | 7511 | 2289 |
| `__builtin_expect(ship->hasCargo(...), 1)` + `expect(amount != 0, 1)` | 8080 | 5244 | 2063 |
| `volatile int n = amount; if (n != 0)` | 8064 | 7309 | 2512 |
| `int n = amount; int *p = &n; if (*p != 0)` | 8060 | 6590 | 1835 |
| `__sync_synchronize()` copy | 8064 | 4714 | 1864 |
| `switch { case 0: break; case 1: body; default: body; }` | 8044 | 7510 | 2246 |
| `switch { case 0: break; case -1: body; default: body; }` | 8044 | 7510 | 2246 |
| `switch { case 0: break; case -1: break; default: body; }` | 8064 | 4665 | 976 |
| `if (amount > 0) __builtin_unreachable(); if (amount < 0)` | 8080 | 4393 | 904 |
| `(short)amount != 0` | 8028 | 7284 | 2281 (`movw/tst`) |
| `(amount & 0x7fffffff) != 0` | 8032 | 7525 | 2294 (`ldr/tst`) |
| `(amount << 16) != 0` | 8028 | 7284 | 2281 (`lsls`) |
| `(amount & 0xffff) != 0` | 8028 | 7284 | 2281 (`movw/tst`) |

## 4. Conclusion

There is no source spelling that both keeps the decrementing IV and emits
`cmp #0` in this function: `cmp #0` is only produced by an equality `icmp`,
and every equality `icmp` on the IV is rewritten by LSR's `ICmpZero` path.
The landed `amount != 0 && amount != 1` is the best exact-semantics shape
(4392/904).  The only 8080 non-flip alternative is the inexact bit-test
`(amount & 0x8000) != 0` (4393/903), which trades raw for aligned.

## 5. Reproduction

```
cp /dev/shm/w9713/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9713/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4392 / 8080
python3 /dev/shm/w9713/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 904
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9713/{fdiff.py,align_diff.py,sweep*.py,gen.py,
measure.sh,ir.sh,irpass.sh,MGame.base.cpp,ne0.afterall,lsr7.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
