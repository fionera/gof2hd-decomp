# Engine::LightSetLightColorAmbient
elf 0x85a94 body 198
Sig: undefined __stdcall LightSetLightColorAmbient(float param_1, float param_2, float param_3, uint param_4)

## decompile
```c

/* AbyssEngine::Engine::LightSetLightColorAmbient(float, float, float, unsigned int) */

void AbyssEngine::Engine::LightSetLightColorAmbient
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
  puVar2 = (undefined4 *)(DAT_00095b5c + 0x95ac2);
  iVar1 = in_stack_00000000 + -0x3fff;
  if (in_stack_00000000 + -0x3fff < *(int *)(param_4 + 0x32c)) {
    iVar1 = *(int *)(param_4 + 0x32c);
  }
  *(int *)(param_4 + 0x32c) = iVar1;
  iVar1 = param_4 + uVar5 * 0x10;
  pcVar3 = (char *)*puVar2;
  *(float *)(iVar1 + 0x268) = in_r1;
  *(undefined4 *)(iVar1 + 0x26c) = in_r2;
  *(undefined4 *)(iVar1 + 0x270) = in_r3;
  *(undefined4 *)(iVar1 + 0x274) = 0x3f800000;
  if (*pcVar3 == '\0') {
    (*(code *)(DAT_001ab694 + 0x1ab698))(in_stack_00000000,0x1200,iVar1 + 0x268);
    return;
  }
  iVar4 = param_4 + uVar5 * 0xc;
  *(float *)(iVar4 + 0x2cc) = (*(float *)(param_4 + 0x288) + in_r1) * *(float *)(param_4 + 0x2a8);
  *(float *)(iVar4 + 0x2d0) =
       (*(float *)(param_4 + 0x28c) + *(float *)(iVar1 + 0x26c)) * *(float *)(param_4 + 0x2ac);
  *(float *)(iVar4 + 0x2d4) =
       (*(float *)(param_4 + 0x290) + *(float *)(iVar1 + 0x270)) * *(float *)(param_4 + 0x2b0);
  (*(code *)(&LAB_001ab668 + DAT_001ab664))();
  return;
}

```

## target disasm
```
  00095a94: push {r7,lr}
  00095a96: mov r7,sp
  00095a98: ldr.w r12,[r7,#0x8]
  00095a9c: sub.w lr,r12,#0x4000
  00095aa0: cmp.w lr,#0x7
  00095aa4: it hi
  00095aa6: pop.hi {r7,pc}
  00095aa8: vmov s4,r2
  00095aac: ldr r2,[0x00095b5c]
  00095aae: vmov s2,r3
  00095ab2: ldr.w r3,[r0,#0x32c]
  00095ab6: vmov s0,r1
  00095aba: movw r1,#0x3fff
  00095abe: add r2,pc
  00095ac0: sub.w r1,r12,r1
  00095ac4: cmp r3,r1
  00095ac6: it gt
  00095ac8: mov.gt r1,r3
  00095aca: str.w r1,[r0,#0x32c]
  00095ace: add.w r1,r0,lr, lsl #0x4
  00095ad2: ldr r2,[r2,#0x0]
  00095ad4: mov.w r3,#0x3f800000
  00095ad8: vstr.32 s0,[r1,#0x268]
  00095adc: vstr.32 s4,[r1,#0x26c]
  00095ae0: vstr.32 s2,[r1,#0x270]
  00095ae4: str.w r3,[r1,#0x274]
  00095ae8: ldrb r2,[r2,#0x0]
  00095aea: cbz r2,0x00095b48
  00095aec: vldr.32 s2,[r0,#0x288]
  00095af0: add.w r3,lr,lr, lsl #0x1
  00095af4: vldr.32 s4,[r0,#0x2a8]
  00095af8: add.w r2,r1,#0x270
  00095afc: vadd.f32 s0,s2,s0
  00095b00: add.w r3,r0,r3, lsl #0x2
  00095b04: add.w r1,r1,#0x26c
  00095b08: vmul.f32 s0,s0,s4
  00095b0c: vstr.32 s0,[r3,#0x2cc]
  00095b10: vldr.32 s0,[r0,#0x28c]
  00095b14: vldr.32 s4,[r1]
  00095b18: vldr.32 s2,[r0,#0x2ac]
  00095b1c: vadd.f32 s0,s0,s4
  00095b20: vmul.f32 s0,s0,s2
  00095b24: vstr.32 s0,[r3,#0x2d0]
  00095b28: vldr.32 s0,[r0,#0x290]
  00095b2c: vldr.32 s4,[r2]
  00095b30: vldr.32 s2,[r0,#0x2b0]
  00095b34: vadd.f32 s0,s0,s4
  00095b38: vmul.f32 s0,s0,s2
  00095b3c: vstr.32 s0,[r3,#0x2d4]
  00095b40: pop.w {r7,lr}
  00095b44: b.w 0x001ab658
  00095b48: add.w r2,r1,#0x268
  00095b4c: mov r0,r12
  00095b4e: mov.w r1,#0x1200
  00095b52: pop.w {r7,lr}
  00095b56: b.w 0x001ab688
```
