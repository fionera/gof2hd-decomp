# PaintCanvas::FontGetYOffset
elf 0x79c60 body 26
Sig: undefined __thiscall FontGetYOffset(PaintCanvas * this, uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::FontGetYOffset(unsigned int) */

void __thiscall AbyssEngine::PaintCanvas::FontGetYOffset(PaintCanvas *this,uint param_1)

{
  if (*(uint *)(this + 0x140) <= param_1) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x001ab230. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ab234 + 0x1ab238))(*(undefined4 *)(*(int *)(this + 0x144) + param_1 * 4));
  return;
}

```

## target disasm
```
  00089c60: ldr.w r2,[r0,#0x140]
  00089c64: cmp r2,r1
  00089c66: itt ls
  00089c68: mov.ls r0,#0x0
  00089c6a: bx.ls lr
  00089c6c: ldr.w r0,[r0,#0x144]
  00089c70: ldr.w r0,[r0,r1,lsl #0x2]
  00089c74: b.w 0x001ab228
  001ab228: bx pc
```
