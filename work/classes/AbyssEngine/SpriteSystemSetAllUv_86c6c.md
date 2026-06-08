# AbyssEngine::SpriteSystemSetAllUv
elf 0x86c6c body 102
Sig: undefined __stdcall SpriteSystemSetAllUv(float param_1, float param_2, float param_3, float param_4, SpriteSystem * param_5)

## decompile
```c

/* AbyssEngine::SpriteSystemSetAllUv(float, float, float, float, AbyssEngine::SpriteSystem*) */

void AbyssEngine::SpriteSystemSetAllUv
               (float param_1,float param_2,float param_3,float param_4,SpriteSystem *param_5)

{
  ushort uVar1;
  float in_r1;
  int iVar2;
  undefined4 in_r2;
  uint uVar3;
  float in_r3;
  undefined4 *puVar4;
  ushort *in_stack_00000000;
  
  if (in_stack_00000000 != (ushort *)0x0) {
    uVar1 = *in_stack_00000000;
    iVar2 = *(int *)(*(int *)(in_stack_00000000 + 8) + 8);
    for (uVar3 = 0; uVar3 = uVar3 & 0xffff, uVar3 < (uint)uVar1 << 3; uVar3 = uVar3 + 8) {
      puVar4 = (undefined4 *)(iVar2 + uVar3 * 4);
      *puVar4 = param_5;
      puVar4[1] = 1.0 - in_r1;
      puVar4[2] = in_r2;
      puVar4[3] = 1.0 - in_r1;
      puVar4[4] = in_r2;
      puVar4[5] = 1.0 - in_r3;
      puVar4[6] = param_5;
      puVar4[7] = 1.0 - in_r3;
    }
    return;
  }
  return;
}

```

## target disasm
```
  00096c6c: ldr.w r12,[sp,#0x0]
  00096c70: cmp.w r12,#0x0
  00096c74: beq 0x00096cd0
  00096c76: vmov.f32 s8,0x3f800000
  00096c7a: vmov s6,r1
  00096c7e: ldr.w r1,[r12,#0x10]
  00096c82: vmov s4,r3
  00096c86: vmov s2,r0
  00096c8a: ldrh.w r0,[r12,#0x0]
  00096c8e: ldr r1,[r1,#0x8]
  00096c90: vmov s0,r2
  00096c94: movs r2,#0x0
  00096c96: lsls r0,r0,#0x3
  00096c98: vsub.f32 s4,s8,s4
  00096c9c: vsub.f32 s6,s8,s6
  00096ca0: uxth r2,r2
  00096ca2: cmp r2,r0
  00096ca4: it cs
  00096ca6: bx.cs lr
  00096ca8: add.w r3,r1,r2, lsl #0x2
  00096cac: adds r2,#0x8
  00096cae: vstr.32 s2,[r3]
  00096cb2: vstr.32 s6,[r3,#0x4]
  00096cb6: vstr.32 s0,[r3,#0x8]
  00096cba: vstr.32 s6,[r3,#0xc]
  00096cbe: vstr.32 s0,[r3,#0x10]
  00096cc2: vstr.32 s4,[r3,#0x14]
  00096cc6: vstr.32 s2,[r3,#0x18]
  00096cca: vstr.32 s4,[r3,#0x1c]
  00096cce: b 0x00096ca0
  00096cd0: bx lr
```
