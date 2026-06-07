# ParticleSystemSprite::setParticle
elf 0x182c7c body 236
Sig: undefined __stdcall setParticle(Vector * param_1, float param_2, uint param_3, float param_4, float param_5, float param_6, float param_7, bool param_8, float param_9, float param_10, Vector * param_11)

## decompile
```c

/* ParticleSystemSprite::setParticle(AbyssEngine::AEMath::Vector const&, float, unsigned int, float,
   float, float, float, bool, float, float, AbyssEngine::AEMath::Vector const&) */

void ParticleSystemSprite::setParticle
               (Vector *param_1,float param_2,uint param_3,float param_4,float param_5,float param_6
               ,float param_7,bool param_8,float param_9,float param_10,Vector *param_11)

{
  uint in_fpscr;
  float fVar1;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s3;
  float extraout_s3_00;
  float in_stack_00000004;
  float in_stack_00000008;
  int in_stack_00000010;
  
  AbyssEngine::PaintCanvas::SpriteSystemSetPosition
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),*(float *)(param_3 + 4),
             param_4,*(float *)(param_3 + 8));
  AbyssEngine::PaintCanvas::SpriteSystemSetSize
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),
             (short)*(undefined4 *)(param_1 + 0x50) + (short)*(undefined4 *)(param_1 + 0x58));
  AbyssEngine::PaintCanvas::SpriteSystemSetUv
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),in_stack_00000008,
             extraout_s1,in_stack_00000004,extraout_s3);
  if (in_stack_00000010 != 0) {
    param_11 = (Vector *)((uint)param_11 & 0xffffff00);
  }
  VectorUnsignedToFloat((uint)param_11 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  VectorUnsignedToFloat(((uint)param_11 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  VectorUnsignedToFloat((uint)param_11 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = (float)VectorUnsignedToFloat
                           (((uint)param_11 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  AbyssEngine::PaintCanvas::SpriteSystemSetRGBA
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),fVar1 * DAT_00192d68,
             extraout_s1_00,DAT_00192d68,extraout_s3_00);
  return;
}

```

## target disasm
```
  00192c7c: push {r4,r5,r6,r7,lr}
  00192c7e: add r7,sp,#0xc
  00192c80: push.w r8
  00192c84: sub sp,#0x10
  00192c86: mov r4,r0
  00192c88: mov r8,r2
  00192c8a: ldr r0,[r0,#0x8]
  00192c8c: mov r5,r3
  00192c8e: ldrd r2,r12,[r4,#0x50]
  00192c92: ldr r6,[r4,#0x58]
  00192c94: ldr r3,[r1,#0x0]
  00192c96: vldr.32 s0,[r1,#0x4]
  00192c9a: vldr.32 s2,[r1,#0x8]
  00192c9e: adds r1,r6,r2
  00192ca0: vstr.32 s0,[sp]
  00192ca4: uxth r2,r1
  00192ca6: mov r1,r12
  00192ca8: vstr.32 s2,[sp,#0x4]
  00192cac: blx 0x000778d8
  00192cb0: vmov s0,r8
  00192cb4: ldr r0,[r4,#0x8]
  00192cb6: ldrd r2,r1,[r4,#0x50]
  00192cba: ldr r3,[r4,#0x58]
  00192cbc: vcvt.s32.f32 s0,s0
  00192cc0: add r2,r3
  00192cc2: uxth r2,r2
  00192cc4: vmov r3,s0
  00192cc8: blx 0x000778cc
  00192ccc: ldr r0,[r4,#0x8]
  00192cce: ldrd r2,r1,[r4,#0x50]
  00192cd2: ldr r6,[r4,#0x58]
  00192cd4: ldr r3,[r7,#0x8]
  00192cd6: add r2,r6
  00192cd8: vldr.32 s0,[r7,#0x10]
  00192cdc: vldr.32 s2,[r7,#0xc]
  00192ce0: vldr.32 s4,[r7,#0x14]
  00192ce4: uxth r2,r2
  00192ce6: vstr.32 s2,[sp]
  00192cea: vstr.32 s0,[sp,#0x4]
  00192cee: vstr.32 s4,[sp,#0x8]
  00192cf2: blx 0x000788c8
  00192cf6: ldr r0,[r7,#0x18]
  00192cf8: vldr.32 s2,[pc,#0x6c]
  00192cfc: cmp r0,#0x0
  00192cfe: it ne
  00192d00: bic.ne r5,r5,#0xff
  00192d04: lsrs r0,r5,#0x18
  00192d06: vmov s0,r0
  00192d0a: vcvt.f32.u32 s0,s0
  00192d0e: ubfx r0,r5,#0x8,#0x8
  00192d12: vmov s4,r0
  00192d16: uxtb r0,r5
  00192d18: vmov s6,r0
  00192d1c: vcvt.f32.u32 s4,s4
  00192d20: vcvt.f32.u32 s6,s6
  00192d24: ubfx r0,r5,#0x10,#0x8
  00192d28: vmul.f32 s0,s0,s2
  00192d2c: vmul.f32 s4,s4,s2
  00192d30: vmul.f32 s6,s6,s2
  00192d34: vmov r3,s0
  00192d38: vmov s0,r0
  00192d3c: vcvt.f32.u32 s0,s0
  00192d40: ldr r0,[r4,#0x8]
  00192d42: ldrd r2,r1,[r4,#0x50]
  00192d46: ldr r6,[r4,#0x58]
  00192d48: vstr.32 s4,[sp,#0x4]
  00192d4c: add r2,r6
  00192d4e: vstr.32 s6,[sp,#0x8]
  00192d52: uxth r2,r2
  00192d54: vmul.f32 s0,s0,s2
  00192d58: vstr.32 s0,[sp]
  00192d5c: blx 0x000788d4
  00192d60: add sp,#0x10
  00192d62: pop.w r8
  00192d66: pop {r4,r5,r6,r7,pc}
```
