# w8912 (wave-144): `Level::createMission` — why the alien-gun table select is a
# `cbz` branch (ARM `-Oz` if-conversion heuristic), and the `volatile` probe

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8912`, based on master
`f5093474f` (wave 143). Continues `level-createmission-wave143-w8812.md`,
`…-wave143-w8811.md`, `…-wave142-w8712.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w8912/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5597
region b2e04..b329a (station loop + alien guard)  1103 / 1174 differing
region b329a..b3394 (alien-gun loop)               233 /  250 differing
region b3394..b34e9 (pirate-fortress loop)         315 /  341 differing
region b34e9..b3752 (literal pool + raid join)       3 /  617 differing
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

The alien-gun loop is 233/250 bytes different at the same address. It is **not**
a pure register-map problem: the original's loop is 250 B (`b329a..b3394`) while
HEAD's is 264 B (`b328a..b3392`), so the 16-byte phase shift (rooted in the
`bool`-vs-`int` alien guard and the `alienGuns` low-register load, see w8811)
makes the whole region compare at shifted addresses.

## NEW result — the `cbz` select is an ARM `-Oz` if-conversion heuristic, not a
## source-level `select`

The original's in-loop table select is a **real branch**:

```text
b32c6: ldr r0, [sp, #64]      ; bigAlienWave (word load, in-loop)
b32c8: cbz r0, b32d0
b32ca: ldr r1, [pc, #732]     ; kAlienStationTurrets
b32cc: add r1, pc
b32ce: b  b32d4
b32d0: ldr r1, [pc, #728]     ; kAlienOutpostTurrets
b32d2: add r1, pc
b32d4: add.w r0, r1, r9       ; t + k*24
b32d8: mov sl, r1             ; sl = t
```

Both arms materialise the array address **in-loop** (`ldr [pc]; add pc`), and
the join keeps `t` in a register (`mov sl, r1`), with **no store/reload of `t`**.

`clang -Oz` normally lowers a pointer `select` to `ARMISD::CMOV` →
`it ne; movne` (`LowerSELECT` → `LowerSELECT_CC` → `getCMOV`, LLVM 7
`ARMISelLowering.cpp:4106/4433/4224`), and `SimplifyCFG::FoldTwoEntryPHINode`
folds a simple pointer diamond to a `select` first. That is exactly HEAD's
`(bigAlienWave + (i<0))` lowering.

The original's branch survives because of the **`optForSize` if-conversion
heuristic** in `ARMBaseInstrInfo::isProfitableToIfCvt` (LLVM 7
`ARMBaseInstrInfo.cpp:1878`):

```cpp
if (MBB.getParent()->getFunction().optForSize()) {
  ...
  if (LastMI->getOpcode() == ARM::t2Bcc) {
    --CmpMI;
    if (CmpMI->getOpcode() == ARM::tCMPi8 || CmpMI->getOpcode() == ARM::t2CMPri) {
      unsigned Reg = CmpMI->getOperand(0).getReg();
      ARMCC::CondCodes P = getInstrPredicate(*CmpMI, PredReg);
      if (P == ARMCC::AL && CmpMI->getOperand(1).getImm() == 0 &&
          isARMLowRegister(Reg))
        return false;              // don't if-convert: let it become cbn?z
    }
  }
}
```

So a machine diamond whose predecessor ends in `cmp <lowreg>, #0; bcc` is left
alone at `-Oz`; the constant-island pass then rewrites the `bcc` to `cbz`. That
is precisely `ldr r0,[sp,#64]; cbz`. Any source shape that reaches the backend
as an IR `select` (CMOV) can *never* produce this, and any shape whose condition
is `(bigAlienWave + (i<0))` produces `cmp.w r1, r9, asr #31` (not `cmp #0`), so
the heuristic does not fire.

## Probe: a `volatile` table pointer reproduces the branch exactly

A standalone reproducer (same flags) shows the only source shape found that
emits the branch is a **non-foldable diamond** — i.e. the arms must not be
speculatable. A `volatile` table pointer does it:

```cpp
const float (*volatile t)[6];
if (bigAlienWave) t = kAlienStationTurrets; else t = kAlienOutpostTurrets;
```

