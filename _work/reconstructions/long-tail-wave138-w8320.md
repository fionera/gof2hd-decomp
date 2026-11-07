# Long-tail wave 138 (w8320): Gun / Generator / SpaceLounge / extab —
# new slot-merge and RA-tie probes, no source win

Status: **no landed `src/` change.** Worktree `wt/w8320`, base `96b069792`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave137-w8220.md`, `long-tail-wave135-w8118.md`,
`gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`,
`gun-calccharactercollision-wave119-w6713.md`,
`extab_lsda_scope_w7742.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` 85,535 differing bytes,
`.ARM.extab` 1,395 (unchanged).

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (93.4 %, 1984 B) | **new negative:** the residual is a stack-slot merge transpose, not a hoist-order transpose; `Vector diff = ep - pp` / `Vector diff;` / `Agent *a = agent` shapes all stay 220 diffs; `Matrix tmp;` alias grows to 2000 B |
| `Generator::createMission` (97.2 %, 1996 B) | **new negative:** plain faithful `bonus = rawBonus - rem` gets the exact `adds r5,r1,r6` / `subne.w r5,r6,r9` block but frame **104** and `rem` in **r9** (2000 B / 83.8); `Agent *a = agent` alias and `volatile int bonus` do not move `rem` to sp+32 |
| `SpaceLounge::drawLounge` (95.7 %, 1948 B) | **new negative:** `short pad2 = pad * 2` in both call sites = 1952 B, pad=fp, x=r9; `pad3 * 2 + width` in both = 1968 B, pad=r4 |
| `.ARM.extab` `Radar::Radar` (7 B) | not reached (time-boxed after the Gun/Generator slot work) |

## 1. Gun: the residual is `StackColoring`, not `MachineLICM`

The w7610/w6713 analysis is confirmed and sharpened with the current
`-mllvm -print-machineinstrs=early-machinelicm` MIR dump.

The two `add rX, sp, #44|104` instructions in the scatter block are **not**
hoisted copies: the loop preheader `bb.5` contains the hoisted
`%308 = t2ADDri %stack.10.view` / `%282 = t2ADDri %stack.11.tmp` pair, but the
register allocator rematerialises both at the scatter use site.  The shipped
binary has the same rematerialisation; the only difference is the slot that
each 12-byte temp is merged into.

`-mllvm -debug-only=stack-coloring` for the HEAD build:

```
Slot #4  12 B  playerPosition
Slot #5  12 B  ref.tmp            (enemyPosition - playerPosition)
Slot #10 60 B  view               (memcpy dest, sp+104)
Slot #11 60 B  MatrixSetTranslation sret (sp+44)
...
Merging #10 and slots #4 together.   // playerPosition -> view slot (sp+104)
Merging #11 and slots #5 together.   // ref.tmp        -> tmp  slot (sp+44)
```

Shipped wants the opposite merge: `playerPosition` at sp+44 (the `tmp` slot)
and `ref.tmp` at sp+104 (the `view` slot).  `StackColoring` processes the two
60-byte survivors in frame-index order (`#10` first) and greedily absorbs the
first disjoint 12-byte slot, so the merge is decided by the **frame-index
order of the two scatter temporaries**, exactly as w6713 §2 stated.

The expression form (`VectorLength(enemyPosition - getPosition())`) creates
the binary-op result temp before the `getPosition()` sret temp, which swaps
`#4`/`#5` and therefore the merge — the slots match the shipped binary, but
the register map becomes `playerPosition=r5`, `delta=r4` (92.6 %).  HEAD has
the shipped register map (`r4`/`r5`) but the transposed slots (93.4 %).

### New variants measured this wave

| variant | pct | size | slots (pp, delta) |
|---|---:|---:|---|
| HEAD | **93.4** | 1984 | r4/104, r5/44 |
| `Vector diff = enemyPosition - playerPosition; VectorLength(diff)` | 93.4 | 1984 | r4/104, r5/44 |
| `Vector diff;` before `playerPosition`, then `diff = ep - pp` | 63.3 | 1964 | r4/44, r4/104 |
| `Agent`-style alias / nested scope around `playerPosition` | 93.4 | 1984 | r4/104, r5/44 |
| `Matrix tmp;` before `view` (named sret alias) | 50.0 | 2000 | frame +16 |
| `if (scatter) ... else if (mine)` source swap | 60.3 | 1976 | body re-roll |

