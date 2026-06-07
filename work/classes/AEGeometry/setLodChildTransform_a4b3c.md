# AEGeometry::setLodChildTransform
elf 0xa4b3c body 76
Sig: undefined __thiscall setLodChildTransform(AEGeometry * this, uint param_1)

## decompile
```c

/* AEGeometry::setLodChildTransform(unsigned int) */

void __thiscall AEGeometry::setLodChildTransform(AEGeometry *this,uint param_1)

{
  uint uVar1;
  void *pvVar2;
  int iVar3;
  uint uVar4;
  
  uVar4 = *(uint *)(this + 0x50);
  if (0 < (int)uVar4) {
    uVar1 = (uint)((ulonglong)uVar4 * 4);
    if ((int)((ulonglong)uVar4 * 4 >> 0x20) != 0) {
      uVar1 = 0xffffffff;
    }
    pvVar2 = operator_new__(uVar1);
    *(void **)(this + 0x58) = pvVar2;
    for (iVar3 = 0; iVar3 < (int)uVar4; iVar3 = iVar3 + 1) {
      AbyssEngine::PaintCanvas::TransformAddChild
                (*(PaintCanvas **)(this + 0x2c),*(uint *)(*(int *)(this + 0x54) + iVar3 * 4),param_1
                );
      uVar4 = *(uint *)(this + 0x50);
    }
  }
  return;
}

```

## target disasm
```
  000b4b3c: push {r4,r5,r6,r7,lr}
  000b4b3e: add r7,sp,#0xc
  000b4b40: push.w r8
  000b4b44: ldr r6,[r0,#0x50]
  000b4b46: mov r8,r1
  000b4b48: mov r5,r0
  000b4b4a: cmp r6,#0x1
  000b4b4c: blt 0x000b4b82
  000b4b4e: movs r0,#0x4
  000b4b50: umull r0,r1,r6,r0
  000b4b54: cmp r1,#0x0
  000b4b56: it ne
  000b4b58: mov.ne r1,#0x1
  000b4b5a: cmp r1,#0x0
  000b4b5c: it ne
  000b4b5e: mov.ne.w r0,#0xffffffff
  000b4b62: blx 0x0006ec08
  000b4b66: movs r4,#0x0
  000b4b68: str r0,[r5,#0x58]
  000b4b6a: b 0x000b4b7e
  000b4b6c: ldr r1,[r5,#0x54]
  000b4b6e: mov r2,r8
  000b4b70: ldr r0,[r5,#0x2c]
  000b4b72: ldr.w r1,[r1,r4,lsl #0x2]
  000b4b76: blx 0x000720d0
  000b4b7a: ldr r6,[r5,#0x50]
  000b4b7c: adds r4,#0x1
  000b4b7e: cmp r4,r6
  000b4b80: blt 0x000b4b6c
  000b4b82: pop.w r8
  000b4b86: pop {r4,r5,r6,r7,pc}
```