gives, in-loop:

```text
ldr r0,[sp,#36]
cbz r0, ...
ldr r0,[pc]; add r0,pc     ; station address, materialised in-arm
b join
ldr r0,[pc]; add r0,pc     ; outpost address, materialised in-arm
join: str r0,[sp,#32]      ; <-- volatile store
      ldr r0,[sp,#32]      ; <-- volatile reload
```

The branch and in-arm address materialisation match the original, but the
`volatile` store/reload (+8 B) does not exist in the original (which keeps `t`
in `sl`). In the real function (`sel=ifelse, order=faithful, tvol=1`) the loop
region diff is 234/250 (vs. 233/250 for HEAD) and the function is 17,180 B, so
the +8 B shifts the whole tail (region `b34e9..b3752` goes 3 → 597 differing)
and the raw diff collapses to 11,612.

## Measured variants (all faithful; no call/store/RNG-order change)

`sel` = table-select spelling, `order` = loop statement order (`head` = HEAD's
`setScaling`-before-`setRotation` + `(*enemies)[i]` reload; `faithful` =
cached `gun`, `setScaling` after `setPosition`, `cargo` then `shipGroup`),
`vol` = `volatile int bigAlienWave`, `tvol` = `const float (*volatile t)[6]`.

| variant | size | raw diff |
|---|---:|---:|
| HEAD (`anchor`, `head`) | **17172** | **5597** |
| `anchor`, `head`, `tvol` (anchor ignores `tvol`) | 17172 | 5597 |
| `ifelse`, `head` | 17144 | 6549 |
| `ifelse`, `head`, `tvol` | 17228 | 15628 |
| `ternary`, `head`, `tvol` | 17188 | 6579 |
| `ifelse`, `faithful` | 17136 | 12970 |
| `ifelse`, `faithful`, `vol` | 17136 | 12970 |
| `ifelse`, `faithful`, `vol`, `tvol` | 17176 | 14921 |
| `ifelse`, `faithful`, `tvol` | 17180 | 11612 |
| `ifelse`, `faithful2` (`shipGroup` then `cargo`), `tvol` | 17180 | 11612 |
| `ifelse`, `faithful`, `tvol`, `bool bigAlienWave` | 17164 | 15150 |
| `ternary`, `faithful`, `vol` | 17136 | 12970 |
| `ternary`, `faithful`, `vol`, `tvol` | 17144 | 13003 |
| `anchor`, `faithful`, `tvol` | 17184 | 6460 |

No variant keeps 17,172 B while reproducing the branch, and every branch-
producing variant is worse on the raw metric because of the tail shift.

## Conclusion / open question for the next wave

The original is a **non-foldable pointer diamond** whose `cmp <lowreg>,#0; bcc`
is left un-if-converted at `-Oz` and becomes `cbz`. Clang 7.0.2 only produces
that from C++ if the diamond reaches the backend as control flow (not a
`select`). A `volatile` pointer does it but adds a store/reload the original
does not have. The remaining question is what makes the original's diamond
non-speculatable *without* a volatile store: candidates not yet tried are a
diamond with **three** PHIs in the merge block (`FoldTwoEntryPHINode` bails at
`NumPhis > 2`), or a `switch` whose lowering is not folded by SimplifyCFG.
Both would need to keep the join value in a register (`mov sl, r1`).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8912/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8912/regiondiff.py _ZN5Level13createMissionEv 0xb329a 0xb3394
python3 /dev/shm/w8912/apply2.py "sel=ifelse,order=faithful,tvol=1"
```

Scratch: `/dev/shm/w8912/{Level.base.cpp,fdiff.py,rundiff.py,regiondiff.py,
apply2.py}`, `/dev/shm/w8912/t/{gen.py,gen2.py,x.cpp}`.
LLVM 7 sources fetched for the analysis: `/dev/shm/w8912/{IfConversion.cpp,
ARMBaseInstrInfo.cpp,ARMISelLowering.cpp,SimplifyCFG.cpp}`.

## Gate counts (before -> after)

No source change landed; gate re-run is identical.

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv raw diff 5597 -> 5597
```
