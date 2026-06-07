# DialogueWindow::hasLevel
elf 0x1680b6 body 10
Sig: undefined __thiscall hasLevel(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::hasLevel() */

bool __thiscall DialogueWindow::hasLevel(DialogueWindow *this)

{
  return this->field_58 != (int *)0x0;
}

```

## target disasm
```
  001780b6: ldr r0,[r0,#0x58]
  001780b8: cmp r0,#0x0
  001780ba: it ne
  001780bc: mov.ne r0,#0x1
  001780be: bx lr
```
