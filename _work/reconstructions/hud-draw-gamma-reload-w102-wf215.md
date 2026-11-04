# Hud::draw — wave-102 continuation: the gamma reload, the split-forcing guard, and dead loads

**Status:** research note, 2026-11-04 (wave 102). Worktree `wt/wf215`, branch
`wave102-wf215`, base `master 53d9ee4d5`. The landed pass-1 commit is
`8e450aac9` (`status &&` guard + `int shift`), **93.0 -> 94.5 %**. This note records
what was tried on top of it and why 94.5 stands.

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint **CLEAN**, drift **0**,
**RATCHET PASS**. Target `_ZN3Hud4drawExxP9PlayerEgobjj`, original size **9332 B**.
Landed score **94.5 %** (`branchdiff=55`, `nondiff=155`, normalised 3149 vs 3145).

This extends `hud-draw-gamma-reload-w101-wf114.md`; read that first for the
gamma-block / `this`-split background.

---

## 0. TL;DR

1. The original gamma block is the **plain** three-use form (receiver evaluated
   first, `Globals::status` reloaded for the mission argument). Confirmed again
   from Ghidra (`real 0x163b90` -> Ghidra `0x173b90`):
   `this_01 = *piVar33; getStation(); getIndex(); getCurrentCampaignMission(*piVar33);`
   `getGammaRayDamagePerSecond(this_01, ...)`. The receiver is the *first* load.
2. The `Globals::status &&` guard is the **only** construct found that forces the
   `this` live-range split at `bb.28`. It is a tuned hack (the original has no null
   check at `1642f2`), but without it the split disappears and the score collapses
   to 82.9 % (9264 B).
3. A **new** shape — hoisting the station index into a local — gives the original's
   two `Globals::status` loads **and** the `this` split, but scores **94.3 %**
   (9332 B, `branchdiff=55`, `nondiff=159`). It is 4 instructions worse than the
   landed shape because the receiver becomes the *second* load (the source order
   puts the index before the receiver) and the spill order flips.
4. The original's dead `ldr.w r8,[sp,#108]` at `164176` / `164210` are **not** a
   source hint: the landed shape has the same class of dead reload at `16428c`
   instead. They are allocator spill-reloads of `&Globals::status`.
5. Branch census: original **712** branches, landed **714**. The two extra are the
   guard's `beq.n` and one EH-landing-pad `b.n` (`loc_23ca`). The original's
   split-forcing construct therefore adds **no branch** — but every branch-free
   construct tried (comma operator, `(void)Globals::status`, `?:` with equal arms)
   is optimised away and loses the split.

---

## 1. Measured variant matrix (official verifier, `score_one.py`)

`size` is our function size; `pct` is the official fuzzy score. All variants keep
the landed `int shift` and the `Globals::status && this->field_0x4b8 >= 1` guard
unless stated.

| variant | size | pct | reload | split |
|---|---|---|---|---|
| **landed `Status *st` (one load)** | **9332** | **94.5** | no | yes |
| hoisted `int stationIdx` from `Globals::status` | 9332 | 94.3 | **yes** | **yes** |
| hoisted `Station *station` from `Globals::status` | 9312 | 73.7 | yes | no |
| hoisted `int stationIdx` from `st` | 9312 | 73.8 | yes | no |
| plain three-use | 9312 | 73.8 | yes | no |
| plain + `float`/`bool` gamma local | 9312 | 73.8 | yes | no |
| plain + `int` gamma local | 9336 | 73.2 | yes | no |
| plain + `(void)Globals::status` / `(void)&Globals::status` | 9312 | 73.8 | yes | no |
| plain + `(Globals::status, hitFlags & 0x24)` | 9312 | 73.8 | yes | no |
| landed + guard removed | 9264 | 82.9 | no | no |
| hoisted index + guard removed | 9264 | 82.8 | no | no |
| landed + `(void)Globals::status, f>=1` | 9264 | 82.9 | no | no |
| landed + `(Globals::status, f)>=1` | 9264 | 82.9 | no | no |
| landed + `Globals::status ? f>=1 : f>=1` | 9264 | 82.9 | no | no |
| landed + `Globals::status == Globals::status && f>=1` | 9264 | 82.9 | no | no |
| landed + `f>=1 && Globals::status` | 9336 | 88.2 | no | yes |
| landed + `status && f>0` | 9332 | 94.5 | no | yes |
| landed + `status != 0 && f>=1` | 9332 | 94.5 | no | yes |
| landed + boost `if/else <` (instead of `?:`) | 9324 | 92.8 | no | yes |
| landed + boost `?: >=` (inverted) | 9332 | 94.4 | no | yes |
| landed + `int passengers` (instead of `bool`) | 9332 | 94.1 | no | yes |
| landed + first `shift` as `short` | 9336 | 89.8 | no | yes |
| landed + second `shift` as `short` | 9336 | 89.1 | no | yes |
| landed + `armorY`/`armorBarY`/`top` as `int`/`uint`/`ushort` | 9332 | 94.5 | no | yes |
| landed + `armorBarY` as `short` | 9332 | 94.4 | no | yes |

