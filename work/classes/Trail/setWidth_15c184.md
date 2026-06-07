# Trail::setWidth
elf 0x15c184 body 114
Sig: undefined __stdcall setWidth(int param_1)

## decompile
```c

/* Trail::setWidth(int) */

void Trail::setWidth(int param_1)

{
  int iVar1;
  int iVar2;
  int in_r1;
  int iVar3;
  int iVar4;
  int iVar5;
  uint *puVar6;
  uint in_fpscr;
  float fVar7;
  float in_s1;
  float extraout_s1;
  float fVar8;
  
  iVar1 = *(int *)(param_1 + 0xc);
  iVar4 = 0;
  puVar6 = *(uint **)(DAT_0016c1f8 + 0x16c1a0);
  for (iVar5 = 0; iVar5 < (*(int *)(param_1 + 0x24) + 1) * 2; iVar5 = iVar5 + 1) {
    iVar2 = *(int *)(param_1 + 0x18);
    iVar3 = *(int *)(iVar2 + iVar4) + (iVar1 - in_r1);
    VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
    fVar8 = (float)VectorSignedToFloat(*(undefined4 *)(iVar2 + iVar4 + 8),
                                       (byte)(in_fpscr >> 0x16) & 3);
    fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(iVar2 + iVar4 + 4),
                                       (byte)(in_fpscr >> 0x16) & 3);
    *(int *)(iVar2 + iVar4) = iVar3;
    AbyssEngine::PaintCanvas::MeshSetPoint
              (*puVar6,(ushort)*(undefined4 *)(param_1 + 0x10),fVar7,in_s1,fVar8);
    iVar4 = iVar4 + 0xc;
    in_s1 = extraout_s1;
  }
  *(int *)(param_1 + 0xc) = in_r1;
  return;
}

```

## target disasm
```
  0016c184: push {r4,r5,r6,r7,lr}
  0016c186: add r7,sp,#0xc
  0016c188: push {r6,r7,r8,r9,r10}
  0016c18c: mov r5,r0
  0016c18e: ldr r0,[r0,#0xc]
  0016c190: mov r8,r1
  0016c192: movs r4,#0x0
  0016c194: sub.w r9,r0,r1
  0016c198: ldr r0,[0x0016c1f8]
  0016c19a: movs r6,#0x0
  0016c19c: add r0,pc
  0016c19e: ldr.w r10,[r0,#0x0]
  0016c1a2: b 0x0016c1e2
  0016c1a4: ldr r0,[r5,#0x18]
  0016c1a6: adds r2,r0,r4
  0016c1a8: ldr r1,[r0,r4]
  0016c1aa: add r1,r9
  0016c1ac: vmov s0,r1
  0016c1b0: vcvt.f32.s32 s0,s0
  0016c1b4: vldr.32 s2,[r2,#0x8]
  0016c1b8: vmov r3,s0
  0016c1bc: vldr.32 s0,[r2,#0x4]
  0016c1c0: vcvt.f32.s32 s2,s2
  0016c1c4: uxth r2,r6
  0016c1c6: vcvt.f32.s32 s0,s0
  0016c1ca: str r1,[r0,r4]
  0016c1cc: ldr r1,[r5,#0x10]
  0016c1ce: ldr.w r0,[r10,#0x0]
  0016c1d2: vstr.32 s2,[sp,#0x4]
  0016c1d6: vstr.32 s0,[sp]
  0016c1da: blx 0x000777f4
  0016c1de: adds r4,#0xc
  0016c1e0: adds r6,#0x1
  0016c1e2: ldr r0,[r5,#0x24]
  0016c1e4: adds r0,#0x1
  0016c1e6: cmp.w r6,r0, lsl #0x1
  0016c1ea: blt 0x0016c1a4
  0016c1ec: str.w r8,[r5,#0xc]
  0016c1f0: pop.w {r2,r3,r8,r9,r10}
  0016c1f4: pop {r4,r5,r6,r7,pc}
```
