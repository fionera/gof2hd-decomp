# PlayerStaticFar::collide
elf 0x11c460 body 142
Sig: undefined __thiscall collide(PlayerStaticFar * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerStaticFar::collide(float, float, float) */

undefined4 __thiscall
PlayerStaticFar::collide(PlayerStaticFar *this,float param_1,float param_2,float param_3)

{
  uint uVar1;
  uint uVar2;
  byte bVar3;
  float in_r1;
  float in_r2;
  float in_r3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  float fVar6;
  
  fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x124),(byte)(in_fpscr >> 0x16) & 3);
  fVar5 = in_r1 - fVar5;
  fVar4 = (float)VectorSignedToFloat(*(int *)(*(int *)(this + 4) + 0x40),
                                     (byte)(in_fpscr >> 0x16) & 3);
  uVar1 = in_fpscr & 0xfffffff;
  if (fVar5 < fVar4) {
    fVar6 = (float)VectorSignedToFloat(-*(int *)(*(int *)(this + 4) + 0x40),
                                       (byte)(uVar1 >> 0x16) & 3);
    uVar2 = uVar1 | (uint)(fVar5 < fVar6) << 0x1f | (uint)(fVar5 == fVar6) << 0x1e;
    bVar3 = (byte)(uVar2 >> 0x18);
    if (!(bool)(bVar3 >> 6 & 1) && (bool)(bVar3 >> 7) == (NAN(fVar5) || NAN(fVar6))) {
      fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x128),(byte)(uVar2 >> 0x16) & 3);
      fVar5 = in_r2 - fVar5;
      if ((fVar5 < fVar4) &&
         (uVar1 = uVar1 | (uint)(fVar5 < fVar6) << 0x1f | (uint)(fVar5 == fVar6) << 0x1e,
         bVar3 = (byte)(uVar1 >> 0x18),
         !(bool)(bVar3 >> 6 & 1) && (bool)(bVar3 >> 7) == (NAN(fVar5) || NAN(fVar6)))) {
        fVar5 = (float)VectorSignedToFloat(*(undefined4 *)(this + 300),(byte)(uVar1 >> 0x16) & 3);
        if (((int)((uint)(in_r3 - fVar5 < fVar4) << 0x1f) < 0) && (fVar6 < in_r3 - fVar5)) {
          return 1;
        }
      }
    }
  }
  return 0;
}

```

## target disasm
```
  0012c460: vldr.32 s0,[r0,#0x124]
  0012c464: vmov s4,r1
  0012c468: vcvt.f32.s32 s2,s0
  0012c46c: ldr.w r12,[r0,#0x4]
  0012c470: ldr.w r1,[r12,#0x40]
  0012c474: vmov s0,r1
  0012c478: vsub.f32 s4,s4,s2
  0012c47c: vcvt.f32.s32 s0,s0
  0012c480: vcmpe.f32 s4,s0
  0012c484: vmrs apsr,fpscr
  0012c488: bpl 0x0012c4ea
  0012c48a: rsbs r1,r1
  0012c48c: vmov s2,r1
  0012c490: vcvt.f32.s32 s2,s2
  0012c494: vcmpe.f32 s4,s2
  0012c498: vmrs apsr,fpscr
  0012c49c: ble 0x0012c4ea
  0012c49e: vldr.32 s6,[r0,#0x128]
  0012c4a2: vmov s4,r2
  0012c4a6: vcvt.f32.s32 s6,s6
  0012c4aa: vsub.f32 s4,s4,s6
  0012c4ae: vcmpe.f32 s4,s0
  0012c4b2: vmrs apsr,fpscr
  0012c4b6: bpl 0x0012c4ea
  0012c4b8: vcmpe.f32 s4,s2
  0012c4bc: vmrs apsr,fpscr
  0012c4c0: ble 0x0012c4ea
  0012c4c2: vldr.32 s6,[r0,#0x12c]
  0012c4c6: vmov s4,r3
  0012c4ca: vcvt.f32.s32 s6,s6
  0012c4ce: vsub.f32 s4,s4,s6
  0012c4d2: vcmpe.f32 s4,s0
  0012c4d6: vmrs apsr,fpscr
  0012c4da: bpl 0x0012c4ea
  0012c4dc: vcmpe.f32 s4,s2
  0012c4e0: vmrs apsr,fpscr
  0012c4e4: itt gt
  0012c4e6: mov.gt r0,#0x1
  0012c4e8: bx.gt lr
  0012c4ea: movs r0,#0x0
  0012c4ec: bx lr
```
