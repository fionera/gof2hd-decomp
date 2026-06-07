# MGame::freeCamTouchEnd
elf 0x178d30 body 162
Sig: undefined __stdcall freeCamTouchEnd(int param_1, int param_2, void * param_3)

## decompile
```c

/* MGame::freeCamTouchEnd(int, int, void*) */

void MGame::freeCamTouchEnd(int param_1,int param_2,void *param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int in_r3;
  uint in_fpscr;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  if (*(int *)(param_1 + 0x98) == in_r3) {
    *(undefined4 *)(param_1 + 0x98) = 0;
  }
  else {
    if (*(int *)(param_1 + 0x9c) != in_r3) goto LAB_00188d54;
    *(undefined4 *)(param_1 + 0x9c) = 0;
  }
  *(undefined4 *)(param_1 + 0xa0) = 0;
LAB_00188d54:
  uVar1 = DAT_00188dd8;
  iVar3 = *(int *)(param_1 + 0x134);
  iVar2 = *(int *)(param_1 + 0x138);
  fVar5 = (float)VectorSignedToFloat(iVar3,(byte)(in_fpscr >> 0x16) & 3);
  fVar4 = (float)VectorSignedToFloat(iVar2,(byte)(in_fpscr >> 0x16) & 3);
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  fVar6 = *(float *)(param_1 + 0x118) + fVar5;
  fVar7 = *(float *)(param_1 + 0x11c) + fVar4;
  if (iVar3 < 0) {
    iVar3 = -iVar3;
  }
  fVar8 = DAT_00188dd4;
  if (3 < iVar3) {
    fVar8 = fVar5;
  }
  fVar5 = DAT_00188dd4;
  if (3 < iVar2) {
    fVar5 = fVar4;
  }
  *(undefined1 *)(param_1 + 0x111) = 1;
  *(undefined4 *)(param_1 + 0x13c) = uVar1;
  *(undefined4 *)(param_1 + 0x140) = uVar1;
  *(undefined1 *)(param_1 + 0x15c) = 0;
  *(float *)(param_1 + 0x148) = fVar8;
  *(float *)(param_1 + 0x14c) = fVar5;
  *(float *)(param_1 + 0x118) = fVar6;
  *(float *)(param_1 + 0x11c) = fVar7;
  *(int *)(param_1 + 300) = (int)fVar6;
  *(int *)(param_1 + 0x130) = (int)fVar7;
  return;
}

```

## target disasm
```
  00188d30: ldr.w r1,[r0,#0x98]
  00188d34: cmp r1,r3
  00188d36: beq 0x00188d48
  00188d38: ldr.w r1,[r0,#0x9c]
  00188d3c: cmp r1,r3
  00188d3e: bne 0x00188d54
  00188d40: movs r1,#0x0
  00188d42: str.w r1,[r0,#0x9c]
  00188d46: b 0x00188d4e
  00188d48: movs r1,#0x0
  00188d4a: str.w r1,[r0,#0x98]
  00188d4e: movs r1,#0x0
  00188d50: str.w r1,[r0,#0xa0]
  00188d54: ldrd r2,r1,[r0,#0x134]
  00188d58: cmp r1,#0x0
  00188d5a: vmov s0,r2
  00188d5e: vcvt.f32.s32 s2,s0
  00188d62: vmov s0,r1
  00188d66: vcvt.f32.s32 s0,s0
  00188d6a: vldr.32 s4,[r0,#0x118]
  00188d6e: vldr.32 s6,[r0,#0x11c]
  00188d72: bpl 0x00188d76
  00188d74: rsbs r1,r1
  00188d76: vadd.f32 s4,s4,s2
  00188d7a: ldr r3,[0x00188dd8]
  00188d7c: vadd.f32 s6,s6,s0
  00188d80: vldr.32 s8,[pc,#0x50]
  00188d84: cmp r2,#0x0
  00188d86: bpl 0x00188d8a
  00188d88: rsbs r2,r2
  00188d8a: cmp r2,#0x3
  00188d8c: vmov.f32 s10,s8
  00188d90: it gt
  00188d92: vmov.gt.f32 s10,s2
  00188d96: cmp r1,#0x3
  00188d98: mov.w r1,#0x1
  00188d9c: it gt
  00188d9e: vmov.gt.f32 s8,s0
  00188da2: strb.w r1,[r0,#0x111]
  00188da6: movs r1,#0x0
  00188da8: strd r3,r3,[r0,#0x13c]
  00188dac: vcvt.s32.f32 s0,s4
  00188db0: vcvt.s32.f32 s2,s6
  00188db4: strb.w r1,[r0,#0x15c]
  00188db8: vstr.32 s10,[r0,#0x148]
  00188dbc: vstr.32 s8,[r0,#0x14c]
  00188dc0: vstr.32 s4,[r0,#0x118]
  00188dc4: vstr.32 s6,[r0,#0x11c]
  00188dc8: vstr.32 s0,[r0,#0x12c]
  00188dcc: vstr.32 s2,[r0,#0x130]
  00188dd0: bx lr
```
