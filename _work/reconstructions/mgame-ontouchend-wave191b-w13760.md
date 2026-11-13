# MGame::OnTouchEnd — wave191b w13760: residual aligned-cluster survey (no win)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original
8,080 B @ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree
`wt/w13760`, branch `wave191b-w13760`, base master `4facb0b48`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w13760`.

Prior notes: `mgame-ontouchend-wave191b-w13735.md` (landed 8 store-order
wins), `-w13729.md`, `-w13718.md`, `-w13692.md`, `-w13681.md`, `-w13672.md`,
`-w13662.md`, `-w13626.md`. The campaign-15/22 chain is landed.

## Result

**No win.** The function stays 8,080/8,080 B, raw **3,661**, aligned **546**
(`.text` 74,067, `.ARM.extab` 561, `.ARM.exidx`/`.rodata`/`.rel.plt`/`.data`/
`.got` all 0 diff). The current base is a local optimum for the aligned
metric: every source reordering tried this session and in the immediately
preceding sessions increases aligned (see table). `src/` is unchanged.

## Tooling correction

`semdiff2.py` (normalises branch targets + literal pools) and `dis2.sh`
(objdump with raw bytes) are the useful pair. `asmdiff.py` needs raw bytes
and silently mis-parses the `--ours` dump without them. Baseline semdiff:
**108 clusters / 524 span**.

## Measurements (all size 8,080 unless noted; base = 3,661 / 546 / 8,080)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (HEAD) | 3661 | 546 | 8080 |
| `cineif` (cinematic switch rewritten as if/else-if) | 3661 | 546 | 8080 |
| `mw_ne0` / `mw_gt0` (menuWindow `!=0` / `>0`) | 3661 | 546 | 8080 |
| `caseswap2` (cinematic case 0 before case 1) | 3661 | 546 | 8080 |
| `freelswap` / `fms` (reward-message vs setFreelanceMission order) | 3663 | 546 | 8080 |
| `wswap` (pauseOpen/hudMenuOpen store swap in weapon body) | 3661 | 612 | 8080 |
| `dockswap` (stream branch before asteroid branch) | 3621 | 602 | 8080 |
| `c_dw` / `c_df` / `c_wf` / `c_dwf` | 3621/3623/3663/3623 | 668/602/612/668 | 8080 |
| `split127after126` | 4798 | 937 | **8108** |
| `ia_exact` | 4716 | 1140 | **8092** |
| `order95` (95 clause after 74) | 4277 | 809 | **8084** |
| `faithful95` (95 after 74, source order) | 4277 | 809 | **8084** |
| `order95swbody` (95 after 74 + switch-first 95 body) | 4275 | 853 | **8084** |
| `sw95body` (95 body switch-first only) | 4252 | 734 | **8088** |
| `faithful_split` (95 after 74 + 127 split) | 4812 | 1015 | **8104** |
| `split127goto` / `sp127goto126` | 4869 | 964 | **8152** |
| `ex_all` | 4694 | 1494 | **8088** |
| `ex_stream0` / `ex_stream1` | 3780 / 3669 | 618 / 581 | 8080 |
| `ex_dp0` / `ex_ast1` | 3980 / 4095 | 635 / 652 | 8084 / 8076 |
| `snd_mfirst` / `snd_mfirst2` | 6049 / 6029 | 946 / 869 | 8084 |
| `mw_neq0` | 5599 | 820 | 8072 |
| `cntlt` / `cntne` / `amtne*` | 6556 / 6307 / 6307 | 1129 / 1282 / 1282 | 8084 / 8088 / 8088 |
| `sw95*` / `order95sw` | 4252 / 4275 | 734 / 853 | 8088 / 8084 |
| `noexpect` / `expect0` | 3685 | 589 | 8080 |
| `docktarget` (single dockTarget + trailing call) | 4036 | 668 | **8108** |

## Findings (verified against the Thumb)

1. **Weapon-slot body.** Original `0x17b9ca`: `movs r6,#3` falls into
   `0x17b9cc`; slots 0/1/2 branch to `0x17b9cc` and the `0x10000` arm enters at
   `0x17b9ca`. Body: `getShip`, `getEquipment(1)`, `getPlayer`,
   `setCurrentSecondaryWeaponIndex`, `setCurrentSecondaryWeapon`, then
   `movs r0,#0; strb r0,[fp,#93]` (pauseOpen 0x5d) **first**, `strb
   r0,[fp,#214]` (hudMenuOpen 0xd6) **second**, `resumeSounds`, `closeHudMenu`,
   `b.w 17afe8`. Ours emits the same single body (no duplication) but stores
   0xd6 then 0x5d. Swapping the stores alone (`wswap`) makes aligned worse (612)
   because block/pool layout shifts.
2. **127 is a separate test in the original** at `0x17bc24` (after the
   134/144 arm chain), `isCampaignMission`/`cmp #127`/`beq.w 17b722` (into the
   shared 96 body). Ours folds 127 into the 96 `||` test at `0x17b6fa`.
   `split127after126` reproduces it but is **+28 B**.
