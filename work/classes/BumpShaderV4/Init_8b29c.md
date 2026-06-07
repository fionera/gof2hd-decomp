# BumpShaderV4::Init
elf 0x8b29c body 246
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShaderV4::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShaderV4::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009b394 + 0x9b2aa));
  iVar2 = DAT_0009b3a0;
  puVar3 = (undefined4 *)(DAT_0009b39c + 0x9b2b6);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9b2ba);
  iVar2 = DAT_0009b3a4 + 0x9b2c4;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3a8 + 0x9b2d0;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3ac + 0x9b2dc;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3b0 + 0x9b2e8;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009b3b4 + 0x9b2f4);
  iVar2 = DAT_0009b3b8 + 0x9b2f8;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3bc + 0x9b306;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3c0 + 0x9b312;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3c4 + 0x9b31e;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3c8 + 0x9b32a;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3cc + 0x9b336;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3d0 + 0x9b342;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3d4 + 0x9b34e;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3d8 + 0x9b35a;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009b3dc + 0x9b366;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x44)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x44),iVar2);
    }
  }
  return;
}

```

## target disasm
```
  0009b29c: push {r4,r5,r7,lr}
  0009b29e: add r7,sp,#0x8
  0009b2a0: ldr r1,[0x0009b394]
  0009b2a2: mov r4,r0
  0009b2a4: ldr r2,[0x0009b398]
  0009b2a6: add r1,pc
  0009b2a8: add r2,pc
  0009b2aa: blx 0x00070954
  0009b2ae: ldr r2,[0x0009b39c]
  0009b2b0: ldr r1,[0x0009b3a0]
  0009b2b2: add r2,pc
  0009b2b4: str r0,[r4,#0x4]
  0009b2b6: add r1,pc
  0009b2b8: ldr r5,[r2,#0x0]
  0009b2ba: blx r5
  0009b2bc: ldr r1,[0x0009b3a4]
  0009b2be: ldr r2,[r4,#0x4]
  0009b2c0: add r1,pc
  0009b2c2: str r0,[r4,#0x20]
  0009b2c4: mov r0,r2
  0009b2c6: blx r5
  0009b2c8: ldr r1,[0x0009b3a8]
  0009b2ca: ldr r2,[r4,#0x4]
  0009b2cc: add r1,pc
  0009b2ce: str r0,[r4,#0x24]
  0009b2d0: mov r0,r2
  0009b2d2: blx r5
  0009b2d4: ldr r1,[0x0009b3ac]
  0009b2d6: ldr r2,[r4,#0x4]
  0009b2d8: add r1,pc
  0009b2da: str r0,[r4,#0x28]
  0009b2dc: mov r0,r2
  0009b2de: blx r5
  0009b2e0: ldr r1,[0x0009b3b0]
  0009b2e2: ldr r2,[r4,#0x4]
  0009b2e4: add r1,pc
  0009b2e6: str r0,[r4,#0x2c]
  0009b2e8: mov r0,r2
  0009b2ea: blx r5
  0009b2ec: ldr r3,[0x0009b3b4]
  0009b2ee: ldr r1,[0x0009b3b8]
  0009b2f0: add r3,pc
  0009b2f2: ldr r2,[r4,#0x4]
  0009b2f4: add r1,pc
  0009b2f6: str r0,[r4,#0x30]
  0009b2f8: ldr r5,[r3,#0x0]
  0009b2fa: mov r0,r2
  0009b2fc: blx r5
  0009b2fe: ldr r1,[0x0009b3bc]
  0009b300: ldr r2,[r4,#0x4]
  0009b302: add r1,pc
  0009b304: str r0,[r4,#0x34]
  0009b306: mov r0,r2
  0009b308: blx r5
  0009b30a: ldr r1,[0x0009b3c0]
  0009b30c: ldr r2,[r4,#0x4]
  0009b30e: add r1,pc
  0009b310: str r0,[r4,#0x38]
  0009b312: mov r0,r2
  0009b314: blx r5
  0009b316: ldr r1,[0x0009b3c4]
  0009b318: ldr r2,[r4,#0x4]
  0009b31a: add r1,pc
  0009b31c: str r0,[r4,#0x3c]
  0009b31e: mov r0,r2
  0009b320: blx r5
  0009b322: ldr r1,[0x0009b3c8]
  0009b324: ldr r2,[r4,#0x4]
  0009b326: add r1,pc
  0009b328: str r0,[r4,#0x40]
  0009b32a: mov r0,r2
  0009b32c: blx r5
  0009b32e: ldr r1,[0x0009b3cc]
  0009b330: ldr r2,[r4,#0x4]
  0009b332: add r1,pc
  0009b334: str r0,[r4,#0x44]
  0009b336: mov r0,r2
  0009b338: blx r5
  0009b33a: ldr r1,[0x0009b3d0]
  0009b33c: ldr r2,[r4,#0x4]
  0009b33e: add r1,pc
  0009b340: str r0,[r4,#0x48]
  0009b342: mov r0,r2
  0009b344: blx r5
  0009b346: ldr r1,[0x0009b3d4]
  0009b348: ldr r2,[r4,#0x4]
  0009b34a: add r1,pc
  0009b34c: str r0,[r4,#0x4c]
  0009b34e: mov r0,r2
  0009b350: blx r5
  0009b352: ldr r1,[0x0009b3d8]
  0009b354: ldr r2,[r4,#0x4]
  0009b356: add r1,pc
  0009b358: str r0,[r4,#0x50]
  0009b35a: mov r0,r2
  0009b35c: blx r5
  0009b35e: ldr r1,[0x0009b3dc]
  0009b360: ldr r2,[r4,#0x4]
  0009b362: add r1,pc
  0009b364: str r0,[r4,#0x54]
  0009b366: mov r0,r2
  0009b368: blx r5
  0009b36a: ldr r1,[r4,#0x4]
  0009b36c: str r0,[r4,#0x58]
  0009b36e: mov r0,r1
  0009b370: blx 0x00070960
  0009b374: add.w r5,r4,#0x44
  0009b378: movs r4,#0x0
  0009b37a: b 0x0009b38c
  0009b37c: ldr.w r0,[r5,r4,lsl #0x2]
  0009b380: cmp r0,#0x0
  0009b382: blt 0x0009b38a
  0009b384: mov r1,r4
  0009b386: blx 0x0007096c
  0009b38a: adds r4,#0x1
  0009b38c: cmp r4,#0x2
  0009b38e: bne 0x0009b37c
  0009b390: pop {r4,r5,r7,pc}
```
