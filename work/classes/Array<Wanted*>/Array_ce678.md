# Array<Wanted*>::Array
elf 0xce678 body 28
Sig: undefined __thiscall Array(Array<Wanted*> * this)

## decompile
```c

/* Array<Wanted*>::Array() */

Array<Wanted*> * __thiscall Array<Wanted*>::Array(Array<Wanted*> *this)

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
  000de678: push {r4,r6,r7,lr}
  000de67a: add r7,sp,#0x8
  000de67c: mov r4,r0
  000de67e: movs r0,#0x4
  000de680: blx 0x0006ec08
  000de684: movs r2,#0x0
  000de686: movs r1,#0x1
  000de688: str r2,[r0,#0x0]
  000de68a: strd r2,r0,[r4,#0x0]
  000de68e: mov r0,r4
  000de690: str r1,[r4,#0x8]
  000de692: pop {r4,r6,r7,pc}
```
