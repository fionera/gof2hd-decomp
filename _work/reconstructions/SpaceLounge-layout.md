# SpaceLounge layout dossier (deferred exclusive item)

Evidence gathered 2026-07-22 (wave-9 session) from the original binary.

The wave-7 worker note "listVisible 0x1f vs 0x1c" understates it — the flag block is
relocated, not reordered:

- Orig `listMode()` @0x1714ea is `ldrb.w r0,[r0,#52]; clz; lsrs #5` → returns
  `field_0x34 == 0`. Our listMode reads listVisible@0x1f. So the list flag the
  original tests lives at **0x34** (where our header has mapVisible).
- A scan of all SpaceLounge fns for `ldrb/strb [rN,#28..31]` finds accesses ONLY at
  **#28 (0x1c)** — never 0x1d/0x1e/0x1f. Orig therefore keeps ONE byte flag at 0x1c
  and the rest of chatActive/popupActive/choiceVisible/listVisible are elsewhere
  (0x34+ region is the first candidate; our chatScroll/chatAnswer/mapVisible block
  must shift accordingly).
- Consequence: our 4-byte flag pack at 0x1c-0x1f is wrong as a block; fixing it means
  re-deriving the whole 0x1c..0x3c span from ctor stores + accessor offsets
  (SpaceLounge() ctor init stores are the fastest source).

Fix procedure (exclusive-mode): objdump the SpaceLounge ctor + the 6 small accessors,
build the offset map for 0x1c..0x40, rewrite SpaceLounge.h in one pass, adjust
SpaceLounge.cpp field names, verify-gate. ~34 SpaceLounge fns currently in the
sub-60% band stand to gain.
