# DialogueWindow::previousPage
elf 0x1680c4 body 26
Sig: undefined __thiscall previousPage(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::previousPage() */

undefined4 __thiscall DialogueWindow::previousPage(DialogueWindow *this)

{
  if (0 < (int)this->field_48) {
    this->field_48 = (int *)((int)this->field_48 + -1);
    loadContent(this);
    return 1;
  }
  return 0;
}

```

## target disasm
```
  001780c4: ldr r1,[r0,#0x48]
  001780c6: cmp r1,#0x1
  001780c8: itt lt
  001780ca: mov.lt r0,#0x0
  001780cc: bx.lt lr
  001780ce: push {r7,lr}
  001780d0: mov r7,sp
  001780d2: subs r1,#0x1
  001780d4: str r1,[r0,#0x48]
  001780d6: blx 0x00077bfc
  001780da: movs r0,#0x1
  001780dc: pop {r7,pc}
```
