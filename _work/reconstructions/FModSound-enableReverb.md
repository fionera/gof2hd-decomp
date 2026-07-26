# FModSound::enableReverb

Symbol: `_ZN9FModSound12enableReverbEi`

Status: typed partial prepared; exactness deferred after two retries and an
independent escalation because of one LLVM MachineSink copy.

## Typed reconstruction

The original 88-byte local frame contains an 80-byte
`FMOD_REVERB_PROPERTIES` object at `sp+0`, `int presetCount` at `sp+80`, and
the stack canary at `sp+84`. The matching source declaration order is:

```cpp
int presetCount;
FMOD_REVERB_PROPERTIES properties;
```

Using those typed locals removes the current raw `char[0x54]` representation
and offset casts. With declarations in this order, the function remains 116
bytes and every stack address, FMOD call, guard, store, and epilogue matches.

## Remaining residual

The original preserves `p1` in `r4` before loading/checking `this->system`.
Current LLVM sinks `mov r4, r1` to immediately after the system-null `cbz`.
`p1` has no use on the null-return edge, so aliases, references, declaration
order, nested positive paths, early returns, common epilogues, lambdas, and
scopes all reduce to the same SSA liveness.

Preventing the sink requires an observable false-edge use, volatile/atomic
state, an opaque call/destructor, assembly/attributes, extra surviving CFG, or
undefined behavior. Those choices are prohibited or change the function.

Land the typed reconstruction only if the full ratchet accepts it as a
non-regressing partial. Requeue byte exactness only if a legitimate source or
compiler change makes `p1` live on both successors.
