# Gun layout drift (+4 from ~0x38 up) — exclusive-mode fix

Evidence (original `_ZN3GunC2EiiiiiifN...VectorES2_` @0x151f20, 0x214B):

- 0x152024: `blx _Znaj; str r0,[r4,#0x3c]` — first `new[]` result
- 0x15202e: `blx _Znaj; str r0,[r4,#0x40]` — second `new[]` result
  Our Gun.cpp allocates `lifetimes` then `hitFlags = new uint8_t[count]`;
  our layout puts them at 0x38/0x3c. Original wants 0x3c/0x40 → everything
  from lifetimes (ours 0x38) onward is shifted +4 vs our header; a 4-byte
  field is missing (or a field is 4B too small) somewhere at/below 0x38.
- MineGun render/update independently blocked on the same delta:
  orig `ldr r0,[r0,#64]` for the hitFlags pointer, ours emits `[r0,#60]`.
- Other original ctor field writes to map during the fix (r9 = r4+124):
  zeroed early: [r4+124], [r4+128..132], [r4+144..148], [r4+152],
  [r4+56..60], [r4+180..184], [r4+264..268]? (recheck strd at [r9,#36]),
  count-ish: r8→[r4,#120], sl→[r4+96]? (`str.w sl,[r9,#-28]` = r4+96),
  vstr s0,[r4+80]; strd r2,r1,[r4,#68]; str r1,[r4,#108];
  str 0,[r4,#160]; strb 0,[r4,#136]; strb 0,[r4,#169];
  strd r3,(r3<<1),[r4,#116]; Array<Vector*> ptr → [r4,#172].
- Gun dtor orig @0x152134 (0x78B) — use for destruction-order cross-check.

Scope: Gun.h field shift affects all subclasses' inherited-field access
(MineGun, BeamGun, SpriteGun, ObjectGun, ...) and every TU including
Gun.h. Gun's own big fns (shoot/shootAt at 0.0%) are ALSO gated by the
AEMath::Matrix 0x40-vs-0x3c mis-size (see transform-layout-mismatch
memory) — fix Matrix first or at least be aware pct won't fully recover
from the Gun.h shift alone.

Plan (exclusive, fleet drained): re-derive the full Gun field map from
ctor+dtor+accessor disasm (the 100% accessors pin many offsets already:
getEnemies/setMagnitude/... — dump and map each), edit Gun.h once,
verify-gate, per-fn regression check across all Gun-family TUs.
