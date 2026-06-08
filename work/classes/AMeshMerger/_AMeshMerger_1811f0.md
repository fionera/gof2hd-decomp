# AMeshMerger::~AMeshMerger
elf 0x1811f0 body 16
Sig: undefined __thiscall ~AMeshMerger(AMeshMerger * this)

## decompile
```c

/* AMeshMerger::~AMeshMerger() */

AMeshMerger * __thiscall AMeshMerger::~AMeshMerger(AMeshMerger *this)

{
  Array<AbyssEngine::Mesh*>::~Array((Array<AbyssEngine::Mesh*> *)(this + 8));
  return this;
}

```

## target disasm
```
  001911f0: push {r4,r6,r7,lr}
  001911f2: add r7,sp,#0x8
  001911f4: mov r4,r0
  001911f6: adds r0,#0x8
  001911f8: blx 0x0006f760
  001911fc: mov r0,r4
  001911fe: pop {r4,r6,r7,pc}
```
