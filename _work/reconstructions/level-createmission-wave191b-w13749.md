# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13749

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13749`, base master `4c4d81cf7`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13734.md` (note-only, raw 2,626),
`...w13715.md`, `...w13702.md`, `...w13691.md`.  Task: test the
return-relocation hypothesis left open by w13734, then chase the case-4-tail
`strd`/`fp=1` register-allocation delta and the no-alias `Globals::rnd`
shape.  **No size-exact raw win was found; note-only, `src/` unchanged.**

```text
                                     size            raw    aligned  insn
base 4c4d81cf7 (checked in)          17172/17172     2626    864     6206/6197
```

`check` on the unchanged tree: byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `RATCHET PASS`.  `.text` diff **72,954**, `.ARM.extab`
**564**, `.ARM.exidx` 0, `.rodata` 0, `.rel.plt` 0 (all other sections 0).
Function ends at `0xb42ec`; literal pool is exactly 6 words
(`b42d4` kBlueprintRaces, `b42d8` kBlueprintPositions, `b42dc`
kBlueprintSpacePoints, `b42e0` gameText, `b42e4` Globals::options, `b42e8`
Globals::globals).

## 1. Return relocation (w13734's strongest untested hypothesis) — costs +24

w13734 proposed moving the `return;` from after `case4Tail` (line 2143) to
immediately after the `nameLeader` if/else (line 2119), adding a `return;` at
the end of `case4Tail`.  The binary really does have **two** returns in this
region:

```text
orig  b408c cmp r0,#0
      b408e beq.w b02ca        ; !nameLeader -> epilogue
      b4092..b40ae assign name
      b40b2 b.w b02ca          ; nameLeader  -> epilogue (the missing return)
      b40b6 ldr.w r8,[sp,#124] ; case4Else reloads `this` into r8
      ...
      b40f2 case4Tail entry (direct from the case-4 dispatch)
      ...
      b41ec b.w b02ca          ; case4Tail epilogue
```

Base emits only the `beq.w`; the `b.w b02ca` at `b40b2` is missing, and base's
`case4Else` keeps `this` in `sl` instead of reloading it into `r8`.

Every source shape that emits the second `return` re-orders the cold blocks:
`case4Tail` jumps from `b40f2` (right after `case4Else`, as in the binary) to
`b1e6e`, and the `nameLeader` block moves from `b408c` to `b418e`.  The extra
branch itself is 4 B; the re-ordering costs 24 B.

| variant (all on base) | raw | aligned | size |
|---|---:|---:|---:|
| **base (`return;` only after case4Tail)** | **2,626** | **864** | **17,172** |
| move return up + add return in case4Tail (`retreloc`) | 11,832 | 1,829 | 17,196 |
| add return after if/else, keep tail return (`ret_after`) | 11,832 | 1,829 | 17,196 |
| `if (nameLeader){assign; return;} else {return;}` (`bothret`) | 11,832 | 1,829 | 17,196 |
| `if/else` + trailing return, no `else` braces (`elseifret`) | 11,832 | 1,829 | 17,196 |
| `if (nameLeader){assign; return;}` fall-through (`ifret_else`) | 2,769 | 865 | 17,172 |
| `if (!nameLeader) return; assign;` (`notret_assign`) | **2,626** | **864** | **17,172** |

`ifret_else` is the only other size-exact shape that emits the `b.w b02ca`
(at `b40ae`, because `cbz` replaces `cmp`+`beq.w`), but it is semantically
wrong (`!nameLeader` then falls into `case4Else`) and raw +143.  `notret_assign`
is byte-identical to base.  **The return relocation is not landable.**

## 2. The case-4-tail `strd` / `fp=1` delta

The binary's loop body stores the `createShip` stack args with a `strd`:

```text
orig  b415c ldr r0,[pc,#392]   ; &Globals::globals
      b415e mov.w fp,#1        ; `true` hoisted into fp
      b4162 mov.w r9,#0        ; loop i
      b4168 ldr.w sl,[r0]      ; sl = Globals::globals
      ...
      b4198 strd r0, fp, [sp]  ; waypoint, true
      b419c movs r0,#0
      b419e str r0, [sp,#8]    ; false
ours  b4158 mov.w r8,#0        ; loop i
      b415c ldr.w fp,[sl,#248] ; fp = this->enemies
      ...
      b4198 str r0,[sp,#0]
      b419a movs r0,#1
      b419c str r0,[sp,#4]
      b419e movs r0,#0
      b41a0 str r0,[sp,#8]
```

`strd` is 4 B; the `str`+`movs`+`str` triple is 8 B.  The binary keeps
`true` in `fp` across the loop and uses `sl` for `Globals::globals`; ours
keeps `this` in `sl`, `Globals::globals` in `r9`, and re-materialises `true`
each iteration.  The root cause is the whole-function register assignment of
`this`: the binary reloads `this` into `r8` at `case4Else` (`ldr.w r8,[sp,#124]`)
and keeps it in `r8` through `case4Tail`, freeing `sl` for `globals`; ours
keeps `this` in `sl` throughout.  This is the LLVM-7 `StackSlotColoring` /
greedy-RA weight-order artefact recorded in
`level-createmission-90-7-17172-b_w3815.md`.

