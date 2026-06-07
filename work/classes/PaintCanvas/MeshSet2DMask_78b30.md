# PaintCanvas::MeshSet2DMask
elf 0x78b30 body 48
Sig: undefined __stdcall MeshSet2DMask(uint param_1, int param_2, int param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSet2DMask(unsigned int, int, int) */

void AbyssEngine::PaintCanvas::MeshSet2DMask(uint param_1,int param_2,int param_3)

{
  PaintCanvas *this;
  
  if (*(uint *)(param_1 + 0x14c) <= (uint)param_2) {
    return;
  }
  this = *(PaintCanvas **)(param_1 + 0x150);
  if ((*(Image2D **)(this + param_2 * 4))[0x14] != (Image2D)0x0) {
    RestoreImage2D(this,*(Image2D **)(this + param_2 * 4));
    this = *(PaintCanvas **)(param_1 + 0x150);
  }
  *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(this + param_2 * 4);
  return;
}

```

## target disasm
```
  00088b30: push {r4,r5,r7,lr}
  00088b32: add r7,sp,#0x8
  00088b34: mov r4,r0
  00088b36: ldr.w r0,[r0,#0x14c]
  00088b3a: mov r5,r1
  00088b3c: cmp r0,r1
  00088b3e: it ls
  00088b40: pop.ls {r4,r5,r7,pc}
  00088b42: ldr.w r0,[r4,#0x150]
  00088b46: ldr.w r1,[r0,r5,lsl #0x2]
  00088b4a: ldrb r2,[r1,#0x14]
  00088b4c: cbz r2,0x00088b56
  00088b4e: blx 0x0006faf0
  00088b52: ldr.w r0,[r4,#0x150]
  00088b56: add.w r0,r0,r5, lsl #0x2
  00088b5a: ldr r0,[r0,#0x0]
  00088b5c: str r0,[r4,#0x20]
  00088b5e: pop {r4,r5,r7,pc}
```
