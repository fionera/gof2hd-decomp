# SpaceLounge::drawLounge + ModStation ctor — wave 105 re-measure (w5320)

Status: research note (2026-11-04). Worktree `wt/w5320`, branch `wave105-w5320`,
base `835748c18`. **No `src/` change landed**: every source shape tested this
wave keeps the official scores at drawLounge raw **95.473251** (pct 95.5,
1948 B) and ModStation ctor raw **98.351648** (pct 98.4, 1008 B). The tree is
clean apart from this note.

Gate (`cmake --build cmake-build-match --target check -j6`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Targets: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, byte=false,
linked=false) and `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (1008/1008 B,
byte=false, linked=false).

Prior work: `spacelounge-drawlounge-w5016.md` (newest drawLounge),
`-w4818.md`, `-w4519.md`, `-w4246.md`, `-w4227.md`, `-w4118.md`;
`modstation-ctor-pass6-w4716.md` (newest ctor), `-pass6-w4605.md`,
`-pass5-w4263.md`, `-pass4-w4220.md`. This note records only the new evidence
gathered in wave 105 and supersedes nothing.

## 1. Evidence used

* REA/Ghidra 12.1.2 decompile of `_ZN11SpaceLounge10drawLoungeEv` from
  `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
  (`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile ... --procedure
  _ZN11SpaceLounge10drawLoungeEv`). The pseudo-code confirms the committed
  source shape: `iVar10 = layout->field_0x94` (pad), `iVar12 = iVar10 * 2`,
  `iVar6 = (int)(topLeft.x + (bottomRight.x - topLeft.x) * 0.5)` (centerX),
  `iVar21 = (int)(bottomRight.y - (float)iVar12)` (top), and the clamp
  `iVar6 = (Globals::w - iVar9) - (iVar18 + iVar12)`.
* Repo side-by-side disassembly:
  `python3 tools/verify/verify.py --show <mangled>` on the linked `.so`.
* Raw (unrounded) `difflib.SequenceMatcher(..., autojunk=False)` ratio over the
  normalised instruction stream, matching `tools/verify/verify.py:score`.

## 2. drawLounge: exact residual

`osize = usize = 1948`, 178 differing bytes in 27 runs, 704 instructions each
side. The whole residual is the mode-0 name-plate clamp/join and the
post-`drawBox` agent/top restore. Register map (unchanged from w5016):

```text
value        original        ours (base)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

The original clamp right arm is two instructions; ours is four:

```text
orig:  itt gt
       addgt.w r1, r6, sl        ; width + pad*2
       subgt   r5, r0, r1        ; x = (w - inset) - (width + pad*2)
ours:  itttt gt
       movgt   r1, #0
       subgt.w r1, r1, fp, lsl #1
       subgt   r1, r1, r6
       addgt.w r9, r1, r0
