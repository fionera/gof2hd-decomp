# Transform::UpdateKeyFrames
elf 0x75a08 body 1288
Sig: undefined __thiscall UpdateKeyFrames(Transform * this, KeyFrame * param_1, int param_2)

## decompile
```c

/* AbyssEngine::Transform::UpdateKeyFrames(AbyssEngine::KeyFrame*, int) */

void __thiscall
AbyssEngine::Transform::UpdateKeyFrames(Transform *this,KeyFrame *param_1,int param_2)

{
  float fVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  float *pfVar5;
  int iVar6;
  float *pfVar7;
  uint uVar8;
  float fVar9;
  int iVar10;
  
  iVar3 = 0;
  while (iVar3 + 1 < param_2) {
    iVar6 = *(int *)(this + 0x120);
    pfVar5 = *(float **)(iVar6 + iVar3 * 4 + 4);
    fVar9 = *(float *)(param_1 + 0x50);
    iVar10 = *(int *)(param_1 + 0x54);
    fVar1 = (float)__aeabi_l2f((int)fVar9 - (int)pfVar5[0x14],
                               (iVar10 - (int)pfVar5[0x15]) -
                               (uint)((uint)fVar9 < (uint)pfVar5[0x14]));
    pfVar7 = *(float **)(iVar6 + iVar3 * 4);
    fVar9 = (float)__aeabi_l2f((int)fVar9 - (int)pfVar7[0x14],
                               (iVar10 - (int)pfVar7[0x15]) -
                               (uint)((uint)fVar9 < (uint)pfVar7[0x14]));
    uVar8 = *(uint *)(param_1 + 0x58);
    uVar2 = *(uint *)(param_1 + 0x5c);
    fVar1 = 1.0 - fVar1 / fVar9;
    if (((uVar8 & 0x40) != 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x19))) {
      pfVar5[6] = pfVar7[6] + fVar1 * (*(float *)(param_1 + 0x18) - pfVar7[6]);
    }
    if (((int)(uVar8 << 0x18) < 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x18))) {
      pfVar5[7] = pfVar7[7] + fVar1 * (*(float *)(param_1 + 0x1c) - pfVar7[7]);
    }
    if (((int)(uVar8 << 0x17) < 0) && ((*(byte *)((int)pfVar5 + 0x59) & 1) == 0)) {
      pfVar5[8] = pfVar7[8] + fVar1 * (*(float *)(param_1 + 0x20) - pfVar7[8]);
    }
    if (((uVar8 & 1) != 0) && (((uint)pfVar5[0x16] & 1) == 0)) {
      *pfVar5 = *pfVar7 + fVar1 * (*(float *)param_1 - *pfVar7);
    }
    if (((int)(uVar8 << 0x1e) < 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x1e))) {
      pfVar5[1] = pfVar7[1] + fVar1 * (*(float *)(param_1 + 4) - pfVar7[1]);
    }
    if (((int)(uVar8 << 0x1d) < 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x1d))) {
      pfVar5[2] = pfVar7[2] + fVar1 * (*(float *)(param_1 + 8) - pfVar7[2]);
    }
    if (((int)(uVar8 << 0x1c) < 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x1c))) {
      pfVar5[3] = pfVar7[3] + fVar1 * (*(float *)(param_1 + 0xc) - pfVar7[3]);
    }
    if (((int)(uVar8 << 0x1b) < 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x1b))) {
      pfVar5[4] = pfVar7[4] + fVar1 * (*(float *)(param_1 + 0x10) - pfVar7[4]);
    }
    if (((int)(uVar8 << 0x1a) < 0) && (-1 < (int)((uint)*(byte *)(pfVar5 + 0x16) << 0x1a))) {
      pfVar5[5] = pfVar7[5] + fVar1 * (*(float *)(param_1 + 0x14) - pfVar7[5]);
    }
    if (((int)(uVar8 << 0x16) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x59) << 0x1e))) {
      pfVar5[0x12] = pfVar7[0x12] + fVar1 * (*(float *)(param_1 + 0x48) - pfVar7[0x12]);
    }
    if (((int)(uVar8 << 0xf) < 0) && ((*(byte *)((int)pfVar5 + 0x5a) & 1) == 0)) {
      pfVar5[0xf] = pfVar7[0xf] + fVar1 * (*(float *)(param_1 + 0x3c) - pfVar7[0xf]);
    }
    if (((int)(uVar8 << 0xe) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x5a) << 0x1e))) {
      pfVar5[0x10] = pfVar7[0x10] + fVar1 * (*(float *)(param_1 + 0x40) - pfVar7[0x10]);
    }
    if (((int)(uVar8 << 0xd) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x5a) << 0x1d))) {
      pfVar5[0x11] = pfVar7[0x11] + fVar1 * (*(float *)(param_1 + 0x44) - pfVar7[0x11]);
    }
    if (((int)(uVar8 << 0x15) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x59) << 0x1d))) {
      pfVar5[9] = pfVar7[9] + fVar1 * (*(float *)(param_1 + 0x24) - pfVar7[9]);
    }
    if (((int)(uVar8 << 0x14) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x59) << 0x1c))) {
      pfVar5[10] = pfVar7[10] + fVar1 * (*(float *)(param_1 + 0x28) - pfVar7[10]);
    }
    if (((int)(uVar8 << 0x13) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x59) << 0x1b))) {
      pfVar5[0xb] = pfVar7[0xb] + fVar1 * (*(float *)(param_1 + 0x2c) - pfVar7[0xb]);
    }
    if (((int)(uVar8 << 0x12) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x59) << 0x1a))) {
      pfVar5[0xc] = pfVar7[0xc] + fVar1 * (*(float *)(param_1 + 0x30) - pfVar7[0xc]);
    }
    if (((int)(uVar8 << 0x11) < 0) && (-1 < (int)((uint)*(byte *)((int)pfVar5 + 0x59) << 0x19))) {
      pfVar5[0xd] = pfVar7[0xd] + fVar1 * (*(float *)(param_1 + 0x34) - pfVar7[0xd]);
    }
    fVar9 = pfVar5[0x16];
    if ((int)(uVar8 << 0x10) < 0) {
      if (((uint)fVar9 & 0x8000) == 0) {
        pfVar5[0xe] = pfVar7[0xe] + fVar1 * (*(float *)(param_1 + 0x38) - pfVar7[0xe]);
      }
    }
    pfVar5[0x16] = (float)(uVar8 | (uint)fVar9);
    pfVar5[0x17] = (float)((uint)pfVar5[0x17] | uVar2);
    iVar3 = iVar3 + 1;
  }
  uVar8 = *(uint *)(this + 0x11c);
  while (param_2 = param_2 + 1, (uint)param_2 < uVar8) {
    uVar2 = *(uint *)(param_1 + 0x58);
    if (((uVar2 & 0x40) != 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x19))) {
      *(undefined4 *)(iVar3 + 0x18) = *(undefined4 *)(param_1 + 0x18);
    }
    if (((int)(uVar2 << 0x18) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x18))) {
      *(undefined4 *)(iVar3 + 0x1c) = *(undefined4 *)(param_1 + 0x1c);
    }
    if (((int)(uVar2 << 0x17) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4), (*(byte *)(iVar3 + 0x59) & 1) == 0))
    {
      *(undefined4 *)(iVar3 + 0x20) = *(undefined4 *)(param_1 + 0x20);
    }
    if (((uVar2 & 1) != 0) &&
       (puVar4 = *(undefined4 **)(*(int *)(this + 0x120) + param_2 * 4),
       (*(byte *)(puVar4 + 0x16) & 1) == 0)) {
      *puVar4 = *(undefined4 *)param_1;
    }
    if (((int)(uVar2 << 0x1e) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x1e))) {
      *(undefined4 *)(iVar3 + 4) = *(undefined4 *)(param_1 + 4);
    }
    if (((int)(uVar2 << 0x1d) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x1d))) {
      *(undefined4 *)(iVar3 + 8) = *(undefined4 *)(param_1 + 8);
    }
    if (((int)(uVar2 << 0x1c) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x1c))) {
      *(undefined4 *)(iVar3 + 0xc) = *(undefined4 *)(param_1 + 0xc);
    }
    if (((int)(uVar2 << 0x1b) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x1b))) {
      *(undefined4 *)(iVar3 + 0x10) = *(undefined4 *)(param_1 + 0x10);
    }
    if (((int)(uVar2 << 0x1a) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x58) << 0x1a))) {
      *(undefined4 *)(iVar3 + 0x14) = *(undefined4 *)(param_1 + 0x14);
    }
    if (((int)(uVar2 << 0x16) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x1e))) {
      *(undefined4 *)(iVar3 + 0x48) = *(undefined4 *)(param_1 + 0x48);
    }
    if (((int)(uVar2 << 0xf) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4), (*(byte *)(iVar3 + 0x5a) & 1) == 0))
    {
      *(undefined4 *)(iVar3 + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
    }
    if (((int)(uVar2 << 0xe) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x5a) << 0x1e))) {
      *(undefined4 *)(iVar3 + 0x40) = *(undefined4 *)(param_1 + 0x40);
    }
    if (((int)(uVar2 << 0xd) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x5a) << 0x1d))) {
      *(undefined4 *)(iVar3 + 0x44) = *(undefined4 *)(param_1 + 0x44);
    }
    if (((int)(uVar2 << 0x15) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x1d))) {
      *(undefined4 *)(iVar3 + 0x24) = *(undefined4 *)(param_1 + 0x24);
    }
    if (((int)(uVar2 << 0x14) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x1c))) {
      *(undefined4 *)(iVar3 + 0x28) = *(undefined4 *)(param_1 + 0x28);
    }
    if (((int)(uVar2 << 0x13) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x1b))) {
      *(undefined4 *)(iVar3 + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
    }
    if (((int)(uVar2 << 0x12) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x1a))) {
      *(undefined4 *)(iVar3 + 0x30) = *(undefined4 *)(param_1 + 0x30);
    }
    if (((int)(uVar2 << 0x11) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x19))) {
      *(undefined4 *)(iVar3 + 0x34) = *(undefined4 *)(param_1 + 0x34);
    }
    if (((int)(uVar2 << 0x10) < 0) &&
       (iVar3 = *(int *)(*(int *)(this + 0x120) + param_2 * 4),
       -1 < (int)((uint)*(byte *)(iVar3 + 0x59) << 0x18))) {
      *(undefined4 *)(iVar3 + 0x38) = *(undefined4 *)(param_1 + 0x38);
    }
  }
  return;
}

```

