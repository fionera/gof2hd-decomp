# LevelScript::process — wave 134 / w7765 (16/32-bit balance: `this` register-colouring census)

Status: **analysis only, no source change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bbffef482` (wave-134 head).
Scratch helpers in `/dev/shm/w7765/` (not tracked): `score.py`, `dump3.py`,
`wdiff.py`, `wdiff2.py`, `wdiff3.py`, `percase2.py`, `bigdiff.py`, `batch*.py`,
`LevelScript.base.cpp`.

## Result

```
                         before (master)   after (no code change)
difflib pct              58.166538         58.166538
orig / ours insns        20169 / 20050     20169 / 20050
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
16-bit insns (ours)      11600 / 11359     11600 / 11359
32-bit insns (ours)       7740 /  7865      7740 /  7865
process diffbytes        54359             54359
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7765/score.py`).
Widths are counted from the objdump byte column with the same pool/jump-table
masking verify uses; they are lower than the w7746 note's 13044/7786 because
that note counted pool words and `.short` jump-table entries as instructions.

## NEW census: the width deficit is `this` in `sl` vs `r4/r5/r6`

Aligning the two instruction streams with a register-insensitive key
(`mnemonic + memory displacement`, registers canonicalised to `R`) and looking
at every aligned pair whose *encoding width* differs gives a very clean answer:

| pair (orig -> ours) | count | example |
|---|---:|---|
| `ldr r0,[r5,#imm]` -> `ldr.w r0,[sl,#imm]` | 40 | `this->m_pCamera` / `m_pLevel` |
| `ldr r0,[r6,#imm]` -> `ldr.w r0,[sl,#imm]` | 23 | same |
| `ldr r0,[r4,#imm]` -> `ldr.w r0,[r9,#imm]` | 9 | same |
| `ldr r0,[r4,#imm]` -> `ldr.w r0,[sl,#imm]` | 9 | same |
| `add rX,sp,#imm` -> `add.w sl,sp,#imm` | 8+4+3 | stack `Vector` temporaries |
| `ldr r0,[sp,#imm]` -> `ldr.w r0,[sl,#imm]` | 7 | spilled vs register `this` |
| reverse direction (`ldr.w` -> `ldr`) | 62 | mostly `ldr.w r0,[sl,#imm]` -> `ldr r0,[r5,#imm]` |

Net: **233 aligned pairs widen** (orig 16-bit -> ours 32-bit) and only 62 narrow.
The dominant single cause is that the original **reloads `this` from its spill
slot `[sp,#88]` into a low register (r4/r5/r6) at the start of a case body** and
uses `[r5,#20]` / `[r6,#24]` (2-byte encodings), whereas our allocator keeps
`this` in `sl` for the whole function and emits `[sl,#20]` (4-byte encodings).

This is not a prologue issue: the prologue spill of `this` to `[sp,#88]` is
identical on both sides (`str.w sl,[sp,#88]`). The difference is that the
original's `sl` is a general-purpose callee-saved register reused for
`&__stack_chk_guard`, `player`, `messages`, `sp`, loop counters, etc. (20+
distinct assignments), while ours dedicates `sl` to `this` (only two
assignments: `mov sl,r0`, `ldr.w sl,[sp,#88]`). The original's `this` is
reloaded from `[sp,#88]` 328 times; ours 200 times.

Representative confirmation — case 131 state 1 (`0xd94` orig / `0xd68` ours)
is **instruction-for-instruction identical except its first three instructions**:

```
orig  0xd94  ldr   r5,[sp,#88]      ; this -> r5
      0xd96  ldr   r0,[r5,#28]      ; m_nState
      0xd98  cmp   r0,#2
ours  0xd68  ldr.w r9,[sp,#88]      ; this -> r9
      0xd6c  ldr.w r0,[r9,#28]
      0xd70  cmp   r0,#2
```

Every later instruction of that 649-insn body matches. The same is true of case
70 state 0/1 (`0x1c8a` orig / `0x1bdc` ours): identical apart from `this` being
reloaded into `r5`/`r6` in the original vs `sl` in ours.

## Experiments (all built and scored on pristine master; each reverted)

| variant | size | pct | 16-bit | note |
|---|---:|---:|---:|---|
| baseline | 57232 | **58.1665** | 11359 | |
| mission-1 `player->rotate` (see §1) | 57352 | 56.9567 | 11286 | +120, correct but not size-neutral |
| `LevelScript *self = this;` + `self->` in case 16 | 57232 | 58.1665 | 11359 | **byte-identical**: LLVM coalesces the alias |
| case 131 direct `m_tempVec`/`m_dirVec` (drop pointer locals) | 57232 | 58.1665 | 11359 | **byte-identical** |
| case 16 `unsigned` -> `int` loop counters | 57232 | 58.1665 | 11359 | **byte-identical** |
| `__builtin_expect(m_nState == 0, 1)` case 92 | 57232 | 58.1665 | 11359 | **byte-identical** |
| `Status *status = Globals::status;` all body uses | 57084 | 40.5999 | 11143 | -148, re-colours badly |
| mission-1 + `Status *status` all body | 57076 | 40.6319 | 11143 | |
| swap mission-0 state-5 / state-4 arms | 57256 | 55.7961 | 11360 | +24 |
| mission-0 state-3 `m_pLevel->getPlayer()->setCollide(false)` | 57240 | 58.0194 | 11359 | +8 |
| mission-1 + case 131 direct / case 16 int / case 92 expect | 57352 | 56.9567 | 11286 | companions are no-ops |
| mission-1 + swap state-5/4 | 57360 | 54.7842 | 11292 | |

## §1 Real semantic bug found: mission-1 rotate uses a fresh `getPlayer()`

Original `0x137dac` (mission-1 body):

```
137da6  ldr   r0,[sp,#84]      ; cached `player` local
137daa  ...
137dac  blx   PlayerEgo::rotate
```

Our source (`LevelScript.cpp:947`) wrote
`m_pLevel->getPlayer()->rotate(0.0f, angle, angle);`, which emits an extra
`Level::getPlayer()` call. The faithful statement is `player->rotate(...)`.
This is the +1 `getPlayer` in the call-count census. Fixing it is correct but
re-rolls the whole function to **57352 B (+120)** and 56.96 %, so it cannot be
landed under the "keep 57232 B" rule without a -120 companion. It also *lowers*
the 16-bit count (11286), i.e. it is not the width levers the note set out to
find.

The reverse direction also exists: the original calls `getPlayer()` fresh at
some `setCollide` sites where our source uses the cached `player` (e.g. the
mission-0 state-3 `setCollide(false)` at `LevelScript.cpp:740`). Making that
site fresh is +8 B / 58.02 %, so the two do not cancel.

## Why local edits cannot move the colouring

All four "local variable placement/type" edits that should have changed the
IR — the `self` alias, direct member access instead of pointer locals, the
loop-counter signedness, and `__builtin_expect` — compile **byte-identically**.
The `this` colouring is a single global live-range decision: the allocator
keeps `this` in `sl` because `sl` is otherwise free in our IR, while the
original's IR has enough simultaneous live values that `this` is split and
reloaded into r4-r7. Reconstructing that requires either (a) the ~119 missing
instructions (ours 20050 vs 20169) that raise the pressure, or (b) a source
shape that forces `this` out of `sl` without a `volatile`/attribute hack.

## Next lead

1. Fix the mission-1 `player->rotate` bug together with a **-120 B** faithful
   companion. The status all-body cache is -148 but re-colours to 40.6 %, so
   the companion must be found in the same region (mission-1 body) or be a
   genuinely missing statement, not a global caching change.
2. Find the ~119 missing instructions. The call-count census is *not* it:
   ours has **more** calls than the original in 18 of 23 differing symbols
   (getPlayer +4, getLandmarks +2, getStarSystem +2, setTarget +1, ...) and
   fewer only in `translate` (-1) and `getPosition` (-1). The missing
   instructions are arithmetic/branch, not calls.
3. The `this`-reload pattern is the right target, but it needs a global
   pressure change; do not expect a single-body edit to move it.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7765/score.py                          # 58.166538, 57232/57232
python3 /dev/shm/w7765/dump3.py                          # orig3.txt / ours3.txt
python3 /dev/shm/w7765/wdiff2.py                         # width-pair census
python3 /dev/shm/w7765/batch2.py                         # local-placement sweep
```
