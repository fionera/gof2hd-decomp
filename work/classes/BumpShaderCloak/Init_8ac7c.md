# BumpShaderCloak::Init
elf 0x8ac7c body 416
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpShaderCloak::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpShaderCloak::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009ae1c + 0x9ac90));
  iVar2 = DAT_0009ae28;
  puVar3 = (undefined4 *)(DAT_0009ae24 + 0x9ac9c);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9aca0);
  iVar2 = DAT_0009ae2c + 0x9acaa;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae30 + 0x9acb6;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae34 + 0x9acc2;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae38 + 0x9acce;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009ae3c + 0x9acda);
  iVar2 = DAT_0009ae40 + 0x9acde;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae44 + 0x9acec;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae48 + 0x9acf8;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae4c + 0x9ad04;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae50 + 0x9ad10;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae54 + 0x9ad1c;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae58 + 0x9ad28;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae5c + 0x9ad34;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae60 + 0x9ad40;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae64 + 0x9ad4c;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae68 + 0x9ad58;
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae6c + 0x9ad64;
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae70 + 0x9ad70;
  *(undefined4 *)(param_1 + 0x6c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae74 + 0x9ad7c;
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae78 + 0x9ad88;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae7c + 0x9ad94;
  *(undefined4 *)(param_1 + 0x70) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae80 + 0x9ada0;
  *(undefined4 *)(param_1 + 0x74) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae84 + 0x9adac;
  *(undefined4 *)(param_1 + 0x78) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x8c) = uVar1;
  glActiveTexture(0x84c7);
  Engine::ActivateRefractFBO();
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),DAT_0009ae88 + 0x9adca);
  iVar2 = DAT_0009ae8c + 0x9add2;
  *(undefined4 *)(param_1 + 0x80) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009ae90 + 0x9ade0;
  *(undefined4 *)(param_1 + 0x84) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x90) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  pcVar4 = *(code **)(DAT_0009ae94 + 0x9adfa);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x48),0);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x4c),1);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x50),6);
  (*pcVar4)(*(undefined4 *)(param_1 + 0x90),7);
  return;
}

