# BumpShaderV3::Init
elf 0x8c1a8 body 368
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShaderV3::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShaderV3::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009c318 + 0x9c1b6));
  iVar2 = DAT_0009c324;
  puVar3 = (undefined4 *)(DAT_0009c320 + 0x9c1c2);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9c1c6);
  iVar2 = DAT_0009c328 + 0x9c1d0;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c32c + 0x9c1dc;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c330 + 0x9c1e8;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c334 + 0x9c1f4;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009c338 + 0x9c200);
  iVar2 = DAT_0009c33c + 0x9c204;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c340 + 0x9c212;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c344 + 0x9c21e;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c348 + 0x9c22a;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c34c + 0x9c236;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c350 + 0x9c242;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c354 + 0x9c24e;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c358 + 0x9c25a;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c35c + 0x9c266;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c360 + 0x9c272;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c364 + 0x9c27e;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c368 + 0x9c28a;
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c36c + 0x9c296;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c370 + 0x9c2a2;
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c374 + 0x9c2ae;
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c378 + 0x9c2ba;
  *(undefined4 *)(param_1 + 0x6c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c37c + 0x9c2c6;
  *(undefined4 *)(param_1 + 0x70) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c380 + 0x9c2d2;
  *(undefined4 *)(param_1 + 0x74) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c384 + 0x9c2de;
  *(undefined4 *)(param_1 + 0x78) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009c388 + 0x9c2ea;
  *(undefined4 *)(param_1 + 0x7c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x80) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 3; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x48)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x48),iVar2);
    }
  }
  return;
}

