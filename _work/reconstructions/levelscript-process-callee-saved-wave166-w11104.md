# LevelScript::process — wave 166 / w11104: callee-saved map, `Status*` shape + alias probes

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact. Owned TU `src/game/world/LevelScript.cpp`. Worktree `wt/w11104`,
base master `85c1f88df` (wave 165). Scratch `/dev/shm/w11104/`.

Result: **no source win landed** (analysis note only).  The wave-164 base
(54,247 raw / 17,532 aligned, 57,232 B) is still the best size-exact shape
measured.  Every size-exact shape tested keeps `this=fp`, `canary=sl`.

```
                         base (HEAD)        original
process raw diffbytes    54,247             —
process aligned insns    17,532             —
orig / ours size         57,232 / 57,232   57,232
insn count (ours/orig)   20,709 / 20,830
.rel.plt                 0 (BYTE-IDENTICAL)
byte_exact               4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. Corrected map: r9 is scratch in the original, not a long-lived value

The wave-164 note tabulated the original's `Status*` value as living in `r9`.
That is a misreading.  The original prologue is

```
mov sl, r0                 ; this -> sl
...
ldr.w fp,[r0]              ; fp = &__stack_chk_guard
ldr.w r0,[fp]              ; canary
str r0,[sp,#428]
ldr.w r0,[r4,#24]!         ; r4 = &m_pLevel
blx getMessages            ; r5 = messages
ldr r0,[r4]; blx getPlayer ; player -> [sp,#84]
ldr r0,[r4]; blx getEnemies; r8 = enemies
ldr r0,[pc]; add r0,pc
ldr r4,[r0]                ; r4 = &Globals::status   (GOT load)
ldr r0,[r4]                ; Status* value (reloaded at every use)
blx getCurrentCampaignMission
...
ldr r0,[r4]                ; second dispatch reloads the value
blx getCurrentCampaignMission
```

`r9` is a plain scratch register in the original: its first write is
`mov r9, r2` at `0x137ebc` (inside the mission-0 body), and it is reused
throughout (`ldr.w r9,[sp,#96]`, `mov r9, sl`, ...).  So the original has
**six** whole-function values in callee-saved registers
(`&status=r4, messages=r5, delta=r6, enemies=r8, this=sl, guard=fp`) and
leaves `r9` free.  Our base has **seven** because of the cached
`Status *status` local (`Status*=r4, messages=r5, delta=r6, enemies=r8,
&status=r9, guard=sl, this=fp`).

The original does *not* cache the `Status*` value: both dispatches reload
`[r4]`.  So the faithful source is direct `Globals::status->` access (or a
`Status *&` reference), not a `Status *` local.

## 2. `Status *&` / direct / `Status **` — identical codegen, +1 raw

All three spellings of "don't cache the value" compile to the same code
(the reference is a pure alias; no IR difference):

| variant | raw | aligned | size | map |
|---|---:|---:|---:|---|
| base `Status *status = Globals::status;` | **54,247** | **17,532** | 57,232 | this=fp, guard=sl, &status=r9, Status*=r4 |
| `Status *&status = Globals::status;` | 54,248 | 17,525 | 57,232 | this=fp, guard=sl, &status=r9 |
| no local, `Globals::status->` twice | 54,248 | 17,525 | 57,232 | same |
| `Status **status = &Globals::status;` + `(*status)->` | 54,248 | 17,525 | 57,232 | same |
| first dispatch direct, second via local | 54,247 | 17,532 | 57,232 | base (CSE'd) |
| first via local, second direct | 54,248 | 17,525 | 57,232 | same as direct |

Removing the cached value does **not** move `&status` to `r4`: `r4` simply
stays free (used only for the transient `&m_pLevel` copy) and `&status`
still gets `r9`.  The +1 raw is the second dispatch: original
`ldr r0,[r4]` (2 B, low reg) vs ours `ldr.w r0,[r9]` (4 B, high reg),
partly compensated elsewhere.  Size stays exact.

## 3. Declaration order / first-use order

Re-confirmed the wave-164 24-permutation result on the wave-165 base: the
base order (`messages, player, enemies, status`) is the best raw and every
size-exact order keeps `this=fp`.  Moving `status` first is not size-exact
(+84 B, frame/register map completely different: `this=r6`, `delta=r8`,
`messages=sl`, `&status=r9`, `Status*=r4`).

`__builtin_expect` on the mission-0 dispatch (the first body call) is *not*
neutral in the current base: e=0 -> 55,791 / 57,420 B, e=1 -> 55,673 /
57,444 B.  (It is a real branch-weight/block-order change, not an RA tie.)

`return m_bCinematicBreak;` vs `!= 0` is codegen-identical.

## 4. Alias probes: only the stack-spilled `m_pLevel` alias is a lever

Aliasing a register-resident local is coalesced away and is bit-identical
to the base (54,247 / 17,532 / 57,232):

| alias (2 uses) | raw | aligned | size |
|---|---:|---:|---:|
| `PlayerEgo *pl = player;` | 54,247 | 17,532 | 57,232 |
| `Array<KIPlayer*> *foes = enemies;` | 54,247 | 17,532 | 57,232 |
| `Array<RadioMessage*> *msgs = messages;` | 54,247 | 17,532 | 57,232 |

`Level *self = m_pLevel;` is different because `m_pLevel` is stack-spilled
(`[sp,#80]`): the alias forces a reload into a new long-lived register
value.  It **does flip the map to the original's `this=sl`, `guard=fp`**:

```
mov sl, r0        ; this -> sl        (matches original)
mov fp, sl
...
ldr.w r8,[fp]     ; self = m_pLevel
...
ldr.w r9,[r0]     ; &Globals::status -> r9   (original: r4)
ldr.w r4,[r9]     ; Status* -> r4
str.w sl,[sp,#80] ; this spill
```

but it also re-lays the frame: `sub sp,#424` (0x1a8) instead of 0x1b0,
spill slots at `[sp,#64..#88]`, canary at `[sp,#420]`, and the function
grows to **57,416 B (+184)** with raw 55,501 / aligned 21,550.

Placement matters.  Declaring the alias at the very top (before
`getMessages`) and using it for the two accessor calls reproduces the
wave-164 N=2 row exactly: **57,240 B (+8)**, raw 54,933, aligned 17,549,
frame back to 0x1b0, but the map is *unchanged* (`this=fp`, `guard=sl`,
`&status=r9`) because the alias is coalesced into the `m_pLevel` load that
the accessor block already needs (`mov r0, r4` for all three calls instead
of the original's `ldr r0,[r4]` reloads).  So the +8 alias is not the
original shape either.

## 5. Why `this`/guard are the last two (LLVM 7 greedy RA)

The two whole-function values that end up in `sl`/`fp` are the ones whose
live ranges are **split** by `RAGreedy`: `enqueue()` gives `RS_Split`
ranges `Prio = Size` (no `1<<31`/`1<<29` bits), so they are dequeued after
every `RS_Assign` range and take the last two callee-saved registers.
`this` and the `__stack_chk_guard` address both span the whole function;
which of the two is `r10` vs `r11` is decided by their split-range sizes,
i.e. by their exact last-use slot.  In our base the guard's last use is
the epilogue canary reload (after the return-value load), so guard > this
(guard=r10, this=r11).  In the original `this` wins (this=r10, guard=r11).
That difference is a property of the whole 57 kB body (the reconstruction
still differs by 54,247 raw bytes), not of any local declaration shape:
`this` is used at the very end (`ldrb [this,#17]`) in both binaries, so
the ordering is decided by code we have not matched yet.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 54247 aligned= 17532 size= 57232/57232 insn=20709/20830
```

## 7. Conclusion

* The original map is `&status=r4, messages=r5, delta=r6, enemies=r8,
  this=sl, guard=fp`, with `r9` free scratch and the `Status*` value
  reloaded per use.  The wave-164 "Status*=r9" entry is corrected here.
* The faithful `Status *&`/direct shape is +1 raw (54,248) and does not
  move `&status` off `r9`; it is not a gate win.
* `this=sl` is reachable only via the stack-spilled `m_pLevel` alias, which
  is +184 B and re-lays the frame; no size-exact shape with `this=sl` was
  found.
* The `this`/guard swap is a split-range size tie decided by the whole
  function body; it should be re-checked once the remaining body diffs
  (especially the late `Globals::status`/`this` uses) are matched.
