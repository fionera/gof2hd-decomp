# Array<RepairBeam*>::~Array
elf 0x9a25e body 22
Sig: undefined __thiscall ~Array(Array<RepairBeam*> * this)

## decompile
```c

/* Array<RepairBeam*>::~Array() */

Array<RepairBeam*> * __thiscall Array<RepairBeam*>::~Array(Array<RepairBeam*> *this)

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
  000aa25e: push {r4,r6,r7,lr}
  000aa260: add r7,sp,#0x8
  000aa262: mov r4,r0
  000aa264: ldr r0,[r0,#0x4]
  000aa266: cbz r0,0x000aa26c
  000aa268: blx 0x0006ebfc
  000aa26c: movs r0,#0x0
  000aa26e: str r0,[r4,#0x4]
  000aa270: mov r0,r4
  000aa272: pop {r4,r6,r7,pc}
```
