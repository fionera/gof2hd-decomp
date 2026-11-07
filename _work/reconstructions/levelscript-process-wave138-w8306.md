# LevelScript::process — wave 138 / w8306: mission-0 low-register map + state-4 body order

Status: **analysis only, no source change landed**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `96b069792` (wave-137 head, which
already contains w8201's four `getPlayer` receiver fixes). Scratch helpers in
`/dev/shm/w8306/` (not tracked): `score.py`, `db.py`, `regdiff.py`, `calls.py`,
`variants.py`, `v2.py`…`v9.py`, `LevelScript.base.cpp`.

## Result

```
                         baseline (HEAD)   best experiment (m4bc_rgd)
official verify pct      57.4              57.4
orig / ours size         57232 / 57232     57232 / 57236   (+4, not landable)
process diffbytes        55151             55039           (-112)
byte_exact               4507              2741 (size shift)
linked_exact             4507              4507
size-exact               4524/4524         -
```

The mission's stated baseline was 58.9 %; on the actual wave-137 head the
official fuzzy score is **57.4 %** (w8201/w8202/w8205/w8207 re-rolled
`MachineBlockPlacement` after the wave-136 note). All numbers below are measured
on this head.

## 1. Mission-0 state-0 setup: the low-register map (confirmed)

The original's state-0 setup (`0x137e9c`) uses a **register permutation** that
ours does not:

```
orig 137e9c  mov   r4, sl          ; this -> r4
     137ea4  str   r0, [r4,#28]    ; m_nState = 1     (2-byte str)
     137ea6  mov   sl, fp          ; status -> sl     (fp freed for temp2)
     137eb2  ldr   r0, [r4,#20]    ; m_pCamera        (2-byte ldr)
     137f24  ldr.w r9, [sp,#96]    ; enemies -> r9 (hoisted BEFORE loop)
     137f28  add.w fp, sp, #280    ; fp = &temp2
     137f2c  mov.w r4, #1.0f       ; r4 = 1.0f
     137f76  mov   fp, sl          ; status restored
     137f78  ldr.w sl, [sp,#88]    ; this restored
ours 137ea4  str.w r0, [sl,#28]    ; this stays in sl (4-byte str.w)
     137eac  mov   r9, fp          ; status -> r9
     137ebc  ldr.w r0, [sl,#20]    ; m_pCamera (4-byte ldr.w)
     137f2a  add.w sl, sp, #280    ; sl = &temp2
     137f2e  mov.w fp, #1.0f       ; fp = 1.0f
     137f72  ldrd  sl, r9, [sp,#88]; this/status restored
```

The state-1/2/3 wobble blocks (`0x1383fe`/`0x138440`/`0x138482`/`0x1389d4`)
already match instruction-for-instruction; the state-0 setup is the only
mission-0 block with the swap.

## 2. The double-index loop is the cause, and it re-colours the prologue

Original loop (`0x137f34`) indexes `(*enemies)[i]` **twice** (reloads
`enemies->data` for both calls) and keeps `enemies` in `r9` across the loop.
Our source caches the element in a local, so clang emits one load and reuses it.

The faithful form is exactly:

```cpp
(*enemies)[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
(*enemies)[i]->geometry->setVisible(true);
```

**Every spelling coalesces to the same bytes** (measured identical: direct
double-index, `KIPlayer *enemy` first call only, `KIPlayer *enemy` second call
only, `AEGeometry *g` cache, `Array<KIPlayer*> &en`, `Array<KIPlayer*> *en`
alias). All give **57292 B (+60), pct 50.8**.

The +60 is not the loop body (+12 B); it is a **global re-colour**: the extra
live use of `enemies` makes `this` move out of `sl` and `delta` out of `r6`:

```
baseline prologue:  mov sl,r0 ; mov r4,sl ; mov r6,r1 ; ldr.w fp,[r0]
DI prologue:        mov sl,r0 ; mov r5,sl ; mov r4,r1 ; ldr r6,[r0]
```

The baseline prologue's first three assignments (`this`→`r4`, `delta`→`r6`,
`stackguard`→`fp`) already match the original exactly; the DI loop destroys
them. This is the same `sl`-ownership wall as w8103/w7765/w8204.

## 3. NEW finding: the state-4 teardown has four misplaced calls

This is a genuine **body-order** defect not reported before. The original's
state-4 teardown (`0x138d2a`) is:

