# PlayerEgo::goingToStation
elf 0x9b8c8 body 46
Sig: undefined __thiscall goingToStation(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::goingToStation() */

bool __thiscall PlayerEgo::goingToStation(PlayerEgo *this)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = Level::getLandmarks();
  if (**(int **)(iVar1 + 4) != 0) {
    piVar2 = this[9].field_18;
    iVar1 = Level::getLandmarks();
    return piVar2 == (int *)**(undefined4 **)(iVar1 + 4);
  }
  return false;
}

```

## target disasm
```
  000ab8c8: push {r4,r6,r7,lr}
  000ab8ca: add r7,sp,#0x8
  000ab8cc: mov r4,r0
  000ab8ce: ldr r0,[r0,#0xc]
  000ab8d0: blx 0x000725b0
  000ab8d4: ldr r0,[r0,#0x4]
  000ab8d6: ldr r0,[r0,#0x0]
  000ab8d8: cbz r0,0x000ab8f2
  000ab8da: ldr r0,[r4,#0xc]
  000ab8dc: ldr.w r4,[r4,#0x15c]
  000ab8e0: blx 0x000725b0
  000ab8e4: ldr r0,[r0,#0x4]
  000ab8e6: ldr r0,[r0,#0x0]
  000ab8e8: subs r0,r4,r0
  000ab8ea: clz r0,r0
  000ab8ee: lsrs r0,r0,#0x5
  000ab8f0: pop {r4,r6,r7,pc}
  000ab8f2: movs r0,#0x0
  000ab8f4: pop {r4,r6,r7,pc}
```
