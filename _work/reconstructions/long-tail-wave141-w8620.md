# Long-tail wave 141 (w8620): Gun / Generator / SpaceLounge / Radar extab —
# plain-clamp truth for SpaceLounge, weight/placement probes, no landed `src/` change

Status: **no landed `src/` change.** Worktree `wt/w8620`, base `e18e0c2a4`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave139-w8420.md`, `long-tail-wave138-w8320.md`,
`gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`, `extab_lsda_scope_w7742.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` `0x1337c8` (86,472 differing
bytes), `.ARM.extab` `0x26298` (1,411 differing bytes). Raw same-address diff
bytes (fdiff): Gun **463**, Generator **445**, SpaceLounge **175**,
Radar extab **7**.

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (1984 B) | **new negatives:** named `Vector kpos` in the k-loop scope (both `*td = kpos` and a nested `{ }` scope) is *identical* to HEAD (463 raw / 1984 B); `*td = getPosition() - position` 1760/1956; `*td = *td - position` 1743/1936 |
| `Generator::createMission` (1996 B) | **new:** `__builtin_expect((type\|4)==12, 0)` reaches raw **386** but frame 2004 B (bonus r9 / rem r5); adding a second expect on the inner `bonus % 50 != 0` does **not** change the 1801/1996 reshuffle |
| `SpaceLounge::drawLounge` (1948 B) | **new truth:** the shipped clamp is the *plain* `width + pad*2` (`addgt.w r1, r6, sl`), not HEAD's `short pad3` sign-extend; plain is 445 raw / 1948 B with pad=fp, x=r9. Best plain-clamp variant: expect-on-clamp 425 raw. HEAD's pad3 form (175) is a wrong-source/better-score accident |
| `.ARM.extab` `Radar::Radar` (7 B) | `enemy_compound` fixes row 1 (2462) but moves row 19 to 2368; every other shape is >= 7. No flip |

## 1. SpaceLounge: HEAD's `pad3` clamp is not what the shipped binary does

The shipped right-edge clamp (`0x1712dc`):

```text
1712dc: bfc4       itt gt
1712de: eb06 010a  addgt.w r1, r6, sl      ; width + pad*2
1712e2: 1a45       subgt   r5, r0, r1      ; x = w - inset - (width + pad*2)
```

`sl` is `pad*2`, computed once at `0x170ed2` (`mov.w sl, r9, lsl #1`) and
reused from the `top` computation (`vsub.f32 s0, s18, s4`).  So the original
source is the **plain** `width + pad * 2`, with `pad` an `int` (the load is
`ldr.w r9, [r6, #148]`, 32-bit).

HEAD instead uses `short pad3 = pad; ... (width + pad3 * 2)`, which clang
lowers to a 16-bit sign-extend:

```text
ittt gt
movgt.w r1, fp, lsl #16
addgt.w r1, r6, r1, asr #15
subgt.w r9, r0, r1
```

That is 4 instructions instead of the shipped 3.  HEAD is still the better
score (175 vs 445) only because it keeps the *early* branch offsets identical
(HEAD 740 instructions, plain 741 = shipped 741).  The plain form's first size
divergence is at instruction 529:

```text
orig 0x58c: add.w r5, r0, r9      ; x = inset + pad, x in r5
ours 0x58c: mov   r5, fp          ; r5 = pad
ours 0x58e: add.w r9, r0, fp      ; x = inset + pad, x in r9
```

i.e. the shipped colouring is **pad=r9, x=r5**; ours is **pad=fp, x=r9**.

### RA trace of the plain form (instrumented clang, `RA_TRACE=1`)

