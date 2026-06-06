# Level::getFriendRoute
elf 0xc40da body 6
Sig: undefined __thiscall getFriendRoute(Level * this)

## decompile
```c

/* Level::getFriendRoute() */

undefined4 __thiscall Level::getFriendRoute(Level *this)

{
  return *(undefined4 *)(this + 0x10c);
}

```
## target disasm
```
  000d40da: ldr.w r0,[r0,#0x10c]
  000d40de: bx lr
```
