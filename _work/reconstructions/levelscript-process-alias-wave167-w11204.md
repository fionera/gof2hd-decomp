# LevelScript::process — wave 167 / w11204: size-exact `m_pLevel` alias shapes

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11204`, base master
`d00057e6d` (wave 166).  Scratch `/dev/shm/w11204/`.

Follow-up to `levelscript-process-callee-saved-wave166-w11104.md`.  Question:
can the stack-spilled `m_pLevel` alias (`Level *self = m_pLevel;`) be expressed
in a shape that stays **57,232 B** and keeps `.rel.plt` byte-identical?

Result: **no source win landed**.  The only size-exact alias spellings are
codegen-identical to direct `m_pLevel->` access; every spelling that actually
caches the pointer value is **+8 B** (or more).

```
base (HEAD)                    raw 54,095  aligned 17,621  57,232 B
direct Globals::status         raw 54,096  aligned 17,614  57,232 B
.rel.plt                       0 differing bytes (BYTE-IDENTICAL)
```

## 1. The original accessor block is already reproduced by direct access

Original prologue (`0x137d18..0x137d54`):

```
mov sl, r0
ldr.w fp,[r0]              ; fp = &__stack_chk_guard
ldr.w r0,[fp]
str r0,[sp,#428]
ldr.w r0,[r4,#24]!         ; r4 = &m_pLevel, r0 = m_pLevel
blx getMessages            ; r5 = messages
ldr r0,[r4]                ; reload m_pLevel
blx getPlayer              ; player -> [sp,#84]
strd r4,r0,[sp,#80]        ; spill &m_pLevel, player
ldr r0,[r4]                ; reload m_pLevel
blx getEnemies             ; r8 = enemies
```

The wave-164/166 base with `Status *status = Globals::status;` and **direct**
`m_pLevel->getMessages()/getPlayer()/getEnemies()` emits this block
byte-for-byte (same `ldr.w r0,[r4,#24]!`, same two `ldr r0,[r4]` reloads, same
`strd r4,r0,[sp,#80]`).  The original therefore does **not** cache the
`m_pLevel` value in a local pointer; it keeps `&m_pLevel` in `r4` and reloads
the member.  A `Level *self = m_pLevel;` local is not the original source shape.

## 2. Alias spelling sweep (all 3 accessor calls use the alias)

Measured on the wave-166 base (`Status *status` local kept unless noted).

| declaration | raw | aligned | size | map |
|---|---:|---:|---:|---|
| `Level *&self = m_pLevel;` (ref to member) | **54,095** | **17,621** | **57,232** | unchanged, byte-identical to base |
| `Level *self = m_pLevel;` | 54,968 | 17,638 | 57,240 (+8) | unchanged |
| `Level &self = *m_pLevel;` | 54,968 | 17,638 | 57,240 (+8) | unchanged |
| `Level *const self = m_pLevel;` | 54,968 | 17,638 | 57,240 (+8) | unchanged |
| `auto *self = m_pLevel;` | 54,968 | 17,638 | 57,240 (+8) | unchanged |
| `Level *self = (Level *) m_pLevel;` | 54,968 | 17,638 | 57,240 (+8) | unchanged |
| `Level *self = this->m_pLevel;` | 54,968 | 17,638 | 57,240 (+8) | unchanged |

With direct `Globals::status->` (no `Status *` local) the same table shifts by
the known +1: reference form 54,096 / 17,614 / 57,232; cached-pointer forms
54,966 / 17,613 / 57,240.

**The reference-to-member form is the only size-exact spelling.**  It is a pure
alias: clang emits exactly the direct-access code (r4 = `&m_pLevel`, value
reloaded per call), so it is byte-identical to the base and not a gate win.

## 3. Scope / first-use-order sweep

Alias used for a subset of the three accessor calls (declaration at top):

| used for | raw | aligned | size |
|---|---:|---:|---:|
| getMessages only | 54,095 | 17,621 | 57,232 (coalesced into the first load) |
| getPlayer only | 55,242 | 17,657 | 57,276 (+44) |
| getEnemies only | 55,248 | 17,659 | 57,276 (+44) |
| getPlayer + getEnemies | 54,966 | 17,613 | 57,240 (+8) |
| all three | 54,966 | 17,613 | 57,240 (+8) |

A narrower scope around all three (`{ Level *self = m_pLevel; ... }`) is still
+8 (57,240).  A scope around getMessages only is size-exact but a no-op.
Declaring the alias after the accessor block makes it dead (identical to
direct).  So no scope/order spelling reaches 57,232 with a cached value.

## 4. Broad alias replacement flips `this=sl` but changes size

Replacing `m_pLevel` with a cached `Level *self` over a prefix of the body
(declaration after the accessor block, so the prologue stays direct):

| replaced occurrences | raw | aligned | size | map |
|---|---:|---:|---:|---|
| 3..35 | 55,100 | 17,814 | 57,108 | this=fp |
| 3..36 | 55,213 | 18,866 | 57,228 | this=sl, guard=fp |
| 3..36 + occurrence 214 | **55,215** | **18,873** | **57,232** | this=sl |
| 3..39 | 55,152 | 17,071 | 57,184 | this=sl |
| 3..39 + occurrence 214 | 55,215 | 18,873 | 57,232 | this=sl |
| all (0..345) | 55,360 | 19,867 | 57,096 | this=sl, guard=fp |

The `this=sl` flip happens once the cached alias's live range reaches
occurrence 36 (line 875, `m_pLevel->getPlayer()->setCollide(true)`).  A
size-exact `this=sl` shape exists (occurrences 3..36 plus 214), and its
`.rel.plt` is byte-identical (0), but raw is **+1,120** (55,215 vs 54,095) and
the rest of the map is not original (`&m_pLevel=r5`, `messages=r8`,
`enemies=fp`, `guard=r9`, `&status=r4`).  It is not a raw win.

Minimal flip sets (no prefix) also flip `this=sl` but are all size-changing:
`{3,36}` = 55,074 / 57,268; `{3,20,36}` = 54,741 / 57,260.

## 5. Conclusion

* The original accessor block (r4 = `&m_pLevel`, two reloads) is exactly the
  direct `m_pLevel->` shape; the base already matches it.
* `Level *&self = m_pLevel;` (reference to the member) is the only
  size-exact alias spelling, and it is codegen-identical to direct access —
  a no-op, not a gate win.
* Every spelling that caches the pointer value (`Level *`, `Level &`,
  `const Level *`, narrower scope, different first use) is +8 B (57,240) or
  more; the +8 is the cached value load plus its spill/reload, independent of
  the `Status` shape.
* Broad alias replacement can reach a size-exact `this=sl` map (57,232 B,
  `.rel.plt` identical) but costs +1,120 raw bytes and does not reproduce the
  rest of the original callee-saved map.
* No source change landed; the tree is at the wave-166 base.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 54095 aligned= 17621 size= 57232/57232 insn= 20715/20830
```
