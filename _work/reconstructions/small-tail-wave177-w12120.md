# Small tails, wave 177 (w12120): five residual tails re-swept

Status: **no `src/` change landed**.  Worktree `wt/w12120`, branch
`wave177-w12120`, base master `9445ac5fb`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12120`.

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`, size 4524/4524,
`data_exact 843/843`, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical (0).  Whole-`.text` differing bytes 79,591;
`.ARM.extab` 1,321 (size/base exact); `.rodata` 0; `.data` 0.

Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 39 | 11 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every candidate below was compiled with the official TU command and measured
with a **new object-level oracle** (committed beside this note as
`w12120_oracle.py`), then the size-exact candidates were re-linked and
measured with `tools/fn_diff.py`.  Nothing beat the committed raw count, so
`src/` is at base `9445ac5fb`.

## New tool: `_work/reconstructions/w12120_oracle.py`

The prior wave notes used ad-hoc scratch scripts that were not kept.  This
wave's oracle is committed so the next pass can reproduce the instruction
diff in ~0.5 s per variant instead of a full 20 s link:

* compiles one TU with the exact `compile_commands.json` command into
  `scratch/w12120/t.o`;
* disassembles the target function from its own `.text.<mangled>` section
  (the C1 alias of a C2 lives in the C2 section, resolved through `-t`/`-h`);
* decodes the object's inline literal pools (`.word` runs) and drops the
  same byte ranges from the original, so pool words are not mistaken for
  instructions;
* normalises calls through `R_ARM_THM_CALL` relocations and the original's
  `@plt`/`@@Base` annotations to the same symbol name;
* normalises local branches by their **raw 16-bit encoding** (objdump's
  printed target for a `cbz`/`b.n` in a `-ffunction-sections` object is a
  bogus cross-section symbol VMA, so the encoding is the only honest key);
* prints `osize`, `usize`, instruction counts and the `difflib` instruction
  diff.  For the landed base of each target it reproduces the known
  `tools/fn_diff.py` residual exactly (e.g. MGame `diff=6` for raw 16,
  ModStation `diff=9` for raw 39, MenuTouchWindow `diff=18` for raw 28).

Usage:

```sh
python3 _work/reconstructions/w12120_oracle.py src/game/menu/MGame.cpp _ZN5MGame8OnUpdateEv
```

## SpaceLounge::drawLounge — 175/41/1948, no win

Residual is the `pad` register: original `pad=r9`, `x=r5`; ours `pad=fp`,
`x=r9`.  The only differing instructions at the load site are
`ldr.w r9,[r6,#148]` / `mov.w sl,r9,lsl#1` vs `ldr.w fp,[r6,#148]` /
`mov.w sl,fp,lsl#1`; everything else in the mode-0 prologue is identical.
The clamp right arm is the known 4-instruction `movgt/subgt/subgt/addgt`
form (the faithful 2-instruction arm makes clang materialise `mov r5,fp` in
both arms and measures raw 445).

New shapes this wave (object oracle, all size 1948 unless noted):

| shape | diff |
|---|---:|
| master | **35** |
| ternary `x` (single `?:` chain) | 35 |
| `pad2 = pad + pad` used in both `top` and the clamp | 45 |
| `pad2` in `top` only | 35 |
| `pad` before `topLeft`/`bottomRight` | 35 |
| `padClamp` separate reload | 35 |
| `pad` via a `Layout *pl` local | 35 |
| `const int &pad` | 172 |
| `unsigned int pad` | 37 |
| `intptr_t`/`long` pad | 35 |
| `short` pad | 159 |
| `lineHeight` moved before `pad`/`top`/`centerX` | 192–200 |
| `__atomic_signal_fence` after `pad` / before `pad` | 58 / 67 |
| fence after `top` / after `width` | 35 / 35 |
| `x` declared after `top` / at block top | 35 / 35 |
| `xMin`/`xMax` locals | 66 |
| `xMax - width - pad - pad` (flat subtraction) | 50 |
| no `x` local (use `centerX` directly) | 35 |

Full-link confirmations: `pad intptr`, `pad long`, `top pad2 local` all
measure raw **175 / aligned 41 / 1948** exactly.  The `pad` colouring is a
greedy-RA tie that no source shape tested moves to `r9` without changing the
frame.

