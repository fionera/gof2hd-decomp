# PaintCanvas::ReleaseAllResources
elf 0x77c84 body 552
Sig: undefined __stdcall ReleaseAllResources(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::ReleaseAllResources() */

void AbyssEngine::PaintCanvas::ReleaseAllResources(void)

{
  int in_r0;
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  Transform *this;
  Material *this_00;
  void *pvVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  uint uVar9;
  int *piVar10;
  
  piVar1 = *(int **)(DAT_00087eac + 0x87c96);
  iVar7 = *piVar1;
  **(undefined4 **)(DAT_00087eb0 + 0x87c98) = 0;
  iVar5 = *(int *)(in_r0 + 0x134);
  for (iVar2 = 0; iVar5 != iVar2; iVar2 = iVar2 + 1) {
    *(undefined4 *)(*(int *)(*(int *)(in_r0 + 0x138) + iVar2 * 4) + 8) = 0xffffffff;
  }
  iVar2 = 0;
  piVar10 = *(int **)(DAT_00087eb4 + 0x87cce);
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x10); uVar9 = uVar9 + 1) {
    piVar8 = *(int **)(*(int *)(in_r0 + 0x14) + iVar2);
    if (*piVar8 == -1) {
      puVar3 = (undefined4 *)(*(int *)(in_r0 + 0x14) + uVar9 * 4);
    }
    else {
      glDeleteTextures(1,&stack0xffffffd4);
      *piVar10 = *piVar10 + -1;
      iVar5 = *(int *)(in_r0 + 0x14);
      *(int *)(*(int *)(in_r0 + 0x34) + 0x70) =
           *(int *)(*(int *)(in_r0 + 0x34) + 0x70) - *(int *)(*(int *)(iVar5 + iVar2) + 0x18);
      piVar8 = *(int **)(iVar5 + iVar2);
      puVar3 = (undefined4 *)(iVar5 + iVar2);
    }
    if (piVar8 != (int *)0x0) {
      String::~String((String *)(piVar8 + 1));
      operator_delete(piVar8);
      puVar3 = (undefined4 *)(*(int *)(in_r0 + 0x14) + iVar2);
    }
    *puVar3 = 0;
    iVar2 = iVar2 + 4;
  }
  iVar2 = 0;
  *(undefined4 *)(in_r0 + 0x10) = 0;
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x140); uVar9 = uVar9 + 1) {
    if (*(int *)(*(int *)(in_r0 + 0x144) + iVar2) != 0) {
      ImageFontRelease(*(Engine **)(in_r0 + 0x34),(ImageFont **)(*(int *)(in_r0 + 0x144) + iVar2));
    }
    iVar2 = iVar2 + 4;
  }
  ArrayRemoveAll<AbyssEngine::ImageFont*>((Array *)(in_r0 + 0x140));
  iVar2 = 0;
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x14c); uVar9 = uVar9 + 1) {
    if (*(int *)(*(int *)(in_r0 + 0x150) + iVar2) != 0) {
      Image2DRelease(*(Engine **)(in_r0 + 0x34),(Image2D **)(*(int *)(in_r0 + 0x150) + iVar2));
    }
    iVar2 = iVar2 + 4;
  }
  ArrayRemoveAll<AbyssEngine::Image2D*>((Array *)(in_r0 + 0x14c));
  iVar2 = 0;
  piVar10 = *(int **)(DAT_00087eb8 + 0x87d96);
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x24); uVar9 = uVar9 + 1) {
    iVar6 = *(int *)(in_r0 + 0x28);
    iVar5 = *(int *)(iVar6 + iVar2);
    if (iVar5 != 0) {
      *piVar10 = *piVar10 - *(int *)(iVar5 + 0x7c);
      MeshRelease(*(Engine **)(in_r0 + 0x34),(Mesh **)(iVar6 + iVar2));
    }
    iVar2 = iVar2 + 4;
  }
  ArrayRemoveAll<AbyssEngine::Mesh*>((Array *)(in_r0 + 0x24));
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x158); uVar9 = uVar9 + 1) {
    this = *(Transform **)(*(int *)(in_r0 + 0x15c) + uVar9 * 4);
    if (this != (Transform *)0x0) {
      pvVar4 = (void *)Transform::~Transform(this);
      operator_delete(pvVar4);
      *(undefined4 *)(*(int *)(in_r0 + 0x15c) + uVar9 * 4) = 0;
    }
  }
  ArrayRemoveAll<AbyssEngine::Transform*>((Array *)(in_r0 + 0x158));
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x164); uVar9 = uVar9 + 1) {
    pvVar4 = *(void **)(*(int *)(in_r0 + 0x168) + uVar9 * 4);
    if (pvVar4 != (void *)0x0) {
      operator_delete(pvVar4);
      *(undefined4 *)(*(int *)(in_r0 + 0x168) + uVar9 * 4) = 0;
    }
  }
  ArrayRemoveAll<AbyssEngine::Camera*>((Array *)(in_r0 + 0x164));
  *(undefined4 *)(in_r0 + 0x170) = 0xffffffff;
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x174); uVar9 = uVar9 + 1) {
    this_00 = *(Material **)(*(int *)(in_r0 + 0x178) + uVar9 * 4);
    if (this_00 != (Material *)0x0) {
      pvVar4 = (void *)Material::~Material(this_00);
      operator_delete(pvVar4);
      *(undefined4 *)(*(int *)(in_r0 + 0x178) + uVar9 * 4) = 0;
    }
  }
  ArrayRemoveAll<AbyssEngine::Material*>((Array *)(in_r0 + 0x174));
  iVar2 = 0;
  for (uVar9 = 0; uVar9 < *(uint *)(in_r0 + 0x180); uVar9 = uVar9 + 1) {
    if (*(int *)(*(int *)(in_r0 + 0x184) + iVar2) != 0) {
      SpriteSystemRelease(*(Engine **)(in_r0 + 0x34),
                          (SpriteSystem **)(*(int *)(in_r0 + 0x184) + iVar2));
    }
    iVar2 = iVar2 + 4;
  }
  ArrayRemoveAll<AbyssEngine::SpriteSystem*>((Array *)(in_r0 + 0x180));
  *(undefined4 *)(in_r0 + 0x1cc) = 0;
  iVar2 = *piVar1;
  iVar7 = iVar2 - iVar7;
  if (iVar7 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar7,iVar2);
  }
  return;
}

