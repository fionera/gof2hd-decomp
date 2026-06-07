# PlayerEgo::deleteHackingGame
elf 0xa1126 body 28
Sig: undefined __thiscall deleteHackingGame(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::deleteHackingGame() */

void __thiscall PlayerEgo::deleteHackingGame(PlayerEgo *this)

{
  void *pvVar1;
  
  if ((HackingGame *)this[0xd].field_14 != (HackingGame *)0x0) {
    pvVar1 = (void *)HackingGame::~HackingGame((HackingGame *)this[0xd].field_14);
    operator_delete(pvVar1);
  }
  this[0xd].field_14 = (int *)0x0;
  return;
}

```

## target disasm
```
  000b1126: push {r4,r6,r7,lr}
  000b1128: add r7,sp,#0x8
  000b112a: mov r4,r0
  000b112c: ldr.w r0,[r0,#0x1e8]
  000b1130: cbz r0,0x000b113a
  000b1132: blx 0x0007273c
  000b1136: blx 0x0006eb48
  000b113a: movs r0,#0x0
  000b113c: str.w r0,[r4,#0x1e8]
  000b1140: pop {r4,r6,r7,pc}
```
