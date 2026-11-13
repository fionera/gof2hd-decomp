# MGame::OnUpdate — level-load / r9-anchor sweep: no size-exact win (raw 16 / aligned 6), wave 191b / w13781

Status: **note only, no `src/` change.**  Worktree `wt/w13781`, branch
`wave191b-w13781`, base master `5968bc1ac`.  The landed w13713 body is
re-measured and confirmed at **raw 16 / aligned 6 / insn 4002:4002**, size
**11,440/11,440** exact, `.text` **74,651**, `.ARM.extab` **562**,
`.ARM.exidx` **0**, `.rodata` **0**.  Every size-exact source shape tried this
wave reproduces the same 16 raw bytes; every shape that fixes the level-load
position drops the function to 11,392 (or grows it to 11,452/11,456).
Nothing was committed to `src/`.

Follows `mgame-onupdate-wave191b-w13713.md` (the landed anchor),
`mgame-onupdate-last16-wave163-w10919.md`,
`mgame-onupdate-wave191b-w13654.md`.

## 1. Residual

All 16 raw bytes are three runs inside one basic block (the `>= 122 && <= 124`
branch body).  `tools/fn_diff.py --json` reports 6 aligned categories:
`extra-ours:ldr.w` 1, `extra-orig:ldr.w` 1, `regalloc:movw` 1,
`regalloc:movweq` 1, `opcode:r2,/mov` 1, `opcode:mov/adds` 1.

Original `0x17f182`:

```asm
blx   nextInt
mov   r4, r0
ldr.w r0, [r9]              ; Globals::status
blx   getCurrentCampaignMission
movw  r2, #3166
ldr.w r1, [fp, #120]        ; this->level, loaded AFTER ccm
cmp   r0, #122
it    eq
movweq r2, #3164
add   r2, r4
mov   r0, r1
movs  r1, #28
blx   createRadioMessage
b.w   17e26c                ; shared getPlayingTime/strd tail
```

Ours (HEAD, w13713 shape):

```asm
blx   nextInt
mov   r4, r0
ldr.w r0, [r9]
ldr.w r5, [fp, #120]        ; this->level hoisted BEFORE ccm
blx   getCurrentCampaignMission
movw  r1, #3166
cmp   r0, #122
it    eq
movweq r1, #3164
mov   r0, r5
adds  r2, r1, r4
movs  r1, #28
blx   createRadioMessage
b.w   17e26c
```

The `Level *level` local is live across the opaque `getCurrentCampaignMission`
call, so clang must keep it in a callee-saved register (r5) and hoists the
load above the call; the base constant then lands in r1 and the sum needs the
3-operand `adds r2, r1, r4`.  The original instead has the load *after* the
call, in caller-saved r1 (`ldr.w r1, [fp, #120]` + `mov r0, r1`), the base in
r2 and the 2-operand `add r2, r4`.

## 2. Why the obvious fix does not work

The original IR order must be `nextInt` -> `ccm` -> `load this->level` ->
`createRadioMessage`, i.e. the `ccm` call sits in its own statement *before*
the `createRadioMessage` statement (clang emits the member-call object
expression first, so inlining the ternary into the argument list still loads
`this->level` before `ccm`).  That source shape is:

```cpp
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
this->level->createRadioMessage(28, radioIndex + radioBase);
```

It reproduces the original block *exactly* — `movw r2,#3166`,
`ldr.w r1,[fp,#120]`, `cmp`, `it eq`, `movweq r2,#3164`, `add r2,r4`,
`mov r0,r1`, `movs r1,#28` — but the whole function re-allocates: the GOT
base of `@Globals::status` moves from r9 to r5, and the shared tail at
`0x17e26c` can no longer be entered (`ldr.w r2,[r9]` is r9-specific), so the
branch inlines `ldr r0,[r5]; blx getPlayingTime; ldr r2,[r5]` and jumps into
the middle of the tail.  Result: **11,392 bytes** (-48), raw 5,196.

This is a register-allocation attractor flip, not a local scheduling issue:
`RAGreedy` processes intervals by spill weight, so one IR-level load moving
changes the queue order and re-colours the whole 11 KB function.  The w13713
`Level *level` local happens to be the shape that lands on the *original's*
attractor (r9 = `&Globals::status`, r5 = scratch throughout the 122..124
chain); the original's own shape lands on a different one.

## 3. Variants measured this wave

All builds `-j2` under `ulimit -v 12000000`; raw/aligned from
`tools/fn_diff.py`, `.text`/`.extab` are section byte diffs vs the original.

