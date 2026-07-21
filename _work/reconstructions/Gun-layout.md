# Gun layout 0x38–0x4c span — RESOLVED (intra-span reorder, applied)

Initial hypothesis ("+4 drift from ~0x38 up") was WRONG: total size and every
offset outside 0x38–0x48 were already correct (accessors byte-exact, sentinel
static_asserts at 0x54+ all held). The real defect was a 3-field reorder
inside the span.

Evidence (original `_ZN3GunC2EiiiiiifN...VectorES2_` @0x151f20 0x214B,
dtor @0x152134 0x78B):

- ctor 0x152024/0x15202e: two `blx _Znaj` results stored to [r4,#0x3c] and
  [r4,#0x40] — our Gun.cpp allocates `lifetimes` (int[]) then `hitFlags`
  (uint8_t[]) in that order → lifetimes@0x3c, hitFlags@0x40.
- dtor: `ldr r0,[r0,#60]; cbz; blx _ZdaPv` (NULL-CHECKED) then
  `ldr r0,[r4,#64]; blx _ZdaPv` (unchecked) — matches our dtor's
  checked-then-unchecked delete order → confirms 0x3c/0x40 assignment.
- `strd r2,r1,[r4,#68]` + `str r1,[r4,#108]` ↔ our
  `initialLifetime=p5; fireDelay=p6; timer=p6` → initialLifetime@0x44,
  fireDelay@0x48.
- 0x38 is only ever ZEROED by the ctor (early strd batch zeroes 0x38/0x3c);
  greps show `field_0x48` (old name) was unused in Gun-family code — the
  only hits were Layout/HangarWindow/JniBridge (different classes).

Fix applied (Gun.h, header-only): old span
`{int* lifetimes@0x38; uint8_t* hitFlags@0x3c; int initialLifetime@0x40;
int fireDelay@0x44; int field_0x48;}` reordered to
`{int field_0x38; int* lifetimes@0x3c; uint8_t* hitFlags@0x40;
int initialLifetime@0x44; int fireDelay@0x48;}` + static_asserts.
No size change (Gun stays 0x114; geometries@0x10c, randomFlags@0x110).

Result (gated): linked 2226→2227 (MineGun::render → linked-exact),
Gun D1/D2 83.3→92.9, Player::refillGunDelay 86.4→90.9, Gun::update +2.3,
Player::shoot +0.8. No regressions.

Residual follow-ups:
- Our ctor never zeroes `field_0x38` (orig zeroes 0x38 in the early strd
  batch) — add `this->field_0x38 = 0;` to Gun.cpp ctor, gate-check.
- Gun::shoot/shootAt remain ~0% — ALSO gated by the AEMath::Matrix
  0x40-vs-0x3c mis-size (see transform-layout-mismatch memory); the Gun.h
  fix alone cannot recover them. Matrix re-size is the next exclusive item.
