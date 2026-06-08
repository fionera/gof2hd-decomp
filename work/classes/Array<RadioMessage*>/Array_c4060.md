# Array<RadioMessage*>::Array
elf 0xc4060 body 28
Sig: undefined __thiscall Array(Array<RadioMessage*> * this)

## decompile
```c

/* Array<RadioMessage*>::Array() */

Array<RadioMessage*> * __thiscall Array<RadioMessage*>::Array(Array<RadioMessage*> *this)

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
  000d4060: push {r4,r6,r7,lr}
  000d4062: add r7,sp,#0x8
  000d4064: mov r4,r0
  000d4066: movs r0,#0x4
  000d4068: blx 0x0006ec08
  000d406c: movs r2,#0x0
  000d406e: movs r1,#0x1
  000d4070: str r2,[r0,#0x0]
  000d4072: strd r2,r0,[r4,#0x0]
  000d4076: mov r0,r4
  000d4078: str r1,[r4,#0x8]
  000d407a: pop {r4,r6,r7,pc}
```
