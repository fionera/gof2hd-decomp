# GenericShader::Init
elf 0x8e7dc body 270
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::GenericShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::GenericShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009e8ec + 0x9e7ea));
  iVar2 = DAT_0009e8f8;
  puVar3 = (undefined4 *)(DAT_0009e8f4 + 0x9e7f6);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9e7fa);
  iVar2 = DAT_0009e8fc + 0x9e804;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e900 + 0x9e810;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e904 + 0x9e81c;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e908 + 0x9e828;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009e90c + 0x9e834);
  iVar2 = DAT_0009e910 + 0x9e838;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e914 + 0x9e846;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e918 + 0x9e852;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e91c + 0x9e85e;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e920 + 0x9e86a;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e924 + 0x9e876;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e928 + 0x9e882;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e92c + 0x9e88e;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e930 + 0x9e89a;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e934 + 0x9e8a6;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e938 + 0x9e8b2;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009e93c + 0x9e8be;
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 3; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x44)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x44),iVar2);
    }
  }
  return;
}

```

## target disasm
```
  0009e7dc: push {r4,r5,r7,lr}
  0009e7de: add r7,sp,#0x8
  0009e7e0: ldr r1,[0x0009e8ec]
  0009e7e2: mov r4,r0
  0009e7e4: ldr r2,[0x0009e8f0]
  0009e7e6: add r1,pc
  0009e7e8: add r2,pc
  0009e7ea: blx 0x00070954
  0009e7ee: ldr r2,[0x0009e8f4]
  0009e7f0: ldr r1,[0x0009e8f8]
  0009e7f2: add r2,pc
  0009e7f4: str r0,[r4,#0x4]
  0009e7f6: add r1,pc
  0009e7f8: ldr r5,[r2,#0x0]
  0009e7fa: blx r5
  0009e7fc: ldr r1,[0x0009e8fc]
  0009e7fe: ldr r2,[r4,#0x4]
  0009e800: add r1,pc
  0009e802: str r0,[r4,#0x20]
  0009e804: mov r0,r2
  0009e806: blx r5
  0009e808: ldr r1,[0x0009e900]
  0009e80a: ldr r2,[r4,#0x4]
  0009e80c: add r1,pc
  0009e80e: str r0,[r4,#0x24]
  0009e810: mov r0,r2
  0009e812: blx r5
  0009e814: ldr r1,[0x0009e904]
  0009e816: ldr r2,[r4,#0x4]
  0009e818: add r1,pc
  0009e81a: str r0,[r4,#0x28]
  0009e81c: mov r0,r2
  0009e81e: blx r5
  0009e820: ldr r1,[0x0009e908]
  0009e822: ldr r2,[r4,#0x4]
  0009e824: add r1,pc
  0009e826: str r0,[r4,#0x2c]
  0009e828: mov r0,r2
  0009e82a: blx r5
  0009e82c: ldr r3,[0x0009e90c]
  0009e82e: ldr r1,[0x0009e910]
  0009e830: add r3,pc
  0009e832: ldr r2,[r4,#0x4]
  0009e834: add r1,pc
  0009e836: str r0,[r4,#0x30]
  0009e838: ldr r5,[r3,#0x0]
  0009e83a: mov r0,r2
  0009e83c: blx r5
  0009e83e: ldr r1,[0x0009e914]
  0009e840: ldr r2,[r4,#0x4]
  0009e842: add r1,pc
  0009e844: str r0,[r4,#0x34]
  0009e846: mov r0,r2
  0009e848: blx r5
  0009e84a: ldr r1,[0x0009e918]
  0009e84c: ldr r2,[r4,#0x4]
  0009e84e: add r1,pc
  0009e850: str r0,[r4,#0x38]
  0009e852: mov r0,r2
  0009e854: blx r5
  0009e856: ldr r1,[0x0009e91c]
  0009e858: ldr r2,[r4,#0x4]
  0009e85a: add r1,pc
  0009e85c: str r0,[r4,#0x3c]
  0009e85e: mov r0,r2
  0009e860: blx r5
  0009e862: ldr r1,[0x0009e920]
  0009e864: ldr r2,[r4,#0x4]
  0009e866: add r1,pc
  0009e868: str r0,[r4,#0x40]
  0009e86a: mov r0,r2
  0009e86c: blx r5
  0009e86e: ldr r1,[0x0009e924]
  0009e870: ldr r2,[r4,#0x4]
  0009e872: add r1,pc
  0009e874: str r0,[r4,#0x44]
  0009e876: mov r0,r2
  0009e878: blx r5
  0009e87a: ldr r1,[0x0009e928]
  0009e87c: ldr r2,[r4,#0x4]
  0009e87e: add r1,pc
  0009e880: str r0,[r4,#0x48]
  0009e882: mov r0,r2
  0009e884: blx r5
  0009e886: ldr r1,[0x0009e92c]
  0009e888: ldr r2,[r4,#0x4]
  0009e88a: add r1,pc
  0009e88c: str r0,[r4,#0x4c]
  0009e88e: mov r0,r2
  0009e890: blx r5
  0009e892: ldr r1,[0x0009e930]
  0009e894: ldr r2,[r4,#0x4]
  0009e896: add r1,pc
  0009e898: str r0,[r4,#0x50]
  0009e89a: mov r0,r2
  0009e89c: blx r5
  0009e89e: ldr r1,[0x0009e934]
  0009e8a0: ldr r2,[r4,#0x4]
  0009e8a2: add r1,pc
  0009e8a4: str r0,[r4,#0x54]
  0009e8a6: mov r0,r2
  0009e8a8: blx r5
  0009e8aa: ldr r1,[0x0009e938]
  0009e8ac: ldr r2,[r4,#0x4]
  0009e8ae: add r1,pc
  0009e8b0: str r0,[r4,#0x58]
  0009e8b2: mov r0,r2
  0009e8b4: blx r5
  0009e8b6: ldr r1,[0x0009e93c]
  0009e8b8: ldr r2,[r4,#0x4]
  0009e8ba: add r1,pc
  0009e8bc: str r0,[r4,#0x5c]
  0009e8be: mov r0,r2
  0009e8c0: blx r5
  0009e8c2: ldr r1,[r4,#0x4]
  0009e8c4: str r0,[r4,#0x60]
  0009e8c6: mov r0,r1
  0009e8c8: blx 0x00070960
  0009e8cc: add.w r5,r4,#0x44
  0009e8d0: movs r4,#0x0
  0009e8d2: b 0x0009e8e4
  0009e8d4: ldr.w r0,[r5,r4,lsl #0x2]
  0009e8d8: cmp r0,#0x0
  0009e8da: blt 0x0009e8e2
  0009e8dc: mov r1,r4
  0009e8de: blx 0x0007096c
  0009e8e2: adds r4,#0x1
  0009e8e4: cmp r4,#0x3
  0009e8e6: bne 0x0009e8d4
  0009e8e8: pop {r4,r5,r7,pc}
```
