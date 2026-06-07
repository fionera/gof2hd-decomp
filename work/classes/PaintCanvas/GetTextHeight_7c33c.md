# PaintCanvas::GetTextHeight
elf 0x7c33c body 26
Sig: undefined __stdcall GetTextHeight(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetTextHeight(unsigned int) */

void AbyssEngine::PaintCanvas::GetTextHeight(uint param_1)

{
  uint in_r1;
  
  if (*(uint *)(param_1 + 0x140) <= in_r1) {
    return;
  }
  (*(code *)(DAT_001ab354 + 0x1ab358))(*(undefined4 *)(*(int *)(param_1 + 0x144) + in_r1 * 4));
  return;
}

```

## target disasm
```
  0008c33c: ldr.w r2,[r0,#0x140]
  0008c340: cmp r2,r1
  0008c342: itt ls
  0008c344: mov.ls r0,#0x0
  0008c346: bx.ls lr
  0008c348: ldr.w r0,[r0,#0x144]
  0008c34c: ldr.w r0,[r0,r1,lsl #0x2]
  0008c350: b.w 0x001ab348
  001ab348: bx pc
```
