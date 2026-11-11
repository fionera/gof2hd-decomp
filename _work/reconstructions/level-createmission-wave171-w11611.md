# w11611 (wave 171): `Level::createMission` — `bool alienWave` de-sinks the
# guard `icmp`; exact OR-form guard + insn 6197/6197 (5,435 -> 6,769 raw /
# 767 -> 840 aligned, 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11611`, branch
`wave171-w11611`, base master `50716a763` (wave 170).  Continues
`level-createmission-wave169-w11411.md` and
`level-createmission-wave169-w11511.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. The gate, restated

The station loop's first predicate is `if (i == first && alienWave)`.
With `int alienWave` the post-opt IR is

```text
%1124 = icmp ne i32 %alienWave, 0     ; def block = preheader (shared with alienGuns)
...
%1409 = icmp eq i32 %i, %first        ; def block = loop body
%1410 = and i1 %1124, %1409
br i1 %1410, ...
```

`CodeGenPrepare::SinkCmpExpression` (cgp.cpp:1187) clones `%1124` into the
loop block because its user is there.  Both `and` operands are then
in-block at isel, so `SelectionDAGBuilder::FindMergedConditions` splits
the `and` into the short-circuit form
`ldr alienWave; cmp #0; beq; cmp i,first; bne` (16 B).

## 2. The shape: `bool alienWave`

Declaring `bool alienWave = false;` makes the operand an **i1 phi**
(not a `CmpInst`), so CGP has nothing to sink.  `FindMergedConditions`
then sees operand(0) is not `InBlock`, emits a single `brcond` on the
`and`, and the ARM DAG lowers `and i1 (i==first), alienWave` by De
Morgan as `orrs (zext(i!=first)), (alienWave^1)` — the original's
18-byte guard:

```text
orig: b2e04 subs.w r0, fp, r3
      b2e08 mov    r1, r0
      b2e0a it     ne
      b2e0c movne  r1, #1
      b2e0e ldr    r2, [sp, #60]
      b2e10 eor.w  r2, r2, #1
      b2e14 orrs   r1, r2
      b2e16 bne.w  ...
ours: b2e04 subs.w r0, fp, r3
      b2e08 it     ne
      b2e0a movne  r0, #1
      b2e0c ldr    r1, [sp, #60]
      b2e0e eor.w  r1, r1, #1
      b2e12 orrs   r0, r1
      b2e14 bne.w  ...
```

The only residual is the coalesced `mov r1,r0` (2 B shorter): the
select result and the sub share `r0` in our RA, the original keeps them
in `r1`/`r0`.  No source spelling tried (explicit `i - first == 0`,
`(i==first) & alienWave`, `!(a||b)`, decl/assignment reorders, swapped
outer condition, `< 30`) changes that 2-byte RA choice.

## 3. Size: 17,172 B requires the convoy-default body order 1,0,2

`bool alienWave` alone is 17,156 B: the zero stores of `alienWave` and
`bigAlienWave` sink into the `rnd > 29` arm of the pirate branch
(16 B deficit).  Declaring `pirateFortress`/`bigAlienWave` first
(P,B,a) restores 17,172 B but shifts the flag-init block by 2 B
(raw 11,484).  The size-exact shape that also keeps the flag-init
block is `bool alienWave` **plus** the convoy `switch (k)` default
body store order `offset[1], offset[0], offset[2]` (was `2,0,1`):
that recovers the original's shared 4-way `str` tail and makes the
instruction count exactly **6197/6197**.

## 4. Measured matrix (all `link_so -j2` + `tools/fn_diff.py`)

| shape | size | raw | aligned | insn |
|---|---:|---:|---:|---:|
| HEAD (`int alienWave`) | 17172 | **5435** | **767** | 6204/6197 |
| `bool alienWave` (B,a,P) | 17156 | 12403 | 818 | 6198/6197 |
| `bool alienWave` (P,B,a) | 17172 | 11484 | 798 | 6205/6197 |
| `bool alienWave` + convoy default 1,0,2 | **17172** | 6769 | 840 | **6197/6197** |
| `bool alienWave` + convoy default 0,1,2 | 17148 | 12224 | 878 | 6187/6197 |
| both `alienWave`,`pirateFortress` bool (6 orders) | 17156-17168 | 15656-15968 | 1961-3341 | — |
| all three bool (6 orders) | 17168-17184 | 14777-15954 | 2029-3339 | — |
| `unsigned char alienWave` | 17156 | 12413 | 748 | 6199/6197 |
| `int` + shared `atFirst` i32-or (both guards) | 17220 | 14908 | 1372 | 6212/6197 |

`unsigned char alienWave` has the best *aligned* count (748) but keeps
the split guard (the `icmp ne i8` is still a `CmpInst`); raw 12,413.

## 5. Result

Landed the `bool alienWave` + convoy-default 1,0,2 shape.  The guard is
now the original's branchless OR form (same address `b2e04`, same
operand order, same `subs/eor/orrs`), and the instruction count is
exactly 6197/6197.  Raw rises 5,435 -> 6,769 because the 2-byte
`mov r1,r0` coalescing difference shifts the remaining ~2.6 KB of the
function; aligned 767 -> 840.  Size stays 17,172 B and the gate is
unchanged (byte 4507, linked 4507, size 4524, data 843, imports 207,
lint CLEAN, parity 5370/5370).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11611/`: `Level.cpp.base`, `measure.sh`, `mkll.sh`,
`cmpdis.py`, `run*.py`, `sweep*.py`, `base.so`, `sb_pba.so`, `p1.so`,
`cm.ll`, `cm_bool.ll`, `cgp.cpp`, `sdb.cpp`, `armisel.cpp`,
`mir_base_sel.txt`, `pmi_base.txt` (378 MB).
