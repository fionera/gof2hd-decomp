# Small tail wave 161 (w10619): five-function full-stream diff, no metric win

Status: **note only, no `src/` change**.  Worktree `wt/w10619`, base master
`08af4633b` (wave 159).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10619`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, avg 99.93.  All five bodies stay
size-exact.  File size 2,187,328 B unchanged.

Metrics: raw same-address diff bytes (`/dev/shm/w10619/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10619/align_diff.py`).  A new
side-by-side full-stream tool `/dev/shm/w10619/sbs.py` was used to inspect
every differing instruction (not just the category summary).

| fn | raw | aligned | size | first real difference |
|---|---:|---:|---:|---|
| `Generator::createMission` | 162 | 64 | 1996 | bonus block: `volatile rem` prevents the `bonus = rawBonus - rem` fold |
| `Gun::calcCharacterCollision` | 463 | 57 | 1984 | `i*3` in r5 (orig r4); sret/delta pair reversed |
| `SpaceLounge::drawLounge` | 175 | 41 | 1948 | `pad` in fp (orig r9), `x` in r9 (orig r5) |
| `MenuTouchWindow::OnTouchBegin` | 29 | 15 | 1896 | case-8 `listEntryWidth` in r4 (orig ip) |
| `ModStation::ModStation` C1 | 43 | 12 | 1008 | yaw-table literal pool order + PostRA schedule |

## 1. ModStation — pool order is flippable, schedule is not

The first difference is `ldr r4,=Yaw` / `ldr r3,=YawIPad` (ours) vs
`ldr r3,=YawIPad` / `ldr r4,=Yaw` (original).  The literal-pool order is
`[Pitch, Yaw, YawIPad]` in ours and `[Pitch, YawIPad, Yaw]` in the
original.  The pool order can be flipped with

```cpp
const float *yawTable = gHangarCamYaw;
if (Globals::iPad) yawTable = gHangarCamYawIPad;
```

This gives pool `[Pitch, YawIPad, Yaw]` and makes the middle of the block
(`cmp; mov; str; str; it; moveq`) instruction-identical to the original,
but the register pair is swapped (r4=YawIPad, r3=Yaw) and the raw/aligned
counts stay 43/12.  The remaining schedule deltas are the `add r3,pc`
placement (orig after `add r0,pc`; ours after `ldr r2`), and the tail
`ldr r3; movs r0,#2; str.w r8; movs r1,#0` vs
`str.w r8; mov r4,r2; ldr r3; movs r0,#2`.  A small clang-7 probe confirms
`if (c) t = B;` with `t = A` emits the conditional literal first, so the
pool order is a genuine source-shape lever, not a link/rodata artefact.

Variants measured this wave (all 1008 B, all reverted): base 43/12;
`if (iPad) yawTable = YawIPad` 43/12; `if (!iPad) yawTable = Yaw` 43/12;
`yawTable` declared before `pitchTable` 46/13; yaw load before pitch load
1000 B; pitch/yaw declaration swap 43/12.

## 2. Generator — exact bonus block still costs +4 B

Confirmed the wave-159 shape

```cpp
int remv = rawBonus % 50;
volatile int rem = remv;
bonus = rawBonus + remv;
int rv = rem;
if (bonus % 50 != 0) bonus = rawBonus - rv;
```

is **86 / 31 / 2000**.  The block is instruction-identical to the original
except the spill slot (`[sp,#44]` vs `[sp,#32]`).  The +4 B is one extra
EH landing-pad handler (`add r0,sp,#68` at `0x97938`, absent in the
original) plus the dead `b.n` stub at `0x97932`.  A non-volatile
`int rem; ... bonus = rawBonus - rem;` reproduces the algebraic fold
(`subne r5,r6,r0`) but keeps `rem` in r9 and needs `subne.w`, again
2000 B (177/116).  Moving the `float rem = reward % 50` block before the
bonus block is worse (215/48/2000).  No 1996 B shape found.

## 3. Gun / SpaceLounge / MenuTouchWindow

* **Gun**: full stream confirms the known Greedy-RA tie.  `i*3` is r5
  (orig r4); the getPosition sret and the subtraction temp swap the same
  pair.  HEAD (named `playerPosition`) remains the best pair (463/57).
* **SpaceLounge**: first difference is `pad` (`ldr.w fp,[r6,#148]` vs
  `r9`), then `x` (`vmov r9,s18` vs `r5`).  The wave-160 `boxW` lever
  flips the pair but grows the frame (1952 B).  No new size-exact shape.
* **MenuTouchWindow**: first difference is case-8 `listEntryWidth`
  (`ldr.w ip,[sl,#340]` vs `r4`), a pure RA tie; the case-15 block then
  rotates r3/r6/r2/r5 to r2/r3/r6/r4.  `Layout *layout` instead of the
  reference is identical (29/15).  Local `bufs`/`idx` temporaries shrink
  the body to 1892/1884, so they cannot be landed.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10619
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w10619/sbs.py <mangled>                # full instruction stream
python3 /dev/shm/w10619/fdiff.py <mangled>
python3 /dev/shm/w10619/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10619/`: `fdiff.py`, `align_diff.py`,
`sbs.py`, `sweepmod.py`, `sweepgen.py`, `sweepsl.py`, `sweepmtw.py`,
`dumpmod.py`, `t.c`, `*.base.cpp`.

## 5. Files changed

* `_work/reconstructions/small-tail-wave161-w10619.md` (this note).
  No `src/` change; all five bodies stay size-exact at their wave-159
  values (1996 / 1984 / 1948 / 1896 / 1008).
