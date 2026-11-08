# MGame::OnTouchEnd — wave 147 (w9312): the join body's CFG successor, and the cold-block order after B

Status: analysis only, no source change landed (baseline kept). 2026-11-08.
Worktree `/home/debian/GalaxyOnFire2/wt/w9312`, base `9c9744ca5` (wave 146 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w9312`.
Full `check` green before and after: byte **4507/4524**, linked **4507/4524**,
size **4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 146) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4127 | **4127** |
| aligned (align_diff) | 906 | **906** |

## 1. What I decoded in the original `0x17b04e..0x17b0b0`

This region is a **cold-block cluster**, not a single join. In address order it is:

| orig | block | source construct |
|---|---|---|
| `0x17b04e` | `b.n 17afe8` | end of the `hudTouchFlags == 0` join body **B** |
| `0x17b050` | `ldrb [fp,#350]; bne 17b858; b.n 17b814` | menuTouchOpen tail, `if (!freeCamMode && inCinematicMode())` test **(5)** |
| `0x17b05c` | `getText(292)` + `ChoiceWindow::set` + `String::~String`; `b.w 17a6e4` | mining branch, "no ore refined" `choiceWindow->set(getText(292))` |
| `0x17b07e` | `getMission()->getType()`; `cmp r5,#12`; `bne 17b374`; `changeCredits(-reward)` | failed-mission type-12 chain |
| `0x17b0aa` | `levelScript->resetStartSequenceOver(); b.w 17b8e2` | `else { resetStartSequenceOver(); }` of `if (won)` |
| `0x17b0b6` | `setFreelanceMission(empty)`; `showMissionRewardMessage(reward+bonus,false)` | `if (won)` non-campaign branch |

The same blocks exist in ours but in a **different order** and at different
addresses:

| block | orig | ours | delta |
|---|---|---|---|
| freeCamMode test (5) | `0x17b050` | `0x17b04e` | -2 |
| mining `getText(292)` | `0x17b05c` | `0x17b02c` | -0x30 |
| type-12 chain | `0x17b07e` | `0x17b05a` | -0x24 |
| `resetStartSequenceOver` | `0x17b0aa` | `0x17b0b8` | +0xe |
| `setFreelanceMission`/reward | `0x17b0b6` | `0x17b0d0` | +0x1a |

So the residual in this window is **two independent things**:

1. **The cold-block worklist order is swapped**: the original lays the
   menuTouchOpen tail (`freeCamMode`) *before* the mining `getText(292)` block;
   ours lays `getText(292)` first. Source order is `getText(292)` (pauseOpen
   block, early) before `freeCamMode` (menuTouchOpen block, late), so ours is
   source order and the original is not. This is a MachineBlockPlacement
   worklist difference, not a source-shape difference.
2. **The join body B's CFG successor differs** (see §2).

## 2. The real structural find: the wingman body branches to J, not T

The original's `else if (wingmanCommand >= 0)` body ends at `0x17bf4a` with
`b.w 17afe2` — i.e. it branches to the **join test J**, not to the epilogue T
(`0x17afe8`). The `weaponSlot >= 0` body ends at `0x17ba24` with `b.w 17afe8`
— it branches straight to **T**. The flag-chain merge (`0x17ad9c bpl.w 17afe2`,
`0x17ada6 b.n 17afe2`) also goes to J.

Our base does the opposite: the wingman body ends `b.n 17b9fc` (**T**), and the
weaponSlot body falls through into T (`0x17b9fc`).

This asymmetry is exactly the CFG of:

```cpp
if (weaponSlot >= 0) {
    A
} else {
    if (wingmanCommand >= 0) {
        Bw
    }
    if (this->hudTouchFlags == 0) {   // separate if, NOT else-if
        B
    }
}
```

- A -> merge (T)  ✓
- Bw -> J (the separate `if` after the wingman `if`)  ✓
- neither -> J  ✓

The current `else if` chain gives `Bw -> merge (T)` for all bodies, which is
what our base emits. The `else if` form is therefore *not* the original's
source shape; the join test is a **separate `if` inside the `else` of the
weaponSlot test**. (This is the "next join block" the wave-146 note's open
question #1 was pointing at.)

### Measured variant

`sepif` = the shape above, everything else unchanged:

| variant | size | raw | wingman body terminator |
|---|---:|---:|---|
| base (`else if`) | 8080 | **4127** | `b.n 17b9fc` (T) |
| `sepif` (separate `if` in `else`) | **8084** | 4407 | `b.w 17aeb2` (J) ✓ |
| `sepif` + `else if` inner (`sepif_elif`) | 8080 | 4127 | `b.n 17b9fc` (T) |

`sepif` reproduces the original's `Bw -> J` edge but costs **+4 B** (8084):
the wingman terminator grows `b.n`->`b.w` (+2) and the exception-cleanup tail
grows another +2 (extra instruction at `0x17c096` in the landing-pad region).
That is a size-exact regression, so it cannot be committed under the gate.

The `else if`-inside-`else` spelling (`sepif_elif`, 8080/4127) is byte-identical
to the base — the outer `else` block is folded away, so only the *separate if*
spelling changes the CFG.

### The 0x5c cluster

The "0x5c gap" is the size of the J/T/B cluster plus the wingman terminator:

| | J | T | B | wingman term | total |
|---|---:|---:|---:|---:|---:|
| original | 6 | 0x1c | 0x34 (short `beq.n`/`b.n` to T) | `b.w` 4 | **0x5a** |
| ours (base) | 0xa (`cmp`+`bne.w`) | 0x1c | 0x38 (long `beq.w`/`b.w` to T) | `b.n` 2 | 0x62 |

The original's B is 4 B smaller because T is adjacent (short branches); its
wingman terminator is 2 B larger because J is far. The two effects nearly
cancel, but the *addresses* are completely different, which is what the raw
diff measures. This is why the `sepif` shape (which reproduces `Bw -> J`) still
lands at 8084: it pays the wingman `b.w` without getting the adjacent-T saving.

## 3. Why T is not the join test's fallthrough in ours

In the original, J is `ldr; cbz r0, B` — B is the branch target and **T is the
fallthrough**. In ours J is `ldr; cmp #0; bne.w T` — T is the branch target and
**B is the fallthrough**. T sits at `0x17afe8` in the original (right after J)
but at `0x17b9fc` in ours (right after the weaponSlot body, which falls into it).

MachineBlockPlacement claims T in the weaponSlot body's chain first because the
weaponSlot body precedes J in the IR block order; the original's build claimed T
in J's chain instead. This is the same class of layout difference as the w9214
`!prof` finding: the original was built with branch weights, so the backend
honours IR/chain order rather than the static zero-heuristic. `__builtin_expect`
on the join test alone overshoots (measured `exp_join0` = 5716 @ 8068,
`exp_join1` = 4127 @ 8080; `exp_menuopen1` = 5926 @ 8068,
`exp_menuopen0` = 6183 @ 8044; `exp_weapon1/0` = 4127 @ 8080;
`exp_wing1` = 4127 @ 8080).

## 4. Variants measured this wave (all 8080 B unless noted)

| variant | raw | note |
|---|---:|---|
| base | **4127** | reference |
| `sepif` (separate `if` in `else`) | 4407 | **8084 B**, size regression |
| `sepif_elif` | 4127 | identical to base |
| `!this->hudTouchFlags` | 4127 | canonicalises |
| `this->hudTouchFlags < 1` | 4127 | canonicalises |
| `!(this->hudTouchFlags > 0)` | 4127 | canonicalises |
| `this->hudMenuOpen != 0` | 4127 | canonicalises |
| chain order `0x200,0x400,0x800` | 4132 | w9210's variant, still worse |
| `bool failed; if (failed)` (faithful) | 4741 | **8076 B**, size regression |
| `sepif` + faithful `failed` | 4741 | **8076 B** |
| `count != 0` / `amount != 0` / `if (count)` | 6579 | **8060 B**, size regression |
| `count >= 1` / `amount < 0` | 4127 | canonicalises |
| `__builtin_expect(hudMenuOpen,1/0)` | 5926/6183 | 8068/8044 B |
| `__builtin_expect(weaponSlot>=0,1/0)` | 4127 | same |
| `__builtin_expect(wingmanCommand>=0,1)` | 4127 | same |
| `__builtin_expect(hudTouchFlags==0,1/0)` | 4127/5716 | 8080/8068 B |

## 5. Open questions

1. The `sepif` shape is the faithful CFG but is +4 B. A companion source shape
   that saves 4 B in the same function (without disturbing the layout) would
   let it land. Candidate: the `0x17a5c4` `count > 0` residual — the original
   uses `cmp r9,#0; beq` (`amount == 0`) while ours uses
   `cmp r9,#0xffffffff; bgt` (`amount >= 0`). Both are valid for
   `count = -amount >= 0`; `count != 0` / `amount != 0` / `if (count)` all
   compile to the original's `cmp #0` but re-lay the function to 8060 B.
2. The cold-block order `[freeCamMode][getText292]` (original) vs
   `[getText292][freeCamMode]` (ours) is a MachineBlockPlacement worklist
   difference. It needs the same `!prof`/branch-weight lever as w9214; no
   source spelling tried here changes it.

## 6. Reproduction

```
cp /dev/shm/w9312/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9312/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4127 / 8080
python3 _work/reconstructions/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 906
python3 /dev/shm/w9312/gen.py sepif                          # writes /dev/shm/w9312/var.cpp
/dev/shm/w9312/measure.sh                                    # 4407 / 8084
```

Scratch: `/dev/shm/w9312/{fdiff.py,windows.py,structdiff.py,shift.py,shift2.py,
blocks.py,gen.py,gen2.py,gen3.py,gen4.py,gen5.py,measure.sh,MGame.base.cpp,
var.cpp,orig.txt,ours.txt,ours2.txt,struct.txt,MGame.ll,base.ll,ote.ll}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
