# BloomShader::Init
elf 0x895ec body 452
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BloomShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::BloomShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  int iVar5;
  undefined4 *puVar6;
  int iVar7;
  int iVar8;
  code *pcVar9;
  code *pcVar10;
  code *pcVar11;
  
  iVar7 = DAT_000997b4 + 0x99602;
  pcVar9 = *(code **)(DAT_000997b0 + 0x995fe);
  uVar1 = (*pcVar9)(param_1,iVar7,DAT_000997b8 + 0x99606);
  iVar8 = DAT_000997bc;
  *(undefined4 *)(param_1 + 4) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar8 + 0x99616);
  iVar8 = DAT_000997c0;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar8 + 0x99622);
  iVar8 = DAT_000997c4;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar8 + 0x9962e);
  iVar8 = DAT_000997c8;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar8 + 0x9963a);
  puVar6 = (undefined4 *)(DAT_000997cc + 0x99642);
  iVar7 = DAT_000997d0 + 0x99646;
  *(undefined4 *)(param_1 + 0x74) = uVar1;
  pcVar11 = (code *)*puVar6;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 4));
  iVar2 = DAT_000997d4 + 0x99658;
  *(undefined4 *)(param_1 + 0x8c) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 4));
  iVar3 = DAT_000997dc + 0x9966a;
  pcVar10 = *(code **)(DAT_000997d8 + 0x99668);
  *(undefined4 *)(param_1 + 0x94) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 4));
  iVar8 = DAT_000997e0;
  *(undefined4 *)(param_1 + 0x90) = uVar1;
  iVar8 = iVar8 + 0x99680;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 4),iVar8);
  *(undefined4 *)(param_1 + 0x98) = uVar1;
  pcVar4 = *(code **)(DAT_000997e4 + 0x9968e);
  (*pcVar4)(*(undefined4 *)(param_1 + 4));
  pcVar9 = *(code **)(DAT_000997e8 + 0x9969c);
  (*pcVar9)(*(undefined4 *)(param_1 + 0x98),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x20),iVar7);
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x20),iVar2);
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x20),iVar3);
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x20),iVar8);
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  (*pcVar4)(*(undefined4 *)(param_1 + 0x20));
  (*pcVar9)(*(undefined4 *)(param_1 + 0x30),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x38),iVar7);
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x38),iVar2);
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x38),iVar3);
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x38),iVar8);
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  iVar5 = DAT_000997ec + 0x9970a;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x38));
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  (*pcVar4)(*(undefined4 *)(param_1 + 0x38));
  (*pcVar9)(*(undefined4 *)(param_1 + 0x48),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x54),iVar7);
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x54),iVar2);
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x54),iVar3);
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x54),iVar8);
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x54),iVar5);
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  (*pcVar4)(*(undefined4 *)(param_1 + 0x54));
  (*pcVar9)(*(undefined4 *)(param_1 + 100),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x74),iVar7);
  *(undefined4 *)(param_1 + 0x78) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x74),iVar2);
  *(undefined4 *)(param_1 + 0x80) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x74),iVar3);
  *(undefined4 *)(param_1 + 0x7c) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x74),iVar8);
  iVar8 = DAT_000997f0 + 0x99782;
  *(undefined4 *)(param_1 + 0x84) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x74),iVar8);
  *(undefined4 *)(param_1 + 0x88) = uVar1;
  (*pcVar4)(*(undefined4 *)(param_1 + 0x74));
  (*pcVar9)(*(undefined4 *)(param_1 + 0x84),0);
  (*pcVar9)(*(undefined4 *)(param_1 + 0x88),1);
  return;
}

