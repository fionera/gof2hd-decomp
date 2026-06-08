# AbyssEngine::SpriteSystemCreate
elf 0x86adc body 394
Sig: undefined __stdcall SpriteSystemCreate(Engine * param_1, ushort param_2, bool param_3, SpriteSystem * * param_4)

## decompile
```c

/* AbyssEngine::SpriteSystemCreate(AbyssEngine::Engine*, unsigned short, bool,
   AbyssEngine::SpriteSystem**) */

undefined4
AbyssEngine::SpriteSystemCreate(Engine *param_1,ushort param_2,bool param_3,SpriteSystem **param_4)

{
  char cVar1;
  SpriteSystem *pSVar2;
  int iVar3;
  void *pvVar4;
  undefined4 uVar5;
  ushort uVar6;
  uint uVar7;
  SpriteSystem *pSVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  
  uVar7 = (uint)param_2;
  if (uVar7 == 0) {
    uVar5 = 0xfffffffc;
  }
  else {
    pSVar2 = operator_new(0x14);
    *(undefined4 *)(pSVar2 + 4) = 0;
    *(undefined4 *)(pSVar2 + 8) = 0;
    uVar11 = uVar7 << 1;
    *(undefined4 *)(pSVar2 + 0x10) = 0;
    *param_4 = pSVar2;
    *(ushort *)pSVar2 = param_2;
    iVar3 = MeshCreate(param_1,(uVar7 & 0x3fff) << 2,uVar11 & 0xffff,0x1f,pSVar2 + 0x10);
    pSVar8 = *param_4;
    pSVar2 = pSVar8 + 0x10;
    if (iVar3 == 1) {
      iVar3 = *(int *)(*(Mesh **)pSVar2 + 0x2c);
      pvVar4 = operator_new__(uVar7 * 0xc);
      *(void **)(pSVar8 + 4) = pvVar4;
      __aeabi_memclr4(*(undefined4 *)(*param_4 + 4),uVar7 * 0xc);
      pSVar2 = *param_4;
      pSVar2[0xc] = (SpriteSystem)param_3;
      if (param_3) {
        pvVar4 = operator_new__(2);
        *(void **)(pSVar2 + 8) = pvVar4;
        **(undefined2 **)(*param_4 + 8) = 0;
      }
      else {
        pvVar4 = operator_new__(uVar11);
        *(void **)(pSVar2 + 8) = pvVar4;
        __aeabi_memclr(*(undefined4 *)(*param_4 + 8),uVar11);
      }
      uVar6 = 0;
      for (uVar11 = 0; uVar11 = uVar11 & 0xffff, uVar11 < uVar7 * 6; uVar11 = uVar11 + 6) {
        iVar10 = iVar3 + uVar11 * 2;
        *(ushort *)(iVar3 + uVar11 * 2) = uVar6;
        *(ushort *)(iVar10 + 2) = uVar6 | 1;
        *(ushort *)(iVar10 + 4) = uVar6 | 2;
        *(ushort *)(iVar10 + 6) = uVar6;
        *(ushort *)(iVar10 + 8) = uVar6 | 2;
        *(ushort *)(iVar10 + 10) = uVar6 | 3;
        uVar6 = uVar6 + 4;
      }
      iVar3 = *(int *)(*param_4 + 0x10);
      uVar7 = (uint)*(ushort *)(iVar3 + 2);
      for (uVar6 = 0; uVar11 = (uint)uVar6, uVar11 <= uVar7 * 4 && uVar7 * 4 - uVar11 != 0;
          uVar6 = uVar6 + 1) {
        *(undefined4 *)(*(int *)(iVar3 + 0xc) + uVar11 * 4) = 0x3f800000;
      }
      iVar10 = 0;
      cVar1 = **(char **)(DAT_00096c68 + 0x96c30);
      for (; uVar7 != 0; uVar7 = uVar7 - 1) {
        iVar9 = *(int *)(iVar3 + 0x10);
        *(undefined4 *)(iVar9 + iVar10) = 0;
        iVar9 = iVar9 + iVar10;
        *(undefined4 *)(iVar9 + 4) = 0;
        *(undefined4 *)(iVar9 + 8) = 0x3f800000;
        if (cVar1 != '\0') {
          iVar9 = *(int *)(iVar3 + 0x14);
          *(undefined4 *)(iVar9 + iVar10) = 0x3f800000;
          iVar9 = iVar9 + iVar10;
          *(undefined4 *)(iVar9 + 4) = 0;
          *(undefined4 *)(iVar9 + 8) = 0;
          iVar9 = *(int *)(iVar3 + 0x18);
          *(undefined4 *)(iVar9 + iVar10) = 0;
          iVar9 = iVar9 + iVar10;
          *(undefined4 *)(iVar9 + 4) = 0x3f800000;
          *(undefined4 *)(iVar9 + 8) = 0;
        }
        iVar10 = iVar10 + 0xc;
      }
      uVar5 = 1;
    }
    else {
      MeshRelease(param_1,(Mesh **)pSVar2);
      pSVar2 = *param_4 + 4;
      if (*(void **)pSVar2 != (void *)0x0) {
        operator_delete__(*(void **)pSVar2);
        pSVar2 = *param_4 + 4;
      }
      *(undefined4 *)pSVar2 = 0;
      pSVar2 = *param_4 + 8;
      if (*(void **)pSVar2 != (void *)0x0) {
        operator_delete__(*(void **)pSVar2);
        pSVar2 = *param_4 + 8;
      }
      *(undefined4 *)pSVar2 = 0;
      if (*param_4 != (SpriteSystem *)0x0) {
        operator_delete(*param_4);
      }
      *param_4 = (SpriteSystem *)0x0;
      uVar5 = 0xffffffff;
    }
  }
  return uVar5;
}

```

