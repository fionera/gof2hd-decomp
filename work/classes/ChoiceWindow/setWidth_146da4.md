# ChoiceWindow::setWidth
elf 0x146da4 body 28
Sig: undefined __thiscall setWidth(ChoiceWindow * this, int param_1)

## decompile
```c

/* ChoiceWindow::setWidth(int) */

void __thiscall ChoiceWindow::setWidth(ChoiceWindow *this,int param_1)

{
  int iVar1;
  
  iVar1 = DAT_00156dc0;
  *(int *)(this + 8) = param_1;
  *(int *)this = **(int **)(iVar1 + 0x156db0) / 2 - param_1 / 2;
  return;
}

```

## target disasm
```
  00156da4: ldr r2,[0x00156dc0]
  00156da6: str r1,[r0,#0x8]
  00156da8: add.w r1,r1,r1, lsr #0x1f
  00156dac: add r2,pc
  00156dae: ldr r2,[r2,#0x0]
  00156db0: ldr r2,[r2,#0x0]
  00156db2: add.w r2,r2,r2, lsr #0x1f
  00156db6: asrs r2,r2,#0x1
  00156db8: sub.w r1,r2,r1, asr #0x1
  00156dbc: str r1,[r0,#0x0]
  00156dbe: bx lr
```
