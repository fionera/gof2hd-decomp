# MGame::OnTouchEnd — wave 146 (w9210): the hudTouchFlags join is the `else` of the weaponSlot/wingman chain

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9210`,
base `31297fadc` (wave 145 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9210`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 145) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4180 | **4127** |
| `.text` diff bytes | 85,651 | **85,598** |

## 1. The original join block, decoded

Original `0x17afd8..0x17b04e` (the `hudTouchFlags` join and the tail, with the 0x400 body
immediately in front of it):

```
17afd8: e9db 021d  ldrd  r0, r2, [fp, #116]   ; this->hud, this->level
17afdc: 2102       movs  r1, #2
17afde: f6ed e9ce  blx   Hud::initHudMenu(2, level)   ; <-- shared with the 0x200 body
17afe2: f8db 00f8  ldr.w r0, [fp, #248]       ; hudTouchFlags
17afe6: b1c0       cbz   r0, 17b01a           ; if (flags == 0) goto B   <-- T is fallthrough
17afe8: f8da 0000  ldr.w r0, [sl]             ; T: this->player
17afec: f6eb ec16  blx   PlayerEgo::isAutoPilot()
17aff0: f080 0001  eor.w r0, r0, #1
17aff4: ea50 0008  orrs.w r0, r0, r8          ; r8 = wasAutoPilot
17aff8: bf02       ittt  eq
17affa: 48ef       ldreq r0, [pc, #956]       ; 100.0f
17affc: f04f 517e  moveq.w r1, #1065353216    ; 1.0f
17b000: e9cb 106c  strdeq r1, r0, [fp, #432] ; field_0x1b0 = 1.0f; field_0x1b4 = 100.0f
17b004: 9832       ldr   r0, [sp, #200]       ; stack canary
17b006: f8d9 1000  ldr.w r1, [r9]
17b00a: 1a08       subs  r0, r1, r0
17b00c: bf02       ittt  eq
17b00e: b033       addeq sp, #204
17b010: e8bd 0f00  ldmiaeq.w sp!, {r8,r9,sl,fp}
17b014: bdf0       popeq {r4,r5,r6,r7,pc}
17b016: f6e3 ec06  blx   __stack_chk_fail
17b01a: f8db 0074  ldr.w r0, [fp, #116]      ; B: this->hud
17b01e: f6ec ecf2  blx   Hud::closeHudMenu()
17b022: 2400       movs  r4, #0
17b024: 4658       mov   r0, fp
17b026: f88b 40d6  strb.w r4, [fp, #214]     ; hudMenuOpen = 0
17b02a: e9cb 4426  strd  r4, r4, [fp, #152]  ; touch0Id = nullptr; touch1Id = nullptr
17b02e: f88b 405d  strb.w r4, [fp, #93]      ; pauseOpen = 0
17b032: f6ed e92c  blx   MGame::resumeSounds()
17b036: f89b 00c8  ldrb.w r0, [fp, #200]     ; _bc8
17b03a: 2800       cmp   r0, #0
17b03c: d0d4       beq.n 17afe8               ; if (!_bc8) goto T
17b03e: f8db 0058  ldr.w r0, [fp, #88]       ; this->player
17b042: f88b 40c8  strb.w r4, [fp, #200]     ; _bc8 = 0
17b046: 2800       cmp   r0, #0
17b048: d0ce       beq.n 17afe8               ; if (player == nullptr) goto T
17b04a: f6e7 ee2a  blx   PlayerEgo::resetGunDelay()
17b04e: e7cb       b.n   17afe8               ; goto T
```

Two things are witnessed here that the base did not reproduce:

1. **The join test is the `else` of the `weaponSlot`/`wingmanCommand` chain.** The
   weaponSlot body (`0x17b9cc..0x17ba24`) ends with `b.w 17afe8` — it jumps straight to
   T and **skips** the join test. The wingman body (`0x17bee2..0x17bf4a`) ends with
   `b.w 17afe2` — it goes **through** the join test. The flag-chain merge
   (`0x17ad9c bpl.w 17afe2`, `0x17ada6 b.n 17afe2`) also goes through the join test.
   That is exactly the CFG of `if (weaponSlot >= 0) A; else if (wingmanCommand >= 0) Bw;
   else if (hudTouchFlags == 0) B;` — the join test is the final `else if`, not a
   separate `if` after the chain.
2. **The 0x200 and 0x400 bodies share one `blx initHudMenu`**, and the shared block is
   owned by the **0x400 body** (`0x17afde`), with the 0x200 body branching forward to it
   (`0x17adc0: ... b.n 17afde`). The join test is the fallthrough of that shared `blx`.

