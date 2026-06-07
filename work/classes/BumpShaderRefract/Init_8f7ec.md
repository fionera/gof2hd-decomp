# BumpShaderRefract::Init
elf 0x8f7ec body 230
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShaderRefract::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShaderRefract::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009f8d4 + 0x9f800));
  iVar2 = DAT_0009f8e0;
  puVar3 = (undefined4 *)(DAT_0009f8dc + 0x9f80c);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9f810);
  iVar2 = DAT_0009f8e4 + 0x9f81a;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f8e8 + 0x9f826;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009f8ec + 0x9f832);
  iVar2 = DAT_0009f8f0 + 0x9f836;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f8f4 + 0x9f844;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f8f8 + 0x9f850;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f8fc + 0x9f85c;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f900 + 0x9f868;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f904 + 0x9f874;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009f908 + 0x9f880;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  glActiveTexture(0x84c7);
  Engine::ActivateRefractFBO();
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),DAT_0009f90c + 0x9f89c);
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x30)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x30),iVar2);
    }
  }
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x3c),7);
  return;
}

```

## target disasm
```
  0009f7ec: push {r4,r5,r6,r7,lr}
  0009f7ee: add r7,sp,#0xc
  0009f7f0: push.w r11
  0009f7f4: mov r5,r1
  0009f7f6: ldr r1,[0x0009f8d4]
  0009f7f8: ldr r2,[0x0009f8d8]
  0009f7fa: mov r4,r0
  0009f7fc: add r1,pc
  0009f7fe: add r2,pc
  0009f800: blx 0x00070954
  0009f804: ldr r2,[0x0009f8dc]
  0009f806: ldr r1,[0x0009f8e0]
  0009f808: add r2,pc
  0009f80a: str r0,[r4,#0x4]
  0009f80c: add r1,pc
  0009f80e: ldr r6,[r2,#0x0]
  0009f810: blx r6
  0009f812: ldr r1,[0x0009f8e4]
  0009f814: ldr r2,[r4,#0x4]
  0009f816: add r1,pc
  0009f818: str r0,[r4,#0x20]
  0009f81a: mov r0,r2
  0009f81c: blx r6
  0009f81e: ldr r1,[0x0009f8e8]
  0009f820: ldr r2,[r4,#0x4]
  0009f822: add r1,pc
  0009f824: str r0,[r4,#0x24]
  0009f826: mov r0,r2
  0009f828: blx r6
  0009f82a: ldr r3,[0x0009f8ec]
  0009f82c: ldr r1,[0x0009f8f0]
  0009f82e: add r3,pc
  0009f830: ldr r2,[r4,#0x4]
  0009f832: add r1,pc
  0009f834: str r0,[r4,#0x28]
  0009f836: ldr r6,[r3,#0x0]
  0009f838: mov r0,r2
  0009f83a: blx r6
  0009f83c: ldr r1,[0x0009f8f4]
  0009f83e: ldr r2,[r4,#0x4]
  0009f840: add r1,pc
  0009f842: str r0,[r4,#0x2c]
  0009f844: mov r0,r2
  0009f846: blx r6
  0009f848: ldr r1,[0x0009f8f8]
  0009f84a: ldr r2,[r4,#0x4]
  0009f84c: add r1,pc
  0009f84e: str r0,[r4,#0x30]
  0009f850: mov r0,r2
  0009f852: blx r6
  0009f854: ldr r1,[0x0009f8fc]
  0009f856: ldr r2,[r4,#0x4]
  0009f858: add r1,pc
  0009f85a: str r0,[r4,#0x34]
  0009f85c: mov r0,r2
  0009f85e: blx r6
  0009f860: ldr r1,[0x0009f900]
  0009f862: ldr r2,[r4,#0x4]
  0009f864: add r1,pc
  0009f866: str r0,[r4,#0x38]
  0009f868: mov r0,r2
  0009f86a: blx r6
  0009f86c: ldr r1,[0x0009f904]
  0009f86e: ldr r2,[r4,#0x4]
  0009f870: add r1,pc
  0009f872: str r0,[r4,#0x40]
  0009f874: mov r0,r2
  0009f876: blx r6
  0009f878: ldr r1,[0x0009f908]
  0009f87a: ldr r2,[r4,#0x4]
  0009f87c: add r1,pc
  0009f87e: str r0,[r4,#0x44]
  0009f880: mov r0,r2
  0009f882: blx r6
  0009f884: str r0,[r4,#0x48]
  0009f886: movw r0,#0x84c7
  0009f88a: blx 0x0006fb8c
  0009f88e: mov r0,r5
  0009f890: blx 0x00070c3c
  0009f894: ldr r1,[0x0009f90c]
  0009f896: ldr r0,[r4,#0x4]
  0009f898: add r1,pc
  0009f89a: blx r6
  0009f89c: ldr r1,[r4,#0x4]
  0009f89e: str r0,[r4,#0x3c]
  0009f8a0: mov r0,r1
  0009f8a2: blx 0x00070960
  0009f8a6: add.w r6,r4,#0x30
  0009f8aa: movs r5,#0x0
  0009f8ac: b 0x0009f8be
  0009f8ae: ldr.w r0,[r6,r5,lsl #0x2]
  0009f8b2: cmp r0,#0x0
  0009f8b4: blt 0x0009f8bc
  0009f8b6: mov r1,r5
  0009f8b8: blx 0x0007096c
  0009f8bc: adds r5,#0x1
  0009f8be: cmp r5,#0x2
  0009f8c0: bne 0x0009f8ae
  0009f8c2: ldr r0,[r4,#0x3c]
  0009f8c4: movs r1,#0x7
  0009f8c6: pop.w r11
  0009f8ca: pop.w {r4,r5,r6,r7,lr}
  0009f8ce: b.w 0x001ab768
```
