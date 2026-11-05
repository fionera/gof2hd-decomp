# ModStation ctor + MGame::OnUpdate — wave 113 (w6120): far=pitch and Status-anchor sweeps

Status: **research pass, no `src/` change landed.** Worktree `wt/w6120`, branch
`wave113-w6120`, base `a979ace18`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Two targets were swept:

* `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B — baseline **11 diff bytes / 4 runs,
  99.2 %** (wave-112 shape, unchanged).
* `_ZN5MGame8OnUpdateEv`, 11440 B — baseline **41 diff bytes / 1 run, 99.9 %**
  (wave-112 shape, unchanged).

Nothing beat either baseline. This note records the new measurements and the
mechanism found for the ModStation residual, so the next pass does not repeat
them.

## 1. ModStation — the original's far call really takes `pitch`

Wave 112 landed the shape where the far `MatrixSetRotation` passes `yaw` as its
first argument (a source-shape device to keep `yaw` live and pin the literal
pool order). Re-deriving the original disassembly shows the original's far call
first argument is **`pitch`**:

```text
d5446  ldr.w r2,[r0,r8,lsl#2]   ; r2 = pitch (near arg)
...
d5456  ldr.w r3,[r0,r8,lsl#2]   ; r3 = yaw   (near arg)
...
d546e  mov   r4,r2              ; r4 = pitch, kept across the near call
d5470  blx   MatrixSetRotation  ; near
...
d54f0  mov   r2,r4              ; far arg 1 = pitch
d54f2  ldr.w r3,[r1,r0,lsl#2]   ; far arg 2 = fresh yaw load
```

So the semantically clean spelling (`farKey, pitch, (iPad?yawIPad:yaw)[race]`)
is the original's shape. It reproduces the **register allocation exactly**:
`r2 = pitch`, `r3 = yaw`, `r4 = pitch` preserved with `mov r4,r2` right before
the `blx`, and no `mov r3,r4`. The only remaining difference is the **post-RA
schedule of the three literal materialisations**.

### 1.1 far=pitch: 42 diff bytes, 7 runs (98.6 %)

With the current wave-112 preamble (volatile int bitcasts + fence) and the far
call changed to `pitch`, the near block is:

```text
original                              far=pitch (ours)
d5436 ldr r0,[pc]  ; Pitch            d5436 ldr r0,[pc]  ; Pitch
d5438 ldr r3,[pc]  ; YawIPad           d5438 ldr r3,[pc]  ; YawIPad
d543a ldr r4,[pc]  ; Yaw               d543a add r0,pc
d543c add r0,pc                        d543c ldrb r1,[sl]
d543e add r3,pc                        d5440 ldr r4,[pc]  ; Yaw
d5440 ldrb r1,[sl]                     d5442 add r3,pc
d5444 add r4,pc                        d5444 ldr r2,[r0,r8,lsl#2] ; pitch
d5446 ldr r2,[r0,r8,lsl#2] ; pitch     d5448 add r4,pc
d544a mov r0,r3                        d544a mov r0,r3
d544c cmp r1,#0                        d544c cmp r1,#0
d544e str r3,[sp,#180]                 d544e str r3,[sp,#180]
d5450 str r4,[sp,#176]                 d5450 str r4,[sp,#176]
d5452 it eq                            d5452 it eq
d5454 moveq r0,r4                      d5454 moveq r0,r4
d5456 ldr r3,[r0,r8,lsl#2] ; yaw       d5456 str r8,[sp,#184]
d545a movs r0,#2                       d545a mov r4,r2
d545c str r8,[sp,#184]                 d545c ldr r3,[r0,r8,lsl#2] ; yaw
d5460 movs r1,#0                       d5460 movs r0,#2
d5462 add r8,sp,#256                   d5462 movs r1,#0
d5466 strd r1,r0,[sp]                  d5464 add r8,sp,#256
d546a add r1,sp,#320                   d5468 strd r1,r0,[sp]
d546c mov r0,r8                        d546c add r1,sp,#320
d546e mov r4,r2                        d546e mov r0,r8
d5470 blx                              d5470 blx
```

Four instructions are schedule-permuted (`ldr r4` literal, `str r8`,
`mov r4,r2`, `ldr r3` yaw value). Diff runs:

```text
0x16a..0x179 (16 B)   instruction order
0x186 (1 B), 0x188..0x18c (5 B), 0x18e..0x19e (17 B)   branch/offset tail
0x3e0, 0x3e4, 0x3e8   REL32 pool words
```

The three pool words are **not a pool-order change**: the order is still
`{Pitch, YawIPad, Yaw}`. The REL32 values differ only because the `add rX,pc`
sites moved by 2/4/4 bytes (`0x128bf2/0x128c12/0x128c34` vs
`0x128bf0/0x128c16/0x128c38`); the arrays themselves are at identical .rodata
addresses (`0x1fe030/0x1fe058/0x1fe080` in both binaries).

### 1.2 Root cause: the pre-RA scheduler moves the `ldrb` when `yaw` is dead

`-mllvm -print-after-all` (scratch `/dev/shm/w6120/mir.txt`) shows the
difference is already present in the **pre-RA** `Machine Instruction Scheduler`
output:

```text
baseline (far=yaw)                    far=pitch
%116 LDRLIT Pitch                     %116 LDRLIT Pitch
%165 LDRLIT YawIPad                   %165 LDRLIT YawIPad
%119 LDRLIT Yaw                       %118 ldrb [iPad]
%118 ldrb [iPad]                      %119 LDRLIT Yaw
CompilerBarrier                       CompilerBarrier
```

The post-RA "top-down list latency scheduler" then hoists all three `ldr`s in
the baseline but cannot hoist the Yaw `ldr` above the `ldrb` in far=pitch.
Keeping `yaw` live across the near call (the wave-112 device) is what makes the
pre-RA scheduler emit the three literal loads before the `ldrb`; that is
independent of the far-call argument, and no source shape tried decouples them.

### 1.3 far=pitch variants measured (all 1008 B)

Baseline (far=yaw): 11 B / 4 runs / 99.2 %.

| variant | diff B | runs | score |
|---|---:|---:|---:|
| fence after tables (wave-112 placement), far=pitch | 42 | 7 | 98.6 |
| fence after both loads, far=pitch | 42 | 7 | 98.6 |
| fence after float locals, far=pitch | 42 | 7 | 98.6 |
| no fence, far=pitch | 43 | 8 | 98.4 |
| fence after pitch load, far=pitch | 44 | 7 | 98.4 |
| fence after pitchTable only, far=pitch | 39 | 8 | 98.4 |
| far arg `*(const float *)&pitchBits` | 42 | 7 | 98.6 |
| `float farPitch = pitch` copy | 42 | 7 | 98.6 |
| near yaw = fresh volatile load, far=pitch | 43 | 7 | 97.5 |
| `pitchPtr`/`yawPtr` locals, far=pitch | 42 | 7 | 98.6 |
| `bool/int ipad = Globals::iPad` cached, far=pitch | 42 | 7 | 98.6 |
| `*(const volatile char *)&Globals::iPad` in the ternary | 42 | 7 | 98.6 |
| yaw table declared before pitch table, far=pitch | 44 | 7 | 98.4 |
| yaw load before pitch load | 1000 B | — | 88.6 |
| plain `float pitch = table[race]` (no bitcasts) | 667 | 86 | 86.7 |
| plain floats + fence | 629 | 81 | 85.9 |
| `float` volatile loads | 1016 B | — | 88.6 |

A 40-combination sweep (declaration order × load order × 5 fence positions ×
2 far-argument spellings) bottoms out at **42 B / 7 runs**; every far=pitch
shape is worse than the landed 11 B / 4 runs.

### 1.4 Why the landed shape is still the best

The landed wave-112 shape (`farKey, yaw, ...`) keeps `yaw` live, so the RA puts
it in `r4` and the near call needs one extra `mov r3,r4`; the literal scheduling
is perfect. far=pitch removes the `mov` but loses the literal scheduling
(≈16 B of instruction order + the induced branch tail). Net: 11 B beats 42 B.

## 2. MGame::OnUpdate — Status anchor sweeps

Baseline: **41 B / 1 run / 11440 B / 99.9 %** at `+0x17bb..+0x17e3`. The
original reloads `Globals::status` (`ldr.w r0,[r9]`) before each of the four
uses in the mission-23 idle condition; ours caches one `Status *st` in `r5`,
emits `mov r0,r5` for the ccm and the getPlayingTime calls, and relies on that
value to occupy `r5` so the `&Globals::status` GOT base gets `r9` (wave-112
note, §2).

New shapes measured this wave (`/dev/shm/w6120/trym4.py`, `trym5.py`):

| variant | size | diff B | runs | score |
|---|---:|---:|---:|---:|
| baseline | 11440 | 41 | 1 | 99.9 |
| `Status *st = *(Status *volatile *)&Globals::status` | 11440 | 41 | 1 | 99.9 |
| ccm load volatile (`(*(Status *volatile *)&Globals::status)->…`) | 11440 | 853 | 36 | 99.0 |
| ccm + getMission loads volatile | 11440 | 853 | 36 | 99.0 |
| volatile `st` load, all four sites plain | 11392 | — | — | 87.2 |
| `(st = Globals::status)` inside the `&&` | 11392 | — | — | 87.2 |
| m23 all `Globals::status`, `st` used at m24 getPlayingTime | 11440 | 270 | 9 | 99.7 |
| m23 all `Globals::status`, `st` used at m122 getPlayingTime | 11440 | 540 | 20 | 99.5 |
| m23 all `Globals::status`, `st` used at alien getPlayingTime | 11444 | — | — | 92.5 |

The volatile `st` load is still forwarded to the ccm (the compiler uses the
stronger volatile load's value for the later plain load), so the block stays
`ldr r5,[r9] / mov r0,r5`. Making the *ccm* load volatile instead separates the
loads but adds a fifth status load at the top (`ldr r5,[r9]` plus
`ldr r0,[r9]`), which is 2 B longer and recolours the whole function
(853 B / 36 runs).

The m23-clean variants ("m23 all `Globals::status`") do produce
`ldr r0,[r9]` for the ccm, but the anchor `st` is then loaded at the top of the
same block and forwarded to the ccm anyway (270 B / 9 runs), or the anchor is
moved to a later block and the GOT base recolours.

### 2.1 Mechanism restated

`Globals::status` is a single invariant load in the idle block, so machine CSE
merges any `Status *st = Globals::status;` with the ccm load. The merged value
then has ≥ 2 uses, is a multi-MBB global range, wins `r5` in the greedy queue,
and cannot coalesce into `r0` — hence `mov r0,r5`. The original instead
rematerialises the load at each of the four uses (`ldr r0,[r9]`), which is what
a 4-use value does when the greedy allocator splits it. The only way to get
both (four rematerialised loads *and* `r5` occupied when the `&Globals::status`
GOT base is allocated) is to have a separate long-lived value in `r5`; no
source-level anchor tried so far does that without adding or moving an
instruction inside the block.

## 3. Structural state (unchanged — no `src/` change)

```text
sections            26 (exact; same count both binaries)
file size           2,187,328 B (exact, both)
.text               0x1337c8 @ 0x69440   (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08  (size/base exact)
.ARM.extab          0x26298  @ 0x1a3b78  (size/base exact)
.rodata             0x41c27 ours / 0x41b67 orig (+0xC0 rank table)
.got                0x430c   @ 0x20fcf4  (size/base exact)
.data               0x296c   @ 0x214000  (size/base exact)
.bss                0x69ac   @ 0x216970  (size/base exact)
```

No section, `.ARM.extab` or file-size delta was introduced.

## 4. Gate (note-only pass, pristine source restored)

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN)
drift:  0 field(s) across 0 class(es)
```

Before -> after: byte 4500 -> 4500, linked 4500 -> 4500. ModStation 11 B / 4
runs / 1008 B; MGame::OnUpdate 41 B / 1 run / 11440 B.

## 5. Scratch (not committed)

`/dev/shm/w6120/` — `measure.py`, `resolve.py`, `offdump.py`, `sweep.py`
(40-combination ModStation sweep), `try2.py`–`try8.py` (ModStation variants),
`trym4.py`/`trym5.py` (MGame variants), `mir.sh`/`mirm.sh` (`-print-after-all`
dumps), `mir.txt` (ModStation pass dump), `mirm.txt` (MGame pass dump).

## 6. References

* `_work/reconstructions/modstation-ctor-wave112-w6018.md` — landed 11 B / 4 runs shape.
* `_work/reconstructions/modstation-ctor-pass5-w4263.md` §6.2 — hand-IR `best16d`.
* `_work/reconstructions/mgame-onupdate-wave112-w6019.md` — greedy-RA r5/r9 mechanism, 40 shapes.
* `_work/reconstructions/residual-taxonomy-wave111-w5920.md` — diff taxonomy.
