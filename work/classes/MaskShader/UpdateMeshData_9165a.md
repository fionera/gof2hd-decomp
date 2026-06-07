# MaskShader::UpdateMeshData
elf 0x9165a body 290
Sig: undefined __thiscall UpdateMeshData(MaskShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::MaskShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::MaskShader::UpdateMeshData(MaskShader *this,Mesh *param_1,Engine *param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  
  if (-1 < *(int *)(this + 0x2c)) {
    glUniformMatrix4fv(*(int *)(this + 0x2c),1,0,param_2 + 0x104);
  }
  if (this[9] != (MaskShader)0x0) {
    if (-1 < *(int *)(this + 0x38)) {
      glUniform4fv(*(int *)(this + 0x38),1,param_2 + 0xd0);
    }
    this[9] = (MaskShader)0x0;
  }
  if (-1 < *(int *)(this + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glEnableVertexAttribArray();
  }
  piVar2 = *(int **)(**(int **)(param_2 + 0x30) + 0x20);
  if (piVar2 != (int *)0x0) {
    Engine::SetTextureSlot((uint)param_2,piVar2[1]);
  }
  if (param_1[0x5c] == (Mesh)0x0) {
    if (-1 < *(int *)(this + 0x20)) {
      glVertexAttribPointer(*(int *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    }
    if (-1 < *(int *)(this + 0x24)) {
      glVertexAttribPointer(*(int *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
    }
    if (piVar2 == (int *)0x0) {
      return;
    }
    iVar1 = *(int *)(this + 0x28);
    if (iVar1 < 0) {
      return;
    }
    uVar3 = *(undefined4 *)(*piVar2 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    glVertexAttribPointer(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    if (piVar2 == (int *)0x0) {
      return;
    }
    glBindBuffer(0x8892,*(undefined4 *)(*piVar2 + 0x68));
    iVar1 = *(int *)(this + 0x28);
    uVar3 = 0;
  }
  glVertexAttribPointer(iVar1,2,0x1406,0,0,uVar3);
  return;
}

```

## target disasm
```
  000a165a: push {r4,r5,r6,r7,lr}
  000a165c: add r7,sp,#0xc
  000a165e: push.w r8
  000a1662: sub sp,#0x8
  000a1664: mov r4,r0
  000a1666: ldr r0,[r0,#0x2c]
  000a1668: mov r6,r2
  000a166a: mov r8,r1
  000a166c: cmp r0,#0x0
  000a166e: blt 0x000a167c
  000a1670: add.w r3,r6,#0x104
  000a1674: movs r1,#0x1
  000a1676: movs r2,#0x0
  000a1678: blx 0x00070990
  000a167c: ldrb r0,[r4,#0x9]
  000a167e: cbz r0,0x000a1694
  000a1680: ldr r0,[r4,#0x38]
  000a1682: cmp r0,#0x0
  000a1684: blt 0x000a1690
  000a1686: add.w r2,r6,#0xd0
  000a168a: movs r1,#0x1
  000a168c: blx 0x00070984
  000a1690: movs r0,#0x0
  000a1692: strb r0,[r4,#0x9]
  000a1694: ldr r0,[r4,#0x20]
  000a1696: cmp r0,#0x0
  000a1698: blt 0x000a169e
  000a169a: blx 0x00070a5c
  000a169e: ldr r0,[r4,#0x24]
  000a16a0: cmp r0,#0x0
  000a16a2: blt 0x000a16a8
  000a16a4: blx 0x00070a5c
  000a16a8: ldr r0,[r4,#0x28]
  000a16aa: cmp r0,#0x0
  000a16ac: blt 0x000a16b2
  000a16ae: blx 0x00070a5c
  000a16b2: ldr r0,[r6,#0x30]
  000a16b4: ldr r0,[r0,#0x0]
  000a16b6: ldr r5,[r0,#0x20]
  000a16b8: cbz r5,0x000a16c4
  000a16ba: ldr r1,[r5,#0x4]
  000a16bc: mov r0,r6
  000a16be: movs r2,#0x1
  000a16c0: blx 0x000712d8
  000a16c4: ldrb.w r0,[r8,#0x5c]
  000a16c8: cbz r0,0x000a171e
  000a16ca: ldr.w r1,[r8,#0x60]
  000a16ce: movw r0,#0x8892
  000a16d2: blx 0x0006ed04
  000a16d6: ldr r0,[r4,#0x20]
  000a16d8: movs r6,#0x0
  000a16da: movs r1,#0x3
  000a16dc: movw r2,#0x1406
  000a16e0: movs r3,#0x0
  000a16e2: strd r6,r6,[sp,#0x0]
  000a16e6: blx 0x000709c0
  000a16ea: ldr.w r1,[r8,#0x68]
  000a16ee: movw r0,#0x8892
  000a16f2: blx 0x0006ed04
  000a16f6: ldr r0,[r4,#0x24]
  000a16f8: movs r1,#0x2
  000a16fa: movw r2,#0x1406
  000a16fe: movs r3,#0x0
  000a1700: strd r6,r6,[sp,#0x0]
  000a1704: blx 0x000709c0
  000a1708: cbz r5,0x000a1774
  000a170a: ldr r0,[r5,#0x0]
  000a170c: ldr r1,[r0,#0x68]
  000a170e: movw r0,#0x8892
  000a1712: blx 0x0006ed04
  000a1716: ldr r0,[r4,#0x28]
  000a1718: strd r6,r6,[sp,#0x0]
  000a171c: b 0x000a1768
  000a171e: ldr r0,[r4,#0x20]
  000a1720: cmp r0,#0x0
  000a1722: blt 0x000a173a
  000a1724: ldr.w r1,[r8,#0x4]
  000a1728: movs r2,#0x0
  000a172a: movs r3,#0x0
  000a172c: strd r2,r1,[sp,#0x0]
  000a1730: movs r1,#0x3
  000a1732: movw r2,#0x1406
  000a1736: blx 0x000709c0
  000a173a: ldr r0,[r4,#0x24]
  000a173c: cmp r0,#0x0
  000a173e: blt 0x000a1756
  000a1740: ldr.w r1,[r8,#0x8]
  000a1744: movs r2,#0x0
  000a1746: movs r3,#0x0
  000a1748: strd r2,r1,[sp,#0x0]
  000a174c: movs r1,#0x2
  000a174e: movw r2,#0x1406
  000a1752: blx 0x000709c0
  000a1756: cbz r5,0x000a1774
  000a1758: ldr r0,[r4,#0x28]
  000a175a: cmp r0,#0x0
  000a175c: blt 0x000a1774
  000a175e: ldr r1,[r5,#0x0]
  000a1760: movs r2,#0x0
  000a1762: ldr r1,[r1,#0x8]
  000a1764: strd r2,r1,[sp,#0x0]
  000a1768: movs r1,#0x2
  000a176a: movw r2,#0x1406
  000a176e: movs r3,#0x0
  000a1770: blx 0x000709c0
  000a1774: add sp,#0x8
  000a1776: pop.w r8
  000a177a: pop {r4,r5,r6,r7,pc}
```