## Generator::createMission — 40/21/1996, no win

Residual is the bonus block: original stores `rem` at `sp+32` (a dead slot)
and predicates the subtract (`cmp r1,#0; it ne; subne r5,r6,r0`); ours
stores `rem` at `sp+44` and branches (`cbz r1,...; ldr r0,[sp,#44]; subs`).
Frame and size are identical (1996).

New shapes this wave (all size 1996 unless noted):

| shape | diff |
|---|---:|
| master (`volatile int rem` + `volatile int bv` anchor) | **25** |
| `b = b % 50 != 0 ? rawBonus - rem : b` | 25 |
| `volatile const int rem` | 25 |
| `__builtin_expect(b % 50 != 0, 1)` | 25 |
| hoist `int sub = rawBonus - rem` out of the `if` | 135 |
| non-volatile `rem` | 116 |
| `remV` declared at function top (volatile / plain) | 99 / 116 |

Full-link confirmations: `ternary` and `volatile const int rem` both measure
raw **40 / aligned 21 / 1996**.  The `sp+32` reuse is a stack-slot colouring
tie; moving the declaration or changing the type only makes it worse.

## ModStation::ModStation C1/C2 — 39/11/1008, no win

Residual is the `pitchTable`/`yawTable` materialisation at `0xd5438`:
original loads `YawIPad` into `r3` and `Yaw` into `r4`; ours loads
`YawIPad` into `r4` and `Yaw` into `r3` (the pool slots `d56b4`/`d56b8` and
the `str`/`ldrd` stack slots `[sp,#176]`/`[sp,#180]` are otherwise
identical).  The selection is `mov r0,r3; cmp; it eq; moveq r0,r4`.

New shapes this wave (all size 1008, C1/C2 aliased):

| shape | diff |
|---|---:|
| master (fence + `iPad ? YawIPad : Yaw`) | **9** |
| `const float *const` both tables | 9 |
| `pitchTable` local removed (direct `gHangarCamPitch[race]`) | 9 |
| `yawTable` declared before `pitchTable` | 9 |
| swap `pitchBits`/`yawBits` order | 38 |
| `yawTable` local removed | 40 |
| remove the `__atomic_signal_fence` | 10 |
| fence after the two table declarations | 9 |

Full-link confirmations: `const ptr` raw **39/11/1008**; `no pitch local`
and `yaw before pitch` raw **42/12/1008** (worse).  The `r3`/`r4` swap is a
pure allocator tie; the first-loaded operand always takes `r4` in our
compile.

## MenuTouchWindow::OnTouchBegin — 28/16/1896, no win

Two independent RA ties remain:

1. case 3 (options/iPad), first up-button condition: original
   `ldr.w ip,[sl,#340]` + `add r3,ip`; ours `ldr.w r4,[sl,#340]` +
   `add r3,r4` (4 bytes).  `r4` is dead at that point in both.
2. case 15 scrollbar-hit block: a five-register permutation around the
   `heapBufB[field_0x1e0]` index and the `GetImage2DWidth/Height` calls
   (original `r3/r6/r2/r5/r2` vs ours `r2/r3/r6/r4/r5`).

New shapes this wave (all size 1896 unless noted):

| shape | diff |
|---|---:|
| master | **18** |
| case-3 term4 `top + margin + width` | 18 |
| case-3 term4 associations `margin+(top+width)`, `(margin+top)+width`, `top+(margin+width)`, `+0`, `(long)width` | 18 each |
| case-3 `layout` pointer local | 18 |
| case-3 `iPad != 0` | 18 |
| case-3 `__builtin_expect(term4,1)` | 18 |
| case-3 fence before the `if` / after `downButtonPressed` | 20 / 20 |
| case-3 term4 subtract form / `gt` form | 36 / 35 |
| case-3 `entryW` local (one or both conditions) | 33 / 81 |
| case-3 term permutations `3012`, `0312`, `1023` | 112 / 67 / 31 |
| case-15 `Layout *layout` pointer | 18 |
| case-15 `int w = Globals::w` local | 18 |
| case-15 `img` / `imgW` / `imgW+imgH` locals | 57 / 69 / 98 |
| case-15 swap the two condition terms | 79 |

