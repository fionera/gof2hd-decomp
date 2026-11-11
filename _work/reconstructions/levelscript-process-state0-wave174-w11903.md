# LevelScript::process — wave 174 / w11903: mission-0 state-0 block / first literal pool

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11903`, branch `wave174-w11903`, base master `be9dd429b`.
Follow-up to `levelscript-process-state0-wave173-w11803.md`.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    53,711
process aligned insns    18,228
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20728
first literal pool       0x1380c0 (original 0x1380a4, +28 B)
whole .text diffbytes    80,897
.rel.plt                 29392 B, byte-identical (0)
gate                     byte 4507 / linked 4507 / size 4524 / data 843 /
                         imports 207 / lint CLEAN / drift 0 / parity 5370
```

The task: keep the w11803 literal-pool improvement (`0x1380c0 -> 0x1380a4`) while
recovering the raw bytes that w11803's `sepif` shape lost (w11803's candidate was
raw ≈ 55.9k / `.text` 82,884).  **No size-exact raw win below 53,711 was found**;
this note records the full search and the best pool-correct variant.

## Where the +28 pre-pool bytes come from (confirmed)

| region | original | ours (base) | delta |
|---|---:|---:|---:|
| prologue + mission-1 body (`0x137d18..0x137e84`) | 364 B | 370 B | **+6** |
| mission-0 state-0 body (`0x137e84..0x137f7c`) | 248 B | 236 B | **-12** |
| tail preamble (`0x137f7c..0x137f82`) | 6 B | 6 B | 0 |
| tail body up to first pool (`0x137f82..0x1380a4`) | 296 B | 330 B | **+34** |
| **total to first pool** | **908 B** | **936 B** | **+28** |

The `+6` is the stack-canary pointer `&__stack_chk_guard` landing in `r10` (`sl`)
instead of `r11` (`fp`): the original does `ldr.w fp,[r0]` / `ldr.w r0,[fp]`
(8 B), ours does `ldr.w sl,[r0]` / `ldr.w r0,[sl]` (8 B) but the *persistent*
`this` pointer is then in `fp` (ours) vs `sl` (original), which shifts the whole
function's register roles and adds a `ldr.w r9,[r0]` + `ldr.w r4,[r9]` +
`mov r0,r4` (10 B) where the original has `ldr r4,[r0]` + `ldr r0,[r4]` (4 B).

The `-12` is the mission-0 3-ship loop.  The original evaluates `(*enemies)[i]`
twice (it does not cache the `KIPlayer *`):

```
original 137f24: ldr.w r9,[sp,#96]        ; hoist enemies into r9
         137f34: ldr.w r0,[r9,#4]        ; enemies->data_
         137f3c: ldr.w r0,[r0,r5,lsl#2]  ; (*enemies)[i]
         137f40: ldr   r0,[r0,#8]        ; ->geometry
         137f52: ldr.w r0,[r9,#4]        ; enemies->data_ again
         137f58: ldr.w r0,[r0,r5,lsl#2]  ; (*enemies)[i] again
         137f5c: ldr   r0,[r0,#8]
ours     137f34: ldr   r0,[sp,#96]        ; reload enemies from stack
         137f3a: ldr   r0,[r0,#4]
         137f3c: ldr.w r4,[r0,r5,lsl#2]  ; e = (*enemies)[i]
         137f40: ldr   r0,[r4,#8]
         137f52: ldr   r0,[r4,#8]        ; cached e
```

The source-level double-deref (`(*enemies)[i]->geometry->...` twice, no
`KIPlayer *e`) reproduces the original's two dereferences, but the compiler
then caches `enemies` in `r4` *inside* the loop instead of hoisting it to `r9`
(r9 is occupied by the scratch `Vector`), so the block still does not match and
the function grows to 57,344 B (raw 55,626).  The hoist is blocked by the same
`this`/canary register-role swap.

The `+34` tail is `+4` (extra `str.w sl,[sp,#68]` spill of `this` in the tail
preamble), `+2` (first wingman loop layout), `-2` (status reload), `-2`
(`ittt ge` vs two `blt.w`), `+2` (`beq.w` vs `beq.n`), `+26` (second wingman
loop body placed **before** the pool).

