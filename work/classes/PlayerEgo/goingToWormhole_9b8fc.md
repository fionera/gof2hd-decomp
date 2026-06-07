# PlayerEgo::goingToWormhole
elf 0x9b8fc body 30
Sig: undefined __thiscall goingToWormhole(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::goingToWormhole() */

bool __thiscall PlayerEgo::goingToWormhole(PlayerEgo *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = this[9].field_18;
  iVar1 = Level::getLandmarks();
  return piVar2 == *(int **)(*(int *)(iVar1 + 4) + 0xc);
}

```

## target disasm
```
  000ab8fc: push {r4,r6,r7,lr}
  000ab8fe: add r7,sp,#0x8
  000ab900: ldr r1,[r0,#0xc]
  000ab902: ldr.w r4,[r0,#0x15c]
  000ab906: mov r0,r1
  000ab908: blx 0x000725b0
  000ab90c: ldr r0,[r0,#0x4]
  000ab90e: ldr r0,[r0,#0xc]
  000ab910: subs r0,r4,r0
  000ab912: clz r0,r0
  000ab916: lsrs r0,r0,#0x5
  000ab918: pop {r4,r6,r7,pc}
```
