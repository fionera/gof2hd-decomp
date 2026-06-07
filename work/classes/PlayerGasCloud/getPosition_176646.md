# PlayerGasCloud::getPosition
elf 0x176646 body 12
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerGasCloud::getPosition() */

void PlayerGasCloud::getPosition(void)

{
  AEGeometry::getPosition();
  return;
}

```

## target disasm
```
  00186646: push {r7,lr}
  00186648: mov r7,sp
  0018664a: ldr r1,[r1,#0x8]
  0018664c: blx 0x000720b8
  00186650: pop {r7,pc}
```
