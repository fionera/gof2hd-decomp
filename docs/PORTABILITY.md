# Portability refactor: offset-soup → named-field structs

Goal: make the decomp compile as a **native 64-bit macOS** binary (for the invoker) *without*
emulation, while staying **byte-exact on armv7** (the matching target). The blocker is that the
corpus accesses fields by hardcoded 32-bit byte offsets (`P(self,0x69)`, `i32(self,0x144)`,
`fi_held(self)` = `*(FI**)(self+4)`), which break on 64-bit (pointers are 8 bytes, so every offset
past a pointer shifts).

## The fix (per class)
1. **Recover the layout** from all the class's methods: offset → (type, name). Type comes from how
   each offset is accessed (`i32`→int32, `f32`→float, `pp`/`F<T*>`→pointer). Include base-class
   fields (model inheritance).
2. **Emit a real struct** with named fields in offset order; **pointer fields are real pointers**.
3. **Rewrite accessors** `F<T>(self,off)` / `i32(self,off)` / `fi_held(self)` → `self->field`.
4. **De-hardcode sizes**: `operator new(0x68)` → `new T` / `sizeof`, `memclr(this+0x158,0x48)` →
   field-range clears. Use `__SIZE_TYPE__` for `operator new` (4 bytes armv7 / 8 arm64).

## Why it stays byte-exact on armv7
On 32-bit, a real struct `{ void* vptr; FI* held; }` lays vptr@0, held@4 — identical to the old
offsets — so the compiler emits the same loads/stores. **Proven** on AENormalFile: the refactored
source produced a `.text` section byte-for-byte identical to the offset form (`strd r2,r1,[r0]`),
and the same source also compiles clean for `arm64-apple-macos`.

## Scale
~286 classes / ~3,100 files. A multi-wave effort (like the original recovery). Prioritize the boot
path (FileInterface*, String, MGame, ApplicationManager, the ndk23 layer) so the 64-bit invoker can
start. Each converted class must still pass `make verify` (byte-exact on armv7) and `-fsyntax-only`
for `arm64-apple-macos` (portable).

## Pilot
AENormalFile (8 methods) — done: `vptr`+`held` named fields, dual-arch clean, byte-identical armv7.
