# Union audit: collapse "two identical fields, different names" — keep genuine type-puns

Working dir: /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp. The codebase matches the original
libgof2hdaa.so at the symbol level — symbol parity MUST be preserved (field names are not symbols, so
renaming fields is parity-safe, but the whole tree must still COMPILE and offsets/static_asserts must
not change).

For EACH `union { ... }` in your assigned file, classify and act:

## COLLAPSE (the members are the SAME field with different names)
Criteria — ALL members are the same underlying value, not a reinterpretation:
- same type (e.g. all `int`, all `uint8_t`, all `Foo*`), OR same-size integers differing only in
  signedness (`int32_t`/`uint32_t`) used for one value;
- typically one member is a decompiler placeholder `field_0xNN` and the other is descriptive, OR both
  are descriptive but mean literally the same thing.

Action:
1. Keep ONE name — prefer the descriptive one over `field_0xNN`; if both descriptive and identical in
   meaning, keep the clearer / more-used one.
2. Delete the `union { ... }` wrapper, leaving the single kept member (same type, same offset).
3. Rename EVERY use of the dropped name(s) across all of `src/` to the kept name.
   **CRITICAL**: `field_0xNN` names are reused across unrelated classes. Before rewriting a
   `x->field_0xNN` / `x.field_0xNN` hit, verify the receiver `x` is actually THIS class (check the
   declared type / the cast). Do NOT touch same-named fields on other classes.
4. If a kept member's signedness differs from a dropped one, add a cast at the few sites that need it
   rather than keeping the union.

## KEEP (genuine type-pun — leave the union, ensure a same-line `// lint: union_decl <reason>` waiver)
The members reinterpret the SAME bytes as DIFFERENT types/representations:
- `int` vs `float` (bit reinterpret);
- a wide field (`int`/`uint16_t`) overlaid with a `struct { uint8_t ...; }` of named bytes;
- an array (`float[15]`) overlaid with a typed struct/`Matrix`;
- a pointer overlaid with an integer handle or with flag bytes;
- members of DIFFERENT sizes (`uint16_t` vs `uint8_t`).
These model the binary's real memory layout and cannot be unified by renaming. The waiver goes on the
`union {` opening line (the linter waives a criterion named as a word in a `// lint:` comment within
+-2 lines).

Do NOT run the build (the orchestrator rebuilds + checks symbol parity centrally). Report, per union:
collapsed (kept name + dropped names + #cross-file renames) or kept-as-type-pun (one-line reason).

## Follow-up: is each union a DECOMPILER ARTIFACT or a GENUINE binary overlay?

DECOMPILER ARTIFACT (FIX — collapse to the one real field, rewrite the few divergent uses):
- A real COMPOSITE type (Vector / Matrix / a named struct) overlaid with scalar/component views that
  exist ONLY to bulk-init/zero it — e.g. `union { Vector v; struct { int vX, vY, vZ; }; }` where vX/
  vY/vZ are only ever set to 0. The int-bit zero equals the float 0.0f, so the component struct is a
  decompiler view for "zero the Vector". FIX: drop the scalar view, keep the composite, rewrite the
  init to the composite (`v = Vector();` / `v.x = v.y = v.z = 0;` / memset).
- A named struct (the real, full-width type) shadowed by a redundant scalar sub-view of its first
  member (the Blk16 case) — keep the struct, rewrite the scalar use as `.member`.

GENUINE OVERLAY (KEEP, with an accurate same-line `// lint: union_decl` reason):
- int<->float where BOTH the integer value AND the float value are genuinely computed/used as their
  types (not merely zeroed): empPoints/empPointsF, rank/rankBits, touchX/touchXf, fixed-point, etc.
- a flag WORD (`int`/`uint16`) that is cleared/compared as a word AND whose individual named flag
  BYTES are set/read — both access widths are real (faithful overlay).
- a pointer slot reused as an int handle or flag bytes in a different object state.
- an equal-size raw-array vs typed-class view where BOTH are load-bearing and the typed side is
  layout-sensitive (Matrix) — collapsing is high-risk; keep.

When in doubt, KEEP and say why. Verify the receiver type before any cross-file rename (field_0xNN /
component names are reused across classes). Do NOT run the build — the orchestrator rebuilds + checks
symbol parity centrally. Report per union: artifact (what you did) or genuine (one-line reason).
