# SpaceLounge::draw3DShip
elf 0x1714d4 body 14
Sig: undefined __stdcall draw3DShip(void)

## decompile
```c

/* SpaceLounge::draw3DShip() */

void SpaceLounge::draw3DShip(void)

{
  int in_r0;
  
  if (*(char *)(in_r0 + 0x1c) == '\0') {
    return;
  }
  (*(code *)(DAT_001ac4c4 + 0x1ac4c8))(*(undefined4 *)(in_r0 + 0xc));
  return;
}

```

## target disasm
```
  001814d4: ldrb r1,[r0,#0x1c]
  001814d6: cbz r1,0x001814de
  001814d8: ldr r0,[r0,#0xc]
  001814da: b.w 0x001ac4b8
  001814de: bx lr
  001ac4b8: bx pc
```
