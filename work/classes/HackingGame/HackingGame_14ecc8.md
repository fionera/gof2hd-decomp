# HackingGame::HackingGame
elf 0x14ecc8 body 204
Sig: undefined __thiscall HackingGame(HackingGame * this, int param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* HackingGame::HackingGame(int, int, int, int, int) */

HackingGame * __thiscall
HackingGame::HackingGame
          (HackingGame *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  HackingGame *pHVar1;
  code *pcVar2;
  undefined4 *puVar3;
  int iVar4;
  
  *(int *)(this + 0x124) = param_1;
  pHVar1 = this + param_1 * 0x30 + 100;
  puVar3 = *(undefined4 **)(DAT_0015ed94 + 0x15ecf8);
  for (iVar4 = 0; iVar4 != 6; iVar4 = iVar4 + 1) {
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)*puVar3,(short)iVar4 + 0x1f4a,(uint *)(pHVar1 + -0x18));
    AbyssEngine::PaintCanvas::Image2DCreate
              ((PaintCanvas *)*puVar3,(short)iVar4 + 0x1f50,(uint *)pHVar1);
    pHVar1 = pHVar1 + 4;
  }
  pcVar2 = *(code **)(DAT_0015ed98 + 0x15ed2e);
  (*pcVar2)((PaintCanvas *)*puVar3,0x1f48,this + 0x114);
  (*pcVar2)(*puVar3,0x1f49,this + 0x10c);
  (*pcVar2)(*puVar3,0x1f47,this + 0x110);
  (*pcVar2)(*puVar3,0x1f46,this + 0x118);
  (*pcVar2)(*puVar3,0x1f44,this + 0x11c);
  (*pcVar2)(*puVar3,0x1f45,this + 0x120);
  *(int *)this = param_2;
  *(int *)(this + 0x134) = param_3;
  *(int *)(this + 0x138) = param_4;
  *(int *)(this + 0x13c) = param_5;
  reInit(this);
  return this;
}

```

## target disasm
```
  0015ecc8: push {r4,r5,r6,r7,lr}
  0015ecca: add r7,sp,#0xc
  0015eccc: push {r7,r8,r9,r10,r11}
  0015ecd0: mov r4,r0
  0015ecd2: str.w r1,[r0,#0x124]
  0015ecd6: add.w r0,r1,r1, lsl #0x1
  0015ecda: str r3,[sp,#0x0]
  0015ecdc: mov r11,r2
  0015ecde: mov.w r9,#0x0
  0015ece2: add.w r0,r4,r0, lsl #0x4
  0015ece6: movw r10,#0x1f4a
  0015ecea: add.w r5,r0,#0x64
  0015ecee: ldr r0,[0x0015ed94]
  0015ecf0: movw r8,#0x1f50
  0015ecf4: add r0,pc
  0015ecf6: ldr r6,[r0,#0x0]
  0015ecf8: b 0x0015ed1c
  0015ecfa: add.w r1,r9,r10
  0015ecfe: sub.w r2,r5,#0x18
  0015ed02: uxth r1,r1
  0015ed04: blx 0x00071cf8
  0015ed08: add.w r1,r9,r8
  0015ed0c: ldr r0,[r6,#0x0]
  0015ed0e: mov r2,r5
  0015ed10: uxth r1,r1
  0015ed12: blx 0x00071cf8
  0015ed16: adds r5,#0x4
  0015ed18: add.w r9,r9,#0x1
  0015ed1c: ldr r0,[r6,#0x0]
  0015ed1e: cmp.w r9,#0x6
  0015ed22: bne 0x0015ecfa
  0015ed24: ldr r1,[0x0015ed98]
  0015ed26: add.w r2,r4,#0x114
  0015ed2a: add r1,pc
  0015ed2c: ldr r5,[r1,#0x0]
  0015ed2e: movw r1,#0x1f48
  0015ed32: blx r5
  0015ed34: ldr r0,[r6,#0x0]
  0015ed36: add.w r2,r4,#0x10c
  0015ed3a: movw r1,#0x1f49
  0015ed3e: blx r5
  0015ed40: ldr r0,[r6,#0x0]
  0015ed42: add.w r2,r4,#0x110
  0015ed46: movw r1,#0x1f47
  0015ed4a: blx r5
  0015ed4c: ldr r0,[r6,#0x0]
  0015ed4e: add.w r2,r4,#0x118
  0015ed52: movw r1,#0x1f46
  0015ed56: blx r5
  0015ed58: ldr r0,[r6,#0x0]
  0015ed5a: add.w r2,r4,#0x11c
  0015ed5e: movw r1,#0x1f44
  0015ed62: blx r5
  0015ed64: ldr r0,[r6,#0x0]
  0015ed66: add.w r2,r4,#0x120
  0015ed6a: movw r1,#0x1f45
  0015ed6e: blx r5
  0015ed70: ldr r0,[sp,#0x0]
  0015ed72: str.w r11,[r4,#0x0]
  0015ed76: str.w r0,[r4,#0x134]
  0015ed7a: ldr r0,[r7,#0x8]
  0015ed7c: str.w r0,[r4,#0x138]
  0015ed80: ldr r0,[r7,#0xc]
  0015ed82: str.w r0,[r4,#0x13c]
  0015ed86: mov r0,r4
  0015ed88: blx 0x00072c70
  0015ed8c: mov r0,r4
  0015ed8e: pop.w {r3,r8,r9,r10,r11}
  0015ed92: pop {r4,r5,r6,r7,pc}
```
