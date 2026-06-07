# GlowPPShader::Init
elf 0x8cf20 body 368
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::GlowPPShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::GlowPPShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  int iVar7;
  code *pcVar8;
  code *pcVar9;
  code *pcVar10;
  code *pcVar11;
  
  iVar7 = DAT_0009d094 + 0x9cf36;
  pcVar9 = *(code **)(DAT_0009d090 + 0x9cf32);
  uVar1 = (*pcVar9)(param_1,iVar7,DAT_0009d098 + 0x9cf3a);
  iVar4 = DAT_0009d09c;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar4 + 0x9cf4a);
  iVar4 = DAT_0009d0a0;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar4 + 0x9cf56);
  iVar4 = DAT_0009d0a4;
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  uVar1 = (*pcVar9)(param_1,iVar7,iVar4 + 0x9cf62);
  puVar6 = (undefined4 *)(DAT_0009d0a8 + 0x9cf6a);
  iVar7 = DAT_0009d0ac + 0x9cf6e;
  *(undefined4 *)(param_1 + 0x78) = uVar1;
  pcVar11 = (code *)*puVar6;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x20));
  iVar2 = DAT_0009d0b0 + 0x9cf80;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x20));
  iVar3 = DAT_0009d0b8 + 0x9cf90;
  pcVar10 = *(code **)(DAT_0009d0b4 + 0x9cf8e);
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x20));
  iVar4 = DAT_0009d0bc;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  iVar4 = iVar4 + 0x9cfa4;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x20));
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  pcVar8 = *(code **)(DAT_0009d0c0 + 0x9cfae);
  (*pcVar8)(*(undefined4 *)(param_1 + 0x20));
  pcVar9 = *(code **)(DAT_0009d0c4 + 0x9cfba);
  (*pcVar9)(*(undefined4 *)(param_1 + 0x30),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x40),iVar7);
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x40),iVar2);
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x40),iVar3);
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x40),iVar4);
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  iVar5 = DAT_0009d0c8 + 0x9cfee;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x40));
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  (*pcVar8)(*(undefined4 *)(param_1 + 0x40));
  (*pcVar9)(*(undefined4 *)(param_1 + 0x50),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x5c),iVar7);
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x5c),iVar2);
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x5c),iVar3);
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x5c),iVar4);
  *(undefined4 *)(param_1 + 0x6c) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x5c),iVar5);
  *(undefined4 *)(param_1 + 0x70) = uVar1;
  (*pcVar8)(*(undefined4 *)(param_1 + 0x5c));
  (*pcVar9)(*(undefined4 *)(param_1 + 0x6c),0);
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x78),iVar7);
  *(undefined4 *)(param_1 + 0x7c) = uVar1;
  uVar1 = (*pcVar11)(*(undefined4 *)(param_1 + 0x78),iVar2);
  *(undefined4 *)(param_1 + 0x84) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x78),iVar3);
  *(undefined4 *)(param_1 + 0x80) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x78),iVar4);
  iVar4 = DAT_0009d0cc + 0x9d062;
  *(undefined4 *)(param_1 + 0x88) = uVar1;
  uVar1 = (*pcVar10)(*(undefined4 *)(param_1 + 0x78),iVar4);
  *(undefined4 *)(param_1 + 0x8c) = uVar1;
  (*pcVar8)(*(undefined4 *)(param_1 + 0x78));
  (*pcVar9)(*(undefined4 *)(param_1 + 0x88),0);
  (*pcVar9)(*(undefined4 *)(param_1 + 0x8c),1);
  return;
}

