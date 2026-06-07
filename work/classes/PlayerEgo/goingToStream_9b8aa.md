# PlayerEgo::goingToStream
elf 0x9b8aa body 30
Sig: undefined __thiscall goingToStream(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::goingToStream() */

bool __thiscall PlayerEgo::goingToStream(PlayerEgo *this)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = this[9].field_18;
  iVar1 = Level::getLandmarks();
  return piVar2 == *(int **)(*(int *)(iVar1 + 4) + 4);
}

```

## target disasm
```
  000ab8aa: push {r4,r6,r7,lr}
  000ab8ac: add r7,sp,#0x8
  000ab8ae: ldr r1,[r0,#0xc]
  000ab8b0: ldr.w r4,[r0,#0x15c]
  000ab8b4: mov r0,r1
  000ab8b6: blx 0x000725b0
  000ab8ba: ldr r0,[r0,#0x4]
  000ab8bc: ldr r0,[r0,#0x4]
  000ab8be: subs r0,r4,r0
  000ab8c0: clz r0,r0
  000ab8c4: lsrs r0,r0,#0x5
  000ab8c6: pop {r4,r6,r7,pc}
```
