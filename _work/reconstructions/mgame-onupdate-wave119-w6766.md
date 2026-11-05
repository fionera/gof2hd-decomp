# MGame::OnUpdate — wave-119 angle-B anchor sweep (w6766)

Status: **research pass, no `src/` change landed.** Worktree `wt/w6766`, branch
`wave119-w6766`, base = `c8a402ec2` (wave-119 notes commit). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source: **11440 B, 41 diff bytes, one run, fuzzy 99.9 %,
3862/3863 normalised instructions**. This note extends
`mgame-onupdate-wave119-w6726.md` (same wave, same residual) with a post-RA
(`Virtual Register Rewriter`) dump of the 7.0.2 build and 21 further measured
source shapes. Nothing beats 41.

## 1. Baseline (re-verified this session)

```
orig 11440 ours 11440  diffbytes 41  runs 1
pct 99.9   ninsn 3862/3863
run 0x17bb..0x17e3   (absolute 0x17e093 .. 0x17e0bb)
```

The residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2766-2772`):

```cpp
Status *st = Globals::status;
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getMission()->isEmpty() &&
    st->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
    3600000) {
```

Original vs ours (block after the `r9 = &Globals::status` GOT load):

```
original                          ours
17e090 ldr.w r0,[r9]   ccm arg    17e090 ldr.w r5,[r9]   ; st
17e094 blx   ccm                  17e094 mov   r0,r5
17e098 cmp   r0,#23               17e096 blx   ccm
17e09a bne   17e176               17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]              17e09c bne   17e176
17e0a0 blx   isMining             17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                17e0a2 blx   isMining
17e0a6 bne   17e176               17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]   mission    17e0a8 bne   17e176
17e0ac blx   getMission           17e0aa ldr.w r0,[r9]   mission
17e0b0 blx   isEmpty              17e0ae blx   getMission
17e0b4 cmp   r0,#0                17e0b2 blx   isEmpty
17e0b6 beq   17e176               17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]   pt arg     17e0b8 beq   17e176
17e0bc blx   getPlayingTime       17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]   field      17e0bc blx   getPlayingTime
                                  17e0c0 ldr.w r2,[r9]   field
```

Both blocks are 52 B (`4+4+4+4` vs `4+2+4+2+4`); the run is a register-field
permutation plus the induced 2-byte branch-offset shift. From `0x17e176`
(mission-24) to the end of the function the two disassemblies are
byte-identical.

`tools/verify/verify.py:score` sees exactly two `replace` opcodes over
3862/3863 instructions (`2*3860/(3862+3863) = 99.935 %` -> `99.9`). Removing
**one** replace gives `99.961 %` -> `100.0`; that needs either a 0-diff shape
or a one-`mov` shape of exactly the same size.

## 2. New evidence: post-RA VirtRegRewriter dump (7.0.2)

`-mllvm -print-after-all` on the NDK r18b clang (7.0.2) writes the
`Virtual Register Rewriter` dump with physical registers. Reproduce from
`/dev/shm/w6766/cc_cmd.txt` (the exact CMake compile line) by appending
`-mllvm -print-after-all` (the dumps are ~318 MB; the OnUpdate VirtRegRewriter
block is the one whose header is
`# Machine code for function _ZN5MGame8OnUpdateEv: NoPHIs, TracksLiveness, NoVRegs`).

Baseline (`st` present), `bb.238`..`bb.241`:

```
42016B  renamable $r9 = t2LDRi12 killed renamable $r0, 0  ; &Globals::status
42032B  renamable $r5 = t2LDRi12 renamable $r9, 0         ; %128 = st
42064B  $r0 = COPY renamable $r5                          ; ccm arg
42608B  $r0 = COPY killed renamable $r5                   ; pt arg
42688B  renamable $r2 = t2LDRi12 renamable $r9, 0         ; field
```

No-`st` build, same block:

```
42016B  renamable $r5 = t2LDRi12 killed renamable $r0, 0  ; &Globals::status -> r5
42032B  renamable $r0 = t2LDRi12 renamable $r5, 0         ; ccm
42592B  renamable $r0 = t2LDRi12 renamable $r5, 0         ; pt
42704B  renamable $r2 = t2LDRi12 renamable $r5, 0         ; field
```

