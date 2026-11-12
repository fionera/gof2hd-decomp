# `Gun::calcCharacterCollision` + `SpaceLounge::drawLounge` wave 185 (w12920):
# both residuals are RA/hoist plateaus; no size-exact raw win

Worker: w12920. Branch `wave185-w12920`, base master `f390b0e68`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w12920`.

## 0. Result

**No `src/` change landed.**  Every size-exact shape measured this wave is
equal to or worse than the committed body.  The tree is clean apart from
this note.

| symbol | raw | aligned | size | pct |
|---|---:|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | **458** | 63 | 1984/1984 | 93.4 |
| `_ZN11SpaceLounge10drawLoungeEv` | **175** | 41 | 1948/1948 | 95.7 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, `.rel.plt` byte-identical, 27/27 headers, file size 2,187,328
exact.  `.text` diff 74,591, `.ARM.extab` diff 647 (size/base exact at
`0x1a3b78`, 156,312 B), build-id 20, total 75,258.

## 1. `Gun::calcCharacterCollision` — 458/63/1984

The residual is the same eight clusters as wave 125/133/176.  The dominant
one is the `Early Machine LICM` hoist of `&stack.10`:

```
0x153198  orig: f8dd b028  ldr.w fp,[sp,#40]   ; reload i
          ours: ac1a       add   r4,sp,#104    ; hoisted &delta/view
```

That 4-vs-2-byte substitution shifts the whole collision-check block by two
bytes (the 158-byte run at function offset `0x223` and the 42-byte run at
`0x2c2`), and it also steals `r4` from `i*3` (`r4` shipped, `r5` ours),
which cascades through the pp/delta slots, the view block and the rotated
k-loop.  The `wave176` `int diff = magnitude - distance` fold is still the
best size-exact shape.

New sweeps this wave (all full link + `tools/fn_diff.py`, `-j2`):

* k-loop temp spellings: `Vector ep = getPosition(); *td = ep; *td -= p;`,
  `Vector ep; ep = getPosition(); ...`, `ep -= p; *td = ep`,
  `*td = getPosition() - p`, `Vector *p = td`, reference forms — all
  458/1984 or worse (named forms are identical; the rest are 1892–1972 B).
* loop forms: `while`, `for(;;)`, `for(;;++k)`, `k != size()`, cached
  `n`, `int`/`long`/`size_t`/`uint32_t` induction variables, `k++`,
  `__builtin_expect(k < size(), 0/1/2/3/7/100)` — 458/1984 or 1940–1988 B.
* condition/expect forms: `diff > 0`, `diff >= 1`, `diff != 0`,
  `0 < diff`, `diff`, `distance < magnitude`, `magnitude > distance`,
  `distance <= magnitude - 1`, `distance - magnitude < 0`, expect weights
  0/1/2/3/7/100, `const`/`long`/`short`/`unsigned` diff — best 458/1984.
* falloff clamp: if/else, nested else, ternary, two independent ifs,
  `>=`/`<=`, `std::min`/`std::max` — best 458/1984.
* `empDamage` test/cast and the volatile `dv` damage spelling — best
  458/1984 (`volatile int *dv = &this->damage` is identical to the
  committed reference form).
* `#pragma clang loop unroll(disable)` / `unroll_count(1)` /
  `vectorize(disable)` / `interleave(disable)` / `nounroll` — identical.
* single `__builtin_expect` probes on `mine`, `isActive`, `hitpoints`,
  `isAsteroid`, `explodeOnContact`, `empDamage`, `owner`, `playerGun`,
  `impact`, `scatter`, `geometries` — every size-1984 candidate is ≥458;
  the rest change size (1880–1996 B).

Root cause unchanged: the shipped is the `expr` grouping *without* LICM
hoisting; our clang hoists the trivially rematerializable `t2ADDri
%stack.10` to the i-loop header, and the same frame index is what places
both the k-loop sret and the view at `sp+104`.  No source shape found this
wave separates those two requirements.

## 2. `SpaceLounge::drawLounge` — 175/41/1948

The residual is the right-edge clamp RA tie-break:

```
value      shipped        ours
pad        r9             fp (r11)
x          r5             r9
arm        addgt.w r1,r6,sl ; subgt r5,r0,r1
           (2 insns, 8 B)  movgt r1,#0 ; subgt.w r1,r1,fp,lsl#1 ;
                           subgt r1,r1,r6 ; addgt.w r9,r1,r0 (4 insns, 14 B)
```

The clamp block plus its downstream `x` uses account for ~128 of the 175
raw bytes; the remaining ~31 are the exception landing-pad order at
`0x17148a` (the shipped has a cleanup block that destroys only `sp+104`;
ours has an extra branch to the `sp+92` cleanup).

New sweeps this wave (full link + `tools/fn_diff.py`):

* clamp association: 28 spellings of `w - inset - width - 2*pad`
  (`-`, `+ -`, nested parens, `2*pad`, `pad*2`, `pad+pad`, reversed
  operand order).  The committed flat `+ (-width - pad - pad)` is the
  unique 175; every `pad*2` family is 445; `-(width+pad+pad)` is 176.
* `pad2` locals (`pad+pad`, `pad*2`, before/after `pad`, `const`),
  `unsigned short`/`uint16_t`/`uint8_t` pad (176), `int32_t`/`long`/
  `ptrdiff_t`/`intptr_t` (175), `uint32_t`/`size_t`/`uintptr_t` (177),
  `short`/`int16_t`/`int8_t`/`float`/`double`/`long long` (1964–2096 B).
* declaration permutations of `{pad, SetColor, centerX, top, x}` (30
  valid topological orders): all orders with `pad` before `SetColor`
  are 175; every order with `SetColor` before `pad` is 254.
* permutations of `{topLeft, bottomRight, pad, SetColor}` (24 orders):
  best 175; moving `SetColor` before the vector loads grows to 1952.
* `top`/`drawBox`/clamp `pad+pad` vs `pad*2` vs `2*pad` combinations:
  clamp must use the `pad+pad` spelling for 175; `pad*2` in the clamp is
  445.
* `Layout *layout` placement (function top, mode-0 top, after hover,
  before pad) and direct `Globals::layout->field_0x94` reloads — 175 only
  when the single `pad` local is kept; direct reloads grow to 1960.
* condition/control flow: nested else, ternary, `&&` form, local
  `left`/`right`, `__builtin_expect` on first/second/both clamp
  conditions (weights 0/1/2/3/7/100) — best 175; all others 1952–1956 B.
* top-level / hover / `isKnown` / name / role / footer expects — best
  175; the rest change size or score.
* `const`/`const Agent *`/`Agent &`/`const Vector` qualifiers, name/role
  direct-init, `String(...)` wrapping, `String("", false)`, lambda and
  if/else role construction — best 175; if/else role grows to 2028.
* landing-pad analysis: the shipped cleanup order at `0x17148a`
  (`mov r4,r0; b.n 171496`) vs ours (`b.n 171490`) is tied to the
  `role` temporary at `sp+92`; no size-1948 source form found that
  shortens its lifetime without changing the main body.

Root cause unchanged: `pad` is selected after `x`'s re-enqueue, so `x`
takes `r9` whole and `pad` later takes `fp`; the shipped is the opposite
colour.  Every shape that forces `pad=r9, x=r5` either adds a
`field_0x94` reload (raw 310+) or uses 64-bit pad arithmetic (size
2008).

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12920
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv _ZN11SpaceLounge10drawLoungeEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=458/175`, `aligned=63/41`, sizes `1984/1948`,
`RATCHET PASS`, byte 4507 / linked 4507, data 843/843, imports 207,
lint CLEAN, drift 0.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12920/`
(`oracle.py`, `bytesdiff.py`, `sweep_*.py`, `gun_mir.txt`, `sl_mir.txt`).

## 4. Files changed

`_work/reconstructions/gun-spacelounge-wave185-w12920.md` only.
