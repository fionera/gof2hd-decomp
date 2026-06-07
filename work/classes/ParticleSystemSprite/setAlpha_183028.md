# ParticleSystemSprite::setAlpha
elf 0x183028 body 136
Sig: undefined __stdcall setAlpha(int param_1, uint param_2, float param_3)

## decompile
```c

/* ParticleSystemSprite::setAlpha(int, unsigned int, float) */

undefined8 ParticleSystemSprite::setAlpha(int param_1,uint param_2,float param_3)

{
  uint in_r2;
  float in_r3;
  uint in_fpscr;
  float fVar1;
  float in_s1;
  float fVar2;
  float in_s3;
  float fVar3;
  
  fVar1 = (float)VectorUnsignedToFloat(in_r2 & 0xff,(byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorUnsignedToFloat((in_r2 & 0xffff) >> 8,(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = (float)VectorUnsignedToFloat((in_r2 & 0xffffff) >> 0x10,(byte)(in_fpscr >> 0x16) & 3);
  VectorUnsignedToFloat(in_r2 >> 0x18,(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = fVar1 * DAT_001930b0;
  fVar2 = fVar2 * DAT_001930b0;
  fVar3 = fVar3 * DAT_001930b0;
  if (*(char *)(param_1 + 0x45) == '\0') {
    fVar1 = fVar1 * in_r3;
  }
  else {
    fVar2 = fVar2 * in_r3;
    fVar3 = fVar3 * in_r3;
  }
  AbyssEngine::PaintCanvas::SpriteSystemSetRGBA
            (*(uint *)(param_1 + 8),(ushort)*(undefined4 *)(param_1 + 0x54),fVar1,in_s1,fVar2,in_s3)
  ;
  return CONCAT44(fVar2,fVar3);
}

```

## target disasm
```
  00193028: push {r3,r4,r5,r6,r7,lr}
  0019302a: add r7,sp,#0x10
  0019302c: uxtb.w lr,r2
  00193030: ubfx r12,r2,#0x8,#0x8
  00193034: vmov s0,lr
  00193038: vldr.32 s8,[pc,#0x74]
  0019303c: vmov s2,r12
  00193040: vcvt.f32.u32 s0,s0
  00193044: ubfx lr,r2,#0x10,#0x8
  00193048: lsrs r2,r2,#0x18
  0019304a: vcvt.f32.u32 s2,s2
  0019304e: vmov s4,lr
  00193052: vmov s6,r2
  00193056: vcvt.f32.u32 s4,s4
  0019305a: vcvt.f32.u32 s6,s6
  0019305e: ldrb.w r2,[r0,#0x45]
  00193062: vmul.f32 s0,s0,s8
  00193066: cmp r2,#0x0
  00193068: vmul.f32 s2,s2,s8
  0019306c: vmul.f32 s4,s4,s8
  00193070: vmul.f32 s6,s6,s8
  00193074: vmov s8,r3
  00193078: iteee eq
  0019307a: vmul.eq.f32 s0,s0,s8
  0019307e: vmul.ne.f32 s2,s2,s8
  00193082: vmul.ne.f32 s4,s4,s8
  00193086: vmul.ne.f32 s6,s6,s8
  0019308a: ldr.w lr,[r0,#0x8]
  0019308e: ldrd r12,r0,[r0,#0x54]
  00193092: add r0,r1
  00193094: vmov r3,s6
  00193098: uxth r2,r0
  0019309a: mov r0,lr
  0019309c: mov r1,r12
  0019309e: vstr.32 s4,[sp]
  001930a2: vstr.32 s2,[sp,#0x4]
  001930a6: vstr.32 s0,[sp,#0x8]
  001930aa: blx 0x000788d4
  001930ae: pop {r0,r1,r2,r3,r7,pc}
```
