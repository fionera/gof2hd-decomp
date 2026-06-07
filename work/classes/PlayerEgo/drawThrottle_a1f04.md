# PlayerEgo::drawThrottle
elf 0xa1f04 body 450
Sig: undefined __thiscall drawThrottle(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::drawThrottle() */

void __thiscall PlayerEgo::drawThrottle(PlayerEgo *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  String *pSVar5;
  uint uVar6;
  uint *puVar7;
  int *piVar8;
  float *pfVar9;
  uint in_fpscr;
  uint uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  int iVar15;
  String aSStack_38 [12];
  int local_2c;
  
  piVar8 = *(int **)(DAT_000b20e4 + 0xb1f18);
  local_2c = *piVar8;
  if (*(char *)&this[0x18].field_10 != '\0') {
    piVar1 = this[0x18].field_14;
    if (500 < (int)piVar1) {
      piVar1 = (int *)(2000 - (int)piVar1);
    }
    VectorSignedToFloat(piVar1,(byte)(in_fpscr >> 0x16) & 3);
    uVar10 = in_fpscr & 0xfffffff;
    puVar7 = *(uint **)(DAT_000b20e8 + 0xb1f68);
    AbyssEngine::PaintCanvas::SetColor(*puVar7);
    iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
    iVar3 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar7);
    fVar11 = (float)VectorSignedToFloat(iVar3,(byte)(uVar10 >> 0x16) & 3);
    pfVar9 = *(float **)(DAT_000b20ec + 0xb1fa2);
    fVar12 = (float)this[5].field_8 * fVar11;
    fVar14 = (float)VectorSignedToFloat(iVar2 / 2,(byte)(uVar10 >> 0x16) & 3);
    iVar15 = (int)fVar12;
    fVar13 = (float)VectorSignedToFloat(iVar15,(byte)(uVar10 >> 0x16) & 3);
    AbyssEngine::PaintCanvas::DrawRegion2D
              ((PaintCanvas *)*puVar7,this[0xf].field_1C,0,iVar3 - iVar15,iVar2,(int)fVar12,
               (float)(int)((pfVar9[1] + fVar11) - fVar13),0,0,0,(int)(*pfVar9 - fVar14));
    AbyssEngine::String::String(aSStack_38,(int)((float)this[5].field_8 * DAT_000b20dc));
    uVar4 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar7);
    uVar6 = *puVar7;
    fVar13 = *pfVar9;
    pSVar5 = (String *)**(undefined4 **)(DAT_000b20f0 + 0xb2036);
    iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(uVar6,pSVar5);
    fVar11 = (float)VectorSignedToFloat(uVar4,(byte)(uVar10 >> 0x16) & 3);
    fVar12 = (float)VectorSignedToFloat(iVar2 / 2,(byte)(uVar10 >> 0x16) & 3);
    fVar11 = (float)VectorSignedToFloat((int)(fVar11 / DAT_000b20e0),(byte)(uVar10 >> 0x16) & 3);
    AbyssEngine::PaintCanvas::DrawString
              (uVar6,pSVar5,(int)aSStack_38,(int)((fVar13 - fVar12) + -1.0),
               SUB41((int)(pfVar9[1] + fVar11),0));
    AbyssEngine::PaintCanvas::SetColor(*puVar7);
    AbyssEngine::String::~String(aSStack_38);
  }
  if (*piVar8 != local_2c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b1f04: push {r4,r5,r6,r7,lr}
  000b1f06: add r7,sp,#0xc
  000b1f08: push {r8,r9,r10}
  000b1f0c: vpush {d2,d3,d4,d5,d6,d7,d8}
  000b1f10: mov r4,r0
  000b1f12: ldr r0,[0x000b20e4]
  000b1f14: add r0,pc
  000b1f16: ldr.w r8,[r0,#0x0]
  000b1f1a: ldr.w r0,[r8,#0x0]
  000b1f1e: str r0,[sp,#0x2c]
  000b1f20: ldrb.w r0,[r4,#0x370]
  000b1f24: cmp r0,#0x0
  000b1f26: beq.w 0x000b20ae
  000b1f2a: ldr.w r0,[r4,#0x374]
  000b1f2e: vldr.32 s2,[pc,#0x1a4]
  000b1f32: cmp.w r0,#0x1f4
  000b1f36: it gt
  000b1f38: rsb.gt.w r0,r0,#0x7d0
  000b1f3c: vmov s0,r0
  000b1f40: vcvt.f32.s32 s0,s0
  000b1f44: vdiv.f32 s0,s0,s2
  000b1f48: vmov.f32 s2,0x3f800000
  000b1f4c: vcmpe.f32 s0,s2
  000b1f50: vmrs apsr,fpscr
  000b1f54: it mi
  000b1f56: vmov.mi.f32 s2,s0
  000b1f5a: vldr.32 s0,[pc,#0x17c]
  000b1f5e: ldr r0,[0x000b20e8]
  000b1f60: vmul.f32 s0,s2,s0
  000b1f64: add r0,pc
  000b1f66: ldr r6,[r0,#0x0]
  000b1f68: vcvt.s32.f32 s0,s0
  000b1f6c: ldr r0,[r6,#0x0]
  000b1f6e: vmov r1,s0
  000b1f72: sub.w r1,r1,#0x100
  000b1f76: blx 0x0006fac0
  000b1f7a: ldr.w r1,[r4,#0x238]
  000b1f7e: ldr r0,[r6,#0x0]
  000b1f80: blx 0x00072d84
  000b1f84: mov r5,r0
  000b1f86: ldr r0,[r6,#0x0]
  000b1f88: ldr.w r1,[r4,#0x238]
  000b1f8c: blx 0x00072d90
  000b1f90: vmov s0,r0
  000b1f94: vcvt.f32.s32 s0,s0
  000b1f98: vldr.32 s2,[r4,#0xbc]
  000b1f9c: ldr r1,[0x000b20ec]
  000b1f9e: add r1,pc
  000b1fa0: ldr.w r10,[r1,#0x0]
  000b1fa4: add.w r1,r5,r5, lsr #0x1f
  000b1fa8: vmul.f32 s2,s2,s0
  000b1fac: asrs r1,r1,#0x1
  000b1fae: vmov s8,r1
  000b1fb2: vldr.32 s6,[r10,#0x4]
  000b1fb6: vldr.32 s4,[r10]
  000b1fba: vcvt.f32.s32 s8,s8
  000b1fbe: vadd.f32 s0,s6,s0
  000b1fc2: vcvt.s32.f32 s10,s2
  000b1fc6: vsub.f32 s4,s4,s8
  000b1fca: vcvt.f32.s32 s6,s10
  000b1fce: ldr r2,[r6,#0x0]
  000b1fd0: vmov r3,s10
  000b1fd4: ldr.w r1,[r4,#0x238]
  000b1fd8: vcvt.s32.f32 s4,s4
  000b1fdc: vcvt.s32.f32 s2,s2
  000b1fe0: vsub.f32 s0,s0,s6
  000b1fe4: vcvt.s32.f32 s0,s0
  000b1fe8: str r5,[sp,#0x0]
  000b1fea: movs r5,#0x0
  000b1fec: strd r5,r5,[sp,#0x8]
  000b1ff0: str r5,[sp,#0x10]
  000b1ff2: vstr.32 s2,[sp,#0x4]
  000b1ff6: vstr.32 s4,[sp,#0x14]
  000b1ffa: subs r3,r0,r3
  000b1ffc: mov r0,r2
  000b1ffe: movs r2,#0x0
  000b2000: vstr.32 s0,[sp,#0x18]
  000b2004: blx 0x00072d9c
  000b2008: vldr.32 s0,[pc,#0xd0]
  000b200c: add r0,sp,#0x20
  000b200e: vldr.32 s2,[r4,#0xbc]
  000b2012: vmul.f32 s0,s2,s0
  000b2016: vcvt.s32.f32 s0,s0
  000b201a: vmov r1,s0
  000b201e: blx 0x0006f658
  000b2022: ldr.w r1,[r4,#0x238]
  000b2026: ldr r0,[r6,#0x0]
  000b2028: blx 0x00072d90
  000b202c: mov r9,r0
  000b202e: ldr r0,[0x000b20f0]
  000b2030: ldr r5,[r6,#0x0]
  000b2032: add r0,pc
  000b2034: vldr.32 s16,[r10]
  000b2038: ldr r0,[r0,#0x0]
  000b203a: ldr r4,[r0,#0x0]
  000b203c: add r2,sp,#0x20
  000b203e: mov r0,r5
  000b2040: mov r1,r4
  000b2042: blx 0x0006faa8
  000b2046: vmov s0,r9
  000b204a: vldr.32 s2,[pc,#0x94]
  000b204e: add.w r0,r0,r0, lsr #0x1f
  000b2052: vmov.f32 s6,0xbf800000
  000b2056: vcvt.f32.s32 s0,s0
  000b205a: asrs r0,r0,#0x1
  000b205c: vdiv.f32 s0,s0,s2
  000b2060: vmov s2,r0
  000b2064: vcvt.f32.s32 s2,s2
  000b2068: vcvt.s32.f32 s0,s0
  000b206c: vsub.f32 s2,s16,s2
  000b2070: vcvt.f32.s32 s0,s0
  000b2074: vldr.32 s4,[r10,#0x4]
  000b2078: vadd.f32 s0,s4,s0
  000b207c: vadd.f32 s2,s2,s6
  000b2080: movs r0,#0x0
  000b2082: vcvt.s32.f32 s0,s0
  000b2086: str r0,[sp,#0x4]
  000b2088: add r2,sp,#0x20
  000b208a: mov r0,r5
  000b208c: mov r1,r4
  000b208e: vcvt.s32.f32 s2,s2
  000b2092: vstr.32 s0,[sp]
  000b2096: vmov r3,s2
  000b209a: blx 0x0006fe14
  000b209e: ldr r0,[r6,#0x0]
  000b20a0: mov.w r1,#0xffffffff
  000b20a4: blx 0x0006fac0
  000b20a8: add r0,sp,#0x20
  000b20aa: blx 0x0006ee30
  000b20ae: ldr r0,[sp,#0x2c]
  000b20b0: ldr.w r1,[r8,#0x0]
  000b20b4: subs r0,r1,r0
  000b20b6: ittt eq
  000b20b8: vpop.eq {d2,d3,d4,d5,d6,d7,d8}
  000b20bc: pop.eq.w {r8,r9,r10}
  000b20c0: pop.eq {r4,r5,r6,r7,pc}
  000b20c2: blx 0x0006e824
```