```text
HINTDBG drawLounge pad weight=2.077779e-04 size=6368 li=%2 [496r,6064B:0)[6752B,7552r:0)
FINALDBG drawLounge raw=2147484255 size=4220 w=2.946140e-04 li=%607 ...
select %2 ... try $r4 i0=%394(w=2.24e-04) %7(3.85e-04) %20(4.54e-04) %465(6.64e-04)
              try $r5 i0=%19(4.71e-04) %46(6.25e-04)
              try $r6 i0=%606(4.53e-04)
              try $r8 i0=%1(2.27e-04)
              try $r9 i0=%607(2.95e-04)     <- x
              try $r10 i0=%47(6.08e-04)
              try $r11 i0=%365(2.15e-04)
FINAL %2 -> $noreg                          ; pad spilled / rematerialised
```

`x` (`%607`) is allocated first (weight 2.95e-04 > pad 2.08e-04) and takes
`r5`; it is then split and its second range takes `r9`.  Pad can evict
neither: every callee-saved candidate holds a heavier interval.  The shipped
build has the opposite priority outcome (pad keeps r9, x keeps r5).

### Variants measured this wave (all 1948 B unless noted)

| variant | raw diff | size | pad | notes |
|---|---:|---:|---|---|
| HEAD (`short pad3` clamp) | **175** | 1948 | fp | wrong clamp, best score |
| plain `width + pad*2` | 445 | 1948 | fp | shipped clamp, wrong colouring |
| plain + `__builtin_expect(clamp, 0)` | **425** | 1948 | fp | best plain-clamp result |
| plain + `__builtin_expect(clamp, 1)` | 429 | 1948 | fp | |
| plain + expect on 2nd clamp | 445 | 1948 | fp | no change |
| plain, split subtraction `w - inset - width - pad*2` | 445 | 1948 | fp | |
| plain, `w - inset - width - (pad+pad)` | 445 | 1948 | fp | |
| plain, `w - inset - width - pad - pad` | 464 | 1944 | fp | |
| `pad3` in `top` + plain clamp | 1838 | 1968 | fp | |
| `pad3` clamp + `top` uses `pad*2` | 175 | 1948 | fp | = HEAD |
| no `__builtin_expect(isKnown)` | 514 | 1948 | fp | |
| `__builtin_expect(isKnown, 0)` | 1835 | 1984 | fp | |
| `boxW` after `lineHeight` (before `drawBox`) | 1833 | 1968 | r9 | same as before `lineHeight` |
| `short boxW` | 1848 | 1964 | fp | |
| `x` declared before `top` | 1847 | 1972 | fp | |
| pad/x/lineHeight/layout reordered | 1851 | 1964 | fp | |
| pad declared after `top` | 524 | 1948 | fp | |
| HEAD + expect-on-clamp | 486 | 1952 | fp | |

No source shape reached pad=r9 at 1948 B.  The `boxW` shapes that reach r9
(1968 B) have 734 instructions vs the shipped 741 — a genuinely different
allocation, not a register swap.

## 2. Generator: expect-on-outer with weight 0 gets raw 386 but +8 B

The faithful block (shipped `0x9770c..0x97722`) is

```text
adds  r5, r1, r6        ; bonus = rem + rawBonus
str   r1, [sp, #32]     ; spill rem
movs  r1, #50
mov   r0, r5
blx   __aeabi_idivmod
ldr   r0, [sp, #32]     ; reload rem
cmp   r1, #0
it    ne
subne r5, r6, r0        ; bonus = rawBonus - rem
str   r5, [sp, #16]     ; bonus -> sp+16
ldr   r5, [sp, #24]
```

New measurements this wave:

| variant | raw diff | size | bonus/rem |
|---|---:|---:|---|
| HEAD (`bonus -= rem*2`) | **445** | 1996 | r6 / r5 |
| `__builtin_expect((type\|4)==12, 1)` + faithful | 1801 | 1996 | exact block, reshuffled |
| same + expect on inner `bonus % 50 != 0` (0) | 1801 | 1996 | no change |
| same + expect on inner `bonus % 50 != 0` (1) | 1801 | 1996 | no change |
| `__builtin_expect((type\|4)==12, 0)` + faithful | **386** | 2004 | r9 / r5 |
| above + change the `race<4 && type==13` expect to 0 | 1879 | 2004 | |
| above + drop the `race<4 && type==13` expect | 1793 | 2008 | |

