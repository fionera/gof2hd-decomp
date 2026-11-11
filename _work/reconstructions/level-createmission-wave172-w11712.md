# w11712 (wave 172): `Level::createMission` — scope/lifetime, type and
# convoy-switch sweeps; no raw win (5,425 / 936 / 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11712`, branch
`wave172-w11712`, base master `41ef4c8b6` (wave 171).  Continues
`level-createmission-wave170-w11612.md` (drop cached `Status*`, 5,435 -> 5,425)
and `level-createmission-wave169-w11512.md` (scope/lifetime + sp84/sp88).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5425   align 936   insn 6204/6197
```

## Where the 5,425 raw bytes actually are

The raw diff is dominated by **one 2-byte phase shift**, not by 5,425 independent
deltas.  The two streams are byte-in-phase (`Uoff - Ooff == 0`) up to `0x146e`;
the only difference before that is 191 bytes of regalloc/slot-colour noise.
At `0xb1450` ours emits one extra `str r1,[sp,#376]` (the convoy-switch
default body) and the whole tail of the function is then 2 bytes out of phase
until local compensations.  The `difflib` opcode map shows the shift profile
(+0 -> +10 -> +2 -> ... -> 0) with `replace O334c U334c (117/1)` being the
pirateFortress loop laid out at a different block position.

The extra 2 B is the `b.n` that skips the default case: in the original the
switch tail `str r1,[sp,#380]` is *after* the default body (default falls
through), in ours the shared tail is *before* the default and needs a branch.
Making the default share the tail requires source store order `offset[1],
offset[0], offset[2]`, but that makes LICM hoist the four `offset[2]` constants
into `s16..s22` (`vmov.f32` x4 = +16 B, tail becomes `vstr s0`).  This is the
known w11612 trade-off; the landed `2,0,1` default order is the only
size-exact shape.

## Sweeps measured this wave (all size-exact unless noted)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD | 17172 | **5425** | **936** |
| convoy default store order `0,1,2` | 17164 | 11334 | 1007 |
| convoy default store order `0,2,1` | 17172 | 5431 | 936 |
| convoy default store order `1,0,2` | 17188 | 11326 | 975 |
| convoy default store order `1,2,0` | 17172 | 5432 | 931 |
| convoy default store order `2,1,0` | 17172 | 5428 | 933 |
| default-first switch + order `1,0,2` | 17164 | 11331 | 1007 |
| `case 3:` instead of `default:` + order `1,0,2` | 17188 | 11330 | 836 |
| `Vector offset` + `.y/.x/.z` order `1,0,2` | 17168 | 5584 | 1022 |
| `for (int i=10;i<14;i++) switch (i)` cases 10-12 | 17172 | 7284 | 1027 |
| `switch (i - 10)` | 17172 | 5425 | 936 |
| swap source order `case 0`/`case 1` | 17172 | 5431 | 938 |
| swap source order `case 1`/`case 2` | 17172 | 5425 | 936 |
| `guns` declared/assigned after the raidRace==8 if | 17172 | 6367 | 1380 |
| `int guns;` before, assignment after the if | 17172 | 6367 | 1380 |
| outer `guns` hoisted next to `fortressGuns` | 17168 | 7417 | 1878 |
| `unsigned blueprint` | 17172 | 5425 | 936 |
| `unsigned extraAliens` | 17172 | 5428 | 937 |
| `unsigned fortressGuns` | 17172 | 5425 | 936 |
| `unsigned pirates` | 17172 | 5425 | 936 |
| `unsigned convoy` | 17172 | 5425 | 936 |
| `unsigned pirateEscorts` | 17172 | 5426 | 937 |
| `unsigned security` | 17172 | 5450 | 1065 |
| `unsigned extraFriend` | 17172 | 5425 | 936 |
| `unsigned raidRoll` | 17172 | 5426 | 938 |
| `unsigned localRace` | 17172 | 5425 | 936 |
| `unsigned raidRace` | 17172 | 5425 | 936 |
| `unsigned alienPercent` | 17172 | 6020 | 966 |
| `bigAlienWave` truthy (`bigAlienWave ? 8 : 7`) | 17172 | 5428 | 939 |
| `fortressGuns = pirateFortress * 5` | 17148 | 7459 | 1370 |
| `alienGuns = alienWave * (...)` | 17172 | 12511 | 1147 |

## sp84 / sp88 pair re-checked

`raid` = `[sp,#84]`: 17 references in both, identical addresses/registers.
`__stack_chk_guard` = `[sp,#88]`: 2 references in both.  Nothing left there.

## Spill-slot rotation (the remaining colour delta)

The three long-lived locals `blueprint`, `extraAliens`, `fortressGuns` are
coloured to different stack slots:

```text
             original   ours
blueprint      sp+48    sp+44   (first store 0x06dc)
extraAliens    sp+44    sp+40   (first store 0x0870)
fortressGuns   sp+40    sp+48   (first store 0x2a86)
```

Definition order is identical in both (blueprint, extraAliens, fortressGuns),
so this is PEI spill-slot index assignment, not source declaration order.
Type changes (`int`/`unsigned`) on all three are neutral or worse; declaration
moves that change the live-range start are size-changing.  No source knob tried
rotates the three slots.

## Result

No size-exact shape reduced raw below 5,425.  Tree left clean at the baseline
(5,425 / 936 / 17,172 B).  The one actionable lead is still the convoy-switch
default tail: if the `offset[2]` phi can be kept in a GPR (original uses
`str r1`, ours uses `vstr s0` after LICM hoists four `vmov.f32`), the 2-byte
phase shift disappears and raw should collapse.
