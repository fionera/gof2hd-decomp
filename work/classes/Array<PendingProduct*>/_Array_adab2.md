# Array<PendingProduct*>::~Array
elf 0xadab2 body 22
Sig: undefined __thiscall ~Array(Array<PendingProduct*> * this)

## decompile
```c

/* Array<PendingProduct*>::~Array() */

Array<PendingProduct*> * __thiscall Array<PendingProduct*>::~Array(Array<PendingProduct*> *this)

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
  000bdab2: push {r4,r6,r7,lr}
  000bdab4: add r7,sp,#0x8
  000bdab6: mov r4,r0
  000bdab8: ldr r0,[r0,#0x4]
  000bdaba: cbz r0,0x000bdac0
  000bdabc: blx 0x0006ebfc
  000bdac0: movs r0,#0x0
  000bdac2: str r0,[r4,#0x4]
  000bdac4: mov r0,r4
  000bdac6: pop {r4,r6,r7,pc}
```