---

## 2. The hoisted-index shape (the interesting new data point)

Source:

```cpp
    int stationIdx = Globals::status->getStation()->getIndex();
    if (Globals::status->getGammaRayDamagePerSecond(stationIdx,
                                                    Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

Emitted gamma block (ours, 94.3):

```
164010: ldr   r5, [r0, #0]      ; r5 = &Globals::status
164012: ldr   r0, [r5, #0]      ; load #1
164014: blx   Status::getStation
164018: blx   Station::getIndex
16401c: str   r5, [sp, #108]    ; spill after the calls (matches original)
16401e: mov   r4, r0            ; r4 = index
164020: ldr   r5, [r5, #0]      ; load #2
164022: mov   r0, r5            ; receiver = load #2  <-- differs
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r5            ; receiver = load #2  <-- differs
16402c: mov   r1, r4
16402e: blx   Status::getGammaRayDamagePerSecond
```

Original (receiver = load #1, spill between load #2 and the call):

```
164010: ldr   r6, [r0, #0]      ; r6 = &Globals::status
164012: ldr   r4, [r6, #0]      ; load #1
164014: mov   r0, r4
164016: blx   Status::getStation
16401a: blx   Station::getIndex
16401e: mov   r5, r0
164020: ldr   r0, [r6, #0]      ; load #2
164022: str   r6, [sp, #108]
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r4            ; receiver = load #1
16402c: mov   r1, r5
16402e: blx   Status::getGammaRayDamagePerSecond
```

The source order decides which load is the receiver: C++ evaluates the object
expression before the arguments, so only the plain form (receiver textually first)
gives receiver = load #1. Hoisting the index moves the receiver after the calls and
the compiler uses the reload as the receiver. There is no source spelling that both
hoists the index *and* keeps the receiver as the first load without introducing a
`Status *` local, and a `Status *` local removes the split (73.8 %).

---

## 3. Dead `&Globals::status` reloads are allocator artefacts

`[sp,#108]` holds `&Globals::status` in both binaries (stored at `164022` original /
`164012` landed). Reload sites:

| binary | reloads of `[sp,#108]` used as `&Globals::status` |
|---|---|
| original | `164176` (dead), `164210` (dead), `16436e`, `16492a`, `164eba`, `165352`, `1655b2`, `165b4a`, `165d48` |
| landed | `16428c` (dead), `164370`, `16492a`, `164eba`, `165352`, `1655b2`, `165b4e`, `165d46` |

The dead reload just moves from `16428c` (ours) to `164176`/`164210` (original).
It is a greedy-allocator spill reload, not evidence of a missing source construct
(the w101 note hypothesised otherwise).

---

## 4. Conclusion

The landed 9332 B / 94.5 % shape remains the best measured. The original's exact
combination (plain gamma block, receiver = first load, late spill, `this` split,
no null check) was not reproduced by any of ~40 additional shapes. The split is a
greedy-allocator artefact that in our build can only be forced by a real branch
(`Globals::status && ...`), which the original does not have; every branch-free
forcing construct is optimised away. The remaining hypothesis is unchanged: an
unidentified source construct outside the gamma block raises the split weight at
`bb.28` without adding a branch.

---

## 5. Reproduce

```
cd /home/debian/GalaxyOnFire2/wt/wf215
cp /dev/shm/wf215/Hud.best1.cpp src/game/ui/Hud.cpp   # landed 94.5 source
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/wf215/score_one.py                   # pct=94.5 osize=9332 usize=9332
cmake --build cmake-build-match --target check -j8    # RATCHET PASS, byte 4500
cmake --build cmake-build-match --target drift -j8    # 0 fields
```

Ghidra: project `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`,
project name `android_2.0.16_libgof2hdaa.so_ghidra`, program
`android_2.0.16_libgof2hdaa.so`, address `0x173b90` (= real `0x163b90` + image base
`0x10000`).
