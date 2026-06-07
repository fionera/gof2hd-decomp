# IParticleSystem::emit
elf 0x181dc4 body 2326
Sig: undefined __thiscall emit(IParticleSystem * this, int param_1)

## decompile
```c

/* IParticleSystem::emit(int) */

void __thiscall IParticleSystem::emit(IParticleSystem *this,int param_1)

{
  uint uVar1;
  byte bVar2;
  float fVar3;
  float fVar4;
  uint uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  IParticleSystem *pIVar9;
  int iVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int *piVar16;
  int iVar17;
  undefined4 uVar18;
  int iVar19;
  int iVar20;
  code *pcVar21;
  int iVar22;
  uint in_fpscr;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  float fVar31;
  float fVar32;
  undefined4 uVar33;
  undefined4 uVar34;
  undefined4 uVar35;
  undefined4 uVar36;
  float fVar37;
  undefined4 *local_10c;
  Vector aVStack_f8 [12];
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  float local_e0;
  float local_dc;
  float local_d8;
  Vector aVStack_d4 [12];
  Vector aVStack_c8 [12];
  Matrix aMStack_bc [12];
  Matrix aMStack_b0 [12];
  Matrix aMStack_a4 [12];
  Matrix aMStack_98 [28];
  undefined4 local_7c;
  undefined4 uStack_78;
  undefined4 local_74;
  undefined4 uStack_70;
  int local_6c;
  
  piVar16 = *(int **)(DAT_00192138 + 0x191ddc);
  local_6c = *piVar16;
  if (((this[0xc] == (IParticleSystem)0x0) || (this[0xd] == (IParticleSystem)0x0)) ||
     (iVar14 = *(int *)(*(int *)(this + 0x3c) + (uint)(byte)this[0x44] * 4), iVar14 == -1))
  goto LAB_001926e2;
  uVar5 = *(uint *)(this + 0x34);
  if ((uVar5 & 0x80) == 0) {
    if ((int)(uVar5 << 0x17) < 0) goto LAB_001926e2;
  }
  else if (((int)(uVar5 << 0x17) < 0) || (**(int **)(this + 0x68) != -1)) goto LAB_001926e2;
  AbyssEngine::AEMath::MatrixGetPosition(aMStack_98);
  AbyssEngine::AEMath::MatrixGetRight(aMStack_a4);
  if (this[0x4c] != (IParticleSystem)0x0) {
    AbyssEngine::AEMath::operator-((AEMath *)&local_7c,(Vector *)aMStack_a4);
    AbyssEngine::AEMath::Vector::operator=((Vector *)aMStack_a4,(Vector *)&local_7c);
  }
  AbyssEngine::AEMath::MatrixGetUp(aMStack_b0);
  AbyssEngine::AEMath::MatrixGetDir(aMStack_bc);
  iVar22 = *(int *)(DAT_0019213c + 0x191e7e) + iVar14 * 0xa0;
  fVar6 = (float)AbyssEngine::AEMath::VectorDot((Vector *)(this + 0x1c),(Vector *)(this + 0x1c));
  fVar23 = (float)VectorSignedToFloat(*(undefined4 *)(iVar22 + 0x98),(byte)(in_fpscr >> 0x16) & 3);
  in_fpscr = in_fpscr & 0xfffffff;
  if (fVar6 < fVar23) goto LAB_001926e2;
  fVar26 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  fVar23 = (float)AbyssEngine::AEMath::operator*
                            ((Vector *)&local_7c,*(float *)(this + 0x60) + fVar26);
  AbyssEngine::AEMath::operator/(aVStack_c8,fVar23);
  fVar23 = (float)AbyssEngine::AEMath::VectorDot(aVStack_c8,aVStack_c8);
  iVar20 = DAT_00192144;
  fVar27 = 1.5;
  fVar7 = (float)(DAT_00192144 - ((int)fVar23 >> 1));
  uVar5 = *(uint *)(this + 0x34);
  fVar7 = (fVar23 * -0.5 * fVar7 * fVar7 + 1.5) * fVar7;
  fVar23 = 1.0 / fVar7;
  if ((uVar5 & 0x10) == 0) {
    if ((int)(uVar5 << 0x1a) < 0) {
      iVar15 = (int)(*(float *)(iVar22 + 0x2c) * (*(float *)(this + 0x60) + fVar26) * DAT_00192148);
      fVar8 = (float)VectorSignedToFloat(iVar15,(byte)(in_fpscr >> 0x16) & 3);
      fVar8 = *(float *)(this + 0x60) + fVar26 + (fVar8 * DAT_0019214c) / *(float *)(iVar22 + 0x2c);
      goto LAB_00191f92;
    }
    iVar15 = *(int *)(iVar22 + 0x10);
    if ((int)(uVar5 << 0x19) < 0) {
      this[0xc] = (IParticleSystem)0x0;
    }
  }
  else {
    fVar8 = fVar23 / *(float *)(iVar22 + 0x2c);
    iVar15 = (int)fVar8;
    fVar25 = (float)VectorSignedToFloat(iVar15,(byte)(in_fpscr >> 0x16) & 3);
    fVar8 = ((*(float *)(this + 0x60) + fVar26) * (fVar8 - fVar25)) / fVar8;
LAB_00191f92:
    *(float *)(this + 0x60) = fVar8;
  }
  if (0 < iVar15) {
    AbyssEngine::AEMath::operator-(aVStack_d4,(Vector *)aMStack_98);
    fVar4 = DAT_00192154;
    fVar3 = DAT_00192150;
    fVar25 = DAT_00192140;
    fVar8 = DAT_00192150;
    if (((byte)this[0x34] & 0xc0) == 0) {
      fVar8 = (float)(iVar20 - ((int)fVar6 >> 1));
      fVar27 = fVar6 * -0.5 * fVar8 * fVar8 + 1.5;
      fVar8 = fVar27 * fVar8;
    }
    local_7c = *(undefined4 *)(iVar22 + 0x88);
    local_74 = *(undefined4 *)(iVar22 + 0x8c);
    uStack_78 = *(undefined4 *)(iVar22 + 0x90);
    uStack_70 = *(undefined4 *)(iVar22 + 0x94);
    pIVar9 = this + 0x10;
    local_10c = &local_7c;
    iVar20 = 0;
    while (iVar13 = iVar20 + 1, iVar13 <= iVar15) {
      *(char *)(*(int *)(this + 0x6c) + *(int *)(this + 0x50)) = (char)iVar14;
      *(undefined4 *)(*(int *)(this + 0x68) + *(int *)(this + 0x50) * 4) = 0;
      if ((int)((uint)(byte)this[0x37] << 0x1e) < 0) {
        local_10c = (undefined4 *)rotateUVs((float *)this,(int)&local_7c,*(float **)(this + 0x50));
      }
      iVar19 = *(int *)(iVar22 + 0x50);
      if (iVar19 == 0) {
        local_e0 = 0.0;
        local_dc = 0.0;
        local_d8 = 0.0;
      }
      else {
        iVar17 = iVar19 << 1;
        pcVar21 = *(code **)(DAT_00192158 + 0x1920d6);
        iVar10 = (*pcVar21)(pIVar9,iVar17);
        fVar6 = *(float *)(iVar22 + 0x58);
        fVar29 = (float)VectorSignedToFloat(iVar10 - iVar19,(byte)(in_fpscr >> 0x16) & 3);
        iVar10 = (*pcVar21)(pIVar9,iVar17);
        fVar31 = *(float *)(iVar22 + 0x5c);
        fVar32 = (float)VectorSignedToFloat(iVar10 - iVar19,(byte)(in_fpscr >> 0x16) & 3);
        iVar10 = (*pcVar21)(pIVar9,iVar17);
        local_e0 = fVar6 + fVar29;
        local_dc = fVar31 + fVar32;
        local_d8 = (float)VectorSignedToFloat(iVar10 - iVar19,(byte)(in_fpscr >> 0x16) & 3);
        local_d8 = *(float *)(iVar22 + 0x60) + local_d8;
      }
      AbyssEngine::AEMath::Vector::operator=
                ((Vector *)(*(int *)(this + 100) + *(int *)(this + 0x50) * 0xc),(Vector *)&local_e0)
      ;
      fVar6 = *(float *)(iVar22 + 100);
      uVar5 = in_fpscr & 0xfffffff | (uint)(fVar6 == 0.0) << 0x1e;
      if ((byte)(uVar5 >> 0x1e) == 0) {
        AbyssEngine::AEMath::operator*((Vector *)&local_e0,fVar6);
        AbyssEngine::AEMath::Vector::operator-=
                  ((Vector *)(*(int *)(this + 100) + *(int *)(this + 0x50) * 0xc),
                   (Vector *)&local_e0);
      }
      uVar5 = uVar5 & 0xfffffff | (uint)(*(float *)(iVar22 + 0x68) == 0.0) << 0x1e;
      if ((byte)(uVar5 >> 0x1e) == 0) {
        AbyssEngine::AEMath::operator*((Vector *)&local_e0,*(float *)(iVar22 + 0x68));
        AbyssEngine::AEMath::Vector::operator+=
                  ((Vector *)(*(int *)(this + 100) + *(int *)(this + 0x50) * 0xc),
                   (Vector *)&local_e0);
      }
      uVar5 = uVar5 & 0xfffffff | (uint)(*(float *)(iVar22 + 0x6c) == 0.0) << 0x1e;
      if ((byte)(uVar5 >> 0x1e) == 0) {
        AbyssEngine::AEMath::operator*((Vector *)&local_e0,*(float *)(iVar22 + 0x6c));
        AbyssEngine::AEMath::Vector::operator+=
                  ((Vector *)(*(int *)(this + 100) + *(int *)(this + 0x50) * 0xc),
                   (Vector *)&local_e0);
      }
      uVar5 = uVar5 & 0xfffffff | (uint)(*(float *)(iVar22 + 0x70) == 0.0) << 0x1e;
      if ((byte)(uVar5 >> 0x1e) == 0) {
        AbyssEngine::AEMath::operator*((Vector *)&local_e0,*(float *)(iVar22 + 0x70));
        AbyssEngine::AEMath::Vector::operator+=
                  ((Vector *)(*(int *)(this + 100) + *(int *)(this + 0x50) * 0xc),
                   (Vector *)&local_e0);
      }
      if (*(int *)(iVar22 + 0x30) == 1) {
        fVar6 = (float)VectorSignedToFloat(iVar13,(byte)(uVar5 >> 0x16) & 3);
      }
      else {
        uVar11 = AbyssEngine::AERandom::nextInt((int)pIVar9);
        fVar29 = (float)VectorSignedToFloat(uVar11,(byte)(uVar5 >> 0x16) & 3);
        fVar6 = (float)VectorSignedToFloat(iVar20,(byte)(uVar5 >> 0x16) & 3);
        fVar6 = fVar6 + fVar29 * fVar4;
      }
      local_e0 = 0.0;
      local_dc = 0.0;
      local_d8 = 0.0;
      if ((*(uint *)(this + 0x34) & 0xc0) == 0) {
        uVar5 = uVar5 & 0xfffffff;
        if (1.0 <= fVar23) {
          if ((int)(*(uint *)(this + 0x34) << 0x1b) < 0) {
            fVar27 = *(float *)(iVar22 + 0x2c);
          }
          else {
            fVar27 = (float)VectorSignedToFloat(iVar15,(byte)(uVar5 >> 0x16) & 3);
            fVar27 = fVar23 / fVar27;
          }
          AbyssEngine::AEMath::operator*(aVStack_f8,fVar6 * fVar27);
          AbyssEngine::AEMath::operator*((Vector *)&local_ec,fVar7);
          AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)&local_ec);
          AbyssEngine::AEMath::operator+((Vector *)&local_ec,aVStack_d4);
          AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)&local_ec);
        }
        else {
          AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)aMStack_98);
          fVar6 = (float)VectorSignedToFloat(iVar13,(byte)(uVar5 >> 0x16) & 3);
          iVar15 = iVar13;
        }
      }
      else {
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_e0,(Vector *)aMStack_98);
        fVar6 = fVar3;
      }
      fVar29 = *(float *)(iVar22 + 0x78);
      if ((int)((uint)(byte)this[0x34] << 0x18) < 0) {
        iVar10 = (int)fVar29;
        pcVar21 = *(code **)(DAT_00192700 + 0x19243a);
        iVar19 = iVar10 << 1;
        iVar20 = (*pcVar21)(pIVar9,iVar19);
        uVar11 = VectorSignedToFloat(iVar20 - iVar10,(byte)(uVar5 >> 0x16) & 3);
        iVar20 = (*pcVar21)(pIVar9,iVar19);
        uVar33 = VectorSignedToFloat(iVar20 - iVar10,(byte)(uVar5 >> 0x16) & 3);
        iVar20 = (*pcVar21)(pIVar9,iVar19);
        local_e4 = VectorSignedToFloat(iVar20 - iVar10,(byte)(uVar5 >> 0x16) & 3);
        local_ec = uVar11;
        local_e8 = uVar33;
        AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_e0,(Vector *)&local_ec);
      }
      else {
        uVar5 = uVar5 & 0xfffffff | (uint)(fVar29 == 0.0) << 0x1e;
        if ((byte)(uVar5 >> 0x1e) == 0) {
          AbyssEngine::AEMath::operator*((Vector *)&local_ec,fVar29);
          AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_e0,(Vector *)&local_ec);
        }
        uVar5 = uVar5 & 0xfffffff | (uint)(*(float *)(iVar22 + 0x7c) == 0.0) << 0x1e;
        if ((byte)(uVar5 >> 0x1e) == 0) {
          AbyssEngine::AEMath::operator*((Vector *)&local_ec,*(float *)(iVar22 + 0x7c));
          AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_e0,(Vector *)&local_ec);
        }
        uVar5 = uVar5 & 0xfffffff | (uint)(*(float *)(iVar22 + 0x80) == 0.0) << 0x1e;
        if ((byte)(uVar5 >> 0x1e) == 0) {
          AbyssEngine::AEMath::operator*((Vector *)&local_ec,*(float *)(iVar22 + 0x80));
          AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_e0,(Vector *)&local_ec);
        }
        uVar5 = uVar5 & 0xfffffff | (uint)(*(float *)(iVar22 + 0x84) == 0.0) << 0x1e;
        if ((byte)(uVar5 >> 0x1e) == 0) {
          uVar11 = AbyssEngine::AERandom::nextInt((int)pIVar9);
          fVar29 = (float)VectorSignedToFloat(uVar11,(byte)(uVar5 >> 0x16) & 3);
          AbyssEngine::AEMath::operator*((Vector *)&local_ec,fVar29);
          AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_e0,(Vector *)&local_ec);
        }
        iVar20 = *(int *)(iVar22 + 0x48);
        if (iVar20 != 0) {
          iVar19 = AbyssEngine::AERandom::nextInt((int)pIVar9);
          uVar11 = VectorSignedToFloat(iVar19 - iVar20,(byte)(uVar5 >> 0x16) & 3);
          iVar19 = AbyssEngine::AERandom::nextInt((int)pIVar9);
          local_e4 = VectorSignedToFloat(iVar19 - iVar20,(byte)(uVar5 >> 0x16) & 3);
          local_e8 = 0;
          local_ec = uVar11;
          AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_e0,(Vector *)&local_ec);
        }
        if (*(int *)(iVar22 + 0x4c) != 0) {
          iVar20 = AbyssEngine::AERandom::nextInt((int)pIVar9);
          fVar29 = (float)VectorSignedToFloat(iVar20 - *(int *)(iVar22 + 0x4c),
                                              (byte)(uVar5 >> 0x16) & 3);
          local_dc = local_dc + fVar29;
        }
      }
      fVar29 = *(float *)(iVar22 + 0x14);
      if (*(int *)(iVar22 + 0x18) == 0) {
        uVar34 = *local_10c;
        uVar33 = local_10c[1];
        uVar36 = local_10c[2];
        uVar35 = local_10c[3];
        uVar11 = *(undefined4 *)(iVar22 + 0x34);
        if (*(int *)(iVar22 + 0x3c) < 1) {
          uVar18 = 0;
          fVar31 = *(float *)(iVar22 + 0x40);
          uVar1 = uVar5 & 0xfffffff | (uint)(fVar31 < 0.0) << 0x1f | (uint)(fVar31 == 0.0) << 0x1e;
          uVar5 = uVar1 | (uint)NAN(fVar31) << 0x1c;
          bVar2 = (byte)(uVar1 >> 0x18);
          if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar5 >> 0x1c) & 1)) {
            uVar18 = 1;
          }
        }
        else {
          uVar18 = 1;
        }
        fVar24 = *(float *)(iVar22 + 0x20);
        fVar32 = *(float *)(iVar22 + 0x1c);
        fVar31 = *(float *)(iVar22 + 0x24);
        uVar5 = uVar5 & 0xfffffff | (uint)(fVar31 == 0.0) << 0x1e;
        if ((byte)(uVar5 >> 0x1e) == 0) {
          AbyssEngine::AEMath::operator*(fVar31,(Vector *)&local_ec);
        }
        else {
          local_ec = 0;
          local_e8 = 0;
          local_e4 = 0;
        }
        (**(code **)(*(int *)this + 0x18))
                  (this,&local_e0,fVar29,uVar11,uVar34,uVar36,uVar33,uVar35,uVar18,fVar32,fVar24,
                   (Vector *)&local_ec);
      }
      else {
        uVar11 = AbyssEngine::AERandom::nextInt((int)pIVar9);
        fVar31 = (float)VectorSignedToFloat(uVar11,(byte)(uVar5 >> 0x16) & 3);
        uVar34 = *local_10c;
        uVar33 = local_10c[1];
        uVar36 = local_10c[2];
        uVar35 = local_10c[3];
        uVar11 = *(undefined4 *)(iVar22 + 0x34);
        if (*(int *)(iVar22 + 0x3c) < 1) {
          uVar18 = 0;
          fVar32 = *(float *)(iVar22 + 0x40);
          uVar1 = uVar5 & 0xfffffff | (uint)(fVar32 < 0.0) << 0x1f | (uint)(fVar32 == 0.0) << 0x1e;
          uVar5 = uVar1 | (uint)NAN(fVar32) << 0x1c;
          bVar2 = (byte)(uVar1 >> 0x18);
          if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(uVar5 >> 0x1c) & 1)) {
            uVar18 = 1;
          }
        }
        else {
          uVar18 = 1;
        }
        fVar28 = *(float *)(iVar22 + 0x1c);
        uVar12 = AbyssEngine::AERandom::nextInt((int)pIVar9);
        fVar30 = *(float *)(iVar22 + 0x20);
        fVar37 = (float)VectorSignedToFloat(uVar12,(byte)(uVar5 >> 0x16) & 3);
        uVar12 = AbyssEngine::AERandom::nextInt((int)pIVar9);
        fVar32 = (float)VectorSignedToFloat(uVar12,(byte)(uVar5 >> 0x16) & 3);
        fVar24 = *(float *)(iVar22 + 0x24);
        uVar5 = uVar5 & 0xfffffff | (uint)(fVar24 == 0.0) << 0x1e;
        if ((byte)(uVar5 >> 0x1e) == 0) {
          AbyssEngine::AEMath::operator*(fVar24,(Vector *)&local_ec);
        }
        else {
          local_ec = 0;
          local_e8 = 0;
          local_e4 = 0;
        }
        (**(code **)(*(int *)this + 0x18))
                  (this,&local_e0,fVar29 + fVar31,uVar11,uVar34,uVar36,uVar33,uVar35,uVar18,
                   fVar28 + fVar37,fVar30 + fVar32,&local_ec);
      }
      fVar29 = *(float *)(iVar22 + 100);
      uVar5 = uVar5 & 0xfffffff | (uint)(fVar29 == 0.0) << 0x1e;
      if ((byte)(uVar5 >> 0x1e) == 0) {
        fVar29 = (float)AbyssEngine::AEMath::operator*(aVStack_f8,fVar29);
        AbyssEngine::AEMath::operator*((Vector *)&local_ec,fVar29);
        AbyssEngine::AEMath::Vector::operator+=
                  ((Vector *)(*(int *)(this + 100) + *(int *)(this + 0x50) * 0xc),
                   (Vector *)&local_ec);
      }
      fVar29 = (float)VectorSignedToFloat(iVar15,(byte)(uVar5 >> 0x16) & 3);
      fVar6 = fVar8 * fVar27 * (fVar29 - fVar6) * fVar25;
      uVar5 = uVar5 & 0xfffffff | (uint)(fVar6 < fVar26) << 0x1f | (uint)(fVar6 == fVar26) << 0x1e;
      in_fpscr = uVar5 | (uint)(NAN(fVar6) || NAN(fVar26)) << 0x1c;
      bVar2 = (byte)(uVar5 >> 0x18);
      if (!(bool)(bVar2 >> 6 & 1) && bVar2 >> 7 == ((byte)(in_fpscr >> 0x1c) & 1)) {
        fVar6 = fVar26;
      }
      (**(code **)(*(int *)this + 0x14))(this,*(undefined4 *)(this + 0x50),fVar6);
      iVar20 = *(int *)(this + 0x50) + 1;
      if (*(int *)(this + 0x48) <= iVar20) {
        iVar20 = 0;
      }
      *(int *)(this + 0x50) = iVar20;
      iVar20 = iVar13;
    }
  }
LAB_001926e2:
  if (*piVar16 == local_6c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  00191dc4: push {r4,r5,r6,r7,lr}
  00191dc6: add r7,sp,#0xc
  00191dc8: push {r7,r8,r9,r10,r11}
  00191dcc: vpush {d8,d9,d10,d11,d12,d13,d14,d15}
  00191dd0: sub sp,#0x130
  00191dd2: mov r11,r0
  00191dd4: ldr r0,[0x00192138]
  00191dd6: mov r6,r1
  00191dd8: add r0,pc
  00191dda: ldr r4,[r0,#0x0]
  00191ddc: ldr r0,[r4,#0x0]
  00191dde: str r0,[sp,#0x12c]
  00191de0: ldrb.w r0,[r11,#0xc]
  00191de4: cmp r0,#0x0
  00191de6: beq.w 0x001926e2
  00191dea: ldrb.w r0,[r11,#0xd]
  00191dee: cmp r0,#0x0
  00191df0: beq.w 0x001926e2
  00191df4: ldrb.w r0,[r11,#0x44]
  00191df8: ldr.w r1,[r11,#0x3c]
  00191dfc: ldr.w r1,[r1,r0,lsl #0x2]
  00191e00: str r1,[sp,#0x88]
  00191e02: mov r0,r1
  00191e04: adds r0,r1,#0x1
  00191e06: beq.w 0x001926e2
  00191e0a: ldr.w r0,[r11,#0x34]
  00191e0e: tst r0,#0x80
  00191e12: bne 0x00191e1c
  00191e14: lsls r0,r0,#0x17
  00191e16: bpl 0x00191e2e
  00191e18: b.w 0x001926e2
  00191e1c: lsls r0,r0,#0x17
  00191e1e: bmi.w 0x001926e2
  00191e22: ldr.w r0,[r11,#0x68]
  00191e26: ldr r0,[r0,#0x0]
  00191e28: adds r0,#0x1
  00191e2a: bne.w 0x001926e2
  00191e2e: ldr.w r1,[r11,#0x18]
  00191e32: add r0,sp,#0x100
  00191e34: blx 0x0006f16c
  00191e38: ldr.w r1,[r11,#0x18]
  00191e3c: add r0,sp,#0xf4
  00191e3e: blx 0x0006f4e4
  00191e42: ldrb.w r0,[r11,#0x4c]
  00191e46: cbz r0,0x00191e5e
  00191e48: add.w r8,sp,#0x11c
  00191e4c: add r5,sp,#0xf4
  00191e4e: mov r0,r8
  00191e50: mov r1,r5
  00191e52: blx 0x0006f4cc
  00191e56: mov r0,r5
  00191e58: mov r1,r8
  00191e5a: blx 0x0006eb3c
  00191e5e: ldr.w r1,[r11,#0x18]
  00191e62: add r0,sp,#0xe8
  00191e64: blx 0x0006f4d8
  00191e68: ldr.w r1,[r11,#0x18]
  00191e6c: add r0,sp,#0xdc
  00191e6e: blx 0x0006f4c0
  00191e72: ldr r0,[0x0019213c]
  00191e74: add.w r5,r11,#0x1c
  00191e78: ldr r1,[sp,#0x88]
  00191e7a: add r0,pc
  00191e7c: ldr r0,[r0,#0x0]
  00191e7e: add.w r1,r1,r1, lsl #0x2
  00191e82: add.w r10,r0,r1, lsl #0x5
  00191e86: mov r0,r5
  00191e88: mov r1,r5
  00191e8a: blx 0x0006ec68
  00191e8e: vldr.32 s0,[r10,#0x98]
  00191e92: vmov s28,r0
  00191e96: vcvt.f32.s32 s0,s0
  00191e9a: vcmpe.f32 s28,s0
  00191e9e: vmrs apsr,fpscr
  00191ea2: bmi.w 0x001926e2
  00191ea6: vmov s0,r6
  00191eaa: add.w r9,sp,#0x11c
  00191eae: mov r8,r0
  00191eb0: mov r1,r5
  00191eb2: vcvt.f32.s32 s16,s0
  00191eb6: vldr.32 s0,[r11,#0x60]
  00191eba: mov r0,r9
  00191ebc: str r5,[sp,#0x84]
  00191ebe: vadd.f32 s0,s0,s16
  00191ec2: vmov r2,s0
  00191ec6: blx 0x0006ec74
  00191eca: add r6,sp,#0xd0
  00191ecc: ldr r2,[0x00192140]
  00191ece: mov r1,r9
  00191ed0: mov r0,r6
  00191ed2: blx 0x00073024
  00191ed6: mov r0,r6
  00191ed8: mov r1,r6
  00191eda: blx 0x0006ec68
  00191ede: vmov.f32 s30,0xbf000000
  00191ee2: ldr r6,[0x00192144]
  00191ee4: vmov s0,r0
  00191ee8: vmov.f32 s17,0x3fc00000
  00191eec: sub.w r0,r6,r0, asr #0x1
  00191ef0: vmov s2,r0
  00191ef4: ldr.w r0,[r11,#0x34]
  00191ef8: tst r0,#0x10
  00191efc: vmul.f32 s0,s0,s30
  00191f00: vmov.f32 s4,s17
  00191f04: vmul.f32 s0,s0,s2
  00191f08: vmla.f32 s4,s0,s2
  00191f0c: vmov.f32 s0,0x3f800000
  00191f10: vmul.f32 s2,s4,s2
  00191f14: vdiv.f32 s22,s0,s2
  00191f18: vstr.32 s2,[sp,#0x28]
  00191f1c: bne 0x00191f34
  00191f1e: lsls r1,r0,#0x1a
  00191f20: bmi 0x00191f5e
  00191f22: ldr.w r1,[r10,#0x10]
  00191f26: lsls r0,r0,#0x19
  00191f28: itt mi
  00191f2a: mov.mi r0,#0x0
  00191f2c: strb.mi.w r0,[r11,#0xc]
  00191f30: mov r0,r1
  00191f32: b 0x00191f96
  00191f34: vldr.32 s0,[r10,#0x2c]
  00191f38: vdiv.f32 s0,s22,s0
  00191f3c: vcvt.s32.f32 s2,s0
  00191f40: vmov r0,s2
  00191f44: vcvt.f32.s32 s2,s2
  00191f48: vldr.32 s4,[r11,#0x60]
  00191f4c: vadd.f32 s4,s4,s16
  00191f50: vsub.f32 s2,s0,s2
  00191f54: vmul.f32 s2,s4,s2
  00191f58: vdiv.f32 s0,s2,s0
  00191f5c: b 0x00191f92
  00191f5e: vldr.32 s0,[r11,#0x60]
  00191f62: vldr.32 s2,[r10,#0x2c]
  00191f66: vadd.f32 s0,s0,s16
  00191f6a: vldr.32 s6,[pc,#0x1dc]
  00191f6e: vmul.f32 s4,s2,s0
  00191f72: vmul.f32 s4,s4,s6
  00191f76: vldr.32 s6,[pc,#0x1d4]
  00191f7a: vcvt.s32.f32 s4,s4
  00191f7e: vmov r0,s4
  00191f82: vcvt.f32.s32 s4,s4
  00191f86: vmul.f32 s4,s4,s6
  00191f8a: vdiv.f32 s2,s4,s2
  00191f8e: vadd.f32 s0,s0,s2
  00191f92: vstr.32 s0,[r11,#0x60]
  00191f96: mov r5,r0
  00191f98: cmp r0,#0x1
  00191f9a: blt.w 0x001926e2
  00191f9e: add r0,sp,#0xc4
  00191fa0: add r1,sp,#0x100
  00191fa2: add r2,sp,#0xd0
  00191fa4: str r4,[sp,#0x20]
  00191fa6: blx 0x0006ec38
  00191faa: vldr.32 s24,[pc,#0x1a4]
  00191fae: ldrb.w r0,[r11,#0x34]
  00191fb2: vmov.f32 s26,s24
  00191fb6: tst r0,#0xc0
  00191fba: bne 0x00191fd4
  00191fbc: vmul.f32 s0,s28,s30
  00191fc0: sub.w r0,r6,r8, asr #0x1
  00191fc4: vmov s2,r0
  00191fc8: vmul.f32 s0,s0,s2
  00191fcc: vmla.f32 s17,s0,s2
  00191fd0: vmul.f32 s26,s17,s2
  00191fd4: add.w r3,r10,#0x88
  00191fd8: add.w r9,sp,#0xb8
  00191fdc: vldr.32 s28,[pc,#0x160]
  00191fe0: movs r4,#0x0
  00191fe2: ldmia r3,{r0,r1,r2,r3}
  00191fe4: mov r8,r5
  00191fe6: vldr.32 s30,[pc,#0x16c]
  00191fea: strd r0,r2,[sp,#0x11c]
  00191fee: add.w r0,r10,#0x4c
  00191ff2: strd r1,r3,[sp,#0x124]
  00191ff6: str r0,[sp,#0x4c]
  00191ff8: add.w r0,r10,#0x48
  00191ffc: str r0,[sp,#0x3c]
  00191ffe: add.w r0,r10,#0x84
  00192002: str r0,[sp,#0x38]
  00192004: add.w r0,r10,#0x40
  00192008: str r0,[sp,#0x2c]
  0019200a: add.w r0,r10,#0x24
  0019200e: str r0,[sp,#0x60]
  00192010: add.w r0,r10,#0x20
  00192014: str r0,[sp,#0x5c]
  00192016: add.w r0,r10,#0x1c
  0019201a: str r0,[sp,#0x58]
  0019201c: add.w r0,r10,#0x80
  00192020: str r0,[sp,#0x34]
  00192022: add.w r0,r10,#0x3c
  00192026: str r0,[sp,#0x54]
  00192028: add.w r0,r10,#0x34
  0019202c: str r0,[sp,#0x50]
  0019202e: add.w r0,r10,#0x7c
  00192032: str r0,[sp,#0x30]
  00192034: add.w r0,r10,#0x2c
  00192038: str r0,[sp,#0x24]
  0019203a: add.w r0,r10,#0x14
  0019203e: str r0,[sp,#0x7c]
  00192040: add.w r0,r10,#0x18
  00192044: str r0,[sp,#0x90]
  00192046: add.w r0,r10,#0x78
  0019204a: str r0,[sp,#0x78]
  0019204c: add.w r0,r10,#0x30
  00192050: str r0,[sp,#0x74]
  00192052: add.w r0,r10,#0x70
  00192056: str r0,[sp,#0x70]
  00192058: add.w r0,r10,#0x6c
  0019205c: str r0,[sp,#0x6c]
  0019205e: add.w r0,r10,#0x68
  00192062: str r0,[sp,#0x68]
  00192064: add.w r0,r10,#0x60
  00192068: str r0,[sp,#0x48]
  0019206a: add.w r0,r10,#0x5c
  0019206e: str r0,[sp,#0x44]
  00192070: add.w r0,r10,#0x58
  00192074: str r0,[sp,#0x40]
  00192076: add.w r0,r10,#0x64
  0019207a: str r0,[sp,#0x94]
  0019207c: add.w r0,r10,#0x50
  00192080: str r0,[sp,#0x64]
  00192082: add.w r0,r11,#0x10
  00192086: mov.w r10,#0x0
  0019208a: str r0,[sp,#0x9c]
  0019208c: add r0,sp,#0x11c
  0019208e: str r0,[sp,#0x8c]
  00192090: b 0x001926d4
  00192092: ldr.w r0,[r11,#0x50]
  00192096: ldr.w r1,[r11,#0x6c]
  0019209a: ldr r2,[sp,#0x88]
  0019209c: strb r2,[r1,r0]
  0019209e: ldr.w r0,[r11,#0x50]
  001920a2: ldr.w r1,[r11,#0x68]
  001920a6: str.w r4,[r1,r0,lsl #0x2]
  001920aa: ldrb.w r0,[r11,#0x37]
  001920ae: lsls r0,r0,#0x1e
  001920b0: bpl 0x001920c2
  001920b2: ldr.w r2,[r11,#0x50]
  001920b6: add r1,sp,#0x11c
  001920b8: add r3,sp,#0x10c
  001920ba: mov r0,r11
  001920bc: blx 0x00078874
  001920c0: str r0,[sp,#0x8c]
  001920c2: ldr r0,[sp,#0x64]
  001920c4: ldr r6,[r0,#0x0]
  001920c6: cmp r6,#0x0
  001920c8: beq 0x0019215c
  001920ca: ldr r0,[0x00192158]
  001920cc: lsls r5,r6,#0x1
  001920ce: ldr r4,[sp,#0x9c]
  001920d0: mov r1,r5
  001920d2: add r0,pc
  001920d4: ldr.w r9,[r0,#0x0]
  001920d8: mov r0,r4
  001920da: blx r9
  001920dc: subs r0,r0,r6
  001920de: ldr r1,[sp,#0x40]
  001920e0: vmov s0,r0
  001920e4: mov r0,r4
  001920e6: vldr.32 s18,[r1]
  001920ea: mov r1,r5
  001920ec: vcvt.f32.s32 s19,s0
  001920f0: blx r9
  001920f2: subs r0,r0,r6
  001920f4: ldr r1,[sp,#0x44]
  001920f6: vmov s0,r0
  001920fa: mov r0,r4
  001920fc: vldr.32 s21,[r1]
  00192100: mov r1,r5
  00192102: vcvt.f32.s32 s23,s0
  00192106: blx r9
  00192108: subs r0,r0,r6
  0019210a: vadd.f32 s4,s18,s19
  0019210e: vadd.f32 s6,s21,s23
  00192112: add.w r9,sp,#0xb8
  00192116: vmov s0,r0
  0019211a: movs r4,#0x0
  0019211c: vcvt.f32.s32 s0,s0
  00192120: ldr r0,[sp,#0x48]
  00192122: vldr.32 s2,[r0]
  00192126: vstr.32 s4,[sp,#0xb8]
  0019212a: vstr.32 s6,[sp,#0xbc]
  0019212e: vadd.f32 s0,s2,s0
  00192132: vstr.32 s0,[sp,#0xc0]
  00192136: b 0x00192162
  0019215c: strd r4,r4,[sp,#0xb8]
  00192160: str r4,[sp,#0xc0]
  00192162: ldr.w r0,[r11,#0x50]
  00192166: ldr.w r1,[r11,#0x64]
  0019216a: add.w r0,r0,r0, lsl #0x1
  0019216e: add.w r0,r1,r0, lsl #0x2
  00192172: mov r1,r9
  00192174: blx 0x0006eb3c
  00192178: ldr r0,[sp,#0x94]
  0019217a: vldr.32 s0,[r0]
  0019217e: vcmpe.f32 s0,#0
  00192182: vmrs apsr,fpscr
  00192186: beq 0x001921aa
  00192188: vmov r2,s0
  0019218c: ldr r1,[sp,#0x84]
  0019218e: mov r0,r9
  00192190: blx 0x0006ec74
  00192194: ldr.w r0,[r11,#0x50]
  00192198: ldr.w r1,[r11,#0x64]
  0019219c: add.w r0,r0,r0, lsl #0x1
  001921a0: add.w r0,r1,r0, lsl #0x2
  001921a4: mov r1,r9
  001921a6: blx 0x00072694
  001921aa: ldr r0,[sp,#0x68]
  001921ac: vldr.32 s0,[r0]
  001921b0: vcmpe.f32 s0,#0
  001921b4: vmrs apsr,fpscr
  001921b8: beq 0x001921dc
  001921ba: vmov r2,s0
  001921be: add r1,sp,#0xf4
  001921c0: mov r0,r9
  001921c2: blx 0x0006ec74
  001921c6: ldr.w r0,[r11,#0x50]
  001921ca: ldr.w r1,[r11,#0x64]
  001921ce: add.w r0,r0,r0, lsl #0x1
  001921d2: add.w r0,r1,r0, lsl #0x2
  001921d6: mov r1,r9
  001921d8: blx 0x00073534
  001921dc: ldr r0,[sp,#0x6c]
  001921de: vldr.32 s0,[r0]
  001921e2: vcmpe.f32 s0,#0
  001921e6: vmrs apsr,fpscr
  001921ea: beq 0x0019220e
  001921ec: vmov r2,s0
  001921f0: add r1,sp,#0xe8
  001921f2: mov r0,r9
  001921f4: blx 0x0006ec74
  001921f8: ldr.w r0,[r11,#0x50]
  001921fc: ldr.w r1,[r11,#0x64]
  00192200: add.w r0,r0,r0, lsl #0x1
  00192204: add.w r0,r1,r0, lsl #0x2
  00192208: mov r1,r9
  0019220a: blx 0x00073534
  0019220e: ldr r0,[sp,#0x70]
  00192210: vldr.32 s0,[r0]
  00192214: vcmpe.f32 s0,#0
  00192218: vmrs apsr,fpscr
  0019221c: beq 0x00192240
  0019221e: vmov r2,s0
  00192222: add r1,sp,#0xdc
  00192224: mov r0,r9
  00192226: blx 0x0006ec74
  0019222a: ldr.w r0,[r11,#0x50]
  0019222e: ldr.w r1,[r11,#0x64]
  00192232: add.w r0,r0,r0, lsl #0x1
  00192236: add.w r0,r1,r0, lsl #0x2
  0019223a: mov r1,r9
  0019223c: blx 0x00073534
  00192240: ldr r0,[sp,#0x74]
  00192242: ldr r0,[r0,#0x0]
  00192244: cmp r0,#0x1
  00192246: bne 0x00192254
  00192248: ldr r0,[sp,#0x98]
  0019224a: vmov s0,r0
  0019224e: vcvt.f32.s32 s19,s0
  00192252: b 0x00192272
  00192254: ldr r0,[sp,#0x9c]
  00192256: movw r1,#0x2710
  0019225a: blx 0x00071848
  0019225e: vmov s0,r0
  00192262: vmov s2,r10
  00192266: vcvt.f32.s32 s0,s0
  0019226a: vcvt.f32.s32 s19,s2
  0019226e: vmla.f32 s19,s0,s30
  00192272: strd r4,r4,[sp,#0xb8]
  00192276: add.w r10,sp,#0xac
  0019227a: str r4,[sp,#0xc0]
  0019227c: ldr.w r0,[r11,#0x34]
  00192280: tst r0,#0xc0
  00192284: beq 0x00192294
  00192286: add r1,sp,#0x100
  00192288: mov r0,r9
  0019228a: blx 0x0006eb3c
  0019228e: vmov.f32 s19,s24
  00192292: b 0x0019230c
  00192294: vmov.f32 s0,0x3f800000
  00192298: vcmpe.f32 s22,s0
  0019229c: vmrs apsr,fpscr
  001922a0: bpl 0x001922b8
  001922a2: add r1,sp,#0x100
  001922a4: mov r0,r9
  001922a6: blx 0x0006eb3c
  001922aa: ldr.w r8,[sp,#0x98]
  001922ae: vmov s0,r8
  001922b2: vcvt.f32.s32 s19,s0
  001922b6: b 0x0019230c
  001922b8: lsls r0,r0,#0x1b
  001922ba: bmi 0x001922ca
  001922bc: vmov s0,r8
  001922c0: vcvt.f32.s32 s0,s0
  001922c4: vdiv.f32 s17,s22,s0
  001922c8: b 0x001922d0
  001922ca: ldr r0,[sp,#0x24]
  001922cc: vldr.32 s17,[r0]
  001922d0: vmul.f32 s0,s19,s17
  001922d4: add r5,sp,#0xa0
  001922d6: add r1,sp,#0xd0
  001922d8: mov r0,r5
  001922da: vmov r2,s0
  001922de: blx 0x0006ec74
  001922e2: vldr.32 s0,[sp,#0x28]
  001922e6: mov r0,r10
  001922e8: mov r1,r5
  001922ea: vmov r2,s0
  001922ee: blx 0x0006ec74
  001922f2: mov r0,r9
  001922f4: mov r1,r10
  001922f6: blx 0x0006eb3c
  001922fa: add r1,sp,#0xc4
  001922fc: mov r0,r10
  001922fe: mov r2,r9
  00192300: blx 0x0006f1cc
  00192304: mov r0,r9
  00192306: mov r1,r10
  00192308: blx 0x0006eb3c
  0019230c: ldr r1,[sp,#0x78]
  0019230e: ldrb.w r0,[r11,#0x34]
  00192312: vldr.32 s0,[r1]
  00192316: lsls r0,r0,#0x18
  00192318: bmi.w 0x0019242c
  0019231c: vcmpe.f32 s0,#0
  00192320: vmrs apsr,fpscr
  00192324: beq 0x0019233a
  00192326: vmov r2,s0
  0019232a: add r1,sp,#0xf4
  0019232c: mov r0,r10
  0019232e: blx 0x0006ec74
  00192332: mov r0,r9
  00192334: mov r1,r10
  00192336: blx 0x00073534
  0019233a: ldr r0,[sp,#0x30]
  0019233c: vldr.32 s0,[r0]
  00192340: vcmpe.f32 s0,#0
  00192344: vmrs apsr,fpscr
  00192348: beq 0x0019235e
  0019234a: vmov r2,s0
  0019234e: add r1,sp,#0xe8
  00192350: mov r0,r10
  00192352: blx 0x0006ec74
  00192356: mov r0,r9
  00192358: mov r1,r10
  0019235a: blx 0x00073534
  0019235e: ldr r0,[sp,#0x34]
  00192360: vldr.32 s0,[r0]
  00192364: vcmpe.f32 s0,#0
  00192368: vmrs apsr,fpscr
  0019236c: beq 0x00192382
  0019236e: vmov r2,s0
  00192372: add r1,sp,#0xdc
  00192374: mov r0,r10
  00192376: blx 0x0006ec74
  0019237a: mov r0,r9
  0019237c: mov r1,r10
  0019237e: blx 0x00073534
  00192382: ldr r0,[sp,#0x38]
  00192384: vldr.32 s0,[r0]
  00192388: vcmpe.f32 s0,#0
  0019238c: vmrs apsr,fpscr
  00192390: beq 0x001923bc
  00192392: vcvt.s32.f32 s0,s0
  00192396: ldr r0,[sp,#0x9c]
  00192398: vmov r1,s0
  0019239c: blx 0x00071848
  001923a0: vmov s0,r0
  001923a4: add r1,sp,#0xdc
  001923a6: mov r0,r10
  001923a8: vcvt.f32.s32 s0,s0
  001923ac: vmov r2,s0
  001923b0: blx 0x0006ec74
  001923b4: mov r0,r9
  001923b6: mov r1,r10
  001923b8: blx 0x00073534
  001923bc: ldr r0,[sp,#0x3c]
  001923be: ldr r6,[r0,#0x0]
  001923c0: cbz r6,0x001923fe
  001923c2: ldr r4,[sp,#0x9c]
  001923c4: lsls r5,r6,#0x1
  001923c6: mov r1,r5
  001923c8: mov r0,r4
  001923ca: blx 0x00071848
  001923ce: subs r0,r0,r6
  001923d0: mov r1,r5
  001923d2: vmov s0,r0
  001923d6: mov r0,r4
  001923d8: vcvt.f32.s32 s18,s0
  001923dc: blx 0x00071848
  001923e0: subs r0,r0,r6
  001923e2: mov r1,r10
  001923e4: vmov s0,r0
  001923e8: movs r0,#0x0
  001923ea: vcvt.f32.s32 s0,s0
  001923ee: str r0,[sp,#0xb0]
  001923f0: mov r0,r9
  001923f2: vstr.32 s18,[sp,#0xac]
  001923f6: vstr.32 s0,[sp,#0xb4]
  001923fa: blx 0x00073534
  001923fe: ldr r0,[sp,#0x4c]
  00192400: mov r9,r8
  00192402: ldr r0,[r0,#0x0]
  00192404: cmp r0,#0x0
  00192406: beq 0x0019248c
  00192408: lsls r1,r0,#0x1
  0019240a: ldr r0,[sp,#0x9c]
  0019240c: blx 0x00071848
  00192410: ldr r1,[sp,#0x4c]
  00192412: ldr r1,[r1,#0x0]
  00192414: subs r0,r0,r1
  00192416: vmov s0,r0
  0019241a: vcvt.f32.s32 s0,s0
  0019241e: vldr.32 s2,[sp,#0xbc]
  00192422: vadd.f32 s0,s2,s0
  00192426: vstr.32 s0,[sp,#0xbc]
  0019242a: b 0x0019248c
  0019242c: ldr r0,[0x00192700]
  0019242e: vcvt.s32.f32 s0,s0
  00192432: ldr r4,[sp,#0x9c]
  00192434: mov r10,r9
  00192436: add r0,pc
  00192438: mov r9,r8
  0019243a: ldr.w r8,[r0,#0x0]
  0019243e: mov r0,r4
  00192440: vmov r6,s0
  00192444: lsls r5,r6,#0x1
  00192446: mov r1,r5
  00192448: blx r8
  0019244a: subs r0,r0,r6
  0019244c: mov r1,r5
  0019244e: vmov s0,r0
  00192452: mov r0,r4
  00192454: vcvt.f32.s32 s18,s0
  00192458: blx r8
  0019245a: subs r0,r0,r6
  0019245c: mov r1,r5
  0019245e: vmov s0,r0
  00192462: mov r0,r4
  00192464: vcvt.f32.s32 s21,s0
  00192468: blx r8
  0019246a: subs r0,r0,r6
  0019246c: vmov s0,r0
  00192470: mov r0,r10
  00192472: add.w r10,sp,#0xac
  00192476: vcvt.f32.s32 s0,s0
  0019247a: vstr.32 s18,[sp,#0xac]
  0019247e: mov r1,r10
  00192480: vstr.32 s21,[sp,#0xb0]
  00192484: vstr.32 s0,[sp,#0xb4]
  00192488: blx 0x00073534
  0019248c: ldr r0,[sp,#0x7c]
  0019248e: vldr.32 s21,[r0]
  00192492: ldr r0,[sp,#0x90]
  00192494: ldr r1,[r0,#0x0]
  00192496: cbz r1,0x001924cc
  00192498: ldr r0,[sp,#0x9c]
  0019249a: blx 0x00071848
  0019249e: vmov s0,r0
  001924a2: mov r10,r9
  001924a4: vcvt.f32.s32 s0,s0
  001924a8: ldr r0,[sp,#0x8c]
  001924aa: vldr.32 s25,[r0]
  001924ae: vldr.32 s23,[r0,#0x4]
  001924b2: vldr.32 s29,[r0,#0x8]
  001924b6: vldr.32 s27,[r0,#0xc]
  001924ba: ldr r0,[sp,#0x50]
  001924bc: ldr r1,[r0,#0x0]
  001924be: ldr r0,[sp,#0x54]
  001924c0: str r1,[sp,#0x80]
  001924c2: ldr r0,[r0,#0x0]
  001924c4: cmp r0,#0x0
  001924c6: ble 0x001924ee
  001924c8: movs r5,#0x1
  001924ca: b 0x00192502
  001924cc: ldr r0,[sp,#0x8c]
  001924ce: vldr.32 s25,[r0]
  001924d2: vldr.32 s23,[r0,#0x4]
  001924d6: vldr.32 s29,[r0,#0x8]
  001924da: vldr.32 s27,[r0,#0xc]
  001924de: ldr r0,[sp,#0x50]
  001924e0: ldr r5,[r0,#0x0]
  001924e2: ldr r0,[sp,#0x54]
  001924e4: ldr r0,[r0,#0x0]
  001924e6: cmp r0,#0x0
  001924e8: ble 0x001925b4
  001924ea: movs r6,#0x1
  001924ec: b 0x001925c8
  001924ee: ldr r0,[sp,#0x2c]
  001924f0: movs r5,#0x0
  001924f2: vldr.32 s2,[r0]
  001924f6: vcmpe.f32 s2,#0
  001924fa: vmrs apsr,fpscr
  001924fe: it gt
  00192500: mov.gt r5,#0x1
  00192502: ldr r0,[sp,#0x58]
  00192504: add.w r9,sp,#0xb8
  00192508: ldr r4,[sp,#0x9c]
  0019250a: vadd.f32 s21,s21,s0
  0019250e: vldr.32 s18,[r0]
  00192512: ldr r0,[sp,#0x90]
  00192514: ldr r1,[r0,#0x0]
  00192516: mov r6,r0
  00192518: mov r0,r4
  0019251a: blx 0x00071848
  0019251e: ldr r1,[sp,#0x5c]
  00192520: vmov s0,r0
  00192524: mov r0,r4
  00192526: vldr.32 s20,[r1]
  0019252a: ldr r1,[r6,#0x0]
  0019252c: vcvt.f32.s32 s31,s0
  00192530: blx 0x00071848
  00192534: vmov s0,r0
  00192538: vadd.f32 s31,s18,s31
  0019253c: vcvt.f32.s32 s0,s0
  00192540: ldr r0,[sp,#0x60]
  00192542: vadd.f32 s18,s20,s0
  00192546: vldr.32 s0,[r0]
  0019254a: vcmpe.f32 s0,#0
  0019254e: vmrs apsr,fpscr
  00192552: beq 0x00192576
  00192554: vmov r1,s0
  00192558: ldr.w r0,[r11,#0x50]
  0019255c: ldr.w r2,[r11,#0x64]
  00192560: add.w r8,sp,#0xac
  00192564: add.w r0,r0,r0, lsl #0x1
  00192568: add.w r2,r2,r0, lsl #0x2
  0019256c: mov r0,r8
  0019256e: blx 0x00072658
  00192572: movs r4,#0x0
  00192574: b 0x00192582
  00192576: add.w r8,sp,#0xac
  0019257a: movs r4,#0x0
  0019257c: strd r4,r4,[sp,#0xac]
  00192580: str r4,[sp,#0xb4]
  00192582: vmov r2,s21
  00192586: ldr.w r0,[r11,#0x0]
  0019258a: ldr r3,[sp,#0x80]
  0019258c: mov r1,r9
  0019258e: ldr r6,[r0,#0x18]
  00192590: mov r0,r11
  00192592: str.w r8,[sp,#0x1c]
  00192596: vstr.32 s31,[sp,#0x14]
  0019259a: vstr.32 s18,[sp,#0x18]
  0019259e: str r5,[sp,#0x10]
  001925a0: vstr.32 s25,[sp]
  001925a4: vstr.32 s29,[sp,#0x4]
  001925a8: vstr.32 s23,[sp,#0x8]
  001925ac: vstr.32 s27,[sp,#0xc]
  001925b0: blx r6
  001925b2: b 0x00192640
  001925b4: ldr r0,[sp,#0x2c]
  001925b6: movs r6,#0x0
  001925b8: vldr.32 s0,[r0]
  001925bc: vcmpe.f32 s0,#0
  001925c0: vmrs apsr,fpscr
  001925c4: it gt
  001925c6: mov.gt r6,#0x1
  001925c8: ldr r0,[sp,#0x5c]
  001925ca: vldr.32 s31,[r0]
  001925ce: ldr r0,[sp,#0x58]
  001925d0: vldr.32 s18,[r0]
  001925d4: ldr r0,[sp,#0x60]
  001925d6: vldr.32 s0,[r0]
  001925da: vcmpe.f32 s0,#0
  001925de: vmrs apsr,fpscr
  001925e2: beq 0x00192600
  001925e4: vmov r1,s0
  001925e8: ldr.w r0,[r11,#0x50]
  001925ec: ldr.w r2,[r11,#0x64]
  001925f0: add.w r0,r0,r0, lsl #0x1
  001925f4: add.w r2,r2,r0, lsl #0x2
  001925f8: mov r0,r10
  001925fa: blx 0x00072658
  001925fe: b 0x00192608
  00192600: movs r0,#0x0
  00192602: strd r0,r0,[sp,#0xac]
  00192606: str r0,[sp,#0xb4]
  00192608: vmov r2,s21
  0019260c: ldr.w r0,[r11,#0x0]
  00192610: mov r3,r5
  00192612: ldr r4,[r0,#0x18]
  00192614: mov r0,r11
  00192616: str r6,[sp,#0x10]
  00192618: add r6,sp,#0xb8
  0019261a: str.w r10,[sp,#0x1c]
  0019261e: mov r1,r6
  00192620: vstr.32 s18,[sp,#0x14]
  00192624: vstr.32 s31,[sp,#0x18]
  00192628: vstr.32 s25,[sp]
  0019262c: vstr.32 s29,[sp,#0x4]
  00192630: vstr.32 s23,[sp,#0x8]
  00192634: vstr.32 s27,[sp,#0xc]
  00192638: blx r4
  0019263a: mov r10,r9
  0019263c: mov r9,r6
  0019263e: movs r4,#0x0
  00192640: ldr r0,[sp,#0x94]
  00192642: vldr.32 s0,[r0]
  00192646: vcmpe.f32 s0,#0
  0019264a: vmrs apsr,fpscr
  0019264e: beq 0x00192682
  00192650: vmov r2,s0
  00192654: add r5,sp,#0xa0
  00192656: ldr r1,[sp,#0x84]
  00192658: mov r0,r5
  0019265a: blx 0x0006ec74
  0019265e: add r6,sp,#0xac
  00192660: mov r1,r5
  00192662: mov.w r2,#0x40000000
  00192666: mov r0,r6
  00192668: blx 0x0006ec74
  0019266c: ldr.w r0,[r11,#0x50]
  00192670: ldr.w r1,[r11,#0x64]
  00192674: add.w r0,r0,r0, lsl #0x1
  00192678: add.w r0,r1,r0, lsl #0x2
  0019267c: mov r1,r6
  0019267e: blx 0x00073534
  00192682: mov r8,r10
  00192684: vmov s0,r8
  00192688: vcvt.f32.s32 s0,s0
  0019268c: vsub.f32 s0,s0,s19
  00192690: vmul.f32 s0,s17,s0
  00192694: vmul.f32 s0,s0,s28
  00192698: vmul.f32 s0,s26,s0
  0019269c: vcmpe.f32 s0,s16
  001926a0: vmrs apsr,fpscr
  001926a4: it gt
  001926a6: vmov.gt.f32 s0,s16
  001926aa: ldr.w r0,[r11,#0x0]
  001926ae: vmov r2,s0
  001926b2: ldr.w r1,[r11,#0x50]
  001926b6: ldr r3,[r0,#0x14]
  001926b8: mov r0,r11
  001926ba: blx r3
  001926bc: ldr.w r1,[r11,#0x50]
  001926c0: ldr.w r0,[r11,#0x48]
  001926c4: adds r1,#0x1
  001926c6: cmp r1,r0
  001926c8: it ge
  001926ca: mov.ge r1,r4
  001926cc: str.w r1,[r11,#0x50]
  001926d0: ldr.w r10,[sp,#0x98]
  001926d4: add.w r0,r10,#0x1
  001926d8: str r0,[sp,#0x98]
  001926da: cmp r0,r8
  001926dc: ble.w 0x00192092
  001926e0: ldr r4,[sp,#0x20]
  001926e2: ldr r0,[sp,#0x12c]
  001926e4: ldr r1,[r4,#0x0]
  001926e6: subs r0,r1,r0
  001926e8: itttt eq
  001926ea: add.eq sp,#0x130
  001926ec: vpop.eq {d8,d9,d10,d11,d12,d13,d14,d15}
  001926f0: add.eq sp,#0x4
  001926f2: pop.eq.w {r8,r9,r10,r11}
  001926f6: it eq
  001926f8: pop.eq {r4,r5,r6,r7,pc}
  001926fa: blx 0x0006e824
```