So the weight-0 outer expect is the first shape to beat HEAD's raw count
(386 < 445), but it costs 8 bytes (frame 104 -> 108).  The inner expect does
not change rem's weight, confirming w8420: the lever must be on the
`(type|4)==12` branch, and that lever also reshuffles block placement.

## 3. `.ARM.extab` Radar::Radar: the two pads are byte-identical code

The record is `fn 0x12d484`, `reclen 256`, `diffbytes 7`.  The two landing
pads are `0x99e` and `0x9a0`, both `b.n 0x12de28` (2-byte branch to the same
cleanup dispatcher).  Only the LSDA row assignment differs:

| row | orig lp | ours lp |
|---|---:|---:|
| 1, 3, 11, 13, 15 (outer cleanup) | 2462 (0x99e) | 2464 (0x9a0) |
| 19, 21 (enemyGuns) | 2464 (0x9a0) | 2462 (0x99e) |

i.e. a pure emission-order swap of the two `landingpad` blocks.  New shapes
(all measured with `extab_lsda_decode.py`):

| shape | extab diff |
|---|---:|
| HEAD | **7** |
| playerGuns compound scope | 7 |
| `if (playerGuns)` / `if (enemyGuns)` truthy | 7 |
| `for (int i ...)` (player / enemy / both) | 7 |
| enemyGuns compound scope | 7 |
| enemyGuns compound scope | 11 (row 1 -> 2462 correct, row 19 -> 2368) |
| both blocks compound | 28 |
| swap playerGuns/enemyGuns block order | 38 |
| both pointers declared at top | 38 |
| `while (playerGuns != nullptr)` | 34 |
| enemyGuns nested `if (getEnemyGuns() != nullptr)` | 39 |
| enemy loop with local `AbstractGun *g` | 38 |
| player loop with local `AbstractGun *g` | 38 |
| enemy loop with cached `unsigned n` | 38 |

`enemy_compound` is the only shape that fixes the outer pad, but it moves the
enemy pad to a different cleanup (2368).  The two pads are interchangeable
code, so only a change in `landingpad` block emission order (not in the loop
bodies) can flip them; no such source lever was found.

## 4. Gun: named k-loop temp in loop scope is a no-op

| variant | raw diff | size |
|---|---:|---:|
| HEAD (`*td = enemy->getPosition(); *td -= position;`) | **463** | 1984 |
| `Vector kpos = enemy->getPosition(); *td = kpos; *td -= position;` | 463 | 1984 |
| same but `kpos` in a nested `{ }` scope | 463 | 1984 |
| `*td = enemy->getPosition() - position;` | 1760 | 1956 |
| `*td = enemy->getPosition(); *td = *td - position;` | 1743 | 1936 |

A named k-loop `Vector` in the loop scope does not change the frame-index
order, so the `%stack.10`/`%stack.11` LICM hoist order is unchanged.  The
w7739/w8320 conclusion stands: the shipped is the `expr` grouping with a
clean preheader (no hoisted stack-address adds).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8620
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8620/fdiff.py <mangled>                 # raw same-address diff
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                         # Radar record fn 0x12d484
```

Scratch (not committed), `/dev/shm/w8620/`: `fdiff.py`, `show.sh`,
`dis1.py` (per-symbol orig/ours disassembly from raw bytes),
`cc_{sl,gen,radar}.sh` (exact per-TU compile), `sl_try.py`, `gen_try.py`,
`radar_try.sh`, `sl_batch6..10.py`, `gen_batch9.py`, `radar_batch2..3.py`,
`gun_batch3.py`, plus `SL.head.cpp` / `GEN.head.cpp` / `GUN.head.cpp`.
RA evidence: `sl_plain_trace.txt` (`RA_TRACE=1` with the instrumented clang
`/tmp/llvm_android_702b/build2/bin/clang++`).

## 6. Files changed

`_work/reconstructions/long-tail-wave141-w8620.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
