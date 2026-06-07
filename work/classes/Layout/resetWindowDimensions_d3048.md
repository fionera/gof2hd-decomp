# Layout::resetWindowDimensions
elf 0xd3048 body 212
Sig: undefined __thiscall resetWindowDimensions(Layout * this)

## decompile
```c

/* Layout::resetWindowDimensions() */

void __thiscall Layout::resetWindowDimensions(Layout *this)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  float unaff_r4;
  float unaff_r5;
  int *piVar5;
  int *piVar6;
  code *pcVar7;
  
  piVar6 = *(int **)(DAT_000e311c + 0xe305e);
  iVar1 = *piVar6;
  uVar4 = **(undefined4 **)(DAT_000e3124 + 0xe3064);
  uVar2 = **(undefined4 **)(DAT_000e3120 + 0xe3060);
  pcVar7 = *(code **)(DAT_000e3128 + 0xe306c);
  *(undefined4 *)(this + 0x2dc) = 0;
  *(undefined4 *)(this + 0x2e0) = 0;
  *(undefined4 *)(this + 0x2e4) = uVar2;
  *(undefined4 *)(this + 0x2e8) = uVar4;
  (*pcVar7)(*(undefined4 *)(this + 0x3bc),uVar2,0,0x12);
  piVar5 = *(int **)(DAT_000e312c + 0xe3092);
  (*pcVar7)(*(undefined4 *)(this + 0x3b4),*(int *)(*piVar5 + 0x28) + *(int *)(this + 0x2dc),
            (*(int *)(this + 0x2e0) + *(int *)(this + 0x2e8)) - *(int *)(this + 0x3fc),0x21);
  (*pcVar7)(*(undefined4 *)(this + 0x3b8),*(int *)(*piVar5 + 0x28) + *(int *)(this + 0x2dc),
            (*(int *)(this + 0x2e0) + *(int *)(this + 0x2e8)) - *(int *)(this + 0x3fc),0x21);
  if (*(int *)(this + 0x3b4) != 0) {
    TouchButton::getPosition();
    **(int **)(DAT_000e3130 + 0xe30e8) = (int)unaff_r4;
    TouchButton::getPosition();
    **(int **)(DAT_000e3134 + 0xe3104) = (int)unaff_r5;
  }
  iVar3 = *piVar6;
  iVar1 = iVar3 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar3);
  }
  return;
}

```

## target disasm
```
  000e3048: push {r4,r5,r6,r7,lr}
  000e304a: add r7,sp,#0xc
  000e304c: push {r4,r5,r6,r7,r8,r9,r11}
  000e3050: mov r4,r0
  000e3052: ldr r0,[0x000e311c]
  000e3054: ldr r1,[0x000e3120]
  000e3056: movs r5,#0x0
  000e3058: ldr r2,[0x000e3124]
  000e305a: add r0,pc
  000e305c: add r1,pc
  000e305e: ldr r3,[0x000e3128]
  000e3060: add r2,pc
  000e3062: ldr.w r8,[r0,#0x0]
  000e3066: ldr r0,[r1,#0x0]
  000e3068: add r3,pc
  000e306a: ldr r2,[r2,#0x0]
  000e306c: ldr.w r1,[r8,#0x0]
  000e3070: str r1,[sp,#0xc]
  000e3072: ldr r2,[r2,#0x0]
  000e3074: ldr r1,[r0,#0x0]
  000e3076: ldr.w r0,[r4,#0x3bc]
  000e307a: ldr.w r9,[r3,#0x0]
  000e307e: movs r3,#0x12
  000e3080: strd r5,r5,[r4,#0x2dc]
  000e3084: strd r1,r2,[r4,#0x2e4]
  000e3088: movs r2,#0x0
  000e308a: blx r9
  000e308c: ldr r0,[0x000e312c]
  000e308e: add r0,pc
  000e3090: ldr r5,[r0,#0x0]
  000e3092: ldr r1,[r5,#0x0]
  000e3094: ldrd r12,r2,[r4,#0x2dc]
  000e3098: ldr.w r6,[r4,#0x2e8]
  000e309c: ldr r1,[r1,#0x28]
  000e309e: ldr.w r3,[r4,#0x3fc]
  000e30a2: add r2,r6
  000e30a4: ldr.w r0,[r4,#0x3b4]
  000e30a8: add r1,r12
  000e30aa: subs r2,r2,r3
  000e30ac: movs r3,#0x21
  000e30ae: blx r9
  000e30b0: ldr r1,[r5,#0x0]
  000e30b2: ldrd r3,r2,[r4,#0x2dc]
  000e30b6: ldr.w r6,[r4,#0x2e8]
  000e30ba: ldr r1,[r1,#0x28]
  000e30bc: add r2,r6
  000e30be: ldr.w r6,[r4,#0x3fc]
  000e30c2: ldr.w r0,[r4,#0x3b8]
  000e30c6: add r1,r3
  000e30c8: subs r2,r2,r6
  000e30ca: movs r3,#0x21
  000e30cc: blx r9
  000e30ce: ldr.w r1,[r4,#0x3b4]
  000e30d2: cbz r1,0x000e3108
  000e30d4: mov r0,sp
  000e30d6: blx 0x00074dd0
  000e30da: vldr.32 s0,[sp]
  000e30de: ldr r0,[0x000e3130]
  000e30e0: vcvt.s32.f32 s0,s0
  000e30e4: add r0,pc
  000e30e6: ldr r0,[r0,#0x0]
  000e30e8: vstr.32 s0,[r0]
  000e30ec: mov r0,sp
  000e30ee: ldr.w r1,[r4,#0x3b4]
  000e30f2: blx 0x00074dd0
  000e30f6: vldr.32 s0,[sp,#0x4]
  000e30fa: ldr r0,[0x000e3134]
  000e30fc: vcvt.s32.f32 s0,s0
  000e3100: add r0,pc
  000e3102: ldr r0,[r0,#0x0]
  000e3104: vstr.32 s0,[r0]
  000e3108: ldr r0,[sp,#0xc]
  000e310a: ldr.w r1,[r8,#0x0]
  000e310e: subs r0,r1,r0
  000e3110: itt eq
  000e3112: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000e3116: pop.eq {r4,r5,r6,r7,pc}
  000e3118: blx 0x0006e824
```
