# Hud::draw / SpaceLounge::drawLounge / MenuTouchWindow::OnTouchBegin — wave-117 triage (w6520)

**Status:** research note (2026-11-05). Worktree `wt/w6520`, branch
`wave117-w6520`, base `c22678d64` (wave-116 harvest). **No `src/` change
landed**: every measured variant of all three named functions is at or below
the committed HEAD. The tree is clean apart from this note.

| function | raw fuzzy | size | insns (orig/ours) | byte/linked |
|---|---:|---:|---:|---|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | **95.605267 %** | 9332 / 9332 | 3149 / 3154 | no / no |
| `_ZN11SpaceLounge10drawLoungeEv` | **95.676047 %** | 1948 / 1948 | 729 / 728 | no / no |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | **98.092643 %** | 1896 / 1896 | 734 / 734 | no / no |
| (bonus) `_ZN5MGame8OnUpdateEv` | **99.935275 %** | 11440 / 11440 | 3862 / 3863 | no / no |

Gate on the untouched tree: **RATCHET PASS**, `byte_exact 4501`,
`linked_exact 4501`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint
**CLEAN**, drift **0**. Structural state unchanged: 27 sections, file size
2,187,328 B, `.text 0x1337c8`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26294`
(4 B short), `.rodata 0x41c27`.

---

## 1. The RocketGun `operator[]` lever does not exist in any of the three

The wave-116 recipe (`ptr->data()[i]` → `(*ptr)[i]`) is only live when the
source contains a `data()[i]` element access.  Grep result over the whole
files:

```text
src/game/ui/Hud.cpp              : no data()
src/game/menu/SpaceLounge.cpp    : no data()
src/game/ui/MenuTouchWindow.cpp  : no data()
```

`Hud::draw`'s only indexed accesses are the plain C arrays
`fireButtonImages[fireImageIndex]` / `fireButtonPressedImages[fireImageIndex]`
(`unsigned int[4]`, not `Array<>`), and `SpaceLounge::drawLounge` /
`MenuTouchWindow::OnTouchBegin` already use the winning `(*ptr)[i]` form
throughout.  There is no container-syntax lever left in these three; the
residual is register allocation / scheduling / EH landing-pad placement.

---

## 2. `Hud::draw` (95.605267 %, 9332 B)

### 2.1 The original second bar is `String`-first, but HEAD's `halfW`-first wins

The original at `0x165ba6`–`0x165bf8` evaluates the `String` construction
*before* `GetImage2DWidth`:

```text
165ba6: ldr r0,[pc]        ; Globals::gameText
165baa: mov.w r1,#318
165bae: ldrb.w r2,[sl,#631]   ; this->field_0x277
165bb2: add r0,pc
165bb4: cmp r2,#0
...      blx GameText::getText
         blx String::String(String const&, bool)
165bce: ldr r1,[sl,#892]   ; this->image_0x37c
165bd2: ldr r0,[r9]        ; Globals::Canvas
165bd6: blx PaintCanvas::GetImage2DWidth
...
165bf6: blx PaintCanvas::GetImage2DHeight
```

so the faithful order is `String text; int halfW; int barH`.  HEAD has
`int halfW; String text; int barH`.  The faithful order reproduces the second
bar byte-for-byte (verified against the original disassembly), but the whole
function shrinks to **9328 B** and the score falls to **93.8 %**.  A full
re-sweep (`H`/`S` order × `frameH` × `total` × `barW` × `halfW` × `barH`,
64 builds) confirms the unique best is HEAD:

| order | frameH | total | barW | halfW | barH | size | raw % |
|---|---|---|---|---:|---:|---:|---:|
| **H** | **short** | **short** | **unsigned short** | **int** | **int** | **9332** | **95.605267** |
| S | int | short | unsigned short | int | int | 9328 | 94.3 |
| S | short | short | int | int | int | 9328 | 94.1 |
| H | int | short | unsigned short | int | int | 9328 | 93.9 |
| S | short | short | int | short | int | 9340 | 93.9 |

The 4-byte size difference is load-bearing: any non-9332 size shifts every
later function's PIC literal deltas and collapses the gate (wave-4253 note).

### 2.2 `Status` gamma block: the faithful direct form is worse

The original gamma block keeps `&Globals::status` in `r6`, loads the value into
`r4`, and **reloads** it for `getCurrentCampaignMission`:

```text
164010: ldr r6,[r0,#0]     ; r6 = &Globals::status
164012: ldr r4,[r6,#0]     ; r4 = Globals::status
164014: mov r0,r4
164016: blx Status::getStation
16401a: blx Station::getIndex
16401e: mov r5,r0
164020: ldr r0,[r6,#0]     ; reload
164022: str r6,[sp,#108]
164024: blx Status::getCurrentCampaignMission
```

Writing the condition with three direct `Globals::status` references reproduces
this block exactly, but the whole function drops to **9312 B / 74.8 %** because
the shield block loses the branch-local `this` spill (`str r5,[sp,#104]`).
The committed `Status *st = Globals::status;` local is what forces that spill
(wave-4253) and is worth ~21 points.  Mixed spellings (direct call / `st`
arguments in either combination) compile to the same 95.605267 % as HEAD
(the compiler CSEs them); `Status *&st` / `Status *const &st` behave like the
direct form (74.8 %).

### 2.3 Guard / dead-load residual

The original damage block has two dead `ldr.w r8,[sp,#108]` loads
(`0x164176`, `0x164210`, `[sp,#108]` = `&Globals::status`) and no null check at
the fourth bar (`0x1642f2` starts directly with `ldr r0,[sl,#1208]`).  HEAD
instead emits a real `Globals::status &&` check at `0x1642ee`
(`ldr r0,[r8]; cmp r0,#0; beq`).  Removing the guard, or folding it with
`__builtin_assume(Globals::status != 0)`, shrinks the body to **9264 B /
83.3 %**; the guard is an accidental RA/pool anchor.  `st &&` (which the
compiler can fold) explodes to 9364 B / 63.9 %.  Guard spellings
`Globals::status`, `!= 0`, `!= nullptr`, `> 0` are byte-identical.

### 2.4 Recipe (best known, no change)

Keep exactly:

```cpp
Status *st = Globals::status;                     // forces the shield-block this spill
if (st->getGammaRayDamagePerSecond(...) > 0.0f)   // gamma block
...
if (Globals::status && this->field_0x4b8 >= 1)    // real check keeps 9332 B
...
short frameH; int textH; int transferred; short total;      // docking bar
...
int halfW; String text; int barH;                 // second bar (tuned order)
unsigned short barW; int barH;
```

No source-reachable variant beat 95.605267 %.

---

## 3. `SpaceLounge::drawLounge` (95.676047 %, 1948 B)

### 3.1 The original clamp is the faithful `width + pad * 2`

Confirmed against the original at `0x1712dc`:

```text
1712dc: itt gt
1712de: addgt.w r1,r6,sl      ; width + (pad*2), sl = pad*2 from 0x170ed2
1712e2: subgt   r5,r0,r1      ; x = (w - inset) - (width + pad*2)
```

HEAD's `short pad3 = pad; ... (width + pad3 * 2)` emits the 16-bit sign-extend
(`movgt r1,fp,lsl #16; addgt r1,r6,r1,asr #15`), which is *not* the original.
The faithful form scores **93.964335 %** because `pad` lands in `fp` and the
plain form needs a second `add` at the join; the tuned form scores
**95.676047 %**.  This is the same finding as
`spacelounge-drawlounge-wave116-w6417.md` §1 (the note's description of the
original is correct; HEAD is the tuned shape).

### 3.2 Recipe (best known, no change)

Keep HEAD's `short pad3` clamp (tuned but still real C++).  The faithful
`width + pad * 2` is the structurally right source but loses 1.7 points to
greedy RA.  The remaining residual is the `pad -> r9` / `x -> r5` colouring and
the tail EH landing-pad selectors (one `nop` + one `b.n`).

---

## 4. `MenuTouchWindow::OnTouchBegin` (98.092643 %, 1896 B)

Only **one** scheduling difference remains, in the case-15 scrollbar hit test:

```text
original:  ldr r5,[r2,#12]   ; layout.field_0xc_leftMargin
           ldr r6,[r2,#32]   ; layout.field_0x20_top
           blx  GetImage2DHeight
           adds r1,r6,r5
HEAD:      ldr r4,[r6,#32]   ; top, using the first condition's layout pointer
           ...
           ldr r5,[r2,#12]   ; leftMargin, using the reloaded pointer
           blx  GetImage2DHeight
           adds r1,r5,r4
```

The source is

```cpp
Layout &layout = *Globals::layout;
if (x > Globals::w - layout.buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
    y < layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])))
    this->scrollbarHit = 1;
else
    this->scrollbarHit = 0;
```

Measured variants (all 1896 B): `layout`/`Globals::layout` mixed spellings
(`g_l` 98.0, `g_g` 90.3, `l_l` 90.9), pointer form `Layout *layout` (98.1,
identical), `int w = Globals::w` (98.1), condition swap (89.1), negated form
(98.1), ternary (89.4), no-parens (97.1), `top`-group-first (90.7),
`leftMargin`-group-first (98.1, same single load diff), subtraction form
(91.5), hoisted `bottom` local (89.2).  None beat 98.1.  The single load
reorder is a greedy-RA/scheduler artefact, not source shape: every spelling
that keeps the `Layout &layout` local makes the compiler CSE the first
condition's layout pointer into `r6` and hoist the second condition's first
operand (`top` or `leftMargin`) to `0x12a214`; the original reloads
`Globals::layout` into `r2` at `0x12a212` and uses `r2` for both operands.

---

## 5. Bonus: `MGame::OnUpdate` is one cached-status load from 100 %

This is the closest non-exact function in the tree and shares the same
"cached vs reloaded `Globals::status`" residual as `Hud::draw`:

```text
original:  ldr r9,[r0]        ; r9 = &Globals::status
           ldr r0,[r9]        ; 0x17e090
           blx  Status::getCurrentCampaignMission
           ...
           ldr r0,[r9]        ; 0x17e0a8
           blx  Status::getMission
           ...
           ldr r0,[r9]        ; 0x17e0b8
           blx  Status::getPlayingTime
HEAD:      ldr r5,[r9]        ; caches Globals::status
           mov r0,r5
           ...
           mov r0,r5          ; 0x17e0ba
           blx  Status::getPlayingTime
```

The source at `src/game/menu/MGame.cpp:2767` has
`Status *st = Globals::status;` used only for `st->getPlayingTime()`.  Removing
the local (all direct `Globals::status` refs) is faithful but cascades through
the 11 KB function to **11392 B / 87.2 %**; keeping `st` but using it for
`getMission`/`field_0x100` also loses (92.5–92.7 %).  HEAD's one-use local is
the best measured shape (99.935275 %).

---

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness lived in `/dev/shm/w6520/` (`score.py`, `raw.py`, `norm.py`,
`sweep_second.py`, `sweep_dock.py`, `sweep_combo.py`, `try*.py`); none of it is
in the repo.
