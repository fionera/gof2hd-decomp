# AbyssEngine::ImageFontGetWidth
elf 0x7213e body 124
Sig: undefined __stdcall ImageFontGetWidth(ImageFont * param_1, ushort * param_2, uint param_3)

## decompile
```c

/* AbyssEngine::ImageFontGetWidth(AbyssEngine::ImageFont*, unsigned short const*, unsigned int) */

int AbyssEngine::ImageFontGetWidth(ImageFont *param_1,ushort *param_2,uint param_3)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  ushort uVar6;
  ushort uVar7;
  int iVar8;
  
  iVar2 = 0;
  if (param_1 != (ImageFont *)0x0 && param_2 != (ushort *)0x0) {
    iVar2 = 0;
    for (uVar7 = 0; uVar7 < param_3; uVar7 = uVar7 + 1) {
      uVar6 = 0;
      do {
        uVar4 = (uint)uVar6;
        if (*(ushort *)param_1 <= uVar4) goto LAB_000821a8;
        uVar6 = uVar6 + 1;
        uVar1 = *(ushort *)(*(int *)(param_1 + 4) + uVar4 * 2);
      } while (uVar1 != param_2[uVar7]);
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
LAB_000821a8:
    }
  }
  return iVar2;
}

```

## target disasm
```
  0008213e: push {r4,r5,r6,r7,lr}
  00082140: add r7,sp,#0xc
  00082142: push {r8,r9,r11}
  00082146: mov lr,r0
  00082148: cmp.w lr,#0x0
  0008214c: mov.w r0,#0x0
  00082150: it ne
  00082152: cmp.ne r1,#0x0
  00082154: beq 0x000821b4
  00082156: movs r0,#0x0
  00082158: mov.w r12,#0x0
  0008215c: b 0x000821ac
  0008215e: ldrh.w r8,[lr,#0x0]
  00082162: movs r6,#0x0
  00082164: uxth r4,r6
  00082166: cmp r4,r8
  00082168: bcs 0x000821a8
  0008216a: ldr.w r5,[lr,#0x4]
  0008216e: adds r6,#0x1
  00082170: ldrh.w r3,[r1,r9,lsl #0x1]
  00082174: ldrh.w r5,[r5,r4,lsl #0x1]
  00082178: cmp r5,r3
  0008217a: bne 0x00082164
  0008217c: ldr.w r3,[lr,#0xc]
  00082180: ldr.w r3,[r3,r4,lsl #0x2]
  00082184: ldr r3,[r3,#0x4]
  00082186: vldr.32 s0,[r3,#0xc]
  0008218a: ldrsh.w r3,[lr,#0x10]
  0008218e: vcvt.s32.f32 s0,s0
  00082192: vmov r4,s0
  00082196: add r3,r4
  00082198: cmp r4,#0xb
  0008219a: mov r4,r3
  0008219c: it eq
  0008219e: sub.eq r4,#0x2
  000821a0: cmp r5,#0x20
  000821a2: it ne
  000821a4: mov.ne r4,r3
  000821a6: add r0,r4
  000821a8: add.w r12,r12,#0x1
  000821ac: uxth.w r9,r12
  000821b0: cmp r9,r2
  000821b2: bcc 0x0008215e
  000821b4: pop.w {r8,r9,r11}
  000821b8: pop {r4,r5,r6,r7,pc}
```
