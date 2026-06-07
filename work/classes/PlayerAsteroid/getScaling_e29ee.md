# PlayerAsteroid::getScaling
elf 0xe29ee body 6
Sig: undefined __thiscall getScaling(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::getScaling() */

undefined4 __thiscall PlayerAsteroid::getScaling(PlayerAsteroid *this)

{
  return *(undefined4 *)(this + 0x138);
}

```

## target disasm
```
  000f29ee: ldr.w r0,[r0,#0x138]
  000f29f2: bx lr
```
