# Near-miss trio wave 145 (w8918): MGame / MenuTouchWindow / ModStation —
# greedy-RA base colour, full 16-way layout sweep, and the pitch-copy position

Status: **no landed `src/` change.** Worktree `wt/w8918`, base `f5093474f`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave144-w8918.md`, `near-miss-trio-wave143-w8815.md`,
`near-miss-trio-wave142-w8716.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (anchor CSE at the ccm/pt loads) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 4 clusters (2 + 26 B) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (pitch/yaw copy placement) |

## 1. MGame::OnUpdate — MIR proof that the r9 base is a greedy-RA artifact

New this wave: the pre-RA / post-RA MIR of the **no-anchor** shape was read
end to end (`-mllvm -print-machineinstrs`).  The base `&Globals::status`
is one virtual value `%2932`; the RA assigns it `$r5` in the condition and
inserts `$r9 = COPY $r5` at the body entry (`mov r9,r5`).  The original has
`$r9` from the first load and **no copy**.  The greedy allocator picks `r5`
because it is the lowest free register at the interval start; the body needs
`r5` for the `GameText` pointer, so it splits and re-colours the body to
`r9`.  The original's allocator did not split.  Any source shape that keeps
`r9` must therefore either (a) occupy `r5` at the condition start, or
(b) carry a value live across the condition — and (b) is exactly the anchor
whose CSE is the 41-byte residual.