## 2. The landed change

```cpp
-        }
-        if (this->hudTouchFlags == 0) {
+        } else if (this->hudTouchFlags == 0) {
```

Semantically identical: `hudTouchFlags == 0` implies `weaponSlot == -1` and
`wingmanCommand == -1` (both are set only from flag bits), so the join body runs exactly
when the previous chain did nothing. The change is a faithful source-shape change (it
reproduces the original's A -> T edge), not a metric-only reorder.

Effect: the join test moves from `0x17b9c0` (after the weaponSlot body, where the base
had it) to `0x17aeb2` (right after the 0x800 `useCloak` body). The weaponSlot body now
falls through to T, matching the original's `b.w 17afe8`. Raw **4180 -> 4127** (-53),
`.text` **85,651 -> 85,598** (-53). Size stays 8080.

## 3. What is still wrong (honest assessment)

The join block is now in the right *shape* but not at the right *address*:

| | original | landed |
|---|---|---|
| join test | `0x17afe2` (`ldr; cbz B`, T fallthrough) | `0x17aeb2` (`ldr; cmp #0; bne.w T`, B fallthrough) |
| T (tail) | `0x17afe8` (right after J) | `0x17b9fc` (after the weaponSlot body) |
| B (join body) | `0x17b01a` (after T) | `0x17aebc` (right after J) |

The 0x5c gap is therefore **not** closed. The residual is a block-placement difference,
not a source-shape difference: in the original the shared `initHudMenu` block is owned by
the 0x400 body and is followed by `J -> T -> B`; in the landed build the shared block is
owned by the 0x200 body and `J` follows the 0x800 body, with `B` as its fallthrough.
The campaign region therefore still sits at `+0x12` (was `-0x26` in the base), and the
raw diff is dominated by that shift.

## 4. Variants measured this wave (all 8080 B unless noted)

| variant | raw | note |
|---|---:|---|
| base (separate `if`) | 4180 | reference |
| **`else if` join (landed)** | **4127** | best |
| `else if` + branch order 0x200,0x400,0x800 | 4132 | shared blx moves to 0x400 body; J follows it, but B is still fallthrough |
| `else if` + branch order 0x400,0x200,0x800 | 4134 | |
| `else if` + branch order 0x400,0x800,0x200 | 4130 | |
| `else if` + `!flags` / `<=0` / `<1` / `==false` / `==0u` | 4127 | all canonicalise to the same IR |
| `else if` + `weaponSlot != -1` / `wingmanCommand != -1` | 4127 | same |
| `else if` + wingman block before weaponSlot | 4127 | same |
| `else if` + `__builtin_expect(flags==0,1)` / `(flags!=0,1)` | 4127 | same |
| `else if` + `__builtin_expect(flags==0,0)` | 5716 @ 8068 B | size regression |
| `else if` + 0x200,0x400,0x800 + `expect(flags==0,0)` | 6082 @ 8064 B | size regression |
| join block moved before the chain (separate `if`) | 5588 @ 8060 B | size regression |
| base + branch order 0x200,0x400,0x800 | 5889 @ 8052 B | size regression |

## 5. Open questions

1. The shared `initHudMenu` block must be owned by the 0x400 body (original `0x17afde`)
   and the join test must be its fallthrough. The branch order 0x200,0x400,0x800 gets the
   ownership right but still lays `B` as the join-test fallthrough. A shape that also
   makes T the fallthrough (so the test is `cbz B`) is the remaining lever.
2. The mission-selection chain is `+0x6c` relative to the original in the landed build,
   while the campaign region is `+0x12`; the blocks between them (0x400 body + JTB +
   freeCamMode + mining text + type12 chain) are ~0x5a B shorter in ours. Matching the
   original's cold-block worklist order is the only way to close the remaining shift.

## 6. Reproduction

```
cp /dev/shm/w9210/MGame.elseif.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9210/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4127 / 8080
python3 /dev/shm/w9210/seqdiff3.py /dev/shm/w9210/orig.txt /dev/shm/w9210/ours.txt
```

Scratch: `/dev/shm/w9210/{fdiff.py,windows.py,seqdiff.py,seqdiff2.py,seqdiff3.py,
gen.py,gen2.py,gen3.py,gen4.py,measure.sh,MGame.base.cpp,MGame.elseif.cpp,
MGame.base.ll,MGame.elseif.ll,ote.base.ll,ote.elseif.ll,orig.txt,ours.txt}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
