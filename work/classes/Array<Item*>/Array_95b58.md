# Array<Item*>::Array
elf 0x95b58 body 28
Sig: undefined __thiscall Array(Array<Item*> * this)

## decompile
```c

/* Array<Item*>::Array() */

Array<Item*> * __thiscall Array<Item*>::Array(Array<Item*> *this)

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
  000a5b58: push {r4,r6,r7,lr}
  000a5b5a: add r7,sp,#0x8
  000a5b5c: mov r4,r0
  000a5b5e: movs r0,#0x4
  000a5b60: blx 0x0006ec08
  000a5b64: movs r2,#0x0
  000a5b66: movs r1,#0x1
  000a5b68: str r2,[r0,#0x0]
  000a5b6a: strd r2,r0,[r4,#0x0]
  000a5b6e: mov r0,r4
  000a5b70: str r1,[r4,#0x8]
  000a5b72: pop {r4,r6,r7,pc}
```
