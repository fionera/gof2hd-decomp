# AERandom::next
elf 0x71a54 body 76
Sig: undefined __thiscall next(AERandom * this, int param_1)

## decompile
```c

/* AbyssEngine::AERandom::next(int) */

uint __thiscall AbyssEngine::AERandom::next(AERandom *this,int param_1)

{
  longlong lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  lVar1 = (ulonglong)*(uint *)this * (ulonglong)DAT_00081aa0;
  uVar3 = (uint)lVar1;
  uVar4 = uVar3 + 0xb;
  uVar2 = DAT_00081aa0 * *(int *)(this + 4) + *(uint *)this * 5 + (int)((ulonglong)lVar1 >> 0x20) +
          (uint)(0xfffffff4 < uVar3) & 0xffff;
  *(uint *)this = uVar4;
  *(uint *)(this + 4) = uVar2;
  uVar3 = uVar4 >> (0x30U - param_1 & 0xff) | uVar2 << (0x20 - (0x30U - param_1) & 0xff);
  if (-1 < (int)(0x10U - param_1)) {
    uVar3 = uVar2 >> (0x10U - param_1 & 0xff);
  }
  return uVar3;
}

```

## target disasm
```
  00081a54: push {r4,r5,r6,r7,lr}
  00081a56: add r7,sp,#0xc
  00081a58: push.w r11
  00081a5c: ldr r6,[0x00081aa0]
  00081a5e: ldrd r3,r2,[r0,#0x0]
  00081a62: muls r2,r6
  00081a64: umull r4,r5,r3,r6
  00081a68: add.w r3,r3,r3, lsl #0x2
  00081a6c: add r3,r5
  00081a6e: adds.w r6,r4,#0xb
  00081a72: adcs r2,r3
  00081a74: uxth r2,r2
  00081a76: strd r6,r2,[r0,#0x0]
  00081a7a: rsb.w r0,r1,#0x30
  00081a7e: rsb.w r3,r0,#0x20
  00081a82: rsb.w r1,r1,#0x10
  00081a86: lsr.w r0,r6,r0
  00081a8a: cmp r1,#0x0
  00081a8c: lsl.w r3,r2,r3
  00081a90: orr.w r0,r0,r3
  00081a94: it ge
  00081a96: lsr.ge.w r0,r2,r1
  00081a9a: pop.w r11
  00081a9e: pop {r4,r5,r6,r7,pc}
```