```

## target disasm
```
  000995ec: push {r4,r5,r6,r7,lr}
  000995ee: add r7,sp,#0xc
  000995f0: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000995f4: mov r4,r0
  000995f6: ldr r0,[0x000997b0]
  000995f8: ldr r5,[0x000997b4]
  000995fa: add r0,pc
  000995fc: ldr r2,[0x000997b8]
  000995fe: add r5,pc
  00099600: ldr r6,[r0,#0x0]
  00099602: add r2,pc
  00099604: mov r0,r4
  00099606: mov r1,r5
  00099608: blx r6
  0009960a: ldr r2,[0x000997bc]
  0009960c: mov r1,r5
  0009960e: str r0,[r4,#0x4]
  00099610: mov r0,r4
  00099612: add r2,pc
  00099614: blx r6
  00099616: ldr r2,[0x000997c0]
  00099618: mov r1,r5
  0009961a: str r0,[r4,#0x20]
  0009961c: mov r0,r4
  0009961e: add r2,pc
  00099620: blx r6
  00099622: ldr r2,[0x000997c4]
  00099624: mov r1,r5
  00099626: str r0,[r4,#0x38]
  00099628: mov r0,r4
  0009962a: add r2,pc
  0009962c: blx r6
  0009962e: ldr r2,[0x000997c8]
  00099630: mov r1,r5
  00099632: str r0,[r4,#0x54]
  00099634: mov r0,r4
  00099636: add r2,pc
  00099638: blx r6
  0009963a: ldr r2,[0x000997cc]
  0009963c: ldr r1,[0x000997d0]
  0009963e: add r2,pc
  00099640: ldr r3,[r4,#0x4]
  00099642: add r1,pc
  00099644: str r0,[r4,#0x74]
  00099646: ldr.w r9,[r2,#0x0]
  0009964a: mov r0,r3
  0009964c: mov r10,r1
  0009964e: blx r9
  00099650: ldr r1,[0x000997d4]
  00099652: ldr r2,[r4,#0x4]
  00099654: add r1,pc
  00099656: str.w r0,[r4,#0x8c]
  0009965a: mov r0,r2
  0009965c: mov r11,r1
  0009965e: blx r9
  00099660: ldr r2,[0x000997d8]
  00099662: ldr r1,[0x000997dc]
  00099664: add r2,pc
  00099666: add r1,pc
  00099668: ldr r6,[r2,#0x0]
  0009966a: ldr r2,[r4,#0x4]
  0009966c: mov r8,r1
  0009966e: str.w r0,[r4,#0x94]
  00099672: mov r0,r2
  00099674: blx r6
  00099676: ldr r5,[0x000997e0]
  00099678: str.w r0,[r4,#0x90]
  0009967c: add r5,pc
  0009967e: ldr r0,[r4,#0x4]
  00099680: mov r1,r5
  00099682: blx r6
  00099684: str.w r0,[r4,#0x98]
  00099688: ldr r0,[0x000997e4]
  0009968a: add r0,pc
  0009968c: ldr r1,[r0,#0x0]
  0009968e: ldr r0,[r4,#0x4]
  00099690: str r1,[sp,#0x14]
  00099692: blx r1
  00099694: ldr r0,[0x000997e8]
  00099696: movs r1,#0x0
  00099698: add r0,pc
  0009969a: ldr r0,[r0,#0x0]
  0009969c: str r0,[sp,#0x18]
  0009969e: ldr.w r0,[r4,#0x98]
  000996a2: ldr r2,[sp,#0x18]
  000996a4: blx r2
  000996a6: ldr r0,[r4,#0x20]
  000996a8: mov r1,r10
  000996aa: str.w r10,[sp,#0x10]
  000996ae: blx r9
  000996b0: str r0,[r4,#0x24]
  000996b2: mov r1,r11
  000996b4: ldr r0,[r4,#0x20]
  000996b6: blx r9
  000996b8: str r0,[r4,#0x2c]
  000996ba: mov r1,r8
  000996bc: ldr r0,[r4,#0x20]
  000996be: blx r6
  000996c0: str r0,[r4,#0x28]
  000996c2: mov r1,r5
  000996c4: ldr r0,[r4,#0x20]
  000996c6: str r5,[sp,#0xc]
  000996c8: blx r6
  000996ca: str r0,[r4,#0x30]
  000996cc: ldr r0,[r4,#0x20]
  000996ce: ldr r1,[sp,#0x14]
  000996d0: blx r1
  000996d2: ldr r0,[r4,#0x30]
  000996d4: movs r1,#0x0
  000996d6: ldr r2,[sp,#0x18]
  000996d8: blx r2
  000996da: ldr r0,[r4,#0x38]
  000996dc: mov r1,r10
  000996de: blx r9
  000996e0: str r0,[r4,#0x3c]
  000996e2: mov r1,r11
  000996e4: ldr r0,[r4,#0x38]
  000996e6: mov r10,r11
  000996e8: str.w r11,[sp,#0x8]
  000996ec: blx r9
  000996ee: str r0,[r4,#0x44]
  000996f0: mov r1,r8
  000996f2: ldr r0,[r4,#0x38]
  000996f4: mov r11,r8
  000996f6: blx r6
  000996f8: str r0,[r4,#0x40]
  000996fa: mov r1,r5
  000996fc: ldr r0,[r4,#0x38]
  000996fe: blx r6
  00099700: str r0,[r4,#0x48]
  00099702: ldr r1,[0x000997ec]
  00099704: ldr r0,[r4,#0x38]
  00099706: add r1,pc
  00099708: str r1,[sp,#0x4]
  0009970a: blx r6
  0009970c: str r0,[r4,#0x4c]
  0009970e: ldr r0,[r4,#0x38]
  00099710: ldr r5,[sp,#0x14]
  00099712: blx r5
  00099714: ldr r0,[r4,#0x48]
  00099716: movs r1,#0x0
  00099718: ldr r2,[sp,#0x18]
  0009971a: blx r2
  0009971c: ldr.w r8,[sp,#0x10]
  00099720: ldr r0,[r4,#0x54]
  00099722: mov r1,r8
  00099724: blx r9
  00099726: str r0,[r4,#0x58]
  00099728: mov r1,r10
  0009972a: ldr r0,[r4,#0x54]
  0009972c: blx r9
  0009972e: str r0,[r4,#0x60]
  00099730: mov r1,r11
  00099732: ldr r0,[r4,#0x54]
  00099734: blx r6
  00099736: str r0,[r4,#0x5c]
  00099738: ldr.w r10,[sp,#0xc]
  0009973c: ldr r0,[r4,#0x54]
  0009973e: mov r1,r10
  00099740: blx r6
  00099742: str r0,[r4,#0x64]
  00099744: ldr r0,[r4,#0x54]
  00099746: ldr r1,[sp,#0x4]
  00099748: blx r6
  0009974a: str r0,[r4,#0x68]
  0009974c: ldr r0,[r4,#0x54]
  0009974e: blx r5
  00099750: ldr r0,[r4,#0x64]
  00099752: movs r1,#0x0
  00099754: ldr r2,[sp,#0x18]
  00099756: blx r2
  00099758: ldr r0,[r4,#0x74]
  0009975a: mov r1,r8
  0009975c: blx r9
  0009975e: str r0,[r4,#0x78]
  00099760: ldr r0,[r4,#0x74]
  00099762: ldr r1,[sp,#0x8]
  00099764: blx r9
  00099766: str.w r0,[r4,#0x80]
  0009976a: mov r1,r11
  0009976c: ldr r0,[r4,#0x74]
  0009976e: blx r6
  00099770: ldr r1,[r4,#0x74]
  00099772: str r0,[r4,#0x7c]
  00099774: mov r0,r1
  00099776: mov r1,r10
  00099778: blx r6
  0009977a: ldr r1,[0x000997f0]
  0009977c: ldr r2,[r4,#0x74]
  0009977e: add r1,pc
  00099780: str.w r0,[r4,#0x84]
  00099784: mov r0,r2
  00099786: blx r6
  00099788: ldr r1,[r4,#0x74]
  0009978a: str.w r0,[r4,#0x88]
  0009978e: mov r0,r1
  00099790: blx r5
  00099792: ldr.w r0,[r4,#0x84]
  00099796: movs r1,#0x0
  00099798: ldr r2,[sp,#0x18]
  0009979a: blx r2
  0009979c: ldr.w r0,[r4,#0x88]
  000997a0: movs r1,#0x1
  000997a2: ldr r2,[sp,#0x18]
  000997a4: add sp,#0x1c
  000997a6: pop.w {r8,r9,r10,r11}
  000997aa: pop.w {r4,r5,r6,r7,lr}
  000997ae: bx r2
```