```

`pad` is an unhinted `0xA` interval (plain load, no COPY in MIR), so it is
selected after every `0xE` interval; by then `x` has been evicted from `r5`
into `r9` and stays whole there. The target needs `pad` whole in `r9` and `x`
split `r5 -> r3 -> r9`.

## 3. drawLounge: new source shapes tried (all <= 95.473251 raw)

All measured on the linked `.so` with the official verifier:

```text
variant                                             raw        size
base (flat clamp + __builtin_expect)                95.473251  1948
clamp -(width + pad*2)                              93.964335  1948
clamp -(width + pad + pad)                          95.3       1948
clamp right < pad + x + width                       95.473251  1948
clamp x + pad + width > right                       95.473251  1948
clamp width + pad + x > right                       95.473251  1948
pad2 local (pad*2 or pad+pad) in top/box/clamp      93.964335  1948
top = bottomRight.y - (float)(pad*2)                95.473251  1948
swap centerX/top declaration order                  95.473251  1948
const int pad                                       95.473251  1948
x = centerX + pad - pad                             95.473251  1948
x = centerX + pad*0                                 95.473251  1948
pad + pad + width in drawBox/DrawRectangle          95.473251  1948
top pad+pad+(pad-pad) etc.                          95.473251  1948
inline centerX into x (no centerX local)            87.5       1948
move centerX/top block after width branch           70.1       1976
precompute xPad / topPad / boxW locals              81.0/94.6/71.0  1964/1948/1968
cache Globals::font in a local                      66.4       1976
cache Globals::w in a local                         84.8       1960
```

The compiler canonicalises every algebraic pad-use inflation before the
register allocator, so pad's weight does not move. The only shapes that change
the allocation are the faithful `pad2` clamp (which makes the clamp arm match
but transposes pad/x and drops the score) and `-stress-regalloc` (knobs, not
source).

## 4. ModStation ctor: exact residual

`osize = usize = 1008`, **39 differing bytes in 8 runs** (unchanged from
w4716 with the landed fence):

```text
+0x169..+0x169  1B  orig=4b  ours=4c
+0x16b..+0x16b  1B  orig=4c  ours=4b
+0x16e..+0x179 12B  orig=7b449af800107c4450f82820
                    ours=9af800107b4450f828207c44
+0x186..+0x186  1B  orig=50  ours=cd
+0x188..+0x18c  5B  orig=28300220cd  ours=b880144650
+0x18e..+0x19e 17B  orig=b88000210df58078cde9001050a9404614
                    ours=2830022000210df58078cde9001050a940
