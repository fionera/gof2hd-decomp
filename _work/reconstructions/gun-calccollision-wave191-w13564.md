# `Gun::calcCharacterCollision` wave 191 (w13564): the k-loop layout is a
# post-RA side effect, and the faithful k-loop body is instruction-exact

Worker: w13564. Branch `wave191b-w13564`, base master `a7c4defa9`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13564`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| before (HEAD `a7c4defa9`) | **455** | **93** | 1984/1984 | 660 |
| after this pass (note only) | **455** | **93** | 1984/1984 | 660 |

`git status --porcelain` is empty apart from this note.

## 1. New evidence (a): the faithful k-loop body is instruction-exact

The original's k-loop guard is `if (this->magnitude > distance)` — the
disassembly is `cmp r0,r1; ble` *before* `subs r0,r0,r1` (diff), not
`if (diff > 0)`. With the faithful statement order

```c++
int distance = (int) VectorLength(*td);
if (__builtin_expect(this->magnitude > distance, 0)) {   // lint: arm_builtin
    int diff = this->magnitude - distance;
    float falloff = (float) diff / (float) this->magnitude;
    if (falloff > 1.0f) falloff = 1.0f;
    else if (falloff < 0.0f) falloff = 0.0f;
    enemy->damage(...);
    if (this->empDamage > 0) enemy->damageEmp(...);
}
```

the k-loop body becomes instruction-for-instruction identical to the original:
**raw 463 / aligned 57 / 1984 B / 660 insns** (`magcde_exp`). `aligned` 57 is
the best ever measured for this function (committed is 93). It loses on raw
only because the 226-B phase shift and the k-loop block placement are
unchanged; every remaining instruction diff is in those two clusters.

Without the `expect` (plain `magnitude > distance`, faithful C-D-E) the same
source gives **raw 1886 / aligned 257 / 1908 B / 646 insns** (`magcde`): the
whole function re-allocates (`this`→r4) and the k-loop *is* body-first, i.e.
the original's layout.

So the faithful source is right; the only thing the committed `expect(diff>0,0)`
buys is the 76-B spill-anchor RA (`this`=r9, `td`=r6, `enemy`=r8,
`&stack.10`=r4, `i` spilled), and it simultaneously breaks the k-loop layout.

## 2. New evidence (b): the k-loop block order is decided *after* RA

I dumped the MIR around the k-loop for both the committed build and the
no-`expect` build (`-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv`, `-g0`):

* pre-RA (`ARM pre- register allocation load / store` dump) **both builds**
  lay the k-loop out header-first:
  `bb.80` = header (`ldr [this+180]`, `cmp`, `bcc`), `bb.81` = body
  (`getPosition`), `bb.88` = latch, `bb.89` = exit.
* final disassembly: committed is header-first
  (`0x153440` header, `0x15344c` body, `0x153516` latch, `b.n` back);
  no-`expect` is body-first (`0x153402 movs r5,#0; b.n 0x1534ba`, body at
  `0x153406`, header at `0x1534ba`, `bcc` back) — the original's layout.

Therefore the 167-B k-loop cluster is **not** a `MachineBlockPlacement`
difference at the pre-RA stage: the early MBP leaves both builds header-first.
The rotation is introduced by the late post-RA
`Branch Probability Basic Block Placement` pass (dump at the end of the
pipeline), and it rotates the no-`expect` build but not the committed one.
This is consistent with `MachineBlockPlacement::rotateLoop`: at the early
`buildLoopChains` call the k-loop chain is still just `[header]`, so
`Bottom == ExitingBB` and `rotateLoop` returns immediately; only the post-RA
re-run sees the merged `[header, body, …, latch]` chain and can rotate, and
its decision then depends on the RA-shaped layout.

Consequence: the k-loop layout cannot be fixed by a source-level loop rewrite
alone while the RA stays the committed one; it is downstream of the RA.

## 3. The 226-B phase shift, restated exactly

Ours is 2 B short from `0x15319a` (2-B `add r4,sp,#104` where the original has
the 4-B `ldr.w fp,[sp,#40]`), grows to 4 B at `0x153262` (same swap in the
non-asteroid block), and is re-synchronised at `0x15327c` by the extra 4-B
`ldr.w fp,[sp,#40]` at `0x153278`. The original's reload at `0x153198` covers
*both* the `if (mine) continue` increment (`0x15325a`) and the
`lifetimes[i] = -1000000` store (`0x153346`); ours splits it into a reload in
the increment block (`0x153254`) and a reload before the `empDamage` check
(`0x153278`). The split point is the 50/50 `if (mine)` branch
(`bb.36`, `successors: 0x40000000/0x40000000` in the pre-RA MIR).

`&stack.10` (fi#10, the 60-B slot shared by `view`, the scatter
`playerPosition` and the k-loop `getPosition` sret) is the value that takes
r4 across that branch; it is live from `0x153198` only on the
`geometries == nullptr` path (the `geometries != nullptr` path re-defines it
at `0x1533ea`). If the k-loop sret address were rematerialised at
`0x153444` (as in the original) the `0x153198` def would be dead and the
`ldr.w fp` would take its place, which is exactly the original.

## 4. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 unless stated; none beats 455.

### 4.1 k-loop / anchor

