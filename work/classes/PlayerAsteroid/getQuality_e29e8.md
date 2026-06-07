# PlayerAsteroid::getQuality
elf 0xe29e8 body 6
Sig: undefined __thiscall getQuality(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::getQuality() */

undefined4 __thiscall PlayerAsteroid::getQuality(PlayerAsteroid *this)

{
  return *(undefined4 *)(this + 0x150);
}

```

## target disasm
```
  000f29e8: ldr.w r0,[r0,#0x150]
  000f29ec: bx lr
```
