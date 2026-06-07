# PaintCanvas::CameraSetCurrent
elf 0x7b7ec body 28
Sig: undefined __thiscall CameraSetCurrent(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::CameraSetCurrent(unsigned int) */

void __thiscall AbyssEngine::PaintCanvas::CameraSetCurrent(PaintCanvas *this,uint param_1)

{
  undefined4 *puVar1;
  
  *(uint *)(this + 0x170) = param_1;
  if (param_1 < *(uint *)(this + 0x164)) {
    puVar1 = *(undefined4 **)(*(int *)(this + 0x168) + param_1 * 4);
    (*(code *)(DAT_001ab2d4 + 0x1ab2d8))(this,*puVar1,puVar1[1],puVar1[2]);
    return;
  }
  return;
}

```

## target disasm
```
  0008b7ec: ldr.w r2,[r0,#0x164]
  0008b7f0: str.w r1,[r0,#0x170]
  0008b7f4: cmp r2,r1
  0008b7f6: it ls
  0008b7f8: bx.ls lr
  0008b7fa: ldr.w r2,[r0,#0x168]
  0008b7fe: ldr.w r3,[r2,r1,lsl #0x2]
  0008b802: ldmia r3,{r1,r2,r3}
  0008b804: b.w 0x001ab2c8
```
