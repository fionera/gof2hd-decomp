# GenericShader1::Init
elf 0x8ec2c body 216
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::GenericShader1::Init(AbyssEngine::Engine*) */

void AbyssEngine::GenericShader1::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009ed04 + 0x9ec3a));
  iVar2 = DAT_0009ed10;
  puVar3 = (undefined4 *)(DAT_0009ed0c + 0x9ec46);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9ec4a);
  iVar2 = DAT_0009ed14 + 0x9ec54;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed18 + 0x9ec60;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed1c + 0x9ec6c;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed20 + 0x9ec78;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009ed24 + 0x9ec84);
  iVar2 = DAT_0009ed28 + 0x9ec88;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed2c + 0x9ec96;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed30 + 0x9eca2;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed34 + 0x9ecae;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed38 + 0x9ecba;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed3c + 0x9ecc6;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed40 + 0x9ecd2;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed44 + 0x9ecde;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ed48 + 0x9ecea;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x44),0);
  return;
}

```

## target disasm
```
  0009ec2c: push {r4,r5,r7,lr}
  0009ec2e: add r7,sp,#0x8
  0009ec30: ldr r1,[0x0009ed04]
  0009ec32: mov r4,r0
  0009ec34: ldr r2,[0x0009ed08]
  0009ec36: add r1,pc
  0009ec38: add r2,pc
  0009ec3a: blx 0x00070954
  0009ec3e: ldr r2,[0x0009ed0c]
  0009ec40: ldr r1,[0x0009ed10]
  0009ec42: add r2,pc
  0009ec44: str r0,[r4,#0x4]
  0009ec46: add r1,pc
  0009ec48: ldr r5,[r2,#0x0]
  0009ec4a: blx r5
  0009ec4c: ldr r1,[0x0009ed14]
  0009ec4e: ldr r2,[r4,#0x4]
  0009ec50: add r1,pc
  0009ec52: str r0,[r4,#0x20]
  0009ec54: mov r0,r2
  0009ec56: blx r5
  0009ec58: ldr r1,[0x0009ed18]
  0009ec5a: ldr r2,[r4,#0x4]
  0009ec5c: add r1,pc
  0009ec5e: str r0,[r4,#0x24]
  0009ec60: mov r0,r2
  0009ec62: blx r5
  0009ec64: ldr r1,[0x0009ed1c]
  0009ec66: ldr r2,[r4,#0x4]
  0009ec68: add r1,pc
  0009ec6a: str r0,[r4,#0x28]
  0009ec6c: mov r0,r2
  0009ec6e: blx r5
  0009ec70: ldr r1,[0x0009ed20]
  0009ec72: ldr r2,[r4,#0x4]
  0009ec74: add r1,pc
  0009ec76: str r0,[r4,#0x2c]
  0009ec78: mov r0,r2
  0009ec7a: blx r5
  0009ec7c: ldr r3,[0x0009ed24]
  0009ec7e: ldr r1,[0x0009ed28]
  0009ec80: add r3,pc
  0009ec82: ldr r2,[r4,#0x4]
  0009ec84: add r1,pc
  0009ec86: str r0,[r4,#0x30]
  0009ec88: ldr r5,[r3,#0x0]
  0009ec8a: mov r0,r2
  0009ec8c: blx r5
  0009ec8e: ldr r1,[0x0009ed2c]
  0009ec90: ldr r2,[r4,#0x4]
  0009ec92: add r1,pc
  0009ec94: str r0,[r4,#0x34]
  0009ec96: mov r0,r2
  0009ec98: blx r5
  0009ec9a: ldr r1,[0x0009ed30]
  0009ec9c: ldr r2,[r4,#0x4]
  0009ec9e: add r1,pc
  0009eca0: str r0,[r4,#0x38]
  0009eca2: mov r0,r2
  0009eca4: blx r5
  0009eca6: ldr r1,[0x0009ed34]
  0009eca8: ldr r2,[r4,#0x4]
  0009ecaa: add r1,pc
  0009ecac: str r0,[r4,#0x3c]
  0009ecae: mov r0,r2
  0009ecb0: blx r5
  0009ecb2: ldr r1,[0x0009ed38]
  0009ecb4: ldr r2,[r4,#0x4]
  0009ecb6: add r1,pc
  0009ecb8: str r0,[r4,#0x40]
  0009ecba: mov r0,r2
  0009ecbc: blx r5
  0009ecbe: ldr r1,[0x0009ed3c]
  0009ecc0: ldr r2,[r4,#0x4]
  0009ecc2: add r1,pc
  0009ecc4: str r0,[r4,#0x44]
  0009ecc6: mov r0,r2
  0009ecc8: blx r5
  0009ecca: ldr r1,[0x0009ed40]
  0009eccc: ldr r2,[r4,#0x4]
  0009ecce: add r1,pc
  0009ecd0: str r0,[r4,#0x48]
  0009ecd2: mov r0,r2
  0009ecd4: blx r5
  0009ecd6: ldr r1,[0x0009ed44]
  0009ecd8: ldr r2,[r4,#0x4]
  0009ecda: add r1,pc
  0009ecdc: str r0,[r4,#0x4c]
  0009ecde: mov r0,r2
  0009ece0: blx r5
  0009ece2: ldr r1,[0x0009ed48]
  0009ece4: ldr r2,[r4,#0x4]
  0009ece6: add r1,pc
  0009ece8: str r0,[r4,#0x50]
  0009ecea: mov r0,r2
  0009ecec: blx r5
  0009ecee: ldr r1,[r4,#0x4]
  0009ecf0: str r0,[r4,#0x54]
  0009ecf2: mov r0,r1
  0009ecf4: blx 0x00070960
  0009ecf8: ldr r0,[r4,#0x44]
  0009ecfa: movs r1,#0x0
  0009ecfc: pop.w {r4,r5,r7,lr}
  0009ed00: b.w 0x001ab768
```