3. **dockToAsteroid shared tail.** Original stream branch `0x17bf4e`:
   `ldr r2,[fp,#128]; ldr r0,[fp,#88]; ldr r1,[r2,#36]; b.n 17bcf4`, where
   `0x17bcf4` is the asteroid branch's shared `blx dockToAsteroid` (followed by
   `b.n 17bb50`, the shared `hudEvent(6,...)`). Ours emits its own `blx` at
   `0x17bf5e` + `b.n 17bb62` (two call sites). The hudEvent *is* shared in
   ours; only the dockToAsteroid call is not. `ex_*` / `docktarget` variants
   all miss size-exact + better.
4. **Mining counter.** Original `cmp.w r9,#0; beq.n` (negative counter); base
   `cmp.w r9,#0xffffffff; bgt.n`. `count > 0` is the only size-exact form;
   `!=0`/`<0` flip the induction variable (+8/+4).
5. **Engine-sound cold block.** Original `0x17b1e8: cmp r2,#0; beq.w 17b5a0;
   blx PlayEngineSound; ...`; base has it at `0x17b0bc..0x17b0ec`.
   `snd_mfirst` = 8,084.
6. **Campaign chain order.** Original
   `15,22,43,42,65,81,74,95,96@17b708,127 separate@17bc24,100,110,120,126,134,144,155,161,162`.
   Ours `15,22,43,42,95,65,81,74,96+127 merged,100,...`. Original 95 body starts
   `switch_to_target_setting=1` then the getters then `getStation(10)`/
   `departStation`; ours has the getters then the switch. `order95` (move 95
   after 74) = 8,084 and aligned **809** (worse than 546) — the reorder fixes
   the 95 position but shifts every downstream block and pool entry.
7. **95/96/127 share one departStation tail in the original.** Original 95 body
   ends `movs r1,#10; b.n 17b77c`; 96 ends `movs r1,#98` and falls into the same
   `17b77c` (`getStation`/`departStation`/`active=0`/module 2). Ours gives 95
   its own copy. Making 95's body structurally identical to 96 and adjacent
   (`order95swbody`) still costs +4 B and aligned 853.
8. **Base has no missing semantic block.** `semdiff2.py` shows only
   block-placement / literal-pool / duplicated-call differences. The big
   "insert 63 insns" cluster at ours `0x17b37a..0x17b3c0` is literal-pool words
   disassembled as instructions.
9. `menuWindow->OnTouchEnd(...) == 1` vs `!=0`/`>0` is identical; `!=1` is
   8,072.
10. Literal-pool words differ at `0x17a7f8` (`c0ba0600` vs `dcba0600`) and
    `0x17a808` (`3efe0400` vs `5efe0400`) — address/relocation differences, not
    layout.
11. **Cinematic switch.** Original tests `cmp r0,#1; beq 17b7f4` first then
    `cmp r0,#0; bne 17b810`; ours tests `cmp r0,#0` first. Reordering source
    `case 0` before `case 1` (`caseswap2`) produces identical output; rewriting
    the switch as an if/else-if chain (`cineif`) also produces identical output
    — clang normalises switch order, so this is not source-controllable.
12. Original `17b6c4` region is the 154/157/158 skip-cutscene arm followed by
    case 1 then case 0; ours emits the case-0 body at `17b6d0` with `movs
    r1,#5` while the original's case-1 body at `17b7f4` uses `movs r1,#2`.
    Both blocks exist, placed in swapped order.
13. `mov r1,r0` vs `mov r4,r0` frequency diff — original has 3 `mov r1,r0`
    (String ctor arg moves at 17a206, 17a5ac, 17a756) where ours has `mov
    r4,r0`; a register-allocation consequence of block placement, not source.
14. Original 95 block at `17b694` is reached from the 74 arm's false branch
    (`beq.w 17b694` / `bne.w 17b694` at 17b556/17b564) and falls through to the
    96 test at `17b708`.
15. Original 100 body at `17b93c` is a separate block after the mining test at
    `17b910`; ours has it at `17b922` directly after the stack-canary epilogue.
    Original inserts `17b910..17b938` (mining test + `_bc8` dispatch) before it.
16. Original 110 body at `17ba28` follows the shared weapon-slot body
    (`17ba24 b.w 17afe8`), then 120 at `17bac0`, 134 at `17bc24/17bc3a`, 144 at
    `17bd00`, 155 at `17bd84..`, 161/162 later. Ours orders 110 at `17b9dc`,
    120 at `17bad8`, etc. — same relative order, shifted.
17. The 47 `operand:ldr` aligned diffs are pc-relative literal-pool distances;
    the 22 `operand:b.n` diffs are branch-target index differences. Both are
    block-order consequences, so aligned is effectively a block-order metric.
    The base is the closest of every spelling tried.

## Next steps if resumed

* The only remaining structural lever is the shared `dockToAsteroid` call
  (finding 3): it is worth 4 B in the original and would need a source shape
  that makes clang tail-merge the stream branch into the asteroid call without
  changing the branch order (a `goto`-based single call site was not tried in a
  size-exact form).
* Fixing 95's position (finding 6/7) always costs +4 B and worsens aligned, so
  it is not a Pareto win.
