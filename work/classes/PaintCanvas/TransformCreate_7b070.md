# PaintCanvas::TransformCreate
elf 0x7b070 body 50
Sig: undefined __thiscall TransformCreate(PaintCanvas * this, uint * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformCreate(unsigned int&) */

void __thiscall AbyssEngine::PaintCanvas::TransformCreate(PaintCanvas *this,uint *param_1)

{
  Transform *this_00;
  
  this_00 = operator_new(0x180);
  Transform::Transform(this_00);
  ArrayAdd<AbyssEngine::Transform*>(this_00,(Array *)(this + 0x158));
  *param_1 = *(int *)(this + 0x158) - 1;
  return;
}

```

## target disasm
```
  0008b070: push {r4,r5,r6,r7,lr}
  0008b072: add r7,sp,#0xc
  0008b074: push.w r11
  0008b078: mov r5,r0
  0008b07a: mov.w r0,#0x180
  0008b07e: mov r4,r1
  0008b080: blx 0x0006eb24
  0008b084: mov r6,r0
  0008b086: blx 0x0006eb84
  0008b08a: add.w r1,r5,#0x158
  0008b08e: mov r0,r6
  0008b090: blx 0x0006f790
  0008b094: ldr.w r0,[r5,#0x158]
  0008b098: subs r0,#0x1
  0008b09a: str r0,[r4,#0x0]
  0008b09c: pop.w r11
  0008b0a0: pop {r4,r5,r6,r7,pc}
```
