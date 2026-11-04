# MGame::OnUpdate — idle-contact `Status*` r5/r9 residual (wave 106, w5408)

Status: research pass, 2026-11-04. Worktree `wt/w5408`, branch `wave106-w5408`,
base `486f8a119`. **No source change landed**: the function is already at its best
measured shape (41 diff bytes, size exact, 99.9 %). This note extends the wave-105
analysis (`mgame-onupdate-wave105-w5312.md`) with a much larger source-shape sweep
and a precise statement of *why* the residual cannot be closed from C++ source
without changing the emitted code.

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8 (offset 0 = 0x17c8d8).

## 1. Exact remaining diff (baseline)

`python3 /dev/shm/w5408/fdiff.py _ZN5MGame8OnUpdateEv` on the pristine source:

```
size 11440 11440
diffbytes 41 runs 2
0x17ba 0x17ba 1
0x17bc 0x17e3 40
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2770-2776`):

```
original                                    ours (current source)
17e08c ldr.w r9,[r0]   ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                       17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission       17e094 mov   r0,r5
17e098 cmp   r0,#23                        17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                        17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx   isMining                      17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                         17e0a2 blx   isMining
17e0a6 bne   17e176                        17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                       17e0a8 bne   17e176
17e0ac blx   getMission                    17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                       17e0ae blx   getMission
17e0b4 cmp   r0,#0                         17e0b2 blx   isEmpty
17e0b6 beq   17e176                        17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                       17e0b8 beq   17e176
17e0bc blx   getPlayingTime                 17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                       17e0bc blx   getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

The original has **four fresh `ldr.w rX,[r9]` loads and no cached `Status*`**.
Both blocks are 52 bytes; ours has one extra (2-byte) instruction and one fewer
(4-byte) load, so the *byte* size is identical while the *instruction slot*
count differs by one. The whole residual is therefore a pure greedy-RA colour
choice plus the 2-byte shift it induces.

## 2. Source witness: the original is the no-`st` shape

The Unity-remake decompilation of the same routine
(`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/MGame.c:4906`)
renders the whole idle block with direct globals:

```c
iVar4 = Status::getCurrentCampaignMission(Globals::status);
if ((iVar4 == 0x17) && (iVar4 = PlayerEgo::isMining(...), iVar4 == 0)) {
  pMVar13 = (Mission *)Status::getMission(Globals::status);
  iVar4 = Mission::isEmpty(pMVar13);
  ...
  lVar39 = Status::getPlayingTime(Globals::status);
  ... *(longlong *)(Globals::status + 0x100) ...
```

so the original C++ is `Globals::status->...` four times, i.e. the IR is the
no-`st` shape (`%1091/%1098/%1102/%1104` distinct loads). The `st` local in our
tree is a **size/RA anchor, not a source witness**.

## 3. Machine-IR / RA evidence

`llc -debug-only=regalloc` on both builds (see `/dev/shm/w5408/base_ra.txt`,
`nost_ra.txt`; machine-IR dumps `base_mir2.txt`, `nost_mir2.txt`):

* baseline: `%1053` (`&Globals::status`, the GOT base) is assigned **r9** and
  stays whole; `%128` (`st`) is assigned **r5** and is live `[42032,42608)`.
* no-`st`: `%1052` (GOT base) is assigned r9, then **evicted** by `%2928`
  (the `gWingmenRadioMissions` `tLDRLIT_ga_pcrel`) at slot 50280, re-split;
  the later part `%2939` gets r9 and the main part `%2932` gets **r5**
  (`hints: $r9` is missed). The body then starts with a real `mov r9,r5`.

The deciding number is the region-split static cost
(`calculateRegionSplitCost` → `addSplitConstraints`):

| build | `$r5 static` | `$r9 static` | chosen |
|---|---:|---:|---|
| baseline (`st`) | 0.001645315469 | 0.0008723807036 | **r9** |
| no-`st` | 0.0004561142685 | 0.0008723807036 | **r5** |

`$r9`'s cost is *identical* in both builds. The only difference is `$r5`: the
`st` value occupies r5 inside the compact region, so assigning the GOT base to
r5 would have to evict it. **The anchor works by making r5 expensive, not by
changing r9.**

The machine-IR diff between the two builds is more than the m23 load: with
`st`, the GOT base stays live across the m23 body (reused at slot 52048); without
`st`, it is rematerialised there (`t2LDRi12 %tgpr, 0 :: load 4 from got`). That
live-range difference is what flips the compact-region split.

## 4. Variants measured this pass

All measured against the linked original with `link_so` + `fdiff.py`
(size, diff bytes, runs). `official.py` in `/dev/shm/w5408` automates it.

| variant | size | diff | runs |
|---|---:|---:|---:|
| baseline `st` at getPlayingTime (current) | **11440** | **41** | **2** |
| `Status **sp = &Globals::status; Status *st = *sp;` (ccm via `(*sp)`) | 11440 | 41 | 2 |
| `auto *st` / `Status *const st` / `Status &st` spellings | 11440 | 41 | 2 |
| `3600000LL` / `3600000L` / `(long long)3600000` / `3600LL*1000` | 11440 | 41 | 2 |
| `*(long long *)&...field_0x100` / `*(long long *)((char*)status+0x100)` | 11440 | 41 | 2 |
| `isMining() == false`, `isEmpty() == true`, `(void)st`, `(Status*)(void*)` ccm | 11440 | 41 | 2 |
| no `st` (faithful source shape) | 11392 | 5148 | 208 |
| `st` used at m24 branch (anchor) | 11440 | 230 | 10 |
| `st` used at m122 branch (anchor) | 11440 | 492 | 21 |
| `st` used at alienOrbit branch (anchor) | 11440 | 589 | 22 |
| `st` used at hint `getShip()` (anchor) | 11464 | 5057 | 298 |
| `st` assigned between isMining and getMission (comma), used at m122 | 11440 | 470 | 23 |
| `st` assigned between isEmpty and getPlayingTime (comma), used at m122 | 11440 | 455 | 24 |
| `st` assigned at top of m23 body, used at final store field | 11444 | 4673 | 293 |
| `st` assigned at top of m23 body, used at final store getPlayingTime | 11548 | 6266 | 296 |
| `st` at getPlayingTime + `&& st == Globals::status` | 11444 | 5000 | 307 |
| `st` at getPlayingTime + `&& st != nullptr` | 11444 | 4997 | 305 |
| `st` at getPlayingTime + `&& st == st` | 11392 | 5148 | 208 |
| `__builtin_expect(m23cond, 0/1)` | 11444/11404 | 5116/5152 | 257/238 |
| `__builtin_expect(elapsedTime>5000, 0/1)` | 11324/11344 | 5952/5685 | 294/236 |
| `__builtin_expect(inner if, 0/1)` | 11480/11364 | 6212/9028 | 265/359 |
| volatile ccm read + `st` at m122 | 11440 | 850 | 38 |
| volatile `st` read + `st` at m122 | 11440 | 492 | 21 |
| `st` declared before outer `elapsedTime` if, used at getPlayingTime | 11392 | 8721 | 535 |
| `st` declared before jump-drive if, used at getPlayingTime | 11372 | 6698 | 334 |
| `st` declared before inner if, used at getPlayingTime | 11392 | 8724 | 536 |

### 4.1 Why the anchor always re-colours

Every anchor that keeps the GOT base in r9 *and* leaves the m23 loads fresh
(`st` used at m24/m122/alien) changes the code at the anchor's use site
(`ldr.w r0,[r9]` -> `mov r0,r5`), and every anchor placed *inside* the m23
condition instead CSEs with one of the four loads (whichever is adjacent), so
one of the four stays cached. The best such shape is `st` used at the m24
branch: **11440 B / 230 diff / 10 runs** — the m23 block then has 3 of the 4
loads fresh and only the `ccm` cached, but the m24 branch itself changes.

## 5. What is proven vs hypothesis

Proven:

* The residual is exactly the m23 `Status*` cache decision; everything else in
  the 11440-byte function is byte-identical.
* The original IR is the no-`st` shape (4 distinct loads, GOT base `r9`).
* The `st` local is an RA anchor: it raises the region-split `$r5` static cost
  above `$r9`'s, which is why the baseline keeps the GOT base in r9.
* Dropping `st` gives the faithful shape but re-colours the GOT base to r5 and
  loses ~48 B / 200+ runs.
* CSE-equivalent spellings of `st` (`auto`, `const`, reference, `(Status*)`,
  `&*`, `Status**` + `*sp`) are all byte-identical to the baseline.
* `__builtin_expect` on any of the three enclosing conditions re-colours the
  function and is strictly worse.

Hypothesis (not proven): the original source must contain a code-neutral IR
difference that raises the no-`st` region-split `$r5` cost (or lowers `$r9`'s)
without adding an instruction — e.g. a value that occupies r5 inside the
compact region but is dead at every use. No such C++ construct was found; the
only code-neutral constructs tried (`(void)st`, `st == st`, `sp`/`st` aliasing)
are all optimised away before RA.

## 6. Gate (note-only pass)

`check` green: byte_exact **4500/4524**, linked_exact **4500/4524**,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint **CLEAN**, drift **0**, `RATCHET PASS`.
Before -> after: byte 4500 -> 4500, linked 4500 -> 4500.

## 7. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5408/fdiff.py _ZN5MGame8OnUpdateEv      # size / diffbytes / runs
python3 /dev/shm/w5408/official.py                        # baseline measurement
cmake --build cmake-build-match --target check -j6
```

Scratch drivers in `/dev/shm/w5408`: `fdiff.py`, `dis2.py`, `fmeas.py`,
`fmeas2.py`, `official.py`, `vtest.py`, `vtest2.py`, `base_ra.txt`,
`nost_ra.txt`, `base_mir2.txt`, `nost_mir2.txt`.
