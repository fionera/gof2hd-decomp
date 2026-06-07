# PaintCanvas::ReleaseSpriteSystemResource
elf 0x7c7f4 body 28
Sig: undefined __thiscall ReleaseSpriteSystemResource(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::ReleaseSpriteSystemResource(unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::ReleaseSpriteSystemResource(PaintCanvas *this,uint param_1)

{
  if (param_1 < *(uint *)(this + 0x180)) {
    (*(code *)(DAT_001ab364 + 0x1ab368))
              (*(undefined4 *)(this + 0x34),*(int *)(this + 0x184) + param_1 * 4);
    return;
  }
  return;
}

```

## target disasm
```
  0008c7f4: ldr.w r2,[r0,#0x180]
  0008c7f8: cmp r2,r1
  0008c7fa: it ls
  0008c7fc: bx.ls lr
  0008c7fe: ldr r2,[r0,#0x34]
  0008c800: ldr.w r0,[r0,#0x184]
  0008c804: add.w r1,r0,r1, lsl #0x2
  0008c808: mov r0,r2
  0008c80a: b.w 0x001ab358
  001ab358: bx pc
```
