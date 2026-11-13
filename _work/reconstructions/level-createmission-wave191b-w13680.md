# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13680

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13680`, base master `b221b3826`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13650.md` (the landed semantic
alienGuns interleave, raw +10 / aligned −4), `...w13638.md`, `...w13625.md`.

```text
                                     size            raw    aligned  insn
base b221b3826 (interleave landed)   17172/17172     2636    841     6206/6197
this pass (landed)                   17172/17172     2626    864     6206/6197
```

Landed: **the +10 raw cost of the alienGuns interleave is recovered exactly**
(raw 2636 -> 2626, back to the pre-interleave base raw), by moving
`gun->cargo = nullptr` to the top of the loop body (before the first
rotation) and `gun->player->setMaxHitpoints(1000)` to after the second
rotation.  The semantic interleave is untouched: `setRotation` still receives
the `t[k][3..5]` rotation and `setPosition` still uses the `t[k][0..2]`
rotation.

Net: raw **2636 -> 2626 (−10)**, aligned **841 -> 864 (+23)**, size exact
17,172/17,172, `.ARM.extab` unchanged (577 differing bytes, size/base exact
at `0x1a3b78`), `.text` diff 71,247 -> **71,237** (−10), `.ARM.exidx` 0,
`.rodata` 0.  All section sizes unchanged.  `check` green: byte_exact 4508,
linked_exact 4508, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370, `RATCHET PASS`.

## 1. The +10 is exactly the 6-byte shift of the second rotation block

w13650's interleave moved `setRotation` from after the second rotation to
between the two rotations.  The only bytes it changed relative to the
pre-interleave base are 10 bytes at `0xb3320..0xb3333` — the second
`MatrixRotateVector`/`Vector::operator=` block, which shifts from
`0xb330a..0xb3328` (base, matching the original's `blx` addresses
`0xb3320`/`0xb3328`) to `0xb3312..0xb3330` (interleave, 6 B later than the
original's `0xb330c`).  Everything else in the function is byte-identical to
base.  So the recovery has to come from inside this loop body.

The `(bigAlienWave + (i < 0) + (i < 0))` size crutch is load-bearing and
cannot be shortened: every single-term form gives 17,168 B and a different
(global) register allocation.  The crutch's select is 6 B longer than the
binary's `cbz`, and the `ldrd`/`strd` Vector construction is 2 B shorter;
the net +6 B is what shifts the block.

## 2. Store/statement order is the compensation lever

The +10 is recovered by reordering the independent setters so the compiler
re-schedules the loop body into the original's byte layout.  The landed
order is:

```cpp
for (...) {
    KIPlayer *gun = (*this->enemies)[i];
    (*this->enemies)[i] = this->createStaticObject(nullptr, 6772, true);
    const float (*t)[6] = (bigAlienWave + (i < 0) + (i < 0))
                              ? kAlienStationTurrets : kAlienOutpostTurrets;
    gun->cargo = nullptr;                       // was after the 2nd rotation
    v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
    gun->geometry->setRotation(v);              // semantic interleave kept
    v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
    gun->player->setMaxHitpoints(1000);         // was before the select
    gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
    ((PlayerTurret *) gun)->setScaling(6.0f);
    gun->shipGroup = localRace;
}
```

`cargo`, `setMaxHitpoints`, `setRotation`, `setPosition`, `setScaling` and
`shipGroup` all write independent fields of the freshly created turret; the
reorder is semantically neutral (final state identical), and the
rotation/setPosition pairing — the actual w13650 semantic fix — is preserved.

## 3. Measured table (all size 17,172 unless noted)

Starting from the landed interleave (raw 2,636 / aligned 841), sweeping
`cargo` gap × `setMaxHitpoints` gap × `{setPosition,setScaling,shipGroup}`
order × `gun` before/after `createStaticObject`:

| variant | raw | aligned |
|---|---:|---:|
| landed (cargo after 2nd rotation, S1 before select) | 2,636 | 841 |
| cargo after `setRotation`, S1 before select (`S4_PSG`) | 2,635 | 1,022 |
| **cargo first, S1 after 2nd rotation (landed)** | **2,626** | **864** |
| cargo first, S1 after 2nd rotation, `gun` after store | 2,627 | 863 |
| cargo first, S1 after 2nd rotation, PSG permuted | 2,628–2,635 | 864–871 |
| cargo first, S1 before 2nd rotation | 2,803 | 1,053 |
| cargo first, S1 before select | 2,640 | 1,022 |

The 2,626 raw result is insensitive to the `t` declaration position, the
`gun`/`create` order (2,626 vs 2,627), `Vector v` init form, `for`/`while`
loop form, `++i`/`i++`, the `setPosition` argument form (inline `Vector{...}`
vs a named `Vector p`), and `const Vector &base` vs a copy.  All of those
tie at **2,626 / 864**.  The aligned cost is the price of the statement
reorder: the loop body's instruction order now differs from the binary's
more than the landed shape's did, even though the byte-level diff is 10 B
smaller.  The whole-`.text` diff is the primary binary-level metric and it
improves by 10 (71,247 -> 71,237); `.ARM.extab` is unchanged.

Other shapes measured on top of the landed interleave (no size-exact raw
win): plain `bigAlienWave` ternary (17,168, raw 4,371); element-wise
`Vector` construction (17,188, raw 4,305); separate `Vector v2` for the
second rotation (17,168, raw 3,863); direct
`setRotation(MatrixRotateVector(...))` (17,168, raw 3,854); `base + v`
(17,140, raw 4,457); per-component float extraction (17,164, raw 4,245);
duplicate `cargo` store (raw 2,937–2,941); `__builtin_expect` on the select
(neutral); every `(i < 0)` crutch spelling that is not the two-term add
(17,168 or 17,172 with raw >= 2,946).

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13680
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=864 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13680/`
(`measure.py`, `harness.py`, `mkwin.py`, `sweep_full.py`, `try*.py`,
`base.so`, `win.so`, `Level.base.cpp`).
