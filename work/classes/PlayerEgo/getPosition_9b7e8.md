# PlayerEgo::getPosition
elf 0x9b7e8 body 12
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerEgo::getPosition() */

void PlayerEgo::getPosition(void)

{
  AEGeometry::getPosition();
  return;
}

```

## target disasm
```
  000ab7e8: push {r7,lr}
  000ab7ea: mov r7,sp
  000ab7ec: ldr r1,[r1,#0x8]
  000ab7ee: blx 0x000720b8
  000ab7f2: pop {r7,pc}
```
