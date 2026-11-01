# .text layout drift — the dominant byte_exact gap

State at `cfd7e87d` (+ `tools/text_drift.py`):

```
byte_exact 1513 / 4523      linked_exact 4492      avg 99.89
.text 0x1337b0 (orig 0x1337c8, -24)   .ARM.exidx 0x6f88 (orig 0x6f70, +24)
```

Of the 2979 functions that are linked-exact but not byte-exact, **almost all are at the
wrong .text address**. A shifted function's PC-relative literal-pool words differ by exactly
the shift (example: `MTitle::OnRelease` @0x97a2c vs 0x97a5c, every pool word differs by
0x30, every instruction identical). So byte-exactness is gated on the *layout*, not on the
source shape.

## Where the drift comes from

`tools/text_drift.py` compares the gap (local code + alignment) between consecutive exported
functions. Exported-function size mismatches in the game region are only three:

| address | function | orig | ours |
|---|---|---|---|
| 0x6db20 | `AbyssEngine::getAppVersion()` | 0 | 2 |
| 0x971b8 | `Generator::createMission(Agent*, Array<SolarSystem*>*)` | 1996 | 1992 |
| 0x147e24 | `HangarWindow::initialize()` | 3052 | 2984 |

Everything else is **local (static) functions whose size is off**. The drift is cumulative:

| original address | cumulative drift | cause |
|---|---|---|
| 0x69578 | 0 | start |
| 0x6db20 | +4 | `getAppVersion` +2, local +2 |
| 0x6e918 | 0 | EaseInOutMatrix ctor gap -4 |
| 0x7efcc | +16 | PaintCanvas TU (after `WarmUpTexture`) |
| 0x81008 | +14 | MatrixDebugOut gap -2 |
| 0x8103c | +12 | Quaternion::Quaternion(Matrix) gap -2 |
| 0x81818 | +52 | GameText TU (+40 after `setSubstituteArray`) |
| 0x83ee8 | +50 | glError gap -2 |
| 0x83eec | +52 | Engine ctor gap +2 |
| 0x84348 | +56 | AfterGLInit gap +4 |
| 0x87672 | **+48** | ConfigReader gap -8 |
| 0x148a10 | -28 | `HangarWindow::initialize` -68 (w3273) |
| 0x157040 | -24 | local +4 |
| 0x175e08 | -10 | ... |
| 0x18f590 | -40 | libzip plain-`.text` block |
| 0x191d20 | +48 | plain `.text` group (libcrypto/libgcc/libc++abi) |

**1269 exported functions sit in the +48 block** (0x87672 .. 0x147e24) — i.e. most of the
game code. Killing the +56 of early local-code drift (PaintCanvas +16, GameText +40) and the
-8 ConfigReader gap puts that whole block at drift 0 and should flip ~1200 functions to
byte-exact in one go.

## Second, independent problem: the plain `.text` group

`tools/gen_section_order.py` places the plain `.text` group with a tuned `-0x180`
(`PLAIN_TEXT_LEAD`) hack, which lands it *between* `zip_stat_init` and `zip_fopen` (9804 B
of other code in between) instead of at the original's libcrypto start (0x18f730, right
before `SHA224_Init`). The comment says our plain block is 0x44 bytes larger than the
original's. Fix the block size first, then drop the hack. ~178 third-party exports are
currently non-byte because of this.

## Third: delta-0 non-byte functions

366 exported functions sit at the correct address and are linked-exact but not byte-exact —
their literal pool entries are in a different order inside the function. These are genuine
source-shape differences (constant materialisation order), not layout.
