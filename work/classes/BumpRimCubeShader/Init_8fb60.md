# BumpRimCubeShader::Init
elf 0x8fb60 body 476
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::BumpRimCubeShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::BumpRimCubeShader::Init(Engine *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  iVar1 = ShaderBaseStruct::LoadBindShader((char *)param_1,(char *)(DAT_0009fd3c + 0x9fb72));
  *(int *)(param_1 + 4) = iVar1;
  if (iVar1 == 0) {
    iVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009fd44 + 0x9fb86));
    *(int *)(param_1 + 4) = iVar1;
  }
  pcVar4 = *(code **)(DAT_0009fd4c + 0x9fb94);
  uVar2 = (*pcVar4)(iVar1,DAT_0009fd50 + 0x9fb96);
  iVar1 = DAT_0009fd54 + 0x9fba0;
  *(undefined4 *)(param_1 + 0x20) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd58 + 0x9fbac;
  *(undefined4 *)(param_1 + 0x24) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd5c + 0x9fbb8;
  *(undefined4 *)(param_1 + 0x28) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd60 + 0x9fbc4;
  *(undefined4 *)(param_1 + 0x2c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  puVar3 = (undefined4 *)(DAT_0009fd64 + 0x9fbd0);
  iVar1 = DAT_0009fd68 + 0x9fbd4;
  *(undefined4 *)(param_1 + 0x30) = uVar2;
  pcVar4 = (code *)*puVar3;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd6c + 0x9fbe2;
  *(undefined4 *)(param_1 + 0x34) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd70 + 0x9fbee;
  *(undefined4 *)(param_1 + 0x38) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd74 + 0x9fbfa;
  *(undefined4 *)(param_1 + 0x3c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd78 + 0x9fc06;
  *(undefined4 *)(param_1 + 0x40) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd7c + 0x9fc12;
  *(undefined4 *)(param_1 + 0x44) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd80 + 0x9fc1e;
  *(undefined4 *)(param_1 + 0x48) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd84 + 0x9fc2a;
  *(undefined4 *)(param_1 + 0x4c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd88 + 0x9fc36;
  *(undefined4 *)(param_1 + 0x50) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd8c + 0x9fc42;
  *(undefined4 *)(param_1 + 0x54) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd90 + 0x9fc4e;
  *(undefined4 *)(param_1 + 0x58) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd94 + 0x9fc5a;
  *(undefined4 *)(param_1 + 0x5c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd98 + 0x9fc66;
  *(undefined4 *)(param_1 + 100) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fd9c + 0x9fc72;
  *(undefined4 *)(param_1 + 0x6c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fda0 + 0x9fc7e;
  *(undefined4 *)(param_1 + 0x60) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fda4 + 0x9fc8a;
  *(undefined4 *)(param_1 + 0x68) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fda8 + 0x9fc96;
  *(undefined4 *)(param_1 + 0x70) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdac + 0x9fca2;
  *(undefined4 *)(param_1 + 0x74) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdb0 + 0x9fcae;
  *(undefined4 *)(param_1 + 0x78) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdb4 + 0x9fcba;
  *(undefined4 *)(param_1 + 0x7c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdb8 + 0x9fcc6;
  *(undefined4 *)(param_1 + 0x80) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdbc + 0x9fcd4;
  *(undefined4 *)(param_1 + 0x84) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdc0 + 0x9fce2;
  *(undefined4 *)(param_1 + 0x88) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdc4 + 0x9fcf0;
  *(undefined4 *)(param_1 + 0x8c) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  iVar1 = DAT_0009fdc8 + 0x9fcfe;
  *(undefined4 *)(param_1 + 0x90) = uVar2;
  uVar2 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar1);
  *(undefined4 *)(param_1 + 0x94) = uVar2;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar1 = 0; iVar1 != 2; iVar1 = iVar1 + 1) {
    if (-1 < *(int *)(param_1 + iVar1 * 4 + 0x4c)) {
      glUniform1i(*(int *)(param_1 + iVar1 * 4 + 0x4c),iVar1);
    }
  }
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x54),7);
  return;
}

```

## target disasm
```
  0009fb60: push {r4,r5,r6,r7,lr}
  0009fb62: add r7,sp,#0xc
  0009fb64: push.w r11
  0009fb68: ldr r1,[0x0009fd3c]
  0009fb6a: mov r4,r0
  0009fb6c: ldr r2,[0x0009fd40]
  0009fb6e: add r1,pc
  0009fb70: add r2,pc
  0009fb72: blx 0x00070948
  0009fb76: cmp r0,#0x0
  0009fb78: str r0,[r4,#0x4]
  0009fb7a: bne 0x0009fb8c
  0009fb7c: ldr r1,[0x0009fd44]
  0009fb7e: mov r0,r4
  0009fb80: ldr r2,[0x0009fd48]
  0009fb82: add r1,pc
  0009fb84: add r2,pc
  0009fb86: blx 0x00070954
  0009fb8a: str r0,[r4,#0x4]
  0009fb8c: ldr r2,[0x0009fd4c]
  0009fb8e: ldr r1,[0x0009fd50]
  0009fb90: add r2,pc
  0009fb92: add r1,pc
  0009fb94: ldr r5,[r2,#0x0]
  0009fb96: blx r5
  0009fb98: ldr r1,[0x0009fd54]
  0009fb9a: ldr r2,[r4,#0x4]
  0009fb9c: add r1,pc
  0009fb9e: str r0,[r4,#0x20]
  0009fba0: mov r0,r2
  0009fba2: blx r5
  0009fba4: ldr r1,[0x0009fd58]
  0009fba6: ldr r2,[r4,#0x4]
  0009fba8: add r1,pc
  0009fbaa: str r0,[r4,#0x24]
  0009fbac: mov r0,r2
  0009fbae: blx r5
  0009fbb0: ldr r1,[0x0009fd5c]
  0009fbb2: ldr r2,[r4,#0x4]
  0009fbb4: add r1,pc
  0009fbb6: str r0,[r4,#0x28]
  0009fbb8: mov r0,r2
  0009fbba: blx r5
  0009fbbc: ldr r1,[0x0009fd60]
  0009fbbe: ldr r2,[r4,#0x4]
  0009fbc0: add r1,pc
  0009fbc2: str r0,[r4,#0x2c]
  0009fbc4: mov r0,r2
  0009fbc6: blx r5
  0009fbc8: ldr r3,[0x0009fd64]
  0009fbca: ldr r1,[0x0009fd68]
  0009fbcc: add r3,pc
  0009fbce: ldr r2,[r4,#0x4]
  0009fbd0: add r1,pc
  0009fbd2: str r0,[r4,#0x30]
  0009fbd4: ldr r5,[r3,#0x0]
  0009fbd6: mov r0,r2
  0009fbd8: blx r5
  0009fbda: ldr r1,[0x0009fd6c]
  0009fbdc: ldr r2,[r4,#0x4]
  0009fbde: add r1,pc
  0009fbe0: str r0,[r4,#0x34]
  0009fbe2: mov r0,r2
  0009fbe4: blx r5
  0009fbe6: ldr r1,[0x0009fd70]
  0009fbe8: ldr r2,[r4,#0x4]
  0009fbea: add r1,pc
  0009fbec: str r0,[r4,#0x38]
  0009fbee: mov r0,r2
  0009fbf0: blx r5
  0009fbf2: ldr r1,[0x0009fd74]
  0009fbf4: ldr r2,[r4,#0x4]
  0009fbf6: add r1,pc
  0009fbf8: str r0,[r4,#0x3c]
  0009fbfa: mov r0,r2
  0009fbfc: blx r5
  0009fbfe: ldr r1,[0x0009fd78]
  0009fc00: ldr r2,[r4,#0x4]
  0009fc02: add r1,pc
  0009fc04: str r0,[r4,#0x40]
  0009fc06: mov r0,r2
  0009fc08: blx r5
  0009fc0a: ldr r1,[0x0009fd7c]
  0009fc0c: ldr r2,[r4,#0x4]
  0009fc0e: add r1,pc
  0009fc10: str r0,[r4,#0x44]
  0009fc12: mov r0,r2
  0009fc14: blx r5
  0009fc16: ldr r1,[0x0009fd80]
  0009fc18: ldr r2,[r4,#0x4]
  0009fc1a: add r1,pc
  0009fc1c: str r0,[r4,#0x48]
  0009fc1e: mov r0,r2
  0009fc20: blx r5
  0009fc22: ldr r1,[0x0009fd84]
  0009fc24: ldr r2,[r4,#0x4]
  0009fc26: add r1,pc
  0009fc28: str r0,[r4,#0x4c]
  0009fc2a: mov r0,r2
  0009fc2c: blx r5
  0009fc2e: ldr r1,[0x0009fd88]
  0009fc30: ldr r2,[r4,#0x4]
  0009fc32: add r1,pc
  0009fc34: str r0,[r4,#0x50]
  0009fc36: mov r0,r2
  0009fc38: blx r5
  0009fc3a: ldr r1,[0x0009fd8c]
  0009fc3c: ldr r2,[r4,#0x4]
  0009fc3e: add r1,pc
  0009fc40: str r0,[r4,#0x54]
  0009fc42: mov r0,r2
  0009fc44: blx r5
  0009fc46: ldr r1,[0x0009fd90]
  0009fc48: ldr r2,[r4,#0x4]
  0009fc4a: add r1,pc
  0009fc4c: str r0,[r4,#0x58]
  0009fc4e: mov r0,r2
  0009fc50: blx r5
  0009fc52: ldr r1,[0x0009fd94]
  0009fc54: ldr r2,[r4,#0x4]
  0009fc56: add r1,pc
  0009fc58: str r0,[r4,#0x5c]
  0009fc5a: mov r0,r2
  0009fc5c: blx r5
  0009fc5e: ldr r1,[0x0009fd98]
  0009fc60: ldr r2,[r4,#0x4]
  0009fc62: add r1,pc
  0009fc64: str r0,[r4,#0x64]
  0009fc66: mov r0,r2
  0009fc68: blx r5
  0009fc6a: ldr r1,[0x0009fd9c]
  0009fc6c: ldr r2,[r4,#0x4]
  0009fc6e: add r1,pc
  0009fc70: str r0,[r4,#0x6c]
  0009fc72: mov r0,r2
  0009fc74: blx r5
  0009fc76: ldr r1,[0x0009fda0]
  0009fc78: ldr r2,[r4,#0x4]
  0009fc7a: add r1,pc
  0009fc7c: str r0,[r4,#0x60]
  0009fc7e: mov r0,r2
  0009fc80: blx r5
  0009fc82: ldr r1,[0x0009fda4]
  0009fc84: ldr r2,[r4,#0x4]
  0009fc86: add r1,pc
  0009fc88: str r0,[r4,#0x68]
  0009fc8a: mov r0,r2
  0009fc8c: blx r5
  0009fc8e: ldr r1,[0x0009fda8]
  0009fc90: ldr r2,[r4,#0x4]
  0009fc92: add r1,pc
  0009fc94: str r0,[r4,#0x70]
  0009fc96: mov r0,r2
  0009fc98: blx r5
  0009fc9a: ldr r1,[0x0009fdac]
  0009fc9c: ldr r2,[r4,#0x4]
  0009fc9e: add r1,pc
  0009fca0: str r0,[r4,#0x74]
  0009fca2: mov r0,r2
  0009fca4: blx r5
  0009fca6: ldr r1,[0x0009fdb0]
  0009fca8: ldr r2,[r4,#0x4]
  0009fcaa: add r1,pc
  0009fcac: str r0,[r4,#0x78]
  0009fcae: mov r0,r2
  0009fcb0: blx r5
  0009fcb2: ldr r1,[0x0009fdb4]
  0009fcb4: ldr r2,[r4,#0x4]
  0009fcb6: add r1,pc
  0009fcb8: str r0,[r4,#0x7c]
  0009fcba: mov r0,r2
  0009fcbc: blx r5
  0009fcbe: ldr r1,[0x0009fdb8]
  0009fcc0: ldr r2,[r4,#0x4]
  0009fcc2: add r1,pc
  0009fcc4: str.w r0,[r4,#0x80]
  0009fcc8: mov r0,r2
  0009fcca: blx r5
  0009fccc: ldr r1,[0x0009fdbc]
  0009fcce: ldr r2,[r4,#0x4]
  0009fcd0: add r1,pc
  0009fcd2: str.w r0,[r4,#0x84]
  0009fcd6: mov r0,r2
  0009fcd8: blx r5
  0009fcda: ldr r1,[0x0009fdc0]
  0009fcdc: ldr r2,[r4,#0x4]
  0009fcde: add r1,pc
  0009fce0: str.w r0,[r4,#0x88]
  0009fce4: mov r0,r2
  0009fce6: blx r5
  0009fce8: ldr r1,[0x0009fdc4]
  0009fcea: ldr r2,[r4,#0x4]
  0009fcec: add r1,pc
  0009fcee: str.w r0,[r4,#0x8c]
  0009fcf2: mov r0,r2
  0009fcf4: blx r5
  0009fcf6: ldr r1,[0x0009fdc8]
  0009fcf8: ldr r2,[r4,#0x4]
  0009fcfa: add r1,pc
  0009fcfc: str.w r0,[r4,#0x90]
  0009fd00: mov r0,r2
  0009fd02: blx r5
  0009fd04: ldr r1,[r4,#0x4]
  0009fd06: str.w r0,[r4,#0x94]
  0009fd0a: mov r0,r1
  0009fd0c: blx 0x00070960
  0009fd10: add.w r6,r4,#0x4c
  0009fd14: movs r5,#0x0
  0009fd16: b 0x0009fd28
  0009fd18: ldr.w r0,[r6,r5,lsl #0x2]
  0009fd1c: cmp r0,#0x0
  0009fd1e: blt 0x0009fd26
  0009fd20: mov r1,r5
  0009fd22: blx 0x0007096c
  0009fd26: adds r5,#0x1
  0009fd28: cmp r5,#0x2
  0009fd2a: bne 0x0009fd18
  0009fd2c: ldr r0,[r4,#0x54]
  0009fd2e: movs r1,#0x7
  0009fd30: pop.w r11
  0009fd34: pop.w {r4,r5,r6,r7,lr}
  0009fd38: b.w 0x001ab768
```
