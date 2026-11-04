# wf314: `Level::createMission` (92.028388 %, 17172 B) — wave-103

Scope: worktree `/home/debian/GalaxyOnFire2/wt/wf314`, branch `wave103-wf314`,
base `1d2b4398e` (master). Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).
New oracle: `_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/`
(clang-3.8 -O2 mod rebuild; logic/source-shape oracle only).
**No source change is committed**: every variant measured below is neutral or a
regression. The deliverable is this note.

## Baseline and result (unchanged)

```text
_ZN5Level13createMissionEv   fuzzy 92.000000000 (raw 92.028388)  size 17172 B
                             orig 6055 insns / ours 6063 insns
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after.
`report.json` is not committed.

## Important measurement correction (wave-103)

The wave-98/102 harnesses wrote `Level.cpp` and then ran
`cmake --build ... --target link_so` **without checking the build's exit code**.
After a source write, the first `cmake --build` invocation in this worktree
sometimes returns `rc=1` (ninja reports a stop) while leaving the previous
`libgof2hdaa.so` in place; a second invocation succeeds. Any variant measured in
that window silently scored the *previous* binary. All numbers in this note were
re-measured with a harness that retries until `rc==0` before scoring. The
wave-98/102 headline numbers reproduce exactly under the robust harness, so
their landed conclusions stand; only the "neutral" rows needed re-checking.

Robust harness (scratch, not committed):

```python
def run(src, label):
    open(TARGET,'w').write(src); os.utime(TARGET,None)
    for _ in range(3):
        r = subprocess.run(['cmake','--build','cmake-build-match',
                            '--target','link_so','-j8'], cwd=REPO,
                           capture_output=True, text=True)
        if r.returncode == 0: break
    print(label, subprocess.run(['python3',SCORE],capture_output=True,text=True).stdout)
