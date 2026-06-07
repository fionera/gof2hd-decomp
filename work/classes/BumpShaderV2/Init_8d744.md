# BumpShaderV2::Init
elf 0x8d744 body 246
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShaderV2::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShaderV2::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009d83c + 0x9d752));
  iVar2 = DAT_0009d848;
  puVar3 = (undefined4 *)(DAT_0009d844 + 0x9d75e);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9d762);
  iVar2 = DAT_0009d84c + 0x9d76c;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d850 + 0x9d778;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d854 + 0x9d784;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d858 + 0x9d790;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009d85c + 0x9d79c);
  iVar2 = DAT_0009d860 + 0x9d7a0;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d864 + 0x9d7ae;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d868 + 0x9d7ba;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d86c + 0x9d7c6;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d870 + 0x9d7d2;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d874 + 0x9d7de;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d878 + 0x9d7ea;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d87c + 0x9d7f6;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d880 + 0x9d802;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009d884 + 0x9d80e;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x48)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x48),iVar2);
    }
  }
  return;
}

```

## target disasm
```
  0009d744: push {r4,r5,r7,lr}
  0009d746: add r7,sp,#0x8
  0009d748: ldr r1,[0x0009d83c]
  0009d74a: mov r4,r0
  0009d74c: ldr r2,[0x0009d840]
  0009d74e: add r1,pc
  0009d750: add r2,pc
  0009d752: blx 0x00070954
  0009d756: ldr r2,[0x0009d844]
  0009d758: ldr r1,[0x0009d848]
  0009d75a: add r2,pc
  0009d75c: str r0,[r4,#0x4]
  0009d75e: add r1,pc
  0009d760: ldr r5,[r2,#0x0]
  0009d762: blx r5
  0009d764: ldr r1,[0x0009d84c]
  0009d766: ldr r2,[r4,#0x4]
  0009d768: add r1,pc
  0009d76a: str r0,[r4,#0x20]
  0009d76c: mov r0,r2
  0009d76e: blx r5
  0009d770: ldr r1,[0x0009d850]
  0009d772: ldr r2,[r4,#0x4]
  0009d774: add r1,pc
  0009d776: str r0,[r4,#0x24]
  0009d778: mov r0,r2
  0009d77a: blx r5
  0009d77c: ldr r1,[0x0009d854]
  0009d77e: ldr r2,[r4,#0x4]
  0009d780: add r1,pc
  0009d782: str r0,[r4,#0x28]
  0009d784: mov r0,r2
  0009d786: blx r5
  0009d788: ldr r1,[0x0009d858]
  0009d78a: ldr r2,[r4,#0x4]
  0009d78c: add r1,pc
  0009d78e: str r0,[r4,#0x2c]
  0009d790: mov r0,r2
  0009d792: blx r5
  0009d794: ldr r3,[0x0009d85c]
  0009d796: ldr r1,[0x0009d860]
  0009d798: add r3,pc
  0009d79a: ldr r2,[r4,#0x4]
  0009d79c: add r1,pc
  0009d79e: str r0,[r4,#0x30]
  0009d7a0: ldr r5,[r3,#0x0]
  0009d7a2: mov r0,r2
  0009d7a4: blx r5
  0009d7a6: ldr r1,[0x0009d864]
  0009d7a8: ldr r2,[r4,#0x4]
  0009d7aa: add r1,pc
  0009d7ac: str r0,[r4,#0x38]
  0009d7ae: mov r0,r2
  0009d7b0: blx r5
  0009d7b2: ldr r1,[0x0009d868]
  0009d7b4: ldr r2,[r4,#0x4]
  0009d7b6: add r1,pc
  0009d7b8: str r0,[r4,#0x3c]
  0009d7ba: mov r0,r2
  0009d7bc: blx r5
  0009d7be: ldr r1,[0x0009d86c]
  0009d7c0: ldr r2,[r4,#0x4]
  0009d7c2: add r1,pc
  0009d7c4: str r0,[r4,#0x40]
  0009d7c6: mov r0,r2
  0009d7c8: blx r5
  0009d7ca: ldr r1,[0x0009d870]
  0009d7cc: ldr r2,[r4,#0x4]
  0009d7ce: add r1,pc
  0009d7d0: str r0,[r4,#0x44]
  0009d7d2: mov r0,r2
  0009d7d4: blx r5
  0009d7d6: ldr r1,[0x0009d874]
  0009d7d8: ldr r2,[r4,#0x4]
  0009d7da: add r1,pc
  0009d7dc: str r0,[r4,#0x48]
  0009d7de: mov r0,r2
  0009d7e0: blx r5
  0009d7e2: ldr r1,[0x0009d878]
  0009d7e4: ldr r2,[r4,#0x4]
  0009d7e6: add r1,pc
  0009d7e8: str r0,[r4,#0x4c]
  0009d7ea: mov r0,r2
  0009d7ec: blx r5
  0009d7ee: ldr r1,[0x0009d87c]
  0009d7f0: ldr r2,[r4,#0x4]
  0009d7f2: add r1,pc
  0009d7f4: str r0,[r4,#0x50]
  0009d7f6: mov r0,r2
  0009d7f8: blx r5
  0009d7fa: ldr r1,[0x0009d880]
  0009d7fc: ldr r2,[r4,#0x4]
  0009d7fe: add r1,pc
  0009d800: str r0,[r4,#0x54]
  0009d802: mov r0,r2
  0009d804: blx r5
  0009d806: ldr r1,[0x0009d884]
  0009d808: ldr r2,[r4,#0x4]
  0009d80a: add r1,pc
  0009d80c: str r0,[r4,#0x58]
  0009d80e: mov r0,r2
  0009d810: blx r5
  0009d812: ldr r1,[r4,#0x4]
  0009d814: str r0,[r4,#0x5c]
  0009d816: mov r0,r1
  0009d818: blx 0x00070960
  0009d81c: add.w r5,r4,#0x48
  0009d820: movs r4,#0x0
  0009d822: b 0x0009d834
  0009d824: ldr.w r0,[r5,r4,lsl #0x2]
  0009d828: cmp r0,#0x0
  0009d82a: blt 0x0009d832
  0009d82c: mov r1,r4
  0009d82e: blx 0x0007096c
  0009d832: adds r4,#0x1
  0009d834: cmp r4,#0x2
  0009d836: bne 0x0009d824
  0009d838: pop {r4,r5,r7,pc}
```
