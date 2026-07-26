# `PlayerEgo::~PlayerEgo` residual

## Current result

Normal deletion of `easeMatrix @ +0x358` replaced the explicit destructor plus
`::operator delete(field)` source shape. D1/D2 improved from 96.3% to 99.5%.
The original and current now have identical reachable executable instructions
modulo link addresses: the `EaseInOutMatrix` destructor result flows directly
to operator delete, with no redundant reload.

The original is 338 bytes; current is 334. The sole remaining instruction is
an unreachable post-return call at original `0x9a1f2` to helper `0x6dfc4`,
whose body calls `__cxa_begin_catch` and then `std::terminate`. It is a
destructor exception landing pad.

## Exhausted exception-spec retries

- Removing the reconstructed `noexcept(false)` and using implicit exception
  specification did not add the handler because inherited contracts still
  permitted throwing.
- Explicit `PlayerEgo::~PlayerEgo() noexcept` restores the recovered public
  contract but Clang still marks the body `CantUnwind`: every currently
  modeled cleanup call is inferred `nounwind`.
- Marking the inline, empty `EaseInOutMatrix` destructor `noexcept(false)` did
  not add the handler because Clang can see and prove its empty body
  non-throwing. That broader header experiment was reverted.

Original `EaseInOutMatrix` and `RepairBeam` destructors have compact unwind
metadata while their current empty/reconstructed counterparts are
`CantUnwind`, so an unrecovered potentially-throwing destructor body or
declaration remains the likely source. Do not add an artificial throwing call
or opaque handler.

Requeue only with a faithful reconstruction of one of those destructor bodies
and gate its full call-site blast radius. The landed 334-byte partial is
strictly better than restoring the redundant reachable reload.