## target disasm
```
  00096adc: push {r4,r5,r6,r7,lr}
  00096ade: add r7,sp,#0xc
  00096ae0: push {r7,r8,r9,r10,r11}
  00096ae4: mov r11,r3
  00096ae6: mov r8,r2
  00096ae8: mov r5,r1
  00096aea: mov r6,r0
  00096aec: cbz r1,0x00096b6a
  00096aee: movs r0,#0x14
  00096af0: blx 0x0006eb24
  00096af4: movs r1,#0x0
  00096af6: mov r2,r0
  00096af8: strd r1,r1,[r0,#0x4]
  00096afc: lsl.w r9,r5,#0x1
  00096b00: str.w r1,[r2,#0x10]!
  00096b04: movs r3,#0x1f
  00096b06: str.w r0,[r11,#0x0]
  00096b0a: strh r5,[r0,#0x0]
  00096b0c: lsls r0,r5,#0x2
  00096b0e: str r2,[sp,#0x0]
  00096b10: uxth r1,r0
  00096b12: uxth.w r2,r9
  00096b16: mov r0,r6
  00096b18: blx 0x0006f1fc
  00096b1c: ldr.w r4,[r11,#0x0]
  00096b20: cmp r0,#0x1
  00096b22: mov r1,r4
  00096b24: ldr.w r2,[r1,#0x10]!
  00096b28: bne 0x00096b70
  00096b2a: add.w r10,r5,r5, lsl #0x1
  00096b2e: ldr r6,[r2,#0x2c]
  00096b30: lsl.w r5,r10,#0x2
  00096b34: mov r0,r5
  00096b36: blx 0x0006ec08
  00096b3a: str r0,[r4,#0x4]
  00096b3c: mov r1,r5
  00096b3e: ldr.w r0,[r11,#0x0]
  00096b42: ldr r0,[r0,#0x4]
  00096b44: blx 0x0006ec14
  00096b48: ldr.w r5,[r11,#0x0]
  00096b4c: cmp.w r8,#0x0
  00096b50: strb.w r8,[r5,#0xc]
  00096b54: beq 0x00096bbc
  00096b56: movs r0,#0x2
  00096b58: blx 0x0006ec08
  00096b5c: str r0,[r5,#0x8]
  00096b5e: movs r1,#0x0
  00096b60: ldr.w r0,[r11,#0x0]
  00096b64: ldr r0,[r0,#0x8]
  00096b66: strh r1,[r0,#0x0]
  00096b68: b 0x00096bd0
  00096b6a: mvn r0,#0x3
  00096b6e: b 0x00096c60
  00096b70: mov r0,r6
  00096b72: blx 0x0006ebd8
  00096b76: ldr.w r1,[r11,#0x0]
  00096b7a: ldr.w r0,[r1,#0x4]!
  00096b7e: cbz r0,0x00096b8a
  00096b80: blx 0x0006ebfc
  00096b84: ldr.w r0,[r11,#0x0]
  00096b88: adds r1,r0,#0x4
  00096b8a: movs r5,#0x0
  00096b8c: str r5,[r1,#0x0]
  00096b8e: ldr.w r1,[r11,#0x0]
  00096b92: ldr.w r0,[r1,#0x8]!
  00096b96: cbz r0,0x00096ba4
  00096b98: blx 0x0006ebfc
  00096b9c: ldr.w r0,[r11,#0x0]
  00096ba0: add.w r1,r0,#0x8
  00096ba4: str r5,[r1,#0x0]
  00096ba6: ldr.w r0,[r11,#0x0]
  00096baa: cbz r0,0x00096bb0
  00096bac: blx 0x0006eb48
  00096bb0: movs r0,#0x0
  00096bb2: str.w r0,[r11,#0x0]
  00096bb6: mov.w r0,#0xffffffff
  00096bba: b 0x00096c60
  00096bbc: mov r0,r9
  00096bbe: blx 0x0006ec08
  00096bc2: str r0,[r5,#0x8]
  00096bc4: mov r1,r9
  00096bc6: ldr.w r0,[r11,#0x0]
  00096bca: ldr r0,[r0,#0x8]
  00096bcc: blx 0x0006ec20
  00096bd0: lsl.w r0,r10,#0x1
  00096bd4: movs r1,#0x0
  00096bd6: movs r2,#0x0
  00096bd8: b 0x00096bfc
  00096bda: add.w r3,r6,r2, lsl #0x1
  00096bde: orr r5,r1,#0x1
  00096be2: strh.w r1,[r6,r2,lsl #0x1]
  00096be6: adds r2,#0x6
  00096be8: strh r5,[r3,#0x2]
  00096bea: orr r5,r1,#0x2
  00096bee: strh r5,[r3,#0x4]
  00096bf0: strh r1,[r3,#0x6]
  00096bf2: strh r5,[r3,#0x8]
  00096bf4: orr r5,r1,#0x3
  00096bf8: strh r5,[r3,#0xa]
  00096bfa: adds r1,#0x4
  00096bfc: uxth r2,r2
  00096bfe: cmp r2,r0
  00096c00: bcc 0x00096bda
  00096c02: ldr.w r0,[r11,#0x0]
  00096c06: movs r3,#0x0
  00096c08: mov.w r6,#0x3f800000
  00096c0c: ldr r0,[r0,#0x10]
  00096c0e: ldrh r1,[r0,#0x2]
  00096c10: lsls r2,r1,#0x2
  00096c12: b 0x00096c1c
  00096c14: ldr r4,[r0,#0xc]
  00096c16: adds r3,#0x1
  00096c18: str.w r6,[r4,r5,lsl #0x2]
  00096c1c: uxth r5,r3
  00096c1e: cmp r2,r5
  00096c20: bhi 0x00096c14
  00096c22: ldr r2,[0x00096c68]
  00096c24: movs r3,#0x0
  00096c26: mov.w r6,#0x3f800000
  00096c2a: movs r5,#0x0
  00096c2c: add r2,pc
  00096c2e: ldr r2,[r2,#0x0]
  00096c30: ldrb r2,[r2,#0x0]
  00096c32: b 0x00096c5a
  00096c34: ldr r4,[r0,#0x10]
  00096c36: cmp r2,#0x0
  00096c38: str r3,[r4,r5]
  00096c3a: add r4,r5
  00096c3c: strd r3,r6,[r4,#0x4]
  00096c40: beq 0x00096c56
  00096c42: ldr r4,[r0,#0x14]
  00096c44: str r6,[r4,r5]
  00096c46: add r4,r5
  00096c48: strd r3,r3,[r4,#0x4]
  00096c4c: ldr r4,[r0,#0x18]
  00096c4e: str r3,[r4,r5]
  00096c50: add r4,r5
  00096c52: strd r6,r3,[r4,#0x4]
  00096c56: adds r5,#0xc
  00096c58: subs r1,#0x1
  00096c5a: cmp r1,#0x0
  00096c5c: bne 0x00096c34
  00096c5e: movs r0,#0x1
  00096c60: pop.w {r3,r8,r9,r10,r11}
  00096c64: pop {r4,r5,r6,r7,pc}
```
