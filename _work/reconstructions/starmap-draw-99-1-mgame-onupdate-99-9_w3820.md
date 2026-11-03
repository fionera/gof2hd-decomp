# StarMap::draw (99.1%) + MGame::OnUpdate (99.9%) — residual analysis (w3820)

Status: research pass, 2026-11-03. Worktree `wt/w3820`, branch `wave88-w3820`,
based on master `41804126`. Every number below was measured in this worktree
against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's NDK r18b
clang 7.0.2 / gold 1.12 and the official `tools/verify/verify.py`. No source
change was landed: both functions are already at their best measured shape and
every alternative tried regressed the byte count.

Baseline (unchanged): **byte_exact 4498 / linked_exact 4498**, size-exact
4524/4524, `.text 0x1337c8`, lint `CLEAN ✓`, drift 0, avg 99.9%.
`StarMap::draw` = 99.1% / 2644 B; `MGame::OnUpdate` = 99.9% / 11440 B.

The build-flag question is settled (`worst_function_flag_sensitivity_w3751.md`),
so this pass only varied source shape. Scratch drivers and disassembly dumps
live in `/dev/shm/w3820` (`smharness.py`, `harness.py`, `bytediff.py`,
`splitdiff.py`).

## 1. MGame::OnUpdate — one 41-byte register-allocation site

Byte diff (whole 11440-byte body) is a single run at `0x17e093..0x17e0bc`:

```text
orig  17e090: ldr.w r0, [r9]            ; Globals::status
      17e094: blx   Status::getCurrentCampaignMission
      ...
      17e0b8: ldr.w r0, [r9]            ; Globals::status
      17e0bc: blx   Status::getPlayingTime
ours  17e090: ldr.w r5, [r9]            ; st = Globals::status
      17e094: mov   r0, r5
      17e096: blx   Status::getCurrentCampaignMission
      ...
      17e0ba: mov   r0, r5
      17e0bc: blx   Status::getPlayingTime
```

`r9` is `&Globals::status` in both. The whole rest of the function is
byte-identical. The site is the mission-23 idle-contact condition (source
`src/game/menu/MGame.cpp:2772-2776`):

```cpp
Status *st = Globals::status;
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getMission()->isEmpty() &&
    st->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
    3600000) {
```

### Root cause (confirmed in LLVM IR)

`clang -S -emit-llvm` on the TU gives

```text
%1091 = load @_ZN7Globals6statusE          ; st
%1092 = call getCurrentCampaignMission(%1091)   ; CSE'd with st
...
%1098 = load @_ZN7Globals6statusE          ; getMission (fresh)
...
%1102 = call getPlayingTime(%1091)          ; st
%1103 = load @_ZN7Globals6statusE          ; field_0x100 (fresh)
```

`st` is live across the `isMining`/`getMission` calls, so the register
allocator must keep it in a callee-saved register (r5). Because the first
`Globals::status` load is adjacent to the `st` load with no call in between,
clang CSEs it and the first call reuses r5. The original re-loads
`Globals::status` for every use, i.e. it has no live `st` at all.

The size is exactly neutral: ours `ldr r5,[r9]` + `mov r0,r5` + `mov r0,r5`
= 4+2+2 = 8 bytes; orig `ldr r0,[r9]` + `ldr r0,[r9]` = 4+4 = 8 bytes.

### Why removing `st` fails

Dropping `st` (all `Globals::status->`) gives the right loads but the
allocator then picks **r5** for `&Globals::status` instead of r9 and inserts
`mov r9,r5` / `mov r5,r9` shuffles: 11392 B (48 B short), 28 fewer
instructions, large structural diff. So the `st` local is what pins
`&Globals::status` to r9; removing it changes the whole function's allocation.

### Variants measured (official verifier / byte diff)

All 16 combinations of `st` vs `Globals::status` at the four condition sites
(getCurrentCampaignMission / getMission / getPlayingTime / field_0x100) were
tried. Only `?g?g` (getPlayingTime uses `st`, everything else direct — the
current source) gives 41 bytes; every other combination is ~5000 diff bytes.
Also tried and rejected: `Status &st`, `Status *const st`, `Status *&st`,
`Status **stp`, `Status *volatile st`, `(st = Globals::status)->...` inline,
`st` at function/outer-block scope, `st` used in the mission-24 / >=122 /
inAlienOrbit branches, `st` used for the body assignment, volatile/ptr-cast
first call, `Globals::status = st` self-assign, `__builtin_assume`. All are
either 41 (same site) or ≥270 bytes.

