# LevelScript::process — wave 136 / w8204: mission-0 state machine low-register map

Status: **analysis only, no landed source change**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8204`. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `655e3bcc5` (wave-136 docs head). Scratch helpers in
`/dev/shm/w8204/` (not tracked): `parse.py`, `sbs.py`, `score.py`,
`regiondiff.py`, `variant.py`, `run.sh`, `LevelScript.base.cpp`.

## Result

```
                         before (master)   after (no code change)
official verify pct      58.9              58.9
orig / ours insns        20169 / 20049     20169 / 20049
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54327             54327
16-bit / 32-bit (ours)   12798 / 7909      12798 / 7909
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
```

## 1. The original's mission-0 state-0 setup register map (confirmed)

The mission-0 branch is entered at `0x137e84` with `sl = this` (the prologue's
`mov sl,r0`), exactly like ours. The original then does a **register swap** in
the state-0 setup (disasm `0x137e9c`):

```
orig 137e8c  ldr.w r1,[sl,#28]     ; sl = this  (m_nState)
     137e9c  mov   r4, sl          ; r4 = this
     137e9e  ldr   r5,[pc]         ; r5 = -12000.0f
     137ea4  str   r0,[r4,#28]     ; this->m_nState = 1   (2-byte str!)
     137ea6  mov   sl, fp          ; sl = Globals::status (fp is needed for the temp Vector)
     137eb2  ldr   r0,[r4,#20]     ; this->m_pCamera
     137ebc  mov   r9, r2          ; r9 = 2000.0f
     ...
     137f24  ldr.w r9,[sp,#96]     ; r9 = enemies (hoisted out of the loop)
     137f28  add.w fp, sp, #280    ; fp = &temp2
     137f2c  mov.w r4,#1065353216  ; r4 = 1.0f
     137f76  mov   fp, sl          ; fp = status (restored)
     137f78  ldr.w sl,[sp,#88]     ; sl = this (restored)
```

Ours (baseline) instead keeps `this` in `sl` and puts `status` in `r9`:

```
ours 137eaa  str.w r0,[sl,#28]     ; this->m_nState = 1   (4-byte str.w)
     137eae  mov   r9, fp          ; r9 = status
     137ec2  ldr.w r0,[sl,#20]     ; this->m_pCamera
     137ec8  mov   sl, r2          ; sl = 2000.0f
     ...
     137f30  add.w sl, sp, #280    ; sl = &temp2
     137f34  mov.w fp,#1065353216  ; fp = 1.0f
     137f76  mov   fp, r9          ; fp = status
     137f78  ldrd  sl, r9,[sp,#88] ; sl = this, r9 = status
```

So the swap is a pure greedy-allocator tie-break: the original gives `sl` to
`status` and `r4` to `this`; ours gives `sl` to `this` and `r9` to `status`.
`this` is used only twice in the state-0 setup (`m_nState`, `m_pCamera`), so
the original's `r4` form emits two 2-byte `[r4,#imm]` loads/stores where ours
emits 4-byte `[sl,#imm]` ones. This is the same phenomenon w8103/w7765 found on
the whole function, localized to the mission-0 state-0 setup.

## 2. NEW structural finding: the state-0 visibility loop double-indexes

The original's three-ship loop (`0x137f34`) reloads `(*enemies)[i]` for **both**
calls:

```
orig 137f34  ldr.w r0,[r9,#4]          ; enemies->data
     137f3c  ldr.w r0,[r0,r5,lsl #2]   ; enemies->data[i]
     137f40  ldr   r0,[r0,#8]          ; ->geometry
     137f4e  blx   AEGeometry::setDirection
     137f52  ldr.w r0,[r9,#4]          ; enemies->data   (RELOAD)
     137f58  ldr.w r0,[r0,r5,lsl #2]   ; enemies->data[i] (RELOAD)
     137f5c  ldr   r0,[r0,#8]
     137f5e  blx   AEGeometry::setVisible
```

Our source (`LevelScript.cpp:634`) caches the element in a local, so clang emits
one load and reuses it:

```cpp
KIPlayer *enemy = (*enemies)[i];
enemy->geometry->setDirection(...);
enemy->geometry->setVisible(true);
```

The faithful form is to index twice:

```cpp
(*enemies)[i]->geometry->setDirection(Vector{1,0,0}, Vector{0,1,0});
(*enemies)[i]->geometry->setVisible(true);
```

This is a genuine source-level difference (the original loop is 68 B, ours
56 B), and it is the **only** local-pointer-vs-reload difference in the whole
mission-0 branch: the wobble loop (`0x138426`) and the state-2 loop
(`0x1384f8`) already double-index and match instruction-for-instruction.

## 3. Applying the loop fix re-colours the prologue and costs +120 B

Built and scored (all with the real project flags, `link_so` + full verify):

| variant | size | pct | diffbytes | insns |
|---|---:|---:|---:|---:|
| baseline `e,p,m` | **57232** | **58.9** | **54327** | 20049 |
| state-0 loop double-index | 57352 | 51.6 | 55687 | 20077 |
| + `unsigned int i` | 57352 | 51.6 | 55687 | 20077 |
| + `KIPlayer *const enemy` first call only | 57352 | 51.6 | 55687 | 20077 |
| + `KIPlayer *const enemy` second call only | 57352 | 51.6 | 55687 | 20077 |
| + `Array<KIPlayer*> *const enemies` | 57352 | 51.6 | 55687 | 20077 |
| + `const` on all three accessor locals | 57352 | 51.6 | 55687 | 20077 |
| `m,p,e` prologue + loop fix | 57452 | 50.8 | 55635 | 20109 |
| `e,m,p` prologue + loop fix | 57416 | 51.9 | 55553 | 20107 |
| `p,e,m` / `p,m,e` / `m,e,p` + loop fix | 57452 | 50.8 | 55635 | 20109 |

The loop fix adds 32 instructions (the missing reloads) but **+120 B**, because
the extra live value re-colours the prologue: `delta` moves from `r6` to `r4`
(`mov r4,r1` instead of `mov r6,r1`), the spill slots shift
(`messages@76`, `delta@88`, `this@92` instead of `messages@72`, `delta@76`,
`this@88`), and the whole function's 16/32-bit mix moves the wrong way
(16-bit 12798→12794, 32-bit 7909→7941). The loop body itself is only +2 B; the
other +118 B is the global re-colouring. `const`, `unsigned`, and
local-vs-reload spelling are all coalesced by clang (identical bytes).

## 4. Other local-placement experiments (all byte-identical or worse)

| variant | size | pct | note |
|---|---:|---:|---|
| `LevelScript *self = this;` + `self->` in the state-0 setup | 57232 | 58.9 | **byte-identical** (coalesced) |
| `LevelScript &self = *this;` + `self.` in the state-0 setup | 57232 | 58.9 | **byte-identical** |
| `Level *level = m_pLevel;` local in the state-0 setup | 57260 | 46.1 | +28, re-colours |
| `const` on `enemies`/`player`/`messages` declarations | 57232 | 58.9 | **byte-identical** |

## 4b. Body order of the state-0 setup DOES move the size (new evidence)

w8103 reported that local aliases/declaration moves are byte-identical. That is
true for *aliases*, but reordering the four state-0 setup statements is **not**
a no-op: all 23 non-identity permutations of `{m_nState=1, player->setPosition,
m_pCamera->setPosition, MatrixSetTranslation}` were built and scored. None hits
57232:

| order (s=state, p=player, c=camera, m=matrix) | size | pct | diffbytes |
|---|---:|---:|---:|
| **s,p,c,m (master)** | **57232** | **58.9** | **54327** |
| s,c,m,p | 57228 | 58.6 | 54905 |
| p,s,c,m / p,c,s,m / p,c,m,s | 57228 | 58.7 | 54905 / 54905 / 54904 |
| c,s,m,p | 57228 | 58.6 | 54915 |
| c,p,m,s | 57228 | 58.7 | 54903 |
| c,m,s,p / c,m,p,s | 57220 | 58.6 | 55010 / 55012 |
| s,c,p,m / p,s,m,c / c,s,p,m | 57360 | 55.2 | ~55545 |
| s,p,m,c / p,s,m,c / c,p,s,m | 57356 | 55.2 | ~55548 |
| s,m,p,c / m,s,p,c / m,p,s,c / ... (m first) | 57372 | 48.9 | ~55590 |

So the *order* of the setup statements is a real lever (the `-4`/`-12` variants
prove the scheduler/allocator sees them), but the master order is already the
best both in size (exactly 57232) and pct (58.9). Combining the `-4` orders with
the double-index loop still lands at 57340/57352/57360, never 57232.

## 5. Why the `sl`-vs-`r4` swap cannot be forced from source

The allocator's choice is a single tie-break between two long live ranges that
both start in the prologue. `this` is used ~2019 times; `status` is used
throughout the shared tail. The original spills `this` to `[sp,#88]` and
rematerialises it into `r4`/`r5`/`r6` at each case entry; ours keeps `this` in
`sl` and spills `status` to `[sp,#92]`. Local aliases (`self`, `level`,
`camera`) coalesce; declaration-order changes move the call order and change
the size; `const`/`unsigned` are no-ops. This is exactly the conclusion of
w8103/w7765, now confirmed for the mission-0 state machine specifically.

## 6. Conclusion / next leads

1. The state-0 visibility loop is a **real missing-reload** (original indexes
   `(*enemies)[i]` twice). It is not landable size-exact on its own because it
   re-colours the prologue (+120 B, pct 51.6). It needs a companion that keeps
   `delta` in `r6` and the prologue slots at `messages@72, delta@76, this@88`.
2. The mission-0 state-0 setup's `mov r4,sl` / `mov sl,fp` swap is the
   localized form of the w8103 `sl`-ownership problem. Forcing it would recover
   the two 2-byte `[r4,#imm]` accesses and the `str` (vs `str.w`), but no
   source shape tried here moves it.
3. The remaining ~95 missing instructions (20049 vs 20169 after the +32 loop
   fix) are arithmetic/branch, not calls (w7765 census). The next pass should
   audit the mission-0 switch states 6–15 for more double-index / reload
   differences before touching the prologue.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8204/score.py        # 57232, pct 58.9, diffbytes 54327
python3 /dev/shm/w8204/sbs.py 0x137e84 0x137f80   # state-0 setup side-by-side
python3 /dev/shm/w8204/variant.py di   # apply the loop fix and score
```
