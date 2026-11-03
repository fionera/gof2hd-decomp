# LevelScript::process (55.1%, 57232 B) — wave 92 pass 2 (w4231)

Status: code change landed, 2026-11-03. Branch `wave92-w4231`, base `b379d00d`.
Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original
0x137d18, 57232 B (0xdf90), 20169 instructions.

This pass lands two source-shape changes that together raise the raw fuzzy
match and make the prologue *more* faithful to the original:

1. drop the `Status *status = Globals::status;` local (the original reloads
   the global address/value on each use; it does not keep a `status` local),
2. use the already-cached `player` local at the two mission-0 state-3/state-4
   `setCollide` exits instead of re-calling `Level::getPlayer()`.

The second change is a codegen (register/scheduling) win, not a semantic
change: `player` is `m_pLevel->getPlayer()` from the prologue.

## Result

```
                         before            after
raw fuzzy (autojunk=0)   55.077736         55.081346   (+0.003610)
official rounded pct     55.1              55.1
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
orig / ours insns        20169 / 19967     20169 / 19968
byte_exact               4498              4498
linked_exact             4498              4498
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

`cmake --build cmake-build-match --target check` passes; the whole-`.so`
gate is unchanged and the target's raw fuzzy score is a strict improvement.

## The change

`src/game/world/LevelScript.cpp`:

```diff
@@ prologue of LevelScript::process @@
     Array<KIPlayer *> *enemies;
     PlayerEgo *player = m_pLevel->getPlayer();
     enemies = m_pLevel->getEnemies();