```

## target disasm
```
  0009ac7c: push {r4,r5,r6,r7,lr}
  0009ac7e: add r7,sp,#0xc
  0009ac80: push.w r11
  0009ac84: mov r5,r1
  0009ac86: ldr r1,[0x0009ae1c]
  0009ac88: ldr r2,[0x0009ae20]
  0009ac8a: mov r4,r0
  0009ac8c: add r1,pc
  0009ac8e: add r2,pc
  0009ac90: blx 0x00070954
  0009ac94: ldr r2,[0x0009ae24]
  0009ac96: ldr r1,[0x0009ae28]
  0009ac98: add r2,pc
  0009ac9a: str r0,[r4,#0x4]
  0009ac9c: add r1,pc
  0009ac9e: ldr r6,[r2,#0x0]
  0009aca0: blx r6
  0009aca2: ldr r1,[0x0009ae2c]
  0009aca4: ldr r2,[r4,#0x4]
  0009aca6: add r1,pc
  0009aca8: str r0,[r4,#0x20]
  0009acaa: mov r0,r2
  0009acac: blx r6
  0009acae: ldr r1,[0x0009ae30]
  0009acb0: ldr r2,[r4,#0x4]
  0009acb2: add r1,pc
  0009acb4: str r0,[r4,#0x24]
  0009acb6: mov r0,r2
  0009acb8: blx r6
  0009acba: ldr r1,[0x0009ae34]
  0009acbc: ldr r2,[r4,#0x4]
  0009acbe: add r1,pc
  0009acc0: str r0,[r4,#0x28]
  0009acc2: mov r0,r2
  0009acc4: blx r6
  0009acc6: ldr r1,[0x0009ae38]
  0009acc8: ldr r2,[r4,#0x4]
  0009acca: add r1,pc
  0009accc: str r0,[r4,#0x2c]
  0009acce: mov r0,r2
  0009acd0: blx r6
  0009acd2: ldr r3,[0x0009ae3c]
  0009acd4: ldr r1,[0x0009ae40]
  0009acd6: add r3,pc
  0009acd8: ldr r2,[r4,#0x4]
  0009acda: add r1,pc
  0009acdc: str r0,[r4,#0x30]
  0009acde: ldr r6,[r3,#0x0]
  0009ace0: mov r0,r2
  0009ace2: blx r6
  0009ace4: ldr r1,[0x0009ae44]
  0009ace6: ldr r2,[r4,#0x4]
  0009ace8: add r1,pc
  0009acea: str r0,[r4,#0x34]
  0009acec: mov r0,r2
  0009acee: blx r6
  0009acf0: ldr r1,[0x0009ae48]
  0009acf2: ldr r2,[r4,#0x4]
  0009acf4: add r1,pc
  0009acf6: str r0,[r4,#0x38]
  0009acf8: mov r0,r2
  0009acfa: blx r6
  0009acfc: ldr r1,[0x0009ae4c]
  0009acfe: ldr r2,[r4,#0x4]
  0009ad00: add r1,pc
  0009ad02: str r0,[r4,#0x3c]
  0009ad04: mov r0,r2
  0009ad06: blx r6
  0009ad08: ldr r1,[0x0009ae50]
  0009ad0a: ldr r2,[r4,#0x4]
  0009ad0c: add r1,pc
  0009ad0e: str r0,[r4,#0x40]
  0009ad10: mov r0,r2
  0009ad12: blx r6
  0009ad14: ldr r1,[0x0009ae54]
  0009ad16: ldr r2,[r4,#0x4]
  0009ad18: add r1,pc
  0009ad1a: str r0,[r4,#0x44]
  0009ad1c: mov r0,r2
  0009ad1e: blx r6
  0009ad20: ldr r1,[0x0009ae58]
  0009ad22: ldr r2,[r4,#0x4]
  0009ad24: add r1,pc
  0009ad26: str r0,[r4,#0x48]
  0009ad28: mov r0,r2
  0009ad2a: blx r6
  0009ad2c: ldr r1,[0x0009ae5c]
  0009ad2e: ldr r2,[r4,#0x4]
  0009ad30: add r1,pc
  0009ad32: str r0,[r4,#0x4c]
  0009ad34: mov r0,r2
  0009ad36: blx r6
  0009ad38: ldr r1,[0x0009ae60]
  0009ad3a: ldr r2,[r4,#0x4]
  0009ad3c: add r1,pc
  0009ad3e: str r0,[r4,#0x50]
  0009ad40: mov r0,r2
  0009ad42: blx r6
  0009ad44: ldr r1,[0x0009ae64]
  0009ad46: ldr r2,[r4,#0x4]
  0009ad48: add r1,pc
  0009ad4a: str r0,[r4,#0x58]
  0009ad4c: mov r0,r2
  0009ad4e: blx r6
  0009ad50: ldr r1,[0x0009ae68]
  0009ad52: ldr r2,[r4,#0x4]
  0009ad54: add r1,pc
  0009ad56: str r0,[r4,#0x5c]
  0009ad58: mov r0,r2
  0009ad5a: blx r6
  0009ad5c: ldr r1,[0x0009ae6c]
  0009ad5e: ldr r2,[r4,#0x4]
  0009ad60: add r1,pc
  0009ad62: str r0,[r4,#0x64]
  0009ad64: mov r0,r2
  0009ad66: blx r6
  0009ad68: ldr r1,[0x0009ae70]
  0009ad6a: ldr r2,[r4,#0x4]
  0009ad6c: add r1,pc
  0009ad6e: str r0,[r4,#0x6c]
  0009ad70: mov r0,r2
  0009ad72: blx r6
  0009ad74: ldr r1,[0x0009ae74]
  0009ad76: ldr r2,[r4,#0x4]
  0009ad78: add r1,pc
  0009ad7a: str r0,[r4,#0x60]
  0009ad7c: mov r0,r2
  0009ad7e: blx r6
  0009ad80: ldr r1,[0x0009ae78]
  0009ad82: ldr r2,[r4,#0x4]
  0009ad84: add r1,pc
  0009ad86: str r0,[r4,#0x68]
  0009ad88: mov r0,r2
  0009ad8a: blx r6
  0009ad8c: ldr r1,[0x0009ae7c]
  0009ad8e: ldr r2,[r4,#0x4]
  0009ad90: add r1,pc
  0009ad92: str r0,[r4,#0x70]
  0009ad94: mov r0,r2
  0009ad96: blx r6
  0009ad98: ldr r1,[0x0009ae80]
  0009ad9a: ldr r2,[r4,#0x4]
  0009ad9c: add r1,pc
  0009ad9e: str r0,[r4,#0x74]
  0009ada0: mov r0,r2
  0009ada2: blx r6
  0009ada4: ldr r1,[0x0009ae84]
  0009ada6: ldr r2,[r4,#0x4]
  0009ada8: add r1,pc
  0009adaa: str r0,[r4,#0x78]
  0009adac: mov r0,r2
  0009adae: blx r6
  0009adb0: str.w r0,[r4,#0x8c]
  0009adb4: movw r0,#0x84c7
  0009adb8: blx 0x0006fb8c
  0009adbc: mov r0,r5
  0009adbe: blx 0x00070c3c
  0009adc2: ldr r1,[0x0009ae88]
  0009adc4: ldr r0,[r4,#0x4]
  0009adc6: add r1,pc
  0009adc8: blx r6
  0009adca: ldr r1,[0x0009ae8c]
  0009adcc: ldr r2,[r4,#0x4]
  0009adce: add r1,pc
  0009add0: str.w r0,[r4,#0x80]
  0009add4: mov r0,r2
  0009add6: blx r6
  0009add8: ldr r1,[0x0009ae90]
  0009adda: ldr r2,[r4,#0x4]
  0009addc: add r1,pc
  0009adde: str.w r0,[r4,#0x84]
  0009ade2: mov r0,r2
  0009ade4: blx r6
  0009ade6: ldr r1,[r4,#0x4]
  0009ade8: str.w r0,[r4,#0x90]
  0009adec: mov r0,r1
  0009adee: blx 0x00070960
  0009adf2: ldr r0,[0x0009ae94]
  0009adf4: movs r1,#0x0
  0009adf6: add r0,pc
  0009adf8: ldr r5,[r0,#0x0]
  0009adfa: ldr r0,[r4,#0x48]
  0009adfc: blx r5
  0009adfe: ldr r0,[r4,#0x4c]
  0009ae00: movs r1,#0x1
  0009ae02: blx r5
  0009ae04: ldr r0,[r4,#0x50]
  0009ae06: movs r1,#0x6
  0009ae08: blx r5
  0009ae0a: ldr.w r0,[r4,#0x90]
  0009ae0e: movs r1,#0x7
  0009ae10: mov r2,r5
  0009ae12: pop.w r11
  0009ae16: pop.w {r4,r5,r6,r7,lr}
  0009ae1a: bx r2
```
