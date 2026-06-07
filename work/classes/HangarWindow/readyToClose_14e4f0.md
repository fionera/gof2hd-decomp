# HangarWindow::readyToClose
elf 0x14e4f0 body 92
Sig: undefined __thiscall readyToClose(HangarWindow * this)

## decompile
```c

/* HangarWindow::readyToClose() */

bool __thiscall HangarWindow::readyToClose(HangarWindow *this)

{
  int iVar1;
  Station *this_00;
  int iVar2;
  
  iVar1 = HangarList::getCurrentTab(*(HangarList **)(this + 0x14));
  if ((((iVar1 == 4) && (this[0x88] != (HangarWindow)0x0)) && (0 < *(int *)(this + 0x94))) &&
     ((this[0x3c] == (HangarWindow)0x0 &&
      (iVar1 = BluePrint::isEmpty(*(BluePrint **)(this + 0x80)), iVar1 == 0)))) {
    iVar1 = BluePrint::getStationIndex(*(BluePrint **)(this + 0x80));
    this_00 = (Station *)Status::getStation();
    iVar2 = Station::getIndex(this_00);
    if (iVar1 != iVar2) {
      return false;
    }
  }
  return this[0x3c] == (HangarWindow)0x0;
}

```

## target disasm
```
  0015e4f0: push {r4,r5,r7,lr}
  0015e4f2: add r7,sp,#0x8
  0015e4f4: mov r4,r0
  0015e4f6: ldr r0,[r0,#0x14]
  0015e4f8: blx 0x00076f48
  0015e4fc: cmp r0,#0x4
  0015e4fe: bne 0x0015e53c
  0015e500: ldrb.w r0,[r4,#0x88]
  0015e504: cbz r0,0x0015e53c
  0015e506: ldr.w r0,[r4,#0x94]
  0015e50a: cmp r0,#0x1
  0015e50c: blt 0x0015e53c
  0015e50e: ldrb.w r0,[r4,#0x3c]
  0015e512: cbnz r0,0x0015e53c
  0015e514: ldr.w r0,[r4,#0x80]
  0015e518: blx 0x000738f4
  0015e51c: cbnz r0,0x0015e53c
  0015e51e: ldr.w r0,[r4,#0x80]
  0015e522: blx 0x00073624
  0015e526: mov r5,r0
  0015e528: ldr r0,[0x0015e54c]
  0015e52a: add r0,pc
  0015e52c: ldr r0,[r0,#0x0]
  0015e52e: ldr r0,[r0,#0x0]
  0015e530: blx 0x00071c14
  0015e534: blx 0x00071824
  0015e538: cmp r5,r0
  0015e53a: bne 0x0015e548
  0015e53c: ldrb.w r0,[r4,#0x3c]
  0015e540: clz r0,r0
  0015e544: lsrs r0,r0,#0x5
  0015e546: pop {r4,r5,r7,pc}
  0015e548: movs r0,#0x0
  0015e54a: pop {r4,r5,r7,pc}
```
