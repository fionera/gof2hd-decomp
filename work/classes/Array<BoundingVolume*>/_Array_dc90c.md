# Array<BoundingVolume*>::~Array
elf 0xdc90c body 22
Sig: undefined __thiscall ~Array(Array<BoundingVolume*> * this)

## decompile
```c

/* Array<BoundingVolume*>::~Array() */

Array<BoundingVolume*> * __thiscall Array<BoundingVolume*>::~Array(Array<BoundingVolume*> *this)

{
  if (*(void **)(this + 4) != (void *)0x0) {
    operator_delete__(*(void **)(this + 4));
  }
  *(undefined4 *)(this + 4) = 0;
  return this;
}

```

## target disasm
```
  000ec90c: push {r4,r6,r7,lr}
  000ec90e: add r7,sp,#0x8
  000ec910: mov r4,r0
  000ec912: ldr r0,[r0,#0x4]
  000ec914: cbz r0,0x000ec91a
  000ec916: blx 0x0006ebfc
  000ec91a: movs r0,#0x0
  000ec91c: str r0,[r4,#0x4]
  000ec91e: mov r0,r4
  000ec920: pop {r4,r6,r7,pc}
```
