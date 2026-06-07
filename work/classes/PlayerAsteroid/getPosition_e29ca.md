# PlayerAsteroid::getPosition
elf 0xe29ca body 12
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerAsteroid::getPosition() */

void PlayerAsteroid::getPosition(void)

{
  AEGeometry::getPosition();
  return;
}

```

## target disasm
```
  000f29ca: push {r7,lr}
  000f29cc: mov r7,sp
  000f29ce: ldr r1,[r1,#0x8]
  000f29d0: blx 0x000720b8
  000f29d4: pop {r7,pc}
```