```

## target disasm
```
  0009cf20: push {r4,r5,r6,r7,lr}
  0009cf22: add r7,sp,#0xc
  0009cf24: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  0009cf28: mov r4,r0
  0009cf2a: ldr r0,[0x0009d090]
  0009cf2c: ldr r5,[0x0009d094]
  0009cf2e: add r0,pc
  0009cf30: ldr r2,[0x0009d098]
  0009cf32: add r5,pc
  0009cf34: ldr r6,[r0,#0x0]
  0009cf36: add r2,pc
  0009cf38: mov r0,r4
  0009cf3a: mov r1,r5
  0009cf3c: blx r6
  0009cf3e: ldr r2,[0x0009d09c]
  0009cf40: mov r1,r5
  0009cf42: str r0,[r4,#0x20]
  0009cf44: mov r0,r4
  0009cf46: add r2,pc
  0009cf48: blx r6
  0009cf4a: ldr r2,[0x0009d0a0]
  0009cf4c: mov r1,r5
  0009cf4e: str r0,[r4,#0x40]
  0009cf50: mov r0,r4
  0009cf52: add r2,pc
  0009cf54: blx r6
  0009cf56: ldr r2,[0x0009d0a4]
  0009cf58: mov r1,r5
  0009cf5a: str r0,[r4,#0x5c]
  0009cf5c: mov r0,r4
  0009cf5e: add r2,pc
  0009cf60: blx r6
  0009cf62: ldr r2,[0x0009d0a8]
  0009cf64: ldr r1,[0x0009d0ac]
  0009cf66: add r2,pc
  0009cf68: ldr r3,[r4,#0x20]
  0009cf6a: add r1,pc
  0009cf6c: str r0,[r4,#0x78]
  0009cf6e: ldr.w r9,[r2,#0x0]
  0009cf72: mov r0,r3
  0009cf74: mov r10,r1
  0009cf76: blx r9
  0009cf78: ldr r1,[0x0009d0b0]
  0009cf7a: ldr r2,[r4,#0x20]
  0009cf7c: add r1,pc
  0009cf7e: str r0,[r4,#0x24]
  0009cf80: mov r0,r2
  0009cf82: mov r11,r1
  0009cf84: blx r9
  0009cf86: ldr r2,[0x0009d0b4]
  0009cf88: ldr r1,[0x0009d0b8]
  0009cf8a: add r2,pc
  0009cf8c: add r1,pc
  0009cf8e: ldr r6,[r2,#0x0]
  0009cf90: ldr r2,[r4,#0x20]
  0009cf92: mov r8,r1
  0009cf94: str r0,[r4,#0x2c]
  0009cf96: mov r0,r2
  0009cf98: blx r6
  0009cf9a: ldr r1,[0x0009d0bc]
  0009cf9c: str r0,[r4,#0x28]
  0009cf9e: ldr r0,[r4,#0x20]
  0009cfa0: add r1,pc
  0009cfa2: str r1,[sp,#0xc]
  0009cfa4: blx r6
  0009cfa6: str r0,[r4,#0x30]
  0009cfa8: ldr r0,[0x0009d0c0]
  0009cfaa: add r0,pc
  0009cfac: ldr r5,[r0,#0x0]
  0009cfae: ldr r0,[r4,#0x20]
  0009cfb0: blx r5
  0009cfb2: ldr r0,[0x0009d0c4]
  0009cfb4: movs r1,#0x0
  0009cfb6: add r0,pc
  0009cfb8: ldr r0,[r0,#0x0]
  0009cfba: str r0,[sp,#0x10]
  0009cfbc: ldr r0,[r4,#0x30]
  0009cfbe: ldr r2,[sp,#0x10]
  0009cfc0: blx r2
  0009cfc2: ldr r0,[r4,#0x40]
  0009cfc4: mov r1,r10
  0009cfc6: blx r9
  0009cfc8: str r0,[r4,#0x44]
  0009cfca: mov r1,r11
  0009cfcc: ldr r0,[r4,#0x40]
  0009cfce: blx r9
  0009cfd0: str r0,[r4,#0x4c]
  0009cfd2: mov r1,r8
  0009cfd4: ldr r0,[r4,#0x40]
  0009cfd6: str.w r8,[sp,#0x8]
  0009cfda: blx r6
  0009cfdc: str r0,[r4,#0x48]
  0009cfde: ldr r0,[r4,#0x40]
  0009cfe0: ldr r1,[sp,#0xc]
  0009cfe2: blx r6
  0009cfe4: str r0,[r4,#0x50]
  0009cfe6: ldr r1,[0x0009d0c8]
  0009cfe8: ldr r0,[r4,#0x40]
  0009cfea: add r1,pc
  0009cfec: str r1,[sp,#0x4]
  0009cfee: blx r6
  0009cff0: str r0,[r4,#0x54]
  0009cff2: ldr r0,[r4,#0x40]
  0009cff4: blx r5
  0009cff6: ldr r0,[r4,#0x50]
  0009cff8: movs r1,#0x0
  0009cffa: ldr r2,[sp,#0x10]
  0009cffc: blx r2
  0009cffe: ldr r0,[r4,#0x5c]
  0009d000: mov r1,r10
  0009d002: blx r9
  0009d004: str r0,[r4,#0x60]
  0009d006: mov r1,r11
  0009d008: ldr r0,[r4,#0x5c]
  0009d00a: blx r9
  0009d00c: str r0,[r4,#0x68]
  0009d00e: mov r1,r8
  0009d010: ldr r0,[r4,#0x5c]
  0009d012: blx r6
  0009d014: str r0,[r4,#0x64]
  0009d016: ldr.w r8,[sp,#0xc]
  0009d01a: ldr r0,[r4,#0x5c]
  0009d01c: mov r1,r8
  0009d01e: blx r6
  0009d020: str r0,[r4,#0x6c]
  0009d022: ldr r0,[r4,#0x5c]
  0009d024: ldr r1,[sp,#0x4]
  0009d026: blx r6
  0009d028: str r0,[r4,#0x70]
  0009d02a: ldr r0,[r4,#0x5c]
  0009d02c: blx r5
  0009d02e: ldr r0,[r4,#0x6c]
  0009d030: movs r1,#0x0
  0009d032: ldr r2,[sp,#0x10]
  0009d034: blx r2
  0009d036: ldr r0,[r4,#0x78]
  0009d038: mov r1,r10
  0009d03a: blx r9
  0009d03c: str r0,[r4,#0x7c]
  0009d03e: mov r1,r11
  0009d040: ldr r0,[r4,#0x78]
  0009d042: blx r9
  0009d044: str.w r0,[r4,#0x84]
  0009d048: ldr r0,[r4,#0x78]
  0009d04a: ldr r1,[sp,#0x8]
  0009d04c: blx r6
  0009d04e: ldr r1,[r4,#0x78]
  0009d050: str.w r0,[r4,#0x80]
  0009d054: mov r0,r1
  0009d056: mov r1,r8
  0009d058: blx r6
  0009d05a: ldr r1,[0x0009d0cc]
  0009d05c: ldr r2,[r4,#0x78]
  0009d05e: add r1,pc
  0009d060: str.w r0,[r4,#0x88]
  0009d064: mov r0,r2
  0009d066: blx r6
  0009d068: ldr r1,[r4,#0x78]
  0009d06a: str.w r0,[r4,#0x8c]
  0009d06e: mov r0,r1
  0009d070: blx r5
  0009d072: ldr.w r0,[r4,#0x88]
  0009d076: movs r1,#0x0
  0009d078: ldr r2,[sp,#0x10]
  0009d07a: blx r2
  0009d07c: ldr.w r0,[r4,#0x8c]
  0009d080: movs r1,#0x1
  0009d082: ldr r2,[sp,#0x10]
  0009d084: add sp,#0x14
  0009d086: pop.w {r8,r9,r10,r11}
  0009d08a: pop.w {r4,r5,r6,r7,lr}
  0009d08e: bx r2
```
