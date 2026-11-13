# MGame::OnTouchEnd — wave191b w13774: cold-block placement survey (no win)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original
8,080 B @ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree
`wt/w13774`, branch `wave191b-w13774`, base master `9d4d7dd76`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w13774`.

Prior notes: `mgame-ontouchend-wave191b-w13760.md` (residual survey),
`-w13735.md` (8 landed store-order wins), `-w13729.md`, `-w13718.md`.
The campaign-15/22/100 and cinematic case-0 passes are landed.

## Result

**No win.** Function stays 8,080/8,080 B, raw **3,683**, aligned **544**,
insn **2,865/2,865**. Section deltas: `.text` 74,885, `.ARM.extab` 561,
`.ARM.exidx`/`.rodata`/`.rel.plt`/`.data`/`.got` all 0. `src/` unchanged;
`check` green (byte 4508, linked 4508, data 843, imports 207, lint CLEAN,
drift 0, parity 5370/5370). The base is a local optimum for the aligned
metric: every fresh source shape tried this session is either byte-identical
(clang normalises it) or grows size / aligned.

## Measurements (all size 8,080 unless noted; base = 3,683 / 544 / 8,080)

| variant | raw | aligned | size | `.text` Δ | `.ARM.extab` | note |
|---|---:|---:|---:|---:|---:|---|
| base (HEAD) | 3683 | 544 | 8080 | 0 | 561 | |
| `wsswap` (weapon-body store swap) | 3683 | 610 | 8080 | 0 | 561 | confirms w13760 |
| `invws` (invert `weaponSlot>=0`/`else`) | 3683 | 544 | 8080 | 0 | 561 | **byte-identical** |
| `iafirst2` (re-measured) | 4769 | 1139 | **8096** | +32 | 1284 | IA-first |
| `iafirst3` (IA-first, nested) | 4769 | 1139 | **8096** | +32 | 1284 | same as iafirst2 |
| `snd_playfirst` (Play arm first) | 6046 | 866 | **8084** | +32 | 1276 | |
| `snd_late` (engine block after particle effects) | 4471 | 824 | **8072** | +77000 | 599 | exidx 334 |
| `snd_exp_play` (`expect(soundsMuted,0)`) | 3683 | 544 | 8080 | 0 | 561 | **byte-identical** |
| `snd_exp_stop` (`expect(!IsCat&&!muted,0)`) | 4049 | 791 | **8096** | +32 | 1285 | rel.plt 8 |
| `snd_srp` (Stop/Resume/Play order) | 3683 | 544 | 8080 | 0 | 561 | **byte-identical** |
| `snd_nestB` (Play / else{Stop/Resume}) | 6046 | 866 | **8084** | +32 | 1276 | |
| `v_wswap` (other block store swap) | 3683 | 548 | 8080 | 0 | 561 | |
| `dockgoto` (shared dockToAsteroid, goto) | 3972 | 653 | **8076** | +19572 | 592 | exidx 24 |
| `dockgoto2` (same, explicit temps) | 3972 | 653 | **8076** | +19572 | 592 | exidx 24 |
| `m_amtne` (`amount != 0`, `String(count)`) | 6615 | 1302 | **8084** | +32 | 1291 | |
| `mine_inl_ne` (`amount != 0`, `String(-amount)`) | 6615 | 1302 | **8084** | +32 | 1291 | |
| `mine_ne0` (`count != 0`) | 6615 | 1302 | **8084** | +32 | 1291 | |

`.text` Δ is the whole-binary section delta vs the original (base 74,885).
`.ARM.extab` is the absolute diff count.

## Fresh findings (verified against the Thumb)

1. **The engine-sound *main body* already matches.** `IsCategoryEnabled(2)`
   test, `soundsMuted` load and the Resume arm are at the same offsets in
   both (`0x17aee2..0x17af2e`, only branch targets differ). The residual is
   purely where the *cold* Play/Stop arms are placed: ours Play `0x17b0bc`,
   Stop `0x17b618`; original Play `0x17b1e8`, Stop `0x17b5a0`. So the
   `snd_*` cluster is cold-block placement, not a source-semantics gap.
2. **The cold pair is adjacent in both.** The `weaponSlot=0` arm
   (`movs r6,#0`) sits immediately before the engine Play arm in both:
   ours `0x17b0b6`/`0x17b0bc`, original `0x17b1e4`/`0x17b1e8` — same
   relative order, shifted by `0x12e`. Clang's cold-block ordering, not
   source order.
3. **Engine-arm permutations are normalised or fatal.** `Stop/Resume/Play`
   (`snd_srp`) and `expect(soundsMuted,0)` (`snd_exp_play`) are
   byte-identical to base. `Play/Stop/Resume` (`snd_playfirst`) and
   `Play/else{Stop/Resume}` (`snd_nestB`) grow to 8,084/866; nesting the
   Stop arm under `expect` grows to 8,096. Moving the whole if/else after
   `enableParticleEffects` (`snd_late`) gives 8,072/824.
4. **Inverting the weapon-slot if/else is byte-identical.** Rewriting
   `if (weaponSlot >= 0) {weapon} else {wingman}` as
   `if (weaponSlot < 0) {wingman} else {weapon}` (`invws`) produces the
   exact same `.so`; clang normalises the arm order.
5. **Mining `!= 0` forms match the branch but move the counter register.**
   `amount != 0` / `count != 0` / `String(-amount)` all emit the wanted
   `cmp.w r8,#0; beq.n` — but the induction variable moves from `r9`
   (base, original) to `r8`, because `r8` is the `fee` slot in the loop.
   Aligned jumps 544 -> 1302 and size grows to 8,084. The base's
   `cmp.w r9,#-1; bgt.n` is the only size-exact form (w13760 finding 4).
6. **IA-first is correct but globally shifts layout.** The original tests
   `isInstantActionMission` first (`0x17af7e`) then `isCampaignMission`
   (`0x17afa8`, with the `mov r1,r0` save). `iafirst2`/`iafirst3` reproduce
   that but the whole function re-bases (`0x17afe2` start), adding 1 insn
   and 16 B, aligned 1139.
7. **`dockToAsteroid` shared tail still costs layout.** The original stream
   branch is 4 insns (`ldr r2,[fp,#128]; ldr r0,[fp,#88]; ldr r1,[r2,#36];
   b.n 0x17bcf4`) vs ours 5 (own `blx`). Both `goto` shapes (`dockgoto`,
   `dockgoto2`) reproduce the merge at 8,076 B but aligned 653 and `.ARM.exidx`
   +24; they need a +4 B exact compensator that has not been found.
8. **Remaining big clusters are all block order / literal-pool shift.**
   The 63-insn `0x17b37a..` cluster is our pool words disassembled as
   instructions; the 46-insn `0x17b6ec` cluster is the 95-body position; the
   31-insn `0x17bac6` insert is the weapon body emitted after the 110 body
   (orig weapon body `0x17b9cc` before 110 `0x17ba28`). None is
   source-local.

## Next steps if resumed

* The only structural levers left are cold-block placement (Play/Stop arms,
  weapon body vs 110 body) and the shared `dockToAsteroid` call. Both need a
  source shape that changes LLVM block placement without perturbing the
  literal pools; every shape tried so far either normalises away or grows
  size. A `-mllvm -print-after-all` block-placement trace (12 GB `ulimit -v`
  per compile, output to disk) is the next evidence to gather.
