# Radar::draw wave 149 (w9605) — the .text tail is 467 ARM veneers, not libgcc; the fixed 0x40 pad and the 9908 String shape

**Status:** analysis note, no source change landed (gate green at HEAD `7eed2966e`).
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`.

Read with `radar-draw-wave149-w9501.md`, `radar-draw-wave148-w9401.md`,
`radar-draw-wave147-w9309.md`.

Baseline (HEAD, unchanged): original 9908 B; ours 9908 B; raw same-address diff
**8,712**; instruction-index alignment **1,657**. `.text` 86,449 differing bytes;
`.ARM.extab` 1,309 (size exact 156,312); `.rodata` 0x1c9e10.

## 1. The tail is NOT libgcc data — it is 467 linker-generated ARM veneers

w9501 described the tail as "`__aeabi_idiv0` (12 B) then a 7,536 B data blob".
That is wrong. The 7,536 B is **467 16-byte ARM interworking veneers** plus a
**fixed 0x40-byte zero pad**:

```
0x19ae8c  __aeabi_idiv0            12 B   (libgcc _dvmd_lnx.o, last libgcc member)
0x19ae98  467 × 16-byte veneers    7,472 B
0x19cbc8  0x40 zero pad            64 B
0x19cc08  .ARM.exidx starts
```

Each veneer is gold's `elf32_arm_stub_long_branch_v4t_thumb_arm_pic`:

```
4778 46c0   bx pc ; nop          (Thumb)
e59fc000    ldr ip, [pc]         (ARM)
e08cf00f    add pc, ip, pc       (ARM)
xxxxxxxx    dcd R_ARM_REL32(X)    (ARM)
```

All 467 veneer targets are distinct `.plt` entries. **The whole tail from
0x17f1b6 to 0x19cc08 is byte-identical to the original** (last differing byte in
`.text` is 0x17f1b6), so there is no libgcc-member difference to exploit: our
`_aeabi_ldivmod.o` (0x38 B) and `_dvmd_lnx.o` (0xC B) are exactly the original's.

Link map (gold `-Map`) confirms the tail order:
`... cxa_demangle.o ... posix_memalign.o (0x3e) _aeabi_ldivmod.o (0x38)
_dvmd_lnx.o (0xc)` then the stub table owned by `_dvmd_lnx.o`'s `.text`.

## 2. Why a veneer exists: only `b.w` tail calls need one

Scanning the original: **all 921 references to the veneer range are `b.w`**
(no `bl`/`blx`). A Thumb `blx <imm>` can switch to ARM directly and needs no
veneer; a Thumb `b.w` (tail call) cannot, so gold emits one veneer per distinct
PLT target that is tail-called. 301 of the 467 veneers have exactly one call
site. Imports called only with `blx` (e.g. `strlen`, `memcpy`) have no veneer.

## 3. The 0x40 pad is fixed (measured)

Experiment: retarget `Item::isInList(Item*, ItemArray*)` from
`isInList(int, ItemArray*)` to `isInList(int, 1, ItemArray*)`
(`src/game/mission/Item.cpp`, +4 B of code). Result:

| | veneers | first veneer | last veneer | .text size | pad after veneers |
|---|---:|---:|---:|---:|---:|
| HEAD | 467 | 0x19ae98 | 0x19cbb8 | 0x1337c8 | 0x40 |
| retarget | 466 | 0x19aeb8 | 0x19cbc8 | 0x1337d8 | 0x40 |

So: one fewer veneer = exactly −16 B of stub table, and the trailing pad stays
**0x40**. But the +4 B of code before the veneers quantised badly: the
`OPENSSL_atomic_add` input section is 32-byte aligned, and +8 B of accumulated
code growth crossed the 0x18fe40 boundary, adding +24 B of inter-section pad.
Net `.text` **+16**, not −12.

Consequence: a **size-neutral** veneer removal (same code bytes, one fewer
tail-call target) would shrink `.text` by exactly 16 B and is the clean lever.
Every tail call clang emits is a `b.w`; `bool r = f(); return r;` and
`return f() ? true : false;` still tail-call at `-Oz` (measured on
`Item::isInList`). A `volatile bool r` forces `blx` but grows the 6 B function
to 52 B. A same-signature retarget that is byte-neutral was not found in the
time box.

## 4. The 9908 String shape on the CURRENT HEAD (new measurement)

w9501's 9908/8549/2103 String shape was measured on the wave-147 base. On the
current wave-148 HEAD the same forms give **9896** (assign) / **9892**
(copy-init), not 9908. Sweeping the non-witnessed `bool` locals recovers 9908:

| variant (current HEAD + String copy) | usize | raw | align | extab |
|---|---:|---:|---:|---:|
| HEAD | 9908 | 8712 | 1657 | 156312 |
| `String c = lockLabel;` (copy-init) | 9892 | 8705 | 1805 | 156324 |
| `String c; c = lockLabel;` (assign) | 9896 | 8706 | 1805 | 156324 |
| **copy-init + `int wreck` (enemy loop)** | **9908** | **8678** | **1792** | **156324** |
| assign + `int wreck` | 9912 | — | — | 156324 |

`wreck` (enemy-loop, non-witnessed; the asteroid-loop `wreck` is already `int`)
is the only single flip that lands 9908. It is a **raw win (−34) but an aligned
regression (+135)** over HEAD, and it cannot land alone: `.ARM.extab` +12 pushes
`.rodata` to 0x1c9e20, so `byte_exact` is **4044** (measured) until `.text`
shrinks 12..28.

## 5. Exact arithmetic / levers for the next pass

`.rodata` is 16-aligned. With the String shape (extab 0x262a4) `.rodata` lands
at 0x1c9e10 iff `.text` ends in `(0x19cbf4, 0x19cc08]`, i.e. `.text` −12..−28.

* **Best lever:** a byte-neutral veneer removal (−16) → `.rodata` 0x1c9e10,
  `byte_exact` back to 4507 with the 9908 String shape.
* **Alternative:** drop `_dvmd_lnx.o` (`__aeabi_idiv0`, exactly 12 B). It is
  pulled in by `_aeabi_ldivmod.o` (`__aeabi_ldiv0`), which in turn is pulled in
  by `Transform.cpp` (`__aeabi_ldivmod`), so it is not removable without
  changing the 64-bit division in `Transform.cpp` — which the original also has.
* `posix_memalign` (0x3e = 62 B) is pulled in by libc++abi
  `fallback_malloc.o`; not a 12..28 shrink.

## 6. Gate (unchanged, note-only)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
