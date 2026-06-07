# PaintCanvas::FontSetSpacing
elf 0x79c18 body 26
Sig: undefined __thiscall FontSetSpacing(PaintCanvas * this, uint param_1, short param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::FontSetSpacing(unsigned int, short) */

void __thiscall
AbyssEngine::PaintCanvas::FontSetSpacing(PaintCanvas *this,uint param_1,short param_2)

{
  if (param_1 < *(uint *)(this + 0x140)) {
    (*(code *)(DAT_001ab204 + 0x1ab208))
              (*(undefined4 *)(*(int *)(this + 0x144) + param_1 * 4),(int)param_2);
    return;
  }
  return;
}

```

## target disasm
```
  00089c18: ldr.w r3,[r0,#0x140]
  00089c1c: cmp r3,r1
  00089c1e: it ls
  00089c20: bx.ls lr
  00089c22: ldr.w r0,[r0,#0x144]
  00089c26: ldr.w r0,[r0,r1,lsl #0x2]
  00089c2a: mov r1,r2
  00089c2c: b.w 0x001ab1f8
  001ab1f8: bx pc
```
