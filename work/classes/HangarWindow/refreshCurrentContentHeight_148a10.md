# HangarWindow::refreshCurrentContentHeight
elf 0x148a10 body 44
Sig: undefined __thiscall refreshCurrentContentHeight(HangarWindow * this)

## decompile
```c

/* HangarWindow::refreshCurrentContentHeight() */

void __thiscall HangarWindow::refreshCurrentContentHeight(HangarWindow *this)

{
  int *piVar1;
  
  piVar1 = (int *)HangarList::getCurrentTabItems(*(HangarList **)(this + 0x14));
  if (piVar1 != (int *)0x0) {
    *(int *)(this + 0xd4) =
         *(int *)(this + 0x10c) * (*piVar1 + -1) +
         *piVar1 * *(int *)(**(int **)(DAT_00158a3c + 0x158a26) + 0x70);
  }
  return;
}

```

## target disasm
```
  00158a10: push {r4,r6,r7,lr}
  00158a12: add r7,sp,#0x8
  00158a14: mov r4,r0
  00158a16: ldr r0,[r0,#0x14]
  00158a18: blx 0x00076f3c
  00158a1c: cbz r0,0x00158a3a
  00158a1e: ldr r1,[0x00158a3c]
  00158a20: ldr r0,[r0,#0x0]
  00158a22: add r1,pc
  00158a24: ldr.w r2,[r4,#0x10c]
  00158a28: ldr r1,[r1,#0x0]
  00158a2a: ldr r1,[r1,#0x0]
  00158a2c: ldr r1,[r1,#0x70]
  00158a2e: muls r1,r0
  00158a30: subs r0,#0x1
  00158a32: mla r0,r2,r0,r1
  00158a36: str.w r0,[r4,#0xd4]
  00158a3a: pop {r4,r6,r7,pc}
```
