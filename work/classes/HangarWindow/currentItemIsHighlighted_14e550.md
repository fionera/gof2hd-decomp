# HangarWindow::currentItemIsHighlighted
elf 0x14e550 body 32
Sig: undefined __thiscall currentItemIsHighlighted(HangarWindow * this)

## decompile
```c

/* HangarWindow::currentItemIsHighlighted() */

bool __thiscall HangarWindow::currentItemIsHighlighted(HangarWindow *this)

{
  bool bVar1;
  int iVar2;
  
  iVar2 = HangarList::getCurrentItem(*(HangarList **)(this + 0x14));
  if (iVar2 == 0) {
    bVar1 = false;
  }
  else {
    bVar1 = iVar2 == *(int *)(this + 0x68);
  }
  return bVar1;
}

```

## target disasm
```
  0015e550: push {r4,r6,r7,lr}
  0015e552: add r7,sp,#0x8
  0015e554: mov r4,r0
  0015e556: ldr r0,[r0,#0x14]
  0015e558: blx 0x000770bc
  0015e55c: cmp r0,#0x0
  0015e55e: iteee eq
  0015e560: mov.eq r0,#0x0
  0015e562: ldr.ne r1,[r4,#0x68]
  0015e564: sub.ne r0,r0,r1
  0015e566: clz.ne r0,r0
  0015e56a: it ne
  0015e56c: lsr.ne r0,r0,#0x5
  0015e56e: pop {r4,r6,r7,pc}
```