```

## target disasm
```
  0009c1a8: push {r4,r5,r7,lr}
  0009c1aa: add r7,sp,#0x8
  0009c1ac: ldr r1,[0x0009c318]
  0009c1ae: mov r4,r0
  0009c1b0: ldr r2,[0x0009c31c]
  0009c1b2: add r1,pc
  0009c1b4: add r2,pc
  0009c1b6: blx 0x00070954
  0009c1ba: ldr r2,[0x0009c320]
  0009c1bc: ldr r1,[0x0009c324]
  0009c1be: add r2,pc
  0009c1c0: str r0,[r4,#0x4]
  0009c1c2: add r1,pc
  0009c1c4: ldr r5,[r2,#0x0]
  0009c1c6: blx r5
  0009c1c8: ldr r1,[0x0009c328]
  0009c1ca: ldr r2,[r4,#0x4]
  0009c1cc: add r1,pc
  0009c1ce: str r0,[r4,#0x20]
  0009c1d0: mov r0,r2
  0009c1d2: blx r5
  0009c1d4: ldr r1,[0x0009c32c]
  0009c1d6: ldr r2,[r4,#0x4]
  0009c1d8: add r1,pc
  0009c1da: str r0,[r4,#0x24]
  0009c1dc: mov r0,r2
  0009c1de: blx r5
  0009c1e0: ldr r1,[0x0009c330]
  0009c1e2: ldr r2,[r4,#0x4]
  0009c1e4: add r1,pc
  0009c1e6: str r0,[r4,#0x28]
  0009c1e8: mov r0,r2
  0009c1ea: blx r5
  0009c1ec: ldr r1,[0x0009c334]
  0009c1ee: ldr r2,[r4,#0x4]
  0009c1f0: add r1,pc
  0009c1f2: str r0,[r4,#0x2c]
  0009c1f4: mov r0,r2
  0009c1f6: blx r5
  0009c1f8: ldr r3,[0x0009c338]
  0009c1fa: ldr r1,[0x0009c33c]
  0009c1fc: add r3,pc
  0009c1fe: ldr r2,[r4,#0x4]
  0009c200: add r1,pc
  0009c202: str r0,[r4,#0x30]
  0009c204: ldr r5,[r3,#0x0]
  0009c206: mov r0,r2
  0009c208: blx r5
  0009c20a: ldr r1,[0x0009c340]
  0009c20c: ldr r2,[r4,#0x4]
  0009c20e: add r1,pc
  0009c210: str r0,[r4,#0x34]
  0009c212: mov r0,r2
  0009c214: blx r5
  0009c216: ldr r1,[0x0009c344]
  0009c218: ldr r2,[r4,#0x4]
  0009c21a: add r1,pc
  0009c21c: str r0,[r4,#0x38]
  0009c21e: mov r0,r2
  0009c220: blx r5
  0009c222: ldr r1,[0x0009c348]
  0009c224: ldr r2,[r4,#0x4]
  0009c226: add r1,pc
  0009c228: str r0,[r4,#0x3c]
  0009c22a: mov r0,r2
  0009c22c: blx r5
  0009c22e: ldr r1,[0x0009c34c]
  0009c230: ldr r2,[r4,#0x4]
  0009c232: add r1,pc
  0009c234: str r0,[r4,#0x40]
  0009c236: mov r0,r2
  0009c238: blx r5
  0009c23a: ldr r1,[0x0009c350]
  0009c23c: ldr r2,[r4,#0x4]
  0009c23e: add r1,pc
  0009c240: str r0,[r4,#0x44]
  0009c242: mov r0,r2
  0009c244: blx r5
  0009c246: ldr r1,[0x0009c354]
  0009c248: ldr r2,[r4,#0x4]
  0009c24a: add r1,pc
  0009c24c: str r0,[r4,#0x48]
  0009c24e: mov r0,r2
  0009c250: blx r5
  0009c252: ldr r1,[0x0009c358]
  0009c254: ldr r2,[r4,#0x4]
  0009c256: add r1,pc
  0009c258: str r0,[r4,#0x4c]
  0009c25a: mov r0,r2
  0009c25c: blx r5
  0009c25e: ldr r1,[0x0009c35c]
  0009c260: ldr r2,[r4,#0x4]
  0009c262: add r1,pc
  0009c264: str r0,[r4,#0x50]
  0009c266: mov r0,r2
  0009c268: blx r5
  0009c26a: ldr r1,[0x0009c360]
  0009c26c: ldr r2,[r4,#0x4]
  0009c26e: add r1,pc
  0009c270: str r0,[r4,#0x54]
  0009c272: mov r0,r2
  0009c274: blx r5
  0009c276: ldr r1,[0x0009c364]
  0009c278: ldr r2,[r4,#0x4]
  0009c27a: add r1,pc
  0009c27c: str r0,[r4,#0x58]
  0009c27e: mov r0,r2
  0009c280: blx r5
  0009c282: ldr r1,[0x0009c368]
  0009c284: ldr r2,[r4,#0x4]
  0009c286: add r1,pc
  0009c288: str r0,[r4,#0x60]
  0009c28a: mov r0,r2
  0009c28c: blx r5
  0009c28e: ldr r1,[0x0009c36c]
  0009c290: ldr r2,[r4,#0x4]
  0009c292: add r1,pc
  0009c294: str r0,[r4,#0x68]
  0009c296: mov r0,r2
  0009c298: blx r5
  0009c29a: ldr r1,[0x0009c370]
  0009c29c: ldr r2,[r4,#0x4]
  0009c29e: add r1,pc
  0009c2a0: str r0,[r4,#0x5c]
  0009c2a2: mov r0,r2
  0009c2a4: blx r5
  0009c2a6: ldr r1,[0x0009c374]
  0009c2a8: ldr r2,[r4,#0x4]
  0009c2aa: add r1,pc
  0009c2ac: str r0,[r4,#0x64]
  0009c2ae: mov r0,r2
  0009c2b0: blx r5
  0009c2b2: ldr r1,[0x0009c378]
  0009c2b4: ldr r2,[r4,#0x4]
  0009c2b6: add r1,pc
  0009c2b8: str r0,[r4,#0x6c]
  0009c2ba: mov r0,r2
  0009c2bc: blx r5
  0009c2be: ldr r1,[0x0009c37c]
  0009c2c0: ldr r2,[r4,#0x4]
  0009c2c2: add r1,pc
  0009c2c4: str r0,[r4,#0x70]
  0009c2c6: mov r0,r2
  0009c2c8: blx r5
  0009c2ca: ldr r1,[0x0009c380]
  0009c2cc: ldr r2,[r4,#0x4]
  0009c2ce: add r1,pc
  0009c2d0: str r0,[r4,#0x74]
  0009c2d2: mov r0,r2
  0009c2d4: blx r5
  0009c2d6: ldr r1,[0x0009c384]
  0009c2d8: ldr r2,[r4,#0x4]
  0009c2da: add r1,pc
  0009c2dc: str r0,[r4,#0x78]
  0009c2de: mov r0,r2
  0009c2e0: blx r5
  0009c2e2: ldr r1,[0x0009c388]
  0009c2e4: ldr r2,[r4,#0x4]
  0009c2e6: add r1,pc
  0009c2e8: str r0,[r4,#0x7c]
  0009c2ea: mov r0,r2
  0009c2ec: blx r5
  0009c2ee: ldr r1,[r4,#0x4]
  0009c2f0: str.w r0,[r4,#0x80]
  0009c2f4: mov r0,r1
  0009c2f6: blx 0x00070960
  0009c2fa: add.w r5,r4,#0x48
  0009c2fe: movs r4,#0x0
  0009c300: b 0x0009c312
  0009c302: ldr.w r0,[r5,r4,lsl #0x2]
  0009c306: cmp r0,#0x0
  0009c308: blt 0x0009c310
  0009c30a: mov r1,r4
  0009c30c: blx 0x0007096c
  0009c310: adds r4,#0x1
  0009c312: cmp r4,#0x3
  0009c314: bne 0x0009c302
  0009c316: pop {r4,r5,r7,pc}
```
