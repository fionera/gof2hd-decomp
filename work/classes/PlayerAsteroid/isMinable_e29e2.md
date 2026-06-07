# PlayerAsteroid::isMinable
elf 0xe29e2 body 6
Sig: undefined __thiscall isMinable(PlayerAsteroid * this)

## decompile
```c

/* PlayerAsteroid::isMinable() */

PlayerAsteroid __thiscall PlayerAsteroid::isMinable(PlayerAsteroid *this)

{
  return this[0x13c];
}

```

## target disasm
```
  000f29e2: ldrb.w r0,[r0,#0x13c]
  000f29e6: bx lr
```
