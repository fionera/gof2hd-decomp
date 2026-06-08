# AbyssEngine::ImageFontGetWidth
elf 0x721e6 body 128
Sig: undefined __stdcall ImageFontGetWidth(ImageFont * param_1, ushort * param_2, uint param_3, uint param_4)

## decompile
```c

/* AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int,
   unsigned int) */

int AbyssEngine::ImageFontGetWidth(ImageFont *param_1,ushort *param_2,uint param_3,uint param_4)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  ushort uVar6;
  uint uVar7;
  int iVar8;
  
  if (param_1 == (ImageFont *)0x0 || param_2 == (ushort *)0x0) {
    return 0;
  }
  uVar7 = param_4 + param_3;
  iVar2 = 0;
  do {
    if (uVar7 <= (param_3 & 0xffff)) {
      return iVar2;
    }
    uVar6 = 0;
    do {
      uVar4 = (uint)uVar6;
      if (*(ushort *)param_1 <= uVar4) goto LAB_00082252;
      uVar6 = uVar6 + 1;
      uVar1 = *(ushort *)(*(int *)(param_1 + 4) + uVar4 * 2);
    } while (uVar1 != param_2[param_3 & 0xffff]);
    iVar8 = (int)*(float *)(*(int *)(*(int *)(*(int *)(param_1 + 0xc) + uVar4 * 4) + 4) + 0xc);
    iVar3 = *(short *)(param_1 + 0x10) + iVar8;
    iVar5 = iVar3;
    if (iVar8 == 0xb) {
      iVar5 = iVar3 + -2;
    }
    if (uVar1 != 0x20) {
      iVar5 = iVar3;
    }
    iVar2 = iVar2 + iVar5;
LAB_00082252:
    param_3 = param_3 + 1;
  } while( true );
}

```

## target disasm
```
  000821e6: mov r12,r0
  000821e8: cmp.w r12,#0x0
  000821ec: mov.w r0,#0x0
  000821f0: it ne
  000821f2: cmp.ne r1,#0x0
  000821f4: bne 0x000821f8
  000821f6: bx lr
  000821f8: push {r4,r5,r6,r7,lr}
  000821fa: add r7,sp,#0xc
  000821fc: push {r8,r9,r11}
  00082200: add.w lr,r3,r2
  00082204: movs r0,#0x0
  00082206: b 0x00082254
  00082208: ldrh.w r8,[r12,#0x0]
  0008220c: movs r6,#0x0
  0008220e: uxth r4,r6
  00082210: cmp r4,r8
  00082212: bcs 0x00082252
  00082214: ldr.w r5,[r12,#0x4]
  00082218: adds r6,#0x1
  0008221a: ldrh.w r3,[r1,r9,lsl #0x1]
  0008221e: ldrh.w r5,[r5,r4,lsl #0x1]
  00082222: cmp r5,r3
  00082224: bne 0x0008220e
  00082226: ldr.w r3,[r12,#0xc]
  0008222a: ldr.w r3,[r3,r4,lsl #0x2]
  0008222e: ldr r3,[r3,#0x4]
  00082230: vldr.32 s0,[r3,#0xc]
  00082234: ldrsh.w r3,[r12,#0x10]
  00082238: vcvt.s32.f32 s0,s0
  0008223c: vmov r4,s0
  00082240: add r3,r4
  00082242: cmp r4,#0xb
  00082244: mov r4,r3
  00082246: it eq
  00082248: sub.eq r4,#0x2
  0008224a: cmp r5,#0x20
  0008224c: it ne
  0008224e: mov.ne r4,r3
  00082250: add r0,r4
  00082252: adds r2,#0x1
  00082254: uxth.w r9,r2
  00082258: cmp lr,r9
  0008225a: bhi 0x00082208
  0008225c: pop.w {r8,r9,r11}
  00082260: pop.w {r4,r5,r6,r7,lr}
  00082264: bx lr
```
