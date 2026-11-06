# MGame::OnTouchEnd — wave 124 (w7414): keepObjectives-first, reset block +0xE

Status: one landed source tweak, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7414`,
base `6f8ee2320`, owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch: `/dev/shm/w7414*` (not committed).

Target: `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | before | after |
|---|---|---|
| fuzzy match | 82.3635 % (report 82.4) | **82.7748 % (report 82.8)** |
| body size (orig/ours) | 8080 / 8080 | 8080 / 8080 |
| `resetStartSequenceOver` block | `0x17af50` | `0x17af5e` (+0xE) |
| `.rel.plt` differing slots | 2 (3326-3327) | 2 (3326-3327, unchanged) |

## Change (won-body statement order only)

```diff
                 } else {
+                    bool keepObjectives = ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
                     Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : (Mission *) (intptr_t) Globals::status->getCampaignMission();
-                    Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
-                    bool keepObjectives = campaign->hasWon() != false ? !Globals::status->getMission()->isCampaignMission() : false;
```

This is the original's source order (`keepObjectives` first, then `mission` selection; the
original calls `getCampaignMission()` twice rather than keeping a `campaign` temporary — see
`0x17ade0`/`0x17ade8`). `Status::getCampaignMission()` is a pure getter (`return (*missions)[0]`),
so dropping the temporary is semantically identical. The compiler canonicalises the `? :` form
(`if`/`if-else`/ternary all measure the same, 82.7748 / 8080).

## Evidence

Original won-body order (`0x17ade0`):

```
17ade0: ldr r0,[r8] ; getCampaignMission
17ade8: blx getCampaignMission
17adec: blx hasWon
17adf0: cmp r0,#0
17adf2: beq.w 17af5a          ; keepObjectives = false
17adf6: getMission ; isCampaignMission ; eor sl,r0,#1
17af5e: getMission ; hasWon ... ; mission selection
```

Our build before the change computed `mission` first (and kept a `campaign` temporary); after
the change the keepObjectives block is emitted first, which pulls the cold
`resetStartSequenceOver` block from `0x17af50` to `0x17af5e` (it is still placed immediately
after the first won-body block, before `initHudMenu(2)`).

`resetStartSequenceOver` block after the change:

```
17af5a: ldr.w r0,[fp,#124]    ; this->levelScript
17af5e: blx resetStartSequenceOver
17af62: b.w 17b8f2            ; cutscene tail (frameTime=0; resetAnalogStick; releaseAllKeys)
```

`.rel.plt` order is unchanged: `resetStartSequenceOver` (0x213a84) still precedes
`isInstantActionMission` (0x213a88); the original has them the other way round.

## Gate (full `check`)

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` CLEAN, `drift` 0 fields across 0 classes. `.ARM.exidx` stays byte-identical.

Section byte deltas vs the original (baseline -> this change):

| section | size | differing (base) | differing (this) |
|---|---|---|---|
| `.text` | 1259464 | 87014 | **86838** (−176) |
| `.ARM.extab` | 156312 | 1446 | **1443** (−3) |
| `.rel.plt` | 29392 | 4 | 4 |
| `.note.gnu.build-id` | 36 | 20 | 20 |

`.ARM.exidx`, `.rodata`, `.data`, `.bss`, `.got`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`
are byte-identical.

## Cold `resetStartSequenceOver` block: what was tried and why it did not reach 0x17b0aa

The original places the reset block at `0x17b0aa`, immediately after the failed `type == 12`
block (`0x17b07e..0x17b0a8`) and immediately before the won freelance block (`0x17b0b6`).
The current `won`-local shape places it at `0x17af5e`; the pre-wave-123 natural `else` shape
places it at `0x17b160` (and is 81.679 %); the bare-`hasWon()` natural form places it at
`0x17b43a` but is 8084 B.

Two hard constraints drove the search:

1. **Size must stay 8080.** `OnTouchEnd` sits mid-`.text`; any size change shifts every later
   PC-relative literal pool and drops `byte_exact` from 4506 to ~3106. The natural
   `if (failed) … else if (hasWon) … else { reset }` form is 8084 B at every spelling tried.
2. **`.rel.plt` first-reference order must not regress.** The reset call must stay after
   `isInstantActionMission` to match the original's slot order; moving it early keeps the
   current 2-slot delta, moving it past `removeObjectives` grows it to 7 slots.

Shapes measured (all `won` local unless noted; score / size / reset addr / PLT diff lines):

