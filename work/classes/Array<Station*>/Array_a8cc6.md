# Array<Station*>::Array
elf 0xa8cc6 body 28
Sig: undefined __thiscall Array(Array<Station*> * this)

## decompile
```c

/* Array<Station*>::Array() */

Array<Station*> * __thiscall Array<Station*>::Array(Array<Station*> *this)

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
  000b8cc6: push {r4,r6,r7,lr}
  000b8cc8: add r7,sp,#0x8
  000b8cca: mov r4,r0
  000b8ccc: movs r0,#0x4
  000b8cce: blx 0x0006ec08
  000b8cd2: movs r2,#0x0
  000b8cd4: movs r1,#0x1
  000b8cd6: str r2,[r0,#0x0]
  000b8cd8: strd r2,r0,[r4,#0x0]
  000b8cdc: mov r0,r4
  000b8cde: str r1,[r4,#0x8]
  000b8ce0: pop {r4,r6,r7,pc}
```
