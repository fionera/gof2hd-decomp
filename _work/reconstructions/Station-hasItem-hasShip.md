# `Station::hasItem` / `Station::hasShip` scheduling checkpoint

Targets:

- `_ZN7Station7hasItemEi`, original `0xa6e42`, 58 bytes, 96.0%.
- `_ZN7Station7hasShipEi`, original `0xa6e7c`, 58 bytes, 96.0%.

Both current functions are 58 bytes and match structurally except for one
preservation-copy schedule. For `hasItem` the original begins:

```text
ldr r1, [r0, #40]
mov r5, r0
cbz r1, null_return
```

The current build emits:

```text
ldr r1, [r0, #40]
cbz r1, null_return
mov r5, r0
```

`hasShip` has the identical residual with member offset `#44`. Every other
instruction is exact apart from expected link-address differences.

An explicit `Station *station = this` alias plus a typed member snapshot was
tested for both methods. It produced the same 58-byte output and did not move
the preservation copy. A fresh retry checked natural alias, reference, and
early-return forms; all leave `this` live only on the non-null successor, so
LLVM MachineSink may legally sink the copy past `cbz`.

Requeue only with authentic source or binary evidence that makes `this` live
on the null path. Do not add dummy false-edge uses, volatile aliases, asm,
attributes, or other compiler-forcing constructs. `Station::clone` must
remain untouched and exact.