## target disasm
```
  00085a08: push {r4,r5,r6,r7,lr}
  00085a0a: add r7,sp,#0xc
  00085a0c: push {r7,r8,r9,r10,r11}
  00085a10: vpush {d5,d6,d7,d8}
  00085a14: vmov.f32 s16,0x3f800000
  00085a18: add.w r10,r1,#0x58
  00085a1c: mov r12,r1
  00085a1e: movs r6,#0x0
  00085a20: adds r1,#0x50
  00085a22: str r2,[sp,#0xc]
  00085a24: str r1,[sp,#0x4]
  00085a26: str r0,[sp,#0x8]
  00085a28: b 0x00085cf6
  00085a2a: ldr.w r5,[r0,#0x120]
  00085a2e: mov r8,r12
  00085a30: str r2,[sp,#0x14]
  00085a32: add.w r0,r5,r6, lsl #0x2
  00085a36: ldr r4,[r0,#0x4]
  00085a38: ldr r0,[sp,#0x4]
  00085a3a: ldrd r9,r11,[r0,#0x0]
  00085a3e: ldrd r0,r1,[r4,#0x50]
  00085a42: subs.w r0,r9,r0
  00085a46: sbc.w r1,r11,r1
  00085a4a: blx 0x0006f7d8
  00085a4e: ldr.w r5,[r5,r6,lsl #0x2]
  00085a52: str r0,[sp,#0x10]
  00085a54: ldrd r0,r1,[r5,#0x50]
  00085a58: subs.w r0,r9,r0
  00085a5c: sbc.w r1,r11,r1
  00085a60: blx 0x0006f7d8
  00085a64: vmov s0,r0
  00085a68: ldr r0,[sp,#0x10]
  00085a6a: ldrd r1,lr,[r10,#0x0]
  00085a6e: mov r12,r8
  00085a70: vmov s2,r0
  00085a74: tst r1,#0x40
  00085a78: vdiv.f32 s0,s2,s0
  00085a7c: vsub.f32 s0,s16,s0
  00085a80: beq 0x00085a9e
  00085a82: ldrb.w r2,[r4,#0x58]
  00085a86: lsls r2,r2,#0x19
  00085a88: bmi 0x00085a9e
  00085a8a: vldr.32 s2,[r5,#0x18]
  00085a8e: vldr.32 s4,[r12,#0x18]
  00085a92: vsub.f32 s4,s4,s2
  00085a96: vmla.f32 s2,s0,s4
  00085a9a: vstr.32 s2,[r4,#0x18]
  00085a9e: lsls r2,r1,#0x18
  00085aa0: bpl 0x00085abe
  00085aa2: ldrb.w r2,[r4,#0x58]
  00085aa6: lsls r2,r2,#0x18
  00085aa8: bmi 0x00085abe
  00085aaa: vldr.32 s2,[r5,#0x1c]
  00085aae: vldr.32 s4,[r12,#0x1c]
  00085ab2: vsub.f32 s4,s4,s2
  00085ab6: vmla.f32 s2,s0,s4
  00085aba: vstr.32 s2,[r4,#0x1c]
  00085abe: ldr r0,[sp,#0x8]
  00085ac0: lsls r2,r1,#0x17
  00085ac2: bpl 0x00085ae0
  00085ac4: ldrb.w r2,[r4,#0x59]
  00085ac8: lsls r2,r2,#0x1f
  00085aca: bne 0x00085ae0
  00085acc: vldr.32 s2,[r5,#0x20]
  00085ad0: vldr.32 s4,[r12,#0x20]
  00085ad4: vsub.f32 s4,s4,s2
  00085ad8: vmla.f32 s2,s0,s4
  00085adc: vstr.32 s2,[r4,#0x20]
  00085ae0: lsls r2,r1,#0x1f
  00085ae2: beq 0x00085b00
  00085ae4: ldrb.w r2,[r4,#0x58]
  00085ae8: lsls r2,r2,#0x1f
  00085aea: bne 0x00085b00
  00085aec: vldr.32 s2,[r5]
  00085af0: vldr.32 s4,[r12]
  00085af4: vsub.f32 s4,s4,s2
  00085af8: vmla.f32 s2,s0,s4
  00085afc: vstr.32 s2,[r4]
  00085b00: lsls r2,r1,#0x1e
  00085b02: bpl 0x00085b20
  00085b04: ldrb.w r2,[r4,#0x58]
  00085b08: lsls r2,r2,#0x1e
  00085b0a: bmi 0x00085b20
  00085b0c: vldr.32 s2,[r5,#0x4]
  00085b10: vldr.32 s4,[r12,#0x4]
  00085b14: vsub.f32 s4,s4,s2
  00085b18: vmla.f32 s2,s0,s4
  00085b1c: vstr.32 s2,[r4,#0x4]
  00085b20: lsls r2,r1,#0x1d
  00085b22: bpl 0x00085b40
  00085b24: ldrb.w r2,[r4,#0x58]
  00085b28: lsls r2,r2,#0x1d
  00085b2a: bmi 0x00085b40
  00085b2c: vldr.32 s2,[r5,#0x8]
  00085b30: vldr.32 s4,[r12,#0x8]
  00085b34: vsub.f32 s4,s4,s2
  00085b38: vmla.f32 s2,s0,s4
  00085b3c: vstr.32 s2,[r4,#0x8]
  00085b40: lsls r2,r1,#0x1c
  00085b42: bpl 0x00085b60
  00085b44: ldrb.w r2,[r4,#0x58]
  00085b48: lsls r2,r2,#0x1c
  00085b4a: bmi 0x00085b60
  00085b4c: vldr.32 s2,[r5,#0xc]
  00085b50: vldr.32 s4,[r12,#0xc]
  00085b54: vsub.f32 s4,s4,s2
  00085b58: vmla.f32 s2,s0,s4
  00085b5c: vstr.32 s2,[r4,#0xc]
  00085b60: lsls r2,r1,#0x1b
  00085b62: bpl 0x00085b80
  00085b64: ldrb.w r2,[r4,#0x58]
  00085b68: lsls r2,r2,#0x1b
  00085b6a: bmi 0x00085b80
  00085b6c: vldr.32 s2,[r5,#0x10]
  00085b70: vldr.32 s4,[r12,#0x10]
  00085b74: vsub.f32 s4,s4,s2
  00085b78: vmla.f32 s2,s0,s4
  00085b7c: vstr.32 s2,[r4,#0x10]
  00085b80: lsls r2,r1,#0x1a
  00085b82: bpl 0x00085ba0
  00085b84: ldrb.w r2,[r4,#0x58]
  00085b88: lsls r2,r2,#0x1a
  00085b8a: bmi 0x00085ba0
  00085b8c: vldr.32 s2,[r5,#0x14]
  00085b90: vldr.32 s4,[r12,#0x14]
  00085b94: vsub.f32 s4,s4,s2
  00085b98: vmla.f32 s2,s0,s4
  00085b9c: vstr.32 s2,[r4,#0x14]
  00085ba0: lsls r2,r1,#0x16
  00085ba2: bpl 0x00085bc0
  00085ba4: ldrb.w r2,[r4,#0x59]
  00085ba8: lsls r2,r2,#0x1e
  00085baa: bmi 0x00085bc0
  00085bac: vldr.32 s2,[r5,#0x48]
  00085bb0: vldr.32 s4,[r12,#0x48]
  00085bb4: vsub.f32 s4,s4,s2
  00085bb8: vmla.f32 s2,s0,s4
  00085bbc: vstr.32 s2,[r4,#0x48]
  00085bc0: lsls r2,r1,#0xf
  00085bc2: bpl 0x00085be0
  00085bc4: ldrb.w r2,[r4,#0x5a]
  00085bc8: lsls r2,r2,#0x1f
  00085bca: bne 0x00085be0
  00085bcc: vldr.32 s2,[r5,#0x3c]
  00085bd0: vldr.32 s4,[r12,#0x3c]
  00085bd4: vsub.f32 s4,s4,s2
  00085bd8: vmla.f32 s2,s0,s4
  00085bdc: vstr.32 s2,[r4,#0x3c]
  00085be0: lsls r2,r1,#0xe
  00085be2: bpl 0x00085c00
  00085be4: ldrb.w r2,[r4,#0x5a]
  00085be8: lsls r2,r2,#0x1e
  00085bea: bmi 0x00085c00
  00085bec: vldr.32 s2,[r5,#0x40]
  00085bf0: vldr.32 s4,[r12,#0x40]
  00085bf4: vsub.f32 s4,s4,s2
  00085bf8: vmla.f32 s2,s0,s4
  00085bfc: vstr.32 s2,[r4,#0x40]
  00085c00: lsls r2,r1,#0xd
  00085c02: bpl 0x00085c20
  00085c04: ldrb.w r2,[r4,#0x5a]
  00085c08: lsls r2,r2,#0x1d
  00085c0a: bmi 0x00085c20
  00085c0c: vldr.32 s2,[r5,#0x44]
  00085c10: vldr.32 s4,[r12,#0x44]
  00085c14: vsub.f32 s4,s4,s2
  00085c18: vmla.f32 s2,s0,s4
  00085c1c: vstr.32 s2,[r4,#0x44]
  00085c20: lsls r2,r1,#0x15
  00085c22: bpl 0x00085c40
  00085c24: ldrb.w r2,[r4,#0x59]
  00085c28: lsls r2,r2,#0x1d
  00085c2a: bmi 0x00085c40
  00085c2c: vldr.32 s2,[r5,#0x24]
  00085c30: vldr.32 s4,[r12,#0x24]
  00085c34: vsub.f32 s4,s4,s2
  00085c38: vmla.f32 s2,s0,s4
  00085c3c: vstr.32 s2,[r4,#0x24]
  00085c40: lsls r2,r1,#0x14
  00085c42: bpl 0x00085c60
  00085c44: ldrb.w r2,[r4,#0x59]
  00085c48: lsls r2,r2,#0x1c
  00085c4a: bmi 0x00085c60
  00085c4c: vldr.32 s2,[r5,#0x28]
  00085c50: vldr.32 s4,[r12,#0x28]
  00085c54: vsub.f32 s4,s4,s2
  00085c58: vmla.f32 s2,s0,s4
  00085c5c: vstr.32 s2,[r4,#0x28]
  00085c60: lsls r2,r1,#0x13
  00085c62: bpl 0x00085c80
  00085c64: ldrb.w r2,[r4,#0x59]
  00085c68: lsls r2,r2,#0x1b
  00085c6a: bmi 0x00085c80
  00085c6c: vldr.32 s2,[r5,#0x2c]
  00085c70: vldr.32 s4,[r12,#0x2c]
  00085c74: vsub.f32 s4,s4,s2
  00085c78: vmla.f32 s2,s0,s4
  00085c7c: vstr.32 s2,[r4,#0x2c]
  00085c80: lsls r2,r1,#0x12
  00085c82: bpl 0x00085ca0
  00085c84: ldrb.w r2,[r4,#0x59]
  00085c88: lsls r2,r2,#0x1a
  00085c8a: bmi 0x00085ca0
  00085c8c: vldr.32 s2,[r5,#0x30]
  00085c90: vldr.32 s4,[r12,#0x30]
  00085c94: vsub.f32 s4,s4,s2
  00085c98: vmla.f32 s2,s0,s4
  00085c9c: vstr.32 s2,[r4,#0x30]
  00085ca0: lsls r2,r1,#0x11
  00085ca2: bpl 0x00085cc0
  00085ca4: ldrb.w r2,[r4,#0x59]
  00085ca8: lsls r2,r2,#0x19
  00085caa: bmi 0x00085cc0
  00085cac: vldr.32 s2,[r5,#0x34]
  00085cb0: vldr.32 s4,[r12,#0x34]
  00085cb4: vsub.f32 s4,s4,s2
  00085cb8: vmla.f32 s2,s0,s4
  00085cbc: vstr.32 s2,[r4,#0x34]
  00085cc0: mov r2,r4
  00085cc2: lsls r6,r1,#0x10
  00085cc4: ldr.w r3,[r2,#0x58]!
  00085cc8: ldr r6,[r2,#0x4]
  00085cca: bpl 0x00085cea
  00085ccc: mov r9,r5
  00085cce: ands r5,r3,#0x8000
  00085cd2: itttt eq
  00085cd4: vldr.eq.32 s2,[r9,#0x38]
  00085cd8: vldr.eq.32 s4,[r12,#0x38]
  00085cdc: vsub.eq.f32 s4,s4,s2
  00085ce0: vmla.eq.f32 s2,s0,s4
  00085ce4: it eq
  00085ce6: vstr.eq.32 s2,[r4,#0x38]
  00085cea: orrs r1,r3
  00085cec: orr.w r3,r6,lr
  00085cf0: strd r1,r3,[r2,#0x0]
  00085cf4: ldr r6,[sp,#0x14]
  00085cf6: ldr r1,[sp,#0xc]
  00085cf8: adds r2,r6,#0x1
  00085cfa: cmp r2,r1
  00085cfc: blt.w 0x00085a2a
  00085d00: ldr.w r5,[r0,#0x11c]
  00085d04: ldr r1,[sp,#0xc]
  00085d06: b 0x00085efc
  00085d08: ldr.w r2,[r10,#0x0]
  00085d0c: tst r2,#0x40
  00085d10: beq 0x00085d28
  00085d12: ldr.w r3,[r0,#0x120]
  00085d16: ldr.w r3,[r3,r1,lsl #0x2]
  00085d1a: ldrb.w r6,[r3,#0x58]
  00085d1e: lsls r6,r6,#0x19
  00085d20: itt pl
  00085d22: ldr.pl.w r6,[r12,#0x18]
  00085d26: str.pl r6,[r3,#0x18]
  00085d28: lsls r3,r2,#0x18
  00085d2a: bpl 0x00085d42
  00085d2c: ldr.w r3,[r0,#0x120]
  00085d30: ldr.w r3,[r3,r1,lsl #0x2]
  00085d34: ldrb.w r6,[r3,#0x58]
  00085d38: lsls r6,r6,#0x18
  00085d3a: itt pl
  00085d3c: ldr.pl.w r6,[r12,#0x1c]
  00085d40: str.pl r6,[r3,#0x1c]
  00085d42: lsls r3,r2,#0x17
  00085d44: bpl 0x00085d5c
  00085d46: ldr.w r3,[r0,#0x120]
  00085d4a: ldr.w r3,[r3,r1,lsl #0x2]
  00085d4e: ldrb.w r6,[r3,#0x59]
  00085d52: lsls r6,r6,#0x1f
  00085d54: itt eq
  00085d56: ldr.eq.w r6,[r12,#0x20]
  00085d5a: str.eq r6,[r3,#0x20]
  00085d5c: lsls r3,r2,#0x1f
  00085d5e: beq 0x00085d76
  00085d60: ldr.w r3,[r0,#0x120]
  00085d64: ldr.w r3,[r3,r1,lsl #0x2]
  00085d68: ldrb.w r6,[r3,#0x58]
  00085d6c: lsls r6,r6,#0x1f
  00085d6e: itt eq
  00085d70: ldr.eq.w r6,[r12,#0x0]
  00085d74: str.eq r6,[r3,#0x0]
  00085d76: lsls r3,r2,#0x1e
  00085d78: bpl 0x00085d90
  00085d7a: ldr.w r3,[r0,#0x120]
  00085d7e: ldr.w r3,[r3,r1,lsl #0x2]
  00085d82: ldrb.w r6,[r3,#0x58]
  00085d86: lsls r6,r6,#0x1e
  00085d88: itt pl
  00085d8a: ldr.pl.w r6,[r12,#0x4]
  00085d8e: str.pl r6,[r3,#0x4]
  00085d90: lsls r3,r2,#0x1d
  00085d92: bpl 0x00085daa
  00085d94: ldr.w r3,[r0,#0x120]
  00085d98: ldr.w r3,[r3,r1,lsl #0x2]
  00085d9c: ldrb.w r6,[r3,#0x58]
  00085da0: lsls r6,r6,#0x1d
  00085da2: itt pl
  00085da4: ldr.pl.w r6,[r12,#0x8]
  00085da8: str.pl r6,[r3,#0x8]
  00085daa: lsls r3,r2,#0x1c
  00085dac: bpl 0x00085dc4
  00085dae: ldr.w r3,[r0,#0x120]
  00085db2: ldr.w r3,[r3,r1,lsl #0x2]
  00085db6: ldrb.w r6,[r3,#0x58]
  00085dba: lsls r6,r6,#0x1c
  00085dbc: itt pl
  00085dbe: ldr.pl.w r6,[r12,#0xc]
  00085dc2: str.pl r6,[r3,#0xc]
  00085dc4: lsls r3,r2,#0x1b
  00085dc6: bpl 0x00085dde
  00085dc8: ldr.w r3,[r0,#0x120]
  00085dcc: ldr.w r3,[r3,r1,lsl #0x2]
  00085dd0: ldrb.w r6,[r3,#0x58]
  00085dd4: lsls r6,r6,#0x1b
  00085dd6: itt pl
  00085dd8: ldr.pl.w r6,[r12,#0x10]
  00085ddc: str.pl r6,[r3,#0x10]
  00085dde: lsls r3,r2,#0x1a
  00085de0: bpl 0x00085df8
  00085de2: ldr.w r3,[r0,#0x120]
  00085de6: ldr.w r3,[r3,r1,lsl #0x2]
  00085dea: ldrb.w r6,[r3,#0x58]
  00085dee: lsls r6,r6,#0x1a
  00085df0: itt pl
  00085df2: ldr.pl.w r6,[r12,#0x14]
  00085df6: str.pl r6,[r3,#0x14]
  00085df8: lsls r3,r2,#0x16
  00085dfa: bpl 0x00085e12
  00085dfc: ldr.w r3,[r0,#0x120]
  00085e00: ldr.w r3,[r3,r1,lsl #0x2]
  00085e04: ldrb.w r6,[r3,#0x59]
  00085e08: lsls r6,r6,#0x1e
  00085e0a: itt pl
  00085e0c: ldr.pl.w r6,[r12,#0x48]
  00085e10: str.pl r6,[r3,#0x48]
  00085e12: lsls r3,r2,#0xf
  00085e14: bpl 0x00085e2c
  00085e16: ldr.w r3,[r0,#0x120]
  00085e1a: ldr.w r3,[r3,r1,lsl #0x2]
  00085e1e: ldrb.w r6,[r3,#0x5a]
  00085e22: lsls r6,r6,#0x1f
  00085e24: itt eq
  00085e26: ldr.eq.w r6,[r12,#0x3c]
  00085e2a: str.eq r6,[r3,#0x3c]
  00085e2c: lsls r3,r2,#0xe
  00085e2e: bpl 0x00085e46
  00085e30: ldr.w r3,[r0,#0x120]
  00085e34: ldr.w r3,[r3,r1,lsl #0x2]
  00085e38: ldrb.w r6,[r3,#0x5a]
  00085e3c: lsls r6,r6,#0x1e
  00085e3e: itt pl
  00085e40: ldr.pl.w r6,[r12,#0x40]
  00085e44: str.pl r6,[r3,#0x40]
  00085e46: lsls r3,r2,#0xd
  00085e48: bpl 0x00085e60
  00085e4a: ldr.w r3,[r0,#0x120]
  00085e4e: ldr.w r3,[r3,r1,lsl #0x2]
  00085e52: ldrb.w r6,[r3,#0x5a]
  00085e56: lsls r6,r6,#0x1d
  00085e58: itt pl
  00085e5a: ldr.pl.w r6,[r12,#0x44]
  00085e5e: str.pl r6,[r3,#0x44]
  00085e60: lsls r3,r2,#0x15
  00085e62: bpl 0x00085e7a
  00085e64: ldr.w r3,[r0,#0x120]
  00085e68: ldr.w r3,[r3,r1,lsl #0x2]
  00085e6c: ldrb.w r6,[r3,#0x59]
  00085e70: lsls r6,r6,#0x1d
  00085e72: itt pl
  00085e74: ldr.pl.w r6,[r12,#0x24]
  00085e78: str.pl r6,[r3,#0x24]
  00085e7a: lsls r3,r2,#0x14
  00085e7c: bpl 0x00085e94
  00085e7e: ldr.w r3,[r0,#0x120]
  00085e82: ldr.w r3,[r3,r1,lsl #0x2]
  00085e86: ldrb.w r6,[r3,#0x59]
  00085e8a: lsls r6,r6,#0x1c
  00085e8c: itt pl
  00085e8e: ldr.pl.w r6,[r12,#0x28]
  00085e92: str.pl r6,[r3,#0x28]
  00085e94: lsls r3,r2,#0x13
  00085e96: bpl 0x00085eae
  00085e98: ldr.w r3,[r0,#0x120]
  00085e9c: ldr.w r3,[r3,r1,lsl #0x2]
  00085ea0: ldrb.w r6,[r3,#0x59]
  00085ea4: lsls r6,r6,#0x1b
  00085ea6: itt pl
  00085ea8: ldr.pl.w r6,[r12,#0x2c]
  00085eac: str.pl r6,[r3,#0x2c]
  00085eae: lsls r3,r2,#0x12
  00085eb0: bpl 0x00085ec8
  00085eb2: ldr.w r3,[r0,#0x120]
  00085eb6: ldr.w r3,[r3,r1,lsl #0x2]
  00085eba: ldrb.w r6,[r3,#0x59]
  00085ebe: lsls r6,r6,#0x1a
  00085ec0: itt pl
  00085ec2: ldr.pl.w r6,[r12,#0x30]
  00085ec6: str.pl r6,[r3,#0x30]
  00085ec8: lsls r3,r2,#0x11
  00085eca: bpl 0x00085ee2
  00085ecc: ldr.w r3,[r0,#0x120]
  00085ed0: ldr.w r3,[r3,r1,lsl #0x2]
  00085ed4: ldrb.w r6,[r3,#0x59]
  00085ed8: lsls r6,r6,#0x19
  00085eda: itt pl
  00085edc: ldr.pl.w r6,[r12,#0x34]
  00085ee0: str.pl r6,[r3,#0x34]
  00085ee2: lsls r2,r2,#0x10
  00085ee4: bpl 0x00085efc
  00085ee6: ldr.w r2,[r0,#0x120]
  00085eea: ldr.w r2,[r2,r1,lsl #0x2]
  00085eee: ldrb.w r3,[r2,#0x59]
  00085ef2: lsls r3,r3,#0x18
  00085ef4: itt pl
  00085ef6: ldr.pl.w r3,[r12,#0x38]
  00085efa: str.pl r3,[r2,#0x38]
  00085efc: adds r1,#0x1
  00085efe: cmp r1,r5
  00085f00: bcc.w 0x00085d08
  00085f04: vpop {d5,d6,d7,d8}
  00085f08: add sp,#0x4
  00085f0a: pop.w {r8,r9,r10,r11}
  00085f0e: pop {r4,r5,r6,r7,pc}
```