| shape | score | size | reset | PLT |
|---|---|---|---|---|
| base `if (!failed && !won) reset; else if (failed) … else …` | 82.3635 | 8080 | 17af50 | 4 |
| **keepObjectives-first (landed)** | **82.7748** | **8080** | **17af5e** | **4** |
| keepObjectives-first + `if (!(failed‖won))` | 82.7748 | 8080 | 17af5e | 4 |
| natural `else { reset }`, bare `hasWon()` | 82.2703 | 8084 | 17b43a | 14 |
| natural `else { reset }`, bare + keep-first | 82.6095 | 8084 | 17b43a | 14 |
| natural, `hasWon() == true` (pre-wave-123) | 81.6790 | 8080 | 17b160 | 6 |
| nested `if (!failed) { if (won) … else reset }` | 72.8582 | 8088 | 17af60 | 10 |
| `if (failed) … else if (won) … else reset` (`won` local) | 72.9302 | 8088 | 17af60 | 10 |
| `if (failed) … else if (!won) reset …` (`won` local) | 72.9302 | 8088 | 17af60 | 10 |
| `goto` tail / `goto` reset label | same as base / 72.99 | 8080 / 8088-8092 | 17af50 / 17b71a-17b75a | 4-16 |
| `__builtin_expect` on failed / won / reset (0 and 1) | 55.9-82.36 | 7820-8088 | 17adf0-17bfe0 | 10-24 |
| instant-first dispatch (`isInstantActionMission` before `isCampaignMission`) | 72.9993-73.3826 | 8072-8076 | 17af50-17af5e | 4 |
| separate `if (instant) return;` before campaign/freelance | 72.9993-73.3826 | 8072-8076 | 17af50-17af5e | 4 |

`__builtin_expect` hints move the block to either the very front (`0x17adf0`) or the very end
(`0x17bfe0`) and change size, so none is landable.

Root cause of the remaining gap: the reset block is the `else` of the won check. clang's
`MachineBlockPlacement` places it immediately after the first won-body block
(`keepObjectives` here, `mission` selection in the base). The original instead keeps the whole
won-body chain (`keepObjectives` → `mission` selection → `isInstantActionMission` →
`isCampaignMission` → `nextCampaignMission`) contiguous, then places the failed `type == 12`
block, then the reset block. Reproducing that needs the failed `type == 12` block to be laid
out *after* the won-body dispatch; every 8080 B shape tried keeps it before the dispatch. The
original's instant-first dispatch order is itself only reproducible at 8072-8076 B in our
tree, so the two differences are coupled.

## Additional wave-124 sweep (w7414): shapes that move the reset block further

All measured against the landed 82.7748 % / 8080 B state. The original's reset block is at
`0x17b0aa`; the landed block is at `0x17af5e`. The table records score / size / reset address /
`.rel.plt` differing lines.

| shape | score | size | reset | PLT |
|---|---|---|---|---|
| landed (keepObjectives-first) | **82.7748** | **8080** | **17af5e** | **4** |
| swap branches: `if (!failed&&!won) reset; else if (!failed) won; else failed` | 82.7027 | 8080 | 17af5e | 4 |
| `failed` first, `else if (!won) reset`, else won (won hoisted) | 72.9924 | 8084 | 17af6a | 10 |
| `if (failed) {...} else { bool won; if (won) won-body else reset }` | 80.2307 | 8076 | 17b15e | 6 |
| `if (!failed) { bool won; if (!won) reset else won-body } else failed` | 80.1586 | 8076 | 17b15e | 6 |
| separate `if (!failed&&!won) reset;` then `if (failed) … else …` | 72.1341 | 8072 | 17ab4e | 22 |
| failed-head early-return then common dispatch (original CFG shape) | 73.0873 | 8092 | 17b0ca | 12 |
| `switch ((failed?2:0)|(won?1:0))` | 78.3172 | 8120 | 17adc6 | 10 |
| `campaign` temporary restored (keepObjectives first) | 81.5709 | 8084 | 17af5e | 4 |
| reuse one `campaign` for both predicates | 81.1824 | 8072 | 17af60 | 4 |
| bare `hasWon()` in `won` (W2) | 81.7658 | 8088 | 17af86 | 4 |
| bare `goingToWaypoint()` in failed cleanup (Y2) | 81.6150 | 8076 | 17af5e | 4 |
| W2 + Y2 | 81.7658 | 8088 | 17af86 | 4 |
| `keepObjectives` spelled `!= false` / `== true` | 82.7748 | 8080 | 17af5e | 4 |

The early-return shape reproduces the original's *control flow* (failed head inline, then the
won computation, then the common dispatch, failed tail outlined) and lands the reset block at
`0x17b0ca`, within `0x20` of the original `0x17b0aa` — but costs 12 B (8092), which shifts every
later `.text` symbol and collapses `byte_exact`. The 8080 B size is the binding constraint: every
shape that reproduces the original's failed-tail outline is >= 8084 B.

The landed change is therefore the best 8080 B / 4-PLT-line point found: it keeps the named
`won` local and the inverted guard, computes `keepObjectives` before the `mission` selection
(the original's source order), and drops the `campaign` temporary. The cold block moves +0xE
without changing size or the `.rel.plt` first-reference order.