The `Matrix &view = *CameraGetLocal(...)` variant (w6713) still reaches the
exact shipped slot+register map (`pp=r4/sp+44`, `delta=r5/sp+104`) but drops
the `__aeabi_memcpy` and scores 80.9 %.  No shape found that keeps the memcpy
and swaps the merge.

## 2. Generator: the faithful block is exact, the frame is not

The shipped block `0x9770c..0x97722` is:

```
9770c adds r5, r1, r6      ; bonus = rem + rawBonus
9770e str  r1, [sp, #32]   ; spill rem
97710 movs r1, #50
97712 mov  r0, r5
97714 blx  __aeabi_idivmod
97718 ldr  r0, [sp, #32]   ; reload rem
9771a cmp  r1, #0
9771c it   ne
9771e subne r5, r6, r0     ; bonus = rawBonus - rem
97720 str  r5, [sp, #16]   ; spill bonus
97722 ldr  r5, [sp, #24]
```

The **plain** faithful spelling (not tried before; w8220 only tried it with
`volatile int rem`):

```c++
int rawBonus = (int) (reward * standing->getMissionBonus(agent->getRace()));
int rem = rawBonus % 50;
bonus = rawBonus + rem;
if (bonus % 50 != 0) {
    bonus = rawBonus - rem;      // HEAD used: bonus -= rem * 2;
}
```

emits the exact `adds r5,r1,r6` / `cmp/it ne/subne.w r5,r6,r9` form, but:

```
9770e mov  r9, r1          ; rem -> r9        (shipped: str r1,[sp,#32])
97720 str  r5, [sp, #20]   ; bonus -> sp+20   (shipped: sp+16)
97722 ldr  r5, [sp, #28]   ; level            (shipped: sp+24)
frame 104 (shipped 96), size 2000 B, pct 83.8
```

The extra 8 bytes are the `agent` spill slot: the allocator chooses to spill
`agent` (long live range, one reload at `getImageParts`) and keep `rem` in
`r9`, whereas the shipped build spills `rem` (one store + one reload) and
keeps `agent` in `r9`.  This is a greedy-RA tie-break, not a source-order
effect.

New attempts to break the tie:

| variant | pct | size | rem | frame |
|---|---:|---:|---|---:|
| HEAD (`bonus -= rem*2`) | **97.2** | 1996 | r5 | 96 |
| plain faithful (`bonus = rawBonus - rem`) | 83.8 | 2000 | r9 | 104 |
| faithful + `Agent *a = agent` alias | 83.8 | 2000 | r9 | 104 |
| `volatile int bonus` + faithful | 79.3 | 1992 | r5 | 96 |
| `volatile int b2` + faithful (w8220) | 96.7 | 1996 | r9 | 104 |

The `volatile int b2` shape is still the closest (exact adds/subne, only the
`rem` store and the +4 slot shift differ), but every way found to force `rem`
to a slot also grows the frame or re-rolls the tail.

## 3. SpaceLounge: the `boxW` lever still costs 20 B

| variant | pad | x | size |
|---|---|---|---:|
| HEAD | fp | r9 | **1948** |
| `short pad2 = pad * 2;` used in both call sites | fp | r9 | 1952 |
| `pad3 * 2 + width` in both `drawBox` and `DrawRectangle` | r4 | fp | 1968 |
| w7740 `int boxW = pad*2+width` before `drawBox` | r9 | r5 | 1968 |

`short pad2` keeps the HEAD register map but adds one instruction (the
sign-extend of `pad2`), so it is 4 B over.  `pad3 * 2` in both call sites
re-rolls `pad` to r4.  The w7740 `boxW` target colouring (pad=r9, x=r5) still
costs 20 B because `lineHeight` moves to r8 and `pad*2` to fp.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8320
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
```

Scratch (not committed), `/dev/shm/w8320/`: `score2.sh` (full link + verify
one symbol), `pct.py`, `hoist.sh` (instrumented clang
`/tmp/llvm_android_702b/build2/bin/clang++`), `mir_early.txt` (pre-RA MIR),
`mir_licm.txt` (post-RA MIR), `stackcol.txt` (StackColoring merge log),
`show_gunhead.txt`, `show_genplain.txt`, `show_slpad2.txt`, and the variant
`.cpp` files.

## 5. Files changed

`_work/reconstructions/long-tail-wave138-w8320.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
