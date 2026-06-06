# Level::getEnemyRoute
elf 0xc40d4 body 6
Sig: undefined __thiscall getEnemyRoute(Level * this)

## decompile
```c

/* Level::getEnemyRoute() */

undefined4 __thiscall Level::getEnemyRoute(Level *this)

{
  return *(undefined4 *)(this + 0x110);
}

```
## target disasm
```
  000d40d4: ldr.w r0,[r0,#0x110]
  000d40d8: bx lr
```