So the anchor works exactly as described in the w6726 note: `%128` (`st`) is
a multi-MBB range that is dequeued before the `RS_Split` GOT base, takes `r5`,
and forces `&Globals::status` to `r9`.

### 2.1 The no-`st` recolour is global, not local

Normalising intra-function branch targets and pool offsets
(`/dev/shm/w6766/insdiff.py`) shows the no-`st` build differs from the
original in ~90 places, not just the idle block:

* every `ldr.w rX,[r9]` of the original becomes the 2-byte `ldr rX,[r5,#0]`
  (that alone accounts for the 48 B size loss: 11392 vs 11440);
* the frame slots swap throughout: `str.w fp,[sp,#32]` <-> `[sp,#36]`,
  `ldr r1,[sp,#32]` <-> `[sp,#36]`, `str r0,[sp,#36]` <-> `[sp,#32]`, ...
  (dozens of occurrences after `0x17e0e6`);
* `mov r5,sl` / `mov r4,sl` and `mov r4,r0` / `mov r9,r0` change at
  `0x17e3xx`;
* the m23-body status loads also use `r5` (`ldr r0,[r5,#0]` at the body's
  `getPlayingTime`/`strd` sites), then `mov r9,r5` at `0x17e0d2`;
* 28 fewer instructions overall (3862 vs 3834) and fuzzy 87.2 %.

That is why the no-`st` source scores 87.2 % even though only one block is
"semantically" different: the register choice cascades through the whole
function.

## 3. Instruction-count argument (why an anchor can never be exact)

The original idle block is 4 wide status loads + `ldrd` = 5 instructions,
20 B. Any anchor (`Status *st = Globals::status`) must be CSE'd with one of
those four loads (otherwise it is an extra instruction *and* an extra load),
and the CSE'd site then needs `ldr rX,[r9]` + `mov r0,rX` = +1 instruction.
The anchor's other use site replaces a 4-byte load with a 2-byte `mov`
(1:1 in instruction count, -2 B). Net: **+1 instruction, same size**. The
baseline is exactly this shape (6 instructions, 20 B).

A 5-instruction / 20-byte block requires four 4-byte instructions before the
`ldrd`, i.e. four `ldr.w` — i.e. **no anchor**. So the only exact shape is the
plain source with `&Globals::status` allocated to `r9`, which is what the
original has and what 7.0.2 refuses to do for every plain spelling tried.

## 4. Variants measured this session

