# DialogueWindow::isFirstPage
elf 0x1680ac body 10
Sig: undefined __thiscall isFirstPage(DialogueWindow * this)

## decompile
```c

/* DialogueWindow::isFirstPage() */

bool __thiscall DialogueWindow::isFirstPage(DialogueWindow *this)

{
  return this->field_48 == (int *)0x0;
}

```

## target disasm
```
  001780ac: ldr r0,[r0,#0x48]
  001780ae: clz r0,r0
  001780b2: lsrs r0,r0,#0x5
  001780b4: bx lr
```
