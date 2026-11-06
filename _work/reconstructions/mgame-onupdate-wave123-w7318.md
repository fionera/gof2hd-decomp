# MGame::OnUpdate — mission-23 idle-contact `Status*` residual, wave 123 / w7318

Status: **research pass, no `src/` change landed.** Worktree `wt/w7318`, base =
master `1c8c9b20e` (wave-122 final snapshot). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source (the `Status *st` anchor at the mission-23 idle
check): **11440 B, 41 diff bytes, one run, 99.935275 %,
3862/3863 normalised instructions**. This note extends
`mgame-onupdate-wave119-w6956.md` / `...-w6938.md` / `...-w6911.md` with the
wave-123 `PlayerEgo *p` anchor sweep and a further batch of measured shapes.
Nothing beats 41 bytes.

## 1. Baseline (re-verified this session)

```
orig 11440 0x17c8d8   ours 11440 0x17c8d8
diff bytes 41  runs 1   (function-relative 0x17bb..0x17e3,
                         absolute 0x17e093..0x17e0bb)
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2764-2772`). Original vs ours (`r9` =
`&Globals::status`):

```
original                                   ours (current source)
17e08c ldr.w r9,[r0]                       17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]   ; ccm               17e090 ldr.w r5,[r9]   ; st
17e094 blx  ccm                            17e094 mov  r0,r5
17e098 cmp  r0,#23                         17e096 blx  ccm
17e09a bne  17e176                         17e09a cmp  r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne  17e176
17e0a0 blx  isMining                       17e09e ldr.w r0,[sl]
17e0a4 cmp  r0,#0                          17e0a2 blx  isMining
17e0a6 bne  17e176                         17e0a6 cmp  r0,#0
17e0a8 ldr.w r0,[r9]   ; mission           17e0a8 bne  17e176
17e0ac blx  getMission                     17e0aa ldr.w r0,[r9]
17e0b0 blx  isEmpty                        17e0ae blx  getMission
17e0b4 cmp  r0,#0                          17e0b2 blx  isEmpty
17e0b6 beq  17e176                         17e0b6 cmp  r0,#0
17e0b8 ldr.w r0,[r9]   ; pt arg             17e0b8 beq  17e176
17e0bc blx  getPlayingTime                 17e0ba mov  r0,r5
17e0c0 ldr.w r2,[r9]   ; field              17e0bc blx  getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

Both blocks are 16 B of status loads + the 4 B `isMining` receiver load;
the run is a register-field permutation plus the induced 2-byte branch
offset shift. From `0x17e176` (mission-24) to the end of the function the
two disassemblies are byte-identical.

## 2. Mechanism (recap from wave 119, re-confirmed)

`RegAllocGreedy::enqueue` (LLVM 7.0.2, `RegAllocGreedy.cpp:673`):
`RS_Split` ranges get `Prio = Size` (dequeued last); global ranges get
`(1u<<31)|(1u<<29)+Size`. `tryAssign` returns the first free register with
`getCostPerUse == 0` (r0–r7) immediately.

The `&Globals::status` GOT base (`%2931`, live `0x17e08c`…wingmen loop) is
an `RS_Split` range (region-split at the m23 body, `bb.242`). The current
source's `Status *st` is a multi-MBB **global** range (`%128`, live
`42032B`…`42608B`, used at ccm and pt) and is therefore dequeued before the
GOT base, takes `r5`, and forces the GOT base onto `r9`. Its load is
machine-CSE'd with the ccm load, producing `ldr r5,[r9] / mov r0,r5` at ccm
and `mov r0,r5` at pt.

Byte budget of the four `Globals::status` value loads:
plain `4+4+4+4 = 16 B`; anchor with `k` uses via `mov`
`4 + 2k + 4(4-k) = 20 - 2k`. Size 11440 therefore requires exactly `k = 2`
cached uses. The anchor is always loaded before the first call after its
declaration, so the ccm load is always one of the two CSE'd uses. The only
byte-neutral pair is **(ccm, pt)** — the current source.

The original has **no** anchor load and **no** split copies at the m23 body
(`0x17e0da` is `movs r0,#1 / strb.w r0,[fp,#93]`). Its GOT base is `r9`
continuously from `0x17e08c` to `0x17f192`, i.e. the pre-body and body pieces
were coalesced onto `r9`, and `Globals::status` is re-loaded from `r9` at
every site. So the original has a live value in `r5` across the condition
that the plain source does not — but it is not a cached `Status*`.

## 3. Wave-123 variants measured (full link)

`cmake --build cmake-build-match --target link_so` +
`/dev/shm/w7318/diffbytes.py`; `size` is `nm`'s function size,
`diff`/`runs` are the linked `.text` byte diff.

