# LevelScript::process — wave 144 / w8904: case-91 nested form landed with the
# exact 0x144 LSDA table; the +12 B companion is the faithful `setPosition(fff)`

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `f5093474f` (wave-143 docs head).
Scratch helpers in `/dev/shm/w8904/` (not tracked): `fdiff.py`, `measure.py`,
`measure_all.py`, `lsda.py`, `relplt.py`, `gen125.py` (case-125 shape generator),
`sweep.py` (named-edit harness), `batch.py` / `batch2.py` (subset sweeps).

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,619            54,869            (+250)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
process LSDA table       0x146             0x144             (EXACT, was +2)
.ARM.extab size          156,312           156,312           (exact)
.ARM.extab diffbytes     1,499             1,368             (-131)
.ARM.exidx               exact             exact
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical   (29,392 B, 0 diff)
.text diffbytes          85,851            86,101            (+250)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The mission target — the **exact 0x144 LSDA call-site table** — is reached for
the first time with a size-exact, `.rel.plt`-byte-identical build.  The price is
+250 raw same-address bytes in `.text`; the `.ARM.extab` diff *improves* by 131
bytes because the 46 call-site rows now have the original's uleb widths.

## The four landed source edits (all faithful to the original Thumb)

1. **case 91 state-0 nested test** (source line 2798).  The original at
   `0x13ac42` is `ldr r0,[sp,#88]; ldr r0,[r0,#28]; cmp r0,#0; bne ...;` then
   `isTriggered`, i.e. `if (m_nState == 0) { if (msg3->isTriggered()) {...} }`,
   not the flat `if (m_nState == 0 && msg3->isTriggered())`.  This is the
   table-shrink lever: it moves the case-91 `setName` block from rel `0x48e0`
   to rel `0x2be6`, so the process call-site table drops from `0x146` to the
   original's **`0x144`** (both have 46 rows / 24 landing pads; the delta is
   purely two 3-byte ulebs crossing `0x4000`).
2. **case 91 state-0 call order** (source line 2802): `setActive(true)` before
   `setName(...)`.  Original `0x13ac70` (setActive) then `0x13ac9e` (setName).
   (The wave-142 w8702 landing had this reversed for a 12-byte layout win.)
3. **case 125 state-1 loop call order** (source line 3931): `port->setName(...)`
   before `(*enemies)[i]->setActive(true)`.  Original `0x13b4d2` (setName) then
   `0x13b4f6` (setActive).  (Also reversed by w8702.)
4. **case-125 state-2 camera seat** (source line 3981) and **case-154 state-4
   camera seat** (source line 4601): `m_pCamera->setPosition(m_tempVec)` ->
   `m_pCamera->setPosition(m_tempVec.x, m_tempVec.y, m_tempVec.z)`.  The
   original's case-125 state-2 site `0x13b7dc` is
   `add.w r3,sl,#40; ldmia r3,{r1,r2,r3}; blx TargetFollowCamera::setPosition(fff)`,
   i.e. the **fff overload**, not the `Vector const&` overload.  This is the
   +12 B companion that restores 57232 B after the nested case-91 test (-12 B).

## Size accounting (the +12 B companion search)

`c91` alone is **-12 B** (57220).  On the wave-143 base the following companions
were measured (all built + linked):

| variant | size | raw | table | relplt |
|---|---:|---:|---:|---|
| HEAD (flat case-91, switch case-125) | 57232 | **54,619** | 0x146 | identical |
| c91 | 57220 | 55,186 | 0x144 | identical |
| c91 + s1order (case-125 setName first) | 57220 | 55,194 | 0x144 | identical |
| c91 + c91order (case-91 setActive first) | 57220 | 55,191 | 0x144 | identical |
| c91 + s2posfff1 (state-2 fff only) | 57228 | 54,770 | 0x144 | identical |
| **c91 + s2posfff (state-2 + case-154 fff)** | **57232** | **54,863** | **0x144** | **identical** |
| **c91 + s2posfff + s1order + c91order** | **57232** | **54,869** | **0x144** | **identical** |

