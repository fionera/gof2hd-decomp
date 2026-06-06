# Level::getAsteroids
elf 0xc40c2 body 6
Sig: undefined __thiscall getAsteroids(Level * this)

## decompile
```c

/* Level::getAsteroids() */

undefined4 __thiscall Level::getAsteroids(Level *this)

{
  return *(undefined4 *)(this + 0xfc);
}

```
## target disasm
```
  000d40c2: ldr.w r0,[r0,#0xfc]
  000d40c6: bx lr
```
