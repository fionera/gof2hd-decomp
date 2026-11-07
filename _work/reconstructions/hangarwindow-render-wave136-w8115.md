# `HangarWindow::render` — wave 136 (w8115): base-order goto+`assume` is IR-identical, the reload is a pure RA tie

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8115`,
base `2e51d765d` (wave-135 docs; `render` 94.747001 %, gate byte 4506 / linked 4506).

This pass: **94.747001 -> 94.747001** (no source change landed). `check` is green
(byte 4506, linked 4506, size 4524, data 843, imports 207, lint CLEAN, drift 0).

## New hypothesis (from wave 135 / w7779)

Force `this`'s reload to be split at the ship load without adding instructions (an
opaque copy of `this` used only by the body), or a same-TU inlining change that
alters the reload's spill weight.

## Finding 1: the base-order `goto`+`assume` branch is instruction-for-instruction
## identical to the original except for the register pair

The wave-132 `goto ship_done` + `__builtin_assume(tab < 6)` shape with **base**
operand order (`this->selectedItem->ship == Globals::status->getShip()`), built at
9696 B, emits this branch (`0x149b1e..`):

```
149b1e ldr  r0,[sp,#108]        ; tab
149b20 ldrd r6,r5,[sp,#132]     ; &Globals::layout=r6, this=r5
149b24 cbz  r0,149b92           ; case 0 -> upgrade test   (MATCHES orig shape)
149b26 cmp  r0,#3
149b28 bne.n 149b40
149b2a ldr  r0,[pc,#336]        ; &Globals::status
149b2c ldr  r1,[r5,#104]        ; this->selectedItem
149b2e add  r0,pc
149b30 ldr  r0,[r0,#0]          ; Globals::status
149b32 ldr  r4,[r1,#12]         ; ship -> r4   (BEFORE the call, as original)
149b34 ldr  r0,[r0,#0]
149b36 blx  getShip
149b3a cmp  r4,r0
149b3c beq.w 14a6cc            ; ship-equal -> join
```

Original (`0x149b1e..0x149b44`, 38 B):

```
149b1e ldr  r0,[sp,#108]
149b20 ldrd r5,r4,[sp,#132]     ; &Globals::layout=r5, this=r4
149b24 cbz  r0,149b96
149b26 cmp  r0,#3
149b28 bne.n 149b44
149b2a ldr  r0,[pc,#340]
149b2c ldr  r1,[r4,#104]
149b2e add  r0,pc
149b30 ldr  r0,[r0,#0]
149b32 ldr  r4,[r1,#12]
149b34 ldr  r0,[r0,#0]
149b36 blx  getShip
149b3a cmp  r4,r0
149b3c ldrd r5,r4,[sp,#132]     ; RELOAD
149b40 beq.w 14a6d0
```

The two instruction streams are **identical in mnemonic and scheduling**; the only
differences are (a) the pair registers `r6,r5` vs `r5,r4` and (b) the missing
`ldrd` reload. So the base-order source already produces exactly the original's
IR/evaluation order; the 4-byte delta is purely the register allocator's choice of
which of `this`/`&Globals::layout` gets the low callee-saved register.

In the base build the allocator assigns `ship=r4`, `this=r5`, `&layout=r6` (three
live-across-call values, three registers). The original assigns `this=r4`,
`&layout=r5`, `ship=r4` (reuse) and reloads `this` after the call. That is a
live-range split of `this` at the ship load, not an IR difference.

## Finding 2: the flipped `goto`+`assume` control has the right pair, wrong ship
## placement

Flipped order (`getShip() == this->selectedItem->ship`) + `goto`+`assume` (wave-135
control) gives `this=r4`, `&layout=r5` (the original pair) but loads the ship
*after* the call into `r1`:

```
149b20 ldrd r5,r4,[sp,#132]     ; pair MATCHES
149b24 cbz  r0,149b92
...
149b32 blx  getShip
149b36 ldr  r1,[r4,#104]
149b38 ldr  r1,[r1,#12]         ; ship -> r1 (after call)
149b3a cmp  r0,r1
149b3c beq.w 14a6cc
```

So the original is exactly "flipped pair + base ship placement", i.e. the
allocator must reuse `r4` for the ship and reload `this`.

## Experiments this wave (all reverted; landed stays 94.747001 %, 9700 B)

Every variant was built and scored with the same `verify` normalization
(`/dev/shm/w8115/score.py`).

### Opaque copies of `this` used only by the body (the wave-135 suggestion)

Base-order `goto`+`assume` shape, body uses `bodyThis`, condition uses `this`:

| variant | pct | size |
|---|---:|---:|
| `HangarWindow *bodyThis = this;` after the `goto` | 85.8 | 9688 |
| same, flipped condition | 86.0 | 9688 |
| `HangarWindow *bodyThis = this;` before `if (tab != 0)` | 83.9 | 9676 |
| `HangarWindow *const bodyThis = this;` before | 83.9 | 9676 |
| `(HangarWindow *)(unsigned long)this` copy before | 83.9 | 9676 |
| `(HangarWindow *)(void *)this` copy before | 83.9 | 9676 |

Every copy is either coalesced away (no second `ldrd`) or forces the whole
function off the 9700 B shape. None produced a reload.

### Local ship / selectedItem (re-measured on the base `goto` shape)

| variant | pct | size |
|---|---:|---:|
| `Ship *ship = this->selectedItem->ship;` before `if (tab != 0)` | 83.9 | 9676 |
| same, flipped compare | 83.8 | 9676 |
| `ListItem *sel = this->selectedItem;` before | 83.9 | 9676 |
| nested `if (tab == 3) { Ship *ship = ...; if (...) goto; }`, base | 86.8 | 9688 |
| nested, flipped | 86.8 | 9688 |

The local is hoisted unconditionally before the `tab != 0` test (the source
`this->selectedItem->ship` load is not sunk), so the branch moves and the whole
allocation changes.

### Upgrade-condition operand order (new)

The original's upgrade block tests `tab == 1` **first**
(`149b96: cmp r0,#1; bne join; 149b9c: ldrb r0,[r4,#285]`), i.e. the original
source is `tab == 1 && this->upgradeMode` (or the compiler reordered it). Our
source is `this->upgradeMode && tab == 1`. This changes the Jump-Threading cost:
with `tab` first the duplicated upgrade block is one unit cheaper, so a single
`assume` no longer blocks the case-0 thread (cost 6, threshold `> 6`). Results:

| variant | pct | size | note |
|---|---:|---:|---|
| landed + `tab == 1 && this->upgradeMode` | 94.7 | 9700 | same pct |
| base `goto` + `tab == 1 && this->upgradeMode` (no assume) | 94.3 | 9700 | case 0 still threaded |
| base `goto` + `tab == 1 && this->upgradeMode` + 1 assume | 94.3 | 9700 | assume folds/too cheap |
| base `goto` + `tab == 1 && this->upgradeMode` + 2 assumes | 89.9 | 9696 | JT blocked, 4 B short |
| flipped `goto` + `tab == 1 && this->upgradeMode` | 94.7 | 9700 | same pct |

So the tab-first order is a real IR difference (it changes the JT cost model) but
does not by itself recover the reload.

### `assume` form / blueprint-expression reorder

All non-foldable `assume` forms (`tab < 6`, `< 100`, `!= 5`, `>= 0`,
`Globals::status != 0`, `this != 0`, `< 4`, `!= 4`) give byte-identical base
`goto` builds: 90.0 % / 9696 B; flipped: 90.3 % / 9696 B. Reordering the
blueprint `DrawImage2D`/`DrawRegion2D`/`DrawString` arguments (the independent
`0x1496da` `ldrd sl,r9` vs `ldrd r9,sl` swap) changes the pct only downward
(93.6–94.5) or leaves it at 94.7.

## Conclusion

The base-order source is not the blocker: it already reproduces the original's
instruction sequence and evaluation order exactly. The last 4 bytes are a greedy
allocator tie-break — the original splits `this` at the ship load
(`this=r4` reused for `ship`, reload at `149b3c`), while our allocator keeps
`this` live in `r5` across the call and uses a fresh `r6` for `&Globals::layout`.
No source-level copy of `this`, local ship/selectedItem, upgrade-order or
`assume` perturbation tried in waves 132-136 changes that tie without moving the
function off 9700 B.

## Open questions / next levers

* A construct that raises `ship`'s allocation priority above `this` at the ship
  def **without** taking a callee-saved register for a new live value, so the
  allocator splits `this` at `149b2c` and reuses `r4`.
* The independent `(this, &Globals::layout)` pair swap at `0x1496da`
  (`ldrd sl,r9` vs `ldrd r9,sl`) is still present and would keep the function
  non-exact even after the ship branch is fixed.
* The original's upgrade block tests `tab` before `upgradeMode`; a source form
  that both orders the test that way and keeps the asymmetric JT is still open.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w8115/{score.py,diffmap.py,mk.py,batch.py,
batch2.py,batch3.py,batch4.py,run.sh,ir_cmd.sh,HangarWindow.landed.cpp}`.
