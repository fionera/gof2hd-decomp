# MeshMerger::update
elf 0x173f2c body 320
Sig: undefined __thiscall update(MeshMerger * this)

## decompile
```c

/* MeshMerger::update() */

void __thiscall MeshMerger::update(MeshMerger *this)

{
  byte bVar1;
  byte *pbVar2;
  short *psVar3;
  int iVar4;
  uint uVar5;
  short *psVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  float in_s0;
  float extraout_s0;
  
  for (iVar7 = 0; iVar4 = *(int *)this, iVar7 < iVar4; iVar7 = iVar7 + 1) {
    uVar5 = AbyssEngine::PaintCanvas::CameraIsSphereinViewFrustum
                      (*(PaintCanvas **)(this + 0xc),
                       (Vector *)(*(int *)(*(int *)(this + 0x18) + iVar7 * 4) + 0x3c),in_s0);
    if ((uVar5 != *(byte *)(*(int *)(this + 0x2c) + iVar7)) &&
       (*(char *)(*(int *)(this + 0x2c) + iVar7) = (char)uVar5,
       *(char *)(*(int *)(this + 0x28) + iVar7) != '\0')) {
      this[0x34] = (MeshMerger)0x1;
    }
    in_s0 = extraout_s0;
  }
  if (this[0x34] != (MeshMerger)0x0) {
    iVar7 = 0;
    iVar9 = 0;
    for (iVar8 = 0; iVar8 < iVar4; iVar8 = iVar8 + 1) {
      if ((*(char *)(*(int *)(this + 0x28) + iVar8) != '\0') &&
         (*(char *)(*(int *)(this + 0x2c) + iVar8) != '\0')) {
        pbVar2 = *(byte **)(this + 0x20);
        bVar1 = *pbVar2;
        iVar10 = *(int *)(*(int *)(this + 0x18) +
                         (iVar4 * *(char *)(*(int *)(this + 0x24) + iVar8) + iVar8) * 4);
        if ((bVar1 & 1) != 0) {
          __aeabi_memcpy4(*(int *)(pbVar2 + 4) + iVar7 * 0xc,*(undefined4 *)(iVar10 + 4),
                          (uint)*(ushort *)(iVar10 + 2) * 0xc);
          pbVar2 = *(byte **)(this + 0x20);
          bVar1 = *pbVar2;
        }
        uVar5 = (uint)bVar1;
        if ((int)(uVar5 << 0x1d) < 0) {
          __aeabi_memcpy4(*(int *)(pbVar2 + 0x10) + iVar7 * 0xc,*(undefined4 *)(iVar10 + 0x10),
                          (uint)*(ushort *)(iVar10 + 2) * 0xc);
          pbVar2 = *(byte **)(this + 0x20);
          uVar5 = (uint)*pbVar2;
        }
        if ((int)(uVar5 << 0x1c) < 0) {
          __aeabi_memcpy4(*(int *)(pbVar2 + 0xc) + iVar7 * 0x10,*(undefined4 *)(iVar10 + 0xc),
                          (uint)*(ushort *)(iVar10 + 2) << 4);
          pbVar2 = *(byte **)(this + 0x20);
          uVar5 = (uint)*pbVar2;
        }
        if ((int)(uVar5 << 0x1e) < 0) {
          __aeabi_memcpy4(*(int *)(pbVar2 + 8) + iVar7 * 8,*(undefined4 *)(iVar10 + 8),
                          (uint)*(ushort *)(iVar10 + 2) << 3);
          pbVar2 = *(byte **)(this + 0x20);
          uVar5 = (uint)*pbVar2;
        }
        if ((int)(uVar5 << 0x1b) < 0) {
          psVar3 = *(short **)(iVar10 + 0x2c);
          psVar6 = (short *)(*(int *)(pbVar2 + 0x2c) + iVar9 * 2);
          for (iVar4 = -(uint)*(ushort *)(iVar10 + 0x28); iVar4 != 0; iVar4 = iVar4 + 1) {
            *psVar6 = *psVar3 + (short)iVar7;
            psVar3 = psVar3 + 1;
            psVar6 = psVar6 + 1;
          }
        }
        iVar4 = *(int *)this;
        iVar9 = iVar9 + (uint)*(ushort *)(iVar10 + 0x28);
        iVar7 = iVar7 + (uint)*(ushort *)(iVar10 + 2);
      }
    }
    iVar4 = *(int *)(this + 0x20);
    *(short *)(iVar4 + 0x28) = (short)iVar9;
    *(short *)(iVar4 + 2) = (short)iVar7;
    this[0x34] = (MeshMerger)0x0;
  }
  return;
}

```

