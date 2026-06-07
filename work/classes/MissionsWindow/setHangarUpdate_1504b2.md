# MissionsWindow::setHangarUpdate
elf 0x1504b2 body 6
Sig: undefined __stdcall setHangarUpdate(bool param_1)

## decompile
```c

/* MissionsWindow::setHangarUpdate(bool) */

void MissionsWindow::setHangarUpdate(bool param_1)

{
  undefined1 in_r1;
  
  *(undefined1 *)(param_1 + 0x23) = in_r1;
  return;
}

```

## target disasm
```
  001604b2: strb.w r1,[r0,#0x23]
  001604b6: bx lr
```