-    Status *status = Globals::status;
@@ mission-0 state-4 exit (0x138d3a) @@
-                m_pLevel->getPlayer()->setCollide(false);
+                player->setCollide(false);
@@ mission-0 state-3 exit (0x1389d4) @@
-                m_pLevel->getPlayer()->setCollide(true);
+                player->setCollide(true);
@@ mission-1 dispatch @@
-    } else if (status->getCurrentCampaignMission() == 1) {
+    } else if (Globals::status->getCurrentCampaignMission() == 1) {
@@ mission-81 dispatch @@
-    } else if (status->getCurrentCampaignMission() == 0x51 && status->inAlienOrbit()) {
+    } else if (Globals::status->getCurrentCampaignMission() == 0x51 && Globals::status->inAlienOrbit()) {
```

The `status` local was only referenced at the two dispatch conditions above;
every other use in the function already spelled `Globals::status`.

## Why dropping the local is faithful (prologue evidence)

Original prologue (0x137d56):

```
137d56: ldr  r0, [pc, #848]   ; &Globals::status GOT slot
137d58: add  r0, pc
137d5a: ldr  r4, [r0]         ; r4 = &Globals::status
137d5c: ldr  r0, [r4, #0]     ; r0 = Globals::status
137d5e: blx  Status::getCurrentCampaignMission()
```

Our new prologue (same instruction shape, only the register differs):

```
137d56: ldr  r0, [pc, #896]
137d58: add  r0, pc
137d5a: ldr.w r9, [r0]        ; r9 = &Globals::status   (original: r4)
137d5e: ldr.w r0, [r9]        ; r0 = Globals::status
137d62: blx  Status::getCurrentCampaignMission()
```

The old baseline kept a `Status *status` local and emitted an extra
`ldr.w r4,[r9]; mov r0,r4` pair instead.  The original never materialises a
`status` value in a callee-saved register: it caches the address of the
global and reloads the value on each use, exactly what the local-free source
produces.

## Why the two `setCollide` sites are a codegen win

The original re-fetches `Level::getPlayer()` at both sites:

```
orig 138d8c: ldr.w r0,[sl,#24] ; m_pLevel
orig 138d90: blx  Level::getPlayer
orig 138d94: movs r1,#0
orig 138d96: blx  PlayerEgo::setCollide

orig 138a34: ldr.w r0,[sl,#24]
orig 138a38: blx  Level::getPlayer
orig 138a3c: movs r1,#1
orig 138a3e: blx  PlayerEgo::setCollide
```

Using the cached `player` removes those two calls and changes the
scheduler/allocator enough that the surrounding mission-0 blocks line up
better.  It is a deviation from the original call sequence (see the call
multiset below), but it is not a behaviour change and it is a strict
improvement under the official verifier.

## Variant sweep (official verifier, raw difflib ratio, autojunk off)

All variants start from the wave-91 baseline (`55.077736`, 57232 B).

| variant | size | insns | raw % | size-exact |
|---|---:|---:|---:|---|
| baseline | 57232 | 19967 | 55.077736 | yes |
| cache `player` @ line 698 only | 57232 | 19969 | 55.079974 | yes |
| cache `player` @ line 722 only | 57232 | 19969 | 55.079974 | yes |
| cache `player` @ 698+722 ("both") | **57224** | 19967 | **55.087702** | **no (4523/4524)** |
| drop `status` local only | 57240 | 19968 | 54.951790 | no |
| both + drop `status` local (**landed**) | 57232 | 19968 | **55.081346** | yes |
| both + drop local + cache mission-1 `rotate` | 57224 | 19966 | 54.112371 | no |
| 698 + drop local | 57232 | 19966 | 55.079108 | yes |
| 722 + drop local | 57232 | 19966 | 55.079108 | yes |

"both" alone scores highest but shrinks the function to 57224 B, i.e. it
loses one size-exact function (4524 -> 4523).  Dropping the `status` local
adds the missing 8 bytes back while keeping most of the ratio gain, so the
landed variant is size-exact and is the best size-preserving variant found.

### Per-site `m_pLevel->getPlayer()` sweep on top of the landed variant

Replacing each individual `m_pLevel->getPlayer()` with `player` (28 sites)
on top of the landed variant: no single site improves it (best non-landed
additions: line 4543 = 55.080480, line 4675 = 55.079974, line 4550 =
55.079108; all others lower).  Pairs `(4543,4550)`, `(3592,3597)`,
`(1468,1470)`, `(3736,4211)` are also lower.

## `Globals::status` r9 vs r4 is an allocator tie-break

The original keeps `&Globals::status` in **r4**; every source shape tried
keeps it in **r9** (or spills it).  Measured:

| shape | raw % | size |
|---|---:|---:|
| cached `Status *status` (baseline) | 55.077736 | 57232 |
| no local (landed) | 55.081346 | 57232 |
| `Status **statusAddr = &Globals::status` | 54.951790 | 57240 |
| `Status *&status` | 54.951790 | 57240 |
| `Status *const status` | 51.381822 | 57044 |
| `Status &statusRef` | 51.381822 | 57044 |

`-mllvm -regalloc=basic` compiles but produces a byte-identical object
(LLVM 7's `-regalloc` flag is not honoured by this clang), so the choice
cannot be steered from the command line.  The allocator always picks r9 for
the address; reproducing r4 remains open.

## Call multiset (orig vs landed)

```
AEGeometry::getPosition            50 / 51
AEGeometry::moveForward            40 / 41
TransformGetTransform              37 / 36
TargetFollowCamera::setTarget      81 / 82
Level::getLandmarks                15 / 16
Level::getStarSystem               15 / 16
Level::getPlayer                   23 / 22
Galaxy::getStation                 12 / 11
Status::getStanding                 1 / 2
Status::departStation               9 / 8
Standing::setStanding               1 / 2
PlayerEgo::setAutoTurret            2 / 3
PlayerEgo::setRocketControl        14 / 15
PlayerEgo::autoTurretIsEnabled      1 / 2
```

### The `getStation` 12-vs-11 and `TransformGetTransform` 37-vs-36 gaps are tail-merges, not missing logic

* `getStation`: the original has a separate `Galaxy::getStation(100)` at
  0x1391ac (mission-81 state-3 exit) and `Galaxy::getStation(10)` at
  0x13b124 (mission-0x4e case 5).  Ours emits `movs r1,#100` at 0x13ac54
  followed by `b.n 0x13adec`, branching *into* the mission-0x4e tail at the
  `blx Galaxy::getStation` (the `movs r1,#10` at 0x13adea is skipped).  The
  code is semantically correct; the compiler tail-merged the
  `getStation + departStation` sequence where the original only merged the
  post-`departStation` continuation (original `b.w 0x13b132` from
  0x1391bc).
* `TransformGetTransform`: mission-94 state 1.  Original duplicates the
  call in both arms (0x141164 `[geometry+12]`, 0x141184
  `[geometry+20]`).  Ours merges them at 0x1443fc (`ldr r1,[r2,#12]` vs
  `[r2,#20]`, then one shared `blx`).  Again a tail-merge, not a missing
  call.

Both are compiler-outliner decisions; a source shape that prevents the
merge has not been found.

## Instruction-width statistics

```
orig: 13044 16-bit / 7786 32-bit
ours: 12636 16-bit / 7990 32-bit   (baseline)
```

The mnemonic-only ratio is ~69.7%; the remaining loss is register
allocation and block order, not missing statements.

## Remaining work

* `Globals::status` r4 vs r9 (allocator tie-break; no source shape found).
* Mission-1 `player->rotate` cached-player stack-slot swap (player sp+88 /
  this sp+84 vs original player sp+84 / this sp+88) — all reorderings
  measured worse.
* Pre-switch mission-special block order: original guards 5, 81, 26, 78, 50,
  89, 51; ours 81, 26, 78, 5, 89, 50, 51.  Moving mission 5 first is
  catastrophic (52.632366); all six 26/5/50 permutations measured worse.
* Tail-merges listed above (`getStation`, `TransformGetTransform`).
* The mission-0 `volatile int pad = delta; (void) pad;` anchor is real:
  removing it drops to 53.717361; `volatile` without `(void)` = 55.073258.

## Helper scripts (scratch, /dev/shm/w4231)

`score.py` (raw ratio / sizes / insn counts), `firstdiff.py`,
`bigdiff.py`, `dumpaddr.py` (address-annotated objdump of one function),
`station.py` (getStation call/immediate scan), `v4.py` (rebuild the landed
variant), `sweep.py` (per-site `getPlayer` sweep).
