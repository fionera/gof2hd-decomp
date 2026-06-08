# Array<AEPakFileEntry*>::Array
elf 0x7021c body 28
Sig: undefined __thiscall Array(Array<AEPakFileEntry*> * this)

## decompile
```c

/* Array<AEPakFileEntry*>::Array() */

Array<AEPakFileEntry*> * __thiscall Array<AEPakFileEntry*>::Array(Array<AEPakFileEntry*> *this)

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
  0008021c: push {r4,r6,r7,lr}
  0008021e: add r7,sp,#0x8
  00080220: mov r4,r0
  00080222: movs r0,#0x4
  00080224: blx 0x0006ec08
  00080228: movs r2,#0x0
  0008022a: movs r1,#0x1
  0008022c: str r2,[r0,#0x0]
  0008022e: strd r2,r0,[r4,#0x0]
  00080232: mov r0,r4
  00080234: str r1,[r4,#0x8]
  00080236: pop {r4,r6,r7,pc}
```
