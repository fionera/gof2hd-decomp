# Hud::draw — wave 176 (w12019): address-store anchor (raw 156 -> 150)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`, 77 LSDA rows (`.ARM.extab` 0x26298).

| metric | before (wave 175) | after |
|---|---:|---:|
| function raw bytes | 156 | **150** |
| aligned | 145 | **143** |
| size | 9,332 | 9,332 |
| whole-`.text` diff | 80,166 | **80,160** |
| whole-`.ARM.extab` diff | 746 | 746 |
| `.rel.plt` diff | 0 | 0 |

Gate after the change: `check` PASS — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370, `.rel.plt` byte-identical (0).

Per-region raw split (regions: prologue 0x163b90, damage 0x16412c, mission
0x1643cc, dock1 0x165700, dock2 0x165880, rate 0x165b50, show 0x165ba6,
tail 0x165d4c):

| region | before | after |
|---|---:|---:|
| prologue | 0 | 0 |
| damage | 25 | **19** |
| mission | 0 | 0 |
| dock1 | 0 | 0 |
| dock2 | 130 | 130 |
| rate | 0 | 0 |
| show | 1 | 1 |
| tail | 0 | 0 |
| sum | 156 | **150** |

## Landed source shape

One change in the damage-indicator block.  The old anchor was a volatile
value read:

```cpp
if (hitFlags & 0x24)
    (void) *(Status * volatile *) &Globals::status;
    this->field_0x4b8 = 300;
```

The new anchor is a dead volatile pointer-to-pointer initialization:

```cpp
if (hitFlags & 0x24)
    Status **volatile p = &Globals::status;
    this->field_0x4b8 = 300;
```

`p` is unused; the declaration is a codegen anchor.  The `field_0x4b8`
store remains unconditional exactly as in the previous shape, so runtime
behaviour is unchanged.

### Why it wins

The old anchor emitted a conditional volatile **value** load (`ldrne r0,[r8]`)
and, because that load was the entire `if` body, left the `field_0x4b8` store
unconditional:

```
164184  tst.w   r1, #36
164188  it      ne
16418a  ldrne.w r0, [r8]          ; volatile value load
16418e  tst.w   r1, #24
164192  mov.w   r0, #300
164196  str.w   r0, [sl, #0x4b8]
16419a  it      ne
16419c  strne.w r0, [sl, #0x4b4]
```

The new anchor is a conditional 4-byte **store of the address** to the
stack slot reserved for `p`:

```
164184  tst.w   r1, #36
164188  mov.w   r0, #300
16418c  it      ne
16418e  strne.w r8, [sp, #0xd0]   ; dead volatile address store
164192  str.w   r0, [sl, #0x4b8]
164196  tst.w   r1, #24
16419a  it      ne
16419c  strne.w r0, [sl, #0x4b4]
```

The value load is gone, and the 4-byte store keeps the function at 9,332 B.
The remaining damage residual is the original's conditional
`movne`/`strne` pair versus our unconditional `mov`/`str` plus the dead
address store.

## Anchor sweep (all measured on the wave-175 base)

| anchor body (`if (hitFlags & 0x24) <body>; this->field_0x4b8 = 300;`) | raw | aligned | size |
|---|---:|---:|---:|
| **`Status **volatile p = &Globals::status;` (landed)** | **150** | **143** | **9,332** |
| `volatile int x = (int) (uintptr_t) &Globals::status;` | 150 | 143 | 9,332 |
| `volatile unsigned int x = (unsigned int) (uintptr_t) &Globals::status;` | 150 | 143 | 9,332 |
| `volatile long x = (long) (uintptr_t) &Globals::status;` | 150 | 143 | 9,332 |
| `volatile unsigned long x = (unsigned long) (uintptr_t) &Globals::status;` | 150 | 143 | 9,332 |
| `volatile short/char x = (short/char) (uintptr_t) &Globals::status;` | 150 | 143 | 9,332 |
| `Status **const volatile p = &Globals::status;` | 150 | 143 | 9,332 |
| `Status *volatile *volatile p = &Globals::status;` | 150 | 143 | 9,332 |
| old `(void) *(Status * volatile *) &Globals::status;` | 156 | 145 | 9,332 |
| `Status *volatile p = Globals::status;` | 1,029 | 209 | 9,332 |
| `(void) (Status *volatile) Globals::status;` | 7,498 | 954 | 9,320 |
| `(void) &Globals::status;` | 7,498 | 954 | 9,320 |
| braced `{ Status **volatile p = &Globals::status; this->field_0x4b8 = 300; }` | 7,091 | 450 | 9,336 |
| `volatile long long x = (long long) (uintptr_t) &Globals::status;` | 7,092 | 457 | 9,336 |
| `volatile float/double x = (float/double) (uintptr_t) &Globals::status;` | 8,822 | 1,454 | 9,276 |

The address-store family is flat at raw 150; the exact spelling does not
matter.  The old value-read anchor is the only 156 shape.  Braced forms make
the `field_0x4b8` store conditional and cost 4 B (9,336); removing the
`short frameH`/`short textH` sxth to compensate re-colours the dock block and
is much worse (6,264 raw at 9,332).

## Remaining residual (150 raw)

* damage 19: conditional address store + unconditional `field_0x4b8` store
  versus the original's conditional `movne`/`strne`;
* dock2 130: register allocation (`barW`/`barH`/`Canvas`/`top`/`barYOffset`
  rotation) unchanged from wave 175;
* show 1: the single literal-pool addend byte for `Globals::gameText`
  (`.got` entry-order artifact).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
# raw=150 aligned=143 size=9332/9332 insn=3227/3226
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY
```
