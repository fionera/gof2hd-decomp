# Hud::draw — the `.ARM.extab` / LSDA size trap, the real 89.1 % baseline, and the dock-block reorder (w4273)

**Status:** analysis only, 2026-11-03. Worktree `wt/w4273`, branch `wave92-w4273`,
base `master b379d00d` (HEAD `eea2e3c1`). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp:1091`), original size **9332 B**.

Gate on entry and on exit (full `check`): **byte_exact 4499 / linked_exact 4499**,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`, lint CLEAN, drift 0, **RATCHET PASS**.

This note extends `hud-draw-status-slot-w4253.md`, `hud-draw-status-slot-w4233.md`,
`hud-draw-st-local-w4215.md`, `hud-draw-slot-coloring-w4114.md`,
`hud-draw-slot-rotation-w4014.md` and `hud-draw-this-spill-w3910.md`.

---

## 0. TL;DR

1. **The 91.7 % figure in `hud-draw-status-slot-w4253.md` and in commit `1b5a5f7b`
   is not reproducible.** Re-running the *official* verifier on HEAD gives
   **89.1 %** for `Hud::draw`; the parent of `1b5a5f7b` gives **86.2 %** (the note
   claims 88.8 % / 91.7 %). The real pass-94 history is **86.2 -> 89.1**.
   `tools/verify/verify.py` was last touched 2026-10-21 (`7ce69b63`), i.e. *before*
   `1b5a5f7b` (2026-11-03), so the tool did not change under the note.
2. **`Hud::draw`'s size is not the only hard constraint — its LSDA size is too.**
   The unwind/LSDA payload of this function lives in `.ARM.extab`, which sits
   immediately below `.rodata`. `.ARM.extab` currently ends *exactly* on
   `.rodata`'s 16-byte-aligned start (`0x1a3b78 + 0x26298 = 0x1c9e10`), so **any
   positive growth of Hud::draw's call-site table shifts `.rodata` by 0x10** and
   silently un-matches **459 byte-exact functions** (byte_exact 4499 -> 4040).
   This is the trap that makes the otherwise-good dock-block reorder unlandable.
3. A one-line statement reorder in the docking-transfer block moves the official
   fuzzy score **89.1 -> 90.7** at the unchanged 9332-byte size — but it grows the
   LSDA call-site table by 12 bytes (77 -> 79 entries), so it **fails the gate**.
   Measured, not guessed: `byte_exact 4499 -> 4040`, `RATCHET FAIL`.

---

## 1. Baseline correction: 89.1 %, not 91.7 %

Reproduction (official scorer, `autojunk=False`, exactly what `check` runs):

