# Sprite::draw
elf 0xd0e68 body 282
Sig: undefined __stdcall draw(float param_1, float param_2)

## decompile
```c

/* Sprite::draw(float, float) */

void Sprite::draw(float param_1,float param_2)

{
  bool bVar1;
  uint uVar2;
  int *in_r0;
  float in_r1;
  float in_r2;
  uint in_fpscr;
  float in_s2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  
  if (*in_r0 == 0) {
    AbyssEngine::PaintCanvas::DrawRegion2D
              ((PaintCanvas *)**(undefined4 **)(DAT_000e0f88 + 0xe0ed0),in_r0[1],in_r0[10],
               in_r0[0xb],in_r0[6],in_r0[7],param_1,0,0,0,in_r0[4] - in_r0[2]);
  }
  else {
    bVar1 = in_r1 != 1.0;
    uVar2 = in_fpscr & 0xfffffff;
    if (bVar1) {
      in_s2 = in_r2;
    }
    if (!bVar1 || (!bVar1 || in_s2 == 1.0)) {
      (*(code *)(DAT_001ac074 + 0x1ac078))();
      return;
    }
    fVar4 = (float)VectorSignedToFloat(in_r0[7],(byte)(uVar2 >> 0x16) & 3);
    fVar3 = (float)VectorSignedToFloat(in_r0[6],(byte)(uVar2 >> 0x16) & 3);
    fVar6 = (float)VectorSignedToFloat(in_r0[5] - in_r0[2],(byte)(uVar2 >> 0x16) & 3);
    fVar5 = (float)VectorSignedToFloat(in_r0[4] - in_r0[2],(byte)(uVar2 >> 0x16) & 3);
    AbyssEngine::PaintCanvas::DrawImage2D
              ((PaintCanvas *)**(undefined4 **)(DAT_000e0f84 + 0xe0e8a),
               *(uint *)(*in_r0 + in_r0[0xe] * 4),(int)(fVar5 - (in_r1 + -1.0) * fVar3 * 0.5),
               (int)(fVar6 - (in_s2 + -1.0) * fVar4 * 0.5),(int)(fVar3 * in_r1),(int)(fVar4 * in_s2)
               ,'\x11','\x11','\0');
  }
  return;
}

```

## target disasm
```
  000e0e68: push {r4,r5,r6,r7,lr}
  000e0e6a: add r7,sp,#0xc
  000e0e6c: push.w r8
  000e0e70: sub sp,#0x20
  000e0e72: mov r3,r0
  000e0e74: ldr r0,[r0,#0x0]
  000e0e76: mov r12,r2
  000e0e78: cbz r0,0x000e0ec6
  000e0e7a: vmov.f32 s4,0x3f800000
  000e0e7e: vmov s0,r1
  000e0e82: ldr r1,[0x000e0f84]
  000e0e84: ldr r4,[r3,#0x38]
  000e0e86: add r1,pc
  000e0e88: ldr.w lr,[r3,#0x8]
  000e0e8c: ldr r2,[r3,#0x10]
  000e0e8e: ldr r5,[r1,#0x0]
  000e0e90: ldr.w r1,[r0,r4,lsl #0x2]
  000e0e94: sub.w r2,r2,lr
  000e0e98: vcmpe.f32 s0,s4
  000e0e9c: vmrs apsr,fpscr
  000e0ea0: ldr r0,[r5,#0x0]
  000e0ea2: ittt ne
  000e0ea4: vmov.ne s2,r12
  000e0ea8: vcmpe.ne.f32 s2,s4
  000e0eac: vmrs.ne apsr,fpscr
  000e0eb0: bne 0x000e0efe
  000e0eb2: ldr r3,[r3,#0x14]
  000e0eb4: sub.w r3,r3,lr
  000e0eb8: add sp,#0x20
  000e0eba: pop.w r8
  000e0ebe: pop.w {r4,r5,r6,r7,lr}
  000e0ec2: b.w 0x001ac068
  000e0ec6: ldr r0,[0x000e0f88]
  000e0ec8: ldrd r1,r5,[r3,#0x4]
  000e0ecc: add r0,pc
  000e0ece: ldrd r12,r4,[r3,#0x10]
  000e0ed2: ldrd lr,r6,[r3,#0x18]
  000e0ed6: sub.w r8,r4,r5
  000e0eda: ldr r0,[r0,#0x0]
  000e0edc: sub.w r5,r12,r5
  000e0ee0: ldrd r2,r3,[r3,#0x28]
  000e0ee4: movs r4,#0x0
  000e0ee6: add.w r12,sp,#0x10
  000e0eea: ldr r0,[r0,#0x0]
  000e0eec: strd lr,r6,[sp,#0x0]
  000e0ef0: strd r4,r4,[sp,#0x8]
  000e0ef4: stm r12,{r4,r5,r8}
  000e0ef8: blx 0x00072d9c
  000e0efc: b 0x000e0f7a
  000e0efe: vmov.f32 s4,0xbf800000
  000e0f02: vldr.32 s8,[r3,#0x1c]
  000e0f06: vldr.32 s6,[r3,#0x18]
  000e0f0a: vmov.f32 s12,0x3f000000
  000e0f0e: vcvt.f32.s32 s8,s8
  000e0f12: vcvt.f32.s32 s6,s6
  000e0f16: ldr r3,[r3,#0x14]
  000e0f18: sub.w r3,r3,lr
  000e0f1c: vadd.f32 s10,s2,s4
  000e0f20: vadd.f32 s4,s0,s4
  000e0f24: vmov s14,r3
  000e0f28: movs r3,#0x11
  000e0f2a: vmul.f32 s2,s8,s2
  000e0f2e: vcvt.f32.s32 s14,s14
  000e0f32: vmul.f32 s0,s6,s0
  000e0f36: vmul.f32 s10,s10,s8
  000e0f3a: vmov s8,r2
  000e0f3e: movs r2,#0x0
  000e0f40: vmul.f32 s4,s4,s6
  000e0f44: vcvt.f32.s32 s8,s8
  000e0f48: vcvt.s32.f32 s2,s2
  000e0f4c: vcvt.s32.f32 s0,s0
  000e0f50: strd r3,r3,[sp,#0x8]
  000e0f54: str r2,[sp,#0x10]
  000e0f56: vmls.f32 s14,s10,s12
  000e0f5a: vmls.f32 s8,s4,s12
  000e0f5e: vcvt.s32.f32 s4,s14
  000e0f62: vcvt.s32.f32 s6,s8
  000e0f66: vstr.32 s0,[sp]
  000e0f6a: vstr.32 s2,[sp,#0x4]
  000e0f6e: vmov r3,s4
  000e0f72: vmov r2,s6
  000e0f76: blx 0x00073564
  000e0f7a: add sp,#0x20
  000e0f7c: pop.w r8
  000e0f80: pop {r4,r5,r6,r7,pc}
```
