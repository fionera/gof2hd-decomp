# AEFile::collectFilesInPakFiles
elf 0x703bc body 330
Sig: undefined __stdcall collectFilesInPakFiles(String * param_1)

## decompile
```c

/* AEFile::collectFilesInPakFiles(AbyssEngine::String&) */

void AEFile::collectFilesInPakFiles(String *param_1)

{
  undefined1 *puVar1;
  uint uVar2;
  char *pcVar3;
  AEPakFileEntry *pAVar4;
  undefined4 uVar5;
  Array *pAVar6;
  int *piVar7;
  int iVar8;
  code *pcVar9;
  undefined4 *puVar10;
  int iVar11;
  undefined8 in_d6;
  undefined4 in_s14;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined8 uVar16;
  String aSStack_54 [12];
  undefined8 local_48;
  undefined4 uStack_40;
  int local_3c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffff8;
  piVar7 = *(int **)(DAT_00080524 + 0x803d4);
  _uStack_40 = CONCAT44(*piVar7,in_s14);
  if ((**(int **)(DAT_00080520 + 0x803d2) != 0) &&
     (*(char *)(**(int **)(DAT_00080520 + 0x803d2) + 4) != '\0')) {
    local_48 = in_d6;
    OpenRead(param_1,(uint *)0x0);
    iVar11 = 1;
    uVar2 = Read(1,(void *)((int)&uStack_40 + 3),0);
    uVar12 = 0;
    uVar13 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
    uVar14 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
    uVar15 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
    pcVar9 = *(code **)(DAT_00080528 + 0x80410);
    puVar10 = *(undefined4 **)(DAT_0008052c + 0x80418);
    while ((uVar2 & 1) != 0) {
      iVar8 = (int)uStack_40._3_1_;
      uVar2 = iVar8 + 1;
      if (iVar8 < -1) {
        uVar2 = 0xffffffff;
      }
      pcVar3 = operator_new__(uVar2);
      (*pcVar9)(iVar8,pcVar3,0);
      pcVar3[uStack_40._3_1_] = '\0';
      (*pcVar9)(4,(int)&local_48 + 4,0);
      (*pcVar9)(4,&local_48,0);
      pAVar4 = operator_new(0x1c);
      *(undefined4 *)(pAVar4 + 0xc) = uVar12;
      *(undefined4 *)(pAVar4 + 0x10) = uVar13;
      *(undefined4 *)(pAVar4 + 0x14) = uVar14;
      *(undefined4 *)(pAVar4 + 0x18) = uVar15;
      *(undefined4 *)pAVar4 = uVar12;
      *(undefined4 *)(pAVar4 + 4) = uVar13;
      *(undefined4 *)(pAVar4 + 8) = uVar14;
      *(undefined4 *)(pAVar4 + 0xc) = uVar15;
      AbyssEngine::String::String((String *)(pAVar4 + 4));
      uVar16 = AbyssEngine::String::String(aSStack_54,pcVar3,false);
      uVar5 = crc32_ccitt((AEFile *)uVar16,(String *)((ulonglong)uVar16 >> 0x20));
      *(undefined4 *)pAVar4 = uVar5;
      AbyssEngine::String::~String(aSStack_54);
      AbyssEngine::String::operator=((String *)(pAVar4 + 4),param_1);
      iVar11 = uStack_40._3_1_ + iVar11 + 8;
      *(int *)(pAVar4 + 0x10) = iVar11;
      *(uint *)(pAVar4 + 0x14) = local_48._4_4_;
      pAVar6 = (Array *)*puVar10;
      *(uint *)(pAVar4 + 0x18) = (uint)local_48;
      ArrayAdd<AEPakFileEntry*>(pAVar4,pAVar6);
      operator_delete__(pcVar3);
      if ((uint)local_48 == 0xffffffff) {
        Skip(local_48._4_4_,0);
        puVar1 = &stack0xfffffff4;
      }
      else {
        Skip((uint)local_48,0);
        puVar1 = &stack0xfffffff0;
      }
      iVar8 = *(int *)(puVar1 + -0x38);
      uVar2 = Read(1,(void *)((int)&uStack_40 + 3),0);
      iVar11 = iVar11 + iVar8 + 1;
    }
    Close(0);
  }
  if (*piVar7 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  000803bc: push {r4,r5,r6,r7,lr}
  000803be: add r7,sp,#0xc
  000803c0: push {r7,r8,r9,r10,r11}
  000803c4: vpush {d3,d4,d5,d6,d7,d8,d9}
  000803c8: str r0,[sp,#0x8]
  000803ca: ldr r0,[0x00080520]
  000803cc: ldr r1,[0x00080524]
  000803ce: add r0,pc
  000803d0: add r1,pc
  000803d2: ldr r0,[r0,#0x0]
  000803d4: ldr r2,[r1,#0x0]
  000803d6: ldr r0,[r0,#0x0]
  000803d8: ldr r1,[r2,#0x0]
  000803da: cmp r0,#0x0
  000803dc: str r1,[sp,#0x24]
  000803de: beq.w 0x000804ee
  000803e2: ldrb r0,[r0,#0x4]
  000803e4: cmp r0,#0x0
  000803e6: beq.w 0x000804ee
  000803ea: ldr r0,[sp,#0x8]
  000803ec: movs r1,#0x0
  000803ee: str r2,[sp,#0x4]
  000803f0: blx 0x0006f34c
  000803f4: sub.w r1,r7,#0x35
  000803f8: movs r0,#0x1
  000803fa: movs r2,#0x0
  000803fc: mov.w r11,#0x1
  00080400: blx 0x0006eb90
  00080404: ldr r1,[0x00080528]
  00080406: vmov.i32 q4,#0x0
  0008040a: add r6,sp,#0xc
  0008040c: add r1,pc
  0008040e: ldr.w r9,[r1,#0x0]
  00080412: ldr r1,[0x0008052c]
  00080414: add r1,pc
  00080416: ldr.w r10,[r1,#0x0]
  0008041a: b 0x000804e2
  0008041c: ldrsb.w r4,[r7,#-0x35]
  00080420: adds r0,r4,#0x1
  00080422: cmp.w r4,#0xffffffff
  00080426: it lt
  00080428: mov.lt.w r0,#0xffffffff
  0008042c: blx 0x0006ec08
  00080430: mov r5,r0
  00080432: mov r0,r4
  00080434: mov r1,r5
  00080436: movs r2,#0x0
  00080438: blx r9
  0008043a: ldrsb.w r0,[r7,#-0x35]
  0008043e: movs r1,#0x0
  00080440: movs r2,#0x0
  00080442: strb r1,[r5,r0]
  00080444: add r1,sp,#0x1c
  00080446: movs r0,#0x4
  00080448: blx r9
  0008044a: add r1,sp,#0x18
  0008044c: movs r0,#0x4
  0008044e: movs r2,#0x0
  00080450: blx r9
  00080452: movs r0,#0x1c
  00080454: blx 0x0006eb24
  00080458: mov r8,r0
  0008045a: adds r0,#0xc
  0008045c: mov r4,r8
  0008045e: vst1.32 {d8,d9},[r0]
  00080462: movs r0,#0x4
  00080464: vst1.64 {d8,d9},[r4],r0
  00080468: mov r0,r4
  0008046a: blx 0x0006efbc
  0008046e: mov r0,r6
  00080470: mov r1,r5
  00080472: movs r2,#0x0
  00080474: blx 0x0006ee18
  00080478: blx 0x0006f358
  0008047c: str.w r0,[r8,#0x0]
  00080480: mov r0,r6
  00080482: blx 0x0006ee30
  00080486: ldr r1,[sp,#0x8]
  00080488: mov r0,r4
  0008048a: blx 0x0006f2b0
  0008048e: ldrsb.w r0,[r7,#-0x35]
  00080492: add r0,r11
  00080494: add.w r4,r0,#0x8
  00080498: ldrd r0,r1,[sp,#0x18]
  0008049c: strd r4,r1,[r8,#0x10]
  000804a0: ldr.w r1,[r10,#0x0]
  000804a4: str.w r0,[r8,#0x18]
  000804a8: mov r0,r8
  000804aa: blx 0x0006f364
  000804ae: mov r0,r5
  000804b0: blx 0x0006ebfc
  000804b4: ldr r0,[sp,#0x18]
  000804b6: adds r1,r0,#0x1
  000804b8: beq 0x000804c4
  000804ba: movs r1,#0x0
  000804bc: blx 0x0006f1f0
  000804c0: add r0,sp,#0x18
  000804c2: b 0x000804ce
  000804c4: ldr r0,[sp,#0x1c]
  000804c6: movs r1,#0x0
  000804c8: blx 0x0006f1f0
  000804cc: add r0,sp,#0x1c
  000804ce: sub.w r1,r7,#0x35
  000804d2: ldr r5,[r0,#0x0]
  000804d4: movs r0,#0x1
  000804d6: movs r2,#0x0
  000804d8: blx 0x0006eb90
  000804dc: adds r1,r4,r5
  000804de: add.w r11,r1,#0x1
  000804e2: lsls r0,r0,#0x1f
  000804e4: bne 0x0008041c
  000804e6: movs r0,#0x0
  000804e8: blx 0x0006ece0
  000804ec: ldr r2,[sp,#0x4]
  000804ee: ldr r0,[sp,#0x24]
  000804f0: ldr r1,[r2,#0x0]
  000804f2: subs r0,r1,r0
  000804f4: itttt eq
  000804f6: vpop.eq {d3,d4,d5,d6,d7,d8,d9}
  000804fa: add.eq sp,#0x4
  000804fc: pop.eq.w {r8,r9,r10,r11}
  00080500: pop.eq {r4,r5,r6,r7,pc}
  00080502: blx 0x0006e824
```
