# Array<Player*>::Array
elf 0xa2b50 body 28
Sig: undefined __thiscall Array(Array<Player*> * this)

## decompile
```c

/* Array<Player*>::Array() */

Array<Player*> * __thiscall Array<Player*>::Array(Array<Player*> *this)

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
  000b2b50: push {r4,r6,r7,lr}
  000b2b52: add r7,sp,#0x8
  000b2b54: mov r4,r0
  000b2b56: movs r0,#0x4
  000b2b58: blx 0x0006ec08
  000b2b5c: movs r2,#0x0
  000b2b5e: movs r1,#0x1
  000b2b60: str r2,[r0,#0x0]
  000b2b62: strd r2,r0,[r4,#0x0]
  000b2b66: mov r0,r4
  000b2b68: str r1,[r4,#0x8]
  000b2b6a: pop {r4,r6,r7,pc}
```
