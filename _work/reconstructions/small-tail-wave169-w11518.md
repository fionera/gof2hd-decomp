# Small tail wave 169 (w11518): Gun / SpaceLounge / Generator re-sweep, no new win

Status: **0 `src/` changes landed**.  Worktree `wt/w11518`, branch
`wave169-w11518`, base master `00c403d5a`.  Scratch `/dev/shm/w11518`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baselines re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1984 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1996 |

(The task text's `_ZN12Generator13createMissionEv` is not the emitted
mangling; the real one is `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`.)

This note extends `small-tail-wave169-w11418.md` / `small-tail-wave168-w11318.md`
and records only shapes not already in those tables.

## Generator::createMission — `int bonus` + `int rem` is the right IR but frame 104

New finding: the *both-non-volatile* shape (`int bonus` + `int rem`) is the
structurally correct one.  It restores the original single predicated store
(`cmp; it ne; subne`) and the original block placement (`bne.w` to the
type==8 costs block placed after `new Mission`), unlike `volatile int rem`
which explodes to 709 raw.  Its only residual is the stack frame: ours is
**104** with 8 scalar slots (20..48) + 8 bytes of padding at sp+16, Strings
at 52/64/76/88, canary 100; the original is **96** with 7 scalar slots
(16..40), Strings at 44/56/68/80, canary 92.  DWARF location lists confirm
`bonus`/`b` share galaxy's slot (SP+20) and `rem` is SSA in r9, so the extra
8 bytes are *not* the two variables the volatile shape dedicates.

| shape | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| baseline (`volatile bonus` + `volatile rem` + `b`) | 127 | 107 | 1996 | 104 |
| `int bonus` + `int rem` (both non-volatile) | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem`, `difficulty = 0` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem`, difficulty decl moved early | 181 | 116 | 2000 | 104 |
| `int bonus = 0` + `int rem` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem = 0; rem = rawBonus % 50` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem`, `bonus` used in place of `b` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem` + `volatile int *remp = &rem` | 162 | 97 | 2000 | 104 |
| `int bonus` + `volatile int rem` (block placement explodes) | 709 | 89 | 2000 | **96** |
| `int bonus` + `int rem`, `__builtin_expect(type == 8, 0)` | 1834 | 405 | 2020 | — |
| `int bonus` + `int rem`, `__builtin_expect(type == 8, 1)` | 1415 | 473 | 2008 | — |
| `volatile bonus` moved before `Galaxy *galaxy` | 127 | 107 | 1996 | 104 |
| `volatile bonus` before reward | 127 | 107 | 1996 | 104 |

The both-non-volatile IR is the right one, but the +8-byte frame is a
stack-colouring fixed point: removing `rem`/`b`/`bonus` as source variables
does not change it.  Raw-first keeps master's `127/107/1996`.

## SpaceLounge::drawLounge — clamp reassociation re-confirmed

Re-ran the `right` local and `pad2` shapes with all other TUs at baseline
(a non-baseline Gun/Generator size shifts SpaceLounge's address and inflates
raw via literal-pool PC-relative words — a measurement trap worth noting).

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 175 | 41 | 1948 |
| `const int pad` | 175 | 41 | 1948 |
| `top` before `centerX` | 175 | 41 | 1948 |
| `pad` via `Layout *layout0` local | 175 | 41 | 1948 |
| `top` uses `pad * 2` | 175 | 41 | 1948 |
| `int right = w - inset;` + `right - (width+pad+pad)` | 176 | 42 | 1948 |
| `int right` + `right - width - pad - pad` | 468 | 64 | 1944 |
| `int pad2 = pad+pad`; top uses pad2; clamp `right-(width+pad2)` | 445 | 54 | 1948 |
| `int pad2 = pad*2`; top uses pad2; clamp `right-(width+pad2)` | 445 | 54 | 1948 |

The `right` local still reassociates to `right + (-2*pad - width)`; the
`pad2` shapes do recover `add r1,r6,sl; sub r5,r0,r1` but the extra live
value cascades through the whole function.  Master keeps `175/41`.

## Gun::calcCharacterCollision — loop rotation is block placement, not source

The raw 463 is dominated by two instruction-count shifts (the scatter `k`
loop, 158+42 raw bytes, and the `Matrix view` address rematerialisation,
40 raw bytes), not by the r4/r5 index tie.  The original `k` loop is
bottom-tested (`movs r5,#0; b.n test; body; adds r5,#1; test; bcc body`);
ours is top-tested.  New shapes (all size-exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 463 | 57 | 1984 |
| `k != size` | 463 | 57 | 1984 |
| `int k` with `(int) size` | 463 | 57 | 1984 |
| `k++` | 463 | 57 | 1984 |
| `Matrix *viewPtr = &view`, use `*viewPtr` | 463 | 57 | 1984 |
| `Matrix &viewRef = view`, use `viewRef` | 463 | 57 | 1984 |
| `unsigned nk = size; for (k<nk)` | 1715 | 266 | 1960 |
| `while (k < size)` | 1748 | 233 | 1940 |
| `do { } while (k < size)` (guarded) | 1892 | 335 | 1872 |
| drop `__builtin_expect(distance < magnitude, 0)` | 1886 | 257 | 1908 |
| drop the mine `__builtin_expect` | 673 | 154 | 1984 |

The loop is rotated by MachineBlockPlacement, not by the source form; every
`do`/`while` rewrite changes size.  Master keeps `463/57`.

## Conclusion

No size-exact raw win was found.  Tree left clean at the wave-169 baseline
(`byte 4507`, `linked 4507`).
