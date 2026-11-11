# w11512 (wave 169): `Level::createMission` — local scopes / temporaries /
# sp84+sp88 re-check; `bool alienWave` lead (no raw win; 5,435 / 767 / 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11512`, branch
`wave169-w11512`, base master `00c403d5a`.  Continues
`level-createmission-wave169-w11412.md` (scope/lifetime + convoy-tail sweep)
and `level-createmission-wave166-w11111.md` (frame map).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370
```

## 1. sp+84 / sp+88 pair re-verified — exact, not the residual

Both builds have **17** references to `[sp,#84]` (`raid`) and **2** to
`[sp,#88]` (`__stack_chk_guard`), at *identical* addresses and with
identical registers (`b0024 str.w fp`, `b02ca ldr.w fp`, `b04c0` …
`b0a4a`, `b2a0a`, `b3756`).  There is nothing left in that pair.

## 2. `bool alienWave` — the real source shape, but raw-neutral/worse

The original's station-loop first predicate is **branchless**:

```text
b2e04 subs.w r0, fp, r3        ; i - first
b2e08 mov    r1, r0
b2e0a it     ne
b2e0c movne  r1, #1
b2e0e ldr    r2, [sp, #60]     ; alienWave
b2e10 eor.w  r2, r2, #1        ; !alienWave  <-- only valid for i1
b2e14 orrs   r1, r2
b2e16 bne.w  b2f2a
```

Ours (int `alienWave`) is short-circuit branchy
(`ldr alienWave; cmp; beq; cmp i,first; bne`).  `eor #1` proves the
original's `alienWave` is a **bool** in IR.  Declaring
`bool alienWave = false;` reproduces the branchless form *exactly*
(ours: `subs.w r0,fp,r3; it ne; movne r0,#1; ldr r1,[sp,#60];
eor.w r1,r1,#1; orrs r0,r1; bne`) and is still stored as a 4-byte
`str` at sp+60, as the original is.

It also fixes the flag-initialisation block at `b0f14`: the original
re-stores `alienWave=0`, `pirateFortress=0`, `bigAlienWave=0` after
`rnd->nextInt(100)`; with `int alienWave` clang CSEs two of the three
away (16 B deficit).

### Size matrix (all `link_so -j2` + `fn_diff`)

| shape | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (int alienWave, `end` hack, `(i<0)` hack) | 17172 | **5435** | **767** |
| `bool alienWave` + `end` hack | 17156 | 12403 | 818 |
| `bool alienWave` + faithful bound recompute | 17164 | 12375 | 830 |
| `bool alienWave` + faithful + decl order `AW,BAW,PF` | 17172 | 11469 | 809 |
| `bool alienWave` + `end` hack + default body `1,0,2` | 17172 | 6769 | 840 |
| `bool alienWave` + faithful + default `1,0,2` | 17180 | 6893 | 837 |
| remove `(bigAlienWave + (i < 0))` ternary hack (int) | 17156 | 6544 | 1221 |
| remove `end` hack (int) | 17172 | 5633 | **690** |

The `bool` shape is the *structurally* correct one: with
`default` body order `1,0,2` the convoy switch becomes the original's
4-way shared `str r1,[sp,#380]` tail, the flag-init block matches, and
the instruction count is exactly 6197/6197.  But the compiler then
lays the station-loop predicate out with `movne r0,#1` instead of the
original's `mov r1,r0; movne r1,#1` (2 B shorter), which shifts the
whole `b2e14..b42ec` region by 2 bytes and inflates the same-address
raw diff to 6,769.  No source spelling tried (`first` local as `bool`
or `int`, operand swap, `!(a||b)`, `!!`, `(bool)`) changes that 2-byte
register-allocation choice, and adding the 2 B back at the predicate
would make the function 17,174 B.

## 3. Other sweeps (all neutral, 17172 / 5435 / 767)

`case0`/`case1` body order `0,1,2`; `stationPirates` `unsigned`→`int`;
`fortressGuns` `int`→`unsigned`; `alienGuns`/`fortressGuns` declaration
swap; brace the `hasWanted` increment; `case 2` `cargo`/`crateCaptured`
swap (+1 raw); `!!`/`(bool)`/`==0`/`==1` spellings of both station-loop
predicates; `__builtin_expect` on the predicates (all size-changing and
worse).

`extraAliens` `int`→`unsigned` 5438/768; `alienGuns` `unsigned`
5490/770; moving `fortressGuns` before the supernova block 10835/1277
(size-exact); moving it before `hasWanted` 17188.

## 4. Conclusion

No size-exact shape reduced raw below 5,435.  The `bool alienWave`
finding is the most valuable lead: it is the correct IR type (proved by
the original's `eor #1`), and combined with the `default` body order
`1,0,2` it reproduces the switch tail, the flag-init block and the
instruction count, but the 2-byte `mov r1,r0` select lowering keeps the
same-address raw diff at 6,769.  A future pass that can force that
select copy (or re-balance the 2 B elsewhere) should land a large raw
win.

Tree left clean at the baseline (5,435 / 767 / 17,172 B).