| variant | size | diff | runs |
|---|---:|---:|---:|
| **baseline: `st` at ccm + pt** | **11440** | **41** | **1** |
| `PlayerEgo *p = this->player;` used at m23+m24 isMining (`p24`) | 11440 | 250 | 11 |
| `p` assigned at isMining, used m23+m24 (`p24b`/BA) | 11440 | 231 | 10 |
| `PlayerEgo *&p` / `PlayerEgo **pp` used at m23 only | 11392 | 5196 | 212 |
| `p` used at m23 only (decl) | 11396 | 5168 | 242 |
| `p` used at m24 only (`p` sunk) | 11440 | 862 | 37 |
| `p` assigned after isMining, used m23+m24 | 11440 | 831 | 38 |
| `st` at m24 ccm / mission / pt | 11440 | 230 | 9 |
| `st` at m24 field | 11436 | — | — |
| `st` at field only | 11436 | 4997 | 302 |
| `st` at pt+field | 11436 | 4985 | 308 |
| `st` at ccm only / ccm+mission | 11392 / 11396 | 5196 / 5173 | 212 / 240 |
| `st` before the outer `elapsedTime > 5000` if (pt) | 11392 | 8769 | 533 |
| `st` at function top (pt) | 11372 | 10890 | 418 |
| `st` assigned after ccm via comma (pt) | 11392 | 5196 | 212 |
| `st` assigned after ccm via comma (pt+field) | 11392 | 5196 | 212 |
| `st` assigned after isMining via comma (pt) | 11400 | 5171 | 228 |
| `st` at m23 body store | 11520 | 6317 | 323 |
| `st` at body store + pt | 11532 | 6377 | 304 |
| volatile `Globals::status` read at ccm (ccm fresh) | 11440 | 853 | 36 |
| `Status *const st` / `auto *st` / `3600000LL` / `>= 3600001` | 11440 | 41 | 1 |
| `GameText *text` cached for the m23 body | 11176 | 8071 | 363 |
| `AERandom *rnd` cached for the m23 body | 11420 | 5042 | 264 |
| both text+rnd cached | 11304 | 8939 | 428 |
| `LevelScript *ls` cached across m23 | 11476 | 6247 | 316 |
| `bool mining = player->isMining()` hoisted before the if | 11384 | 5156 | 250 |
| `int ccm = status->getCurrentCampaignMission()` hoisted | 11392 | 5196 | 212 |
| `bool contact = <whole condition>; if (contact)` | 11392 | 5196 | 212 |
| `st` + second `Status *st2` at m24 ccm | 11436 | 4822 | 305 |
| `st` + `PlayerEgo *p` at m24 isMining | 11516 | 5290 | 285 |

## 4. The `p24` finding

The most interesting non-baseline shape is `p24`: cache
`PlayerEgo *p = this->player;` before the m23 `if` and use `p->isMining()`
in **both** the m23 and m24 arms. The m23 condition then becomes
instruction-identical to the original *except* the `isMining` receiver:

```
17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]     ; ccm        <- matches original
17e094 blx  ccm
17e098 cmp  r0,#23
17e09a bne
17e09c mov  r0,r5        ; isMining   <- original: ldr.w r0,[sl]
17e0a0 blx  isMining
...
17e0a8 ldr.w r0,[r9]     ; mission    <- matches
17e0b8 ldr.w r0,[r9]     ; pt         <- matches
17e0c0 ldr.w r2,[r9]     ; field      <- matches
```

i.e. the four fresh `Globals::status` loads and the `r9` GOT base are all
reproduced. But the `p` local costs an extra `ldr.w r5,[sl]` and turns the
m23 `isMining` receiver into `mov r0,r5` (+2 B), while the m24 arm saves
2 B (`mov r0,r5` instead of `ldr.w r0,[sl]`). The size stays 11440, but the
+2-byte shift propagates through the m23 body and m24 up to the m24
`isMining`, giving **231 diff bytes / 10 runs (99.7 %)** — a different,
larger residual than the baseline's 41 B. Declaring `p` before the ccm
(`p24`) gives 250 B / 11 runs; assigning it after the ccm or after
`isMining` does not help (231 / 831 B).

## 5. Conclusion

The 41-byte / one-run residual is a hard local optimum for every anchor and
plain spelling tried across waves 97/105/106/107/108/112/119/123 (now 180+
measured shapes). The original's shape — four fresh `Globals::status` loads
from a `r9` GOT base, no `st` materialisation, no body split copies —
requires a live `r5` value across the condition that is *not* a cached
`Status*`. No MGame.cpp-only spelling produces it: every spelling that keeps
the 11440-byte budget lands on the identical 41-byte shape, and every
spelling that removes the `st` load loses the `r9` GOT base and recolours
the whole function. The only untested lever remains the cross-TU one from
wave 112 §5 (make the `&Globals::status` GOT load non-rematerialisable via
`src/game/core/Globals.h`), which is out of scope for this TU-only pass.

## 6. Gate (pristine source restored)

```
cmake --build cmake-build-match --target check -j6
  verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
  RATCHET PASS      (goal_lint CLEAN)
```

Before -> after: byte 4506 -> 4506, linked 4506 -> 4506. Target unchanged at
11440 B / 41 diff bytes / 99.935275 %.

## 7. Commands used

```bash
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7318/diffbytes.py
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed): `/dev/shm/w7318/` — `MGame.pristine.cpp` (pristine
TU), `harness2.py` / `vtest.py` (object-level variant harness),
`measure_full.py` (full-link variant runner), `spec*.json` (the variant
sweeps above), `orig_onupdate.txt` / `plain_onupdate.txt` / `greedy_onupdate.mir`
/ `plain_greedy.mir` (disassembly and 7.0.2 MIR).
