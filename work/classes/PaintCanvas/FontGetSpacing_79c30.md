# PaintCanvas::FontGetSpacing
elf 0x79c30 body 26
Sig: undefined __stdcall FontGetSpacing(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::FontGetSpacing(unsigned int) */

void AbyssEngine::PaintCanvas::FontGetSpacing(uint param_1)

{
  uint in_r1;
  
  if (*(uint *)(param_1 + 0x140) <= in_r1) {
    return;
  }
  (*(code *)(DAT_001ab214 + 0x1ab218))(*(undefined4 *)(*(int *)(param_1 + 0x144) + in_r1 * 4));
  return;
}

```

## target disasm
```
  00089c30: ldr.w r2,[r0,#0x140]
  00089c34: cmp r2,r1
  00089c36: itt ls
  00089c38: mov.ls r0,#0x0
  00089c3a: bx.ls lr
  00089c3c: ldr.w r0,[r0,#0x144]
  00089c40: ldr.w r0,[r0,r1,lsl #0x2]
  00089c44: b.w 0x001ab208
  001ab208: bx pc
```