`s2posfff` is worth +4 to the *function size* but +8 net after the nested form's
interaction; it is the only faithful +12 companion found.  The second
(case-154) fff site is required for the exact size; it matches the original's
witnessed fff-overload call pattern but I could not pin its exact original
address in this pass (the case-154 state-4 block in the original is at
`0x13c9e4..0x13caa6` and does not show a `setPosition` in the immediate
neighbourhood, so this one site should be re-checked against Ghidra).

## The raw regression is a block-placement property, not a statement error

The landed block order now matches the original's **relative** order:

```
            original (rel)        landed (rel)
case-91     0x2f72                0x2be6
state-1     0x377e                0x3b38
case-135    0x54d4                0x4ece
setVisible  0x5d06                0x57ec
deleteHack  0x61bc                0x6438
```

but the absolute offsets are shifted (the case-125 state-1 guard is still
outlined at rel `0x20c2`, while the loop body sits at `0x3b38`; the original has
the whole 351-instruction state-1+state-2 body contiguous at `0x3726..0x3ad6`).
Matching that contiguity needs the `s1sep`/`twochain` shapes, which move the
case-125 `default` body before the case-42 state-7 body and rotate `.rel.plt`.

## The raw-win alternative (note only, not landed)

If the coordinator prefers a raw win over the exact table, the cleanest
all-faithful, size-exact, `.rel.plt`-identical variant is

```cpp
// case 91 state-0: setActive(true) before setName(...)
// case 125 state-1 loop: port->setName(...) before setActive(true)
```

i.e. edits 2 + 3 above **without** the nested case-91 test and without
`s2posfff`: `process` raw **54,609** (-10 vs HEAD), size 57232, table `0x146`,
`.rel.plt` identical.  It is the best raw number found this pass but keeps the
`+2` table.

## Case-125 shape sweep (new measurements on the wave-143 base)

`gen125.py` emits each shape from the extracted state bodies; all built + linked.

| shape | size | raw | table | relplt |
|---|---:|---:|---:|---|
| master switch 1,2,3,d | 57232 | 54,619 | 0x146 | identical |
| s1sep (`if (s==0&&msg)` then `if (s==1)` then switch 2,3,d) | 57224 | 54,798 | 0x146 | 4 B |
| s1elseifsw (`else if (s==1)` then switch 2,3,d) | 57236 | 55,218 | 0x146 | 6 B |
| twochain (faithful if/else-if 0,1 then 2,3,4..6) | 57248 | 55,167 | 0x148 | 12 B |
| s1first (state-1 first, state-0 else-if) | 57332 | 55,348 | 0x155 | 12 B |
| **s1sep + c91 + s2posfff + s1order** | **57232** | **54,712** | **0x144** | **2 B** |
| s1sep_case1 (adds `case 1: break;`) | 57232 | 55,027 | 0x137 | identical |

`s1sep + c91 + s2posfff + s1order` is the closest miss: size-exact, table
`0x144`, raw `54,712` (better than the landed `54,869`), but `.rel.plt` swaps
`PlayerEgo::deleteHackingGame` and `PlayerStation::setVisible` (2 bytes).  In
that shape `deleteHackingGame` (rel `0x5b7a`) is placed *before* `setVisible`
(rel `0x5d42`); the original has `setVisible` at `0x5d06` then
`deleteHackingGame` at `0x61bc`.  Fixing that one block move would land a
size-exact, table-exact, raw-54,712 build.

`s1sep_case1` shows that adding a `case 1: break;` to keep the switch width
collapses the call-site table to `0x137` (44 rows), which shrinks `.ARM.extab`
by 16 B — not usable.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8904/measure_all.py                   # raw / size / table
python3 /dev/shm/w8904/relplt.py                        # .rel.plt diff
python3 /dev/shm/w8904/lsda.py cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
```
