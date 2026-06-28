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
