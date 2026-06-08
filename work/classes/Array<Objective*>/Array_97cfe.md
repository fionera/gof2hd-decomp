# Array<Objective*>::Array
elf 0x97cfe body 28
Sig: undefined __thiscall Array(Array<Objective*> * this)

## decompile
```c

/* Array<Objective*>::Array() */

Array<Objective*> * __thiscall Array<Objective*>::Array(Array<Objective*> *this)

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
  000a7cfe: push {r4,r6,r7,lr}
  000a7d00: add r7,sp,#0x8
  000a7d02: mov r4,r0
  000a7d04: movs r0,#0x4
  000a7d06: blx 0x0006ec08
  000a7d0a: movs r2,#0x0
  000a7d0c: movs r1,#0x1
  000a7d0e: str r2,[r0,#0x0]
  000a7d10: strd r2,r0,[r4,#0x0]
  000a7d14: mov r0,r4
  000a7d16: str r1,[r4,#0x8]
  000a7d18: pop {r4,r6,r7,pc}
```
