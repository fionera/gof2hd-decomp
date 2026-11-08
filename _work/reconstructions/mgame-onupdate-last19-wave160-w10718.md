# MGame::OnUpdate — the last 19, wave 160 / w10718

Status: **one landed `src/` change** (`src/game/menu/MGame.cpp`).  Worktree
`wt/w10718`, base master `01c7f92d6` (wave 160 docs; gate byte_exact
4507/4524, raw 318 / aligned 19 / 11,440 B at OnUpdate).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate after the change (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
parity 5370/5370, avg 99.93.  `MGame::OnUpdate` stays size-exact at **11,440 B**.

Metrics: raw same-address diff bytes (`/dev/shm/w10718/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10718/align_diff.py`, path-adjusted
copy of the committed tool).

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD master (w10518: m122 `radioBase` via `Globals::status`, store via `st`) | 318 | **19** | 11,440 |
| **LANDED: m122 `radioBase` via `st` (pristine w149 shape)** | **37** | 109 | 11,440 |
| w10618 (outer `st` before the m23 `if`, used at the m23 `getPlayingTime`; m122 direct) | 41 | 346 | 11,440 |
| comma-assign `st` at the m23 `getMission`, used at the m23 pt (m23 ccm reloads) | 5,148 | 599 | 11,396 |
| `st` before m23 if, used at m23 `getMission` + pt | 4,982 | 99 | 11,436 |
| `st` before m23 if, used at m23 `getMission` + field | 4,966 | 59 | 11,436 |
| `Status *&st` before m23 if, used at m23 ccm + pt | 5,148 | 597 | 11,392 |
| `Status *volatile st` before m23 if, used at m23 pt | 6,076 | 955 | 11,340 |

## What was learned

* The **w10618 merge** on top of master's w10518 shape is the outer-`st`
  shape: raw **41 / aligned 346 / 11,440 B**.  It removes the m122 field
  `r5`/`r2` and store `mov`, but adds two `mov r0,r5` in the m23 block
  (`17e094` and `17e0ba`) where the original reloads `ldr.w r0,[r9]`.
  The aligned 346 is the early 2-byte shift (the greedy alignment inflates
  it); the raw 41 is the honest number.
* The **pristine w149 shape** (m122 `radioBase` via the live `st`) is
  raw **37 / aligned 109 / 11,440 B**.  It dominates w10618 on *both*
  metrics (37 < 41, 109 < 346) and keeps the size exact.  The landed change
  is exactly this one-line revert of w10518:
  `Globals::status->getCurrentCampaignMission()` -> `st->getCurrentCampaignMission()`
  in the m122 `radioBase` initialiser.
* Moving `st` into the m23 block only (the w10618 probe) cannot remove the
  m23 `mov`s: the first use of the snapshot always CSEs the preceding
  `Globals::status` load into a `mov r0,r5`.  Defining `st` *after* the m23
  ccm (comma assignment at `getMission`) does make the ccm reload, but the
  allocator then splits the GOT base elsewhere and the body shrinks to
  11,396 B (size-fatal).
* `Status *&st` is transparent to the allocator (no live value in r5), so the
  GOT base splits to r5 and the body is 11,392 B.  `Status *volatile st`
  forces stack reloads and is much worse (11,340 B).
* A shape that keeps the GOT base in r9 with **no** live `Status*` in r5,
  and therefore emits the original's `ldr.w r0,[r9]` at every site, was not
  found.  Every r9-forcing shape either keeps a live `Status*` (one `mov` at
  a receiver) or is size-fatal.

## Trade-off vs master

The landed shape trades the aligned metric (19 -> 109) for a large raw win
(318 -> 37, i.e. `.text` differing bytes 85,518 -> 85,237).  The raw metric
is the ground truth for byte-identicality; the aligned rise is the short
m122 `mov`-induced block shift that greedy alignment penalises.  If the
coordinator prefers the aligned 19, master's w10518 line should be kept
instead; the two shapes are mutually exclusive one-line variants of the same
`radioBase` initialiser.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10718
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10718/fdiff.py _ZN5MGame8OnUpdateEv
python3 /dev/shm/w10718/align_diff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w10718/`: `fdiff.py`, `align_diff.py`,
`measure.sh`, `sweep*.py`, `MGame.base.cpp`, `plain_all.log`,
`onupdate_vrr.mir`.

## Files changed

* `src/game/menu/MGame.cpp` — m122 `radioBase` reads `st->getCurrentCampaignMission()`
  instead of `Globals::status->...`.  Size 11,440 unchanged; raw 318 -> 37,
  aligned 19 -> 109.
* `_work/reconstructions/mgame-onupdate-last19-wave160-w10718.md` (this note).
