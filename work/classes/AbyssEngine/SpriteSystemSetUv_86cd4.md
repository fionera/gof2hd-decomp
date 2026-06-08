# AbyssEngine::SpriteSystemSetUv
elf 0x86cd4 body 108
Sig: undefined __stdcall SpriteSystemSetUv(ushort param_1, float param_2, float param_3, float param_4, float param_5, SpriteSystem * param_6)

## decompile
```c

/* AbyssEngine::SpriteSystemSetUv(unsigned short, float, float, float, float,
   AbyssEngine::SpriteSystem*) */

void AbyssEngine::SpriteSystemSetUv
               (ushort param_1,float param_2,float param_3,float param_4,float param_5,
               SpriteSystem *param_6)

{
  char cVar1;
  undefined4 *puVar2;
  float in_r2;
  undefined4 in_r3;
  uint unaff_lr;
  float in_stack_00000000;
  ushort *in_stack_00000004;
  
  if (in_stack_00000004 != (ushort *)0x0) {
    unaff_lr = (uint)*in_stack_00000004;
  }
  if (in_stack_00000004 != (ushort *)0x0 && param_1 < unaff_lr) {
    cVar1 = **(char **)(DAT_00096d40 + 0x96cfe);
    puVar2 = (undefined4 *)(*(int *)(*(int *)(in_stack_00000004 + 8) + 8) + (uint)param_1 * 0x20);
    *puVar2 = param_6;
    if (cVar1 == '\0') {
      in_r2 = 1.0 - in_r2;
      in_stack_00000000 = 1.0 - in_stack_00000000;
    }
    puVar2[1] = in_r2;
    puVar2[2] = in_r3;
    puVar2[3] = in_r2;
    puVar2[4] = in_r3;
    puVar2[5] = in_stack_00000000;
    puVar2[6] = param_6;
    puVar2[7] = in_stack_00000000;
    return;
  }
  return;
}

```

## target disasm
```
  00096cd4: push {r7,lr}
  00096cd6: mov r7,sp
  00096cd8: ldr.w r12,[r7,#0xc]
  00096cdc: cmp.w r12,#0x0
  00096ce0: itt ne
  00096ce2: ldrh.w.ne lr,[r12,#0x0]
  00096ce6: cmp.ne lr,r0
  00096ce8: bhi 0x00096cec
  00096cea: pop {r7,pc}
  00096cec: vmov s0,r1
  00096cf0: ldr r1,[0x00096d40]
  00096cf2: vmov s6,r2
  00096cf6: ldr.w r2,[r12,#0x10]
  00096cfa: add r1,pc
  00096cfc: vldr.32 s4,[r7,#0x8]
  00096d00: vmov s2,r3
  00096d04: ldr r1,[r1,#0x0]
  00096d06: ldr r2,[r2,#0x8]
  00096d08: ldrb r1,[r1,#0x0]
  00096d0a: add.w r0,r2,r0, lsl #0x5
  00096d0e: cmp r1,#0x0
  00096d10: vstr.32 s0,[r0]
  00096d14: bne 0x00096d22
  00096d16: vmov.f32 s8,0x3f800000
  00096d1a: vsub.f32 s6,s8,s6
  00096d1e: vsub.f32 s4,s8,s4
  00096d22: vstr.32 s6,[r0,#0x4]
  00096d26: vstr.32 s2,[r0,#0x8]
  00096d2a: vstr.32 s6,[r0,#0xc]
  00096d2e: vstr.32 s2,[r0,#0x10]
  00096d32: vstr.32 s4,[r0,#0x14]
  00096d36: vstr.32 s0,[r0,#0x18]
  00096d3a: vstr.32 s4,[r0,#0x1c]
  00096d3e: pop {r7,pc}
```
