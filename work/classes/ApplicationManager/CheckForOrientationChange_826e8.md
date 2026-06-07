# ApplicationManager::CheckForOrientationChange
elf 0x826e8 body 222
Sig: undefined __thiscall CheckForOrientationChange(ApplicationManager * this)

## decompile
```c

/* AbyssEngine::ApplicationManager::CheckForOrientationChange() */

void __thiscall AbyssEngine::ApplicationManager::CheckForOrientationChange(ApplicationManager *this)

{
  int iVar1;
  PaintCanvas *pPVar2;
  undefined4 uVar3;
  int *piVar4;
  double dVar5;
  
  dVar5 = *(double *)(*(int *)(this + 0xa8) + 0x4b0);
  if (((int)((uint)(dVar5 < -0.5) << 0x1f) < 0) &&
     (pPVar2 = *(PaintCanvas **)this, *(int *)(pPVar2 + 0x30) == 0)) {
    piVar4 = *(int **)(DAT_000927c8 + 0x9278c);
    iVar1 = (*(int *)(this + 0x70) - *(int *)(this + 0x78)) + *piVar4;
    *piVar4 = iVar1;
    if (iVar1 < 0xfb) {
      return;
    }
    uVar3 = 1;
  }
  else {
    if (0.5 < dVar5) {
      pPVar2 = *(PaintCanvas **)this;
      if (*(int *)(pPVar2 + 0x30) == 3) {
        piVar4 = *(int **)(DAT_000927d0 + 0x927a8);
        iVar1 = (*(int *)(this + 0x70) - *(int *)(this + 0x78)) + *piVar4;
        *piVar4 = iVar1;
        if (iVar1 < 0xfb) {
          return;
        }
        uVar3 = 2;
        goto LAB_000927bc;
      }
      if (*(int *)(pPVar2 + 0x30) == 1) {
        piVar4 = *(int **)(DAT_000927cc + 0x9272c);
        iVar1 = (*(int *)(this + 0x70) - *(int *)(this + 0x78)) + *piVar4;
        *piVar4 = iVar1;
        if (iVar1 < 0xfb) {
          return;
        }
        PaintCanvas::SetGameOrientation(pPVar2,0);
        *piVar4 = 0;
        return;
      }
    }
    if ((-1 < (int)((uint)(dVar5 < -0.5) << 0x1f)) ||
       (pPVar2 = *(PaintCanvas **)this, *(int *)(pPVar2 + 0x30) != 2)) {
      **(undefined4 **)(DAT_000927d8 + 0x92782) = 0;
      return;
    }
    piVar4 = *(int **)(DAT_000927d4 + 0x92762);
    iVar1 = (*(int *)(this + 0x70) - *(int *)(this + 0x78)) + *piVar4;
    *piVar4 = iVar1;
    if (iVar1 < 0xfb) {
      return;
    }
    uVar3 = 3;
  }
LAB_000927bc:
  PaintCanvas::SetGameOrientation(pPVar2,uVar3);
  *piVar4 = 0;
  return;
}

```

## target disasm
```
  000926e8: push {r4,r5,r7,lr}
  000926ea: add r7,sp,#0x8
  000926ec: ldr.w r1,[r0,#0xa8]
  000926f0: vmov.f64 d16,-0x4020000000000000
  000926f4: add.w r1,r1,#0x4b0
  000926f8: vldr.64 d17,[r1]
  000926fc: vcmpe.f64 d17,d16
  00092700: vmrs apsr,fpscr
  00092704: bpl 0x0009270c
  00092706: ldr r1,[r0,#0x0]
  00092708: ldr r2,[r1,#0x30]
  0009270a: cbz r2,0x00092786
  0009270c: vmov.f64 d18,0x3fe0000000000000
  00092710: vcmpe.f64 d17,d18
  00092714: vmrs apsr,fpscr
  00092718: ble 0x0009274a
  0009271a: ldr r1,[r0,#0x0]
  0009271c: ldr r2,[r1,#0x30]
  0009271e: cmp r2,#0x3
  00092720: beq 0x000927a2
  00092722: cmp r2,#0x1
  00092724: bne 0x0009274a
  00092726: ldr r2,[0x000927cc]
  00092728: add r2,pc
  0009272a: ldr r4,[r2,#0x0]
  0009272c: ldr r2,[r0,#0x70]
  0009272e: ldr r0,[r0,#0x78]
  00092730: ldr r3,[r4,#0x0]
  00092732: subs r0,r2,r0
  00092734: add r0,r3
  00092736: str r0,[r4,#0x0]
  00092738: cmp r0,#0xfb
  0009273a: blt 0x00092784
  0009273c: mov r0,r1
  0009273e: movs r1,#0x0
  00092740: movs r5,#0x0
  00092742: blx 0x0007042c
  00092746: str r5,[r4,#0x0]
  00092748: pop {r4,r5,r7,pc}
  0009274a: vcmpe.f64 d17,d16
  0009274e: vmrs apsr,fpscr
  00092752: bpl 0x0009277a
  00092754: ldr r1,[r0,#0x0]
  00092756: ldr r2,[r1,#0x30]
  00092758: cmp r2,#0x2
  0009275a: bne 0x0009277a
  0009275c: ldr r2,[0x000927d4]
  0009275e: add r2,pc
  00092760: ldr r4,[r2,#0x0]
  00092762: ldr r2,[r0,#0x70]
  00092764: ldr r0,[r0,#0x78]
  00092766: ldr r3,[r4,#0x0]
  00092768: subs r0,r2,r0
  0009276a: add r0,r3
  0009276c: str r0,[r4,#0x0]
  0009276e: cmp r0,#0xfb
  00092770: it lt
  00092772: pop.lt {r4,r5,r7,pc}
  00092774: mov r0,r1
  00092776: movs r1,#0x3
  00092778: b 0x000927bc
  0009277a: ldr r0,[0x000927d8]
  0009277c: movs r1,#0x0
  0009277e: add r0,pc
  00092780: ldr r0,[r0,#0x0]
  00092782: str r1,[r0,#0x0]
  00092784: pop {r4,r5,r7,pc}
  00092786: ldr r2,[0x000927c8]
  00092788: add r2,pc
  0009278a: ldr r4,[r2,#0x0]
  0009278c: ldr r2,[r0,#0x70]
  0009278e: ldr r0,[r0,#0x78]
  00092790: ldr r3,[r4,#0x0]
  00092792: subs r0,r2,r0
  00092794: add r0,r3
  00092796: str r0,[r4,#0x0]
  00092798: cmp r0,#0xfb
  0009279a: blt 0x00092784
  0009279c: mov r0,r1
  0009279e: movs r1,#0x1
  000927a0: b 0x000927bc
  000927a2: ldr r2,[0x000927d0]
  000927a4: add r2,pc
  000927a6: ldr r4,[r2,#0x0]
  000927a8: ldr r2,[r0,#0x70]
  000927aa: ldr r0,[r0,#0x78]
  000927ac: ldr r3,[r4,#0x0]
  000927ae: subs r0,r2,r0
  000927b0: add r0,r3
  000927b2: str r0,[r4,#0x0]
  000927b4: cmp r0,#0xfb
  000927b6: blt 0x00092784
  000927b8: mov r0,r1
  000927ba: movs r1,#0x2
  000927bc: blx 0x0007042c
  000927c0: movs r0,#0x0
  000927c2: str r0,[r4,#0x0]
  000927c4: pop {r4,r5,r7,pc}
```