```
msg10->isOver()
sound->stop(player->field_0x1c)
sound->play(143,...)
player->setTurretMode(false)
resetCamera(this, m_pLevel)
player->setFreeLookMode(false)               <-- 0x138d6c
m_pCamera->enableFirstPersonCam(false)       <-- 0x138d74
player->hideShipForFirstPersonCameraView(false) <-- 0x138d7e
m_pLevel->getPlayer()                        <-- 0x138d86 (FRESH getPlayer)
m_bCinematicBreak = 1                        <-- 0x138d8c
setCollide(false)
m_pRadar->enabled = 0
player->setComputerControlled(true)
player->player->removeAllGuns()              <-- 0x138daa
m_pCamera->setLookAtCam(true)
m_pCamera->setTarget(player->geometry)
player->geometry->setRotation(0, pi/2, 0)
m_pCamera->setPosition(player->geometry->getPosition())
m_pCamera->translate(...)
m_nState = 5
```

Our source has the four calls (`setFreeLookMode`, `enableFirstPersonCam`,
`hideShipForFirstPersonCameraView`, `removeAllGuns`) at the **start of state 5**
instead, and uses the cached `player` for `setCollide`. The original's state 5
(`0x138fd8`) starts directly at `player->getPosition()` / `getSpeed()`.

The correct source order is therefore:

```cpp
resetCamera(m_pLevel);
player->setFreeLookMode(false);
m_pCamera->enableFirstPersonCam(false);
player->hideShipForFirstPersonCameraView(false);
m_bCinematicBreak = 1;
m_pLevel->getPlayer()->setCollide(false);
m_pRadar->enabled = 0;
player->setComputerControlled(true);
player->player->removeAllGuns();
m_pCamera->setLookAtCam(true);
```

and state 5 drops the four leading calls.

## 4. Measured size deltas (all built + scored on the wave-137 head)

| variant | size | pct | diffbytes |
|---|---:|---:|---:|
| **baseline** | **57232** | **57.4** | **55151** |
| `move4b` (4 calls in state 4, fresh getPlayer) | 57240 | 56.9 | 55210 |
| `move4b_cached` (same, cached player) | 57224 | 57.4 | 55283 |
| `move4b_cached + rgd` (missing `resetGunDelay` @0x144982) | 57236 | 57.4 | **55039** |
| `move4b + rgd1` (line-3196 resetGunDelay → cached) | **57232** | 56.9 | 55228 |
| `s4gp` (state-4 setCollide fresh getPlayer only) | 57232 | 57.4 | 55151 (byte-identical: CSE'd) |
| `swap45` (state-4/state-5 branches swapped) | 57248 | 55.4 | — |
| `move4b + swap45` | 57240 | 55.3 | — |

The `s4gp` result proves `m_pLevel->getPlayer()` is CSE'd with the prologue's
`player` when the result is only used for `setCollide` — the "fresh getPlayer"
is invisible unless it sits between two other calls (as in the state-4
teardown, where it is +16 B).

## 5. Why the best experiment is not landable

`move4b_cached + rgd` improves raw diffbytes by 112 but is **+4 B**, so the
following functions shift and `byte_exact` collapses.

`move4b + rgd1` lands at exactly 57232 but **drops `byte_exact` 4507 → 4503**:
the four `MGame` functions (`successCheck`, `switchCamera`, `dialogueEvent`,
`gameOverCheck`) flip from byte-exact to fuzzy-100 %.  This is the **PLT-order
sensitivity** documented in w8201: changing which `getPlayer`/`resetGunDelay`
call site is the first reference reorders `.rel.plt`, which shifts the PLT
stubs and therefore the `blx` encodings in every TU.  A landable change must
leave the PLT reference order untouched.

## 6. Conclusion / next leads

1. The mission-0 state-0 swap is caused by the double-index loop; the DI loop
   costs +60 because it evicts `this` from `sl`.  No local spelling avoids it.
2. The state-4 teardown really is missing four calls (they live in state 5 in
   our source) and uses a cached `player` where the original calls
   `m_pLevel->getPlayer()` fresh.  This is the most promising remaining
   faithfulness fix, but it needs a **+8 B** companion that does not perturb
   `.rel.plt` (the `rgd` is +12, the fresh `getPlayer` is +16).
3. `s4gp` is byte-identical, so do not spend time on the state-4 `setCollide`
   receiver alone.
4. The next pass should look for a size-neutral state-4/5 body-order change or
   a genuine +8 statement in the mission-0 branch, then re-check
   `byte_exact` (not just `linked_exact`) before landing.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8306/score.py     # 57.4, 57232/57232, byte_exact 4507
python3 /dev/shm/w8306/db.py        # diffbytes 55151
python3 /dev/shm/w8306/regdiff.py 120 220   # state-0 register permutation
python3 /dev/shm/w8306/calls.py /dev/shm/w8306/orig_full.txt 0x138d2a 0x138fd8
python3 /dev/shm/w8306/v8.py m4bc m4bc_rgd  # state-4 move experiments
```
