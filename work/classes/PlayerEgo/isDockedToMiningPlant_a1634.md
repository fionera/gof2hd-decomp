# PlayerEgo::isDockedToMiningPlant
elf 0xa1634 body 68
Sig: undefined __stdcall isDockedToMiningPlant(void)

## decompile
```c

/* PlayerEgo::isDockedToMiningPlant() */

bool PlayerEgo::isDockedToMiningPlant(void)

{
  int in_r0;
  int iVar1;
  Station *this;
  
  if ((*(char *)(in_r0 + 0x356) != '\0') && (*(int *)(in_r0 + 0x1c4) == 1)) {
    Status::getMission();
    iVar1 = Mission::isEmpty();
    if ((iVar1 != 0) && (iVar1 = Status::inAlienOrbit(), iVar1 == 0)) {
      this = (Station *)Status::getStation();
      iVar1 = Station::getIndex(this);
      return iVar1 == 0x67;
    }
  }
  return false;
}

```

## target disasm
```
  000b1634: push {r4,r6,r7,lr}
  000b1636: add r7,sp,#0x8
  000b1638: ldrb.w r1,[r0,#0x356]
  000b163c: cbz r1,0x000b1660
  000b163e: ldr.w r0,[r0,#0x1c4]
  000b1642: cmp r0,#0x1
  000b1644: bne 0x000b1660
  000b1646: ldr r0,[0x000b1678]
  000b1648: add r0,pc
  000b164a: ldr r4,[r0,#0x0]
  000b164c: ldr r0,[r4,#0x0]
  000b164e: blx 0x00072850
  000b1652: blx 0x00072868
  000b1656: cbz r0,0x000b1660
  000b1658: ldr r0,[r4,#0x0]
  000b165a: blx 0x000723d0
  000b165e: cbz r0,0x000b1664
  000b1660: movs r0,#0x0
  000b1662: pop {r4,r6,r7,pc}
  000b1664: ldr r0,[r4,#0x0]
  000b1666: blx 0x00071c14
  000b166a: blx 0x00071824
  000b166e: subs r0,#0x67
  000b1670: clz r0,r0
  000b1674: lsrs r0,r0,#0x5
  000b1676: pop {r4,r6,r7,pc}
```
