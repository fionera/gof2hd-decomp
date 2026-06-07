# AERandom::nextInt
elf 0x71aa4 body 40
Sig: undefined __thiscall nextInt(AERandom * this)

## decompile
```c

/* AbyssEngine::AERandom::nextInt() */

uint __thiscall AbyssEngine::AERandom::nextInt(AERandom *this)

{
  longlong lVar1;
  uint uVar2;
  uint uVar3;
  
  lVar1 = (ulonglong)*(uint *)this * (ulonglong)DAT_00081acc;
  uVar2 = (uint)lVar1;
  uVar3 = uVar2 + 0xb;
  uVar2 = *(uint *)this * 5 + (int)((ulonglong)lVar1 >> 0x20) + DAT_00081acc * *(int *)(this + 4) +
          (uint)(0xfffffff4 < uVar2);
  *(uint *)this = uVar3;
  *(uint *)(this + 4) = uVar2 & 0xffff;
  return uVar3 >> 0x10 | uVar2 * 0x10000;
}

```

## target disasm
```
  00081aa4: push {r4,r5,r7,lr}
  00081aa6: add r7,sp,#0x8
  00081aa8: ldr r5,[0x00081acc]
  00081aaa: ldrd r2,r3,[r0,#0x0]
  00081aae: muls r3,r5
  00081ab0: umull r1,r4,r2,r5
  00081ab4: add.w r2,r2,r2, lsl #0x2
  00081ab8: add r2,r4
  00081aba: adds r1,#0xb
  00081abc: adcs r2,r3
  00081abe: uxth r3,r2
  00081ac0: strd r1,r3,[r0,#0x0]
  00081ac4: lsrs r0,r1,#0x10
  00081ac6: orr.w r0,r0,r2, lsl #0x10
  00081aca: pop {r4,r5,r7,pc}
```
