# Player::ResumeEngineSound(bool) — deferred scheduler residual

Symbol: `_ZN6Player17ResumeEngineSoundEb`

Current committed metric after wave 62: 90.0%, original 52 bytes, ours 52
bytes, not linked. Original address: `0xa40ac`. The body is semantically
reconstructed; the only structural mismatch is the point where clang preserves
`this` in `r4`.

## Stuck diff

```text
orig                                      ours
mov     r2, r1                            mov     r2, r1
ldr.w   r1, [r0, #240]                    ldr.w   r1, [r0, #240]
mov     r4, r0                          | cbz     r1, ...
cbz     r1, ...                         | mov     r4, r0
ldrb.w  r0, [r4, #248]                  | ldrb.w  r0, [r0, #248]
```

Everything after this is structurally identical: the paused/force tests,
`Globals::sound` load, `FModSound::resume(event)`, `eor r0,#1`, and the byte
store at `Player+0xf8`. Link and literal-pool words differ only by relocation.

Verified offsets:

- `Player::engineEvent`: `+0xf0` / 240
- `Player::enginePaused`: `+0xf8` / 248

## Source shapes tested

All three variants compiled to the same 52-byte function and the same 90.0%
diff under the pinned NDK r18b clang `-Oz` toolchain:

1. Baseline cached `FMOD::Event *event`, flattened compound condition.
2. Direct `this->engineEvent` accesses in the condition and resume call.
3. Cached event with nested event-null and paused/force guards.
4. Cached event with a named `bool resumed`, followed by
   `this->enginePaused = !resumed`.

The last form is also consistent with Ghidra's read-only decompilation, but it
does not alter code generation.

## Hypothesis / requeue condition

LLVM's scheduler sinks the `r4 = this` copy until after the early null return
because `this` is not otherwise needed on that path. The original allocator
made the equally valid eager copy. No natural semantic source lever found in
the binary or sibling `PauseEngineSound`/`StopEngineSound` bodies changes that
choice. Do not retry with dummy reads, `volatile`, artificial escapes, opaque
helpers, or other liveness forcing. Requeue only if a broader, genuine source
change in `Player.cpp` changes register allocation or if stronger original
source evidence appears.
