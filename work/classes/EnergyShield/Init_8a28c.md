# EnergyShield::Init
elf 0x8a28c body 266
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::EnergyShield::Init(AbyssEngine::Engine*) */

void AbyssEngine::EnergyShield::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009a398 + 0x9a2a0));
  iVar2 = DAT_0009a3a4;
  puVar3 = (undefined4 *)(DAT_0009a3a0 + 0x9a2ac);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9a2b0);
  iVar2 = DAT_0009a3a8 + 0x9a2ba;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3ac + 0x9a2c6;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009a3b0 + 0x9a2d2);
  iVar2 = DAT_0009a3b4 + 0x9a2d6;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3b8 + 0x9a2e4;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3bc + 0x9a2f0;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3c0 + 0x9a2fc;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3c4 + 0x9a308;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3c8 + 0x9a314;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3cc + 0x9a320;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3d0 + 0x9a32c;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  glActiveTexture(0x84c7);
  Engine::ActivateRefractFBO();
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),DAT_0009a3d4 + 0x9a348);
  iVar2 = DAT_0009a3d8 + 0x9a350;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009a3dc + 0x9a35c;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x38)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x38),iVar2);
    }
  }
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x5c),7);
  return;
}

```

## target disasm
```
  0009a28c: push {r4,r5,r6,r7,lr}
  0009a28e: add r7,sp,#0xc
  0009a290: push.w r11
  0009a294: mov r5,r1
  0009a296: ldr r1,[0x0009a398]
  0009a298: ldr r2,[0x0009a39c]
  0009a29a: mov r4,r0
  0009a29c: add r1,pc
  0009a29e: add r2,pc
  0009a2a0: blx 0x00070954
  0009a2a4: ldr r2,[0x0009a3a0]
  0009a2a6: ldr r1,[0x0009a3a4]
  0009a2a8: add r2,pc
  0009a2aa: str r0,[r4,#0x4]
  0009a2ac: add r1,pc
  0009a2ae: ldr r6,[r2,#0x0]
  0009a2b0: blx r6
  0009a2b2: ldr r1,[0x0009a3a8]
  0009a2b4: ldr r2,[r4,#0x4]
  0009a2b6: add r1,pc
  0009a2b8: str r0,[r4,#0x20]
  0009a2ba: mov r0,r2
  0009a2bc: blx r6
  0009a2be: ldr r1,[0x0009a3ac]
  0009a2c0: ldr r2,[r4,#0x4]
  0009a2c2: add r1,pc
  0009a2c4: str r0,[r4,#0x24]
  0009a2c6: mov r0,r2
  0009a2c8: blx r6
  0009a2ca: ldr r3,[0x0009a3b0]
  0009a2cc: ldr r1,[0x0009a3b4]
  0009a2ce: add r3,pc
  0009a2d0: ldr r2,[r4,#0x4]
  0009a2d2: add r1,pc
  0009a2d4: str r0,[r4,#0x28]
  0009a2d6: ldr r6,[r3,#0x0]
  0009a2d8: mov r0,r2
  0009a2da: blx r6
  0009a2dc: ldr r1,[0x0009a3b8]
  0009a2de: ldr r2,[r4,#0x4]
  0009a2e0: add r1,pc
  0009a2e2: str r0,[r4,#0x2c]
  0009a2e4: mov r0,r2
  0009a2e6: blx r6
  0009a2e8: ldr r1,[0x0009a3bc]
  0009a2ea: ldr r2,[r4,#0x4]
  0009a2ec: add r1,pc
  0009a2ee: str r0,[r4,#0x30]
  0009a2f0: mov r0,r2
  0009a2f2: blx r6
  0009a2f4: ldr r1,[0x0009a3c0]
  0009a2f6: ldr r2,[r4,#0x4]
  0009a2f8: add r1,pc
  0009a2fa: str r0,[r4,#0x34]
  0009a2fc: mov r0,r2
  0009a2fe: blx r6
  0009a300: ldr r1,[0x0009a3c4]
  0009a302: ldr r2,[r4,#0x4]
  0009a304: add r1,pc
  0009a306: str r0,[r4,#0x38]
  0009a308: mov r0,r2
  0009a30a: blx r6
  0009a30c: ldr r1,[0x0009a3c8]
  0009a30e: ldr r2,[r4,#0x4]
  0009a310: add r1,pc
  0009a312: str r0,[r4,#0x3c]
  0009a314: mov r0,r2
  0009a316: blx r6
  0009a318: ldr r1,[0x0009a3cc]
  0009a31a: ldr r2,[r4,#0x4]
  0009a31c: add r1,pc
  0009a31e: str r0,[r4,#0x44]
  0009a320: mov r0,r2
  0009a322: blx r6
  0009a324: ldr r1,[0x0009a3d0]
  0009a326: ldr r2,[r4,#0x4]
  0009a328: add r1,pc
  0009a32a: str r0,[r4,#0x48]
  0009a32c: mov r0,r2
  0009a32e: blx r6
  0009a330: str r0,[r4,#0x58]
  0009a332: movw r0,#0x84c7
  0009a336: blx 0x0006fb8c
  0009a33a: mov r0,r5
  0009a33c: blx 0x00070c3c
  0009a340: ldr r1,[0x0009a3d4]
  0009a342: ldr r0,[r4,#0x4]
  0009a344: add r1,pc
  0009a346: blx r6
  0009a348: ldr r1,[0x0009a3d8]
  0009a34a: ldr r2,[r4,#0x4]
  0009a34c: add r1,pc
  0009a34e: str r0,[r4,#0x54]
  0009a350: mov r0,r2
  0009a352: blx r6
  0009a354: ldr r1,[0x0009a3dc]
  0009a356: ldr r2,[r4,#0x4]
  0009a358: add r1,pc
  0009a35a: str r0,[r4,#0x50]
  0009a35c: mov r0,r2
  0009a35e: blx r6
  0009a360: ldr r1,[r4,#0x4]
  0009a362: str r0,[r4,#0x5c]
  0009a364: mov r0,r1
  0009a366: blx 0x00070960
  0009a36a: add.w r6,r4,#0x38
  0009a36e: movs r5,#0x0
  0009a370: b 0x0009a382
  0009a372: ldr.w r0,[r6,r5,lsl #0x2]
  0009a376: cmp r0,#0x0
  0009a378: blt 0x0009a380
  0009a37a: mov r1,r5
  0009a37c: blx 0x0007096c
  0009a380: adds r5,#0x1
  0009a382: cmp r5,#0x2
  0009a384: bne 0x0009a372
  0009a386: ldr r0,[r4,#0x5c]
  0009a388: movs r1,#0x7
  0009a38a: pop.w r11
  0009a38e: pop.w {r4,r5,r6,r7,lr}
  0009a392: b.w 0x001ab768
```
