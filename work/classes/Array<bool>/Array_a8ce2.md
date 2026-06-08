# Array<bool>::Array
elf 0xa8ce2 body 28
Sig: undefined __thiscall Array(Array<bool> * this)

## decompile
```c

/* Array<bool>::Array() */

Array<bool> * __thiscall Array<bool>::Array(Array<bool> *this)

{
  undefined1 *puVar1;
  
  puVar1 = operator_new__(1);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined1 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  000b8ce2: push {r4,r5,r7,lr}
  000b8ce4: add r7,sp,#0x8
  000b8ce6: mov r4,r0
  000b8ce8: movs r0,#0x1
  000b8cea: movs r5,#0x1
  000b8cec: blx 0x0006ec08
  000b8cf0: movs r1,#0x0
  000b8cf2: strb r1,[r0,#0x0]
  000b8cf4: strd r1,r0,[r4,#0x0]
  000b8cf8: mov r0,r4
  000b8cfa: str r5,[r4,#0x8]
  000b8cfc: pop {r4,r5,r7,pc}
```