New probes (all reverted; all measured with the other two TUs pristine):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Status *st` at pt) | **41** | 11440 | |
| `Status **sp = &Globals::status` before outer `if`, all m23 uses `(*sp)->` | 5148 | 11392 | RA rematerialises `sp` |
| same, before outer `if`, only field+tail | 5148 | 11392 | |
| `Status *&st = Globals::status` before outer `if`, all uses `st->` | 5148 | 11392 | reference to the global pointer |
| `Status *const &st` / `auto &st` | 5148 | 11392 | |
| `Status *volatile &st` before outer `if` | 5148 | 11392 | volatile qualifier dropped |
| `Status *volatile *sp` / `volatile Status **sp` | 5148 / n/a | 11392 | |
| `*(Status *volatile *)&Globals::status` for all four, no anchor | 5148 | 11392 | four fresh 2-byte loads, base still `r5` |
| same + anchor used at body tail | 6260 | 11528 | body re-rolls |
| same + anchor used at field | 6229 | 11524 | |
| same + anchor used at ccm | 41 | 11440 | identical to HEAD |
| `Status *st = *(Status *volatile *)&Globals::status` (volatile anchor), used at pt | 41 | 11440 | volatile is CSE'd away |
| same, used at ccm | 41 | 11440 | |
| `PlayerEgo *player = this->player` local before the anchor | 861 | 11440 | re-colours `sl` |
| `LevelScript *ls = this->levelScript` local before the anchor | 41 | 11440 | no effect |
| anchor used at ccm instead of pt | 41 | 11440 | confirms the CSE |

Conclusion: the 41 bytes are the greedy allocator's base colour, not a
source-order difference.  Every spelling that produces four fresh loads
also produces the 2-byte `r5` base; every spelling that produces the `r9`
base also produces the anchor CSE.  Irreducible with this compiler.

## 2. MenuTouchWindow::OnTouchBegin — full 16-way A/B/C/D sweep

The case-15 block has four `Globals::layout` accesses (A = first
`buttonInsetX`, B = `leftMargin`, C = `top`, D = third `buttonInsetX`).
All 16 ref/direct combinations were rebuilt on this base (the wave-143
table was incomplete).  `0 = direct`, `1 = reference`:

| A | B | C | D | raw | size |
|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 358 | 1892 |
| 0 | 0 | 0 | 1 | 314 | 1888 |
| 0 | 0 | 1 | 0 | 29 | 1896 |
| 0 | 0 | 1 | 1 | 288 | 1892 |
| 0 | 1 | 0 | 0 | **28** | 1896 |
| 0 | 1 | 0 | 1 | 287 | 1892 |
| 0 | 1 | 1 | 0 | 304 | 1892 |
| 0 | 1 | 1 | 1 | 308 | 1888 |
| 1 | * | * | * | same as A=0 | |

So A is irrelevant (the binding is eliminated when it is only used at A),
and **B must be the reference** to keep the 1896 colouring.  The original
disassembly has B *direct* (`ldr r2,[sp,#16]; ldr r2,[r2,#0]`), yet keeps
the reference colouring (`r4 = &Globals::layout`, `fp = Canvas`).  The
residual is precisely the cached `Layout*` in `r6` across the first
`GetImage2DWidth` call.

New probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Layout &layout`, B=ref, C=dir) | **28** | 1896 | |
| `Layout *&layout = Globals::layout` (A/B/C ref) | 358 | 1892 | all-direct colouring |
| same, A direct / B ref | 358 | 1892 | |
| same, A/B/C ref / D direct | 358 | 1892 | |
| `Layout *volatile &layout` (all) | 126 | 1896 | volatile reloads, re-colours |
| normal ref A + `Layout *volatile &layoutV` B | 127 | 1896 | |
| normal ref A + volatile V for A+B | 126 | 1896 | |
| `volatile Layout &layout` | 366 | 1892 | |
| `Layout *volatile lp = Globals::layout` | 1730 | 1924 | stack traffic |
| case-1/2 y-sum reorder (`top+scroll+left`, `left+scroll+top`, `scroll+top+left`, `scroll+(top+left)`, `left+(top+scroll)`) | 28 | 1896 | `ip`/`r4` cluster unchanged |

The 2-byte `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` cluster is a pure
RA choice and is insensitive to every sum reordering and local tried.  It
is *caused by* the same colouring difference: in the original `r4` is still
live at `0x129fd2` (holding `&Globals::layout` from the case-1/2 block, used
again at the case-15 A access), so `scrollOffset` is forced into the scratch
`ip`; in ours `r4` is dead there (the case-15 A access rematerialises the
address into `fp`), so the allocator uses `r4`.  Fixing the 26-byte cluster
would also fix this 2-byte one.  The 26-byte cluster is the reference's
cached live range.  HEAD stays at 28.

## 3. ModStation ctor — faithful `far = pitch` re-confirmed at 43;
##    HEAD's `far = yaw` is still METRIC-ONLY

HEAD (`far = yaw`) = **11** raw / 1008 B.  Faithful `far = pitch` = **43**
raw / 1008 B.  The 43 bytes are exactly (1) literal-pool order
`Pitch, YawIPad, Yaw` (orig) vs `Pitch, Yaw, YawIPad` (ours) and (2) the
`mov r4,r2` pitch-save position: the RA emits it after the yaw load, then
the post-RA top-down list-latency scheduler hoists it above the yaw load
(ours `d545a`; original `d546e`).

New probes (all reverted; all 43 unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`far = yaw`) | **11** | 1008 |
| `float farPitch = pitch;` after the pitch decl, far uses `farPitch` | 43 | 1008 |
| `float farPitch = pitch;` right before the near call | 43 | 1008 |
| `float nearPitch = pitch;` near uses `nearPitch`, far uses `pitch` | 43 | 1008 |
| `float nearPitch = pitch;` near uses `nearPitch`, far uses `yaw` | 11 | 1008 |
| `int farPitchBits = pitchBits;` before near call, far bitcasts it | 43 | 1008 |
| `!iPad ? Yaw : YawIPad` spelling | 43 | 1008 |
| `if (iPad) yawTable = YawIPad;` instead of the ternary | 43 | 1008 |
| swap `pitchTable`/`yawTable` declaration order | 46 | 1008 |

The literal-pool order does not follow the ternary spelling (the IR order
is `Pitch, Yaw, YawIPad` in every spelling), and the copy hoist is a
scheduler decision that survives `-mllvm -enable-post-misched=false`
(wave 144).  HEAD stays for the 11-byte metric; the semantic fix is the
one-line `farKey, yaw,` -> `farKey, pitch,`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8918
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8918/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8918/`: `fdiff.py`, `bdiff.py`,
`dd.py`, `cc.sh`, `try_mg.sh` / `try_mtw.sh` / `try_mod.sh`,
`*.cpp.base` (pristine sources), `mg_mir.txt` / `mod_mir.txt` (focused
machine-instr dumps), `sw2_*.cpp` (16-way sweep), `v_*.cpp` / `m*.cpp` /
`n*.cpp` / `q*.cpp` / `r*.cpp` / `u*.cpp` (variant sources).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave145-w8918.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
