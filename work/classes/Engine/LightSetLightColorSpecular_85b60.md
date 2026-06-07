# Engine::LightSetLightColorSpecular
elf 0x85b60 body 174
Sig: undefined __stdcall LightSetLightColorSpecular(float param_1, float param_2, float param_3, uint param_4)

## decompile
```c

/* AbyssEngine::Engine::LightSetLightColorSpecular(float, float, float, unsigned int) */

void AbyssEngine::Engine::LightSetLightColorSpecular
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
  puVar2 = (undefined4 *)(DAT_00095c10 + 0x95b8e);
  iVar1 = in_stack_00000000 + -0x3fff;
  if (in_stack_00000000 + -0x3fff < *(int *)(param_4 + 0x32c)) {
    iVar1 = *(int *)(param_4 + 0x32c);
  }
  *(int *)(param_4 + 0x32c) = iVar1;
  iVar1 = param_4 + uVar5 * 0x10;
  pcVar3 = (char *)*puVar2;
  *(float *)(iVar1 + 0x248) = in_r1;
  *(undefined4 *)(iVar1 + 0x24c) = in_r2;
  *(undefined4 *)(iVar1 + 0x250) = in_r3;
  *(undefined4 *)(iVar1 + 0x254) = 0x3f800000;
  if (*pcVar3 == '\0') {
    (*(code *)(DAT_001ab694 + 0x1ab698))(in_stack_00000000,0x1202,iVar1 + 0x248);
    return;
  }
  iVar4 = param_4 + uVar5 * 0xc;
  *(float *)(iVar4 + 0x2e4) = *(float *)(param_4 + 0x2b8) * in_r1;
  *(float *)(iVar4 + 0x2e8) = *(float *)(iVar1 + 0x24c) * *(float *)(param_4 + 700);
  *(float *)(iVar4 + 0x2ec) = *(float *)(iVar1 + 0x250) * *(float *)(param_4 + 0x2c0);
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095b60: push {r7,lr}
  00095b62: mov r7,sp
  00095b64: ldr.w r12,[r7,#0x8]
  00095b68: sub.w lr,r12,#0x4000
  00095b6c: cmp.w lr,#0x7
  00095b70: it hi
  00095b72: pop.hi {r7,pc}
  00095b74: vmov s4,r2
  00095b78: ldr r2,[0x00095c10]
  00095b7a: vmov s2,r3
  00095b7e: ldr.w r3,[r0,#0x32c]
  00095b82: vmov s0,r1
  00095b86: movw r1,#0x3fff
  00095b8a: add r2,pc
  00095b8c: sub.w r1,r12,r1
  00095b90: cmp r3,r1
  00095b92: it gt
  00095b94: mov.gt r1,r3
  00095b96: str.w r1,[r0,#0x32c]
  00095b9a: add.w r1,r0,lr, lsl #0x4
  00095b9e: ldr r2,[r2,#0x0]
  00095ba0: mov.w r3,#0x3f800000
  00095ba4: vstr.32 s0,[r1,#0x248]
  00095ba8: vstr.32 s4,[r1,#0x24c]
  00095bac: vstr.32 s2,[r1,#0x250]
  00095bb0: str.w r3,[r1,#0x254]
  00095bb4: ldrb r2,[r2,#0x0]
  00095bb6: cbz r2,0x00095bfc
  00095bb8: vldr.32 s2,[r0,#0x2b8]
  00095bbc: add.w r3,lr,lr, lsl #0x1
  00095bc0: add.w r2,r1,#0x250
  00095bc4: add.w r1,r1,#0x24c
  00095bc8: vmul.f32 s0,s2,s0
  00095bcc: add.w r3,r0,r3, lsl #0x2
  00095bd0: vstr.32 s0,[r3,#0x2e4]
  00095bd4: vldr.32 s0,[r0,#0x2bc]
  00095bd8: vldr.32 s2,[r1]
  00095bdc: vmul.f32 s0,s2,s0
  00095be0: vstr.32 s0,[r3,#0x2e8]
  00095be4: vldr.32 s0,[r0,#0x2c0]
  00095be8: vldr.32 s2,[r2]
  00095bec: vmul.f32 s0,s2,s0
  00095bf0: vstr.32 s0,[r3,#0x2ec]
  00095bf4: pop.w {r7,lr}
  00095bf8: b.w 0x001ab658
  00095bfc: add.w r2,r1,#0x248
  00095c00: mov r0,r12
  00095c02: movw r1,#0x1202
  00095c06: pop.w {r7,lr}
  00095c0a: b.w 0x001ab688
```