```
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

| tree | `Hud::draw` size | official fuzzy |
|---|---|---|
| `1b5a5f7b^` (parent, saved as `Hud_parent.cpp`) | 9332 | **86.2** |
| HEAD `eea2e3c1` | 9332 | **89.1** |
| `hud-draw-status-slot-w4253.md` / `1b5a5f7b` message | 9332 | *claims* 88.8 -> 91.7 |

`git log -1 --format=%ci 7ce69b63` = 2026-10-21, `git log -1 --format=%ci 1b5a5f7b`
= 2026-11-03, so the verifier predates the pass. The 91.7 % number cannot be
reproduced by the tool that defines the metric; treat 89.1 % as the real baseline.

---

## 2. Ground truth for the gamma block (Ghidra `Hud_draw` @ Ghidra 0x173b90 = file 0x163b90, size 9332)

```
16400c  ldr  r0, [pc, #276]        ; GOT &Globals::status
16400e  add  r0, pc
164010  ldr  r6, [r0, #0]          ; r6 = &Globals::status
164012  ldr  r4, [r6, #0]          ; r4 = Globals::status
164014  mov  r0, r4
164016  blx  Status::getStation()
16401a  blx  Station::getIndex()
16401e  mov  r5, r0
164020  ldr  r0, [r6, #0]          ; <-- RELOAD Globals::status (2nd write)
164022  str  r6, [sp, #108]        ; <-- &Globals::status spilled AFTER the reload
164024  blx  Status::getCurrentCampaignMission()
164028  mov  r2, r0
16402a  mov  r0, r4
16402c  mov  r1, r5
16402e  blx  Status::getGammaRayDamagePerSecond(int,int)
```

HEAD (with the `Status *st` local) instead caches the value and spills early:

```
16400c  ldr  r0, [pc, #276]
16400e  add  r0, pc
164010  ldr  r0, [r0, #0]          ; &Globals::status
164012  str  r0, [sp, #108]        ; <-- spill BEFORE the value load
164014  ldr  r4, [r0, #0]          ; r4 = Globals::status (cached)
164016  mov  r0, r4
164018  blx  Status::getStation()
16401c  blx  Station::getIndex()
164020  mov  r5, r0
164022  mov  r0, r4                ; <-- no reload (uses the cached r4)
164024  blx  Status::getCurrentCampaignMission()
```

The block is the **same length** in both (0x16400c..0x16403e); the difference is
purely *which* register holds `Globals::status` and *where* the `&Globals::status`
store lands. `Status *st = Globals::status;` is nevertheless load-bearing: deleting
it (the "clean gamma" shape, i.e. three textual `Globals::status` uses) drops the
frame from 224 to 216, the size from 9332 to 9312 and the score to **74.2 %**, and
loses the shield block's `str r5, [sp, #104]` `this` spill.

### 2.1 The two dead `&Globals::status` reloads

The original also contains two **dead** `ldr.w r8, [sp, #108]` reloads of
`&Globals::status`; ours has neither:

```
164176  f8dd 806c   ldr.w r8, [sp, #108]     ; dead: r8 overwritten at 0x1641e4
164210  f8dd 806c   ldr.w r8, [sp, #108]     ; dead: r8 overwritten at 0x16431c
```

They are 4 bytes each and they are the reason for the **-4 / -8 byte drift** of
everything between 0x16417a and 0x1642f2, which in turn is the reason for the
large number of branch-target-only diffs (`beq.w 0x936` vs `0x934`, ...).
Removing the invented damage-block null guard is not an option: without it the
function shrinks to 9264 B and the score falls to 82.0 %.

---

## 3. The `.ARM.extab` / LSDA trap (new, and the reason the dock reorder cannot land)

### 3.1 Layout

```
.ARM.exidx  0x19cc08  size 0x06f70   (one 8-byte entry per function; unchanged)
.ARM.extab  0x1a3b78  size 0x26298   ends at 0x1c9e10
.rodata     0x1c9e10  size 0x41b67   (align 16)
```

`.ARM.extab` is not only unwind opcodes: for a C++ function with a personality
routine it holds `prel31(personality)` + the unwind opcodes + the **LSDA**
(call-site table). Hud::draw's share is 476 bytes in the original and at HEAD.

### 3.2 Hud::draw's extab entry

```
0x1c57c8: 92010000 9493e97f c94f9702 f0800083 b0b0b0ab   ; prel31 PR + 12 opcode bytes
0x1c57dc: ff ff 01 <uleb cs_table_len> ...               ; LSDA
```

* `ff` = LPStart omitted, `ff` = TType omitted, `01` = DW_EH_PE_uleb128.
* Call-site table: **original 77 entries / 454 bytes**, **HEAD 77 entries / 453 bytes**,
  **staged dock-reorder 79 entries / 465 bytes**.

The two extra entries are real, new landing pads:

```
HEAD    ... (7628,10,lp=9058) (7702,112,lp=9184) ...
staged  ... (7574,10,lp=9058) (7598,4,lp=9056) (7612,4,lp=9054) (7696,112,lp=9184) ...
```

i.e. moving the `String text = *Globals::gameText->getText(...) + String(" ");`
construction earlier splits two call ranges (the `String(" ")` temporary and the
`operator+` result get their own cleanup ranges).

### 3.3 Consequence

`0x1a3b78 + 0x262a4 = 0x1c9e1c` -> `.rodata` is 16-byte aligned -> **`0x1c9e20`**,
a +0x10 shift. Every `.text` literal-pool word that names a `.rodata` address
changes by 0x10, and `verify.py`'s `byte` flag is a raw byte compare
(`be = ob == ub`), so those functions stop being byte-exact even though their
instructions are identical:

```
_ZN3NFC13showMoreGamesEv   head: ...7c041600 03031600   staged: ...8c041600 13031600
                                  ^ +0x10                ^ +0x10
```

Measured: 473 functions change bytes, `byte_exact 4499 -> 4040`, `RATCHET FAIL`.
**Any Hud::draw source change that grows `.ARM.extab` by even 1 byte is fatal**,
because `.ARM.extab` already ends exactly on `.rodata`'s aligned start.
(Shrinking is free up to 8 bytes; growing is not.)

---

## 4. The dock-block reorder: 89.1 -> 90.7, but +12 bytes of LSDA

Original call order in the docking-transfer block (0x165880..0x1659c0):

```
frameH      = GetImage2DHeight(image_0x37c)     -> [sp,#96]
textH       = GetTextHeight(font)               -> [sp,#92] / r9
transferred = getDockTransferedAmount()         -> r4
total       = getDockTotalAmount()              -> r5
getText(3205/3204)                              -> r6
String(" ")                                     -> [sp,#196]
progress    = transferred / total               (vdiv s0, s0, s2)
barOffset   = (int)(textH + frameH*2.5)         -> s16
operator+   (*getText + String(" "))            -> [sp,#208]
~String([sp,#196])
barW        = GetImage2DWidth(image_0x37c)      -> r8
barH        = GetImage2DHeight(image_0x37c)     -> fp
field_0x468 += dt; fadeIn; fade
```

HEAD calls `GetImage2DWidth`/`GetImage2DHeight` **before** `getText`. The one-line
reorder

```cpp
            barOffset = (int) ((float) textH + (float) frameH * 2.5f);
+           String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            this->field_0x468 += dt;
            float fadeIn = (float) this->field_0x468 / 1000.0f;
            float fade = 1.0f;
-           String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
            if (fadeIn < 1.0f)
                fade = fadeIn;
```

gives **89.1 -> 90.7** at 9332 B, frame 224, same 3219-instruction length — but
`.ARM.extab 0x26298 -> 0x262a4` and therefore `byte_exact 4499 -> 4040`.
It is *not* landable as-is.

### 4.1 Full permutation sweep of the dock block

All permutations of `{T=getText, P=progress, B=barOffset, W=barW, H=barH, D=dt/fade}`
after the `frameH/textH/transferred/total` prologue were built and measured
(120 permutations, plus the 20 that keep `T` before `W`/`H` and `P` before `B`).
Every single one that moves `T` earlier grows `.ARM.extab`:

| order | size | `.ARM.extab` | fuzzy |
|---|---|---|---|
| HEAD (`B W H D T`) | 9332 | **156312 (0x26298)** | 89.1 |
| `P B T W H D` (staged) | 9332 | 156324 | **90.7** |
| `P B T H W D` | 9332 | 156324 | 90.5 |
| `P T W B H D` | 9328 | 156324 | 90.4 |
| `T W P B H D` | 9328 | 156324 | 90.2 |
| `T P W B H D` | 9328 | 156324 | 90.1 |
| `P T B W H D` | 9332 | 156324 | 89.9 |
| `T P B W H D` (closest to the original call order) | 9332 | 156324 | 89.8 |
| `B W H T D` / `W H B T D` / ... | 9340 | **156312** | 85.3 |
| `B W D H T` / `W H D T B` / ... | 9344 | **156312** | 84.7 |

Only the permutations that push `T` to the end keep `.ARM.extab` at 0x26298, and
those are all worse than HEAD. `String text` construction forms that avoid the
temporary (`String(const String&, bool)`, `text = text + String(" ")`,
`String const&`/`String const*` hoisting of the `getText` result) were also tried:
they either grow the size (9336/9352/9360) or keep 156312 with a lower score
(85.0).

### 4.2 Other shapes tried (all at 9332 B unless noted)

| variant | `.ARM.extab` | fuzzy |
|---|---|---|
| HEAD | 156312 | 89.1 |
| `Status *st` used in the mission panel instead of `Globals::status` | 156312 | 59.9 (size 9340) |
| `st` used in the damage guard (`st == 0 \|\| ...`) | 156312 | 63.3 (size 9360) |
| damage guard removed (`if (this->field_0x4b8 >= 1)`) | 156312 | 82.0 (size 9264) |
| damage guard `Globals::status != 0 && ...` | 156312 | 88.5 (size 9336) |
| `st` declared at the top of the function | 156312 | 66.1 |
| `gammaY`/`gammaBarY` declaration swap | 156312 | 89.1 (no code change) |
| `sideImage`/`frontImage` declaration swap | 156312 | 88.9 |
| `int` vs `unsigned int hitFlags`, `(x & 1) != 0` | 156312 | 89.1 (no code change) |

---

## 5. What remains for `Hud::draw`

1. **Gamma store placement** — irreducible at 9332 B with the `Status *st` local:
   the local is required for the 224-byte frame and the shield `this` spill, but it
   necessarily caches `Globals::status` and spills `&Globals::status` early. A shape
   that keeps the clean 3x-`Globals::status` reload *and* the 224-byte frame has not
   been found (clean gamma = 9312 B / 74.2 %).
2. **The two dead `ldr.w r8, [sp, #108]`** (0x164176, 0x164210) — 4 bytes each; they
   are the source of the -4/-8 address drift through the damage block and therefore
   of most branch-target-only diffs.
3. **Branch offsets shifted by 2/4/8** — downstream of (1) and (2).
4. **Mission-panel `fp`/`r9` swaps** (0x16418a region: `ldrh.w fp,[r6,#1098]` vs
   `ldrh.w r9,[r6,#1098]`, `sub.w r3,r9,fp` vs `sub.w r3,fp,r9`) and the missing
   `mov sl, r8` restore.
5. **Docking progress computation** — `vdiv s0,s2,s0` operand order, `s24` vs `s16`,
   `vmin` vs `vcmpe/it mi`.
6. **Prologue**: ours is `vpush {d8-d12}`, the original `vpush {d8-d11}` (one extra
   callee-saved VFP register); frame is 224 in both.
7. **Any landing change must keep `.ARM.extab <= 0x26298`** (see §3). A change that
   adds call-site-table entries must be paired with one that removes at least as
   many, in the same commit.

---

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
cmake --build cmake-build-match --target drift -j8      # 0 drifted fields

# official per-function score
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj

# section sizes (the .ARM.extab / .rodata coupling check)
arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so | grep -E 'ARM.extab|\.rodata'
```

`FN=<mangled> cmake --build ... --target verify-fn` is broken in this tree (the
`FN` variable is not exported to the script); call `verify.py --show` directly.

Scratch used during this pass lived in `/dev/shm/w4273` (variant builders
`sweep.py`, `perm*.py`, `micro*.py`, LSDA/extab dumpers `extab2.py`, `lsda3.py`,
and the address-drift tool `addr2.py`).