## target disasm
```
  00183f2c: push {r4,r5,r6,r7,lr}
  00183f2e: add r7,sp,#0xc
  00183f30: push {r8,r9,r11}
  00183f34: mov r4,r0
  00183f36: movs r5,#0x0
  00183f38: movs r6,#0x1
  00183f3a: b 0x00183f62
  00183f3c: ldr r1,[r4,#0x18]
  00183f3e: ldr r0,[r4,#0xc]
  00183f40: ldr.w r1,[r1,r5,lsl #0x2]
  00183f44: ldr r2,[r1,#0x48]
  00183f46: adds r1,#0x3c
  00183f48: blx 0x0006ff94
  00183f4c: ldr r1,[r4,#0x2c]
  00183f4e: ldrb r2,[r1,r5]
  00183f50: cmp r0,r2
  00183f52: beq 0x00183f60
  00183f54: strb r0,[r1,r5]
  00183f56: ldr r0,[r4,#0x28]
  00183f58: ldrb r0,[r0,r5]
  00183f5a: cbz r0,0x00183f60
  00183f5c: strb.w r6,[r4,#0x34]
  00183f60: adds r5,#0x1
  00183f62: ldr r1,[r4,#0x0]
  00183f64: cmp r5,r1
  00183f66: blt 0x00183f3c
  00183f68: ldrb.w r0,[r4,#0x34]
  00183f6c: cmp r0,#0x0
  00183f6e: beq 0x00184066
  00183f70: movs r5,#0x0
  00183f72: mov.w r8,#0x0
  00183f76: movs r6,#0x0
  00183f78: b 0x00184054
  00183f7a: ldr r0,[r4,#0x28]
  00183f7c: ldrb r0,[r0,r6]
  00183f7e: cmp r0,#0x0
  00183f80: beq 0x00184052
  00183f82: ldr r0,[r4,#0x2c]
  00183f84: ldrb r0,[r0,r6]
  00183f86: cmp r0,#0x0
  00183f88: beq 0x00184052
  00183f8a: ldr r2,[r4,#0x18]
  00183f8c: ldrd r0,r3,[r4,#0x20]
  00183f90: ldrsb r3,[r3,r6]
  00183f92: mla r3,r1,r3,r6
  00183f96: ldrb r1,[r0,#0x0]
  00183f98: tst r1,#0x1
  00183f9c: ldr.w r9,[r2,r3,lsl #0x2]
  00183fa0: beq 0x00183fc2
  00183fa2: ldrh.w r2,[r9,#0x2]
  00183fa6: add.w r3,r5,r5, lsl #0x1
  00183faa: ldr r0,[r0,#0x4]
  00183fac: ldr.w r1,[r9,#0x4]
  00183fb0: add.w r2,r2,r2, lsl #0x1
  00183fb4: add.w r0,r0,r3, lsl #0x2
  00183fb8: lsls r2,r2,#0x2
  00183fba: blx 0x0006f6a0
  00183fbe: ldr r0,[r4,#0x20]
  00183fc0: ldrb r1,[r0,#0x0]
  00183fc2: lsls r2,r1,#0x1d
  00183fc4: bpl 0x00183fe6
  00183fc6: ldrh.w r2,[r9,#0x2]
  00183fca: add.w r3,r5,r5, lsl #0x1
  00183fce: ldr r0,[r0,#0x10]
  00183fd0: ldr.w r1,[r9,#0x10]
  00183fd4: add.w r2,r2,r2, lsl #0x1
  00183fd8: add.w r0,r0,r3, lsl #0x2
  00183fdc: lsls r2,r2,#0x2
  00183fde: blx 0x0006f6a0
  00183fe2: ldr r0,[r4,#0x20]
  00183fe4: ldrb r1,[r0,#0x0]
  00183fe6: lsls r2,r1,#0x1c
  00183fe8: bpl 0x00184002
  00183fea: ldrh.w r2,[r9,#0x2]
  00183fee: ldr r0,[r0,#0xc]
  00183ff0: ldr.w r1,[r9,#0xc]
  00183ff4: add.w r0,r0,r5, lsl #0x4
  00183ff8: lsls r2,r2,#0x4
  00183ffa: blx 0x0006f6a0
  00183ffe: ldr r0,[r4,#0x20]
  00184000: ldrb r1,[r0,#0x0]
  00184002: lsls r2,r1,#0x1e
  00184004: bpl 0x0018401e
  00184006: ldrh.w r2,[r9,#0x2]
  0018400a: ldr r0,[r0,#0x8]
  0018400c: ldr.w r1,[r9,#0x8]
  00184010: add.w r0,r0,r5, lsl #0x3
  00184014: lsls r2,r2,#0x3
  00184016: blx 0x0006f6a0
  0018401a: ldr r0,[r4,#0x20]
  0018401c: ldrb r1,[r0,#0x0]
  0018401e: lsls r1,r1,#0x1b
  00184020: bpl 0x00184044
  00184022: ldr r1,[r0,#0x2c]
  00184024: ldrh.w r2,[r9,#0x28]
  00184028: ldr.w r0,[r9,#0x2c]
  0018402c: add.w r1,r1,r8, lsl #0x1
  00184030: rsbs r2,r2
  00184032: b 0x00184040
  00184034: ldrh.w r3,[r0],#0x2
  00184038: adds r2,#0x1
  0018403a: add r3,r5
  0018403c: strh r3,[r1],#0x2
  00184040: cmp r2,#0x0
  00184042: bne 0x00184034
  00184044: ldrh.w r0,[r9,#0x28]
  00184048: ldrh.w r2,[r9,#0x2]
  0018404c: ldr r1,[r4,#0x0]
  0018404e: add r8,r0
  00184050: add r5,r2
  00184052: adds r6,#0x1
  00184054: cmp r6,r1
  00184056: blt 0x00183f7a
  00184058: ldr r0,[r4,#0x20]
  0018405a: strh.w r8,[r0,#0x28]
  0018405e: strh r5,[r0,#0x2]
  00184060: movs r0,#0x0
  00184062: strb.w r0,[r4,#0x34]
  00184066: pop.w {r8,r9,r11}
  0018406a: pop {r4,r5,r6,r7,pc}
```
