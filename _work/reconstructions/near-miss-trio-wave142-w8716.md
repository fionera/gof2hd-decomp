# Near-miss trio wave 142 (w8716): MGame / MenuTouchWindow / ModStation —
# new hypotheses on the anchor, the layout reference and the far x-arg

Status: **no landed `src/` change.** Worktree `wt/w8716`, base `1d10d0694`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave141-w8618.md`, `near-miss-trio-wave140-w8518.md`,
`near-miss-trio-wave139-w8418.md`, `mgame-onupdate-wave129-w7661.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (2-byte shift at the ccm anchor) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 4 clusters (2 + 26 B) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (scheduler) |

## 1. MGame::OnUpdate — the anchor is load-bearing for `r9`, and the
##    no-anchor shape rematerialises the base inside the m23 body

New this wave: the *no-anchor* build was disassembled end-to-end.  It is NOT
"r5 instead of r9 everywhere"; the base is still `r9` at the m24 ccm
(`17e176 ldr.w r0,[r9]`) and at the m23-body tail
(`17e26c ldr.w r0,[r9]`).  The shrink is entirely in the **m23 condition**,
where the compiler rematerialises `&Globals::status` into `r5` and uses the
2-byte `ldr r0,[r5]` encoding:

```
no-anchor 17e08c  ldr r5,[r0]      ; r5 = &Globals::status (GOT)
          17e08e  ldr r0,[r5]      ; ccm   (2 B)
          17e0a4  ldr r0,[r5]      ; mission (2 B)
          17e0b2  ldr r0,[r5]      ; pt    (2 B)
          17e0b8  ldr r2,[r5]      ; field (2 B)
orig      17e08c  ldr.w r9,[r0]
          17e090  ldr.w r0,[r9]    ; ccm   (4 B)
          17e0a8  ldr.w r0,[r9]    ; mission (4 B)
          17e0b8  ldr.w r0,[r9]    ; pt    (4 B)
          17e0c0  ldr.w r2,[r9]    ; field (4 B)
```

The base is kept in `r9` in the original because its live range spans the
whole m23 body (the tail `Globals::status->getPlayingTime()` at `17e26c`),
and the m23 body uses `r5` for the `GameText` pointer (`17e12a`).  In the
no-anchor build the allocator rematerialises the base at the m23 tail instead
of keeping it live, so `r5` is free for the base in the condition and the
4 fresh loads become 2-byte.  The `Status *st` anchor reproduces the original
`r9` colour **only** because it is CSE'd with the ccm load; that CSE is the
41-byte residual.

New probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`st` at pt) | **41** | 11440 | |
| no anchor | 5148 | 11392 | base rematerialises to r5 in the condition |
| `st` at ccm + m23 tail | 6230 | 11524 | anchor live across m23 body; body re-rolls |
| `st` at pt + m23 tail | 6234 | 11520 | same |
| comma `(st = Globals::status, ...)` before pt, used at pt+tail | 6242 | 11556 | anchor not live across isMining |
| `st` at ccm + tail (Ghidra-faithful tail) | 6230 | 11524 | |

Conclusion unchanged but now explained: the 41 bytes are the price of the
`ldr r5,[r9]; mov r0,r5` CSE at the ccm, compensated by `mov r0,r5` at pt.
Every source shape that removes the CSE either re-colours the base to `r5`
(no anchor) or extends the anchor's live range into the m23 body and re-rolls
the body (+84 B).  The two goals are mutually exclusive in this compiler.

## 2. MenuTouchWindow::OnTouchBegin — the original has *no* live layout value

Disassembling the original's case-15 block confirms both the first and second
comparisons read `Globals::layout` **fresh** from `[sp,#16]` (the cached
`&Globals::layout` address), not from a live register:

```
12a1f0  ldr r2,[r4,#0]    ; Globals::layout  (1st compare)
12a1fc  ldr r2,[r2,#40]   ; buttonInsetX
12a212  ldr r2,[sp,#16]   ; &Globals::layout  (2nd compare)
12a218  ldr r2,[r2,#0]    ; Globals::layout
12a21e  ldr r5,[r2,#12]   ; leftMargin
12a220  ldr r6,[r2,#32]   ; top
```

HEAD keeps the `Layout &layout` reference value live in `r6` across the first
`GetImage2DWidth` call, so `leftMargin` comes from `r6` and only `top` is
fresh.  New probes this wave (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD | **28** | 1896 | |
| `Layout *&layout = Globals::layout` + `layout->` (all) | 358 | 1892 | reference-to-pointer; all fresh, size -4 |
| `Layout *&layout` + `Globals::layout->` for buttonInsetX | 358 | 1892 | |
| `Layout &layout` + reference for 3rd compare, both 2nd direct | 314 | 1888 | |
| `Layout *volatile lp` + `lp->` (all) | 1732 | 1916 | volatile pointer reloads, +20 B |
| `Layout *volatile lp` + direct 3rd compare | 1735 | 1924 | |
| outer `layout->` (no inner reference) | 1762 | 1908 | +12 B |

The 1892 all-direct shape is the closest *semantically* (both fresh) but is
4 B short: the inner reference is what forces `[sp,#16]` to hold
`&Globals::layout` and keeps the 1896 size.  Any spelling that removes the
live reference value also removes those 4 bytes.  The 26-byte cluster is
therefore the reference's live-range, not a simple access-path choice.

## 3. ModStation ctor — HEAD's `far = yaw` is a lucky 11; faithful `far = pitch` is 48

Re-measured on this base: HEAD (`far = yaw`) = **11** raw / 1008 B.
Faithful `far = pitch` = **48** raw / 1008 B (wave-141 reported 43 on its
base).  The far=pitch diff is three things:

1. literal-pool order: original `Pitch, Yaw, YawIPad`; ours
   `Pitch, YawIPad, Yaw` (the `ldr r3,[pc]`/`ldr r4,[pc]` pair swaps and the
   pool entries at `d56b4`/`d56b8` swap);
2. `add r3,pc` moves after the pitch load;
3. the RA copy `mov r4,r2` (save pitch) is hoisted above the yaw load
   (ours `d545a`; original `d546e`), and `str.w r8,[sp,#184]` moves with it.

New probes (all reverted, all 48 unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`far = yaw`) | **11** | 1008 |
| `far = pitch` | 48 | 1008 |
| `far = pitch`, `!iPad ? Yaw : YawIPad` (all three spellings) | 48 | 1008 |
| `far = pitch`, `const float` locals | 48 | 1008 |
| `far = pitch`, declaration swap | 744 | 1000 |
| `far = pitch`, `volatile float` loads | 746 | 1016 |

The ternary spelling does not change the LDRLIT order (the compiler emits
`YawIPad` first regardless).  HEAD stays for the raw metric; the semantic
fix (`farKey, yaw` -> `farKey, pitch`) is a one-line change if the project
prefers correctness over the 11-byte count.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8716
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8716/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8716/`: `fdiff.py`, `dd.py` (orig/ours
disasm), `idiff.py` (instruction-sequence diff), `mgvar.py` / `mgvar3.py`,
`mtwvar.py` / `mtwvar2.py` / `mtwvar3.py`, `modvar.py` (variant drivers),
`build.sh` (incremental link), `c1_sched.txt` / `mod_pa.txt`
(`-mllvm -print-after-all` MIR dumps).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave142-w8716.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
