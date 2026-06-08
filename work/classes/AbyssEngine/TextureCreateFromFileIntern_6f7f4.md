# AbyssEngine::TextureCreateFromFileIntern
elf 0x6f7f4 body 1956
Sig: undefined __stdcall TextureCreateFromFileIntern(Engine * param_1, char * param_2, _func_void_Image_ptr_void_ptr * param_3, void * param_4, uint * param_5, float param_6, AELoadedTexture * param_7, bool param_8)

## decompile
```c

/* AbyssEngine::TextureCreateFromFileIntern(AbyssEngine::Engine*, char const*, void
   (*)(AbyssEngine::Image*, void*), void*, unsigned int*, float, AbyssEngine::AELoadedTexture*,
   bool) */

void AbyssEngine::TextureCreateFromFileIntern
               (Engine *param_1,char *param_2,_func_void_Image_ptr_void_ptr *param_3,void *param_4,
               uint *param_5,float param_6,AELoadedTexture *param_7,bool param_8)

{
  Image *pIVar1;
  undefined4 uVar2;
  int iVar3;
  AELoadedTexture *pAVar4;
  String *this;
  undefined4 *puVar5;
  uint uVar6;
  undefined4 uVar7;
  uint uVar8;
  undefined4 uVar9;
  int *piVar10;
  ushort uVar11;
  code *pcVar12;
  char *pcVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  int *piVar17;
  int iVar18;
  uint uVar19;
  undefined3 in_stack_00000009;
  int in_stack_0000000c;
  ushort uVar20;
  String aSStack_38 [12];
  Image *local_2c;
  int local_28;
  
  piVar17 = *(int **)(DAT_0007fb84 + 0x7f80c);
  local_28 = *piVar17;
  local_2c = (Image *)0x0;
  *param_5 = 0;
  uVar2 = glGetError();
  *(undefined4 *)(param_1 + 0x10) = uVar2;
  iVar3 = ImageCreateFromFile(param_1,param_2,&local_2c);
  if (iVar3 != 1) goto LAB_0007ffb0;
  if (param_3 != (_func_void_Image_ptr_void_ptr *)0x0) {
    (*param_3)(local_2c,param_4);
  }
  glGenTextures(1,param_5);
  if (*(int *)(local_2c + 4) == 6) {
    if (**(char **)(DAT_0007fb88 + 0x7f860) != '\0') {
      glBindTexture(0x8513,*param_5);
      glTexParameteri(0x8513,0x2800,0x2601);
      glTexParameteri(0x8513,0x2801,0x2601);
      pIVar1 = local_2c;
      iVar3 = __aeabi_uidiv(*(ushort *)(local_2c + 2),6);
      uVar14 = (uint)*(ushort *)pIVar1;
      pcVar12 = *(code **)(DAT_0007fb8c + 0x7f8ac);
      (*pcVar12)(0x8517,0,0x1908,uVar14,iVar3,0,0x1908,0x1401,*(undefined4 *)(pIVar1 + 0xc));
      iVar18 = iVar3 * uVar14;
      (*pcVar12)(0x8516,0,0x1908,uVar14,iVar3,0,0x1908,0x1401,*(int *)(local_2c + 0xc) + iVar18 * 4)
      ;
      (*pcVar12)(0x8519,0,0x1908,uVar14,iVar3,0,0x1908,0x1401,*(int *)(local_2c + 0xc) + iVar18 * 8)
      ;
      (*pcVar12)(0x8515,0,0x1908,uVar14,iVar3,0,0x1908,0x1401,
                 *(int *)(local_2c + 0xc) + iVar18 * 0xc);
      (*pcVar12)(0x851a,0,0x1908,uVar14,iVar3,0,0x1908,0x1401,
                 *(int *)(local_2c + 0xc) + iVar18 * 0x10);
      (*pcVar12)(0x8518,0,0x1908,uVar14,iVar3,0,0x1908,0x1401,
                 *(int *)(local_2c + 0xc) + iVar18 * 0x14);
    }
    goto switchD_0007faba_caseD_6;
  }
  glBindTexture(0xde1,*param_5);
  glPixelStorei(0xcf5,1);
  if (**(char **)(DAT_0007fb90 + 0x7f998) == '\0') {
    glTexEnvi(0x2300,0x2200,0x2100);
  }
  pcVar13 = *(char **)(DAT_0007fb94 + 0x7f9b2);
  if (*pcVar13 == '\0') {
    glTexParameteri(0xde1,0x2802,0x2901);
    uVar2 = 0x2901;
  }
  else {
    glTexParameteri(0xde1,0x2802,0x812f);
    uVar2 = 0x812f;
  }
  glTexParameteri(0xde1,0x2803,uVar2);
  if (local_2c[8] == (Image)0x0) {
    if (param_1[0x20] != (Engine)0x0) {
      glTexParameteri(0xde1,0x2800,0x2601);
      uVar2 = 0x2801;
      goto LAB_0007fa64;
    }
    glTexParameteri(0xde1,0x2800,0x2600);
    uVar2 = 0x2801;
    uVar7 = 0x2600;
  }
  else if (**(float **)(DAT_0007fb98 + 0x7fa00) <= 0.0) {
    glTexParameteri(0xde1,0x2801,0x2703);
    uVar2 = 0x2800;
LAB_0007fa64:
    uVar7 = 0x2601;
  }
  else {
    glTexParameterf(0xde1,0x84fe,**(float **)(DAT_0007fb98 + 0x7fa00));
    uVar2 = 0x2801;
    uVar7 = 0x2703;
  }
  glTexParameteri(0xde1,uVar2,uVar7);
  if (*pcVar13 != '\0') {
    glTexParameteri(0xde1,0x2800,0x2601);
    glTexParameteri(0xde1,0x2801,0x2601);
  }
  iVar3 = *(int *)(local_2c + 4);
  switch(iVar3) {
  case 1:
    uVar11 = *(ushort *)local_2c;
    uVar20 = *(ushort *)(local_2c + 2);
    uVar2 = *(undefined4 *)(local_2c + 0xc);
    uVar9 = 0x1909;
    uVar7 = 0x1909;
    goto LAB_0007fc06;
  case 2:
    uVar11 = *(ushort *)local_2c;
    uVar20 = *(ushort *)(local_2c + 2);
    uVar2 = *(undefined4 *)(local_2c + 0xc);
    uVar9 = 0x1907;
    uVar7 = 0x1907;
    goto LAB_0007fc06;
  case 3:
    uVar11 = *(ushort *)local_2c;
    uVar20 = *(ushort *)(local_2c + 2);
    uVar2 = *(undefined4 *)(local_2c + 0xc);
    uVar9 = 0x1908;
    uVar7 = 0x1908;
LAB_0007fc06:
    glTexImage2D(0xde1,0,uVar7,uVar11,uVar20,0,uVar9,0x1401,uVar2);
    if (local_2c[8] != (Image)0x0) {
      glGenerateMipmap(0xde1);
    }
    break;
  case 4:
    uVar16 = (uint)*(ushort *)(local_2c + 2);
    uVar14 = (uint)*(ushort *)local_2c;
    if (local_2c[8] == (Image)0x0) {
      uVar7 = *(undefined4 *)(local_2c + 0xc);
      uVar2 = *(undefined4 *)(local_2c + 0x10);
      uVar9 = 0x8c03;
LAB_0007fe48:
      glCompressedTexImage2D(0xde1,0,uVar9,uVar14,uVar16,0,uVar2,uVar7);
    }
    else {
      iVar3 = 0;
      for (uVar19 = 0; uVar19 < *(uint *)(local_2c + 0x10); uVar19 = iVar18 * uVar15 + uVar19) {
        uVar15 = uVar14 >> 3;
        iVar18 = (uVar16 & 0x7ffffffc) << 1;
        if (uVar14 < 0x10) {
          uVar15 = 2;
        }
        if (uVar16 < 8) {
          iVar18 = 0x10;
        }
        glCompressedTexImage2D
                  (0xde1,iVar3,0x8c03,uVar14,uVar16,0,iVar18 * uVar15,
                   *(int *)(local_2c + 0xc) + uVar19);
        uVar6 = 1;
        uVar8 = 1;
        if (1 < uVar16 >> 1) {
          uVar6 = uVar16 >> 1;
        }
        if (1 < uVar14 >> 1) {
          uVar8 = uVar14 >> 1;
        }
        iVar3 = iVar3 + 1;
        uVar14 = uVar8;
        uVar16 = uVar6;
      }
    }
    break;
  case 5:
    uVar16 = (uint)*(ushort *)(local_2c + 2);
    uVar14 = (uint)*(ushort *)local_2c;
    if (local_2c[8] == (Image)0x0) {
      uVar7 = *(undefined4 *)(local_2c + 0xc);
      uVar2 = *(undefined4 *)(local_2c + 0x10);
      uVar9 = 0x8c02;
      goto LAB_0007fe48;
    }
    iVar3 = 0;
    for (uVar19 = 0; uVar19 < *(uint *)(local_2c + 0x10); uVar19 = iVar18 * uVar15 + uVar19) {
      uVar15 = uVar16 >> 2;
      iVar18 = (uVar14 & 0x7ffffffc) << 1;
      if (uVar16 < 8) {
        uVar15 = 2;
      }
      if (uVar14 < 8) {
        iVar18 = 0x10;
      }
      glCompressedTexImage2D
                (0xde1,iVar3,0x8c02,uVar14,uVar16,0,iVar18 * uVar15,
                 *(int *)(local_2c + 0xc) + uVar19);
      uVar6 = 1;
      uVar8 = 1;
      if (1 < uVar16 >> 1) {
        uVar6 = uVar16 >> 1;
      }
      if (1 < uVar14 >> 1) {
        uVar8 = uVar14 >> 1;
      }
      iVar3 = iVar3 + 1;
      uVar14 = uVar8;
      uVar16 = uVar6;
    }
    break;
  case 7:
    uVar16 = (uint)*(ushort *)(local_2c + 2);
    uVar14 = (uint)*(ushort *)local_2c;
    if (local_2c[8] == (Image)0x0) {
      uVar7 = *(undefined4 *)(local_2c + 0xc);
      uVar2 = *(undefined4 *)(local_2c + 0x10);
      uVar9 = 0x8c93;
      goto LAB_0007fe48;
    }
    iVar3 = 0;
    for (uVar19 = 0; uVar19 < *(uint *)(local_2c + 0x10); uVar19 = uVar19 + uVar15) {
      uVar15 = uVar16 * uVar14;
      if (uVar15 < 0x11) {
        uVar15 = 0x10;
      }
      glCompressedTexImage2D
                (0xde1,iVar3,0x8c93,uVar14,uVar16,0,uVar15,*(int *)(local_2c + 0xc) + uVar19);
      uVar6 = 1;
      uVar8 = 1;
      if (1 < uVar16 >> 1) {
        uVar6 = uVar16 >> 1;
      }
      if (1 < uVar14 >> 1) {
        uVar8 = uVar14 >> 1;
      }
      iVar3 = iVar3 + 1;
      uVar14 = uVar8;
      uVar16 = uVar6;
    }
    break;
  case 8:
  case 9:
  case 10:
    uVar9 = 0x83f0;
    uVar16 = (uint)*(ushort *)(local_2c + 2);
    if (iVar3 == 9) {
      uVar9 = 0x83f2;
    }
    uVar14 = (uint)*(ushort *)local_2c;
    if (iVar3 == 10) {
      uVar9 = 0x83f3;
    }
    if (local_2c[8] == (Image)0x0) {
      uVar7 = *(undefined4 *)(local_2c + 0xc);
      uVar2 = *(undefined4 *)(local_2c + 0x10);
      goto LAB_0007fe48;
    }
    iVar3 = 0;
    for (uVar19 = 0; uVar19 < *(uint *)(local_2c + 0x10); uVar19 = uVar19 + uVar15) {
      uVar15 = uVar16 * uVar14;
      uVar6 = *(uint *)(local_2c + 4);
      if (uVar6 - 9 < 2) {
        if ((int)uVar15 < 0x11) {
          uVar15 = 0x10;
        }
      }
      else if (uVar15 < 0x10) {
        uVar15 = uVar6;
        if (uVar6 != 8) {
          uVar15 = 0x10;
        }
      }
      else {
        uVar15 = uVar15 >> 1;
      }
      glCompressedTexImage2D
                (0xde1,iVar3,uVar9,uVar14,uVar16,0,uVar15,*(int *)(local_2c + 0xc) + uVar19);
      uVar6 = 1;
      uVar8 = 1;
      if (1 < uVar16 >> 1) {
        uVar6 = uVar16 >> 1;
      }
      if (1 < uVar14 >> 1) {
        uVar8 = uVar14 >> 1;
      }
      iVar3 = iVar3 + 1;
      uVar16 = uVar6;
      uVar14 = uVar8;
    }
    break;
  case 0xb:
    uVar16 = (uint)*(ushort *)(local_2c + 2);
    uVar14 = (uint)*(ushort *)local_2c;
    if (local_2c[8] == (Image)0x0) {
      uVar7 = *(undefined4 *)(local_2c + 0xc);
      uVar2 = *(undefined4 *)(local_2c + 0x10);
      uVar9 = 0x8d64;
      goto LAB_0007fe48;
    }
    iVar3 = 0;
    for (uVar19 = 0; uVar19 < *(uint *)(local_2c + 0x10); uVar19 = uVar19 + uVar15) {
      uVar15 = uVar16 * uVar14 >> 1;
      if (uVar14 < 8) {
        uVar15 = 8;
      }
      glCompressedTexImage2D
                (0xde1,iVar3,0x8d64,uVar14,uVar16,0,uVar15,*(int *)(local_2c + 0xc) + uVar19);
      uVar6 = 1;
      uVar8 = 1;
      if (1 < uVar16 >> 1) {
        uVar6 = uVar16 >> 1;
      }
      if (1 < uVar14 >> 1) {
        uVar8 = uVar14 >> 1;
      }
      iVar3 = iVar3 + 1;
      uVar14 = uVar8;
      uVar16 = uVar6;
    }
  }
switchD_0007faba_caseD_6:
  if (in_stack_0000000c == 0) {
    iVar3 = glGetError();
    *(int *)(param_1 + 0x10) = iVar3;
    if (iVar3 != 0) {
      String::String(aSStack_38,param_2,false);
      String::operator=((String *)(param_1 + 0x14),aSStack_38);
      String::~String(aSStack_38);
      glDeleteTextures(1,param_5);
      ImageRelease(&local_2c);
      iVar3 = -4;
      goto LAB_0007ffb0;
    }
    AELabelObject(0x1702,*param_5,param_2);
  }
  else if (_param_8 == (uint *)0x0) {
    pAVar4 = operator_new(0x1c);
    this = (String *)String::String((String *)(pAVar4 + 4));
    iVar3 = *(int *)(local_2c + 4);
    *(AELoadedTexture **)(pAVar4 + 0x10) = param_7;
    pAVar4[0x15] = (AELoadedTexture)0x1;
    pAVar4[0x14] = (AELoadedTexture)(iVar3 == 6);
    String::Set(this,param_2);
    iVar3 = glGetError();
    if (iVar3 == 0) {
      AELabelObject(0x1702,*param_5,param_2);
      puVar5 = (undefined4 *)(DAT_0007ffe0 + 0x7ff7a);
      *(uint *)pAVar4 = *param_5;
      piVar10 = (int *)*puVar5;
      iVar3 = *(int *)(local_2c + 0x10);
      iVar18 = *piVar10;
      *(int *)(pAVar4 + 0x18) = iVar3;
      *piVar10 = iVar18 + 1;
    }
    else {
      glDeleteTextures(1,param_5);
      iVar3 = 0;
      *(undefined4 *)(pAVar4 + 0x18) = 0;
      *(undefined4 *)pAVar4 = 0xffffffff;
    }
    *(int *)(param_1 + 0x70) = iVar3 + *(int *)(param_1 + 0x70);
    ArrayAdd<AbyssEngine::AELoadedTexture*>(pAVar4,(Array *)(**(int **)(param_1 + 0x30) + 0x10));
    *param_5 = *(int *)(**(int **)(param_1 + 0x30) + 0x10) - 1;
  }
  else {
    iVar3 = *(int *)(local_2c + 4);
    *(undefined1 *)((int)_param_8 + 0x15) = 1;
    *(bool *)(_param_8 + 5) = iVar3 == 6;
    iVar3 = glGetError();
    if (iVar3 == 0) {
      AELabelObject(0x1702,*param_5,param_2);
      iVar3 = DAT_0007ffe4;
      *_param_8 = *param_5;
      piVar10 = *(int **)(iVar3 + 0x7ff4e);
      uVar14 = *(uint *)(local_2c + 0x10);
      _param_8[6] = uVar14;
      *piVar10 = *piVar10 + 1;
    }
    else {
      *_param_8 = 0xffffffff;
      _param_8[6] = 0;
      glDeleteTextures(1,param_5);
      uVar14 = _param_8[6];
    }
    *(uint *)(param_1 + 0x70) = uVar14 + *(int *)(param_1 + 0x70);
  }
  ImageRelease(&local_2c);
  iVar3 = 1;
LAB_0007ffb0:
  if (*piVar17 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  0007f7f4: push {r4,r5,r6,r7,lr}
  0007f7f6: add r7,sp,#0xc
  0007f7f8: push {r8,r9,r10,r11}
  0007f7fc: sub sp,#0x34
  0007f7fe: mov r10,r0
  0007f800: ldr r0,[0x0007fb84]
  0007f802: ldr.w r9,[r7,#0x8]
  0007f806: mov r5,r3
  0007f808: add r0,pc
  0007f80a: mov r4,r2
  0007f80c: mov r6,r1
  0007f80e: ldr.w r8,[r0,#0x0]
  0007f812: ldr.w r0,[r8,#0x0]
  0007f816: str r0,[sp,#0x30]
  0007f818: movs r0,#0x0
  0007f81a: str r0,[sp,#0x2c]
  0007f81c: str.w r0,[r9,#0x0]
  0007f820: blx 0x0006ed70
  0007f824: add r2,sp,#0x2c
  0007f826: str.w r0,[r10,#0x10]
  0007f82a: mov r0,r10
  0007f82c: mov r1,r6
  0007f82e: blx 0x0006f22c
  0007f832: cmp r0,#0x1
  0007f834: bne.w 0x0007ffb0
  0007f838: cbz r4,0x0007f840
  0007f83a: ldr r0,[sp,#0x2c]
  0007f83c: mov r1,r5
  0007f83e: blx r4
  0007f840: movs r0,#0x1
  0007f842: mov r1,r9
  0007f844: blx 0x0006f238
  0007f848: ldr r0,[sp,#0x2c]
  0007f84a: str.w r10,[sp,#0x1c]
  0007f84e: ldr r0,[r0,#0x4]
  0007f850: strd r8,r6,[sp,#0x14]
  0007f854: cmp r0,#0x6
  0007f856: bne.w 0x0007f97c
  0007f85a: ldr r0,[0x0007fb88]
  0007f85c: add r0,pc
  0007f85e: ldr r0,[r0,#0x0]
  0007f860: ldrb r0,[r0,#0x0]
  0007f862: cmp r0,#0x0
  0007f864: beq.w 0x0007fe4c
  0007f868: ldr r0,[r7,#0x8]
  0007f86a: ldr r1,[r0,#0x0]
  0007f86c: movw r0,#0x8513
  0007f870: blx 0x0006f244
  0007f874: movw r0,#0x8513
  0007f878: mov.w r1,#0x2800
  0007f87c: movw r2,#0x2601
  0007f880: blx 0x0006f250
  0007f884: movw r0,#0x8513
  0007f888: movw r1,#0x2801
  0007f88c: movw r2,#0x2601
  0007f890: blx 0x0006f250
  0007f894: ldr r4,[sp,#0x2c]
  0007f896: movs r1,#0x6
  0007f898: ldrh r0,[r4,#0x2]
  0007f89a: blx 0x0006ec2c
  0007f89e: mov r6,r0
  0007f8a0: ldr r0,[0x0007fb8c]
  0007f8a2: ldrh r5,[r4,#0x0]
  0007f8a4: movw r8,#0x1401
  0007f8a8: add r0,pc
  0007f8aa: ldr r1,[r4,#0xc]
  0007f8ac: mov.w r11,#0x0
  0007f8b0: movw r9,#0x1908
  0007f8b4: ldr r4,[r0,#0x0]
  0007f8b6: movw r0,#0x8517
  0007f8ba: strd r6,r11,[sp,#0x0]
  0007f8be: movw r2,#0x1908
  0007f8c2: strd r9,r8,[sp,#0x8]
  0007f8c6: mov r3,r5
  0007f8c8: str r1,[sp,#0x10]
  0007f8ca: movs r1,#0x0
  0007f8cc: blx r4
  0007f8ce: mul r10,r6,r5
  0007f8d2: ldr r0,[sp,#0x2c]
  0007f8d4: movs r1,#0x0
  0007f8d6: movw r2,#0x1908
  0007f8da: mov r3,r5
  0007f8dc: ldr r0,[r0,#0xc]
  0007f8de: strd r6,r11,[sp,#0x0]
  0007f8e2: strd r9,r8,[sp,#0x8]
  0007f8e6: add.w r0,r0,r10, lsl #0x2
  0007f8ea: str r0,[sp,#0x10]
  0007f8ec: movw r0,#0x8516
  0007f8f0: blx r4
  0007f8f2: ldr r0,[sp,#0x2c]
  0007f8f4: movs r1,#0x0
  0007f8f6: movw r2,#0x1908
  0007f8fa: mov r3,r5
  0007f8fc: ldr r0,[r0,#0xc]
  0007f8fe: strd r6,r11,[sp,#0x0]
  0007f902: add.w r0,r0,r10, lsl #0x3
  0007f906: strd r9,r8,[sp,#0x8]
  0007f90a: str r0,[sp,#0x10]
  0007f90c: movw r0,#0x8519
  0007f910: blx r4
  0007f912: ldr r0,[sp,#0x2c]
  0007f914: add.w r1,r10,r10, lsl #0x1
  0007f918: movw r2,#0x1908
  0007f91c: mov r3,r5
  0007f91e: ldr r0,[r0,#0xc]
  0007f920: strd r6,r11,[sp,#0x0]
  0007f924: add.w r0,r0,r1, lsl #0x2
  0007f928: strd r9,r8,[sp,#0x8]
  0007f92c: str r0,[sp,#0x10]
  0007f92e: movw r0,#0x8515
  0007f932: movs r1,#0x0
  0007f934: blx r4
  0007f936: ldr r0,[sp,#0x2c]
  0007f938: movs r1,#0x0
  0007f93a: movw r2,#0x1908
  0007f93e: mov r3,r5
  0007f940: ldr r0,[r0,#0xc]
  0007f942: strd r6,r11,[sp,#0x0]
  0007f946: add.w r0,r0,r10, lsl #0x4
  0007f94a: strd r9,r8,[sp,#0x8]
  0007f94e: str r0,[sp,#0x10]
  0007f950: movw r0,#0x851a
  0007f954: blx r4
  0007f956: ldr r1,[sp,#0x2c]
  0007f958: add.w r0,r10,r10, lsl #0x2
  0007f95c: movw r2,#0x1908
  0007f960: mov r3,r5
  0007f962: ldr r1,[r1,#0xc]
  0007f964: strd r6,r11,[sp,#0x0]
  0007f968: add.w r0,r1,r0, lsl #0x2
  0007f96c: strd r9,r8,[sp,#0x8]
  0007f970: str r0,[sp,#0x10]
  0007f972: movw r0,#0x8518
  0007f976: movs r1,#0x0
  0007f978: blx r4
  0007f97a: b 0x0007fe4c
  0007f97c: ldr.w r1,[r9,#0x0]
  0007f980: movw r0,#0xde1
  0007f984: blx 0x0006f244
  0007f988: movw r0,#0xcf5
  0007f98c: movs r1,#0x1
  0007f98e: blx 0x0006f25c
  0007f992: ldr r0,[0x0007fb90]
  0007f994: add r0,pc
  0007f996: ldr r0,[r0,#0x0]
  0007f998: ldrb r0,[r0,#0x0]
  0007f99a: cbnz r0,0x0007f9ac
  0007f99c: mov.w r0,#0x2300
  0007f9a0: mov.w r1,#0x2200
  0007f9a4: mov.w r2,#0x2100
  0007f9a8: blx 0x0006f268
  0007f9ac: ldr r0,[0x0007fb94]
  0007f9ae: add r0,pc
  0007f9b0: ldr r4,[r0,#0x0]
  0007f9b2: ldrb r0,[r4,#0x0]
  0007f9b4: cbz r0,0x0007f9d4
  0007f9b6: movw r0,#0xde1
  0007f9ba: movw r1,#0x2802
  0007f9be: movw r2,#0x812f
  0007f9c2: blx 0x0006f250
  0007f9c6: movw r0,#0xde1
  0007f9ca: movw r1,#0x2803
  0007f9ce: movw r2,#0x812f
  0007f9d2: b 0x0007f9f0
  0007f9d4: movw r0,#0xde1
  0007f9d8: movw r1,#0x2802
  0007f9dc: movw r2,#0x2901
  0007f9e0: blx 0x0006f250
  0007f9e4: movw r0,#0xde1
  0007f9e8: movw r1,#0x2803
  0007f9ec: movw r2,#0x2901
  0007f9f0: blx 0x0006f250
  0007f9f4: ldr r0,[sp,#0x2c]
  0007f9f6: ldrb r0,[r0,#0x8]
  0007f9f8: cbz r0,0x0007fa2c
  0007f9fa: ldr r0,[0x0007fb98]
  0007f9fc: add r0,pc
  0007f9fe: ldr r0,[r0,#0x0]
  0007fa00: vldr.32 s0,[r0]
  0007fa04: vcmpe.f32 s0,#0
  0007fa08: vmrs apsr,fpscr
  0007fa0c: ble 0x0007fa4c
  0007fa0e: vmov r2,s0
  0007fa12: movw r0,#0xde1
  0007fa16: movw r1,#0x84fe
  0007fa1a: blx 0x0006f274
  0007fa1e: movw r0,#0xde1
  0007fa22: movw r1,#0x2801
  0007fa26: movw r2,#0x2703
  0007fa2a: b 0x0007fa86
  0007fa2c: ldrb.w r0,[r10,#0x20]
  0007fa30: cbz r0,0x0007fa6a
  0007fa32: movw r0,#0xde1
  0007fa36: mov.w r1,#0x2800
  0007fa3a: movw r2,#0x2601
  0007fa3e: blx 0x0006f250
  0007fa42: movw r0,#0xde1
  0007fa46: movw r1,#0x2801
  0007fa4a: b 0x0007fa64
  0007fa4c: movw r0,#0xde1
  0007fa50: movw r1,#0x2801
  0007fa54: movw r2,#0x2703
  0007fa58: blx 0x0006f250
  0007fa5c: movw r0,#0xde1
  0007fa60: mov.w r1,#0x2800
  0007fa64: movw r2,#0x2601
  0007fa68: b 0x0007fa86
  0007fa6a: movw r0,#0xde1
  0007fa6e: mov.w r1,#0x2800
  0007fa72: mov.w r2,#0x2600
  0007fa76: blx 0x0006f250
  0007fa7a: movw r0,#0xde1
  0007fa7e: movw r1,#0x2801
  0007fa82: mov.w r2,#0x2600
  0007fa86: blx 0x0006f250
  0007fa8a: ldrb r0,[r4,#0x0]
  0007fa8c: cbz r0,0x0007faae
  0007fa8e: movw r0,#0xde1
  0007fa92: mov.w r1,#0x2800
  0007fa96: movw r2,#0x2601
  0007fa9a: blx 0x0006f250
  0007fa9e: movw r0,#0xde1
  0007faa2: movw r1,#0x2801
  0007faa6: movw r2,#0x2601
  0007faaa: blx 0x0006f250
  0007faae: ldr r0,[sp,#0x2c]
  0007fab0: ldr r1,[r0,#0x4]
  0007fab2: subs r2,r1,#0x1
  0007fab4: cmp r2,#0xa
  0007fab6: bhi.w 0x0007fe4c
  0007faba: tbh [pc,r2]
  0007fad4: movw r8,#0x83f0
  0007fad8: ldrh r5,[r0,#0x2]
  0007fada: cmp r1,#0x9
  0007fadc: it eq
  0007fade: movw.eq r8,#0x83f2
  0007fae2: ldrh r6,[r0,#0x0]
  0007fae4: cmp r1,#0xa
  0007fae6: ldrb r2,[r0,#0x8]
  0007fae8: it eq
  0007faea: movw.eq r8,#0x83f3
  0007faee: cbz r2,0x0007fb6a
  0007faf0: mov.w r9,#0x0
  0007faf4: mov.w r10,#0x0
  0007faf8: mov.w r11,#0x0
  0007fafc: b 0x0007fb62
  0007fafe: mul r4,r5,r6
  0007fb02: ldr r1,[r0,#0x4]
  0007fb04: sub.w r2,r1,#0x9
  0007fb08: cmp r2,#0x1
  0007fb0a: bhi 0x0007fb14
  0007fb0c: cmp r4,#0x10
  0007fb0e: it le
  0007fb10: mov.le r4,#0x10
  0007fb12: b 0x0007fb24
  0007fb14: cmp r4,#0xf
  0007fb16: bhi 0x0007fb22
  0007fb18: cmp r1,#0x8
  0007fb1a: it ne
  0007fb1c: mov.ne r1,#0x10
  0007fb1e: mov r4,r1
  0007fb20: b 0x0007fb24
  0007fb22: lsrs r4,r4,#0x1
  0007fb24: ldr r0,[r0,#0xc]
  0007fb26: mov r1,r11
  0007fb28: strd r5,r9,[sp,#0x0]
  0007fb2c: mov r2,r8
  0007fb2e: add r0,r10
  0007fb30: mov r3,r6
  0007fb32: strd r4,r0,[sp,#0x8]
  0007fb36: movw r0,#0xde1
  0007fb3a: blx 0x0006f280
  0007fb3e: lsrs r0,r5,#0x1
  0007fb40: cmp r0,#0x1
  0007fb42: mov.w r1,#0x1
  0007fb46: mov.w r2,#0x1
  0007fb4a: it hi
  0007fb4c: lsr.hi r1,r5,#0x1
  0007fb4e: lsrs r0,r6,#0x1
  0007fb50: cmp r0,#0x1
  0007fb52: it hi
  0007fb54: lsr.hi r2,r6,#0x1
  0007fb56: add r10,r4
  0007fb58: ldr r0,[sp,#0x2c]
  0007fb5a: add.w r11,r11,#0x1
  0007fb5e: mov r6,r2
  0007fb60: mov r5,r1
  0007fb62: ldr r1,[r0,#0x10]
  0007fb64: cmp r10,r1
  0007fb66: bcc 0x0007fafe
  0007fb68: b 0x0007fe4c
  0007fb6a: ldrd r1,r0,[r0,#0xc]
  0007fb6e: movs r2,#0x0
  0007fb70: strd r5,r2,[sp,#0x0]
  0007fb74: mov r2,r8
  0007fb76: mov r3,r6
  0007fb78: strd r0,r1,[sp,#0x8]
  0007fb7c: movw r0,#0xde1
  0007fb80: movs r1,#0x0
  0007fb82: b 0x0007fe48
  0007fb9c: ldrh r3,[r0,#0x0]
  0007fb9e: movw r2,#0x1401
  0007fba2: ldrh r1,[r0,#0x2]
  0007fba4: movw r6,#0x1909
  0007fba8: ldr r0,[r0,#0xc]
  0007fbaa: movs r5,#0x0
  0007fbac: stm sp,{r1,r5,r6}
  0007fbb0: movs r1,#0x0
  0007fbb2: strd r2,r0,[sp,#0xc]
  0007fbb6: movw r0,#0xde1
  0007fbba: movw r2,#0x1909
  0007fbbe: b 0x0007fc06
  0007fbc0: ldrh r3,[r0,#0x0]
  0007fbc2: movw r2,#0x1401
  0007fbc6: ldrh r1,[r0,#0x2]
  0007fbc8: movw r6,#0x1907
  0007fbcc: ldr r0,[r0,#0xc]
  0007fbce: movs r5,#0x0
  0007fbd0: stm sp,{r1,r5,r6}
  0007fbd4: movs r1,#0x0
  0007fbd6: strd r2,r0,[sp,#0xc]
  0007fbda: movw r0,#0xde1
  0007fbde: movw r2,#0x1907
  0007fbe2: b 0x0007fc06
  0007fbe4: ldrh r3,[r0,#0x0]
  0007fbe6: movw r2,#0x1401
  0007fbea: ldrh r1,[r0,#0x2]
  0007fbec: movw r6,#0x1908
  0007fbf0: ldr r0,[r0,#0xc]
  0007fbf2: movs r5,#0x0
  0007fbf4: stm sp,{r1,r5,r6}
  0007fbf8: movs r1,#0x0
  0007fbfa: strd r2,r0,[sp,#0xc]
  0007fbfe: movw r0,#0xde1
  0007fc02: movw r2,#0x1908
  0007fc06: blx 0x0006f28c
  0007fc0a: ldr r0,[sp,#0x2c]
  0007fc0c: ldrb r0,[r0,#0x8]
  0007fc0e: cmp r0,#0x0
  0007fc10: beq.w 0x0007fe4c
  0007fc14: movw r0,#0xde1
  0007fc18: blx 0x0006f298
  0007fc1c: b 0x0007fe4c
  0007fc1e: ldrb r1,[r0,#0x8]
  0007fc20: ldrh r6,[r0,#0x2]
  0007fc22: ldrh r5,[r0,#0x0]
  0007fc24: cmp r1,#0x0
  0007fc26: beq.w 0x0007fdf2
  0007fc2a: mvn r9,#0x7
  0007fc2e: mov.w r10,#0x0
  0007fc32: mov.w r11,#0x0
  0007fc36: b 0x0007fc94
  0007fc38: lsrs r4,r5,#0x3
  0007fc3a: cmp r5,#0x10
  0007fc3c: and.w r8,r9,r6, lsl #0x1
  0007fc40: ldr r0,[r0,#0xc]
  0007fc42: it cc
  0007fc44: mov.cc r4,#0x2
  0007fc46: cmp r6,#0x8
  0007fc48: it cc
  0007fc4a: mov.cc.w r8,#0x10
  0007fc4e: add r0,r11
  0007fc50: mul r1,r8,r4
  0007fc54: movs r2,#0x0
  0007fc56: str r6,[sp,#0x0]
  0007fc58: mov r3,r5
  0007fc5a: strd r2,r1,[sp,#0x4]
  0007fc5e: mov r1,r10
  0007fc60: str r0,[sp,#0xc]
  0007fc62: movw r0,#0xde1
  0007fc66: movw r2,#0x8c03
  0007fc6a: blx 0x0006f280
  0007fc6e: lsrs r0,r6,#0x1
  0007fc70: cmp r0,#0x1
  0007fc72: mov.w r1,#0x1
  0007fc76: mov.w r2,#0x1
  0007fc7a: it hi
  0007fc7c: lsr.hi r1,r6,#0x1
  0007fc7e: lsrs r0,r5,#0x1
  0007fc80: cmp r0,#0x1
  0007fc82: it hi
  0007fc84: lsr.hi r2,r5,#0x1
  0007fc86: mla r11,r8,r4,r11
  0007fc8a: ldr r0,[sp,#0x2c]
  0007fc8c: add.w r10,r10,#0x1
  0007fc90: mov r6,r1
  0007fc92: mov r5,r2
  0007fc94: ldr r1,[r0,#0x10]
  0007fc96: cmp r11,r1
  0007fc98: bcc 0x0007fc38
  0007fc9a: b 0x0007fe4c
  0007fc9c: ldrb r1,[r0,#0x8]
  0007fc9e: ldrh r6,[r0,#0x2]
  0007fca0: ldrh r5,[r0,#0x0]
  0007fca2: cmp r1,#0x0
  0007fca4: beq.w 0x0007fe02
  0007fca8: mvn r9,#0x7
  0007fcac: mov.w r11,#0x0
  0007fcb0: mov.w r10,#0x0
  0007fcb4: b 0x0007fd12
  0007fcb6: lsrs r4,r6,#0x2
  0007fcb8: cmp r6,#0x8
  0007fcba: and.w r8,r9,r5, lsl #0x1
  0007fcbe: ldr r0,[r0,#0xc]
  0007fcc0: it cc
  0007fcc2: mov.cc r4,#0x2
  0007fcc4: cmp r5,#0x8
  0007fcc6: it cc
  0007fcc8: mov.cc.w r8,#0x10
  0007fccc: add r0,r11
  0007fcce: mul r1,r8,r4
  0007fcd2: movs r2,#0x0
  0007fcd4: str r6,[sp,#0x0]
  0007fcd6: mov r3,r5
  0007fcd8: strd r2,r1,[sp,#0x4]
  0007fcdc: mov r1,r10
  0007fcde: str r0,[sp,#0xc]
  0007fce0: movw r0,#0xde1
  0007fce4: movw r2,#0x8c02
  0007fce8: blx 0x0006f280
  0007fcec: lsrs r0,r6,#0x1
  0007fcee: cmp r0,#0x1
  0007fcf0: mov.w r1,#0x1
  0007fcf4: mov.w r2,#0x1
  0007fcf8: it hi
  0007fcfa: lsr.hi r1,r6,#0x1
  0007fcfc: lsrs r0,r5,#0x1
  0007fcfe: cmp r0,#0x1
  0007fd00: it hi
  0007fd02: lsr.hi r2,r5,#0x1
  0007fd04: mla r11,r8,r4,r11
  0007fd08: ldr r0,[sp,#0x2c]
  0007fd0a: add.w r10,r10,#0x1
  0007fd0e: mov r5,r2
  0007fd10: mov r6,r1
  0007fd12: ldr r1,[r0,#0x10]
  0007fd14: cmp r11,r1
  0007fd16: bcc 0x0007fcb6
  0007fd18: b 0x0007fe4c
  0007fd1a: ldrb r1,[r0,#0x8]
  0007fd1c: ldrh r6,[r0,#0x2]
  0007fd1e: ldrh r4,[r0,#0x0]
  0007fd20: cmp r1,#0x0
  0007fd22: beq 0x0007fe1e
  0007fd24: mov.w r8,#0x0
  0007fd28: mov.w r10,#0x0
  0007fd2c: mov.w r9,#0x0
  0007fd30: b 0x0007fd7c
  0007fd32: mul r5,r6,r4
  0007fd36: ldr r0,[r0,#0xc]
  0007fd38: mov r1,r9
  0007fd3a: add r0,r10
  0007fd3c: movw r2,#0x8c93
  0007fd40: mov r3,r4
  0007fd42: cmp r5,#0x10
  0007fd44: it ls
  0007fd46: mov.ls r5,#0x10
  0007fd48: strd r6,r8,[sp,#0x0]
  0007fd4c: strd r5,r0,[sp,#0x8]
  0007fd50: movw r0,#0xde1
  0007fd54: blx 0x0006f280
  0007fd58: lsrs r0,r6,#0x1
  0007fd5a: cmp r0,#0x1
  0007fd5c: mov.w r1,#0x1
  0007fd60: mov.w r2,#0x1
  0007fd64: it hi
  0007fd66: lsr.hi r1,r6,#0x1
  0007fd68: lsrs r0,r4,#0x1
  0007fd6a: cmp r0,#0x1
  0007fd6c: it hi
  0007fd6e: lsr.hi r2,r4,#0x1
  0007fd70: add r10,r5
  0007fd72: ldr r0,[sp,#0x2c]
  0007fd74: add.w r9,r9,#0x1
  0007fd78: mov r4,r2
  0007fd7a: mov r6,r1
  0007fd7c: ldr r1,[r0,#0x10]
  0007fd7e: cmp r10,r1
  0007fd80: bcc 0x0007fd32
  0007fd82: b 0x0007fe4c
  0007fd84: ldrb r1,[r0,#0x8]
  0007fd86: ldrh r6,[r0,#0x2]
  0007fd88: ldrh r4,[r0,#0x0]
  0007fd8a: cmp r1,#0x0
  0007fd8c: beq 0x0007fe2e
  0007fd8e: mov.w r8,#0x0
  0007fd92: mov.w r10,#0x0
  0007fd96: mov.w r9,#0x0
  0007fd9a: b 0x0007fdea
  0007fd9c: mul r1,r6,r4
  0007fda0: ldr r0,[r0,#0xc]
  0007fda2: cmp r4,#0x8
  0007fda4: add r0,r10
  0007fda6: movw r2,#0x8d64
  0007fdaa: mov r3,r4
  0007fdac: lsr.w r5,r1,#0x1
  0007fdb0: mov r1,r9
  0007fdb2: it cc
  0007fdb4: mov.cc r5,#0x8
  0007fdb6: strd r6,r8,[sp,#0x0]
  0007fdba: strd r5,r0,[sp,#0x8]
  0007fdbe: movw r0,#0xde1
  0007fdc2: blx 0x0006f280
  0007fdc6: lsrs r0,r6,#0x1
  0007fdc8: cmp r0,#0x1
  0007fdca: mov.w r1,#0x1
  0007fdce: mov.w r2,#0x1
  0007fdd2: it hi
  0007fdd4: lsr.hi r1,r6,#0x1
  0007fdd6: lsrs r0,r4,#0x1
  0007fdd8: cmp r0,#0x1
  0007fdda: it hi
  0007fddc: lsr.hi r2,r4,#0x1
  0007fdde: add r10,r5
  0007fde0: ldr r0,[sp,#0x2c]
  0007fde2: add.w r9,r9,#0x1
  0007fde6: mov r4,r2
  0007fde8: mov r6,r1
  0007fdea: ldr r1,[r0,#0x10]
  0007fdec: cmp r10,r1
  0007fdee: bcc 0x0007fd9c
  0007fdf0: b 0x0007fe4c
  0007fdf2: ldrd r1,r0,[r0,#0xc]
  0007fdf6: movs r2,#0x0
  0007fdf8: strd r6,r2,[sp,#0x0]
  0007fdfc: movw r2,#0x8c03
  0007fe00: b 0x0007fe10
  0007fe02: ldrd r1,r0,[r0,#0xc]
  0007fe06: movs r2,#0x0
  0007fe08: strd r6,r2,[sp,#0x0]
  0007fe0c: movw r2,#0x8c02
  0007fe10: strd r0,r1,[sp,#0x8]
  0007fe14: movw r0,#0xde1
  0007fe18: movs r1,#0x0
  0007fe1a: mov r3,r5
  0007fe1c: b 0x0007fe48
  0007fe1e: ldrd r1,r0,[r0,#0xc]
  0007fe22: movs r2,#0x0
  0007fe24: strd r6,r2,[sp,#0x0]
  0007fe28: movw r2,#0x8c93
  0007fe2c: b 0x0007fe3c
  0007fe2e: ldrd r1,r0,[r0,#0xc]
  0007fe32: movs r2,#0x0
  0007fe34: strd r6,r2,[sp,#0x0]
  0007fe38: movw r2,#0x8d64
  0007fe3c: strd r0,r1,[sp,#0x8]
  0007fe40: movw r0,#0xde1
  0007fe44: movs r1,#0x0
  0007fe46: mov r3,r4
  0007fe48: blx 0x0006f280
  0007fe4c: ldr r0,[r7,#0x14]
  0007fe4e: cbz r0,0x0007fe94
  0007fe50: ldr r4,[r7,#0x10]
  0007fe52: ldr r6,[sp,#0x1c]
  0007fe54: ldr.w r9,[r7,#0x8]
  0007fe58: cmp r4,#0x0
  0007fe5a: ldr.w r10,[sp,#0x18]
  0007fe5e: beq 0x0007fed8
  0007fe60: ldr r0,[sp,#0x2c]
  0007fe62: movs r1,#0x1
  0007fe64: ldr r0,[r0,#0x4]
  0007fe66: strb r1,[r4,#0x15]
  0007fe68: subs r0,#0x6
  0007fe6a: clz r0,r0
  0007fe6e: lsrs r0,r0,#0x5
  0007fe70: strb r0,[r4,#0x14]
  0007fe72: blx 0x0006ed70
  0007fe76: ldr.w r8,[sp,#0x14]
  0007fe7a: cmp r0,#0x0
  0007fe7c: beq 0x0007ff34
  0007fe7e: mov.w r0,#0xffffffff
  0007fe82: mov r1,r9
  0007fe84: str r0,[r4,#0x0]
  0007fe86: movs r0,#0x0
  0007fe88: str r0,[r4,#0x18]
  0007fe8a: movs r0,#0x1
  0007fe8c: blx 0x0006f2a4
  0007fe90: ldr r0,[r4,#0x18]
  0007fe92: b 0x0007ff5a
  0007fe94: blx 0x0006ed70
  0007fe98: ldr r5,[sp,#0x1c]
  0007fe9a: cmp r0,#0x0
  0007fe9c: ldr r6,[r7,#0x8]
  0007fe9e: ldr r2,[sp,#0x18]
  0007fea0: str r0,[r5,#0x10]
  0007fea2: beq 0x0007ff24
  0007fea4: add r4,sp,#0x20
  0007fea6: mov r1,r2
  0007fea8: movs r2,#0x0
  0007feaa: mov r0,r4
  0007feac: blx 0x0006ee18
  0007feb0: add.w r0,r5,#0x14
  0007feb4: mov r1,r4
  0007feb6: blx 0x0006f2b0
  0007feba: ldr.w r8,[sp,#0x14]
  0007febe: add r0,sp,#0x20
  0007fec0: blx 0x0006ee30
  0007fec4: movs r0,#0x1
  0007fec6: mov r1,r6
  0007fec8: blx 0x0006f2a4
  0007fecc: add r0,sp,#0x2c
  0007fece: blx 0x0006f214
  0007fed2: mvn r0,#0x3
  0007fed6: b 0x0007ffb0
  0007fed8: movs r0,#0x1c
  0007feda: blx 0x0006eb24
  0007fede: mov r5,r0
  0007fee0: adds r0,#0x4
  0007fee2: blx 0x0006efbc
  0007fee6: ldr r1,[sp,#0x2c]
  0007fee8: movs r2,#0x1
  0007feea: vldr.32 s0,[r7,#0xc]
  0007feee: ldr r1,[r1,#0x4]
  0007fef0: vstr.32 s0,[r5,#0x10]
  0007fef4: subs r1,#0x6
  0007fef6: strb r2,[r5,#0x15]
  0007fef8: clz r1,r1
  0007fefc: lsrs r1,r1,#0x5
  0007fefe: strb r1,[r5,#0x14]
  0007ff00: mov r1,r10
  0007ff02: blx 0x0006f2bc
  0007ff06: blx 0x0006ed70
  0007ff0a: ldr.w r8,[sp,#0x14]
  0007ff0e: cbz r0,0x0007ff62
  0007ff10: movs r0,#0x1
  0007ff12: mov r1,r9
  0007ff14: blx 0x0006f2a4
  0007ff18: movs r0,#0x0
  0007ff1a: mov.w r1,#0xffffffff
  0007ff1e: str r0,[r5,#0x18]
  0007ff20: str r1,[r5,#0x0]
  0007ff22: b 0x0007ff88
  0007ff24: ldr r1,[r6,#0x0]
  0007ff26: movw r0,#0x1702
  0007ff2a: blx 0x0006f2c8
  0007ff2e: ldr.w r8,[sp,#0x14]
  0007ff32: b 0x0007ffa8
  0007ff34: ldr.w r1,[r9,#0x0]
  0007ff38: movw r0,#0x1702
  0007ff3c: mov r2,r10
  0007ff3e: blx 0x0006f2c8
  0007ff42: ldr r0,[0x0007ffe4]
  0007ff44: ldr.w r1,[r9,#0x0]
  0007ff48: str r1,[r4,#0x0]
  0007ff4a: add r0,pc
  0007ff4c: ldr r1,[sp,#0x2c]
  0007ff4e: ldr r2,[r0,#0x0]
  0007ff50: ldr r0,[r1,#0x10]
  0007ff52: str r0,[r4,#0x18]
  0007ff54: ldr r1,[r2,#0x0]
  0007ff56: adds r1,#0x1
  0007ff58: str r1,[r2,#0x0]
  0007ff5a: ldr r1,[r6,#0x70]
  0007ff5c: add r0,r1
  0007ff5e: str r0,[r6,#0x70]
  0007ff60: b 0x0007ffa8
  0007ff62: ldr.w r1,[r9,#0x0]
  0007ff66: movw r0,#0x1702
  0007ff6a: mov r2,r10
  0007ff6c: blx 0x0006f2c8
  0007ff70: ldr r0,[0x0007ffe0]
  0007ff72: ldr.w r1,[r9,#0x0]
  0007ff76: add r0,pc
  0007ff78: str r1,[r5,#0x0]
  0007ff7a: ldr r1,[sp,#0x2c]
  0007ff7c: ldr r2,[r0,#0x0]
  0007ff7e: ldr r0,[r1,#0x10]
  0007ff80: ldr r1,[r2,#0x0]
  0007ff82: str r0,[r5,#0x18]
  0007ff84: adds r1,#0x1
  0007ff86: str r1,[r2,#0x0]
  0007ff88: ldr r2,[r6,#0x70]
  0007ff8a: ldr r1,[r6,#0x30]
  0007ff8c: add r0,r2
  0007ff8e: str r0,[r6,#0x70]
  0007ff90: ldr r0,[r1,#0x0]
  0007ff92: add.w r1,r0,#0x10
  0007ff96: mov r0,r5
  0007ff98: blx 0x0006f2d4
  0007ff9c: ldr r0,[r6,#0x30]
  0007ff9e: ldr r0,[r0,#0x0]
  0007ffa0: ldr r0,[r0,#0x10]
  0007ffa2: subs r0,#0x1
  0007ffa4: str.w r0,[r9,#0x0]
  0007ffa8: add r0,sp,#0x2c
  0007ffaa: blx 0x0006f214
  0007ffae: movs r0,#0x1
  0007ffb0: ldr r1,[sp,#0x30]
  0007ffb2: ldr.w r2,[r8,#0x0]
  0007ffb6: subs r1,r2,r1
  0007ffb8: ittt eq
  0007ffba: add.eq sp,#0x34
  0007ffbc: pop.eq.w {r8,r9,r10,r11}
  0007ffc0: pop.eq {r4,r5,r6,r7,pc}
  0007ffc2: blx 0x0006e824
```
