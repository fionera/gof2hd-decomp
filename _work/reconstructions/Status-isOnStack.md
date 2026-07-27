# `Status::isOnStack(Station*)`

## Target

- Symbol: `_ZN6Status9isOnStackEP7Station`
- Original Thumb entry: `0x0a9070` (dynsym value `0x0a9071`)
- Original size: 62 bytes
- Current size: 68 bytes
- Current report score at analysis time: 90.2%
- Original TU witness: `Classes/Game/Status.cpp`

## Proven signature error

The current declaration returns `bool`:

```cpp
bool Status::isOnStack(Station *station);
```

The original source return type was `Station *`. Return types do not participate
in this Itanium mangled name, so both declarations produce the same exported
symbol.

The loop body is already structurally identical. Both versions:

1. preserve the requested `Station *` in `r5`, `this` in `r4`, and index in
   `r6`;
2. iterate indices 0 through 2;
3. load `this->stationStack` at `this+0x1a0`, its data pointer at `+4`, and
   element `data[index]`;
4. skip null elements and call `Station::equals(Station*)` for non-null
   elements;
5. return zero after exhausting the three entries.

The sole structural tail difference occurs after `equals` succeeds. Original
instructions at `0x0a909e` reload the matching element:

```text
ldr.w r0, [r4, #0x1a0]
ldr   r0, [r0, #4]
ldr.w r0, [r0, r6, lsl #2]
```

The original then immediately restores registers and returns that raw pointer
in `r0`. The current function performs the same reload but must normalize it
for a C++ `bool` return:

```text
cmp   r0, #0
it    ne
movne r0, #1
```

Those normalization instructions account for the current six-byte size
excess. No honest `.cpp`-only expression can remove them: every pointer or
integer returned from a function declared `bool` undergoes mandatory Boolean
conversion. Casts or object-representation tricks would either retain the
conversion or introduce ABI-unsafe undefined behavior.

## Independent source evidence

DeepOpen's mapped J2ME method has the signature and behavior:

```java
private static Station isOnStack(Station station) {
    for (int i = 0; i < 3; ++i) {
        if (lastVisitedStations[i] != null
                && lastVisitedStations[i].equals(station)) {
            return lastVisitedStations[i];
        }
    }
    return null;
}
```

The mapping records `isOnStack` as `(Station) -> Station`. This agrees with the
Android binary's raw pointer return and is used only as supporting evidence;
the Android disassembly is authoritative.

## Current callers and semantic impact

Current callers in `Status.cpp` include:

- a Boolean condition in the station-stack path, which remains valid after a
  pointer return because a pointer is contextually convertible to `bool`;
- `departStation`, which currently reconstructs:

```cpp
bool wasOnStack = isOnStack(dest);
Station *prev = isOnStack(dest) ? dest : 0;
```

This calls the method twice and substitutes `dest` for the actual matching
stack element. The original source shape, corroborated by DeepOpen, retains
the returned object:

```cpp
Station *prev = isOnStack(dest);
bool wasOnStack = prev != nullptr;
```

All callers must be audited from the original Android disassembly during the
signature pass. Conditions may stay as pointer truth tests, but callers that
need the matched station must preserve the returned pointer rather than
reconstructing it from the argument.

## Required exclusive change

This is a coordinated signature/caller pass, not an ordinary body-only edit:

1. Change `Status.h` to:

   ```cpp
   Station *isOnStack(Station *station);
   ```

2. Change the definition to return `(*stationStack)[i]` on a match and
   `nullptr` on exhaustion.
3. Update every caller in `Status.cpp` according to its original use,
   especially `departStation`, so the returned matching station is retained.
4. Do not change `Status`, `Station`, `Array`, or `String` layout; the field
   loads already prove the current `stationStack` offset and Array data
   representation for this function.

Because the return type is absent from the mangled name, no exported-symbol
rename is expected. Nevertheless, the header change affects every compiled
caller and must be treated as exclusive.

## Constraints and requeue plan

- Drain concurrent work before editing `Status.h` or `Status.cpp`.
- Do not combine this pass with Status layout, TU membership, global
  provenance, String temporary, or notification-hook work.
- Do not use casts to bypass the declared return type.
- Inspect all call sites in the original binary before authoring the caller
  changes.
- Build and directly verify `Status::isOnStack` plus each changed caller.
- Run the full link, verify, symbol-parity, lint, drift, and ratchet gates.
- Confirm the target shrinks from 68 to 62 bytes and becomes linked/byte exact
  without regressions in the large, codegen-coupled `Status.cpp` TU.
- Update the verification baseline and campaign counts only after the full
  gate passes.