```

## Oracle findings (all verified against the original binary)

1. **Turret tables are identical.** `kAlienStationTurrets` is at `0x1fcd74`
   and `kAlienOutpostTurrets` at `0x1fce34` in *both* binaries (first words
   `0xc5d22f08` / `0x45a64dec` found at those exact addresses in both). The
   oracle's `game/Level.c` turret loop (lines ~3026-3115) indexes
   `DAT_00253544` / `DAT_00253604` with stride `0x18`, i.e. `t[k][0..5]`,
   matching our `const float (*t)[6]`. The handoff note "pool words at
   b35a8/b35ac are 0x149aa4/0x149b5e in both" is wrong about *where*: the
   original's PIC deltas live at `0xb35a8/0xb35ac` (`0x149aa4/0x149b5e`),
   ours at `0xb3590/0xb3594` (`0x149aae/0x149b6c`), because our selector
   instructions sit at different addresses. Both resolve to the same two
   tables (`0x1fcd74` / `0x1fce34`), and the verifier normalizes PIC deltas
   as `pool:pic:addr`, so this is not a data difference.
2. **Crate store order** (`case 9`) already matches the original
   (`str [r0,#80]` then `strb [r0,#76]`); landed in wave-102.
3. **case 4 tail** is a real 4-byte scheduling diff, not a data/constant diff.
   Original `b19c0..b19e8`:
   `ldr r0,[fp]; movs r1,#2; blx nextInt; cmp r0,#0; beq else; ldr r0,[fp];
   movs r1,#2; blx nextInt; adds r1,r0,#2; blx createRoute;
   ldr.w r8,[sp,#124]; str.w r0,[r8,#264]; b tail`.
   Ours `b19be..b19e6` hoists `ldr.w sl,[sp,#124]` to just after the first
   `nextInt` (before the `cmp`) and stores through `sl`, plus a 2-byte `nop`.
   Every source-shape variant tried (separate `Route *route` local,
   `if/else` instead of `goto`, `== 0` polarity swap, ternary) produced a
   byte-identical object, so this is backend scheduling, not source shape.

## Re-measured variant table (robust harness, official verifier normalisation)

`raw` is the unrounded `difflib` ratio; `pct` is the verifier's rounded row.

| variant | size | insns (orig/ours) | raw % |
|---|---:|---:|---:|
| **baseline (landed wave-102)** | **17172** | **6055/6063** | **92.028388** |
| selector `bigAlienWave ? A : B` (no anchor) | 17140 | 6055/6058 | 88.219269 |
| selector `(bigAlienWave + (i<0)) ? A : B` **(base)** | 17172 | 6055/6063 | 92.028388 |
| selector `if/else` with anchor | 17172 | 6055/6063 | 92.028388 |
| selector `(cond ? A[k] : B[k])` row pointer | 17156 | 6055/6057 | 83.603038 |
| duplicated body, plain `if (bigAlienWave)` | 17344 | 6055/6080 | 62.332097 |
| duplicated body, `bigAlienWave == 1` | 17296 | 6055/6065 | 62.590759 |
| duplicated body, `bigAlienWave + (i<0)` | 17296 | 6055/6064 | 62.595924 |
| `bool alienWave` | 17164 | 6055/6061 | 88.395510 |
| alienGuns named `Vector dir/pos` | 17164 | 6055/6059 | 88.657751 |
| alienGuns field-wise `dir.x=...` | 17156 | 6055/6057 | 88.507266 |
| alienGuns `float x,y,z` into `Vector{}` | 17172 | 6055/6063 | 92.028388 (neutral) |
| alienGuns `const float *u = t[k]` | 17180 | 6055/6064 | 90.766565 |
| alienGuns both vectors hoisted | 17148 | 6055/6057 | 90.719947 |
| alienGuns decl order `v,base,rot` | 17172 | 6055/6063 | 91.846839 |
| alienGuns decl order `base,v,rot` | 17172 | 6055/6063 | 91.632283 |
| alienGuns decl order `rot,base,v` | 17172 | 6055/6063 | 91.863344 |
| alienGuns decl order `k` first | 17172 | 6055/6063 | 92.028388 (neutral) |
| pirateFortress decl `v,base,rot` | 17192 | 6055/6072 | 90.756164 |
| pirateFortress decl `base,v,rot` | 17192 | 6055/6072 | 90.756164 |
| pirateFortress decl `k` first | 17172 | 6055/6063 | 92.028388 (neutral) |
| raid: cache `enemies` local | 17168 | 6055/6060 | 90.747008 |
| raid: cache `first`/`last` | 17176 | 6055/6065 | 87.409241 |
| raid: cache both | 17164 | 6055/6058 | 87.393709 |
| raid: loop index `i + field_168` | 17216 | 6055/6074 | 89.174705 |
| raid: loop index without field_168 | 17188 | 6055/6065 | 89.603960 |
| raid: `hard` before `raidType` | 17172 | 6055/6063 | 92.011883 |
| `int guns = alienGuns + fortressGuns` inlined | 17188 | 6055/6067 | 88.615740 |
| `unsigned guns` | 17172 | 6055/6063 | 92.028388 (neutral) |
| oracle alienGuns shape (`7; if(big) 8; if(!alien) 0`) | 17172 | 6055/6063 | 90.642020 |
| oracle alienGuns shape 2 | 17172 | 6055/6063 | 91.929361 |
| oracle alienGuns shape 3 (nested) | 17172 | 6055/6063 | 92.028388 (neutral) |
| `security == 3 && raid` (instead of `&`) | 17196 | 6055/6065 | 87.029703 |
| `raid && security == 3` | 17172 | 6055/6063 | 92.028388 (neutral) |
| hostileCount via local | 17156 | 6055/6060 | 87.891044 |
| friend loop `unsigned i` | 17172 | 6055/6063 | 91.995379 |
| friend loop `i != friendCount` | 17240 | 6055/6069 | 86.572088 |
| `if (i == 0 && hasWanted)` | 17172 | 6055/6064 | 81.937454 |
| hunter loop reload `enemies->size()` | 17164 | 6055/6058 | 91.191282 |
| hunter loop `int i` reload | 17164 | 6055/6058 | 91.191282 |
| hunter loop cache `Globals *g` | 17172 | 6055/6064 | 90.849080 |
| hunter loop cache `AERandom *rnd` | 17164 | 6055/6061 | 91.020139 |
| hunter loop cache `Route *route` | 17168 | 6055/6061 | 90.871575 |
| swap alienGuns/pirateFortress blocks | 17172 | 6055/6062 | 89.263019 |
| `RaidCoords3` moved after `localRace`/`raidRace` | 17180 | 6055/6064 | 86.756333 |
| `RaidCoords3` → `int raidCoords[3]` | 17172 | 6055/6063 | 92.028388 (neutral) |
| case 4: separate `Route *route` local | 17172 | 6055/6063 | 92.028388 (neutral) |
| case 4: `if/else` instead of `goto` | 17172 | 6055/6063 | 92.028388 (neutral) |
| case 10: separate `Objective *obj` local | 17172 | 6055/6063 | 92.028388 (neutral) |
| case 10: `break` instead of `return` | 17172 | 6055/6063 | 92.028388 (neutral) |

## What still differs (unchanged from wave-102)

- `alienGuns` loop: original branchy in-loop `ldr bigAlienWave; cbz` + six
  scalar `ldr`s; ours branchless `cmp.w/it/movne` + `ldmia`/`ldrd` pairs.
  Every source spelling of the selector (ternary, if/else, switch, row
  pointer, duplicated body, `==1`, `!=0`, `!!`, `&`, `|`) compiles to the same
  branchless select; the anchor `(i < 0)` is still required to keep the select
  in the loop and is still the only size-neutral shape found.
- Global callee-saved register permutation: original uses `r9 = k*24`,
  `sl = table base`, `r5 = &v`; ours uses `r8 = k*24`, `r1 = table base`,
  `r9 = &v`. The same `r6`/`sl` role swap appears in the `pirateFortress`
  loop. This cascades into the `raid` block (original keeps `raidType` in
  `r9` and `Globals::status` in `r8`; ours spills `raidType` to `sp+104`).
- Stack-slot colouring: original puts the cached `Globals::globals` pointer at
  `sp+88` and `raid` at `sp+84`; ours has them swapped (`sp+84` / `sp+88`).
  Changing `raid` to `int`/`char`/`unsigned char`/`long`, or rewriting its
  assignment as a single expression, does not move the slots.
- Merged station predicate: original `b2e04` emits
  `(i != first) | !alienWave` in one `orrs`; ours emits two branches
  (`cmp [sp,#60],#0; beq` then `cmp fp,r3; bne`). `bool alienWave` produces
  the `and i1 %phi, %icmp` IR but still lowers to two branches (and costs
  3.6 pp elsewhere), so this is a backend lowering difference, not a source
  difference reachable from the spellings tried.
- `case 10` `objectivesA` store is sunk to the shared return tail
  (`0xb41e4`) instead of emitted at `0xb20f6`.
- EH-pad tail: original main code ends at `0x4214`, ours at `0x420e`.

## Conclusion

The landed wave-102 shape is a strong local optimum: 25+ independent
source-shape perturbations (selector spelling, duplicated bodies, declaration
order, local caching, loop-index algebra, type changes, `&&`/`&`, block order)
all score at or below 92.028388. The remaining ~8 % is backend register
allocation, stack-slot colouring and block placement, not a missing statement,
constant, table or call order. The oracle confirms the turret tables, crate
store order and case-4 tail are already structurally faithful.

## Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/wf314
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/wf314/` (`Level.cpp.base`, `runvar.py`,
`score_fn2.py`, `dis.py`, `diffmap.py`, `try*.py`, `o_*.txt`, `u_*.txt`).
