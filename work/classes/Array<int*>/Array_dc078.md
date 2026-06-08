# Array<int*>::Array
elf 0xdc078 body 28
Sig: undefined __thiscall Array(Array<int*> * this)

## decompile
```c

/* Array<int*>::Array() */

Array<int*> * __thiscall Array<int*>::Array(Array<int*> *this)

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
  000ec078: push {r4,r6,r7,lr}
  000ec07a: add r7,sp,#0x8
  000ec07c: mov r4,r0
  000ec07e: movs r0,#0x4
  000ec080: blx 0x0006ec08
  000ec084: movs r2,#0x0
  000ec086: movs r1,#0x1
  000ec088: str r2,[r0,#0x0]
  000ec08a: strd r2,r0,[r4,#0x0]
  000ec08e: mov r0,r4
  000ec090: str r1,[r4,#0x8]
  000ec092: pop {r4,r6,r7,pc}
```
