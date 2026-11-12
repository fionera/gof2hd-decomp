# `Gun::calcCharacterCollision` wave 190 (w13463): the rotated k-loop is now
# reachable in plain source, but it costs +4 B and moves the cold then-block
# out of line; the 226-B phase shift is independent of body order — no landed win

Worker: w13463. Branch `wave190-w13463`, base master `83d52fe52`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13463`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns. Tree clean apart from
this note.

| build | raw | aligned | size |
|---|---:|---:|---:|
| before (HEAD `83d52fe52`) | **455** | **93** | 1984/1984 |
| after this pass (note only) | **455** | **93** | 1984/1984 |

Gate on exit (full `check`, `-j2`, `ulimit -v 12000000`): `RATCHET PASS`,
byte_exact 4507/4524, linked_exact 4507/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
lint CLEAN.

## 1. New evidence: the k-loop rotation is reachable from plain source

The wave-190 w13432 note reached the shipped rotated k-loop only with
`-mllvm -likely-branch-weight=3 -mllvm -unlikely-branch-weight=1` (size 1980).
This pass reaches a **rotated** loop with ordinary source:

```c++
unsigned k = 0;
for (;;) {
    ... k-loop body ...
    if (++k == this->enemies->size()) break;
}
```

That emits the shipped bottom-tested shape (`b.n latch` header, `adds r5,#1`
then `ldr/ldr/cmp/bcc body`, `b.n exit`) — but **size 1988 (+4)** and the cold
`diff>0` then-block is placed *out of line before the body* instead of inline
after the `ble` as in the original. Every tail form measured the same
(`T1`..`T4`, `R3`..`R9`, `S0`/`S2`): **1988 / 1027 / 662 insns**.

Compensating the +4 by dropping the redundant inner
`if (this->impact != nullptr)` (which alone is 1980 / -4) gives a **size-exact
1984**:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **455** | **93** | 1984 |
| `for(;;)` + `if (++k==size) break;` | 1027 | 156 | 1988 |
| same + drop redundant inner impact check | **979** | 147 | **1984** |

So the rotation is real but a net **raw loss** (+524): the then-block moves
out of line and the phase shift is untouched. The 1984-B size anchor of the
committed body is tied to the *top-tested* cold-then-block layout.

## 2. The 226-B phase shift is independent of body order and rotation

`ranges.py` on the faithful body and on the rotated `U1` both still show the
exact same run:

```
0x153198..0x1531b1   26 B
0x1531b3..0x153250  158 B
0x153252..0x15327b   42 B
```

i.e. the dead `add r4,sp,#104` remat of the hoisted `&view` at `0x153198`
(2 B) where the original reloads `i` into `fp` (`ldr.w fp,[sp,#40]`, 4 B).
It survives every k-loop body order and every rotation, confirming it is the
greedy-RA `i*3`(r5)/`&view`(r4) swap described in w13417/w13445/w13449, not a
layout artefact of the k-loop.

## 3. New cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

All size-exact unless stated; none beats 455.

### 3.1 Faithful body order (clamp -> damage -> damageEmp)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed (emp -> clamp -> damage) | **455** | **93** | 1984 |
| faithful C-D-E | 458 | 63 | 1984 |
| `if (!expect(diff>0,0)) continue;` + faithful | 458 | 63 | 1984 |
| `while (k < size)` + faithful | 458 | 63 | 1984 |
| `k != size` + faithful | 458 | 63 | 1984 |
| `expect(magnitude > distance,0)` + diff inside, faithful | 463 | **57** | 1984 |
| faithful, two separate clamp `if`s | 1027 | 121 | 1960 |
| faithful, separate `Player *ke` in k-loop | 1879 | 368 | 1884 |

### 3.2 Loop-condition expects (faithful body)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `expect(k < size, 1)` | 1799 | 361 | 1948 |
| `expect(k < size, 0)` | 1714 | 308 | 1944 |

### 3.3 View slot / view lifetime (goal: break the `%stack.10` CSE)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| committed | **455** | **93** | 1984 | |
| `char viewbuf[60]` + `Matrix&` + `__builtin_memcpy`, in block | 1370 | 194 | 1924 | 60-B slot folded away |
| same, `alignas(1)` / extra `char pad1` | 1370 | 194 | 1924 | identical |
| `viewbuf+1` / `viewbuf+2` (odd-aligned view) | 1886 | 348 | 1848 | |
| `alignas(8) char viewbuf[60]` hoisted above `if (explosive)`, `TransformSetLocal` moved after scatter | 1802 | 216 | 1980 | frame 208 but slots rotate: `mine` sp+36 -> sp+24 |
| `Matrix &view = *(Matrix*)CameraGetLocal(...)` (no 60-B copy) | 1195 | 177 | 1972 | |
| `char viewbuf[N]` + `Matrix&` at `viewbuf+{4,8}` (N=64..72) | 1886 | 348 | 1848 | copy folded away |

The `char viewbuf` forms confirm the view slot is `sp+104` and the
`MatrixSetTranslation` sret temp is `sp+44`; hoisting the buffer to i-loop
scope keeps the 208-B frame but re-colours the 4-byte int slots, which costs
more than it saves.

### 3.4 Micro-variants

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `empDamage != 0` instead of `> 0` | 455 | 94 | 1984 |
| `else if (this->geometries)` (plain) | 455 | 93 | 1984 |
| `if (this->impact)` (plain) | 455 | 93 | 1984 |
| drop redundant inner `if (this->impact != nullptr)` | 1096 | 135 | 1980 |

## 4. Conclusion

The residual is unchanged and now has two independent, fully characterised
components:

1. **226 B** — the dead `add r4,sp,#104` remat of the hoisted `&view` (greedy
   RA keeps the stack-colored `view`/k-loop-sret slot in r4 across the
   `outerCollide` call and the impact block, pushing `i*3` to r5). Independent
   of body order and loop rotation.
2. **~167 B** — the k-loop block layout. The shipped rotated loop is now
   reachable in plain source (`for(;;)` + bottom `break`) but is +4 B and puts
   the cold then-block out of line; making it size-exact (drop the redundant
   inner impact check) gives 1984 B but raw 979.

The committed **455 / 93 / 1984** body remains the best measured shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13463
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave190-w13463.md` only.
