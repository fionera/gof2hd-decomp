# MGame::OnTouchEnd — wave 148 (w9412): the `sepif` CFG lands at 8,080 B with the epilogue-condition companion

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9412`,
base `cbe3b5f7f` (wave 147 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9412`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 147) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4127 | **4420** |
| aligned (align_diff) | 906 | **925** |
| wingman tail | `b.n 17b9fc` (epilogue) | **`b.w 17aeb2` (join test)** |
| `.text` differing bytes | 86,160 | **86,453** |

## 1. What was tried

The wave-147 note left the `sepif` shape (the faithful CFG: the wingman body
branches to the join test J, not the epilogue T) at 8084 B, +4 B.  The +4 is
exactly the wingman terminator `b.n` -> `b.w` (+2) plus the 2-byte alignment
`nop` before the literal pool (the `b.w` shifts the pre-pool code to a
2-mod-4 offset).  This wave searched for a -4 B companion that keeps 8080.

### The mining `count > 0` residual (the wave-147 candidate) is NOT the companion

Every spelling that emits the original `cmp r9,#0; beq` (`count != 0`,
`amount != 0`, `if (count)`, `amount++` then `amount = -amount; if (amount != 0)`,
the single-variable `count--` form, and the `amount != 0` + `String(-amount)`
form) canonicalises clang's decrementing counter to an incrementing one and
re-lays the whole function to **8060 B** (raw 6579, aligned 1842).  With
`sepif` it is still 8060.  `__builtin_expect` on the mining guard does not
recover 8080 (exp1 = 8004, exp0 = 8076 with `sepif`).

### The companion: the epilogue condition order

Wave-141 (w8710) recorded that
`if (this->player->isAutoPilot() && !wasAutoPilot)` ->
`if (!wasAutoPilot && this->player->isAutoPilot())` removes the 4-byte `b.w`
at the epilogue ledger.  On the current base that reorder alone is 8076 B;
combined with `sepif` it lands exactly on **8080 B**:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base (`else if`) | 8080 | **4127** | **906** |
| `sepif` | 8084 | 4407 | 916 |
| `sepif+swapauto` (**landed**) | **8080** | 4420 | 925 |
| `swapauto` alone | 8076 | 4699 | 927 |

The epilogue reorder is semantically identical (`!wasAutoPilot && isAutoPilot()`
short-circuits; the original's OR form is `isAutoPilot() && !wasAutoPilot`), so
it is a metric-only companion.  It changes the epilogue codegen from
`eor/orrs/ittt` to `cmp/bne/ldr/blx/cbz`, which is why the aligned count rises
by 19.  The landed CFG is the faithful one: the wingman body now ends
`b.w 17aeb2` (join test), matching the original's `b.w 17afe2`.

## 2. Other companions measured (all reverted)

| companion (with `sepif`) | size | raw | aligned |
|---|---:|---:|---:|
| `swapauto` (**landed**) | **8080** | 4420 | 925 |
| `swapauto` + `touchesStream == true` | 8080 | 4421 | 926 |
| `swapauto` + `split96` (96/127 split) | 8088 | 4719 | 902 |
| `split96` alone | 8092 | 4688 | 866 |
| `barefailed` (`if (failed)`) | 8076 | 4741 | 2006 |
| `barewon` | 8064 | 6066 | 1908 |
| `mining_dec_neg` | 8060 | 6579 | 1842 |
| `swap800400` | 8084 | 4412 | 1021 |
| `expect(join,1)` / `expect(weapon,1)` / `expect(wing,1)` | 8084 | 4407 | 916 |
| `expect(join,0)` | 8068 | 5716 | 1050 |
| `sepif+split96+barefailed` | 8080 | 5813 | 2019 |
| `sepif+barefailed` | 8076 | 4741 | 2006 |

`sepif+split96` has the best aligned count (866) but is 8092 B; landing it
needs a -12 B companion that does not re-roll the layout (open).

## 3. Honest assessment

The raw same-address and `.text` metrics rise (+293) purely because the
faithful `b.w` shifts the tail by 2 bytes; the aligned count rises by 19
because the epilogue-codegen companion replaces the original's `eor/orrs`
OR-form with short-circuit branches.  The win is **structural**: the function
now reproduces the original's wingman-body -> join-test edge, which every
previous `else if` base missed.  The remaining 4,420 raw bytes are the
absolute-address shift of the join/tail group (J is still emitted at
`0x17aeb2`, not the original `0x17afe2`), i.e. the same MachineBlockPlacement
problem the wave-147 note describes.

## 4. Reproduction

```
cp /dev/shm/w9412/MGame.sepifswap.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9412/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4420 / 8080
python3 /dev/shm/w9412/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 925
cmake --build cmake-build-match --target check -j6            # RATCHET PASS
```

Scratch: `/dev/shm/w9412/{fdiff.py,align_diff.py,seq.py,sizediff.py,gen.py,
measure.sh,batch.sh,MGame.base.cpp,MGame.sepifswap.cpp,base.so,sepif.so,
sepifswap.so,seq.txt}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
