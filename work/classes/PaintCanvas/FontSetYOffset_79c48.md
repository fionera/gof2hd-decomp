# PaintCanvas::FontSetYOffset
elf 0x79c48 body 26
Sig: undefined __stdcall FontSetYOffset(uint param_1, short param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::FontSetYOffset(unsigned int, short) */

void AbyssEngine::PaintCanvas::FontSetYOffset(uint param_1,short param_2)

{
  if ((uint)(int)param_2 < *(uint *)(param_1 + 0x140)) {
    (*(code *)(DAT_001ab224 + 0x1ab228))(*(undefined4 *)(*(int *)(param_1 + 0x144) + param_2 * 4));
    return;
  }
  return;
}

```

## target disasm
```
  00089c48: ldr.w r3,[r0,#0x140]
  00089c4c: cmp r3,r1
  00089c4e: it ls
  00089c50: bx.ls lr
  00089c52: ldr.w r0,[r0,#0x144]
  00089c56: ldr.w r0,[r0,r1,lsl #0x2]
  00089c5a: mov r1,r2
  00089c5c: b.w 0x001ab218
  001ab218: bx pc
```
