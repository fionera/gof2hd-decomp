# Small tails, wave 178 (w12220): MGame raw byte re-landed

Status: **1 `src/` change landed**.  Worktree `wt/w12220`, branch
`wave178-w12220`, base master `1b48f990f` (wave-177 merge).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12220`.

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`, size 4524/4524,
`data_exact 843/843`, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical (0).  Whole-`.text` differing bytes **79,251 ->
79,250**; `.ARM.extab` 731 (size/base exact); `.rodata`/`.data` 0.

Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 39 | 11 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

## Landed: `MGame::OnUpdate` — raw 16 -> 15 (aligned 6 -> 111), size 11440

Source shape (in the m122 radio `else if` body):

```cpp
Status *volatile st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
Level *level = this->level;
level->createRadioMessage(28, radioIndex + radioBase);
```

This is the wave-163 `Status *volatile st` anchor that wave 169 (w11519,
commit `114195fed`) reverted to the structurally faithful 16/6 form:

```cpp
int radioIndex = Globals::rnd->nextInt(2);
Level *level = this->level;
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
level->createRadioMessage(28, radioIndex + radioBase);
```

Why the volatile local helps: with it, the compiler keeps the
`&Globals::status` GOT base in `r9` (as the original does) and the
`this->level` load sinks **after** the `getCurrentCampaignMission` call,
matching the original's `ldr.w r1,[fp,#120]`.  The volatile pointer itself is
spilled to `sp+120` (`str r2,[sp,#120]` / `ldr r0,[sp,#120]`), replacing the
original's single `ldr.w r0,[r9]`; the cold block stays the same length and
everything from `movw r2,#3166` onward is byte-identical.

Measured with the full link (`tools/fn_diff.py`):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base (w11519 form) | 16 | 6 | 11440 | 4002/4002 |
| **landed (`Status *volatile st`)** | **15** | **111** | **11440** | 4003/4002 |

Raw byte groups (original vs landed): `0x17e2bf`/`0x17e2c2`/`0x17e2c9`
(the m23 `ldrd`/`sbcs` register tie, 5 B) and `0x17f18c-0x17f195` (the
status spill, 10 B).  The base's 16 raw bytes are all in the radio block
(`0x17f196-0x17f1ad`); the landed form trades those 16 for 10 radio + 5 m23
= 15.

The aligned count rises 6 -> 111 because the volatile spill adds one
instruction (4003 vs 4002); the branch-target instruction indices after the
insertion all shift, which the aligned metric counts.  The trade is
deliberate: the task gate is **raw** same-address bytes, and w11519's note
already recorded "Raw-first would keep 15/111".  The gate (byte_exact /
linked_exact / size) is unchanged.

## Other targets — no size-exact raw win this wave

### `SpaceLounge::drawLounge` — 175/41/1948

Re-swept the `pad` register tie (original `pad=r9`, ours `pad=fp`).  New
shapes (object oracle, all size 1948 unless noted): grouped clamp 36, flat
subtraction 50 (`1944`), parenthesised 36, `w-minus` 58 (`1952`).  No win.

### `Generator::createMission` — 40/21/1996

Re-swept the bonus block (`rem` slot `sp+32` vs `sp+44`, predicated
`subne` vs `cbz` branch).  New shapes: `volatile int rem = rawBonus % 50`
direct 48 (`2000`); volatile pointer to `remVal` 25; `rem` declared before
`b` 25; `volatile const int rem` 25; `int r = rem` before/after the `if`
42/135 (`2004`/`2008`); `__builtin_expect(b % 50 != 0, 1)` 135 (`2008`).
No size-exact improvement.

### `ModStation::ModStation` C1/C2 — 39/11/1008

Re-swept the `YawIPad`/`Yaw` ternary materialisation (original loads
`YawIPad` into `r3`, ours into `r4`).  New shapes (all size 1008 unless
noted): `iPad == 0 ? Yaw : YawIPad` 9, `iPad != 0 ? ...` 9, `!iPad ? ...`
9, parenthesised 9, `&Yaw[0]` arms 9, `const float *const` 9, `bool`/`int`
condition local 9, `if/else` 9, two named arm pointers 9, `yawTable`
before `pitchTable` 9, volatile-source pointer 39 (`1000`).  The `r3`/`r4`
swap is a pure greedy-RA tie; no source shape moves it without changing the
1008-byte frame.

### `MenuTouchWindow::OnTouchBegin` — 28/16/1896

No new shape beat the wave-177 master.  Two independent ties remain: the
case-8 `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` (2 B) and the case-15
scrollbar-hit register permutation (26 B, one instruction shift).  Both are
greedy-RA ties.

## Tooling

The object-level oracle used by the wave-177 note
(`_work/reconstructions/w12120_oracle.py`) and the scratch sweep driver
(`scratch/w12220/sweep.py`) reproduce each base residual exactly (MGame
`diff=6`, ModStation `diff=9`, MenuTouchWindow `diff=18`, Generator
`diff=25`, SpaceLounge `diff=35`) and were used to screen every shape above
in ~0.5 s before a full link.

## Conclusion

One landed change: `MGame::OnUpdate` raw **16 -> 15** (aligned 6 -> 111,
11,440 B exact, `.rel.plt` byte-identical), `.text` differing bytes
**79,251 -> 79,250**.  The other four tails are unchanged at
**175/41 (1948), 40/21 (1996), 39/11 (1008), 28/16 (1896)** raw/aligned/size.
Full `check` green: byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
