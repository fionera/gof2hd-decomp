# SimpleRefractionShader::Init
elf 0x8c74c body 266
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::SimpleRefractionShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::SimpleRefractionShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009c858 + 0x9c760));
  iVar2 = DAT_0009c864;
  puVar3 = (undefined4 *)(DAT_0009c860 + 0x9c76c);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9c770);
  iVar2 = DAT_0009c868 + 0x9c77a;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c86c + 0x9c786;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c870 + 0x9c792;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c874 + 0x9c79e;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009c878 + 0x9c7aa);
  iVar2 = DAT_0009c87c + 0x9c7ae;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c880 + 0x9c7bc;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c884 + 0x9c7c8;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c888 + 0x9c7d4;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c88c + 0x9c7e0;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c890 + 0x9c7ec;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c894 + 0x9c7f8;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  glActiveTexture(0x84c7);
  Engine::ActivateRefractFBO();
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),DAT_0009c898 + 0x9c814);
  iVar2 = DAT_0009c89c + 0x9c81c;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x38)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x38),iVar2);
    }
  }
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x54),7);
  return;
}

```

## target disasm
```
  0009c74c: push {r4,r5,r6,r7,lr}
  0009c74e: add r7,sp,#0xc
  0009c750: push.w r11
  0009c754: mov r5,r1
  0009c756: ldr r1,[0x0009c858]
  0009c758: ldr r2,[0x0009c85c]
  0009c75a: mov r4,r0
  0009c75c: add r1,pc
  0009c75e: add r2,pc
  0009c760: blx 0x00070954
  0009c764: ldr r2,[0x0009c860]
  0009c766: ldr r1,[0x0009c864]
  0009c768: add r2,pc
  0009c76a: str r0,[r4,#0x4]
  0009c76c: add r1,pc
  0009c76e: ldr r6,[r2,#0x0]
  0009c770: blx r6
  0009c772: ldr r1,[0x0009c868]
  0009c774: ldr r2,[r4,#0x4]
  0009c776: add r1,pc
  0009c778: str r0,[r4,#0x20]
  0009c77a: mov r0,r2
  0009c77c: blx r6
  0009c77e: ldr r1,[0x0009c86c]
  0009c780: ldr r2,[r4,#0x4]
  0009c782: add r1,pc
  0009c784: str r0,[r4,#0x24]
  0009c786: mov r0,r2
  0009c788: blx r6
  0009c78a: ldr r1,[0x0009c870]
  0009c78c: ldr r2,[r4,#0x4]
  0009c78e: add r1,pc
  0009c790: str r0,[r4,#0x28]
  0009c792: mov r0,r2
  0009c794: blx r6
  0009c796: ldr r1,[0x0009c874]
  0009c798: ldr r2,[r4,#0x4]
  0009c79a: add r1,pc
  0009c79c: str r0,[r4,#0x2c]
  0009c79e: mov r0,r2
  0009c7a0: blx r6
  0009c7a2: ldr r3,[0x0009c878]
  0009c7a4: ldr r1,[0x0009c87c]
  0009c7a6: add r3,pc
  0009c7a8: ldr r2,[r4,#0x4]
  0009c7aa: add r1,pc
  0009c7ac: str r0,[r4,#0x30]
  0009c7ae: ldr r6,[r3,#0x0]
  0009c7b0: mov r0,r2
  0009c7b2: blx r6
  0009c7b4: ldr r1,[0x0009c880]
  0009c7b6: ldr r2,[r4,#0x4]
  0009c7b8: add r1,pc
  0009c7ba: str r0,[r4,#0x34]
  0009c7bc: mov r0,r2
  0009c7be: blx r6
  0009c7c0: ldr r1,[0x0009c884]
  0009c7c2: ldr r2,[r4,#0x4]
  0009c7c4: add r1,pc
  0009c7c6: str r0,[r4,#0x48]
  0009c7c8: mov r0,r2
  0009c7ca: blx r6
  0009c7cc: ldr r1,[0x0009c888]
  0009c7ce: ldr r2,[r4,#0x4]
  0009c7d0: add r1,pc
  0009c7d2: str r0,[r4,#0x38]
  0009c7d4: mov r0,r2
  0009c7d6: blx r6
  0009c7d8: ldr r1,[0x0009c88c]
  0009c7da: ldr r2,[r4,#0x4]
  0009c7dc: add r1,pc
  0009c7de: str r0,[r4,#0x3c]
  0009c7e0: mov r0,r2
  0009c7e2: blx r6
  0009c7e4: ldr r1,[0x0009c890]
  0009c7e6: ldr r2,[r4,#0x4]
  0009c7e8: add r1,pc
  0009c7ea: str r0,[r4,#0x44]
  0009c7ec: mov r0,r2
  0009c7ee: blx r6
  0009c7f0: ldr r1,[0x0009c894]
  0009c7f2: ldr r2,[r4,#0x4]
  0009c7f4: add r1,pc
  0009c7f6: str r0,[r4,#0x40]
  0009c7f8: mov r0,r2
  0009c7fa: blx r6
  0009c7fc: str r0,[r4,#0x50]
  0009c7fe: movw r0,#0x84c7
  0009c802: blx 0x0006fb8c
  0009c806: mov r0,r5
  0009c808: blx 0x00070c3c
  0009c80c: ldr r1,[0x0009c898]
  0009c80e: ldr r0,[r4,#0x4]
  0009c810: add r1,pc
  0009c812: blx r6
  0009c814: ldr r1,[0x0009c89c]
  0009c816: ldr r2,[r4,#0x4]
  0009c818: add r1,pc
  0009c81a: str r0,[r4,#0x4c]
  0009c81c: mov r0,r2
  0009c81e: blx r6
  0009c820: ldr r1,[r4,#0x4]
  0009c822: str r0,[r4,#0x54]
  0009c824: mov r0,r1
  0009c826: blx 0x00070960
  0009c82a: add.w r6,r4,#0x38
  0009c82e: movs r5,#0x0
  0009c830: b 0x0009c842
  0009c832: ldr.w r0,[r6,r5,lsl #0x2]
  0009c836: cmp r0,#0x0
  0009c838: blt 0x0009c840
  0009c83a: mov r1,r5
  0009c83c: blx 0x0007096c
  0009c840: adds r5,#0x1
  0009c842: cmp r5,#0x2
  0009c844: bne 0x0009c832
  0009c846: ldr r0,[r4,#0x54]
  0009c848: movs r1,#0x7
  0009c84a: pop.w r11
  0009c84e: pop.w {r4,r5,r6,r7,lr}
  0009c852: b.w 0x001ab768
```
