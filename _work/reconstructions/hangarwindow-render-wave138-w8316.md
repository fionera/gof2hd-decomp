# `HangarWindow::render` — wave 138 (w8316): spill-weight probes; the ship value is
# *global* because the `getShip()` call is an invoke — new lever

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8316`,
base `96b069792` (wave-137 docs; `render` 94.7 %, gate byte 4507 / linked 4507).

This pass: **94.7 -> 94.7 %** (no source change landed). `check` is green
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0).
Deliverable = this evidence record plus the new invoke/block-split lead.

Metric used below: **diffbytes** = differing bytes in the 9700-byte function body
(`/dev/shm/w8316/db.py`; function at file offset `0x1490d8` in both `.so` files).
Landed baseline: **diffbytes 2062, 183 runs**. (The official `pct` is rounded to
0.1 and cannot separate these variants; all read 94.7.)

## New hypothesis (from wave 137 / w8217)

w8217 showed the RA queue is size-ordered and that the base-order build gives
`this=r5, layout=r6, ship=r4` while the original is `this=r4, layout=r5, ship=r4`
with a `ldrd` reload. The suggested lever: change the spill weight of `this` vs
`&Globals::layout` without moving off 9700 B (same-TU inline, or a local that
extends `this`'s live range).

## Finding 1 (NEW): the ship value is a *global* interval in our build because the
## `getShip()` call ends the basic block (invoke); in the original it does not

`RegAllocGreedy::enqueue` (release_70, fetched and read):

```
if (Stage == RS_Split)                 Prio = Size;                       // split last
else if (Stage == RS_Memory)           Prio = MemOp++;
else if (Stage == RS_Assign && !ForceGlobal && intervalIsInOneMBB(LI))
                                       Prio = beginIndex.getInstrDistance(last);  // local
