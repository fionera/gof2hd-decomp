# Array<Mission*>::Array
elf 0xa8caa body 28
Sig: undefined __thiscall Array(Array<Mission*> * this)

## decompile
```c

/* Array<Mission*>::Array() */

Array<Mission*> * __thiscall Array<Mission*>::Array(Array<Mission*> *this)

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
  000b8caa: push {r4,r6,r7,lr}
  000b8cac: add r7,sp,#0x8
  000b8cae: mov r4,r0
  000b8cb0: movs r0,#0x4
  000b8cb2: blx 0x0006ec08
  000b8cb6: movs r2,#0x0
  000b8cb8: movs r1,#0x1
  000b8cba: str r2,[r0,#0x0]
  000b8cbc: strd r2,r0,[r4,#0x0]
  000b8cc0: mov r0,r4
  000b8cc2: str r1,[r4,#0x8]
  000b8cc4: pop {r4,r6,r7,pc}
```
