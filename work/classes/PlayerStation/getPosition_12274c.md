# PlayerStation::getPosition
elf 0x12274c body 14
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerStation::getPosition() */

void PlayerStation::getPosition(void)

{
  AEGeometry::getPosition();
  return;
}

```

## target disasm
```
  0013274c: push {r7,lr}
  0013274e: mov r7,sp
  00132750: ldr.w r1,[r1,#0x140]
  00132754: blx 0x000720b8
  00132758: pop {r7,pc}
```
