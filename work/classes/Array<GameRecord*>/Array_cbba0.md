# Array<GameRecord*>::Array
elf 0xcbba0 body 28
Sig: undefined __thiscall Array(Array<GameRecord*> * this)

## decompile
```c

/* Array<GameRecord*>::Array() */

Array<GameRecord*> * __thiscall Array<GameRecord*>::Array(Array<GameRecord*> *this)

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
  000dbba0: push {r4,r6,r7,lr}
  000dbba2: add r7,sp,#0x8
  000dbba4: mov r4,r0
  000dbba6: movs r0,#0x4
  000dbba8: blx 0x0006ec08
  000dbbac: movs r2,#0x0
  000dbbae: movs r1,#0x1
  000dbbb0: str r2,[r0,#0x0]
  000dbbb2: strd r2,r0,[r4,#0x0]
  000dbbb6: mov r0,r4
  000dbbb8: str r1,[r4,#0x8]
  000dbbba: pop {r4,r6,r7,pc}
```