Full-link confirmations: `top+margin+width` and `layout ptr` both measure
raw **28 / aligned 16 / 1896**.

## MGame::OnUpdate — 16/6/11440, no win

The m122 radio block is the only differing region (the other 11,424 bytes
are identical):

```
orig  ldr.w r0,[r9]        ; status
      blx   getCurrentCampaignMission
      movw  r2,#3166
      ldr.w r1,[fp,#120]   ; this->level  (loaded AFTER the call)
      cmp   r0,#122
      it    eq
      movweq r2,#3164
      add   r2,r4          ; radioBase + radioIndex
      mov   r0,r1
      movs  r1,#28
      blx   createRadioMessage
ours  ldr.w r0,[r9]
      ldr.w r5,[fp,#120]   ; level hoisted BEFORE the call
      blx   getCurrentCampaignMission
      movw  r1,#3166
      cmp   r0,#122
      it    eq
      movweq r1,#3164
      mov   r0,r5
      adds  r2,r1,r4
      movs  r1,#28
      blx   createRadioMessage
```

The landed `Level *level = this->level` local is what keeps the
`&Globals::status` GOT base in `r9`: it is a non-rematerialisable load live
across the `getCurrentCampaignMission` call, so the allocator cannot split
the GOT base into `r5`.  Any shape that loads `level` after the call frees
`r5`, the GOT base splits into `r5`, and the function shrinks to 11,392 B
(raw 5,148–5,196).

New shapes this wave (all size 11,440 unless noted):

| shape | diff |
|---|---:|
| master (`Level *level` before the ccm) | **6** |
| `Level *const level` | 6 |
| `Level &level = *this->level` | 6 |
| `__builtin_assume(level != nullptr)` / `if (!level) __builtin_unreachable()` | 6 / 6 |
| `if/else` for `radioBase` (instead of `?:`) | 6 |
| `this->level` direct (no local) | 503 (11,392) |
| `Level *level` declared after `radioBase` | 503 (11,392) |
| `int ccm = ...;` then `Level *level` | 503 (11,392) |
| `Level *&` / `Level *const &` reference | 503 |
| `this->level = level;` self-store | 28 (11,448) |
| `if (level == nullptr) return;` | 836 |
| `if (level != this->level) return;` | 503 |

Full-link confirmations: `Level *const` and `Level &` both measure raw
**16 / aligned 6 / 11,440**; `if/else radioBase` also **16 / 6 / 11,440**.
The only remaining lever is to keep `status` in `r9` while loading `level`
after the call; every spelling that does the latter frees `r5` and re-splits
the GOT base.

## Conclusion

No measured win this wave.  The five tails are unchanged at
**175/41 (1948), 40/21 (1996), 39/11 (1008), 28/16 (1896), 16/6 (11440)**
raw/aligned/size.  `src/` is at base `9445ac5fb`; only this note and
`w12120_oracle.py` are added.  Full `check` green: byte_exact 4507,
linked_exact 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Remaining levers, in order of expected payoff:

1. **MGame**: keep `status` in `r9` while loading `level` after the ccm.
   The non-rematerialisable value that holds `r5` across the call must be
   something other than the `level` pointer itself; no source spelling
   tried produces it.
2. **MenuTouchWindow**: force `r4` live at the case-3 term-4 load so the
   allocator picks `ip`; then re-permute the case-15 scrollbar block.
3. **ModStation**: flip the first-loaded ternary operand from `r4` to
   `r3` without changing the 1008-byte frame.
4. **Generator**: reuse the dead `sp+32` slot for `rem` and predicate the
   subtract; the current `volatile` read is what forces the `cbz` branch.
5. **SpaceLounge**: colour `pad` to `r9` and `x` to `r5` without the
   `field_0x94` reload (raw 310) or 64-bit pad arithmetic (size 2008).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12120
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN5MGame8OnUpdateEv
# object-level oracle (single TU, ~0.5 s):
python3 _work/reconstructions/w12120_oracle.py src/game/menu/MGame.cpp _ZN5MGame8OnUpdateEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12120` (`oracle.py`,
`sweep.py`, `fulltest.py`, `showblk.py`, `MGame.base.cpp`, `t.o`).
