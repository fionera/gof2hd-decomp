# PaintCanvas::MaterialChange
elf 0x7c220 body 24
Sig: undefined __thiscall MaterialChange(PaintCanvas * this, uint param_1, BlendMode param_2, uint param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MaterialChange(unsigned int, AbyssEngine::BlendMode, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::MaterialChange
          (PaintCanvas *this,uint param_1,undefined4 param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  
  if (*(uint *)(this + 0x174) <= param_1) {
    return;
  }
  puVar1 = *(undefined4 **)(*(int *)(this + 0x178) + param_1 * 4);
  *puVar1 = param_4;
  puVar1[8] = param_3;
  return;
}

```

## target disasm
```
  0008c220: ldr.w r12,[r0,#0x174]
  0008c224: cmp r12,r1
  0008c226: it ls
  0008c228: bx.ls lr
  0008c22a: ldr.w r0,[r0,#0x178]
  0008c22e: ldr.w r0,[r0,r1,lsl #0x2]
  0008c232: str r3,[r0,#0x0]
  0008c234: str r2,[r0,#0x20]
  0008c236: bx lr
```