### Remaining idea (not landed)

The original almost certainly has a *second* long-lived value that occupies a
low callee-saved register (forcing `&Globals::status` into r9) while
`getPlayingTime` still re-loads `Globals::status`. Every candidate tried
(`st` used in a later branch, `&scratchVector`-style hoists, function-scope
`st`) changed the later codegen too. A source witness for that value is
needed; guessing it is not honest C++.

## 2. StarMap::draw — r8/fp register swap + loop-bound hoist

Byte diff (whole 2644-byte body) is 15 bytes at 10 sites, all the same
register-allocation story:

```text
orig  c9338: add.w  r8, sp, #136        ; r8 = &Vector temp
      c9360: ldr.w  fp, [sp, #16]       ; fp = &this->scratchVector
      ...
      c93b2: mov    r0, fp              ; Vector::operator=(scratchVector)
      c94d6: ldr    r0, [r4, #84]       ; this->systems
      c94d8: ldr    r0, [r0, #0]        ; ->size()  (re-read every iteration)
ours  c9338: add.w  fp, sp, #136        ; fp = &Vector temp
      c9360: ldr.w  r8, [r4, #84]       ; r8 = this->systems (hoisted)
      ...
      c93b2: ldr    r0, [sp, #16]       ; &scratchVector from stack
      c94d6: ldr.w  r0, [r8]            ; ->size()  (hoisted)
```

The source is `src/game/world/StarMap.cpp:913-915`:

```cpp
Array<SolarSystem *> &systems = *this->systems;
for (uint32_t i = 0; i < systems.size(); i++) {
```

The reference makes clang hoist `this->systems` into r8 for the loop bound;
that steals r8 from the `Vector` temp, pushing the temp to fp and
`&this->scratchVector` to `[sp,#16]`. The original re-reads `this->systems`
each iteration (so no `systems` reference) and keeps `&this->scratchVector`
in fp.

### Why removing the reference fails

`for (i = 0; i < this->systems->size(); i++)` restores the re-read and the
r8 temp, but the allocator then hoists `Globals::Canvas` into sl and uses fp
for the animated-route loop counter: 1662 byte-diff (the body shifts 2 bytes
and the animated-route loop's `this->systemPath` / `this->systemPositions`
loads move). `Array<SolarSystem *> *systems = this->systems;` and
`const &systems` both reproduce the current 15-byte diff.

### Variants measured

Removing the ref; pointer form; `(*this->systems).size()`; moving the ref
before `SetColor` / before `scratchVector2` / after the loop; `Vector &sv` /
`Vector *sv` hoists inside the pulse block and across the whole mode block;
`PaintCanvas *canvas` locals; `for`/`while`; `int`/`uint32_t` loop variables.
Best remains the unmodified source at 15 bytes. Moving the ref earlier costs
86 bytes; removing it costs 1662.

### Remaining idea (not landed)

The original's loop bound is `this->systems->size()` (re-read) yet its
allocation matches the *reference* form (r9-style hoist of `&scratchVector`,
no `Globals::Canvas` hoist). A source shape that keeps `&scratchVector` in fp
and the animated-route counter in sl while re-reading `this->systems` was not
found; the allocator's sl/fp choice flips with the ref. Needs a witness for
the original's `scratchVector` access pattern (possibly a `Vector &` local
spanning both route loops) rather than more guessing.

## 3. Conclusion

Both targets are source-shape / register-allocation residuals, not missing
code, wrong constants, wrong callees, or flag problems. No change improves
the byte count without regressing the gate, so the tree is left at the
4498/4498 baseline. The single most important remaining root cause for both
is the same: **a long-lived local in the original source pins a specific
callee-saved register (r9 for `&Globals::status`; fp for `&scratchVector`)
that our reconstruction reaches only indirectly, and every direct spelling we
try makes clang choose a different register.**
