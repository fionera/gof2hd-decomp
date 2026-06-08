# Array<AEGeometry*>::~Array
elf 0x951f6 body 22
Sig: undefined __thiscall ~Array(Array<AEGeometry*> * this)

## decompile
```c

/* Array<AEGeometry*>::~Array() */

Array<AEGeometry*> * __thiscall Array<AEGeometry*>::~Array(Array<AEGeometry*> *this)

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
  000a51f6: push {r4,r6,r7,lr}
  000a51f8: add r7,sp,#0x8
  000a51fa: mov r4,r0
  000a51fc: ldr r0,[r0,#0x4]
  000a51fe: cbz r0,0x000a5204
  000a5200: blx 0x0006ebfc
  000a5204: movs r0,#0x0
  000a5206: str r0,[r4,#0x4]
  000a5208: mov r0,r4
  000a520a: pop {r4,r6,r7,pc}
```
