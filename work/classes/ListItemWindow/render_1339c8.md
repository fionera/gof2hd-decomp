# ListItemWindow::render
elf 0x1339c8 body 164
Sig: undefined __thiscall render(ListItemWindow * this)

## decompile
```c

/* ListItemWindow::render() */

void __thiscall ListItemWindow::render(ListItemWindow *this)

{
  int iVar1;
  Matrix *pMVar2;
  int iVar3;
  undefined4 extraout_r1;
  uint *puVar4;
  int iVar5;
  
  if (this[0x54] == (ListItemWindow)0x0) {
    return;
  }
  puVar4 = *(uint **)(DAT_00143a6c + 0x1439e0);
  AbyssEngine::PaintCanvas::Begin3d();
  iVar1 = **(int **)(DAT_00143a70 + 0x1439ec);
  iVar5 = *(int *)(iVar1 + 0x128);
  iVar3 = *(int *)(this + 0x20) + iVar5 * -2;
  AbyssEngine::PaintCanvas::EnableClip
            ((PaintCanvas *)*puVar4,
             *(int *)(this + 100) + iVar5 + (*(int *)(this + 0x6c) >> 1) + *(int *)(iVar1 + 0x2c),
             *(int *)(this + 0x68) + iVar5 + *(int *)(iVar1 + 0xc) + *(int *)(iVar1 + 0x20),
             ((*(int *)(this + 0x6c) >> 1) - (*(int *)(iVar1 + 0x2c) + iVar5 * 2)) -
             *(int *)(iVar1 + 0x28),iVar3);
  AbyssEngine::PaintCanvas::SetColor(*puVar4);
  pMVar2 = (Matrix *)AbyssEngine::PaintCanvas::CameraGetLocal(*puVar4);
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(this + 0x98),pMVar2);
  AEGeometry::render();
  AbyssEngine::PaintCanvas::End3d();
  (*(code *)(DAT_001ac434 + 0x1ac438))(*puVar4,extraout_r1,iVar3,&stack0xfffffff8);
  return;
}

```

## target disasm
```
  001439c8: push {r4,r5,r6,r7,lr}
  001439ca: add r7,sp,#0xc
  001439cc: push {r6,r7,r8,r9,r10}
  001439d0: mov r4,r0
  001439d2: ldrb.w r0,[r0,#0x54]
  001439d6: cmp r0,#0x0
  001439d8: beq 0x00143a66
  001439da: ldr r0,[0x00143a6c]
  001439dc: add r0,pc
  001439de: ldr r5,[r0,#0x0]
  001439e0: ldr r0,[r5,#0x0]
  001439e2: blx 0x00071da0
  001439e6: ldr r0,[0x00143a70]
  001439e8: add r0,pc
  001439ea: ldr r0,[r0,#0x0]
  001439ec: ldr r0,[r0,#0x0]
  001439ee: ldr.w r8,[r0,#0xc]
  001439f2: ldr.w lr,[r0,#0x20]
  001439f6: ldrd r12,r10,[r0,#0x28]
  001439fa: ldr.w r6,[r0,#0x128]
  001439fe: ldr r1,[r4,#0x20]
  00143a00: ldrd r2,r9,[r4,#0x64]
  00143a04: ldr r3,[r4,#0x6c]
  00143a06: sub.w r1,r1,r6, lsl #0x1
  00143a0a: ldr r0,[r5,#0x0]
  00143a0c: str r1,[sp,#0x0]
  00143a0e: adds r1,r2,r6
  00143a10: add.w r2,r9,r6
  00143a14: add.w r6,r10,r6, lsl #0x1
  00143a18: add.w r1,r1,r3, asr #0x1
  00143a1c: add r2,r8
  00143a1e: rsb r3,r6,r3, asr #0x1
  00143a22: add r1,r10
  00143a24: add r2,lr
  00143a26: sub.w r3,r3,r12
  00143a2a: blx 0x00075b38
  00143a2e: ldr r0,[r5,#0x0]
  00143a30: mov.w r1,#0xffffffff
  00143a34: blx 0x0006fac0
  00143a38: ldr.w r1,[r4,#0x94]
  00143a3c: ldr r0,[r5,#0x0]
  00143a3e: blx 0x0006ff1c
  00143a42: mov r1,r0
  00143a44: add.w r0,r4,#0x98
  00143a48: blx 0x0006f148
  00143a4c: ldr r0,[r4,#0x10]
  00143a4e: blx 0x00072238
  00143a52: ldr r0,[r5,#0x0]
  00143a54: blx 0x00071dac
  00143a58: ldr r0,[r5,#0x0]
  00143a5a: pop.w {r2,r3,r8,r9,r10}
  00143a5e: pop.w {r4,r5,r6,r7,lr}
  00143a62: b.w 0x001ac428
  00143a66: pop.w {r2,r3,r8,r9,r10}
  00143a6a: pop {r4,r5,r6,r7,pc}
```