All from the pristine baseline. `size`/`diff`/`runs` from
`/dev/shm/w6766/fdiff.py`; `pct`/`ninsn` from `/dev/shm/w6766/fz.py`
(replicates `verify.py`'s `score()` on one symbol). `cmake --build
cmake-build-match --target link_so` between every row.

| variant (source shape) | size | diff | runs | pct | insn |
|---|---:|---:|---:|---:|---:|
| **baseline: `st` at top, used at ccm + m23 `getPlayingTime`** | **11440** | **41** | **1** | **99.9** | **3862/3863** |
| `boolvar`: same condition folded into `bool contact = ...; if (contact)` | 11440 | 41 | 1 | 99.9 | 3862/3863 |
| `ll`: `3600000LL` | 11440 | 41 | 1 | 99.9 | 3862/3863 |
| `anchor_st_all_but_mission` (`st` at ccm+pt, mission fresh) | 11440 | 41 | 1 | 99.9 | 3862/3863 |
| `anchor_volccm`: `st` at top, ccm via `reinterpret_cast<Status *volatile *>(&Globals::status)[0]` | 11440 | 853 | 36 | 99.0 | 3862/3862 |
| `anchor_ptfield`: `st` at ccm+pt+`st->field_0x100` | 11436 | 4985 | 307 | 92.7 | 3862/3862 |
| `anchor_top_ptfield` (same, `st` declared at top) | 11436 | 4985 | 307 | 92.7 | 3862/3862 |
| `anchor_ccm_pt_field` (`st->getPlayingTime() - st->field_0x100`) | 11436 | 4985 | 307 | 92.7 | 3862/3862 |
| `anchor_ccm_field` (`st` at ccm + `st->field_0x100` only) | 11436 | 4997 | 301 | 92.5 | 3862/3861 |
| `anchor_ccm_mission_pt` (`st` at ccm+mission+pt) | 11436 | 4986 | 303 | 92.5 | 3862/3862 |
| `mission_pt` (`st` at top, used at mission+pt) | 11436 | 4986 | 303 | 92.5 | 3862/3862 |
| `anchor_st_all` (`st` at all four sites) | 11432 | 5041 | 270 | 92.4 | 3862/3861 |
| `anchor_at_ismining_ptfield` (`(st = Globals::status, !isMining())`) | 11392 | 5193 | 214 | 87.1 | 3862/3834 |
| `anchor_at_mission_ptfield` (`(st = Globals::status, Globals::status->getMission()->isEmpty())`, `st` at pt+field) | 11392 | 5193 | 214 | 87.1 | 3862/3834 |
| `anchor_at_mission_stmission_ptfield` (same, `st->getMission()`) | 11392 | 5193 | 214 | 87.1 | 3862/3834 |
| `anchor_at_mission_ptonly` (comma at mission, `st` at pt only) | 11396 | 5173 | 240 | 87.1 | 3862/3836 |
| `ccm_mission` (`st` at ccm+mission, pt fresh) | 11396 | 5173 | 240 | 87.1 | 3862/3836 |
| `refptr`: `Status *&st = Globals::status;` used at all four sites | 11392 | 5196 | 212 | 87.2 | 3862/3834 |
| `refptr_const`: `Status *const &st = Globals::status;` | 11392 | 5196 | 212 | 87.2 | 3862/3834 |
| `stp`: `Status **stp = &Globals::status;` + `(*stp)->...` | 11392 | 5196 | 212 | 87.2 | 3862/3834 |
| control: plain source, no `st` at all | 11392 | 5196 | 212 | 87.2 | 3862/3834 |

(`anchor_volccm` is size-exact and instruction-count exact, but `ldrd
r2,r3,[r5,#256]` folds the field load into the anchor, so the block is
`ldr r5,[r9] / mov r0,r5 / ... / mov r0,r5 / ldrd` — 5 instructions but the
wrong two positions, and the 2-byte block shrink shifts every branch target
before it, hence 853 diff bytes / 99.0 %.)

Also re-confirmed from the previous session (do not redo):

* `Globals.h` `static Status *status;` -> `static Status *volatile status;`
  (plus `Globals.cpp`, `Item.cpp` `static Status *volatile &status`,
  `MenuTouchWindow.cpp` `Status *volatile const *statusHolder` x2) builds but
  regresses the target to **11608 B / 6986 diff / 556 runs** and is a
  cross-TU change.
* 7.0.1 (`/dev/shm/wf213/llvm701build/bin/clang++`) is not a faithful oracle:
  it picks `r9` for the plain no-`st` source.

## 5. Conclusion

The 41-byte / one-run residual is a hard local optimum for every anchor
spelling: the anchor is required to keep `&Globals::status` in `r9`, but it
necessarily adds one instruction at the load it is CSE'd with. The only exact
shape is the plain source with `r9` chosen by the allocator, and clang 7.0.2
picks `r5` for every plain spelling tried (`Globals::status`, `Status *&`,
`Status **`, `Status &`, `auto`, `const`, comma-assign at every `&&` site).
If a future pass finds a source-level way to make the GOT base `RS_NotSplit`
(or to occupy `r5` across `0x17e08c..0x17e0da` without emitting an
instruction), the plain source becomes byte-exact.

## 6. Gate (this worktree, baseline source)

```
cmake --build cmake-build-match --target check -j6
  RATCHET PASS
  verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
lint  : CLEAN (no new soft violations; hard invariants 0)
drift : 0 field(s) across 0 class(es)
```

No `src/` change, no baseline update, no section/extab change
(`.ARM.extab` still 0x26290, 8 B short: `createShip` -8 only).

## 7. Scratch (not committed)

`/dev/shm/w6766/`: `MGame.base.cpp` (pristine source), `cc_cmd.txt` (exact
compile line), `fdiff.py` (size/diff/runs), `fz.py` (one-symbol fuzzy +
instruction counts), `diffshow.py` (byte-run dump), `insdiff.py`
(instruction-stream opcodes with branch/pool normalisation), `harness.py`
(source-shape -> build -> measure driver), `v1.json`..`v6.json` (variant
batches), `pa_st.err`/`pa_nost.err` (full `-print-after-all`),
`gra_*.mir`/`lia_*.mir` (extracted MIR stages).
