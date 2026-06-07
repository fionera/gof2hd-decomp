# MaskShader::Init
elf 0x9156c body 150
Sig: undefined __stdcall Init(Engine * param_1)

## decompile
```c

/* AbyssEngine::MaskShader::Init(AbyssEngine::Engine*) */

void AbyssEngine::MaskShader::Init(Engine *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  
  uVar1 = ShaderBaseStruct::ES2LoadProgram((char *)param_1,(char *)(DAT_000a1604 + 0xa157a));
  iVar2 = DAT_000a1610;
  puVar3 = (undefined4 *)(DAT_000a160c + 0xa1586);
  *(undefined4 *)(param_1 + 4) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(uVar1,iVar2 + 0xa158a);
  iVar2 = DAT_000a1614 + 0xa1594;
  *(undefined4 *)(param_1 + 0x20) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1618 + 0xa15a0;
  *(undefined4 *)(param_1 + 0x24) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  puVar3 = (undefined4 *)(DAT_000a161c + 0xa15ac);
  iVar2 = DAT_000a1620 + 0xa15b0;
  *(undefined4 *)(param_1 + 0x28) = uVar1;
  pcVar4 = (code *)*puVar3;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1624 + 0xa15be;
  *(undefined4 *)(param_1 + 0x2c) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a1628 + 0xa15ca;
  *(undefined4 *)(param_1 + 0x30) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  iVar2 = DAT_000a162c + 0xa15d6;
  *(undefined4 *)(param_1 + 0x34) = uVar1;
  uVar1 = (*pcVar4)(*(undefined4 *)(param_1 + 4),iVar2);
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  glUseProgram(*(undefined4 *)(param_1 + 4));
  for (iVar2 = 0; iVar2 != 2; iVar2 = iVar2 + 1) {
    if (-1 < *(int *)(param_1 + iVar2 * 4 + 0x30)) {
      glUniform1i(*(int *)(param_1 + iVar2 * 4 + 0x30),iVar2);
    }
  }
  return;
}

```

## target disasm
```
  000a156c: push {r4,r5,r7,lr}
  000a156e: add r7,sp,#0x8
  000a1570: ldr r1,[0x000a1604]
  000a1572: mov r4,r0
  000a1574: ldr r2,[0x000a1608]
  000a1576: add r1,pc
  000a1578: add r2,pc
  000a157a: blx 0x00070954
  000a157e: ldr r2,[0x000a160c]
  000a1580: ldr r1,[0x000a1610]
  000a1582: add r2,pc
  000a1584: str r0,[r4,#0x4]
  000a1586: add r1,pc
  000a1588: ldr r5,[r2,#0x0]
  000a158a: blx r5
  000a158c: ldr r1,[0x000a1614]
  000a158e: ldr r2,[r4,#0x4]
  000a1590: add r1,pc
  000a1592: str r0,[r4,#0x20]
  000a1594: mov r0,r2
  000a1596: blx r5
  000a1598: ldr r1,[0x000a1618]
  000a159a: ldr r2,[r4,#0x4]
  000a159c: add r1,pc
  000a159e: str r0,[r4,#0x24]
  000a15a0: mov r0,r2
  000a15a2: blx r5
  000a15a4: ldr r3,[0x000a161c]
  000a15a6: ldr r1,[0x000a1620]
  000a15a8: add r3,pc
  000a15aa: ldr r2,[r4,#0x4]
  000a15ac: add r1,pc
  000a15ae: str r0,[r4,#0x28]
  000a15b0: ldr r5,[r3,#0x0]
  000a15b2: mov r0,r2
  000a15b4: blx r5
  000a15b6: ldr r1,[0x000a1624]
  000a15b8: ldr r2,[r4,#0x4]
  000a15ba: add r1,pc
  000a15bc: str r0,[r4,#0x2c]
  000a15be: mov r0,r2
  000a15c0: blx r5
  000a15c2: ldr r1,[0x000a1628]
  000a15c4: ldr r2,[r4,#0x4]
  000a15c6: add r1,pc
  000a15c8: str r0,[r4,#0x30]
  000a15ca: mov r0,r2
  000a15cc: blx r5
  000a15ce: ldr r1,[0x000a162c]
  000a15d0: ldr r2,[r4,#0x4]
  000a15d2: add r1,pc
  000a15d4: str r0,[r4,#0x34]
  000a15d6: mov r0,r2
  000a15d8: blx r5
  000a15da: ldr r1,[r4,#0x4]
  000a15dc: str r0,[r4,#0x38]
  000a15de: mov r0,r1
  000a15e0: blx 0x00070960
  000a15e4: add.w r5,r4,#0x30
  000a15e8: movs r4,#0x0
  000a15ea: b 0x000a15fc
  000a15ec: ldr.w r0,[r5,r4,lsl #0x2]
  000a15f0: cmp r0,#0x0
  000a15f2: blt 0x000a15fa
  000a15f4: mov r1,r4
  000a15f6: blx 0x0007096c
  000a15fa: adds r4,#0x1
  000a15fc: cmp r4,#0x2
  000a15fe: bne 0x000a15ec
  000a1600: pop {r4,r5,r7,pc}
```
