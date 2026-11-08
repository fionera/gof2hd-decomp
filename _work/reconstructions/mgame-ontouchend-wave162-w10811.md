# MGame::OnTouchEnd — wave 162 (w10811): `hasWon()` returns `bool` (the `won` block becomes exact)

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10811`, base `29248c1d0` (wave-161 final).
Scratch `/dev/shm/w10811`. Target `_ZN5MGame10OnTouchEndEiiPv`,
original 8080 B @ 0x17a144.

| | base (wave 161) | this wave |
|---|---:|---:|
| size | 8080 | **8080** (exact) |
| raw same-address diff | 4127 | **6053** (+1926) |
| aligned (`align_diff.py`) | 906 | **1085** (+179) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |
| `.text` diff | 85,374 | **87,300** (+1,926) |
| `.ARM.extab` diff | 725 | **761** (+36) |

Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS.  Section sizes all exact; `.rodata`/`.data`/`.bss`/`.ARM.exidx`
byte-identical.

## 1. The finding: `Mission::hasWon()` returns `bool`, not `unsigned char`

The original's `won` guard at `0x17adda` is `cmp r0,#0; beq.w 17b0aa` with
body fall-through and the else block placed **after** the body (at 0x17b0aa).
That is the layout of a **50:50** branch whose condition is a direct i1 test.
`calcZeroHeuristics` (LLVM 7 `BranchProbabilityInfo.cpp:707`) gives every
`icmp eq/ne i8 X, 0` the 20:12 unlikely weight, which re-lays the function to
8064 B — this is exactly the truthy-form failure w10711 documented.

The only static escape from the zero heuristic is the AND-power-of-2 exception
(`bpi.cpp:669-675`), which lowers to `lsls r0,#31` and inverts the block order
(`resetStartSequenceOver` moves from idx 1362 to 1117, rotating four
`.rel.plt` slots).  But if `hasWon()` returns **`bool`**, clang emits a branch
on the i1 directly — no `ICmpInst`, so no zero heuristic, weight 50:50 — and
the far branch becomes `cmp r0,#0; beq.w` exactly as in the original.

Evidence for `bool`:
* `Mission::hasWon` is 4 bytes (`ldrb r0,[r0,#5]; bx lr`), which requires the
  `won` field to be `bool` (a `uint8_t` field + `bool` return normalises with
  `cmp/it/movne` and is 10 bytes).
* Every caller in the original tests the result with `cmp r0,#0` / `cbz` /
  `cbnz`, never `cmp #1`.
* `Mission *mission = hasWon() ? ...` in the original is `cbz r1, ...`
  (0x17af70).

With `bool won; bool hasWon();` the `won` block becomes byte-identical to the
original (modulo the 4-byte shift discussed below):

```
17adc4: blx hasWon
17adc8: cbnz r0, 17addc
17adca: ldr r0,[r8]
17adce: blx getCampaignMission
17add2: blx hasWon
17add6: cmp r0,#0
17add8: beq.w 17b0aa        <- original target 0x17b0aa, exact
17addc: body
```

and `keepObjectives` (`cmp r0,#0; beq.w`) and the `mission = hasWon() ? ...`
selector (`cbz r1`) also match.

## 2. Why the raw metric regressed

The `bool` change is correct for the `won` block but perturbs the register
allocator in the *mining* block: the `add.w r8, sp, #80` that computes a
String temporary address is hoisted from `0x17a5f6` (original) to `0x17a5e8`,
a **4-byte shift** for the rest of the function.  Even compensating the shift
(best alignment offset +4) the raw diff is 6008, so the regression is not only
the shift; the bool type also changes `cbz`/`cmp` choices in later blocks
(the aligned categories grow `extra-orig:movs` 68 and `extra-orig:ldr` 61,
i.e. the original keeps instructions our bool build folds away).

This is a genuine structural milestone (the guard instruction and weight now
match) but it is **not** a raw win.  The coordinator may prefer to keep this
as a separate pass and re-derive the mining-block scheduling.

## 3. Measured alternatives (all size 8080 unless noted)

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| base `B == true` (unsigned char) | 4127 | 906 | 8080 | **0** |
| **bool `hasWon` + `B == true` (landed)** | **6053** | **1085** | **8080** | **0** |
| truthy `A \|\| B` | 6066 | 1908 | 8064 | 0 |
| `A \|\| (B & 1)` (AND) | **4107** | 907 | 8080 | 9 |
| `!( !A && !(B&1) )` | 4107 | — | 8080 | 9 |
| `bool a=A; bool b=B; won=a\|\|b` | 4712 | — | 8080 | 4 |
| `bool b=B; won=A\|\|b` | 4693 | — | 8080 | 4 |
| `B \|\| A` (reordered) | 6002 | — | 8072 | 0 |
| `A \|\| expect(B,1)` | 6066 | — | 8064 | 0 |
| `expect(A\|\|B,1)` | 4790 | — | 8088 | 18 |
| `A \|\| expect(B==1,1)` | 4127 | — | 8080 | 0 |

`__builtin_expect_with_probability` is **not available** in NDK r18b clang
7.0.2 (undeclared identifier), so an arbitrary 50:50 weight cannot be set
from source.

The AND form (`A || (B & 1)`) remains the best raw variant (4107, -20) but
rotates the four `.rel.plt` slots `useCloak` / `maneuverTouchEnd` /
`isInstantActionMission` / `resetStartSequenceOver` (resetStartSequenceOver
first); it is not landable under the `.rel.plt`-byte-identical invariant.

## 4. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10811/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 6053 / 8080
python3 /dev/shm/w10811/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 1085
python3 /dev/shm/w10811/relplt.py                                # diff 0
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10811/{fdiff.py,align_diff.py,relplt.py,sweep*.py,
v1..v11.py,base.so,bool.so,truthy.so,and.so,MGame.base.cpp}`.
