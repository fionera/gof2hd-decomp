# ListItemWindow::ListItemWindow
elf 0x131528 body 106
Sig: undefined __thiscall ListItemWindow(ListItemWindow * this)

## decompile
```c

/* ListItemWindow::ListItemWindow() */

ListItemWindow * __thiscall ListItemWindow::ListItemWindow(ListItemWindow *this)

{
  int iVar1;
  undefined4 *puVar2;
  uint *puVar3;
  
  AbyssEngine::String::String((String *)(this + 0x74));
  AbyssEngine::String::String((String *)(this + 0x80));
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0x98));
  AbyssEngine::AEMath::Matrix::Matrix((Matrix *)(this + 0xd4));
  puVar2 = (undefined4 *)(DAT_001415b0 + 0x141568);
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar3 = (uint *)*puVar2;
  *(undefined4 *)(this + 0x18) = 0;
  iVar1 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar3);
  *(int *)(this + 0x1c) = iVar1 / 2 + -1;
  *(undefined4 *)(this + 0x20) = 0;
  return this;
}

```

## target disasm
```
  00141528: push {r4,r5,r6,r7,lr}
  0014152a: add r7,sp,#0xc
  0014152c: push.w r8
  00141530: add.w r8,r0,#0x74
  00141534: mov r5,r0
  00141536: mov r0,r8
  00141538: blx 0x0006efbc
  0014153c: add.w r6,r5,#0x80
  00141540: mov r0,r6
  00141542: blx 0x0006efbc
  00141546: add.w r0,r5,#0x98
  0014154a: blx 0x0006f118
  0014154e: add.w r0,r5,#0xd4
  00141552: blx 0x0006f118
  00141556: ldr r0,[0x001415ac]
  00141558: vmov.i32 q8,#0x0
  0014155c: ldr r1,[0x001415b0]
  0014155e: movs r2,#0x18
  00141560: add r0,pc
  00141562: mov r3,r5
  00141564: add r1,pc
  00141566: vst1.32 {d16,d17},[r3],r2
  0014156a: movs r4,#0x0
  0014156c: ldr r0,[r0,#0x0]
  0014156e: ldr r2,[r1,#0x0]
  00141570: str r4,[r3,#0x0]
  00141572: ldr r1,[r0,#0x0]
  00141574: ldr r0,[r2,#0x0]
  00141576: blx 0x0006fe20
  0014157a: add.w r0,r0,r0, lsr #0x1f
  0014157e: mov.w r1,#0xffffffff
  00141582: add.w r0,r1,r0, asr #0x1
  00141586: strd r0,r4,[r5,#0x1c]
  0014158a: mov r0,r5
  0014158c: pop.w r8
  00141590: pop {r4,r5,r6,r7,pc}
```
