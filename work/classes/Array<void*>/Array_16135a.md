# Array<void*>::Array
elf 0x16135a body 28
Sig: undefined __thiscall Array(Array<void*> * this)

## decompile
```c

/* Array<void*>::Array() */

Array<void*> * __thiscall Array<void*>::Array(Array<void*> *this)

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
  0017135a: push {r4,r6,r7,lr}
  0017135c: add r7,sp,#0x8
  0017135e: mov r4,r0
  00171360: movs r0,#0x4
  00171362: blx 0x0006ec08
  00171366: movs r2,#0x0
  00171368: movs r1,#0x1
  0017136a: str r2,[r0,#0x0]
  0017136c: strd r2,r0,[r4,#0x0]
  00171370: mov r0,r4
  00171372: str r1,[r4,#0x8]
  00171374: pop {r4,r6,r7,pc}
```
