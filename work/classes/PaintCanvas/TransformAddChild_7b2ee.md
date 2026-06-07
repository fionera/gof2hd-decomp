# PaintCanvas::TransformAddChild
elf 0x7b2ee body 94
Sig: undefined __thiscall TransformAddChild(PaintCanvas * this, uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformAddChild(unsigned int, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::TransformAddChild(PaintCanvas *this,uint param_1,uint param_2)

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
    ArrayAdd<AbyssEngine::Transform*>
              (*(Transform **)(*(int *)(this + 0x15c) + param_2 * 4),
               (Array *)(*(int *)(*(int *)(this + 0x15c) + param_1 * 4) + 0x4c));
    AEMath::BSphere::Merge
              ((BSphere *)(*(int *)(*(int *)(this + 0x15c) + param_1 * 4) + 0xd4),
               *(Transform **)(*(int *)(this + 0x15c) + param_2 * 4));
    (*(code *)(DAT_001ab2b4 + 0x1ab2b8))(*(undefined4 *)(*(int *)(this + 0x15c) + param_1 * 4));
    return;
  }
  return;
}

```

## target disasm
```
  0008b2ee: push {r4,r5,r6,r7,lr}
  0008b2f0: add r7,sp,#0xc
  0008b2f2: push.w r11
  0008b2f6: mov r6,r2
  0008b2f8: mov r5,r0
  0008b2fa: cmp r1,r2
  0008b2fc: mov r4,r1
  0008b2fe: itt ne
  0008b300: ldr.ne.w r0,[r5,#0x158]
  0008b304: cmp.ne r0,r6
  0008b306: bhi 0x0008b30e
  0008b308: pop.w r11
  0008b30c: pop {r4,r5,r6,r7,pc}
  0008b30e: cmp r0,r4
  0008b310: bls 0x0008b308
  0008b312: ldr.w r0,[r5,#0x15c]
  0008b316: ldr.w r1,[r0,r4,lsl #0x2]
  0008b31a: ldr.w r0,[r0,r6,lsl #0x2]
  0008b31e: adds r1,#0x4c
  0008b320: blx 0x0006f790
  0008b324: ldr.w r0,[r5,#0x15c]
  0008b328: ldr.w r2,[r0,r4,lsl #0x2]
  0008b32c: ldr.w r1,[r0,r6,lsl #0x2]
  0008b330: add.w r0,r2,#0xd4
  0008b334: blx 0x0006fd3c
  0008b338: ldr.w r0,[r5,#0x15c]
  0008b33c: ldr.w r0,[r0,r4,lsl #0x2]
  0008b340: pop.w r11
  0008b344: pop.w {r4,r5,r6,r7,lr}
  0008b348: b.w 0x001ab2a8
```
