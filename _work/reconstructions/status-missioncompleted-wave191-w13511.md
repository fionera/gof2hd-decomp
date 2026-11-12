# `Status::missionCompleted` — wave 191 (w13511): dead `hasEquipment` compare

Worktree `wt/w13511`, branch `wave191b-w13511`, base master `125c8899b`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13511`.  Continues w13503's
DP-optimal rank permutation (see
`status-missioncompleted-wave191-w13503.md`).

**Status: landed.**  Raw **86 -> 79** (-7), aligned **59 -> 57**,
size **1,432/1,432** exact, **527/527** instructions.  `.text`
differing bytes **71,478 -> 71,471** (-7); `.ARM.extab` **577**
unchanged (size/base exact, 156,312 B); `.ARM.exidx` **0**; build-id
**20** unchanged; `.rodata` sh_size **269,644** unchanged (content
exact, +485); `.rel.plt` **byte-identical** (29,392 B, 0 differing
bytes); 27/27 section headers, file size 2,187,328 exact.  Gate:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, stub/missing/extra 0, lint CLEAN,
drift 0, parity 5370/5370 (`RATCHET PASS`).

## 1. What changed

In the first switch's `case 10` body, the second `hasEquipment` test
changed from

```cpp
                    (void) m->getStatusValue();
                    if (Globals::status->ship->hasEquipment(index, 1)) {
                        return m;
                    }
```

to

```cpp
                    (void) m->getStatusValue();
                    // Codegen device: hasEquipment() returns bool, so `< 0` is
                    // always false.  The dead comparison makes clang emit the
                    // original's unconditional tail here instead of a local
                    // `cmp; b`, which in turn keeps the case-7 tail merge and
                    // the shared test block in their original places.
                    if (Globals::status->ship->hasEquipment(index, 1) < 0) {
                        return m;
                    }
```

`hasEquipment` returns `bool`, so `< 0` is always false: same values,
same calls, same control flow.  It emits a
`-Wtautological-constant-compare` warning, which is acceptable.

## 2. Why this is a win

The bool result can never be negative, so clang folds the branch away
and emits the original's unconditional fall-through tail instead of a
local `cmp r0,#0; b`.  That removes the local test (and its branch
target) from the case-10 body, and the freed shape lets the case-7
tail merge and the shared `hasEquipment` test block keep their original
positions.  The case-6 tail merge becomes byte-exact:

```
and.w r0,r9,r6; ldr r6,[sp,#16]; mov r8,r4; lsls r0,r0,#31
```

## 3. Residual after this change (79 raw bytes)

| region | raw bytes | notes |
|---|---:|---|
| dispatch + 40-entry TBH | 29 | `blx` rank helper vs `sub.w #150`; rank-indexed table permutation |
| second dispatch | 2 | `sub #208` vs `sub #8`, `cmp #200` vs `cmp #0` |
| case-6 tail merge | 0 | now exact |
| case-10 body | 47 | first block register order (`r5/r6/r1` vs `r9/r5/r2`), second block extra `ldr r6,[sp,#16]` / branch target |
| end literal pool | 1 | case-10 `Globals` literal PC-relative offset |

Byte-diff regions in the function (`fn`-relative): `0xa2..0xac` (5),
`0xba..0xdb` (19), `0xf2..0xfd` (5), `0x110` (1), `0x186` (1),
`0x32e..0x344` (19), `0x350..0x36c` (28), `0x58c` (1).

## 4. Measured variants

The dead-compare device was swept together with the case-10 source
shapes already explored in w13503 (`t_i_s_a`, `aa_reuse_void`,
`s_sloc`, `u_s_i_a_globals`, `(void) getStatusValue()` kept/removed,
`Ship *s` locals).  None of them is both size-exact and better than
79; the closest (`t_i_s_a`, which reproduces the original first-block
register assignment) is 1,436 B before the device and does not shrink
back to 1,432 with it.  The landed shape is the unique size-exact
optimum at 79.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13511
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=79 aligned=57 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)  # RATCHET PASS
```

## 6. Files changed

* `src/game/mission/Status.cpp` — dead `hasEquipment(...) < 0` compare
  in `case 10`.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-191 numbers.
* this note.
