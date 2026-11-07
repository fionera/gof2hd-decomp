# MGame::OnTouchEnd — wave 126 (w7629): weaponSlot tail statement order

Status: one landed source tweak, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7629`,
base `4bbe88776` (wave-125 final), owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch: `/dev/shm/w7629*` (not committed).

Target: `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | before | after |
|---|---|---|
| fuzzy match | 81.9 % | **81.9 %** |
| body size (orig/ours) | 8080 / 8080 | 8080 / 8080 |
| `.rel.plt` differing bytes | 0 | **0** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.ARM.extab` differing bytes | 1,436 | **1,436** |
| `.text` differing bytes | 85,520 | **85,519** |

## Change (semantics-neutral, faithful original statement order)

`MGame::OnTouchEnd`, hudMenu `weaponSlot >= 0` block:

```diff
             if (secondaries != nullptr) {
                 this->level->getPlayer()->setCurrentSecondaryWeaponIndex((*secondaries)[weaponSlot]->getIndex());
                 this->hud->setCurrentSecondaryWeapon((*secondaries)[weaponSlot]);
             }
-            resumeSounds();
             this->pauseOpen = 0;
             this->hudMenuOpen = 0;
+            resumeSounds();
             this->hud->closeHudMenu();
         } else if (wingmanCommand >= 0) {
```

The original writes `pauseOpen = 0`, `hudMenuOpen = 0`, then calls `resumeSounds()`, then
`closeHudMenu()` (see `0x17ba0c..0x17ba24` below). Our previous source called `resumeSounds()`
first. The order is observable only in the emitted stores/calls; it is behaviour-neutral because
`resumeSounds()` does not read either flag. The `.text` delta drops by one byte; the fuzzy score
is unchanged (81.9 %).

## Evidence (original, VMA)

Weapon-slot common tail (`0x17b9cc`):

```
17b9cc: ldr r0,[pc,#440]      ; Globals::status
17b9d4: blx Status::getShip
17b9d8: movs r1,#1
17b9da: blx Ship::getEquipment
17b9de: mov r4,r0
17b9e0: cbz r0, 17ba0c
...
17ba0c: movs r0,#0
17ba0e: strb.w r0,[fp,#93]    ; pauseOpen = 0
17ba12: strb.w r0,[fp,#214]   ; hudMenuOpen = 0
17ba16: mov r0,fp
17ba18: blx MGame::resumeSounds
17ba1c: ldr.w r0,[fp,#116]    ; hud
17ba20: blx Hud::closeHudMenu
17ba24: b.w 17afe8            ; final tail (skips the hudTouchFlags==0 test)
```

The same order is present in the 0x800 block (`0x17ae8c`) and the wingman block (`0x17bf2e`),
where our source already matched.

## Gate (full `check`)

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` CLEAN, `drift` 0 fields. Section sizes all exact; `.rel.plt`, `.ARM.exidx`, `.rodata`,
`.data`, `.bss`, `.got`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn` byte-identical. Only
`.text` (85,519), `.ARM.extab` (1,436) and `.note.gnu.build-id` (20) differ.

## Variants tried and rejected (all measured with the full verify)

| variant | pct | size | PLT | note |
|---|---|---|---|---|
| **landed (weaponSlot order)** | **81.9** | **8080** | **0** | `.text` 85,519 |
| base (wave 125) | 81.9 | 8080 | 0 | `.text` 85,520 |
| mining `if (count != 0)` | 74.3 | 8064 | 0 | original encoding `cmp r9,#0; beq`, but −16 B |
| mining `if (amount != 0)` | 74.3 | 8064 | 0 | same |
| `hasFailed()` bare (drop `== true`) | 73.9 | 8088 | 6 | +8 B, `.rel.plt` 6 |
| `hasFailed() != false` | 73.9 | 8088 | 6 | +8 B |
| `won` second `hasWon()` bare | 77.0 | 8064 | 0 | original `cmp r0,#0; beq`, but −16 B |
| `won` second `hasWon() != false` | 77.0 | 8064 | 0 | same |
| `failed` bare + `won` bare | 74.2 | 8068 | 6 | −12 B |
| `menuWindow->OnTouchEnd(...)` bare | 80.4 | 8080 | 0 | |
| separate `if (hudTouchFlags == 0)` | 79.0 | 8076 | 0 | original CFG (wingman→test), −4 B |
| separate `if` + refetch campaign | 79.2 | 8080 | 0 | |
| wave-124 `if (!failed && !won) reset` form | 79.3 | 8080 | 0 | |
| nested `else { if (wingman) ...; if (flags==0) ... }` | 81.8 | 8080 | 0 | codegen identical to base |
| nested + weaponSlot order | 81.9 | 8080 | 0 | `.text` 85,523 |
| `goto afterHudMenu` + separate `if` | 81.9 | 8080 | 0 | goto folded away, `.text` 85,523 |

## Remaining root cause (unchanged from wave 125)

The original hudMenu cluster is

```
[0x400 body][shared blx initHudMenu][test][final tail][hudTouchFlags==0 body]
```

with the 0x200 body branching forward into the shared `blx`, the test at `0x17afe2`
(`ldr flags; cbz cold`), the final tail at `0x17afe8`, and the cold `hudTouchFlags==0` body at
`0x17b01a`. Our build still emits

```
[0x200 body (inline blx)][...][0x800 body][test][hudTouchFlags==0 body][0x400 body → 0x200 blx]
```

i.e. the 0x200 body owns the shared `blx`, the test is placed after the 0x800 body, and the
final tail is outlined after the weapon-slot bodies. The separate-`if` source shape reproduces
the original CFG (`cbz` to the cold body, wingman falls through to the test) but is 4 B short
(8076 B); every 8080 B shape tried keeps the 0x200 body as the common tail. The campaign
`hasWon() == true` spelling is also still a content mismatch (`cmp #1; bne` vs the original
`cmp #0; beq`), but dropping it shrinks the body to 8064 B, so it cannot be landed alone.
