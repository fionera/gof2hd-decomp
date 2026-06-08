# AbyssEngine::ImageCreateFromFile
elf 0x6f4d4 body 672
Sig: undefined __stdcall ImageCreateFromFile(Engine * param_1, char * param_2, Image * * param_3)

## decompile
```c

/* AbyssEngine::ImageCreateFromFile(AbyssEngine::Engine*, char const*, AbyssEngine::Image**) */

void AbyssEngine::ImageCreateFromFile(Engine *param_1,char *param_2,Image **param_3)

{
  char *pcVar1;
  char *pcVar2;
  undefined4 uVar3;
  Image *pIVar4;
  int iVar5;
  uint uVar6;
  void *pvVar7;
  Image *extraout_r1;
  uint uVar8;
  Image extraout_r2;
  Image IVar9;
  uint *extraout_r2_00;
  int *piVar10;
  bool bVar11;
  uint local_34;
  char local_30 [4];
  undefined4 local_2c;
  ushort local_24;
  byte bStack_21;
  uint local_20;
  int local_1c;
  
  piVar10 = *(int **)(DAT_0007f7c4 + 0x7f4ec);
  local_1c = *piVar10;
  uVar3 = 0xfffffffc;
  if ((param_1 != (Engine *)0x0) && (param_2 != (char *)0x0)) {
    pIVar4 = operator_new(0x14);
    pIVar4[8] = (Image)0x0;
    *(undefined4 *)pIVar4 = 0;
    *(undefined4 *)(pIVar4 + 4) = 0;
    *(undefined4 *)(pIVar4 + 0xc) = 0;
    *param_3 = pIVar4;
    local_20 = 0;
    bStack_21 = 0;
    local_24 = 0;
    iVar5 = AEFile::OpenRead((AEFile *)param_2,(char *)&local_20,extraout_r2_00);
    if (iVar5 == 0) {
      if (*param_3 != (Image *)0x0) {
        operator_delete(*param_3);
      }
      *param_3 = (Image *)0x0;
    }
    else {
      local_2c = DAT_0007f7c0;
      builtin_strncpy(local_30,"****",4);
      iVar5 = AEFile::Read(8,local_30,local_20);
      if (iVar5 != 0) {
        uVar6 = 0;
LAB_0007f546:
        if (uVar6 < 8) goto code_r0x0007f54a;
        iVar5 = AEFile::Read(1,&bStack_21,local_20);
        if ((((iVar5 == 0) || (iVar5 = AEFile::Read(2,*param_3,local_20), iVar5 == 0)) ||
            (iVar5 = AEFile::Read(2,*param_3 + 2,local_20), iVar5 == 0)) ||
           (iVar5 = AEFile::Read(2,&local_24,local_20), iVar5 == 0)) goto LAB_0007f79c;
        AEFile::Skip((uint)local_24 << 3,local_20);
        bVar11 = (bStack_21 & 2) != 0;
        IVar9 = extraout_r2;
        pIVar4 = extraout_r1;
        if (bVar11) {
          pIVar4 = *param_3;
          IVar9 = (Image)0x1;
        }
        if (bVar11) {
          pIVar4[8] = IVar9;
        }
        switch(bStack_21) {
        case 1:
        case 3:
          goto switchD_0007f5ca_caseD_1;
        case 2:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 0xb:
        case 0xc:
        case 0xe:
        case 0x15:
        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
        case 0x1c:
        case 0x1d:
        case 0x1e:
        case 0x1f:
        case 0x25:
          break;
        case 0xd:
        case 0xf:
          iVar5 = AEFile::Read(4,&local_34,local_20);
          uVar6 = local_34;
          if (iVar5 != 0) {
            pvVar7 = operator_new__(local_34);
            *(void **)(*param_3 + 0xc) = pvVar7;
            iVar5 = AEFile::Read(uVar6,*(void **)(*param_3 + 0xc),local_20);
            if (iVar5 != 0) {
              pIVar4 = *param_3;
              uVar3 = 4;
              goto LAB_0007f75e;
            }
          }
          goto LAB_0007f79c;
        case 0x10:
        case 0x12:
          iVar5 = AEFile::Read(4,&local_34,local_20);
          uVar6 = local_34;
          if (iVar5 == 0) goto LAB_0007f79c;
          pvVar7 = operator_new__(local_34);
          *(void **)(*param_3 + 0xc) = pvVar7;
          iVar5 = AEFile::Read(uVar6,*(void **)(*param_3 + 0xc),local_20);
          if (iVar5 == 0) goto LAB_0007f79c;
          pIVar4 = *param_3;
          uVar3 = 5;
LAB_0007f75e:
          *(undefined4 *)(pIVar4 + 4) = uVar3;
          *(uint *)(pIVar4 + 0x10) = local_34;
          break;
        case 0x11:
        case 0x13:
          iVar5 = AEFile::Read(4,*param_3 + 0x10,local_20);
          if (iVar5 != 0) {
            pIVar4 = *param_3;
            uVar3 = 7;
            goto LAB_0007f77a;
          }
          goto LAB_0007f79c;
        case 0x14:
        case 0x16:
        case 0x17:
switchD_0007f5ca_caseD_14:
          iVar5 = AEFile::Read(4,&local_34,local_20);
          uVar6 = local_34;
          if (iVar5 == 0) goto LAB_0007f79c;
          pvVar7 = operator_new__(local_34);
          *(void **)(*param_3 + 0xc) = pvVar7;
          iVar5 = AEFile::Read(uVar6,*(void **)(*param_3 + 0xc),local_20);
          if (iVar5 == 0) goto LAB_0007f79c;
          pIVar4 = *param_3;
          *(undefined4 *)(pIVar4 + 4) = 0xb;
          *(uint *)(pIVar4 + 0x10) = local_34;
          if (bStack_21 == 0x17) {
            pIVar4[8] = (Image)0x0;
          }
          break;
        case 0x20:
        case 0x22:
          iVar5 = AEFile::Read(4,*param_3 + 0x10,local_20);
          if (iVar5 != 0) {
            pIVar4 = *param_3;
            uVar3 = 8;
            goto LAB_0007f77a;
          }
          goto LAB_0007f79c;
        case 0x21:
        case 0x23:
          iVar5 = AEFile::Read(4,*param_3 + 0x10,local_20);
          if (iVar5 != 0) {
            pIVar4 = *param_3;
            uVar3 = 9;
            goto LAB_0007f77a;
          }
          goto LAB_0007f79c;
        case 0x24:
        case 0x26:
          iVar5 = AEFile::Read(4,*param_3 + 0x10,local_20);
          if (iVar5 == 0) goto LAB_0007f79c;
          pIVar4 = *param_3;
          uVar3 = 10;
LAB_0007f77a:
          *(undefined4 *)(pIVar4 + 4) = uVar3;
          pvVar7 = operator_new__(*(uint *)(pIVar4 + 0x10));
          *(void **)(pIVar4 + 0xc) = pvVar7;
          iVar5 = AEFile::Read(*(uint *)(*param_3 + 0x10),*(void **)(*param_3 + 0xc),local_20);
          if (iVar5 == 0) goto LAB_0007f79c;
          break;
        default:
          if (bStack_21 != 0x81) {
            if (bStack_21 != 0x42) {
              if (bStack_21 != 0x40) break;
              (*param_3)[8] = (Image)0x0;
            }
            goto switchD_0007f5ca_caseD_14;
          }
switchD_0007f5ca_caseD_1:
          pIVar4 = *param_3;
          pvVar7 = operator_new__((uint)*(ushort *)pIVar4 * (uint)*(ushort *)(pIVar4 + 2) * 4);
          *(void **)(pIVar4 + 0xc) = pvVar7;
          pIVar4 = *param_3;
          iVar5 = AEFile::Read((uint)*(ushort *)pIVar4 * (uint)*(ushort *)(pIVar4 + 2) * 4,
                               *(void **)(pIVar4 + 0xc),local_20);
          if (iVar5 == 0) goto LAB_0007f79c;
          pIVar4 = *param_3;
          *(uint *)(pIVar4 + 0x10) = (uint)*(ushort *)pIVar4 * (uint)*(ushort *)(pIVar4 + 2) * 4;
          uVar8 = (uint)(char)bStack_21;
          uVar6 = uVar8;
          if (-1 < (int)uVar8) {
            uVar6 = 3;
          }
          if (0x7fffffff < uVar8) {
            uVar6 = 6;
          }
          *(uint *)(pIVar4 + 4) = uVar6;
        }
        AEFile::Close(local_20);
        uVar3 = 1;
        goto LAB_0007f7ac;
      }
LAB_0007f79c:
      ImageRelease(param_3);
      AEFile::Close(local_20);
    }
    uVar3 = 0xffffffff;
  }
LAB_0007f7ac:
  if (*piVar10 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
code_r0x0007f54a:
  pcVar1 = local_30 + uVar6;
  pcVar2 = (char *)(DAT_0007f7c8 + 0x7f548 + uVar6);
  uVar6 = uVar6 + 1;
  if (*pcVar2 != *pcVar1) goto LAB_0007f79c;
  goto LAB_0007f546;
}

```