+0x3e4..+0x3e4  1B  orig=16  ours=34
+0x3e8..+0x3e8  1B  orig=38  ours=12
```

The near-camera rotation block is the only code difference. The two literal
pool slots at `+0x3e4`/`+0x3e8` follow the LDRLIT order.

### 4.1 New observation: the volatile yaw load controls the LDRLIT order

The current source (committed base) is:

```cpp
__atomic_signal_fence(__ATOMIC_SEQ_CST);
const float *pitchTable = gHangarCamPitch;
const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits = *(const volatile int *) &yawTable[race];
```

With the `volatile` yaw load the LDRLIT order is **Yaw (r4) then YawIPad
(r3)** (ours) vs **YawIPad (r3) then Yaw (r4)** (original). Removing the
`volatile` from the yaw load changes the LDRLIT order to the original's
(YawIPad first) but degrades the score to raw 97.8 because the post-RA
scheduler then delays the Yaw LDRLIT past `cmp` and moves the pitch copy
(`mov r4, r2`) before the yaw load:

```text
plain-load ours (97.8):           original (98.351648):
ldr r0,[pc]  ; Pitch              ldr r0,[pc]  ; Pitch
ldr r3,[pc]  ; YawIPad            ldr r3,[pc]  ; YawIPad
add r0,pc                         ldr r4,[pc]  ; Yaw
ldrb r1,[sl]                      add r0,pc
add r3,pc                         add r3,pc
ldr r2,[r0,race*4]                ldrb r1,[sl]
cmp r1,#0                         add r4,pc
ldr r4,[pc]  ; Yaw  <-- delayed   ldr r2,[r0,race*4]
mov r0,r3                         mov r0,r3
str r3,[sp,#180]                  cmp r1,#0
add r4,pc                         str r3,[sp,#180]
str r4,[sp,#176]                  str r4,[sp,#176]
it eq                             it eq
moveq r0,r4                       moveq r0,r4
mov r4,r2  <-- early              ldr r3,[r0,race*4]
ldr r3,[r0,race*4]                movs r0,#2
str r8,[sp,#184]                  str r8,[sp,#184]
...
```

So the `volatile` is a scheduling anchor (it forces the Yaw LDRLIT early) but
it also forces the select operand order into the DAG, which is the reverse of
the module/`.rodata` order.

### 4.2 Root cause: InstCombineSelect canonicalisation

The IR after optimisation is:

```llvm
%98 = load i8, i8* @_ZN7Globals4iPadE
%99 = icmp eq i8 %98, 0
%100 = select i1 %99, [10 x float]* @_ZL13gHangarCamYaw,
                        [10 x float]* @_ZL17gHangarCamYawIPad
```

`select eq, Yaw, YawIPad` (true = Yaw) is the canonical form of the source
`iPad ? YawIPad : Yaw`. SelectionDAG visits the true operand first, so ours
emits `Yaw` before `YawIPad`. The original's select must be the non-canonical
`select ne, YawIPad, Yaw` (true = YawIPad), which emits the module order. The
camKey block is byte-identical because its three selects share one `icmp`
(three uses), so InstCombineSelect's `m_OneUse(m_ICmp(...))` canonicalisation
does not fire; the yaw select's `icmp` has one use and is flipped.

### 4.3 New shapes tried to stop the canonicalisation (all still canonical)

```text
variant                                             result
bool ipad = Globals::iPad != 0; ... ipad ? ...      still icmp eq, one use
int  ipad = Globals::iPad != 0; ...                 still icmp eq, one use
bool ipad = Globals::iPad; (no != 0)                still icmp eq, one use
explicit if/else pointer assignment                 still icmp eq, one use
switch (Globals::iPad) { case 0: ... }              still icmp eq, one use
!Globals::iPad ? Yaw : YawIPad                      still canonicalised
Globals::iPad == 0 ? Yaw : YawIPad                  still canonicalised
Globals::iPad != 0 ? YawIPad : Yaw                  still canonicalised
dummy pitch ternary on the same condition           folded before InstCombine
(ipad, ipad) / (ipad && true) / empty if (ipad)     extra uses folded away
__builtin_expect(ipad, 0) ? ...                     still canonicalised
static inline pickYawTable(bool)                    inlined, still canonicalised
```

Fence/barrier sweep (all 39/8 except where noted):

```text
fence before both table decls (base)                39/8
fence after pitch decl, before yaw decl             39/8
__ATOMIC_ACQ_REL / ACQUIRE / RELEASE before decls   39/8
__ATOMIC_RELAXED before decls                       43/8
fence after pitch load / after yaw load             43/8
fence before MatrixSetTranslation                   103/28
```

Other ctor permutations (all 39/8): swap the two yaw array declarations, swap
the pitch/yaw loads, move `Matrix nearKey` before `int race`, precompute
`race*3`, copy `race` to a new local, `volatile` table pointers, a
`const volatile float *yp = yawTable` local, and `*(const int *)` plain yaw
load (97.8).

## 5. Next hypotheses

1. **drawLounge.** The target needs `pad` to be selected while `r9` is free
   and `r5` is already held by `x`; this is a greedy-RA ordering window. The
   only untried source lever from w4519 §8 is a shape in which `pad`
   participates in a COPY that coalesces onto it and carries a physical `r9`
   hint (e.g. an inlined helper whose parameter is passed as a call argument,
   or a value the coalescer merges with `pad`). A second lever is an extra
   live value across the clamp that reproduces the `-stress-regalloc=12`
   allocation without adding instructions (the original may have cached a
   global that the current source reloads).
2. **ModStation ctor.** Find a source shape in which the yaw select's `icmp`
   is genuinely multi-use at InstCombineSelect time (not folded before it), or
   a shape that keeps the volatile scheduling anchor while emitting the
   LDRLITs in module order. The former would make the select
   `select ne, YawIPad, Yaw` and fix both the LDRLIT order and the `.rodata`
   order at once; the latter needs a post-RA scheduling anchor independent of
   the select operands.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Scratch (not committed, `/dev/shm/w5320`): `rea-drawLounge.json` / `.c`
(Ghidra decompile), `dlA.txt` (drawLounge side-by-side), `ms.txt` / `msN.txt`
(ctor side-by-side, volatile/plain), `rundiff.py` (byte-run diff),
`score_raw.py` (unrounded difflib ratio), `try*.py` (variant sweeps).
