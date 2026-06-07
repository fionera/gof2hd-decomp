# WantedWindow::init
elf 0xe0b38 body 840
Sig: int __thiscall init(WantedWindow * this, EVP_PKEY_CTX * ctx)

## decompile
```c

/* WantedWindow::init() */

int __thiscall WantedWindow::init(WantedWindow *this,EVP_PKEY_CTX *ctx)

{
  Array<Wanted*> *this_00;
  uint *puVar1;
  int *piVar2;
  int iVar3;
  Array *pAVar4;
  TouchButton *pTVar5;
  String *pSVar6;
  void *pvVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int *piVar17;
  
  *(undefined4 *)(this + 0x91) = 0;
  *(undefined4 *)(this + 0x95) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x99) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x9d) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x8c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x90) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  this_00 = operator_new(0xc);
  Array<Wanted*>::Array(this_00);
  iVar3 = DAT_000f0e9c;
  *(Array<Wanted*> **)(this + 0x38) = this_00;
  puVar1 = (uint *)Status::getWanted((Status *)**(undefined4 **)(iVar3 + 0xf0b6a));
  piVar2 = *(int **)(DAT_000f0ea0 + 0xf0b7c);
  iVar3 = *piVar2;
  iVar16 = *(int *)(iVar3 + 0x28);
  iVar14 = *(int *)(iVar3 + 0xcc);
  iVar3 = *(int *)(iVar3 + 0x4c);
  for (uVar13 = 0; uVar13 < *puVar1; uVar13 = uVar13 + 1) {
    Status::getSystem();
    iVar10 = SolarSystem::getRace();
    iVar11 = Wanted::getBoard();
    if (iVar10 == iVar11) {
      Status::getSystem();
      iVar10 = SolarSystem::getRace();
      if ((iVar10 != 0) || (iVar10 = Status::getCurrentCampaignMission(), iVar10 < 0x80)) {
        Status::getSystem();
        iVar10 = SolarSystem::getRace();
        if ((iVar10 == 0) || (iVar10 = Status::getCurrentCampaignMission(), iVar10 < 0xa2))
        goto LAB_000f0be2;
      }
      ArrayAdd<Wanted*>(*(Wanted **)(puVar1[1] + uVar13 * 4),*(Array **)(this + 0x38));
    }
LAB_000f0be2:
  }
  if (**(char **)(DAT_000f0ea4 + 0xf0bf8) == '\0') {
    uVar13 = **(uint **)(DAT_000f0eb8 + 0xf0c22);
    uVar8 = **(uint **)(DAT_000f0ebc + 0xf0c24);
    *(undefined4 *)(this + 0x1c) = 0;
    *(undefined4 *)(this + 0x20) = 0;
  }
  else {
    if (**(char **)(DAT_000f0ea8 + 0xf0c06) == '\0') {
      uVar13 = 1000;
      uVar8 = 0x514;
      if (**(char **)(DAT_000f0eac + 0xf0c36) == '\0') {
        uVar13 = 500;
        uVar8 = 0x28a;
      }
      uVar9 = uVar8 >> 1;
    }
    else {
      uVar13 = 0x2bf;
      uVar9 = 0x1c9;
      uVar8 = 0x392;
    }
    iVar10 = **(int **)(DAT_000f0eb4 + 0xf0c5a);
    *(uint *)(this + 0x1c) = (**(int **)(DAT_000f0eb0 + 0xf0c58) >> 1) - uVar9;
    *(uint *)(this + 0x20) = (iVar10 >> 1) - (uVar13 >> 1);
  }
  uVar9 = 0;
  *(uint *)(this + 0x24) = uVar8;
  *(uint *)(this + 0x28) = uVar13;
  *(undefined4 *)(this + 0x30) = 0;
  do {
    puVar1 = *(uint **)(this + 0x38);
    uVar13 = *puVar1;
    if (uVar13 <= uVar9) {
      uVar9 = *(uint *)(this + 0x30);
LAB_000f0ca6:
      if (uVar9 == uVar13 - 1) {
        iVar10 = Wanted::isActive(*(Wanted **)(puVar1[1] + uVar9 * 4));
        if (iVar10 == 0) {
          uVar9 = 0;
          *(undefined4 *)(this + 0x30) = 0;
        }
        else {
          uVar9 = *(uint *)(this + 0x30);
        }
      }
      *(uint *)(this + 0x34) = uVar9;
      selectWanted(this,uVar9);
      pAVar4 = operator_new(0xc);
      Array<TouchButton*>::Array();
      *(Array **)(this + 0xc) = pAVar4;
      ArraySetLength<TouchButton*>(2,pAVar4);
      pTVar5 = operator_new(200);
      piVar17 = *(int **)(DAT_000f0ec0 + 0xf0cfa);
      pSVar6 = (String *)GameText::getText(*piVar17);
      iVar11 = *(int *)(this + 0x1c);
      iVar12 = *(int *)(this + 0x24);
      iVar10 = Layout::getHelpButtonOffset();
      TouchButton::TouchButton
                (pTVar5,pSVar6,3,(iVar12 + iVar11) - iVar10,*(int *)(this + 0x20),'\x12');
      *(TouchButton **)(*(int *)(*(int *)(this + 0xc) + 4) + 4) = pTVar5;
      pTVar5 = operator_new(200);
      pSVar6 = (String *)GameText::getText(*piVar17);
      iVar12 = *(int *)(this + 0x1c);
      iVar15 = *(int *)(this + 0x24);
      iVar10 = Layout::getHelpButtonOffset();
      iVar11 = TouchButton::getWidth();
      TouchButton::TouchButton
                (pTVar5,pSVar6,3,(((iVar15 + iVar12) - iVar10) - iVar11) + *(int *)(*piVar2 + 0x38),
                 *(int *)(this + 0x20),'\x12');
      **(undefined4 **)(*(int *)(this + 0xc) + 4) = pTVar5;
      TouchButton::setAlwaysPressed(*(TouchButton **)(*(int *)(*(int *)(this + 0xc) + 4) + 4),true);
      Layout::setWindowDimensions
                ((Layout *)*piVar2,*(int *)(this + 0x1c),*(int *)(this + 0x20),*(int *)(this + 0x24)
                 ,*(int *)(this + 0x28));
      iVar10 = *piVar2;
      *(int *)(this + 0xa4) =
           (*(int *)(iVar10 + 0x34) + *(int *)(iVar10 + 0x70)) * **(int **)(this + 0x38);
      *(int *)(this + 0xa8) =
           (((((*(int *)(this + 0x28) - *(int *)(iVar10 + 0x10)) - *(int *)(iVar10 + 0xc)) -
             *(int *)(iVar10 + 0x20)) - *(int *)(iVar10 + 0x24)) - *(int *)(iVar10 + 0x5c)) +
           *(int *)(iVar10 + 0x2c);
      if (*(TouchButton **)(this + 0x18) != (TouchButton *)0x0) {
        pvVar7 = (void *)TouchButton::~TouchButton(*(TouchButton **)(this + 0x18));
        operator_delete(pvVar7);
      }
      *(undefined4 *)(this + 4) = 0;
      *(undefined4 *)(this + 0x18) = 0;
      *(undefined2 *)(this + 0x14) = 0;
      pTVar5 = operator_new(200);
      pSVar6 = (String *)GameText::getText(*piVar17);
      iVar10 = *piVar2;
      TouchButton::TouchButton
                (pTVar5,pSVar6,0,
                 *(int *)(this + 0x1c) + (*(int *)(this + 0x24) >> 1) + *(int *)(iVar10 + 0x2c),
                 (((*(int *)(this + 0x20) - *(int *)(iVar10 + 0x2c)) + *(int *)(this + 0x28)) -
                 *(int *)(iVar10 + 0x10)) - *(int *)(iVar10 + 0x24),(iVar14 - iVar16) - iVar3,'!',
                 '\x04');
      this[0x15] = (WantedWindow)0x0;
      *(TouchButton **)(this + 0x18) = pTVar5;
      *(undefined4 *)this = 1;
      return 1;
    }
    iVar10 = Wanted::isActive(*(Wanted **)(puVar1[1] + uVar9 * 4));
    if (iVar10 != 0) {
      puVar1 = *(uint **)(this + 0x38);
      *(uint *)(this + 0x30) = uVar9;
      uVar13 = *puVar1;
      goto LAB_000f0ca6;
    }
    uVar9 = uVar9 + 1;
  } while( true );
}

```

