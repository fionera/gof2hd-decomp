# TextureLightShader::Init
elf 0x8db68 body 296
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::TextureLightShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::TextureLightShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  code *pcVar5;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_0009dc90 + 0x9db7a));
  iVar2 = DAT_0009dc9c;
  puVar3 = (undefined4 *)(DAT_0009dc98 + 0x9db86);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0x9db8a);
  iVar2 = DAT_0009dca0 + 0x9db94;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_0009dca4 + 0x9dba0);
  iVar2 = DAT_0009dca8 + 0x9dba4;
  *(undefined4 *)(param_1 + 0x6c) = uVar1;
  pcVar5 = (code *)*puVar3;
  uVar1 = (*pcVar5)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcac + 0x9dbb2;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar5)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcb0 + 0x9dbbe;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar5)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcb4 + 0x9dbca;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcb8 + 0x9dbd6;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcbc + 0x9dbe2;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcc0 + 0x9dbee;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcc4 + 0x9dbfa;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcc8 + 0x9dc06;
  *(undefined4 *)(param_1 + 0x3c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dccc + 0x9dc12;
  *(undefined4 *)(param_1 + 100) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcd0 + 0x9dc1e;
  *(undefined4 *)(param_1 + 0x44) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcd4 + 0x9dc2a;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcd8 + 0x9dc36;
  *(undefined4 *)(param_1 + 0x48) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcdc + 0x9dc42;
  *(undefined4 *)(param_1 + 0x50) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dce0 + 0x9dc4e;
  *(undefined4 *)(param_1 + 0x58) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dce4 + 0x9dc5a;
  *(undefined4 *)(param_1 + 0x60) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dce8 + 0x9dc66;
  *(undefined4 *)(param_1 + 0x4c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_0009dcec + 0x9dc72;
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  (*(code *)(DAT_001ab774 + 0x1ab778))(*(undefined4 *)(param_1 + 0x44),0);
  return;
}

```

## target disasm
```
  0009db68: push {r4,r5,r6,r7,lr}
  0009db6a: add r7,sp,#0xc
  0009db6c: push.w r11
  0009db70: ldr r1,[0x0009dc90]
  0009db72: mov r4,r0
  0009db74: ldr r2,[0x0009dc94]
  0009db76: add r1,pc
  0009db78: add r2,pc
  0009db7a: blx 0x00070954
  0009db7e: ldr r2,[0x0009dc98]
  0009db80: ldr r1,[0x0009dc9c]
  0009db82: add r2,pc
  0009db84: str r0,[r4,#0x4]
  0009db86: add r1,pc
  0009db88: ldr r5,[r2,#0x0]
  0009db8a: blx r5
  0009db8c: ldr r1,[0x0009dca0]
  0009db8e: ldr r2,[r4,#0x4]
  0009db90: add r1,pc
  0009db92: str r0,[r4,#0x68]
  0009db94: mov r0,r2
  0009db96: blx r5
  0009db98: ldr r3,[0x0009dca4]
  0009db9a: ldr r1,[0x0009dca8]
  0009db9c: add r3,pc
  0009db9e: ldr r2,[r4,#0x4]
  0009dba0: add r1,pc
  0009dba2: str r0,[r4,#0x6c]
  0009dba4: ldr r6,[r3,#0x0]
  0009dba6: mov r0,r2
  0009dba8: blx r6
  0009dbaa: ldr r1,[0x0009dcac]
  0009dbac: ldr r2,[r4,#0x4]
  0009dbae: add r1,pc
  0009dbb0: str r0,[r4,#0x20]
  0009dbb2: mov r0,r2
  0009dbb4: blx r6
  0009dbb6: ldr r1,[0x0009dcb0]
  0009dbb8: ldr r2,[r4,#0x4]
  0009dbba: add r1,pc
  0009dbbc: str r0,[r4,#0x24]
  0009dbbe: mov r0,r2
  0009dbc0: blx r6
  0009dbc2: ldr r1,[0x0009dcb4]
  0009dbc4: ldr r2,[r4,#0x4]
  0009dbc6: add r1,pc
  0009dbc8: str r0,[r4,#0x28]
  0009dbca: mov r0,r2
  0009dbcc: blx r5
  0009dbce: ldr r1,[0x0009dcb8]
  0009dbd0: ldr r2,[r4,#0x4]
  0009dbd2: add r1,pc
  0009dbd4: str r0,[r4,#0x2c]
  0009dbd6: mov r0,r2
  0009dbd8: blx r5
  0009dbda: ldr r1,[0x0009dcbc]
  0009dbdc: ldr r2,[r4,#0x4]
  0009dbde: add r1,pc
  0009dbe0: str r0,[r4,#0x30]
  0009dbe2: mov r0,r2
  0009dbe4: blx r5
  0009dbe6: ldr r1,[0x0009dcc0]
  0009dbe8: ldr r2,[r4,#0x4]
  0009dbea: add r1,pc
  0009dbec: str r0,[r4,#0x34]
  0009dbee: mov r0,r2
  0009dbf0: blx r5
  0009dbf2: ldr r1,[0x0009dcc4]
  0009dbf4: ldr r2,[r4,#0x4]
  0009dbf6: add r1,pc
  0009dbf8: str r0,[r4,#0x38]
  0009dbfa: mov r0,r2
  0009dbfc: blx r5
  0009dbfe: ldr r1,[0x0009dcc8]
  0009dc00: ldr r2,[r4,#0x4]
  0009dc02: add r1,pc
  0009dc04: str r0,[r4,#0x3c]
  0009dc06: mov r0,r2
  0009dc08: blx r5
  0009dc0a: ldr r1,[0x0009dccc]
  0009dc0c: ldr r2,[r4,#0x4]
  0009dc0e: add r1,pc
  0009dc10: str r0,[r4,#0x64]
  0009dc12: mov r0,r2
  0009dc14: blx r5
  0009dc16: ldr r1,[0x0009dcd0]
  0009dc18: ldr r2,[r4,#0x4]
  0009dc1a: add r1,pc
  0009dc1c: str r0,[r4,#0x44]
  0009dc1e: mov r0,r2
  0009dc20: blx r5
  0009dc22: ldr r1,[0x0009dcd4]
  0009dc24: ldr r2,[r4,#0x4]
  0009dc26: add r1,pc
  0009dc28: str r0,[r4,#0x40]
  0009dc2a: mov r0,r2
  0009dc2c: blx r5
  0009dc2e: ldr r1,[0x0009dcd8]
  0009dc30: ldr r2,[r4,#0x4]
  0009dc32: add r1,pc
  0009dc34: str r0,[r4,#0x48]
  0009dc36: mov r0,r2
  0009dc38: blx r5
  0009dc3a: ldr r1,[0x0009dcdc]
  0009dc3c: ldr r2,[r4,#0x4]
  0009dc3e: add r1,pc
  0009dc40: str r0,[r4,#0x50]
  0009dc42: mov r0,r2
  0009dc44: blx r5
  0009dc46: ldr r1,[0x0009dce0]
  0009dc48: ldr r2,[r4,#0x4]
  0009dc4a: add r1,pc
  0009dc4c: str r0,[r4,#0x58]
  0009dc4e: mov r0,r2
  0009dc50: blx r5
  0009dc52: ldr r1,[0x0009dce4]
  0009dc54: ldr r2,[r4,#0x4]
  0009dc56: add r1,pc
  0009dc58: str r0,[r4,#0x60]
  0009dc5a: mov r0,r2
  0009dc5c: blx r5
  0009dc5e: ldr r1,[0x0009dce8]
  0009dc60: ldr r2,[r4,#0x4]
  0009dc62: add r1,pc
  0009dc64: str r0,[r4,#0x4c]
  0009dc66: mov r0,r2
  0009dc68: blx r5
  0009dc6a: ldr r1,[0x0009dcec]
  0009dc6c: ldr r2,[r4,#0x4]
  0009dc6e: add r1,pc
  0009dc70: str r0,[r4,#0x54]
  0009dc72: mov r0,r2
  0009dc74: blx r5
  0009dc76: ldr r1,[r4,#0x4]
  0009dc78: str r0,[r4,#0x5c]
  0009dc7a: mov r0,r1
  0009dc7c: blx 0x00070960
  0009dc80: ldr r0,[r4,#0x44]
  0009dc82: movs r1,#0x0
  0009dc84: pop.w r11
  0009dc88: pop.w {r4,r5,r6,r7,lr}
  0009dc8c: b.w 0x001ab768
```