Removing the two local aliases (`Globals *globals`, `Array<KIPlayer *> *enemies`)
*does* recover the `strd` and `fp=1`, but at a size cost:

| variant | raw | aligned | size | `strd r0,fp,[sp]` |
|---|---:|---:|---:|---|
| **base (both aliases, cast)** | **2,626** | **864** | **17,172** | no |
| no aliases, cast (`g0e0c`) | 2,785 | 857 | 17,168 (−4) | **yes** |
| no aliases, `Globals::rnd` (`g0e0r`) | **2,569** | 866 | 17,176 (+4) | no |
| enemies alias only, cast (`g0e1c`) | 2,732 | 857 | 17,172 | — |
| globals alias only, cast (`g1e0c`) | 2,782 | 855 | 17,168 | — |
| enemies alias only, `rnd` (`g0e1r`) | 2,824 | 882 | 17,184 | — |
| globals alias only, `rnd` (`g1e0r`) | 2,767 | 869 | 17,180 | — |
| both aliases, `rnd` (`g1e1r`) | 2,825 | 886 | 17,184 | — |

`g0e0r` is 57 raw better but +4 size because `Globals::rnd` needs a **fourth
literal-pool word** (the binary's pool has only 3 data words and therefore
uses the `((AERandom *) Globals::globals)` cast, exactly as base does).
`g0e0c` is −4 size and has the `strd`, but its register assignment is worse
(`globals` in `r9`, loop in `r8`, `this` in `sl`), so raw rises by 159.

New combinations on `g0e0c`:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `g0e0c` + return relocation | 11,767 | 1,823 | 17,204 |
| `g0e0c` + `if (!nameLeader) return;` | 2,785 | 857 | 17,168 |
| `g0e0c` + return after if, keep tail return | 11,767 | 1,823 | 17,204 |

No combination reaches 17,172 with raw < 2,626.

## 3. Other case-4-tail spellings (all size-exact, none a win)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **base** | **2,626** | **864** | 17,172 |
| `bool alwaysEnemy = true;` before loop | 2,626 | 864 | 17,172 |
| `const bool alwaysEnemy/neverAttack` before loop | 2,626 | 864 | 17,172 |
| `unsigned i; for (i = 0; ...)` | 2,626 | 864 | 17,172 |
| reversed alias declaration order | 2,782 | 878 | 17,176 |
| `for (int i ...)` | 2,627 | 864 | 17,172 |
| `float opt44` declared after `hunters` | 2,629 | 858 | 17,172 |
| `float opt44` inlined into `hunterCount` | 2,628 | 864 | 17,172 |
| `__builtin_expect` removed | 2,627 | 865 | 17,172 |
| `__builtin_expect(roll == 0, 0)` | 2,635 | 865 | 17,172 |
| `Level *self = this;` in `case4Else` | 2,626 | 864 | 17,172 |
| `Vector pos = stationPosition` in `case4Else` | 2,626 | 872 | 17,172 |
| `px/py/pz` locals in `case4Else` | 2,626 | 872 | 17,172 |
| `(unsigned) enemies->size()` | 2,626 | 864 | 17,172 |
| `goto done` label after `case4Tail` | 11,832 | 1,829 | 17,196 |
| `unsigned n = enemies->size()` cache | 2,656 | 866 | 17,172 |
| `hunterCount` unsigned | 2,626 | 864 | 17,172 |
| `hunterCount` float | 3,368 | 880 | 17,176 |
| `float opt44` variable | 2,830 | 912 | 17,192 |
| `Level *volatile self = this;` | 2,827 | 897 | 17,184 |
| `Waypoint *wp` local in loop | 2,772 | 908 | 17,172 |
| `++i` | 2,626 | 864 | 17,172 |

## 4. Conclusion

The checked-in shape remains the unique raw minimum at size 17,172
(**2,626 / 864 / 17,172**, insn 6206/6197).  The two structural deltas are
(a) the missing `b.w b02ca` at `b40b2` and (b) the missing `strd r0,fp,[sp]`
at `b4198`; both are consequences of the whole-function register assignment
of `this` (`sl` vs `r8`) and cannot be recovered by any local spelling tried
without paying ≥ 4 B of size or ≥ 143 raw.  `src/` is left unchanged.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13749
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2626 aligned=864 size=17172/17172 insn=6206/6197`,
`.text` diff 72,954, `.ARM.extab` 564, `.ARM.exidx` 0, `.rodata` 0,
`RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13749/`
(`buildvar.py`, `try2.py`, `try3.py`, `try_combo.py`, `try_retreloc.py`,
`adiff.py`, `clusters.py`, `Level.base.cpp`, `base.so`, `retreloc.so`,
`g0e0c.so`, `g0e0r.so`, `ifret_else.so`, `bothret.so`, `notret_assign.so`,
`elseifret.so`, `revorder.so`, `boolvar.so`, `boolvar2.so`, `decl_i.so`).