## target disasm
```
  000f0b38: push {r4,r5,r6,r7,lr}
  000f0b3a: add r7,sp,#0xc
  000f0b3c: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000f0b40: vmov.i32 q8,#0x0
  000f0b44: mov r10,r0
  000f0b46: adds r0,#0x91
  000f0b48: vst1.8 {d16,d17},[r0]
  000f0b4c: add.w r0,r10,#0x84
  000f0b50: vst1.32 {d16,d17},[r0]
  000f0b54: movs r0,#0xc
  000f0b56: blx 0x0006eb24
  000f0b5a: mov r5,r0
  000f0b5c: blx 0x000748e4
  000f0b60: ldr r0,[0x000f0e9c]
  000f0b62: str.w r5,[r10,#0x38]
  000f0b66: add r0,pc
  000f0b68: ldr r4,[r0,#0x0]
  000f0b6a: ldr r0,[r4,#0x0]
  000f0b6c: blx 0x0007450c
  000f0b70: mov r5,r0
  000f0b72: ldr r0,[0x000f0ea0]
  000f0b74: mov.w r8,#0x0
  000f0b78: add r0,pc
  000f0b7a: ldr r0,[r0,#0x0]
  000f0b7c: str r0,[sp,#0x18]
  000f0b7e: ldr r0,[r0,#0x0]
  000f0b80: ldr.w r11,[r0,#0x28]
  000f0b84: ldr.w r9,[r0,#0xcc]
  000f0b88: ldr r1,[r0,#0x4c]
  000f0b8a: str r1,[sp,#0x14]
  000f0b8c: b 0x000f0be6
  000f0b8e: ldr r0,[r4,#0x0]
  000f0b90: blx 0x00071a10
  000f0b94: blx 0x00071950
  000f0b98: mov r6,r0
  000f0b9a: ldr r0,[r5,#0x4]
  000f0b9c: ldr.w r0,[r0,r8,lsl #0x2]
  000f0ba0: blx 0x00073a08
  000f0ba4: cmp r6,r0
  000f0ba6: bne 0x000f0be2
  000f0ba8: ldr r0,[r4,#0x0]
  000f0baa: blx 0x00071a10
  000f0bae: blx 0x00071950
  000f0bb2: cbnz r0,0x000f0bbe
  000f0bb4: ldr r0,[r4,#0x0]
  000f0bb6: blx 0x00071770
  000f0bba: cmp r0,#0x7f
  000f0bbc: bgt 0x000f0bd4
  000f0bbe: ldr r0,[r4,#0x0]
  000f0bc0: blx 0x00071a10
  000f0bc4: blx 0x00071950
  000f0bc8: cbz r0,0x000f0be2
  000f0bca: ldr r0,[r4,#0x0]
  000f0bcc: blx 0x00071770
  000f0bd0: cmp r0,#0xa2
  000f0bd2: blt 0x000f0be2
  000f0bd4: ldr r0,[r5,#0x4]
  000f0bd6: ldr.w r1,[r10,#0x38]
  000f0bda: ldr.w r0,[r0,r8,lsl #0x2]
  000f0bde: blx 0x00075aa8
  000f0be2: add.w r8,r8,#0x1
  000f0be6: ldr r0,[r5,#0x0]
  000f0be8: cmp r8,r0
  000f0bea: bcc 0x000f0b8e
  000f0bec: ldr r0,[0x000f0ea4]
  000f0bee: sub.w r1,r9,r11
  000f0bf2: ldr r2,[sp,#0x14]
  000f0bf4: add r0,pc
  000f0bf6: sub.w r8,r1,r2
  000f0bfa: ldr r0,[r0,#0x0]
  000f0bfc: ldrb r0,[r0,#0x0]
  000f0bfe: cbz r0,0x000f0c18
  000f0c00: ldr r0,[0x000f0ea8]
  000f0c02: add r0,pc
  000f0c04: ldr r0,[r0,#0x0]
  000f0c06: ldrb r0,[r0,#0x0]
  000f0c08: cbz r0,0x000f0c30
  000f0c0a: movw r0,#0x2bf
  000f0c0e: movw r2,#0x1c9
  000f0c12: movw r1,#0x392
  000f0c16: b 0x000f0c50
  000f0c18: ldr r0,[0x000f0eb8]
  000f0c1a: movs r2,#0x0
  000f0c1c: ldr r1,[0x000f0ebc]
  000f0c1e: add r0,pc
  000f0c20: add r1,pc
  000f0c22: ldr r0,[r0,#0x0]
  000f0c24: ldr r1,[r1,#0x0]
  000f0c26: ldr r0,[r0,#0x0]
  000f0c28: ldr r1,[r1,#0x0]
  000f0c2a: strd r2,r2,[r10,#0x1c]
  000f0c2e: b 0x000f0c6e
  000f0c30: ldr r0,[0x000f0eac]
  000f0c32: add r0,pc
  000f0c34: ldr r0,[r0,#0x0]
  000f0c36: ldrb r1,[r0,#0x0]
  000f0c38: mov.w r0,#0x3e8
  000f0c3c: cmp r1,#0x0
  000f0c3e: movw r1,#0x514
  000f0c42: it eq
  000f0c44: mov.eq.w r0,#0x1f4
  000f0c48: it eq
  000f0c4a: movw.eq r1,#0x28a
  000f0c4e: lsrs r2,r1,#0x1
  000f0c50: ldr r3,[0x000f0eb0]
  000f0c52: ldr r4,[0x000f0eb4]
  000f0c54: add r3,pc
  000f0c56: add r4,pc
  000f0c58: ldr r3,[r3,#0x0]
  000f0c5a: ldr r6,[r4,#0x0]
  000f0c5c: ldr r3,[r3,#0x0]
  000f0c5e: rsb r2,r2,r3, asr #0x1
  000f0c62: ldr r3,[r6,#0x0]
  000f0c64: asrs r3,r3,#0x1
  000f0c66: sub.w r3,r3,r0, lsr #0x1
  000f0c6a: strd r2,r3,[r10,#0x1c]
  000f0c6e: movs r5,#0x0
  000f0c70: str.w r1,[r10,#0x24]
  000f0c74: str.w r0,[r10,#0x28]
  000f0c78: str.w r5,[r10,#0x30]
  000f0c7c: b 0x000f0c8c
  000f0c7e: ldr r0,[r0,#0x4]
  000f0c80: ldr.w r0,[r0,r5,lsl #0x2]
  000f0c84: blx 0x00073774
  000f0c88: cbnz r0,0x000f0c9c
  000f0c8a: adds r5,#0x1
  000f0c8c: ldr.w r0,[r10,#0x38]
  000f0c90: ldr r1,[r0,#0x0]
  000f0c92: cmp r5,r1
  000f0c94: bcc 0x000f0c7e
  000f0c96: ldr.w r5,[r10,#0x30]
  000f0c9a: b 0x000f0ca6
  000f0c9c: ldr.w r0,[r10,#0x38]
  000f0ca0: str.w r5,[r10,#0x30]
  000f0ca4: ldr r1,[r0,#0x0]
  000f0ca6: subs r1,#0x1
  000f0ca8: cmp r5,r1
  000f0caa: bne 0x000f0cc4
  000f0cac: ldr r0,[r0,#0x4]
  000f0cae: ldr.w r0,[r0,r5,lsl #0x2]
  000f0cb2: blx 0x00073774
  000f0cb6: cmp r0,#0x0
  000f0cb8: itte eq
  000f0cba: mov.eq r5,#0x0
  000f0cbc: str.w.eq r5,[r10,#0x30]
  000f0cc0: ldr.ne.w r5,[r10,#0x30]
  000f0cc4: mov r0,r10
  000f0cc6: mov r1,r5
  000f0cc8: str.w r5,[r10,#0x34]
  000f0ccc: blx 0x00075ab4
  000f0cd0: movs r0,#0xc
  000f0cd2: blx 0x0006eb24
  000f0cd6: mov r5,r0
  000f0cd8: blx 0x00074f38
  000f0cdc: movs r0,#0x2
  000f0cde: mov r1,r5
  000f0ce0: str.w r8,[sp,#0x14]
  000f0ce4: str.w r5,[r10,#0xc]
  000f0ce8: blx 0x00074f44
  000f0cec: movs r0,#0xc8
  000f0cee: blx 0x0006eb24
  000f0cf2: mov r5,r0
  000f0cf4: ldr r0,[0x000f0ec0]
  000f0cf6: add r0,pc
  000f0cf8: ldr.w r11,[r0,#0x0]
  000f0cfc: ldr.w r0,[r11,#0x0]
  000f0d00: movw r1,#0xc93
  000f0d04: blx 0x00072f70
  000f0d08: mov r8,r0
  000f0d0a: ldr r0,[sp,#0x18]
  000f0d0c: ldr.w r4,[r10,#0x1c]
  000f0d10: ldr.w r6,[r10,#0x24]
  000f0d14: ldr r0,[r0,#0x0]
  000f0d16: blx 0x00075ac0
  000f0d1a: ldr.w r1,[r10,#0x20]
  000f0d1e: movs r2,#0x12
  000f0d20: strd r1,r2,[sp,#0x0]
  000f0d24: adds r1,r6,r4
  000f0d26: subs r3,r1,r0
  000f0d28: mov r0,r5
  000f0d2a: mov r1,r8
  000f0d2c: movs r2,#0x3
  000f0d2e: blx 0x00074578
  000f0d32: ldr.w r0,[r10,#0xc]
  000f0d36: ldr r0,[r0,#0x4]
  000f0d38: str r5,[r0,#0x4]
  000f0d3a: movs r0,#0xc8
  000f0d3c: blx 0x0006eb24
  000f0d40: mov r5,r0
  000f0d42: ldr.w r0,[r11,#0x0]
  000f0d46: movs r1,#0x81
  000f0d48: blx 0x00072f70
  000f0d4c: mov r8,r0
  000f0d4e: ldr r0,[sp,#0x18]
  000f0d50: ldr.w r4,[r10,#0x1c]
  000f0d54: ldr.w r9,[r10,#0x24]
  000f0d58: ldr r0,[r0,#0x0]
  000f0d5a: blx 0x00075ac0
  000f0d5e: mov r6,r0
  000f0d60: ldr.w r0,[r10,#0xc]
  000f0d64: ldr r0,[r0,#0x4]
  000f0d66: ldr r0,[r0,#0x4]
  000f0d68: blx 0x00074da0
  000f0d6c: ldr r1,[sp,#0x18]
  000f0d6e: ldr.w r2,[r10,#0x20]
  000f0d72: ldr r1,[r1,#0x0]
  000f0d74: ldr r1,[r1,#0x38]
  000f0d76: movs r3,#0x12
  000f0d78: strd r2,r3,[sp,#0x0]
  000f0d7c: add.w r2,r9,r4
  000f0d80: subs r2,r2,r6
  000f0d82: subs r0,r2,r0
  000f0d84: movs r2,#0x3
  000f0d86: adds r3,r0,r1
  000f0d88: mov r0,r5
  000f0d8a: mov r1,r8
  000f0d8c: blx 0x00074578
  000f0d90: ldr.w r0,[r10,#0xc]
  000f0d94: movs r1,#0x1
  000f0d96: ldr r0,[r0,#0x4]
  000f0d98: str r5,[r0,#0x0]
  000f0d9a: ldr.w r0,[r10,#0xc]
  000f0d9e: ldr r0,[r0,#0x4]
  000f0da0: ldr r0,[r0,#0x4]
  000f0da2: blx 0x00075acc
  000f0da6: add.w r6,r10,#0x1c
  000f0daa: ldmia r6,{r1,r2,r3,r6}
  000f0dac: ldr r4,[sp,#0x18]
  000f0dae: str r6,[sp,#0x0]
  000f0db0: ldr r0,[r4,#0x0]
  000f0db2: blx 0x00075ad8
  000f0db6: ldr r1,[r4,#0x0]
  000f0db8: ldr.w r3,[r10,#0x38]
  000f0dbc: ldr.w r0,[r10,#0x18]
  000f0dc0: ldr r6,[r1,#0x34]
  000f0dc2: ldr r5,[r1,#0x70]
  000f0dc4: cmp r0,#0x0
  000f0dc6: ldr r3,[r3,#0x0]
  000f0dc8: add r6,r5
  000f0dca: ldr.w r2,[r10,#0x28]
  000f0dce: mul r3,r6,r3
  000f0dd2: str.w r3,[r10,#0xa4]
  000f0dd6: ldrd r3,r6,[r1,#0xc]
  000f0dda: sub.w r2,r2,r6
  000f0dde: ldrd r5,r4,[r1,#0x20]
  000f0de2: sub.w r2,r2,r3
  000f0de6: ldr r3,[r1,#0x2c]
  000f0de8: sub.w r2,r2,r5
  000f0dec: ldr r1,[r1,#0x5c]
  000f0dee: sub.w r2,r2,r4
  000f0df2: sub.w r1,r2,r1
  000f0df6: add r1,r3
  000f0df8: str.w r1,[r10,#0xa8]
  000f0dfc: beq 0x000f0e06
  000f0dfe: blx 0x00074d7c
  000f0e02: blx 0x0006eb48
  000f0e06: movs r0,#0x0
  000f0e08: str.w r0,[r10,#0x4]
  000f0e0c: str.w r0,[r10,#0x18]
  000f0e10: strh.w r0,[r10,#0x14]
  000f0e14: movs r0,#0xc8
  000f0e16: blx 0x0006eb24
  000f0e1a: mov r8,r0
  000f0e1c: ldr.w r0,[r11,#0x0]
  000f0e20: mov.w r1,#0x1a8
  000f0e24: blx 0x00072f70
  000f0e28: mov r12,r0
  000f0e2a: ldr r0,[sp,#0x18]
  000f0e2c: add.w r6,r10,#0x1c
  000f0e30: ldr r0,[r0,#0x0]
  000f0e32: ldmia r6,{r2,r3,r6}
  000f0e34: ldr.w r4,[r10,#0x28]
  000f0e38: ldr r1,[r0,#0x10]
  000f0e3a: ldr r5,[r0,#0x24]
  000f0e3c: ldr r0,[r0,#0x2c]
  000f0e3e: subs r3,r3,r0
  000f0e40: mov.w lr,#0x4
  000f0e44: add r3,r4
  000f0e46: mov.w r9,#0x21
  000f0e4a: subs r1,r3,r1
  000f0e4c: add r3,sp,#0x4
  000f0e4e: subs r1,r1,r5
  000f0e50: str r1,[sp,#0x0]
  000f0e52: ldr r1,[sp,#0x14]
  000f0e54: movs r4,#0x0
  000f0e56: stm r3,{r1,r9,lr}
  000f0e5a: add.w r1,r2,r6, asr #0x1
  000f0e5e: adds r3,r1,r0
  000f0e60: mov r0,r8
  000f0e62: mov r1,r12
  000f0e64: movs r2,#0x0
  000f0e66: blx 0x00074f50
  000f0e6a: movs r0,#0x1
  000f0e6c: strb.w r4,[r10,#0x15]
  000f0e70: str.w r8,[r10,#0x18]
  000f0e74: str.w r0,[r10,#0x0]
  000f0e78: add sp,#0x1c
  000f0e7a: pop.w {r8,r9,r10,r11}
  000f0e7e: pop {r4,r5,r6,r7,pc}
```