else                                   Prio = (1u<<29) + Size;           // global
Prio |= (1u<<31);                                                          // global/local > split
CurQueue.push({Prio, ~Reg});                                               // lower vreg wins ties
```

so **global > local > split**, and within a tier larger size wins.

In our build the ship value is defined in `bb.383` and compared in `bb.384`; the
`getShip()` call is an **invoke** (bb.383 successors = `bb.384` normal + `bb.389`
landing pad), so the block ends with a redundant `t2B %bb.384` and ship is
*global*. `this`/`layout` are spiller reloads (vregs `%3345`/`%3368`) and are
`RS_Split`, so ship (vreg `%403`, global) is allocated first and takes `r4`;
`this` then gets `r5`, `layout` `r6`.

The original has **no branch after the call** (`149b36 blx; 149b3a cmp`), i.e.
the call does not end the block, so the ship value is a **local** interval. A
local still loses to a global, but the original's `this` reload is a *short*
local piece `[149b20,149b2c]` (it dies at `selectedItem`), while ship spans the
call. The decisive difference is that the original allocator can reuse `r4` for
ship (this's piece is dead by `149b32`) and then reload `this` at `149b3c`.

**New, untested lever:** make our `getShip()` call a plain call rather than an
invoke (i.e. move it out of the cleanup region so `bb.383` does not split). The
`String name;` local (destructor) is what keeps the call in an EH region. This
is the first lead that explains *why* the base-order build re-rolls to
`this=r5`; every previous wave only perturbed the source inside the already
split block.

## Finding 2: extending `this`'s live range does not move the ship-branch tie

All probes built with the fast single-TU harness (`/dev/shm/w8316/probe.sh`,
`.o` disassembly) and the ship-branch region inspected. `pair` is the
`ldrd` at branch entry (`layout,this`); `ship` is the register of
`this->selectedItem->ship`.

### Base-order landed source (`selectedItem->ship == getShip()`) — 9700 B

| variant | pair | ship | size | diffbytes |
|---|---|---|---:|---:|
| landed (flipped order, control) | r5,r4 | r1 after call | 9700 | **2062** |
| base order | r6,r5 | r4 before call | 9700 | 2082 |
| base + `ListItem *sel` before `if(tab!=0)` | r6,r5 | r4 | 9700 | – |
| base + `Ship *ship` before `if(tab!=0)` | r6,r5 | r4 | 9700 | – |
| base + `HangarWindow *self` before `if(tab!=0)` | r6,r5 | r4 | 9700 | – |
| base + `HangarWindow *self` at loop-body top | r6,r5 | r4 | 9700 | – |
| base + `(void)this->selectedItem` / `(void)this->field_0x100.a` | r6,r5 | r4 | 9700 | – |
| base + `volatile int *padX = this` / `(volatile int*)this` | r6,r5 | r4 | 9700 | – |
| base + `item->ship` (not `this->selectedItem`) | r6,r5 | r4 | 9676 | – |
| base + `bodyThis = this` used by body | r6,r5 | r4 | 9684 | – |
| base + `bool sameShip = ...` | r6,r5 | r4 | 9700 | – |
| base + nested `if (tab==3) { if (...) goto; }` | r6,r5 | r4 | 9700 | – |

Every local copy of `this`/`selectedItem`/`ship` is coalesced away or forces a
whole-function re-roll; none changes the `pair`/`ship` assignment.

### `goto`+`assume` base-order CFG (wave-132 shape) — 9696 B

Same result: `pair r6,r5`, `ship r4`, no reload. Adding `sel`/`self`/`Status*`
locals, parenthesising, or nesting the `tab==3` test is byte-identical to the
control. Adding `(void)listWidth;` etc. before the test shrinks the function to
9672 B (the assume/JT interaction changes) and still gives `this=r5`.

### Same-TU inline helpers

`static inline bool hangarShipMatches(HangarWindow *w) { return w->selectedItem->ship == Globals::status->getShip(); }`
and `static inline Ship *hangarSelectedShip(HangarWindow *w) { ... }` are fully
inlined and coalesced: byte-identical to the base control.

## Finding 3: the independent `0x1496da` this/layout swap is also size-driven

Landed build at `0x1496da`: `ldrd r9,sl` (layout=r9, this=sl); original
`ldrd sl,r9` (layout=sl, this=r9). The reload vregs are `this=%3341`,
`layout=%3364` (this has the lower vreg, so it should win the tie), but layout's
interval in the blueprint block is larger, so layout is allocated first and gets
`r9`.

Hoisting `int pbw = this->progressBarWidth;` before the `getCompletionRate()`
guard (T1) flips layout to `sl` (matching the original) and keeps 9700 B, but
re-rolls the rest of the block and the function: **diffbytes 2702** (worse than
2062). Hoisting `progressBarHeight`/`progressBarBgImage` gives 9708 B.

## Experiments this wave (all reverted; landed stays 94.7 %, 9700 B, 2062 diffbytes)

* base-order landed source, `goto`+`assume` base CFG, local hoists (`sel`,
  `ship`, `self`, `bodyThis`, `hx`, `lay`, `Status*`), same-TU inline helpers,
  `item->ship`, `volatile` padX retypes, `bool sameShip`, nested `if`, no-op
  `(void)` uses, blueprint hoists (T1/T2/T3), `(void)` sink probes.
* Full `check` after restoring the landed source: **RATCHET PASS**.

## Open questions / next levers

* **Make `getShip()` a non-invoke.** The block split after the call is the only
  structural difference found that explains the global/local ship interval. The
  candidate cause is the live `String name;` cleanup region; a source shape that
  ends `name`'s lifetime (or scopes it) before the button code, without changing
  the 156-row / 948-byte `.ARM.extab` record, is the next thing to try.
* If the call stays an invoke, raise `this`'s reload above `RS_Split` (avoid the
  spiller) or make the ship value local — both currently re-roll the function.
* The `0x1496da` this/layout swap and the `0x14a90a` `r6`/`sl` string-buffer swap
  are independent RA ties; the latter accounts for the 1473-byte diff run at
  `0x14a90a`.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8316/db.py                            # diffbytes
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w8316/{cc.sh,probe.sh,probe2.sh,score.sh,db.py,
dbruns.py,rag.cpp,HangarWindow.landed.cpp,v_base.cpp,v_M.cpp,v_T1.cpp,...}`.