```

## target disasm
```
  00087c84: push {r4,r5,r6,r7,lr}
  00087c86: add r7,sp,#0xc
  00087c88: push {r5,r6,r7,r8,r9,r10,r11}
  00087c8c: mov r4,r0
  00087c8e: ldr r0,[0x00087eac]
  00087c90: ldr r1,[0x00087eb0]
  00087c92: add r0,pc
  00087c94: add r1,pc
  00087c96: ldr r0,[r0,#0x0]
  00087c98: ldr r1,[r1,#0x0]
  00087c9a: str r0,[sp,#0x0]
  00087c9c: ldr r2,[r0,#0x0]
  00087c9e: movs r0,#0x0
  00087ca0: str r0,[r1,#0x0]
  00087ca2: str r2,[sp,#0x8]
  00087ca4: mov.w r2,#0xffffffff
  00087ca8: ldr.w r1,[r4,#0x134]
  00087cac: b 0x00087cba
  00087cae: ldr.w r3,[r4,#0x138]
  00087cb2: ldr.w r3,[r3,r0,lsl #0x2]
  00087cb6: adds r0,#0x1
  00087cb8: str r2,[r3,#0x8]
  00087cba: cmp r1,r0
  00087cbc: bne 0x00087cae
  00087cbe: ldr r0,[0x00087eb4]
  00087cc0: add.w r9,sp,#0x4
  00087cc4: mov.w r10,#0x0
  00087cc8: movs r5,#0x0
  00087cca: add r0,pc
  00087ccc: mov.w r8,#0x0
  00087cd0: ldr.w r11,[r0,#0x0]
  00087cd4: b 0x00087d28
  00087cd6: ldr r0,[r4,#0x14]
  00087cd8: ldr r6,[r0,r5]
  00087cda: ldr r1,[r6,#0x0]
  00087cdc: adds r2,r1,#0x1
  00087cde: beq 0x00087d08
  00087ce0: str r1,[sp,#0x4]
  00087ce2: movs r0,#0x1
  00087ce4: mov r1,r9
  00087ce6: blx 0x0006f2a4
  00087cea: ldr.w r0,[r11,#0x0]
  00087cee: subs r0,#0x1
  00087cf0: str.w r0,[r11,#0x0]
  00087cf4: ldr r0,[r4,#0x14]
  00087cf6: ldr r1,[r4,#0x34]
  00087cf8: ldr r2,[r0,r5]
  00087cfa: ldr r3,[r1,#0x70]
  00087cfc: ldr r2,[r2,#0x18]
  00087cfe: subs r2,r3,r2
  00087d00: str r2,[r1,#0x70]
  00087d02: ldr r6,[r0,r5]
  00087d04: add r0,r5
  00087d06: b 0x00087d0c
  00087d08: add.w r0,r0,r8, lsl #0x2
  00087d0c: cbz r6,0x00087d1e
  00087d0e: adds r0,r6,#0x4
  00087d10: blx 0x0006ee30
  00087d14: mov r0,r6
  00087d16: blx 0x0006eb48
  00087d1a: ldr r0,[r4,#0x14]
  00087d1c: add r0,r5
  00087d1e: str.w r10,[r0,#0x0]
  00087d22: adds r5,#0x4
  00087d24: add.w r8,r8,#0x1
  00087d28: ldr r0,[r4,#0x10]
  00087d2a: cmp r8,r0
  00087d2c: bcc 0x00087cd6
  00087d2e: add.w r8,r4,#0x140
  00087d32: movs r6,#0x0
  00087d34: movs r5,#0x0
  00087d36: str r6,[r4,#0x10]
  00087d38: b 0x00087d4e
  00087d3a: ldr.w r0,[r4,#0x144]
  00087d3e: ldr r1,[r0,r6]
  00087d40: cbz r1,0x00087d4a
  00087d42: adds r1,r0,r6
  00087d44: ldr r0,[r4,#0x34]
  00087d46: blx 0x0006f208
  00087d4a: adds r6,#0x4
  00087d4c: adds r5,#0x1
  00087d4e: ldr.w r0,[r8,#0x0]
  00087d52: cmp r5,r0
  00087d54: bcc 0x00087d3a
  00087d56: mov r0,r8
  00087d58: blx 0x0006f9ac
  00087d5c: add.w r8,r4,#0x14c
  00087d60: movs r6,#0x0
  00087d62: movs r5,#0x0
  00087d64: b 0x00087d7a
  00087d66: ldr.w r0,[r4,#0x150]
  00087d6a: ldr r1,[r0,r6]
  00087d6c: cbz r1,0x00087d76
  00087d6e: adds r1,r0,r6
  00087d70: ldr r0,[r4,#0x34]
  00087d72: blx 0x0006f9b8
  00087d76: adds r6,#0x4
  00087d78: adds r5,#0x1
  00087d7a: ldr.w r0,[r8,#0x0]
  00087d7e: cmp r5,r0
  00087d80: bcc 0x00087d66
  00087d82: mov r0,r8
  00087d84: blx 0x0006f9c4
  00087d88: ldr r0,[0x00087eb8]
  00087d8a: add.w r8,r4,#0x24
  00087d8e: movs r6,#0x0
  00087d90: movs r5,#0x0
  00087d92: add r0,pc
  00087d94: ldr.w r9,[r0,#0x0]
  00087d98: b 0x00087db8
  00087d9a: ldr r1,[r4,#0x28]
  00087d9c: ldr r0,[r1,r6]
  00087d9e: cbz r0,0x00087db4
  00087da0: ldr r0,[r0,#0x7c]
  00087da2: add r1,r6
  00087da4: ldr.w r2,[r9,#0x0]
  00087da8: subs r0,r2,r0
  00087daa: str.w r0,[r9,#0x0]
  00087dae: ldr r0,[r4,#0x34]
  00087db0: blx 0x0006ebd8
  00087db4: adds r6,#0x4
  00087db6: adds r5,#0x1
  00087db8: ldr.w r0,[r8,#0x0]
  00087dbc: cmp r5,r0
  00087dbe: bcc 0x00087d9a
  00087dc0: mov r0,r8
  00087dc2: blx 0x0006f9d0
  00087dc6: add.w r5,r4,#0x158
  00087dca: mov.w r8,#0x0
  00087dce: movs r6,#0x0
  00087dd0: b 0x00087dee
  00087dd2: ldr.w r0,[r4,#0x15c]
  00087dd6: ldr.w r0,[r0,r6,lsl #0x2]
  00087dda: cbz r0,0x00087dec
  00087ddc: blx 0x0006eba8
  00087de0: blx 0x0006eb48
  00087de4: ldr.w r0,[r4,#0x15c]
  00087de8: str.w r8,[r0,r6,lsl #0x2]
  00087dec: adds r6,#0x1
  00087dee: ldr r0,[r5,#0x0]
  00087df0: cmp r6,r0
  00087df2: bcc 0x00087dd2
  00087df4: mov r0,r5
  00087df6: blx 0x0006f9dc
  00087dfa: add.w r5,r4,#0x164
  00087dfe: mov.w r8,#0x0
  00087e02: movs r6,#0x0
  00087e04: b 0x00087e1e
  00087e06: ldr.w r0,[r4,#0x168]
  00087e0a: ldr.w r0,[r0,r6,lsl #0x2]
  00087e0e: cbz r0,0x00087e1c
  00087e10: blx 0x0006eb48
  00087e14: ldr.w r0,[r4,#0x168]
  00087e18: str.w r8,[r0,r6,lsl #0x2]
  00087e1c: adds r6,#0x1
  00087e1e: ldr r0,[r5,#0x0]
  00087e20: cmp r6,r0
  00087e22: bcc 0x00087e06
  00087e24: mov r0,r5
  00087e26: blx 0x0006f9e8
  00087e2a: add.w r5,r4,#0x174
  00087e2e: mov.w r0,#0xffffffff
  00087e32: mov.w r8,#0x0
  00087e36: movs r6,#0x0
  00087e38: str.w r0,[r4,#0x170]
  00087e3c: b 0x00087e5a
  00087e3e: ldr.w r0,[r4,#0x178]
  00087e42: ldr.w r0,[r0,r6,lsl #0x2]
  00087e46: cbz r0,0x00087e58
  00087e48: blx 0x0006f9f4
  00087e4c: blx 0x0006eb48
  00087e50: ldr.w r0,[r4,#0x178]
  00087e54: str.w r8,[r0,r6,lsl #0x2]
  00087e58: adds r6,#0x1
  00087e5a: ldr r0,[r5,#0x0]
  00087e5c: cmp r6,r0
  00087e5e: bcc 0x00087e3e
  00087e60: mov r0,r5
  00087e62: blx 0x0006fa00
  00087e66: add.w r8,r4,#0x180
  00087e6a: movs r6,#0x0
  00087e6c: movs r5,#0x0
  00087e6e: b 0x00087e84
  00087e70: ldr.w r0,[r4,#0x184]
  00087e74: ldr r1,[r0,r6]
  00087e76: cbz r1,0x00087e80
  00087e78: adds r1,r0,r6
  00087e7a: ldr r0,[r4,#0x34]
  00087e7c: blx 0x0006fa0c
  00087e80: adds r6,#0x4
  00087e82: adds r5,#0x1
  00087e84: ldr.w r0,[r8,#0x0]
  00087e88: cmp r5,r0
  00087e8a: bcc 0x00087e70
  00087e8c: mov r0,r8
  00087e8e: blx 0x0006fa18
  00087e92: movs r0,#0x0
  00087e94: str.w r0,[r4,#0x1cc]
  00087e98: ldr r0,[sp,#0x8]
  00087e9a: ldr r1,[sp,#0x0]
  00087e9c: ldr r1,[r1,#0x0]
  00087e9e: subs r0,r1,r0
  00087ea0: itt eq
  00087ea2: pop.eq.w {r1,r2,r3,r8,r9,r10,r11}
  00087ea6: pop.eq {r4,r5,r6,r7,pc}
  00087ea8: blx 0x0006e824
```