## The pool-correct shape: `sepif`

The original's mission chain after the tail is a **separate `if`**, not an
`else if` of `m_bStartSequence` (confirmed against the Unity oracle
`game/LevelScript.c`).  Changing

```cpp
} else if (Globals::status->getCurrentCampaignMission() == 5) {
```
to
```cpp
if (Globals::status->getCurrentCampaignMission() == 5) {
```

makes the L2 loop test block (`bb.34`) fall through to the mission-5 check, so
it is no longer a "water" block and `arm-cp-islands` places the first constant
pool after `bb.30` (the L2 loop entry) — exactly the original layout
`0x1380a4`.  The second wingman loop then lands at `0x138110` and the
mission-5 check at `0x13812a`, both matching the original instruction for
instruction.

### Best pool-correct variant: `start_e1`

`sepif` + remove the deactivation `__builtin_expect(mission >= 2, 1)` +
`__builtin_expect(m_bStartSequence != 0, 1)` on the tail guard:

```cpp
if (__builtin_expect(m_bStartSequence != 0, 1)) { // lint: arm_builtin
...
if (m_pLevel->getPlayer() != 0 &&
    Globals::status->getCurrentCampaignMission() >= 2 &&
    field_0x24 >= 7001) {
...
if (Globals::status->getCurrentCampaignMission() == 5) {
```

| metric | base | `start_e1` |
|---|---:|---:|
| raw diffbytes | 53,711 | **55,036** |
| aligned insns | 18,228 | 18,659 |
| size | 57,232 | **57,232 (size-exact)** |
| first pool | `0x1380c0` | **`0x1380a4`** |
| pool bytes differing | 92/108 | **6/108** |
| whole `.text` diffbytes | 80,897 | 82,224 |
| `.rel.plt` | identical | **9 B differ** |
| byte_exact | 4507 | **4506** |

Region diffs (`start_e1`): pre-pool 752/908, pool 6/108, mid 994/1068,
mid2 1929/2012, rest 51355/53136.  The tail and pool are now essentially
exact; the loss is in `mid2`/`rest` because `MachineBlockPlacement` lays the
mission>=2 chain out differently than the original (the original puts the
Chain-A intro block at `0x1381b2..0x1382dc`, between the mission-5 block and
the mission-26 check; `start_e1` puts the mission-26 check immediately after
the mission-5 block).

**Why it is not landed:** the `sepif` block reorder changes the PLT assignment
order.  `.rel.plt` rotates five entries by one slot
(`MenuTouchWindow::isShowingMessage` moves from slot 2816 to 2812, ahead of
`Gun::shoot`, `PlayerEgo::getRoute`, `PlayerEgo::setTargetFollowCamera`,
`Player::isAlwaysFriend`).  That moves `PlayerEgo::setRocketControl`'s PLT
entry from `0x66bd0` to `0x66bdc`, which makes `BombGun::update` (previously
byte-exact, size 1152) differ by 2 bytes, dropping `byte_exact` 4507 -> 4506.
It also violates the "`.rel.plt` byte-identical" gate.  The mission-0
double-deref alone does **not** change `.rel.plt` (verified), so the rotation is
caused by a block-layout change, not by the pool placement per se.  In fact
`.rel.plt` is extremely sensitive to *any* layout change in `process`: adding
just `__builtin_expect(m_bStartSequence != 0, 1)` (no `sepif`) rotates it by
11 B, and `mission-5 expect 0` by 18 B.  Removing the deactivation expect
(`dact_rm`) is the only tested edit that leaves `.rel.plt` byte-identical
(and it does not move the pool).  Because the pool move requires a layout
change, keeping `.rel.plt` byte-identical and moving the pool are in tension
until the whole block order matches the original.

## Sweep results (all size-checked, pool checked)

Base `sepif_nonemission` = `sepif` + deactivation-expect removed: raw 55,267,
size 57,216, pool `0x1380a4`.

Single transforms on `sepif_nonemission` (delta vs 57,216):