## target disasm
```
  0007f4d4: push {r4,r5,r6,r7,lr}
  0007f4d6: add r7,sp,#0xc
  0007f4d8: push.w r11
  0007f4dc: sub sp,#0x20
  0007f4de: mov r5,r1
  0007f4e0: mov r1,r0
  0007f4e2: ldr r0,[0x0007f7c4]
  0007f4e4: mov r4,r2
  0007f4e6: cmp r1,#0x0
  0007f4e8: add r0,pc
  0007f4ea: ldr r6,[r0,#0x0]
  0007f4ec: ldr r0,[r6,#0x0]
  0007f4ee: str r0,[sp,#0x1c]
  0007f4f0: mvn r0,#0x3
  0007f4f4: beq.w 0x0007f7ac
  0007f4f8: cmp r5,#0x0
  0007f4fa: beq.w 0x0007f7ac
  0007f4fe: movs r0,#0x14
  0007f500: blx 0x0006eb24
  0007f504: movs r1,#0x0
  0007f506: strb r1,[r0,#0x8]
  0007f508: strd r1,r1,[r0,#0x0]
  0007f50c: str r1,[r0,#0xc]
  0007f50e: str r0,[r4,#0x0]
  0007f510: mov r0,r5
  0007f512: str r1,[sp,#0x18]
  0007f514: strb.w r1,[r7,#-0x19]
  0007f518: strh.w r1,[sp,#0x14]
  0007f51c: add r1,sp,#0x18
  0007f51e: blx 0x0006ecd4
  0007f522: cbz r0,0x0007f556
  0007f524: ldr r0,[0x0007f7c0]
  0007f526: add r5,sp,#0x8
  0007f528: ldr r2,[sp,#0x18]
  0007f52a: str r0,[sp,#0xc]
  0007f52c: mov.w r0,#0x2a2a2a2a
  0007f530: str r0,[sp,#0x8]
  0007f532: movs r0,#0x8
  0007f534: mov r1,r5
  0007f536: blx 0x0006eb90
  0007f53a: cmp r0,#0x0
  0007f53c: beq.w 0x0007f79c
  0007f540: ldr r1,[0x0007f7c8]
  0007f542: movs r0,#0x0
  0007f544: add r1,pc
  0007f546: cmp r0,#0x7
  0007f548: bhi 0x0007f564
  0007f54a: ldrb r2,[r5,r0]
  0007f54c: ldrb r3,[r1,r0]
  0007f54e: adds r0,#0x1
  0007f550: cmp r3,r2
  0007f552: beq 0x0007f546
  0007f554: b 0x0007f79c
  0007f556: ldr r0,[r4,#0x0]
  0007f558: cbz r0,0x0007f55e
  0007f55a: blx 0x0006eb48
  0007f55e: movs r0,#0x0
  0007f560: str r0,[r4,#0x0]
  0007f562: b 0x0007f7a8
  0007f564: ldr r2,[sp,#0x18]
  0007f566: sub.w r1,r7,#0x19
  0007f56a: movs r0,#0x1
  0007f56c: blx 0x0006eb90
  0007f570: cmp r0,#0x0
  0007f572: beq.w 0x0007f79c
  0007f576: ldr r2,[sp,#0x18]
  0007f578: movs r0,#0x2
  0007f57a: ldr r1,[r4,#0x0]
  0007f57c: blx 0x0006eb90
  0007f580: cmp r0,#0x0
  0007f582: beq.w 0x0007f79c
  0007f586: ldr r0,[r4,#0x0]
  0007f588: ldr r2,[sp,#0x18]
  0007f58a: adds r1,r0,#0x2
  0007f58c: movs r0,#0x2
  0007f58e: blx 0x0006eb90
  0007f592: cmp r0,#0x0
  0007f594: beq.w 0x0007f79c
  0007f598: ldr r2,[sp,#0x18]
  0007f59a: add r1,sp,#0x14
  0007f59c: movs r0,#0x2
  0007f59e: blx 0x0006eb90
  0007f5a2: cmp r0,#0x0
  0007f5a4: beq.w 0x0007f79c
  0007f5a8: ldrh.w r0,[sp,#0x14]
  0007f5ac: ldr r1,[sp,#0x18]
  0007f5ae: lsls r0,r0,#0x3
  0007f5b0: blx 0x0006f1f0
  0007f5b4: ldrb.w r0,[r7,#-0x19]
  0007f5b8: tst r0,#0x2
  0007f5bc: ittt ne
  0007f5be: ldr.ne r1,[r4,#0x0]
  0007f5c0: mov.ne r2,#0x1
  0007f5c2: strb.ne r2,[r1,#0x8]
  0007f5c4: subs r1,r0,#0x1
  0007f5c6: cmp r1,#0x25
  0007f5c8: bhi 0x0007f64e
  0007f5ca: tbh [pc,r1]
  0007f61a: ldr r2,[sp,#0x18]
  0007f61c: add r1,sp,#0x4
  0007f61e: movs r0,#0x4
  0007f620: blx 0x0006eb90
  0007f624: cmp r0,#0x0
  0007f626: beq.w 0x0007f79c
  0007f62a: ldr r5,[sp,#0x4]
  0007f62c: mov r0,r5
  0007f62e: blx 0x0006ec08
  0007f632: ldr r1,[r4,#0x0]
  0007f634: str r0,[r1,#0xc]
  0007f636: ldr r0,[r4,#0x0]
  0007f638: ldr r2,[sp,#0x18]
  0007f63a: ldr r1,[r0,#0xc]
  0007f63c: mov r0,r5
  0007f63e: blx 0x0006eb90
  0007f642: cmp r0,#0x0
  0007f644: beq.w 0x0007f79c
  0007f648: ldr r0,[r4,#0x0]
  0007f64a: movs r2,#0x4
  0007f64c: b 0x0007f75e
  0007f64e: cmp r0,#0x81
  0007f650: beq 0x0007f6a8
  0007f652: cmp r0,#0x42
  0007f654: beq 0x0007f662
  0007f656: cmp r0,#0x40
  0007f658: bne.w 0x0007f792
  0007f65c: ldr r0,[r4,#0x0]
  0007f65e: movs r1,#0x0
  0007f660: strb r1,[r0,#0x8]
  0007f662: ldr r2,[sp,#0x18]
  0007f664: add r1,sp,#0x4
  0007f666: movs r0,#0x4
  0007f668: blx 0x0006eb90
  0007f66c: cmp r0,#0x0
  0007f66e: beq.w 0x0007f79c
  0007f672: ldr r5,[sp,#0x4]
  0007f674: mov r0,r5
  0007f676: blx 0x0006ec08
  0007f67a: ldr r1,[r4,#0x0]
  0007f67c: str r0,[r1,#0xc]
  0007f67e: ldr r0,[r4,#0x0]
  0007f680: ldr r2,[sp,#0x18]
  0007f682: ldr r1,[r0,#0xc]
  0007f684: mov r0,r5
  0007f686: blx 0x0006eb90
  0007f68a: cmp r0,#0x0
  0007f68c: beq.w 0x0007f79c
  0007f690: ldr r0,[r4,#0x0]
  0007f692: movs r2,#0xb
  0007f694: ldr r1,[sp,#0x4]
  0007f696: str r2,[r0,#0x4]
  0007f698: str r1,[r0,#0x10]
  0007f69a: ldrb.w r1,[r7,#-0x19]
  0007f69e: cmp r1,#0x17
  0007f6a0: itt eq
  0007f6a2: mov.eq r1,#0x0
  0007f6a4: strb.eq r1,[r0,#0x8]
  0007f6a6: b 0x0007f792
  0007f6a8: ldr r5,[r4,#0x0]
  0007f6aa: ldrh r0,[r5,#0x2]
  0007f6ac: ldrh r1,[r5,#0x0]
  0007f6ae: muls r0,r1
  0007f6b0: lsls r0,r0,#0x2
  0007f6b2: blx 0x0006ec08
  0007f6b6: str r0,[r5,#0xc]
  0007f6b8: ldr r0,[r4,#0x0]
  0007f6ba: ldrh r1,[r0,#0x2]
  0007f6bc: ldrh r2,[r0,#0x0]
  0007f6be: mul r3,r2,r1
  0007f6c2: ldr r1,[r0,#0xc]
  0007f6c4: ldr r2,[sp,#0x18]
  0007f6c6: lsls r0,r3,#0x2
  0007f6c8: blx 0x0006eb90
  0007f6cc: cmp r0,#0x0
  0007f6ce: beq 0x0007f79c
  0007f6d0: ldr r0,[r4,#0x0]
  0007f6d2: ldrh r1,[r0,#0x2]
  0007f6d4: ldrh r2,[r0,#0x0]
  0007f6d6: muls r1,r2
  0007f6d8: lsls r1,r1,#0x2
  0007f6da: str r1,[r0,#0x10]
  0007f6dc: ldrsb.w r1,[r7,#-0x19]
  0007f6e0: cmp.w r1,#0xffffffff
  0007f6e4: ite gt
  0007f6e6: mov.gt r1,#0x3
  0007f6e8: mov.le r1,#0x6
  0007f6ea: str r1,[r0,#0x4]
  0007f6ec: b 0x0007f792
  0007f6ee: ldr r0,[r4,#0x0]
  0007f6f0: ldr r2,[sp,#0x18]
  0007f6f2: add.w r1,r0,#0x10
  0007f6f6: movs r0,#0x4
  0007f6f8: blx 0x0006eb90
  0007f6fc: cmp r0,#0x0
  0007f6fe: beq 0x0007f79c
  0007f700: ldr r5,[r4,#0x0]
  0007f702: movs r1,#0x8
  0007f704: b 0x0007f77a
  0007f706: ldr r0,[r4,#0x0]
  0007f708: ldr r2,[sp,#0x18]
  0007f70a: add.w r1,r0,#0x10
  0007f70e: movs r0,#0x4
  0007f710: blx 0x0006eb90
  0007f714: cmp r0,#0x0
  0007f716: beq 0x0007f79c
  0007f718: ldr r5,[r4,#0x0]
  0007f71a: movs r1,#0x9
  0007f71c: b 0x0007f77a
  0007f71e: ldr r0,[r4,#0x0]
  0007f720: ldr r2,[sp,#0x18]
  0007f722: add.w r1,r0,#0x10
  0007f726: movs r0,#0x4
  0007f728: blx 0x0006eb90
  0007f72c: cbz r0,0x0007f79c
  0007f72e: ldr r5,[r4,#0x0]
  0007f730: movs r1,#0xa
  0007f732: b 0x0007f77a
  0007f734: ldr r2,[sp,#0x18]
  0007f736: add r1,sp,#0x4
  0007f738: movs r0,#0x4
  0007f73a: blx 0x0006eb90
  0007f73e: cbz r0,0x0007f79c
  0007f740: ldr r5,[sp,#0x4]
  0007f742: mov r0,r5
  0007f744: blx 0x0006ec08
  0007f748: ldr r1,[r4,#0x0]
  0007f74a: str r0,[r1,#0xc]
  0007f74c: ldr r0,[r4,#0x0]
  0007f74e: ldr r2,[sp,#0x18]
  0007f750: ldr r1,[r0,#0xc]
  0007f752: mov r0,r5
  0007f754: blx 0x0006eb90
  0007f758: cbz r0,0x0007f79c
  0007f75a: ldr r0,[r4,#0x0]
  0007f75c: movs r2,#0x5
  0007f75e: ldr r1,[sp,#0x4]
  0007f760: str r2,[r0,#0x4]
  0007f762: str r1,[r0,#0x10]
  0007f764: b 0x0007f792
  0007f766: ldr r0,[r4,#0x0]
  0007f768: ldr r2,[sp,#0x18]
  0007f76a: add.w r1,r0,#0x10
  0007f76e: movs r0,#0x4
  0007f770: blx 0x0006eb90
  0007f774: cbz r0,0x0007f79c
  0007f776: ldr r5,[r4,#0x0]
  0007f778: movs r1,#0x7
  0007f77a: ldr r0,[r5,#0x10]
  0007f77c: str r1,[r5,#0x4]
  0007f77e: blx 0x0006ec08
  0007f782: str r0,[r5,#0xc]
  0007f784: ldr r0,[r4,#0x0]
  0007f786: ldr r2,[sp,#0x18]
  0007f788: ldr r1,[r0,#0xc]
  0007f78a: ldr r0,[r0,#0x10]
  0007f78c: blx 0x0006eb90
  0007f790: cbz r0,0x0007f79c
  0007f792: ldr r0,[sp,#0x18]
  0007f794: blx 0x0006ece0
  0007f798: movs r0,#0x1
  0007f79a: b 0x0007f7ac
  0007f79c: mov r0,r4
  0007f79e: blx 0x0006f214
  0007f7a2: ldr r0,[sp,#0x18]
  0007f7a4: blx 0x0006ece0
  0007f7a8: mov.w r0,#0xffffffff
  0007f7ac: ldr r1,[sp,#0x1c]
  0007f7ae: ldr r2,[r6,#0x0]
  0007f7b0: subs r1,r2,r1
  0007f7b2: ittt eq
  0007f7b4: add.eq sp,#0x20
  0007f7b6: pop.eq.w r11
  0007f7ba: pop.eq {r4,r5,r6,r7,pc}
  0007f7bc: blx 0x0006e824
```