| variant | raw | aligned | size | `.text` | `.extab` | note |
|---|---:|---:|---:|---:|---:|---|
| **HEAD (w13713)** | **16** | **6** | **11,440** | **74,651** | **562** | `Level *level` before `ccm` |
| `p8` ternary inlined into the call | 16 | 6 | 11,440 | 74,651 | 562 | IR-equivalent to HEAD |
| `p11`/`p15` HEAD + dead `this->level`/`level` re-read | 16 | 6 | 11,440 | 74,651 | 562 | dead code removed |
| `p1` plain (no level local) | 5,196 | 597 | 11,392 | — | 1,453 | r9 -> r5 for `&status` |
| `p2`/`p3`/`p4`/`p16` level local after `ccm`, msg local, `const`, `(void)` | 5,196 | 597 | 11,392 | — | 1,453 | same attractor as `p1` |
| `p5` level local before `ccm` but unused | 5,196 | 597 | 11,392 | — | 1,453 | dead load removed |
| `p9` `Level *&level` reference | 5,196 | 597 | 11,392 | — | 1,453 | load stays at call site |
| `p10` `(unsigned)` sum | 5,196 | 597 | 11,392 | — | 1,453 | |
| `p13` `if/else` instead of `?:` | 5,196 | 597 | 11,392 | — | 1,453 | if-converted to the same select |
| `p14` `int mission = ccm();` local | 5,196 | 597 | 11,392 | — | 1,453 | |
| `p12` `ccm` before `nextInt` | 1,241 | 154 | 11,436 | 82,688 | 656 | wrong call order |
| `p6`/`t8` `Status *st` used for ccm + tail | 946 | 129 | 11,452 | — | 1,344 | r9 kept, +12 (cached value) |
| `t1`/`u1` `Status *st` used for ccm only | 994 | 140 | 11,456 | — | 1,345 | r9 kept, +16 |
| `u2` `st` for ccm + store target | 942 | 143 | 11,452 | — | 1,345 | r9 kept, +12 |
| `u3` `st` for ccm + `getPlayingTime` | 929 | 134 | 11,452 | — | 1,345 | r9 kept, +12; block bytes identical to orig |
| `u5` `st` after `nextInt` | 929 | 136 | 11,452 | — | 1,345 | r9 kept, +12 |
| `t2`/`t3` `Status *st` after `nextInt` | 5,198 | 601 | 11,404 | — | 1,440 | -36 |
| `t4`/`t5` `st` only in the tail | 5,197 | 601 | 11,404 | — | 1,440 | -36 |
| `t6`/`t7` `long long now = ...getPlayingTime()` | 5,198 | 601 | 11,404 | — | 1,440 | -36 |

Earlier waves' variants (volatile/`const`/`ref`/address-of level forms,
`st_anchor`, `level_store`, `rb_*`, `sum_cast`, `anchor_plain*`,
`ll_anchor*`, `rnd_anchor`, …) are in
`mgame-onupdate-wave191b-w13713.md` and
`mgame-onupdate-last16-wave163-w10919.md`; none beat raw 16 at size 11,440.

### The near miss (`u3`, 11,452)

`Status *st = Globals::status;` used for the `ccm` call *and* for
`getPlayingTime`, with `Globals::status` used for the `strd` target, keeps
`&Globals::status` in **r9** and produces the original's block byte-for-byte
(`movw r2,#3166; ldr.w r1,[fp,#120]; cmp; it eq; movweq r2,#3164;
add r2,r4; mov r0,r1; movs r1,#28; blx`), but the cached status value costs
`ldr r5,[r9]` + `mov r0,r5` up front and `mov r0,r5` in the tail: +12 bytes
net (and the tail is no longer shared).  Every `Status *` local form that
keeps r9 pays the same 12–16 bytes; every form that avoids the cache falls
back to the r5 attractor.

## 4. Conclusion

The last 16 bytes are a global register-allocation tie-break for
`&Globals::status` (r9 vs r5), not a scheduling or source-shape error in this
block.  No size-exact source shape found in this wave (or the four earlier
waves) makes the level load sink past `getCurrentCampaignMission` while
keeping the r9 GOT-base pin: sinking the load requires the `ccm` call to be a
separate statement, and that IR form always re-colours `&Globals::status` to
r5 and drops the function to 11,392.  HEAD (w13713) remains the best known
shape at raw 16 / aligned 6 / size 11,440 exact.
