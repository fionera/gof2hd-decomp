# DNSShader::Init
elf 0x8f360 body 264
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::DNSShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::DNSShader::Init(Engine *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  iVar1 = ShaderBaseStruct::LoadBindShader((char *)param_1,(char *)(DAT_0009f468 + 0x9f36e));
  *(int *)(param_1 + 4) = iVar1;
  if (iVar1 == 0) {
    iVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009f470 + 0x9f382));
    *(int *)(param_1 + 4) = iVar1;
  }
  pcVar4 = *(code **)(DAT_0009f478 + 0x9f390);
  uVar2 = (*pcVar4)(iVar1,DAT_0009f47c + 0x9f392);
  iVar1 = DAT_0009f480 + 0x9f39c;
  *(undefined4 *)(param_1 + 0x20) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f484 + 0x9f3a8;
  *(undefined4 *)(param_1 + 0x24) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f488 + 0x9f3b4;
  *(undefined4 *)(param_1 + 0x28) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f48c + 0x9f3c0;
  *(undefined4 *)(param_1 + 0x2c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  puVar3 = (undefined4 *)(DAT_0009f490 + 0x9f3cc);
  iVar1 = DAT_0009f494 + 0x9f3d0;
  *(undefined4 *)(param_1 + 0x30) = uVar2;
  pcVar4 = (code *)*puVar3;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f498 + 0x9f3de;
  *(undefined4 *)(param_1 + 0x34) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f49c + 0x9f3ea;
  *(undefined4 *)(param_1 + 0x38) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4a0 + 0x9f3f6;
  *(undefined4 *)(param_1 + 0x3c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4a4 + 0x9f402;
  *(undefined4 *)(param_1 + 0x40) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4a8 + 0x9f40e;
  *(undefined4 *)(param_1 + 0x44) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4ac + 0x9f41a;
  *(undefined4 *)(param_1 + 0x48) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4b0 + 0x9f426;
  *(undefined4 *)(param_1 + 0x4c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4b4 + 0x9f432;
  *(undefined4 *)(param_1 + 0x50) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4b8 + 0x9f43e;
  *(undefined4 *)(param_1 + 0x54) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4bc + 0x9f44a;
  *(undefined4 *)(param_1 + 0x58) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009f4c0 + 0x9f456;
  *(undefined4 *)(param_1 + 0x5c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  *(undefined4 *)(param_1 + 0x60) = uVar2;
  (*(code *)(DAT_001ab7a4 + 0x1ab7a8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  0009f360: push {r4,r5,r7,lr}
  0009f362: add r7,sp,#0x8
  0009f364: ldr r1,[0x0009f468]
  0009f366: mov r4,r0
  0009f368: ldr r2,[0x0009f46c]
  0009f36a: add r1,pc
  0009f36c: add r2,pc
  0009f36e: blx 0x00070948
  0009f372: cmp r0,#0x0
  0009f374: str r0,[r4,#0x4]
  0009f376: bne 0x0009f388
  0009f378: ldr r1,[0x0009f470]
  0009f37a: mov r0,r4
  0009f37c: ldr r2,[0x0009f474]
  0009f37e: add r1,pc
  0009f380: add r2,pc
  0009f382: blx 0x00070954
  0009f386: str r0,[r4,#0x4]
  0009f388: ldr r2,[0x0009f478]
  0009f38a: ldr r1,[0x0009f47c]
  0009f38c: add r2,pc
  0009f38e: add r1,pc
  0009f390: ldr r5,[r2,#0x0]
  0009f392: blx r5
  0009f394: ldr r1,[0x0009f480]
  0009f396: ldr r2,[r4,#0x4]
  0009f398: add r1,pc
  0009f39a: str r0,[r4,#0x20]
  0009f39c: mov r0,r2
  0009f39e: blx r5
  0009f3a0: ldr r1,[0x0009f484]
  0009f3a2: ldr r2,[r4,#0x4]
  0009f3a4: add r1,pc
  0009f3a6: str r0,[r4,#0x24]
  0009f3a8: mov r0,r2
  0009f3aa: blx r5
  0009f3ac: ldr r1,[0x0009f488]
  0009f3ae: ldr r2,[r4,#0x4]
  0009f3b0: add r1,pc
  0009f3b2: str r0,[r4,#0x28]
  0009f3b4: mov r0,r2
  0009f3b6: blx r5
  0009f3b8: ldr r1,[0x0009f48c]
  0009f3ba: ldr r2,[r4,#0x4]
  0009f3bc: add r1,pc
  0009f3be: str r0,[r4,#0x2c]
  0009f3c0: mov r0,r2
  0009f3c2: blx r5
  0009f3c4: ldr r3,[0x0009f490]
  0009f3c6: ldr r1,[0x0009f494]
  0009f3c8: add r3,pc
  0009f3ca: ldr r2,[r4,#0x4]
  0009f3cc: add r1,pc
  0009f3ce: str r0,[r4,#0x30]
  0009f3d0: ldr r5,[r3,#0x0]
  0009f3d2: mov r0,r2
  0009f3d4: blx r5
  0009f3d6: ldr r1,[0x0009f498]
  0009f3d8: ldr r2,[r4,#0x4]
  0009f3da: add r1,pc
  0009f3dc: str r0,[r4,#0x34]
  0009f3de: mov r0,r2
  0009f3e0: blx r5
  0009f3e2: ldr r1,[0x0009f49c]
  0009f3e4: ldr r2,[r4,#0x4]
  0009f3e6: add r1,pc
  0009f3e8: str r0,[r4,#0x38]
  0009f3ea: mov r0,r2
  0009f3ec: blx r5
  0009f3ee: ldr r1,[0x0009f4a0]
  0009f3f0: ldr r2,[r4,#0x4]
  0009f3f2: add r1,pc
  0009f3f4: str r0,[r4,#0x3c]
  0009f3f6: mov r0,r2
  0009f3f8: blx r5
  0009f3fa: ldr r1,[0x0009f4a4]
  0009f3fc: ldr r2,[r4,#0x4]
  0009f3fe: add r1,pc
  0009f400: str r0,[r4,#0x40]
  0009f402: mov r0,r2
  0009f404: blx r5
  0009f406: ldr r1,[0x0009f4a8]
  0009f408: ldr r2,[r4,#0x4]
  0009f40a: add r1,pc
  0009f40c: str r0,[r4,#0x44]
  0009f40e: mov r0,r2
  0009f410: blx r5
  0009f412: ldr r1,[0x0009f4ac]
  0009f414: ldr r2,[r4,#0x4]
  0009f416: add r1,pc
  0009f418: str r0,[r4,#0x48]
  0009f41a: mov r0,r2
  0009f41c: blx r5
  0009f41e: ldr r1,[0x0009f4b0]
  0009f420: ldr r2,[r4,#0x4]
  0009f422: add r1,pc
  0009f424: str r0,[r4,#0x4c]
  0009f426: mov r0,r2
  0009f428: blx r5
  0009f42a: ldr r1,[0x0009f4b4]
  0009f42c: ldr r2,[r4,#0x4]
  0009f42e: add r1,pc
  0009f430: str r0,[r4,#0x50]
  0009f432: mov r0,r2
  0009f434: blx r5
  0009f436: ldr r1,[0x0009f4b8]
  0009f438: ldr r2,[r4,#0x4]
  0009f43a: add r1,pc
  0009f43c: str r0,[r4,#0x54]
  0009f43e: mov r0,r2
  0009f440: blx r5
  0009f442: ldr r1,[0x0009f4bc]
  0009f444: ldr r2,[r4,#0x4]
  0009f446: add r1,pc
  0009f448: str r0,[r4,#0x58]
  0009f44a: mov r0,r2
  0009f44c: blx r5
  0009f44e: ldr r1,[0x0009f4c0]
  0009f450: ldr r2,[r4,#0x4]
  0009f452: add r1,pc
  0009f454: str r0,[r4,#0x5c]
  0009f456: mov r0,r2
  0009f458: blx r5
  0009f45a: ldr r1,[r4,#0x4]
  0009f45c: str r0,[r4,#0x60]
  0009f45e: mov r0,r1
  0009f460: pop.w {r4,r5,r7,lr}
  0009f464: b.w 0x001ab798
```
