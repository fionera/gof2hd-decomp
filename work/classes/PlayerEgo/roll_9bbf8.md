# PlayerEgo::roll
elf 0x9bbf8 body 438
Sig: undefined __thiscall roll(PlayerEgo * this, int param_1)

## decompile
```c

/* PlayerEgo::roll(int) */

void __thiscall PlayerEgo::roll(PlayerEgo *this,int param_1)

{
  byte bVar1;
  undefined1 uVar2;
  int iVar3;
  float *pfVar4;
  int *piVar5;
  uint in_fpscr;
  uint uVar6;
  uint uVar7;
  float fVar8;
  float fVar9;
  float extraout_s1;
  float fVar10;
  float fVar11;
  AEMath aAStack_58 [60];
  int local_1c;
  
  piVar5 = *(int **)(DAT_000abde8 + 0xabc0c);
  local_1c = *piVar5;
  if ((char)this[0x15].m_pPlayer == '\0') goto LAB_000abd98;
  iVar3 = AEGeometry::getMatrix();
  fVar8 = *(float *)(iVar3 + 0x10);
  fVar10 = *(float *)(iVar3 + 0x14);
  uVar7 = in_fpscr & 0xfffffff | (uint)(fVar10 < 0.0) << 0x1f | (uint)(fVar10 == 0.0) << 0x1e;
  fVar9 = -fVar8;
  if (0.0 < fVar8) {
    fVar9 = fVar8;
  }
  if (0x3b < param_1) {
    param_1 = 0x3c;
  }
  bVar1 = (byte)(uVar7 >> 0x18);
  if ((!(bool)(bVar1 >> 6 & 1) && (bool)(bVar1 >> 7) == NAN(fVar10)) &&
     (uVar7 = in_fpscr & 0xfffffff, fVar9 < DAT_000abdb0)) {
    AbyssEngine::AEMath::MatrixIdentity(aAStack_58,(Matrix *)(this + 0x13));
    AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x13),(Matrix *)aAStack_58);
    *(undefined1 *)((int)&this[0x12].field_20 + 1) = 0;
    *(undefined1 *)&this[0x15].m_pPlayer = 0;
    *(undefined1 *)&this[0x16].m_pLevel = 0;
    goto LAB_000abd98;
  }
  if (*(char *)&this[0x16].m_pLevel == '\0') {
    uVar6 = uVar7 & 0xfffffff;
    uVar7 = uVar6 | (uint)(fVar8 < 0.0) << 0x1f;
    if ((fVar8 == 0.0 || SUB41(uVar7 >> 0x1f,0) != NAN(fVar8)) ||
       (fVar11 = DAT_000abde4, *(char *)((int)&this[0x12].field_20 + 1) != '\x01')) {
      uVar7 = uVar6;
      if (0.0 <= fVar8) {
        if ((NAN(fVar8)) || (fVar11 = DAT_000abdc0, 0.0 <= fVar10)) {
LAB_000abd0a:
          uVar7 = uVar6 | (uint)(fVar8 < 0.0) << 0x1f;
          fVar11 = fVar9;
          if ((fVar8 != 0.0 && SUB41(uVar7 >> 0x1f,0) == NAN(fVar8)) &&
             (uVar7 = uVar6 | (uint)(fVar10 < 0.0) << 0x1f,
             fVar10 != 0.0 && SUB41(uVar7 >> 0x1f,0) == NAN(fVar10))) {
            pfVar4 = (float *)&UNK_000abdd8;
            fVar10 = DAT_000abdd4;
            if (DAT_000abdd4 < fVar8) {
              pfVar4 = (float *)&DAT_000abddc;
            }
LAB_000abd44:
            uVar7 = uVar6;
            fVar11 = *pfVar4;
          }
        }
      }
      else {
        fVar11 = DAT_000abde0;
        if (*(char *)((int)&this[0x12].field_20 + 1) == '\x02') goto LAB_000abce0;
        fVar11 = DAT_000abdc4;
        if (0.0 <= fVar10) {
          if (NAN(fVar10)) goto LAB_000abd0a;
          pfVar4 = (float *)&DAT_000abdcc;
          uVar6 = uVar6 | (uint)(fVar8 < DAT_000abdc8) << 0x1f;
          fVar10 = DAT_000abdc8;
          if (fVar8 != DAT_000abdc8 && SUB41(uVar6 >> 0x1f,0) == (NAN(fVar8) || NAN(DAT_000abdc8)))
          {
            pfVar4 = (float *)&DAT_000abdd0;
          }
          goto LAB_000abd44;
        }
      }
    }
    else {
LAB_000abce0:
      *(undefined1 *)&this[0x16].m_pLevel = 1;
    }
  }
  else {
    uVar7 = uVar7 & 0xfffffff;
    pfVar4 = (float *)&DAT_000abdb8;
    if (!NAN(fVar10)) {
      pfVar4 = (float *)&DAT_000abdbc;
    }
    fVar10 = *pfVar4;
    fVar11 = DAT_000abdb4;
    if (fVar8 < 0.0) {
      fVar11 = fVar10;
    }
  }
  if ((fVar8 != 0.0) && (!NAN(fVar8))) {
    uVar2 = 2;
    if (fVar8 < 0.0) {
      uVar2 = 1;
    }
    *(undefined1 *)((int)&this[0x12].field_20 + 1) = uVar2;
  }
  fVar9 = (float)VectorSignedToFloat(param_1,(byte)((uVar7 & 0xfffffff) >> 0x16) & 3);
  *(undefined1 *)&this[0x15].m_pPlayer = 1;
  AbyssEngine::AEMath::MatrixSetRotation((Matrix *)aAStack_58,fVar11 * fVar9,extraout_s1,fVar10);
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x13),(Matrix *)aAStack_58);
LAB_000abd98:
  if (*piVar5 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000abbf8: push {r4,r5,r6,r7,lr}
  000abbfa: add r7,sp,#0xc
  000abbfc: push.w r8
  000abc00: sub sp,#0x48
  000abc02: mov r4,r0
  000abc04: ldr r0,[0x000abde8]
  000abc06: mov r5,r1
  000abc08: add r0,pc
  000abc0a: ldr.w r8,[r0,#0x0]
  000abc0e: ldr.w r0,[r8,#0x0]
  000abc12: str r0,[sp,#0x44]
  000abc14: ldrb.w r0,[r4,#0x2f4]
  000abc18: cmp r0,#0x0
  000abc1a: beq.w 0x000abd98
  000abc1e: ldr r0,[r4,#0x8]
  000abc20: blx 0x000721cc
  000abc24: vldr.32 s0,[r0,#0x10]
  000abc28: vldr.32 s2,[r0,#0x14]
  000abc2c: vcmpe.f32 s0,#0
  000abc30: vmrs apsr,fpscr
  000abc34: vneg.f32 s4,s0
  000abc38: vcmpe.f32 s2,#0
  000abc3c: it gt
  000abc3e: vmov.gt.f32 s4,s0
  000abc42: cmp r5,#0x3c
  000abc44: it ge
  000abc46: mov.ge r5,#0x3c
  000abc48: vmrs apsr,fpscr
  000abc4c: ble 0x000abc82
  000abc4e: vldr.32 s6,[pc,#0x160]
  000abc52: vcmpe.f32 s4,s6
  000abc56: vmrs apsr,fpscr
  000abc5a: bpl 0x000abc82
  000abc5c: add.w r5,r4,#0x2ac
  000abc60: add r6,sp,#0x8
  000abc62: mov r0,r6
  000abc64: mov r1,r5
  000abc66: blx 0x0006fb2c
  000abc6a: mov r0,r5
  000abc6c: mov r1,r6
  000abc6e: blx 0x0006f148
  000abc72: movs r0,#0x0
  000abc74: strb.w r0,[r4,#0x2a9]
  000abc78: strb.w r0,[r4,#0x2f4]
  000abc7c: strb.w r0,[r4,#0x324]
  000abc80: b 0x000abd98
  000abc82: ldrb.w r0,[r4,#0x324]
  000abc86: cbz r0,0x000abcae
  000abc88: vcmpe.f32 s2,s2
  000abc8c: adr r0,[0xabdb8]
  000abc8e: vmrs apsr,fpscr
  000abc92: vcmpe.f32 s0,#0
  000abc96: vldr.32 s4,[pc,#0x11c]
  000abc9a: it vc
  000abc9c: add.vc r0,#0x4
  000abc9e: vmrs apsr,fpscr
  000abca2: vldr.32 s2,[r0]
  000abca6: it mi
  000abca8: vmov.mi.f32 s4,s2
  000abcac: b 0x000abd48
  000abcae: vcmpe.f32 s0,#0
  000abcb2: vmrs apsr,fpscr
  000abcb6: ble 0x000abcc8
  000abcb8: ldrb.w r0,[r4,#0x2a9]
  000abcbc: cmp r0,#0x1
  000abcbe: bne 0x000abcc8
  000abcc0: movs r0,#0x1
  000abcc2: vldr.32 s4,[pc,#0x120]
  000abcc6: b 0x000abce0
  000abcc8: vcmpe.f32 s0,#0
  000abccc: vmrs apsr,fpscr
  000abcd0: bpl 0x000abce6
  000abcd2: ldrb.w r0,[r4,#0x2a9]
  000abcd6: cmp r0,#0x2
  000abcd8: bne 0x000abcf8
  000abcda: movs r0,#0x1
  000abcdc: vldr.32 s4,[pc,#0x100]
  000abce0: strb.w r0,[r4,#0x324]
  000abce4: b 0x000abd48
  000abce6: blt 0x000abd0a
  000abce8: vcmpe.f32 s2,#0
  000abcec: vmrs apsr,fpscr
  000abcf0: bpl 0x000abd0a
  000abcf2: vldr.32 s4,[pc,#0xcc]
  000abcf6: b 0x000abd48
  000abcf8: vcmpe.f32 s2,#0
  000abcfc: vmrs apsr,fpscr
  000abd00: bpl 0x000abd08
  000abd02: vldr.32 s4,[pc,#0xc0]
  000abd06: b 0x000abd48
  000abd08: bge 0x000abd32
  000abd0a: vcmpe.f32 s0,#0
  000abd0e: vmrs apsr,fpscr
  000abd12: ble 0x000abd48
  000abd14: vcmpe.f32 s2,#0
  000abd18: vmrs apsr,fpscr
  000abd1c: ble 0x000abd48
  000abd1e: vldr.32 s2,[pc,#0xb4]
  000abd22: adr r0,[0xabdd8]
  000abd24: vcmpe.f32 s0,s2
  000abd28: vmrs apsr,fpscr
  000abd2c: it hi
  000abd2e: add.hi r0,#0x4
  000abd30: b 0x000abd44
  000abd32: vldr.32 s2,[pc,#0x94]
  000abd36: adr r0,[0xabdcc]
  000abd38: vcmpe.f32 s0,s2
  000abd3c: vmrs apsr,fpscr
  000abd40: it gt
  000abd42: add.gt r0,#0x4
  000abd44: vldr.32 s4,[r0]
  000abd48: vcmpe.f32 s0,#0
  000abd4c: vmrs apsr,fpscr
  000abd50: beq 0x000abd68
  000abd52: bvs 0x000abd68
  000abd54: b 0x000abd56
  000abd56: vcmpe.f32 s0,#0
  000abd5a: movs r0,#0x2
  000abd5c: vmrs apsr,fpscr
  000abd60: it mi
  000abd62: mov.mi r0,#0x1
  000abd64: strb.w r0,[r4,#0x2a9]
  000abd68: vmov s0,r5
  000abd6c: movs r0,#0x1
  000abd6e: add r5,sp,#0x8
  000abd70: movs r2,#0x0
  000abd72: vcvt.f32.s32 s0,s0
  000abd76: strb.w r0,[r4,#0x2f4]
  000abd7a: add.w r4,r4,#0x2ac
  000abd7e: mov r0,r5
  000abd80: movs r3,#0x0
  000abd82: mov r1,r4
  000abd84: vmul.f32 s0,s4,s0
  000abd88: vstr.32 s0,[sp]
  000abd8c: blx 0x00072094
  000abd90: mov r0,r4
  000abd92: mov r1,r5
  000abd94: blx 0x0006f148
  000abd98: ldr r0,[sp,#0x44]
  000abd9a: ldr.w r1,[r8,#0x0]
  000abd9e: subs r0,r1,r0
  000abda0: ittt eq
  000abda2: add.eq sp,#0x48
  000abda4: pop.eq.w r8
  000abda8: pop.eq {r4,r5,r6,r7,pc}
  000abdaa: blx 0x0006e824
```
