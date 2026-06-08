# ResourceTransform::~ResourceTransform
elf 0x77ee4 body 32
Sig: undefined __thiscall ~ResourceTransform(ResourceTransform * this)

## decompile
```c

/* AbyssEngine::ResourceTransform::~ResourceTransform() */

ResourceTransform * __thiscall
AbyssEngine::ResourceTransform::~ResourceTransform(ResourceTransform *this)

{
  if (*(void **)(this + 0x40) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x40));
  }
  *(undefined4 *)(this + 0x40) = 0;
  if (*(void **)(this + 0x48) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x48));
  }
  *(undefined4 *)(this + 0x48) = 0;
  return this;
}

```

## target disasm
```
  00087ee4: push {r4,r5,r7,lr}
  00087ee6: add r7,sp,#0x8
  00087ee8: mov r4,r0
  00087eea: ldr r0,[r0,#0x40]
  00087eec: cbz r0,0x00087ef2
  00087eee: blx 0x0006ebfc
  00087ef2: ldr r0,[r4,#0x48]
  00087ef4: movs r5,#0x0
  00087ef6: str r5,[r4,#0x40]
  00087ef8: cbz r0,0x00087efe
  00087efa: blx 0x0006ebfc
  00087efe: mov r0,r4
  00087f00: str r5,[r4,#0x48]
  00087f02: pop {r4,r5,r7,pc}
```
