# HangarWindow::highlightItem
elf 0x14bbec body 94
Sig: undefined __thiscall highlightItem(HangarWindow * this, ListItem * param_1)

## decompile
```c

/* HangarWindow::highlightItem(ListItem*) */

undefined4 __thiscall HangarWindow::highlightItem(HangarWindow *this,ListItem *param_1)

{
  byte bVar1;
  int iVar2;
  HangarWindow HVar3;
  float extraout_s0;
  
  if ((param_1 != (ListItem *)0x0) && (iVar2 = ::ListItem::isSelectable(param_1), iVar2 != 0)) {
    HVar3 = (HangarWindow)0x0;
    FModSound::play(**(int **)(DAT_0015bc4c + 0x15bc10),(Vector *)0x7c,(Vector *)0x0,extraout_s0);
    if (*(ListItem **)(this + 0x68) != param_1) {
      bVar1 = ::ListItem::isTextButton(param_1);
      HVar3 = (HangarWindow)(bVar1 ^ 1);
    }
    *(ListItem **)(this + 0x68) = param_1;
    this[0xd2] = HVar3;
    iVar2 = ::ListItem::isShip(param_1);
    if (iVar2 != 0) {
      Ship::adjustPrice(*(Ship **)(*(int *)(this + 0x68) + 0xc));
    }
  }
  return 0;
}

```

## target disasm
```
  0015bbec: push {r4,r5,r6,r7,lr}
  0015bbee: add r7,sp,#0xc
  0015bbf0: push.w r11
  0015bbf4: sub sp,#0x8
  0015bbf6: mov r5,r1
  0015bbf8: mov r4,r0
  0015bbfa: cbz r1,0x0015bc40
  0015bbfc: mov r0,r5
  0015bbfe: blx 0x00076ffc
  0015bc02: cbz r0,0x0015bc40
  0015bc04: ldr r0,[0x0015bc4c]
  0015bc06: movs r6,#0x0
  0015bc08: movs r1,#0x7c
  0015bc0a: movs r2,#0x0
  0015bc0c: add r0,pc
  0015bc0e: movs r3,#0x0
  0015bc10: str r6,[sp,#0x0]
  0015bc12: ldr r0,[r0,#0x0]
  0015bc14: ldr r0,[r0,#0x0]
  0015bc16: blx 0x00071548
  0015bc1a: ldr r0,[r4,#0x68]
  0015bc1c: cmp r0,r5
  0015bc1e: beq 0x0015bc2a
  0015bc20: mov r0,r5
  0015bc22: blx 0x00077008
  0015bc26: eor r6,r0,#0x1
  0015bc2a: mov r0,r5
  0015bc2c: str r5,[r4,#0x68]
  0015bc2e: strb.w r6,[r4,#0xd2]
  0015bc32: blx 0x00076204
  0015bc36: cbz r0,0x0015bc40
  0015bc38: ldr r0,[r4,#0x68]
  0015bc3a: ldr r0,[r0,#0xc]
  0015bc3c: blx 0x000719f8
  0015bc40: movs r0,#0x0
  0015bc42: add sp,#0x8
  0015bc44: pop.w r11
  0015bc48: pop {r4,r5,r6,r7,pc}
```