| variant | raw | size | pool |
|---|---:|---:|---|
| `m_bStartSequence` expect 1 (`start_e1`) | **55,036** | **57,232** | yes |
| mission-26 expect 0 | 55,120 | 57,220 | yes |
| L2 loop-cond expect 0 | 55,080 | 57,184 | yes |
| mission-5 expect 0 | 55,266 | 57,292 | no |
| mission-5 expect 1 | 55,407 | 57,296 | no |
| mission-26 expect 1 | 55,462 | 57,172 | no |
| deactivation expect added | 55,401 | 57,208 | no |
| L1 expect removed | 55,405 | 57,208 | no |
| mission-0 double-deref | 55,380 | 57,340 | no |
| mission-51 expect 0 | 55,202 | 57,180 | no |
| mission-51 expect removed | 55,345 | 57,164 | no |

Full combinations (up to 4 of `bs1`, `m5_0`, `m26_0`, `m26_1`, `dact`, `l1rm`,
`l2e0`, `m51_rm`) were swept (≈340 builds).  The **only** size-exact (57,232)
pool-correct member is `bs1` (`start_e1`).  Best raw overall is
`m5_0+dact+l1rm` at 54,785 but size 57,292.  Base + any of these transforms
(without `sepif`) never moves the pool.

## Register-role swap (the real blocker)

Original: `sl = this`, `fp = &__stack_chk_guard` (the canary pointer is
`0x20fd04`, `__stack_chk_guard`, not `Globals::status`).  Ours: `fp = this`,
`sl = &__stack_chk_guard`.  `sl` is used 98× in the original and 14× in ours;
`fp` is used 17× in the original and 91× in ours.  This swap costs the +6
prologue, the +4 tail reload (`ldrd fp,r9,[sp,#88]` + redundant
`ldr.w r9,[sp,#92]`), and prevents hoisting `enemies` into `r9` in the
mission-0 loop (the `-12`).

The swap is a greedy-register-allocator decision: `this` is live-in and has
the highest spill weight, so it takes the first callee-saved register in the
allocator order (`r11` in ours).  The original gives `r11` to the canary and
`r10` to `this`, i.e. the canary is allocated first.  Source-level attempts to
flip it failed:

- declaration reorders (`status` first, `player` first, `enemies` first,
  all-declare-then-assign): all keep `this -> fp` (raw 53,711..55,472);
- `LevelScript *self = this;` / `*const self`: coalesced away, neutral;
- `Status *const status`, `Status *&status`: neutral (53,711 / 53,712);
- `Status *volatile status`: 55,086, size 57,240;
- removing the local `Status *status` (direct `Globals::status`): 53,712;
- `-mllvm -regalloc-csr-first-time-cost=0..64`: no change;
- `-mllvm -sink-insts-to-avoid-spills`: **does** flip to `sl = this`
  (frame 0x1a8 instead of 0x1b0) but raw rises to 55,150 (base) / 55,281
  (`start_e1`) and the pool moves away, because the frame shrinks by 8 B.

## Conclusion

No source shape found in this pass beats the base raw **53,711** while staying
size-exact (57,232) and keeping the pool at `0x1380a4`.  The only size-exact
pool-correct shape (`start_e1`, raw 55,036) additionally rotates `.rel.plt`
(9 B) and drops `byte_exact` to 4506, so it was left unlanded.  The structural
blockers are:

1. `this` / `&__stack_chk_guard` register-role swap (greedy allocator);
2. the `else if (mission == 5)` chain forces `bb.34` to be a water block, so
   the pool lands after the second wingman loop unless the chain is a separate
   `if` — which reorders the PLT;
3. the original's Chain-A intro block sits between the mission-5 block and the
   mission-26 check (`0x1381b2..0x1382dc`); our layout puts the mission-26
   check first, so the whole post-pool region is shifted.

Tree left at base; only this note is committed.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11903
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
```

`start_e1` source = base with three edits:
1. `} else if (Globals::status->getCurrentCampaignMission() == 5) {` ->
   `if (Globals::status->getCurrentCampaignMission() == 5) {`
2. drop the `__builtin_expect(Globals::status->getCurrentCampaignMission() >= 2, 1)`
   from the deactivation condition;
3. `if (m_bStartSequence != 0) {` ->
   `if (__builtin_expect(m_bStartSequence != 0, 1)) { // lint: arm_builtin`
