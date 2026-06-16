export const meta = {
  name: 'gof2-readable-cleanup',
  description: 'Clean one subsystem of GOF2 decomp TUs into human-style C++ (named typed members, real new/delete), Ghidra+DeepOpen referenced',
  phases: [
    { title: 'Clean' },
  ],
}

// args = array of { class, cpp, header|null, deepopen|null, subsystem }
const entries = (typeof args === 'string' ? JSON.parse(args) : args) || []

const REPORT = {
  type: 'object',
  additionalProperties: false,
  required: ['class', 'compiled', 'fields_named', 'op_new_delete_fixed', 'summary'],
  properties: {
    class: { type: 'string' },
    compiled: { type: 'boolean', description: 'true if the TU passed -fsyntax-only after the edits' },
    fields_named: { type: 'integer', description: 'count of field_0xNN / raw-offset members converted to named typed members' },
    op_new_delete_fixed: { type: 'integer', description: 'count of ::operator new/delete call sites converted to new/delete' },
    members_changed: {
      type: 'array',
      description: 'header member renames/retypes other TUs may read (offset, old name, new name, type)',
      items: {
        type: 'object', additionalProperties: false,
        required: ['offset', 'old', 'new', 'type'],
        properties: { offset: { type: 'string' }, old: { type: 'string' }, new: { type: 'string' }, type: { type: 'string' } },
      },
    },
    rawreads_left: { type: 'array', items: { type: 'string' }, description: 'offsets left as RAWREAD/named-fallback with the reason' },
    globals_touched: { type: 'array', items: { type: 'string' }, description: 'extern globals this file references that still need typing' },
    compile_error: { type: 'string', description: 'first compiler error if compiled=false, else empty' },
    summary: { type: 'string' },
  },
}

function prompt(e) {
  const hdr = e.header ? e.header : '(no paired header — members may live in another header or this is a free-function TU)'
  const dref = e.deepopen ? e.deepopen : '(no DeepOpen Java equivalent)'
  return `You are cleaning ONE class of a decompiled C++ port of "Galaxy on Fire 2" into idiomatic, human-style C++.
Goal of the whole project: code should read as if hand-written — no pointer arithmetic, real typed members, real new/delete.

STRICT SCOPE: edit ONLY these two files. Do not touch any other file.
  - .cpp:    ${e.cpp}
  - header:  ${hdr}
You MAY read any other file for reference, but never edit it.

Working directory: /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp

REFERENCES (read these first):
  1. Ghidra ground-truth layout for class "${e.class}" (offset/size/type/name), extract with:
       python3 -c "import json;print(json.dumps(json.load(open('_work/types/ghidra_layouts.json')).get('${e.class}'),indent=1))"
     The struct size and field OFFSETS are authoritative (32-bit ARM offsets). Types named 'undefined1/2/4/8' mean
     the size is known but the semantic type is not — infer it (see naming priority below).
  2. DeepOpen Java reference (same codebase lineage; use for field NAMES, TYPES, and logic — NOT byte layout):
       ${dref}
  3. Existing target-style exemplars: src/engine/core/Array.h, src/engine/math/AEMath.h.

WHAT TO FIX (in these two files only):
  A. Every raw byte-offset member access of THIS object becomes a named, typed member:
       this->field_0xNN          -> this->semanticName
       i32(this,0xNN)/u32/f32/u8/u16/pp/field<T>(this,0xNN)/FL/UC/US(this,0xNN)  -> this->semanticName
       *(T*)((char*)this+0xNN)    -> this->semanticName
     Declare the member in the HEADER in offset order, with the correct type, keeping "// +0xNN" as a comment.
     NAMING/TYPING priority: (1) this class's own get/set/is/has accessor bodies (they literally encode name@offset);
       (2) the ctor initializer list; (3) how the field is USED (e.g. passed to glUniform4fv with a color -> a color
       uniform-location int; stored from new X() -> X*); (4) Ghidra type; (5) DeepOpen Java field of the same role.
  B. int/undefined members that are really pointers or objects -> the real type (e.g. int -> Mesh*, Array<T>*).
     Embedded char[12] / unsigned char[12] String slots -> a real 'String' member (drop any manual placement-ctor/dtor).
  C. ::operator new(sz) followed by a ctor call -> new T(args);  ::operator delete(p) -> delete p;
     array forms -> new T[n] / delete[] p.  Match the element type from the surrounding code.
  D. Replace obviously-wrong int locals/casts with the proper type where it is LOCAL to this file and safe.

CROSS-TU SAFETY (critical — other TUs read this class by member name):
  - Before renaming a previously-named field_0xNN, GREP for external readers of it, e.g.:
        grep -rn '\\(->\\|\\.\\)field_0xNN\\b' src --include='*.cpp' --include='*.h' | grep -v '${e.cpp}'
    * If NO external reader exists: rename the member OUTRIGHT (no union, no alias) — this is the clean, preferred case.
    * If an external reader DOES exist: keep the old name working with a union alias so those TUs keep compiling:
          union { Type semanticName; uint32_t field_0xNN; };   // or matching old type/size
    Brand-new members (only ever accessed via a raw cast in the .cpp) never need an alias.
  - Do NOT change this class's method signatures, ctor/dtor names, or public method set — that breaks callers.
  - Do NOT rename or retype members of OTHER classes. If you read e.g. engine->field_0x2fc or mesh->field_0xNN
    (a field of a different class), LEAVE it exactly as-is — that owning class gets cleaned separately. List it in
    globals_touched only if it is an extern global.

ESCAPE HATCH: if an offset genuinely cannot be modeled as a faithful member (sparse/non-byte-faithful class, opaque
  type with no header, an offset that doesn't line up with any real field), keep a NAMED member (typed, e.g.
  uint8_t field_0xNN;) or leave the access with a "// RAWREAD: <reason>" comment. This still removes pointer
  arithmetic. Note each such case in rawreads_left. Do NOT invent a wrong type just to avoid a cast.

WATCH OUT:
  - IEEE bit patterns in now-float members: 0x3f800000 -> 1.0f, 0xbf800000 -> -1.0f, 0x40000000 -> 2.0f, etc.
  - Keep includes correct: common.h is GONE — use "engine/core/Array.h", "AEString.h", "fieldaccess.h",
    "aetypes.h", "mathtypes.h". Include the real header for any interface type you newly reference.
  - Do not remove fieldaccess.h include if other (unconverted) helper calls remain in the file.

VERIFY before returning — compile this exact TU (must exit 0, fix until it does):
  c++ -Isrc -std=gnu++14 -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast -fsyntax-only ${e.cpp}
If you cannot make it compile after a real effort, REVERT your edits to leave the file building as you found it, set
compiled=false, and explain in compile_error — never leave the TU broken.

Return the structured report. members_changed must list every header member rename/retype that other TUs might read.`
}

phase('Clean')
const results = await parallel(entries.map((e) => () =>
  agent(prompt(e), { label: `clean:${e.class}`, phase: 'Clean', schema: REPORT })
))

const ok = results.filter(Boolean)
const compiled = ok.filter((r) => r.compiled).length
const failed = ok.filter((r) => r && !r.compiled)
log(`cleaned ${ok.length}/${entries.length} agents returned; ${compiled} compiled clean; ${failed.length} could not compile`)
return {
  total: entries.length,
  returned: ok.length,
  compiled,
  failed: failed.map((r) => ({ class: r.class, error: r.compile_error })),
  reports: ok,
}
