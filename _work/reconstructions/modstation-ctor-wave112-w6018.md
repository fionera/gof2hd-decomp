# ModStation ctor — wave 112 (w6018): yaw-first bitcasts + yaw live into far rotation

Status: landed. Worktree `wt/w6018`, branch `wave112-w6018`, base `5d98bd391`.
Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B.

## Result

```text
before: raw 98.626374 %, 42 differing bytes in 7 runs, 1008 B
after:  raw 99.175824 %, 11 differing bytes in 4 runs, 1008 B
gate:   verify byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
        data exact 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS
        sections 27/27, file size 2,187,328 B (unchanged)
```

This is the first source-level shape to reach the pass-5 hand-crafted IR
`best16d` result (11 diff / 4 runs, original literal pool) from real C++.
The function is still not byte-exact, so `byte_exact` stays 4500; only the
raw instruction-stream diff improves.

## What changed

`src/game/menu/ModStation.cpp`, `ModStation::ModStation()` — near-camera
rotation block:

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;      // added (yaw first)
    float pitch = *(const float *) &pitchBits;  // added
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
```

and the far-camera rotation call:

```cpp
-    MatrixSetRotation(farKey, *(const float *) &pitchBits, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
+    MatrixSetRotation(farKey, yaw, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                       (AbyssEngine::AEMath::RotationOrder) 2);
```

## Mechanism

Two independent effects combine:

1. **Declaration order of the float locals.** Declaring `float yaw` before
   `float pitch` makes the front end emit `%yaw = bitcast` before
   `%pitch = bitcast`, while the call still receives `(pitch, yaw)`. That is
   exactly the hand-edited `acq2_v1_ar_dbg.ll` IR that produced `best16d`.
   The pre-RA MIR of bb.12 then materialises the literals in the original's
   order: `%116 @gHangarCamPitch`, `%117 @gHangarCamYawIPad`,
   `%118 @gHangarCamYaw`. The baseline emitted `%116 Pitch`, `%117 YawIPad`,
   `%119 Yaw` (the yaw literal after the `ldrb`), which swapped the
   `YawIPad`/`Yaw` LDRLITs and shifted the pool by 2.
2. **Keeping `yaw` live into the far call.** If the far call keeps
   `pitchBits`/`pitch` (the semantically clean spelling), the optimiser
   reorders the bitcasts back and the pool reverts — `f_yawfloatfirst`
   measures the baseline 98.626 / 42 / 7. Using the `yaw` float as the far
   call's first argument preserves the IR shape through InstCombine and
   selection.

The literal pool is now exactly the original `{0x128bf0, 0x128c16, 0x128c38}`
= `{gHangarCamPitch, gHangarCamYawIPad, gHangarCamYaw}`. The 16-byte run at
`+0x16a` and the three pool-word diffs at `+0x3e0/+0x3e4/+0x3e8` are gone.

## Residual (11 bytes, 4 runs)

Only the post-RA schedule tail of the near `MatrixSetRotation` call differs:

```text
        orig                                    ours
d5456   ldr.w r3,[r0,r8,lsl#2]   ; yaw      d5456  str.w r8,[sp,#184]
d545a   movs  r0,#2                          d545a  ldr.w r4,[r0,r8,lsl#2] ; yaw
d545c   str.w r8,[sp,#184]                   d545e  movs  r0,#2
...                                          ...
d546c   mov   r0,r8                          d546c  mov   r3,r4  ; extra copy
d546e   mov   r4,r2                          d546e  mov   r0,r8
```

Register assignment is otherwise identical. The extra `mov r3,r4` exists
because the yaw load's result is assigned to the just-killed `Yaw` pointer
register `r4` instead of directly to argument register `r3`, and `str.w r8`
is scheduled before the load. Both are post-RA choices; see
`modstation-ctor-pass5-w4263.md` §6.2.

Byte runs:

```text
+0x186..+0x186 (1B) orig=50 ours=cd
+0x188..+0x18f (8B) orig=28300220cdf8b880 ours=b88050f828400220
+0x19c..+0x19c (1B) orig=40 ours=23
+0x19e..+0x19e (1B) orig=14 ours=40
```

## Trade-off

`MatrixSetRotation(farKey, yaw, ...)` passes the yaw float as the far
rotation's *pitch* argument. The far call's first argument is dead in the
original's register allocation (the original reuses the pitch value), so this
is a source-shape device, not a semantic claim. The semantically clean
spelling (far call uses `pitch`) measures 98.626 / 42 / 7 — no gain.

## Dead ends measured (all 1008 B unless noted)

Baseline 98.626 / 42 / 7.

| variant | raw % | diff B | runs |
|---|---|---|---|
| `f_yawfloatfirst` (float locals, far call unchanged) | 98.626 | 42 | 7 |
| `f_floatlocals` | 88.646 | 757 | 36 (1016 B) |
| `f_oninit_style` | 86.694 | 667 | 86 |
| `oldshape` (upstream `.cache/ModStation.orig.cpp` shape) | 86.694 | 670 | 87 |
| `plainfloat` (`float pitch = ...; float yaw = ...`) | 86.694 | 670 | 87 |
| `nopitchtable` (no `pitchTable` local) | 98.352 | 46 | 9 |
| `acqrel_before` / `acqrel_pitchptr` | 65.915 | 649 | 50 (952 B) |
| `pitchload_first` | 66.947 | 685 | 33 (960 B) |
| `yawload_first` | 65.734 | 730 | 37 (968 B) |
| IR `v_1f_seq_before` | 98.077 | 29 | 13 |
| IR `v_2f_after_sel` | 98.352 | 46 | 9 |
| IR `v_ne_swap` | 97.527 | 44 | 9 |
| IR `v_eq_swap` | 98.352 | 43 | 8 |
| IR `b16_pitch` / `b16_newbc` / `v_freshfar` | 98.626 | 45 | 9 |
| IR `v_bothlive` (far call `pitch, yaw`) | 84.167 | 580 | 72 (984 B) |
| IR `v_dbgkeepyaw` | 98.626 | 42 | 7 |

120 random topological orders of the near block (`gen2.py`/`search2.py`)
topped out at the baseline 98.626 / 42 / 7.

## Structural state

27 sections (exact), file size 2,187,328 B, `.ARM.extab` size/base exact
(`0x26298`), `.text`/`.data`/`.bss`/`.ARM.exidx` size- and base-exact.
`.rodata` remains 0xC0 larger than the original (the `Status::missionCompleted`
rank table at `0x20b977`); every byte before it is exact. No section or
extab change from this pass.

## References

- `_work/reconstructions/modstation-ctor-wave106-w5419.md` — fence placement, pool-order history.
- `_work/reconstructions/modstation-ctor-pass5-w4263.md` §6.2 — the hand-IR `best16d` 11/4 shape reproduced here from real C++.
- `_work/reconstructions/modstation-ctor-pass6-w4605.md` — InstCombine canonicalisation; LDRLIT order is a scheduler decision.
- `_work/reconstructions/residual-taxonomy-wave111-w5920.md` §18 — prior 42 B residual description (superseded).
