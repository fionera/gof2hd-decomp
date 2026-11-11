# Small tail wave 169 (w11519): MGame m122 anchor re-shaped, ModStation/MenuTouchWindow re-swept

Status: **1 `src/` change landed**.  Worktree `wt/w11519`, branch
`wave169-w11519`, base master `00c403d5a`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11519`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baseline residuals re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` / `C2Ev` | 43 | 12 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`; the
incremental link is ~1.1 s.  This note extends
`small-tail-wave169-w11419.md` / `small-tail-wave168-w11318.md`; only the
**new** shapes are recorded here.

## MGame::OnUpdate — landed: m23 `ldrd` tie removed, m122 anchor re-shaped

Landed source shape (the wave-163 "level mid" form):

```cpp
int radioIndex = Globals::rnd->nextInt(2);
Level *level = this->level;
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
level->createRadioMessage(28, radioIndex + radioBase);
```

The `Status *volatile st` local is gone; `Globals::status` is read directly
for the `getCurrentCampaignMission` call.  This removes the long-lived `r2`
that forced the m23 `ldrd r3,r5` (orig `ldrd r2,r3`), so the m23 run is now
byte-identical.  The `Level *level` local before `radioBase` keeps the
GOT-base-in-`r9` / 11440-byte shape (4002 instructions, same as the
original).

Residual is now a single 16-byte run at `0x28be..0x28d6`: clang hoists
`ldr.w r5,[fp,#120]` (`this->level`) one instruction above the
`getCurrentCampaignMission` call; the original loads it after the call and
into `r1`.  Every shape that removes the hoist loses the 11440-byte anchor
(11392) or adds a spill (11444), so this is the size-exact local optimum.

Shapes tried this wave (size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master: `Status *volatile st` + `Level *level` after radioBase | 15 | 111 | 11440 |
| **landed**: nextInt, `Level *level`, direct `Globals::status` | **16** | **6** | **11440** |
| nextInt, volatile-load `st`, `Level *level`, direct | 16 | 6 | 11440 |
| nextInt, `Level *level`, `int mission = ...`, radioBase | 16 | 6 | 11440 |
| nextInt, `Level *level`, if/else radioBase | 16 | 6 | 11440 |
| nextInt, `Level *level`, `__builtin_expect(ccm,0)` | 16 | 6 | 11440 |
| nextInt, `Level *level`, `((MGame *volatile)this)->level` | 16 | 6 | 11440 |
| nextInt, `register Level *level` | 16 | 6 | 11440 |
| nextInt, volatile-load `st`, `Level *level`, `(void) Globals::status` | 16 | 6 | 11440 |
| nextInt, `Level *level`, volatile-load `st`, direct | 19 | 6 | 11440 |
| nextInt, `Level *level`, volatile-load `st`, `(void)` cast | 19 | 6 | 11440 |
| `Level *level` first, direct status | 33 | 8 | 11440 |
| `Level *level` first, volatile `st` | 38 | 119 | 11440 |
| nextInt, volatile-load `st`, `Level *level` after radioBase | 362 | 20 | 11440 |
| nextInt, volatile-load `st`, `Level *level` first | 361 | 25 | 11440 |
| nextInt, volatile-load `st`, direct `this->level` | 362 | 20 | 11440 |
| `Level *level` first, volatile-load `st`, direct | 362 | 24 | 11440 |
| nextInt, `Level *level`, volatile store after ccm | 345 | 22 | 11440 |
| nextInt, `Level *level` after radioBase, direct (natural) | 5196 | 597 | 11392 |
| `Level *level` first, non-volatile `st` | 929 | 115 | 11436 |
| `Status *st` non-volatile + level after | 918 | 109 | 11436 |
| nextInt, `Status *volatile st` after nextInt, level after | 5206 | 599 | 11396 |
| `Level *level` first, `Status *volatile st`, level via st | 920 | 131 | 11444 |
| nextInt, `Level *level`, `Status *volatile st`, direct ccm | 345 | 22 | 11440 |
| nextInt, `Level *level`, `Status *volatile st`, ccm via st | 919 | 130 | 11444 |

The 16/6 form is a +1 raw / -105 aligned trade against master: it removes
the m23 `ldrd` run and the volatile-store spill entirely, leaving one
scheduler-hoisted load.  Raw-first would keep 15/111; the landed shape is
the structurally faithful one and is gate-neutral (byte/linked exact counts
unchanged).

## ModStation::ModStation C1/C2 — no new size-exact raw win

The residual is unchanged: the two `ldr` literal materialisations of the
ternary operands are emitted in the opposite order (`Yaw` first vs the
original's `YawIPad` first).  The DAG dump (`-mllvm -print-machineinstrs`)
shows the first select builds `%117=Yaw, %118=YawIPad`, while the *second*
(farKey) select builds `%162=YawIPad, %163=Yaw`; the first pair is the one
that is emitted, so the order is fixed by the first select's ISel visit.

New shapes this wave (size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master ternary | 43 | 12 | 1008 |
| `if/else` (both polarities) | 43 | 12 | 1008 |
| `iPad == 0 ? Yaw : YawIPad` | 43 | 12 | 1008 |
| `iPad != 0 ? YawIPad : Yaw` | 43 | 12 | 1008 |
| `__builtin_expect(iPad != 0, 0) ? ...` | 43 | 12 | 1008 |
| `!iPad ? Yaw : YawIPad` | 43 | 12 | 1008 |
| `&Yaw[0]` / `(const float *)` arms | 43 | 12 | 1008 |
| local arm pointers (`yawIPadTable`, `yawStdTable`) | 43 | 12 | 1008 |
| `static inline` helper returning the select | 43 | 12 | 1008 |
| farKey via a named `yawTableFar` local | 43 | 12 | 1008 |
| farKey via the same `yawTable` local | 604 | 71 | 984 |
| two named locals (GVN merges the selects) | 604 | 71 | 984 |
| inlined ternary in the `yawBits` read | 698 | 52 | 1000 |
| `yawBits` read before `pitchBits` | 752 | 40 | 1000 |
| `const float *yawTables[2]` local array | 636 | 87 | 996 |
| `iPad & 1 ? ...` | 572 | 24 | 1008 |

Every shape that flips the load order also changes the schedule/frame
(984/996/1000 B).  The 1008-byte forms all keep `Yaw` first.

## MenuTouchWindow::OnTouchBegin — no new size-exact raw win

Residual unchanged: case 8 `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]`
(2 bytes) and the case-15 `leftMargin` cached-vs-reloaded block (26 bytes,
one instruction shift).  New shapes:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master | 28 | 16 | 1896 |
| case-15 via outer `layout` pointer | 1781 | 818 | 1912 |
| case-15 both fields via `Globals::layout` | 362 | 76 | 1892 |
| case-15 leftMargin via `Globals::layout` | 362 | 76 | 1892 |
| case-15 both fields via the reference | 308 | 73 | 1892 |
| case-15 `int leftMargin` local before the call | 320 | 33 | 1892 |
| case-15 second cond parenthesised/reordered | 113 | 63 | 1896 |

The reload form saves two instructions globally (1892 B); the master form
keeps the 1896-byte shape and caches `leftMargin` in `r6`.  The `ip`/`r4`
choice in case 8 is a greedy-RA tie.

## Conclusion

One landed change: MGame::OnUpdate's m122 block is re-shaped to the
wave-163 "level mid" form, removing the m23 `ldrd` tie and the volatile
store (15/111 -> 16/6 raw/aligned, 11440 B, 4002 instructions).  ModStation
and MenuTouchWindow remain SelectionDAG / greedy-RA ties with no
size-exact raw win.
