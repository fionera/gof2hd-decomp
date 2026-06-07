# AERandom::nextInt
elf 0x71ad0 body 16
Sig: undefined __stdcall nextInt(int param_1)

## decompile
```c

/* AbyssEngine::AERandom::nextInt(int) */

void AbyssEngine::AERandom::nextInt(int param_1)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  SolarSystem *pSVar4;
  int iVar5;
  uint in_r1;
  int unaff_r4;
  int unaff_r5;
  int *piVar6;
  code *pcVar7;
  uint unaff_r9;
  int iVar8;
  undefined4 *puVar9;
  bool in_NG;
  bool in_ZR;
  char in_OV;
  uint in_fpscr;
  float fVar10;
  int *in_stack_00000000;
  uint in_stack_00000004;
  float in_stack_0000000c;
  float in_stack_00000010;
  float in_stack_00000014;
  int in_stack_00000018;
  
  if (in_NG == (bool)in_OV) {
    software_interrupt(0x3b5f0);
  }
  if (in_ZR) {
    software_interrupt(0x80e92d);
  }
  if (in_NG) {
    unaff_r4 = unaff_r9 + 0xf7600000;
  }
  do {
    piVar6 = *(int **)(in_r1 + unaff_r9 * 4);
    if ((((0 < *(int *)(unaff_r4 + 0x160)) && ((int)unaff_r9 < *(int *)(unaff_r4 + 0x160))) &&
        (iVar3 = KIPlayer::isDead(), iVar3 != 0)) &&
       ((iVar3 = Player::isActive(), iVar3 == 0 && (*(char *)((int)piVar6 + 0x42) == '\0')))) {
      (**(code **)(*piVar6 + 0x18))(piVar6);
      (**(code **)(*piVar6 + 0x48))(piVar6,0,0,0);
    }
    if (((1 < unaff_r5) && (*(int *)(unaff_r4 + 0x184) < 2)) &&
       ((0 < *(int *)(unaff_r4 + 0x16c) &&
        ((uint)(**(int **)(unaff_r4 + 0xf8) - *(int *)(unaff_r4 + 0x16c)) <= unaff_r9)))) {
      iVar8 = piVar6[10];
      iVar3 = Status::inAlienOrbit();
      if (iVar3 == 0) {
        pSVar4 = (SolarSystem *)Status::getSystem();
        iVar3 = SolarSystem::getSecurityLevel(pSVar4);
        bVar1 = iVar3 == 0;
      }
      else {
        bVar1 = false;
      }
      iVar3 = Status::inAlienOrbit();
      if (iVar3 == 0) {
        pSVar4 = (SolarSystem *)Status::getSystem();
        iVar3 = SolarSystem::getSecurityLevel(pSVar4);
        bVar2 = iVar3 == 1;
      }
      else {
        bVar2 = false;
      }
      iVar3 = KIPlayer::isDead();
      if (((iVar3 != 0) && (iVar3 = Player::isActive(), iVar3 == 0)) &&
         (*(char *)((int)piVar6 + 0x42) == '\0')) {
        if ((bool)(iVar8 != 9 & (bVar1 ^ 1U))) {
          if ((!bVar2) || (iVar3 = *(int *)(unaff_r4 + 0x17c), 2 < iVar3)) goto LAB_000d535a;
        }
        else {
          iVar3 = *(int *)(unaff_r4 + 0x17c);
        }
        *(int *)(unaff_r4 + 0x17c) = iVar3 + 1;
        (**(code **)(*piVar6 + 0x18))(piVar6);
        PlayerEgo::getPosition();
        puVar9 = *(undefined4 **)(DAT_000d55c8 + 0xd546a);
        pcVar7 = *(code **)(DAT_000d55cc + 0xd546c);
        iVar3 = (*pcVar7)(*puVar9,60000);
        iVar8 = (*pcVar7)(*puVar9,2);
        iVar5 = -1;
        if (iVar8 == 0) {
          iVar5 = 1;
        }
        fVar10 = (float)VectorSignedToFloat(iVar5 * (iVar3 + 60000),(byte)(in_fpscr >> 0x16) & 3);
        in_stack_0000000c = in_stack_0000000c + fVar10;
        iVar3 = (*pcVar7)(*puVar9,5000);
        iVar8 = (*pcVar7)(*puVar9,2);
        iVar5 = -1;
        if (iVar8 == 0) {
          iVar5 = 1;
        }
        fVar10 = (float)VectorSignedToFloat(iVar5 * (iVar3 + 5000),(byte)(in_fpscr >> 0x16) & 3);
        in_stack_00000010 = in_stack_00000010 + fVar10;
        iVar3 = (*pcVar7)(*puVar9,60000);
        iVar8 = (*pcVar7)(*puVar9,2);
        iVar5 = -1;
        if (iVar8 == 0) {
          iVar5 = 1;
        }
        fVar10 = (float)VectorSignedToFloat(iVar5 * (iVar3 + 60000),(byte)(in_fpscr >> 0x16) & 3);
        in_stack_00000014 = in_stack_00000014 + fVar10;
        (**(code **)(*piVar6 + 0x48))(piVar6,in_stack_0000000c,in_stack_00000010,in_stack_00000014);
        in_stack_00000004 = 1;
      }
    }
LAB_000d535a:
    unaff_r9 = unaff_r9 + 1;
    if (**(uint **)(unaff_r4 + 0xf8) <= unaff_r9) {
      if ((in_stack_00000004 & 1) != 0) {
        *(int *)(unaff_r4 + 0x184) = *(int *)(unaff_r4 + 0x184) + 1;
      }
      if (*in_stack_00000000 != in_stack_00000018) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return;
    }
    in_r1 = (*(uint **)(unaff_r4 + 0xf8))[1];
  } while( true );
}

```

## target disasm
```
  00081ad0: swige 0x3b5f0
  00081ad4: swieq 0x80e92d
  00081ad8: submi r4,r9,#0x8a00000
  00081adc: b 0x000d5364
```
