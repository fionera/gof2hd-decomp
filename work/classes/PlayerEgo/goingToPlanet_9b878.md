# PlayerEgo::goingToPlanet
elf 0x9b878 body 44
Sig: undefined __thiscall goingToPlanet(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::goingToPlanet() */

bool __thiscall PlayerEgo::goingToPlanet(PlayerEgo *this)

{
  int iVar1;
  
  if (((*(char *)&this[9].field_14 != '\0') && (iVar1 = goingToStream(this), iVar1 == 0)) &&
     (iVar1 = goingToStation(this), iVar1 == 0)) {
    return (char)this[9].field_1C == '\0';
  }
  return false;
}

```

## target disasm
```
  000ab878: push {r4,r6,r7,lr}
  000ab87a: add r7,sp,#0x8
  000ab87c: mov r4,r0
  000ab87e: ldrb.w r0,[r0,#0x158]
  000ab882: cbz r0,0x000ab894
  000ab884: mov r0,r4
  000ab886: blx 0x00072598
  000ab88a: cbnz r0,0x000ab894
  000ab88c: mov r0,r4
  000ab88e: blx 0x000725a4
  000ab892: cbz r0,0x000ab898
  000ab894: movs r0,#0x0
  000ab896: pop {r4,r6,r7,pc}
  000ab898: ldrb.w r0,[r4,#0x160]
  000ab89c: clz r0,r0
  000ab8a0: lsrs r0,r0,#0x5
  000ab8a2: pop {r4,r6,r7,pc}
```
