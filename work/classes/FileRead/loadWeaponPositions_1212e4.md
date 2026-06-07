# FileRead::loadWeaponPositions
elf 0x1212e4 body 448
Sig: undefined __thiscall loadWeaponPositions(FileRead * this, int param_1)

## decompile
```c

/* FileRead::loadWeaponPositions(int) */

void __thiscall FileRead::loadWeaponPositions(FileRead *this,int param_1)

{
  String *pSVar1;
  int iVar2;
  Array<Array<AbyssEngine::AEMath::Vector*>*> *this_00;
  code *pcVar3;
  Array<AbyssEngine::AEMath::Vector*> *this_01;
  Vector *pVVar4;
  int iVar5;
  uint *extraout_r2;
  int iVar6;
  int *piVar7;
  int iVar8;
  code *pcVar9;
  uint in_fpscr;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  short local_40;
  short sStack_3e;
  short local_3c;
  short sStack_3a;
  short local_38;
  short sStack_36;
  uint local_34 [3];
  int local_28;
  
  piVar7 = *(int **)(DAT_001314c4 + 0x1312f8);
  local_28 = *piVar7;
  pSVar1 = (String *)
           AbyssEngine::String::String((String *)local_34,(char *)(DAT_001314c8 + 0x131304),false);
  iVar2 = AEFile::FileExist(pSVar1);
  AbyssEngine::String::~String((String *)local_34);
  if (iVar2 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_001314cc + 0x131320),(char *)local_34,extraout_r2);
    this_00 = operator_new(0xc);
    Array<Array<AbyssEngine::AEMath::Vector*>*>::Array(this_00);
    ArraySetLength<Array<AbyssEngine::AEMath::Vector*>*>(4,(Array *)this_00);
    iVar2 = *(int *)this_00;
    for (iVar6 = 0; iVar2 != iVar6; iVar6 = iVar6 + 1) {
      *(undefined4 *)(*(int *)(this_00 + 4) + iVar6 * 4) = 0;
    }
    sStack_36 = 0;
    local_38 = 0;
    local_40 = 0;
    local_4c = 0;
    pcVar9 = *(code **)(DAT_001314d0 + 0x131370);
    pcVar3 = *(code **)(DAT_001314d4 + 0x131378);
    do {
      AEFile::Read(&sStack_36,local_34[0]);
      AEFile::Read(&local_38,local_34[0]);
      iVar2 = (int)sStack_36;
      iVar6 = 0;
      while( true ) {
        if (local_38 <= iVar6) break;
        (*pcVar9)(&sStack_3a,local_34[0]);
        (*pcVar9)(&local_3c,local_34[0]);
        (*pcVar9)(&sStack_3e,local_34[0]);
        (*pcVar9)(&local_40,local_34[0]);
        if (sStack_3a == 3) {
          (*pcVar3)(&local_44,local_34[0]);
          (*pcVar3)(&uStack_48,local_34[0]);
          (*pcVar3)(&local_4c,local_34[0]);
        }
        if (iVar2 == param_1) {
          iVar8 = (int)sStack_3a;
          if (*(int *)(*(int *)(this_00 + 4) + iVar8 * 4) == 0) {
            this_01 = operator_new(0xc);
            Array<AbyssEngine::AEMath::Vector*>::Array(this_01);
            iVar8 = (int)sStack_3a;
            *(Array<AbyssEngine::AEMath::Vector*> **)(*(int *)(this_00 + 4) + iVar8 * 4) = this_01;
          }
          pVVar4 = operator_new(0xc);
          uVar10 = VectorSignedToFloat((int)local_40,(byte)(in_fpscr >> 0x16) & 3);
          uVar11 = VectorSignedToFloat((int)local_3c,(byte)(in_fpscr >> 0x16) & 3);
          uVar12 = VectorSignedToFloat(-(int)sStack_3e,(byte)(in_fpscr >> 0x16) & 3);
          iVar5 = *(int *)(this_00 + 4);
          *(undefined4 *)pVVar4 = uVar11;
          *(undefined4 *)(pVVar4 + 4) = uVar10;
          *(undefined4 *)(pVVar4 + 8) = uVar12;
          ArrayAdd<AbyssEngine::AEMath::Vector*>(pVVar4,*(Array **)(iVar5 + iVar8 * 4));
          if (sStack_3a == 3) {
            pVVar4 = operator_new(0xc);
            *(undefined4 *)pVVar4 = local_44;
            *(undefined4 *)(pVVar4 + 4) = local_4c;
            *(undefined4 *)(pVVar4 + 8) = uStack_48;
            ArrayAdd<AbyssEngine::AEMath::Vector*>(pVVar4,*(Array **)(*(int *)(this_00 + 4) + 0xc));
          }
        }
        iVar6 = iVar6 + 1;
      }
    } while (iVar2 != param_1);
    AEFile::Close(local_34[0]);
  }
  iVar2 = *piVar7 - local_28;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  001312e4: push {r4,r5,r6,r7,lr}
  001312e6: add r7,sp,#0xc
  001312e8: push {r8,r9,r10,r11}
  001312ec: sub sp,#0x3c
  001312ee: str r1,[sp,#0xc]
  001312f0: movs r2,#0x0
  001312f2: ldr r0,[0x001314c4]
  001312f4: add r0,pc
  001312f6: ldr r4,[r0,#0x0]
  001312f8: ldr r0,[r4,#0x0]
  001312fa: ldr r1,[0x001314c8]
  001312fc: str r0,[sp,#0x38]
  001312fe: add r0,sp,#0x2c
  00131300: add r1,pc
  00131302: blx 0x0006ee18
  00131306: blx 0x0006fff4
  0013130a: mov r5,r0
  0013130c: add r0,sp,#0x2c
  0013130e: blx 0x0006ee30
  00131312: cmp r5,#0x0
  00131314: beq.w 0x0013148a
  00131318: ldr r0,[0x001314cc]
  0013131a: add r1,sp,#0x2c
  0013131c: add r0,pc
  0013131e: blx 0x0006ecd4
  00131322: movs r0,#0xc
  00131324: blx 0x0006eb24
  00131328: mov r11,r0
  0013132a: blx 0x000763a8
  0013132e: movs r0,#0x4
  00131330: mov r1,r11
  00131332: blx 0x000763b4
  00131336: ldr.w r0,[r11,#0x0]
  0013133a: movs r1,#0x0
  0013133c: movs r2,#0x0
  0013133e: b 0x0013134a
  00131340: ldr.w r3,[r11,#0x4]
  00131344: str.w r1,[r3,r2,lsl #0x2]
  00131348: adds r2,#0x1
  0013134a: cmp r0,r2
  0013134c: bne 0x00131340
  0013134e: movs r0,#0x0
  00131350: str r4,[sp,#0x4]
  00131352: strh r0,[r7,#-0x2e]
  00131356: sub.w r6,r7,#0x32
  0013135a: strh.w r0,[sp,#0x28]
  0013135e: add r5,sp,#0x24
  00131360: strh.w r0,[sp,#0x20]
  00131364: sub.w r4,r7,#0x36
  00131368: str r0,[sp,#0x14]
  0013136a: ldr r0,[0x001314d0]
  0013136c: add r0,pc
  0013136e: ldr.w r10,[r0,#0x0]
  00131372: ldr r0,[0x001314d4]
  00131374: add r0,pc
  00131376: ldr r0,[r0,#0x0]
  00131378: str r0,[sp,#0x8]
  0013137a: ldr r1,[sp,#0x2c]
  0013137c: sub.w r0,r7,#0x2e
  00131380: blx 0x00074908
  00131384: ldr r1,[sp,#0x2c]
  00131386: add r0,sp,#0x28
  00131388: blx 0x00074908
  0013138c: ldrsh.w r0,[r7,#-0x2e]
  00131390: mov.w r8,#0x0
  00131394: str r0,[sp,#0x10]
  00131396: b 0x0013146e
  00131398: ldr r1,[sp,#0x2c]
  0013139a: mov r0,r6
  0013139c: blx r10
  0013139e: ldr r1,[sp,#0x2c]
  001313a0: mov r0,r5
  001313a2: blx r10
  001313a4: ldr r1,[sp,#0x2c]
  001313a6: mov r0,r4
  001313a8: blx r10
  001313aa: ldr r1,[sp,#0x2c]
  001313ac: add r0,sp,#0x20
  001313ae: blx r10
  001313b0: ldrh.w r0,[r7,#-0x32]
  001313b4: cmp r0,#0x3
  001313b6: bne 0x001313ce
  001313b8: ldr r1,[sp,#0x2c]
  001313ba: add r0,sp,#0x1c
  001313bc: ldr.w r9,[sp,#0x8]
  001313c0: blx r9
  001313c2: ldr r1,[sp,#0x2c]
  001313c4: add r0,sp,#0x18
  001313c6: blx r9
  001313c8: ldr r1,[sp,#0x2c]
  001313ca: add r0,sp,#0x14
  001313cc: blx r9
  001313ce: ldrd r0,r1,[sp,#0xc]
  001313d2: cmp r1,r0
  001313d4: bne 0x0013146a
  001313d6: ldrsh.w r4,[r7,#-0x32]
  001313da: ldr.w r0,[r11,#0x4]
  001313de: ldr.w r0,[r0,r4,lsl #0x2]
  001313e2: cbnz r0,0x001313fe
  001313e4: movs r0,#0xc
  001313e6: blx 0x0006eb24
  001313ea: mov r9,r0
  001313ec: blx 0x00074530
  001313f0: ldrsh.w r4,[r7,#-0x32]
  001313f4: add r5,sp,#0x24
  001313f6: ldr.w r0,[r11,#0x4]
  001313fa: str.w r9,[r0,r4,lsl #0x2]
  001313fe: movs r0,#0xc
  00131400: blx 0x0006eb24
  00131404: ldrsh.w r1,[r7,#-0x36]
  00131408: ldrsh.w r2,[sp,#0x20]
  0013140c: ldrsh.w r3,[sp,#0x24]
  00131410: rsbs r1,r1
  00131412: vmov s0,r2
  00131416: vmov s2,r3
  0013141a: vmov s4,r1
  0013141e: vcvt.f32.s32 s0,s0
  00131422: vcvt.f32.s32 s2,s2
  00131426: vcvt.f32.s32 s4,s4
  0013142a: ldr.w r1,[r11,#0x4]
  0013142e: vstr.32 s2,[r0]
  00131432: vstr.32 s0,[r0,#0x4]
  00131436: vstr.32 s4,[r0,#0x8]
  0013143a: ldr.w r1,[r1,r4,lsl #0x2]
  0013143e: blx 0x000763c0
  00131442: ldrh.w r0,[r7,#-0x32]
  00131446: sub.w r4,r7,#0x36
  0013144a: cmp r0,#0x3
  0013144c: bne 0x0013146a
  0013144e: movs r0,#0xc
  00131450: blx 0x0006eb24
  00131454: ldr r2,[sp,#0x1c]
  00131456: ldrd r3,r1,[sp,#0x14]
  0013145a: strd r2,r3,[r0,#0x0]
  0013145e: str r1,[r0,#0x8]
  00131460: ldr.w r1,[r11,#0x4]
  00131464: ldr r1,[r1,#0xc]
  00131466: blx 0x000763c0
  0013146a: add.w r8,r8,#0x1
  0013146e: ldrsh.w r0,[sp,#0x28]
  00131472: cmp r8,r0
  00131474: blt 0x00131398
  00131476: ldrd r0,r1,[sp,#0xc]
  0013147a: cmp r1,r0
  0013147c: bne.w 0x0013137a
  00131480: ldr r0,[sp,#0x2c]
  00131482: blx 0x0006ece0
  00131486: ldr r4,[sp,#0x4]
  00131488: b 0x0013148e
  0013148a: mov.w r11,#0x0
  0013148e: ldr r0,[sp,#0x38]
  00131490: ldr r1,[r4,#0x0]
  00131492: subs r0,r1,r0
  00131494: itttt eq
  00131496: mov.eq r0,r11
  00131498: add.eq sp,#0x3c
  0013149a: pop.eq.w {r8,r9,r10,r11}
  0013149e: pop.eq {r4,r5,r6,r7,pc}
  001314a0: blx 0x0006e824
```
