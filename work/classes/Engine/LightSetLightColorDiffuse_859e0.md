# Engine::LightSetLightColorDiffuse
elf 0x859e0 body 174
Sig: undefined __stdcall LightSetLightColorDiffuse(float param_1, float param_2, float param_3, uint param_4)

## decompile
```c

/* AbyssEngine::Engine::LightSetLightColorDiffuse(float, float, float, unsigned int) */

void AbyssEngine::Engine::LightSetLightColorDiffuse
               (float param_1,float param_2,float param_3,uint param_4)

{
  float in_r1;
  int iVar1;
  undefined4 in_r2;
  undefined4 *puVar2;
  char *pcVar3;
  undefined4 in_r3;
  int iVar4;
  uint uVar5;
  int in_stack_00000000;
  
  uVar5 = in_stack_00000000 - 0x4000;
  if (7 < uVar5) {
    return;
  }
  puVar2 = (undefined4 *)(DAT_00095a90 + 0x95a0e);
  iVar1 = in_stack_00000000 + -0x3fff;
  if (in_stack_00000000 + -0x3fff < *(int *)(param_4 + 0x32c)) {
    iVar1 = *(int *)(param_4 + 0x32c);
  }
  *(int *)(param_4 + 0x32c) = iVar1;
  iVar1 = param_4 + uVar5 * 0x10;
  pcVar3 = (char *)*puVar2;
  *(float *)(iVar1 + 0x228) = in_r1;
  *(undefined4 *)(iVar1 + 0x22c) = in_r2;
  *(undefined4 *)(iVar1 + 0x230) = in_r3;
  *(undefined4 *)(iVar1 + 0x234) = 0x3f800000;
  if (*pcVar3 == '\0') {
    (*(code *)(DAT_001ab694 + 0x1ab698))(in_stack_00000000,0x1201,iVar1 + 0x228);
    return;
  }
  iVar4 = param_4 + uVar5 * 0xc;
  *(float *)(iVar4 + 0x2fc) = *(float *)(param_4 + 0x298) * in_r1;
  *(float *)(iVar4 + 0x300) = *(float *)(iVar1 + 0x22c) * *(float *)(param_4 + 0x29c);
  *(float *)(iVar4 + 0x304) = *(float *)(iVar1 + 0x230) * *(float *)(param_4 + 0x2a0);
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  000959e0: push {r7,lr}
  000959e2: mov r7,sp
  000959e4: ldr.w r12,[r7,#0x8]
  000959e8: sub.w lr,r12,#0x4000
  000959ec: cmp.w lr,#0x7
  000959f0: it hi
  000959f2: pop.hi {r7,pc}
  000959f4: vmov s4,r2
  000959f8: ldr r2,[0x00095a90]
  000959fa: vmov s2,r3
  000959fe: ldr.w r3,[r0,#0x32c]
  00095a02: vmov s0,r1
  00095a06: movw r1,#0x3fff
  00095a0a: add r2,pc
  00095a0c: sub.w r1,r12,r1
  00095a10: cmp r3,r1
  00095a12: it gt
  00095a14: mov.gt r1,r3
  00095a16: str.w r1,[r0,#0x32c]
  00095a1a: add.w r1,r0,lr, lsl #0x4
  00095a1e: ldr r2,[r2,#0x0]
  00095a20: mov.w r3,#0x3f800000
  00095a24: vstr.32 s0,[r1,#0x228]
  00095a28: vstr.32 s4,[r1,#0x22c]
  00095a2c: vstr.32 s2,[r1,#0x230]
  00095a30: str.w r3,[r1,#0x234]
  00095a34: ldrb r2,[r2,#0x0]
  00095a36: cbz r2,0x00095a7c
  00095a38: vldr.32 s2,[r0,#0x298]
  00095a3c: add.w r3,lr,lr, lsl #0x1
  00095a40: add.w r2,r1,#0x230
  00095a44: add.w r1,r1,#0x22c
  00095a48: vmul.f32 s0,s2,s0
  00095a4c: add.w r3,r0,r3, lsl #0x2
  00095a50: vstr.32 s0,[r3,#0x2fc]
  00095a54: vldr.32 s0,[r0,#0x29c]
  00095a58: vldr.32 s2,[r1]
  00095a5c: vmul.f32 s0,s2,s0
  00095a60: vstr.32 s0,[r3,#0x300]
  00095a64: vldr.32 s0,[r0,#0x2a0]
  00095a68: vldr.32 s2,[r2]
  00095a6c: vmul.f32 s0,s2,s0
  00095a70: vstr.32 s0,[r3,#0x304]
  00095a74: pop.w {r7,lr}
  00095a78: b.w 0x001ab658
  00095a7c: add.w r2,r1,#0x228
  00095a80: mov r0,r12
  00095a82: movw r1,#0x1201
  00095a86: pop.w {r7,lr}
  00095a8a: b.w 0x001ab688
```
