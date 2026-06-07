# PaintCanvas::DrawTextLines
elf 0x7c78c body 22
Sig: undefined __stdcall DrawTextLines(uint param_1, Array * param_2, int param_3, int param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawTextLines(unsigned int, Array<AbyssEngine::String*>*, int, int) */

void AbyssEngine::PaintCanvas::DrawTextLines(uint param_1,Array *param_2,int param_3,int param_4)

{
  int in_stack_00000000;
  
  DrawTextLines((PaintCanvas *)param_1,(uint)param_2,(Array *)param_3,param_4,in_stack_00000000,
                false);
  return;
}

```

## target disasm
```
  0008c78c: push {r5,r6,r7,lr}
  0008c78e: add r7,sp,#0x8
  0008c790: ldr.w r12,[r7,#0x8]
  0008c794: mov.w lr,#0x0
  0008c798: strd r12,lr,[sp,#0x0]
  0008c79c: blx 0x0006fe2c
  0008c7a0: pop {r2,r3,r7,pc}
```
