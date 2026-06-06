# Level::setInitStreamOut
elf 0xadf84 body 12
Sig: undefined __stdcall setInitStreamOut(void)

## decompile
```c

/* Level::setInitStreamOut() */

void Level::setInitStreamOut(void)

{
  **(undefined1 **)(DAT_000bdf90 + 0xbdf8c) = 1;
  return;
}

```
## target disasm
```
  000bdf84: ldr r0,[0x000bdf90]
  000bdf86: movs r1,#0x1
  000bdf88: add r0,pc
  000bdf8a: ldr r0,[r0,#0x0]
  000bdf8c: strb r1,[r0,#0x0]
  000bdf8e: bx lr
```