| variant | raw | aligned |
|---|---:|---:|
| committed `expect(diff>0,0)`, E-C-D | **455** | **93** |
| faithful `expect(magnitude>distance,0)`, C-D-E (`magcde_exp`) | 463 | **57** |
| faithful `magnitude>distance`, C-D-E, no expect (`magcde`) | 1886 | 257 (1908 B) |
| `expect(diff>=1,0)` | 455 | 93 |
| `expect(diff<=0,1) { continue; }` | 455 | 93 |
| `expect(diff<=0,0) { continue; }` | 1887 | 281 (1908 B) |
| `expect(diff>0,0)` + `expect(empDamage>0,0)` | 928 | 107 |
| `expect(diff>0,0)` + `expect(empDamage>0,1)` | 927 | 124 |
| `expect(diff>0,0)` + `expect(falloff>1.0f,0/1)` | 455 | 93 |
| `expect(diff>0,0)` + `expect(playerGun,1)` | 455 | 93 |
| `expect(diff>0,0)` + `expect(owner…,0/1)` | 554 / 533 | 125 / 112 |
| `expect(diff>0,0)` + `expect(impact!=nullptr,0)` | 1079 | 147 (1992 B) |
| `expect(diff>0,0)` + `expect(geometries!=nullptr,0)` | 1378 | 379 (1972 B) |
| `expect(diff>0,0)` + `expect(scatter,0/1)` | 1760 / 1804 | 371 / 349 (1936/1932 B) |
| `expect(diff>0,0)` + `expect(k<size,0/1)` | 1710 / 1798 | 341 / 379 (1944/1948 B) |
| `expect(mine,0)` on both `if (mine)` branches | 455 | 93 |
| `expect(mine,0)` asteroid only | 1817 | 452 (1964 B) |
| `expect(mine,0)` non-asteroid only | 1015 | 342 (1988 B) |
| `expect(!mine,1)` asteroid / non-asteroid | 1812 / 1821 | 404 / 465 (1940/1924 B) |
| `while (k < size) { …; ++k; }` | 456 | 93 |
| `for (k=0; k<size; ) { …; ++k; }` | 456 | 93 |
| `for (k=0; ; ++k) { if (k>=size) break; … }` | 455 | 93 |
| `for (k=0; k!=size; ++k)` / `k += 1` | 455 | 93 |
| `goto`/label continue for the asteroid `mine` | 1560 | 337 (1972 B) |
| `hitFlags`/`hitPositions` moved after the k-loop | 1805 / 1856 | 300 / 335 (1904/1980 B) |

### 4.2 `i` loop / declarations / view block

| variant | raw | aligned |
|---|---:|---:|
| `for (int i = 0; i < (int) count; ++i)` | 455 | 93 |
| `for (unsigned i = 0, n = count; i < n; ++i)` | 455 | 93 |
| `while (i < count) { … }` | 510 | 98 |
| `this->positions[i]` / `.data()[i]` | 455 | 93 |
| `Vector *pos = data() + i; Vector position = *pos;` | 477 | 97 |
| `const Vector &position = data()[i]` | 1787 | 415 (1920 B) |
| `Matrix *vp = &view` / `Matrix &vp = view` / `Matrix *cam` | 455 | 93 |
| `Matrix view;` at function scope + assign in the block | 1885 | 426 (1960 B) |
| `Vector kpos;` at function scope, used in the k-loop | 1827 | 316 (1972 B) |
| `Vector kpos;` at i-loop scope, used in the k-loop | 1742 | 323 (1976 B) |
| `explodeOnContact` ternary / `bool` | 1920 (1192 B) / 455 | 517 / 93 |
| `scatter`/`mine` declaration swap | 487 | 108 |
| `empDamage >= 1` / `!= 0` / `(unsigned)` | 455 | 93 / 94 / 94 |
| `falloff >= 1.0f` clamp | 455 | 93 |
| reversed clamp (`< 0` first) | 456 | 94 |
| two separate `if` clamps | 1019 | 133 (1960 B) |
| `float diff` instead of `int diff` | 455 | 93 |
| `const int mag = this->magnitude` cached | 455 | 93 |
| `hitPositions[i]` via `operator[]` | 455 | 93 |
| `Player *other = (*enemies)[k]; enemy = other;` | 455 | 93 |

## 5. Root cause summary

Two independent blockers remain, both downstream of the same RA decision:

1. **226-B phase shift / 10-B `i*3` register.** `&stack.10` (fi#10) is kept
   live from `0x153198` across the 50/50 `if (mine)` branch, taking r4; the
   original rematerialises it per use, leaving r4 for `i*3` and using a
   `ldr.w fp,[sp,#40]` reload for `i` at `0x153198`/`0x153262`. The k-loop
   `getPosition` sret shares fi#10 with `view` (stack-coloured), so the
   address is a single IR value the greedy RA can keep live.
2. **167-B k-loop block order.** Introduced by the post-RA
   `Branch Probability Basic Block Placement`, not the early MBP; a source
   rewrite of the loop alone cannot force it while the RA stays committed.

The committed `expect(diff>0,0)` is a *spill anchor* (it forces the 76-B
spill-heavy RA that keeps the function at 1984 B) and simultaneously a
*placement anchor* (it makes the k-loop then-block 0.05 %, which is what the
post-RA MBP reacts to). No source shape tried separates the two effects.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13564
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

MIR recipe used for §2:

```sh
clang++ <original flags> -g0 -S -o mbp.s \
  -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  -c src/game/weapons/Gun.cpp 2> mbp.err
# pre-RA layout: "IR Dump After ARM pre- register allocation load / store optimization pass"
# post-RA layout: "IR Dump After Branch Probability Basic Block Placement"
```

## 7. Files changed

`_work/reconstructions/gun-calccollision-wave191-w13564.md` only.
