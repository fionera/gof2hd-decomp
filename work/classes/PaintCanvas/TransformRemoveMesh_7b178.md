# PaintCanvas::TransformRemoveMesh
elf 0x7b178 body 46
Sig: undefined __stdcall TransformRemoveMesh(uint param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformRemoveMesh(unsigned int, unsigned short) */

void AbyssEngine::PaintCanvas::TransformRemoveMesh(uint param_1,ushort param_2)

{
  int iVar1;
  ushort in_r2;
  
  if (*(uint *)(param_1 + 0x158) <= (uint)param_2) {
    return;
  }
  iVar1 = FindResource((PaintCanvas *)param_1,in_r2);
  if (iVar1 == 0) {
    return;
  }
  (*(code *)(DAT_001ab294 + 0x1ab298))(param_1,(uint)param_2,*(undefined4 *)(iVar1 + 8));
  return;
}

```

## target disasm
```
  0008b178: push {r4,r5,r7,lr}
  0008b17a: add r7,sp,#0x8
  0008b17c: mov r5,r0
  0008b17e: ldr.w r0,[r0,#0x158]
  0008b182: mov r4,r1
  0008b184: cmp r0,r1
  0008b186: it ls
  0008b188: pop.ls {r4,r5,r7,pc}
  0008b18a: mov r0,r5
  0008b18c: mov r1,r2
  0008b18e: blx 0x0006fb98
  0008b192: cbz r0,0x0008b1a2
  0008b194: ldr r2,[r0,#0x8]
  0008b196: mov r0,r5
  0008b198: mov r1,r4
  0008b19a: pop.w {r4,r5,r7,lr}
  0008b19e: b.w 0x001ab288
  0008b1a2: pop {r4,r5,r7,pc}
  001ab288: bx pc
```
