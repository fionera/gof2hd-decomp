# Layout::setWindowDimensions
elf 0xd4c60 body 202
Sig: undefined __thiscall setWindowDimensions(Layout * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* Layout::setWindowDimensions(int, int, int, int) */

void __thiscall
Layout::setWindowDimensions(Layout *this,int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  float unaff_r4;
  float unaff_r5;
  int iVar2;
  int *piVar3;
  int *piVar4;
  code *pcVar5;
  
  piVar4 = *(int **)(DAT_000e4d2c + 0xe4c78);
  iVar2 = *piVar4;
  pcVar5 = *(code **)(DAT_000e4d30 + 0xe4c84);
  *(int *)(this + 0x2dc) = param_1;
  *(int *)(this + 0x2e0) = param_2;
  *(int *)(this + 0x2e4) = param_3;
  *(int *)(this + 0x2e8) = param_4;
  (*pcVar5)(*(undefined4 *)(this + 0x3bc),param_3 + param_1,param_2,0x12);
  piVar3 = *(int **)(DAT_000e4d34 + 0xe4ca0);
  (*pcVar5)(*(undefined4 *)(this + 0x3b4),*(int *)(*piVar3 + 0x28) + *(int *)(this + 0x2dc),
            (*(int *)(this + 0x2e0) + *(int *)(this + 0x2e8)) - *(int *)(this + 0x3fc),0x21);
  (*pcVar5)(*(undefined4 *)(this + 0x3b8),*(int *)(*piVar3 + 0x28) + *(int *)(this + 0x2dc),
            (*(int *)(this + 0x2e0) + *(int *)(this + 0x2e8)) - *(int *)(this + 0x3fc),0x21);
  if (*(int *)(this + 0x3b4) != 0) {
    TouchButton::getPosition();
    **(int **)(DAT_000e4d38 + 0xe4cf6) = (int)unaff_r4;
    TouchButton::getPosition();
    **(int **)(DAT_000e4d3c + 0xe4d12) = (int)unaff_r5;
  }
  iVar1 = *piVar4;
  iVar2 = iVar1 - iVar2;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2,iVar1);
  }
  return;
}

```

## target disasm
```
  000e4c60: push {r4,r5,r6,r7,lr}
  000e4c62: add r7,sp,#0xc
  000e4c64: push {r4,r5,r6,r7,r8,r9,r11}
  000e4c68: mov r4,r0
  000e4c6a: ldr r0,[0x000e4d2c]
  000e4c6c: ldr.w lr,[r7,#0x8]
  000e4c70: add.w r12,r3,r1
  000e4c74: add r0,pc
  000e4c76: ldr.w r8,[r0,#0x0]
  000e4c7a: ldr r0,[0x000e4d30]
  000e4c7c: ldr.w r5,[r8,#0x0]
  000e4c80: add r0,pc
  000e4c82: str r5,[sp,#0xc]
  000e4c84: add.w r5,r4,#0x2dc
  000e4c88: ldr.w r9,[r0,#0x0]
  000e4c8c: ldr.w r0,[r4,#0x3bc]
  000e4c90: stm.w r5!,{r1,r2,r3,lr}
  000e4c94: mov r1,r12
  000e4c96: movs r3,#0x12
  000e4c98: blx r9
  000e4c9a: ldr r0,[0x000e4d34]
  000e4c9c: add r0,pc
  000e4c9e: ldr r5,[r0,#0x0]
  000e4ca0: ldr r1,[r5,#0x0]
  000e4ca2: ldrd r12,r2,[r4,#0x2dc]
  000e4ca6: ldr.w r6,[r4,#0x2e8]
  000e4caa: ldr r1,[r1,#0x28]
  000e4cac: ldr.w r3,[r4,#0x3fc]
  000e4cb0: add r2,r6
  000e4cb2: ldr.w r0,[r4,#0x3b4]
  000e4cb6: add r1,r12
  000e4cb8: subs r2,r2,r3
  000e4cba: movs r3,#0x21
  000e4cbc: blx r9
  000e4cbe: ldr r1,[r5,#0x0]
  000e4cc0: ldrd r3,r2,[r4,#0x2dc]
  000e4cc4: ldr.w r6,[r4,#0x2e8]
  000e4cc8: ldr r1,[r1,#0x28]
  000e4cca: add r2,r6
  000e4ccc: ldr.w r6,[r4,#0x3fc]
  000e4cd0: ldr.w r0,[r4,#0x3b8]
  000e4cd4: add r1,r3
  000e4cd6: subs r2,r2,r6
  000e4cd8: movs r3,#0x21
  000e4cda: blx r9
  000e4cdc: ldr.w r1,[r4,#0x3b4]
  000e4ce0: cbz r1,0x000e4d16
  000e4ce2: mov r0,sp
  000e4ce4: blx 0x00074dd0
  000e4ce8: vldr.32 s0,[sp]
  000e4cec: ldr r0,[0x000e4d38]
  000e4cee: vcvt.s32.f32 s0,s0
  000e4cf2: add r0,pc
  000e4cf4: ldr r0,[r0,#0x0]
  000e4cf6: vstr.32 s0,[r0]
  000e4cfa: mov r0,sp
  000e4cfc: ldr.w r1,[r4,#0x3b4]
  000e4d00: blx 0x00074dd0
  000e4d04: vldr.32 s0,[sp,#0x4]
  000e4d08: ldr r0,[0x000e4d3c]
  000e4d0a: vcvt.s32.f32 s0,s0
  000e4d0e: add r0,pc
  000e4d10: ldr r0,[r0,#0x0]
  000e4d12: vstr.32 s0,[r0]
  000e4d16: ldr r0,[sp,#0xc]
  000e4d18: ldr.w r1,[r8,#0x0]
  000e4d1c: subs r0,r1,r0
  000e4d1e: itt eq
  000e4d20: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000e4d24: pop.eq {r4,r5,r6,r7,pc}
  000e4d26: blx 0x0006e824
```
