# Array<PendingProduct*>::Array
elf 0xa8f92 body 28
Sig: undefined __thiscall Array(Array<PendingProduct*> * this)

## decompile
```c

/* Array<PendingProduct*>::Array() */

Array<PendingProduct*> * __thiscall Array<PendingProduct*>::Array(Array<PendingProduct*> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  000b8f92: push {r4,r6,r7,lr}
  000b8f94: add r7,sp,#0x8
  000b8f96: mov r4,r0
  000b8f98: movs r0,#0x4
  000b8f9a: blx 0x0006ec08
  000b8f9e: movs r2,#0x0
  000b8fa0: movs r1,#0x1
  000b8fa2: str r2,[r0,#0x0]
  000b8fa4: strd r2,r0,[r4,#0x0]
  000b8fa8: mov r0,r4
  000b8faa: str r1,[r4,#0x8]
  000b8fac: pop {r4,r6,r7,pc}
```
