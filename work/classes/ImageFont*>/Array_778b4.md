# ImageFont*>::Array
elf 0x778b4 body 28
Sig: undefined __thiscall Array(Array<AbyssEngine::ImageFont*> * this)

## decompile
```c

/* Array<AbyssEngine::ImageFont*>::Array() */

Array<AbyssEngine::ImageFont*> * __thiscall
Array<AbyssEngine::ImageFont*>::Array(Array<AbyssEngine::ImageFont*> *this)

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
  000878b4: push {r4,r6,r7,lr}
  000878b6: add r7,sp,#0x8
  000878b8: mov r4,r0
  000878ba: movs r0,#0x4
  000878bc: blx 0x0006ec08
  000878c0: movs r2,#0x0
  000878c2: movs r1,#0x1
  000878c4: str r2,[r0,#0x0]
  000878c6: strd r2,r0,[r4,#0x0]
  000878ca: mov r0,r4
  000878cc: str r1,[r4,#0x8]
  000878ce: pop {r4,r6,r7,pc}
```
