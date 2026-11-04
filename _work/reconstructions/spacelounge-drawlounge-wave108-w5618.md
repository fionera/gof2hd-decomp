# SpaceLounge::drawLounge — wave-108 re-measure and RA-ordering sweep (w5618)

Status: research note (2026-11-04). Worktree `wt/w5618`, branch
`wave108-w5618`, base `8f98ec4d6`. **No `src/` change landed**: every source
shape tested this wave keeps the official score at raw **95.538778** (pct 95.5,
1948 B) or lowers it. The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

Gate (`cmake --build cmake-build-match --target check -j6`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Prior work: `spacelounge-drawlounge-wave107-w5519.md` (newest, landed the
`short pad3` form), `-w5016.md`, `-w4818.md`, `-w4519.md`, `-w4246.md`,
`-w4227.md`, `-w4118.md`, `-w4018.md`, `-w3838.md`, `-w3818.md`,
`SpaceLounge-OnRender3D.md`, `SpaceLounge-layout.md`. This note records the
wave-108 sweep and supersedes nothing.

## 1. Baseline and residual

The committed source (HEAD) already carries the wave-107 `short pad3` clamp
form and scores raw **95.538778** (729/728 instructions, 1948/1948 B). The
complete residual is one contiguous region (pad load register, clamp right arm,
join, post-`drawBox` agent/top restore) plus the tail branch-target labels:

```text
value        original        ours (HEAD)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

Original right arm (2 insns):

```text
itt gt
addgt.w r1, r6, sl     ; width + pad*2
subgt   r5, r0, r1     ; x = (Globals::w - inset) - (width + pad*2)
```

HEAD right arm (3 insns, the `short` sign-extend path):

```text
ittt gt
movgt.w r1, fp, lsl #16
addgt.w r1, r6, r1, asr #15
subgt.w r9, r0, r1
```

## 2. Why the target allocation is not reachable from source (proven)

Instrumented LLVM 7.0.2 (same revision as NDK r18b) RA trace on HEAD
(`RA_TRACE=1`, `/tmp/llvm_android_702b/build2/bin/clang++`):

```text
select %608 x        size=4220 w=2.946139e-04 hint=$r3 -> $r5
select %47  boxW     size=768  w=6.080038e-04 hint=$r3 -> $r6
select %46  top-pad  size=736  w=6.251306e-04 hint=$r3 -> $r5   (evicts x)
select %608 x        size=4220 w=2.946139e-04 hint=$r3 -> $r9   (r9 free -> whole)
select %45  x-pad    size=704  w=6.432504e-04 hint=$r2 -> $r6
select %47  boxW     ...                          -> $r10
select %2   pad      size=6384 w=2.159248e-04 no hint -> split (stage 2 -> fp)
```

The spill weights are `totalWeight / (Size + 25*InstrDist)` (LLVM
`normalizeSpillWeight`), so the long x interval (4220 units) is cheap to evict
even though it has 8 def/use contributions, while the short `top-pad` (736
units) and `x-pad` (704 units) intervals are expensive. `pad` `%2` is a plain
load with **no COPY anywhere**, so it has no physical hint, priority
`0xA00018F0` (global, no hint), and is selected after every `0xE...` hinted
call-argument range.

Knob probes on HEAD (instrumented clang, scratch only):

| knob | result |
|---|---|
| `NOHINT=2147483694` (clear `%46` hint) | `%45` (x-pad) still evicts x from r5; x -> r9 |
| `VRW=2147484256:2.5` (x weight 7.37e-4) | x stays r5, but `%47` (boxW) takes r9; pad still split |
| `VRW=2147483650:1.45` (pad weight 3.13e-4) | pad evicts Canvas into r11 (fp), not x from r9 |

So **pad->r9 is necessary but not sufficient**, and no source-level weight
change reaches it: the target needs pad selected before `%46`/`%47`/`%45`,
which requires a physical copy hint that a plain field load cannot have.

## 3. Source shapes measured this wave (all <= 95.538778)

Harness: write the variant over `src/game/menu/SpaceLounge.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, then the official
`tools/verify/verify.py` score on the linked `.so` (`/dev/shm/w5618/score.py`,
`autojunk=False`).

```text
variant                                             raw        size
base (HEAD, short pad3)                             95.538778  1948/1948
faithful -(width + pad*2)                           93.964335  1948/1948
flat +(-width-pad-pad)                              95.473251* 1948/1948
(width + pad3 + pad3)                               92.392049  1948/1952
(width + 2*pad) / (width + (short)pad*2)            93.964335  1948/1948
locals before clamp (boxX/boxY/left)                95.538778  1948/1948
boxY local after clamp                              93.141289  1948/1948
boxY local right after top                          71.086808  1948/1964
boxX+boxY right after top                           70.418668  1948/1952
pad2 local in top+boxW+arm                          93.964335  1948/1948
pad2 early (before top)                             93.964335  1948/1948
pad after centerX / after SetColor                  92.930679  1948/1948
lineHeight after clamp / no local                   95.538778  1948/1948
nested else / ternary else / full ternary           95.538778  1948/1948
x uninit + else centerX / cond centerX              95.538778  1948/1948
w local / inset local / right local                 95.538778 / -- / 89.612188
top before centerX                                  95.538778  1948/1948
float intermediates / static_cast                   95.538778  1948/1948
const pad/top/centerX/lineHeight                    95.538778  1948/1948
unsigned pad                                        95.264242  1948/1948
short pad                                           75.427789  1948/1960
volatile x / volatile width                         61.149111 / 69.446343
volatile rowHeight / inset / w (same codegen)       95.538778  1948/1948
register pad/x/width/top (same codegen)             95.538778  1948/1948
signed char / int16_t pad3                          95.538778  1948/1948
unsigned short / uint16_t pad3                      91.718001  1948/1956
boxW local before clamp / commuted                  95.538778  1948/1948
boxW local after clamp                              71.086808  1948/1964
copy x after clamp (drawX)                          95.538778  1948/1948
copy x before first DrawString                      71.034483  1948/1956
copy x before second DrawString                     95.538778  1948/1948
textX/textY locals                                  90.485969 / 73.553719
rect uses lineHeight                                87.895461  1948/1948
expect on clamp / else-if (0 or 1)                  91.2 - 92.1 1948/1952
expect on mode / hoverAgent                         38.7 - 77.4 1948/1940-44
invert hover / invert mode goto                     95.538778  1948/1948
vector refs / topLeft ref / bottomRight ref         91.283459 / 93.479753 / 91.832533
```

\* the flat form is the pre-wave-107 baseline, reproduced for reference.

## 4. Hypothesis (not proven)

The original's `x` live interval must be split before the `%46` eviction so
that the r5 piece is short enough to outrank `%46`/`%45`, which would leave r9
free for `pad`. The only source lever that shortens x's interval is removing
its last use (the second `DrawString` x coordinate), which is not faithful.
The tail branch-label deltas are a downstream consequence of the 2-byte code
size difference in the clamp/join region; they should collapse once that
region matches.

## 5. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# per-variant raw score (official verify.py internals, autojunk=False)
python3 /dev/shm/w5618/score.py [path/to/libgof2hdaa.so]
# RA trace (instrumented LLVM 7, same revision as NDK r18b)
RA_TRACE=1 /dev/shm/w5618/cc_inst.sh src/game/menu/SpaceLounge.cpp /dev/shm/w5618/x.s
```
