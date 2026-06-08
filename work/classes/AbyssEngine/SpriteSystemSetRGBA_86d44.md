# AbyssEngine::SpriteSystemSetRGBA
elf 0x86d44 body 116
Sig: undefined __stdcall SpriteSystemSetRGBA(ushort param_1, float param_2, float param_3, float param_4, float param_5, SpriteSystem * param_6)

## decompile
```c

/* AbyssEngine::SpriteSystemSetRGBA(unsigned short, float, float, float, float,
   AbyssEngine::SpriteSystem*) */

void AbyssEngine::SpriteSystemSetRGBA
               (ushort param_1,float param_2,float param_3,float param_4,float param_5,
               SpriteSystem *param_6)

{
  undefined4 *puVar1;
  undefined4 in_r2;
  undefined4 in_r3;
  uint unaff_lr;
  undefined4 in_stack_00000000;
  ushort *in_stack_00000004;
  
  if (in_stack_00000004 != (ushort *)0x0) {
    unaff_lr = (uint)*in_stack_00000004;
  }
  if (in_stack_00000004 != (ushort *)0x0 && param_1 < unaff_lr) {
    puVar1 = (undefined4 *)(*(int *)(*(int *)(in_stack_00000004 + 8) + 0xc) + (uint)param_1 * 0x40);
    *puVar1 = param_6;
    puVar1[1] = in_r2;
    puVar1[2] = in_r3;
    puVar1[3] = in_stack_00000000;
    puVar1[4] = param_6;
    puVar1[5] = in_r2;
    puVar1[6] = in_r3;
    puVar1[7] = in_stack_00000000;
    puVar1[8] = param_6;
    puVar1[9] = in_r2;
    puVar1[10] = in_r3;
    puVar1[0xb] = in_stack_00000000;
    puVar1[0xc] = param_6;
    puVar1[0xd] = in_r2;
    puVar1[0xe] = in_r3;
    puVar1[0xf] = in_stack_00000000;
    return;
  }
  return;
}

```

## target disasm
```
  00096d44: push {r7,lr}
  00096d46: mov r7,sp
  00096d48: ldr.w r12,[r7,#0xc]
  00096d4c: cmp.w r12,#0x0
  00096d50: itt ne
  00096d52: ldrh.w.ne lr,[r12,#0x0]
  00096d56: cmp.ne lr,r0
  00096d58: bhi 0x00096d5c
  00096d5a: pop {r7,pc}
  00096d5c: vmov s6,r1
  00096d60: ldr.w r1,[r12,#0x10]
  00096d64: vldr.32 s0,[r7,#0x8]
  00096d68: vmov s2,r3
  00096d6c: vmov s4,r2
  00096d70: ldr r1,[r1,#0xc]
  00096d72: add.w r0,r1,r0, lsl #0x6
  00096d76: vstr.32 s6,[r0]
  00096d7a: vstr.32 s4,[r0,#0x4]
  00096d7e: vstr.32 s2,[r0,#0x8]
  00096d82: vstr.32 s0,[r0,#0xc]
  00096d86: vstr.32 s6,[r0,#0x10]
  00096d8a: vstr.32 s4,[r0,#0x14]
  00096d8e: vstr.32 s2,[r0,#0x18]
  00096d92: vstr.32 s0,[r0,#0x1c]
  00096d96: vstr.32 s6,[r0,#0x20]
  00096d9a: vstr.32 s4,[r0,#0x24]
  00096d9e: vstr.32 s2,[r0,#0x28]
  00096da2: vstr.32 s0,[r0,#0x2c]
  00096da6: vstr.32 s6,[r0,#0x30]
  00096daa: vstr.32 s4,[r0,#0x34]
  00096dae: vstr.32 s2,[r0,#0x38]
  00096db2: vstr.32 s0,[r0,#0x3c]
  00096db6: pop {r7,pc}
```
