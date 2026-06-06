# Level::getFriendsLeft
elf 0xc4548 body 6
Sig: undefined __thiscall getFriendsLeft(Level * this)

## decompile
```c

/* Level::getFriendsLeft() */

undefined4 __thiscall Level::getFriendsLeft(Level *this)

{
  return *(undefined4 *)(this + 0x11c);
}

```
## target disasm
```
  000d4548: ldr.w r0,[r0,#0x11c]
  000d454c: bx lr
```
