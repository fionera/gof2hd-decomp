# PaintCanvas::TransformRemoveChild
elf 0x7b34c body 62
Sig: undefined __thiscall TransformRemoveChild(PaintCanvas * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformRemoveChild(unsigned int, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::TransformRemoveChild(PaintCanvas *this,uint param_1,uint param_2)

{
  PaintCanvas *pPVar1;
  PaintCanvas *pPVar2;
  
  pPVar2 = (PaintCanvas *)param_1;
  pPVar1 = this;
  if (param_1 != param_2) {
    pPVar2 = *(PaintCanvas **)(this + 0x158);
    pPVar1 = pPVar2;
  }
  if ((param_2 <= pPVar2 && (param_1 != param_2 && pPVar1 != (PaintCanvas *)param_2)) &&
     (param_1 < pPVar1)) {
    ArrayRemove<AbyssEngine::Transform*>
              (*(Transform **)(*(int *)(this + 0x15c) + param_2 * 4),
               (Array *)(*(int *)(*(int *)(this + 0x15c) + param_1 * 4) + 0x4c));
    (*(code *)(DAT_001ab2b4 + 0x1ab2b8))(*(undefined4 *)(*(int *)(this + 0x15c) + param_1 * 4));
    return;
  }
  return;
}

```

## target disasm
```
  0008b34c: push {r4,r5,r7,lr}
  0008b34e: add r7,sp,#0x8
  0008b350: mov r5,r0
  0008b352: cmp r1,r2
  0008b354: itt ne
  0008b356: ldr.ne.w r0,[r5,#0x158]
  0008b35a: cmp.ne r0,r2
  0008b35c: mov r4,r1
  0008b35e: bhi 0x0008b362
  0008b360: pop {r4,r5,r7,pc}
  0008b362: cmp r0,r4
  0008b364: bls 0x0008b360
  0008b366: ldr.w r0,[r5,#0x15c]
  0008b36a: ldr.w r1,[r0,r4,lsl #0x2]
  0008b36e: ldr.w r0,[r0,r2,lsl #0x2]
  0008b372: adds r1,#0x4c
  0008b374: blx 0x0006fd48
  0008b378: ldr.w r0,[r5,#0x15c]
  0008b37c: ldr.w r0,[r0,r4,lsl #0x2]
  0008b380: pop.w {r4,r5,r7,lr}
  0008b384: b.w 0x001ab2a8
  001ab2a8: bx pc
```
